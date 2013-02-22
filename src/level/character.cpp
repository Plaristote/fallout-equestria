#include "level/character.hpp"
#include "level/scene_camera.hpp"
#include <panda3d/nodePathCollection.h>

#include "level/level.hpp"
#include <options.hpp>

#define DEFAULT_WEAPON_1 "hooves"
#define DEFAULT_WEAPON_2 "buck"

using namespace std;

CharacterBuff::CharacterBuff(Level* level, ObjectCharacter* character, Data buff)
  : _timeManager(level->GetTimeManager())
{
  Initialize(level, character, buff);
}

CharacterBuff::CharacterBuff(Level* l) : _timeManager(l->GetTimeManager()) {}

void CharacterBuff::Initialize(Level* level, ObjectCharacter* character, Data buff)
{
  Data dataGraphics = buff["graphics"];

  _buff.Duplicate(buff);
  _buff.Output();
  _character = character;
  _name      = buff.Key();
  _duration  = buff["duration"];
  _begin     = _end = 0;
  _module    = 0;
  _task      = 0;
  _context   = Script::Engine::Get()->CreateContext();
  if (_context)
  {
    Data scriptName = buff["script"]["source"];

    if (!(scriptName.Nil()))
    {
      _module  = Script::ModuleManager::Require(scriptName.Value(), "scripts/buffs/" + scriptName.Value());
      if (_module)
      {
	Data   scriptBegin = buff["script"]["hookBegin"];
	Data   scriptEnd   = buff["script"]["hookEnd"];
	string declBegin   = "bool " + scriptBegin.Value() + "(Character@, Character@)";
	string declEnd     = "void " + scriptEnd.Value()   + "(Character@)";

	_begin = _module->GetFunctionByDecl(declBegin.c_str());
	_end   = _module->GetFunctionByDecl(declEnd.c_str());
      }
    }
  }
  if (!(dataGraphics.Nil()))
  {
    WindowFramework* window = level->GetWorld()->window;

    _graphicalEffect = window->load_model(window->get_panda_framework()->get_models(), "models/" + dataGraphics["model"].Value());
    _graphicalEffect.set_name("graphical_effect");
    if (!(dataGraphics["scale"].Nil()))
      _graphicalEffect.set_scale((float)dataGraphics["scale"]);
    if (dataGraphics["color"].NotNil())
    {
      Data color = dataGraphics["color"];
      
      if (color["alpha"].NotNil())
      {
	_graphicalEffect.set_transparency(TransparencyAttrib::M_alpha);
	_graphicalEffect.set_color(color["red"], color["green"], color["blue"], color["alpha"]);
      }
      else
        _graphicalEffect.set_color(color["red"], color["green"], color["alpha"]);
    }
  }
}

void CharacterBuff::Begin(ObjectCharacter* from, TimeManager::Task* task)
{
  if (_begin)
  {
    if (!task)
    {
      _context->Prepare(_begin);
      _context->SetArgAddress(0, _character);
      _context->SetArgAddress(1, from);
      _context->Execute();
      _task = _timeManager.AddTask(TASK_LVL_CITY, false, _duration);
    }
    else
      _task = task;
    _task->Interval.Connect(*this, &CharacterBuff::End);

    if (_graphicalEffect.node())
      _graphicalEffect.reparent_to(_character->GetNodePath());
  }
}

void CharacterBuff::End(void)
{
  if (_end)
  {
    _context->Prepare(_end);
    _context->SetArgAddress(0, _character);
    _context->Execute();

    if (_graphicalEffect.node())
      _graphicalEffect.remove_node();
  }
  _character->DelBuff(this);
}

void ObjectCharacter::PushBuff(Data data, ObjectCharacter* caster)
{
  CharacterBuff* buff = new CharacterBuff(_level, this, data);

  buff->Begin(caster);
  _buffs.push_back(buff);
}

void ObjectCharacter::DelBuff(CharacterBuff* to_del)
{
  std::list<CharacterBuff*>::iterator it = find(_buffs.begin(), _buffs.end(), to_del);
  
  if (it != _buffs.end())
  {
    _buffs.erase(it);
    delete to_del;
  }
}

using namespace std;

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  Data   items = _level->GetItems();  
  string defEquiped[2];
  
  _goToData.objective = 0;  
  _inventory          = new Inventory;
  
  NodePath bodyNP = object->nodePath.find("**/+Character");
  _character      = dynamic_cast<Character*>(bodyNP.node());

  PT(CharacterJointBundle) bodyBundle = _character->get_bundle(0);

  //HAIL MICROSOFT
  string listJoints[] = { "Horn", "Mouth", "BattleSaddle" };
  for (unsigned int i = 0; i<GET_ARRAY_SIZE(listJoints); i++) {
    for (unsigned short it = 0 ; it < 2 ; ++it)
    {
      stringstream       jointName;
      stringstream       npName;
      PT(CharacterJoint) joint;
      NodePath           tmp;

      jointName << "attach_"  << listJoints[i] << "_" << (it + 1);
      npName    << "equiped_" << listJoints[i] << "_" << (it + 1);
      joint     = _character->find_joint(jointName.str());

      if (joint)
      {
        tmp     = bodyNP.attach_new_node(npName.str());
        bodyBundle->control_joint(jointName.str(), tmp.node());

        if (listJoints[i] == "Horn")
          _equiped[it].jointHorn         = tmp;
        else if (listJoints[i] == "Mouth")
          _equiped[it].jointMouth        = tmp;
        else
          _equiped[it].jointBattleSaddle = tmp;
      }
      else
	cout << "/!\\ Joint " << jointName.str() << " doesn't exist for Character " << _object->nodePath.get_name() << endl;
    }
  };

  _type         = ObjectTypes::Character;
  _actionPoints = 0;
  
  //_object->nodePath.set_collide_mask(CollideMask(ColMask::DynObject | ColMask::FovTarget), CollideMask::all_on(), _object->nodePath.get_class_type());
  _object->nodePath.set_collide_mask(CollideMask(ColMask::DynObject | ColMask::FovTarget), 0);

  // Line of sight tools
  _losNode      = new CollisionNode("losRay");
  _losNode->set_from_collide_mask(CollideMask(ColMask::Object | ColMask::FovTarget));
  _losNode->set_into_collide_mask(0);
  _losPath      = object->nodePath.attach_new_node(_losNode);
  _losRay       = new CollisionSegment();
  _losRay->set_point_a(0, 0, 0);
  _losRay->set_point_b(-10, 0, 0);
  _losPath.set_pos(0, 0, 0);
  //_losPath.show();
  _losNode->add_solid(_losRay);
  _losHandlerQueue = new CollisionHandlerQueue();
  _losTraverser.add_collider(_losPath, _losHandlerQueue);  

  // Fov tools
  _fovTargetNode    = new CollisionNode("fovTargetSphere");
  _fovTargetNode->set_from_collide_mask(CollideMask(0));
  _fovTargetNode->set_into_collide_mask(CollideMask(ColMask::FovTarget));
  _fovTargetSphere  = new CollisionSphere(0, 0, 0, 2.5f);
  _fovTargetNp      = _object->nodePath.attach_new_node(_fovTargetNode);
  _fovTargetNode->add_solid(_fovTargetSphere);
  //_fovTargetNp.show();
  
  _fovNode   = new CollisionNode("fovSphere");
  _fovNode->set_from_collide_mask(CollideMask(ColMask::FovTarget));
  _fovNode->set_into_collide_mask(CollideMask(0));
  _fovNp     = _object->nodePath.attach_new_node(_fovNode);
  _fovSphere = new CollisionSphere(0, 0, 0, 0);
  _fovHandlerQueue = new CollisionHandlerQueue();
  _fovNode->add_solid(_fovSphere);
  _fovTraverser.add_collider(_fovNp, _fovHandlerQueue);
  
  // Faction
  _faction        = 0;
  _self_enemyMask = 0;

  // Statistics
  _stats      = 0;
  _statistics = DataTree::Factory::JSON("data/charsheets/" + object->charsheet + ".json");
  if (_statistics)
  {
    Data stats = GetStatistics();
    
    _inventory->SetCapacity(stats["Statistics"]["Carry Weight"]);
    _armorClass = stats["Statistics"]["Armor Class"].Nil() ? 5  : (int)(stats["Statistics"]["Armor Class"]);
    _hitPoints  = stats["Statistics"]["Hit Points"].Nil()  ? 15 : (int)(stats["Statistics"]["Hit Points"]);
    _stats      = new StatController(stats);
    SetStatistics(_statistics, _stats);
  }
  else
  {
    _inventory->SetCapacity(275);
    _hitPoints  = 15;
    _armorClass = 5;
  }
  
  // Script
  _script_context = 0;
  _script_module  = 0;
  if (object->script != "")
  {
    string prefixName = "IA_";
    string prefixPath = "scripts/ai/";

    // Get the running functions
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptMain,  "void main(Character@, float)"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptFight, "void combat(Character@)"));

    // Get the communication functions
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptRequestStopFollowing, "void RequestStopFollowing(Character@, Character@)"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptRequestFollow,        "void RequestFollow(Character@, Character@)"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptRequestAttack,        "void RequestAttack(Character@, Character@)"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptRequestHeal,          "void RequestHeal(Character@, Character@)"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAskMorale,            "int  AskMorale()"));
    _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptSendMessage,          "void ReceiveMessage(string)"));
    LoadScript(prefixName + GetName(), prefixPath + object->script + ".as");

    // Get Default Weapons from script
    if (_script_module)
    {
      asIScriptFunction* funcGetDefWeapon[2];

      funcGetDefWeapon[0] = _script_module->GetFunctionByDecl("string default_weapon_1()");
      funcGetDefWeapon[1] = _script_module->GetFunctionByDecl("string default_weapon_2()");
      for (int i = 0 ; i < 2 ; ++i)
      {
        if (funcGetDefWeapon[i])
        {
          _script_context->Prepare(funcGetDefWeapon[i]);
          _script_context->Execute();
          defEquiped[i] = *(reinterpret_cast<string*>(_script_context->GetReturnAddress()));
        }
      }
    }
  }
  
  // Inventory
  _inventory->LoadInventory(_object);

  // Equiped Items
  defEquiped[0] = DEFAULT_WEAPON_1;
  defEquiped[1] = DEFAULT_WEAPON_2;
  
  for (int i = 0 ; i < 2 ; ++i)
  {
    if (items[defEquiped[i]].Nil())
      continue ;
    _equiped[i].default_ = new InventoryObject(items[defEquiped[i]]);
    _equiped[i].equiped  = _equiped[i].default_;
    _equiped[i].equiped->SetEquiped(true);
    _inventory->AddObject(_equiped[i].equiped);
  }
  
  // Animations (HAIL MICROSOFT)
  string anims[] = { "idle", "walk", "run", "use" };
  /*for_each(anims.begin(), anims.end(), [this](string anim)
  { LoadAnimation(anim); });*/
  for (unsigned int i = 0; i<GET_ARRAY_SIZE(anims); i++)
	  LoadAnimation(anims[i]);

  // Others
  CharacterDied.Connect(*this, &ObjectCharacter::RunDeath);  
}

ObjectCharacter::~ObjectCharacter()
{
  SetStatistics(0, 0);
  _losNode->remove_solid(0);
  _losPath.remove_node();
  _fovTargetNode->remove_solid(0);
  _fovTargetNp.remove_node();
  _fovNode->remove_solid(0);
  _fovNp.remove_node();
}

void ObjectCharacter::SetHitPoints(short hp)
{
  _hitPoints = hp;
  if (_stats)
    _stats->SetCurrentHp(hp);
  HitPointsChanged.Emit(_hitPoints);
  if (hp <= 0)
    CharacterDied.Emit();
}

void ObjectCharacter::StatHpUpdate(short hp)
{
  _hitPoints = hp;
  HitPointsChanged.Emit(_hitPoints);
  if (hp <= 0)
    CharacterDied.Emit();
}

void ObjectCharacter::NullifyStatistics(void)
{
  _stats      = 0;
  _statistics = 0;
}

void ObjectCharacter::SetStatistics(DataTree* statistics, StatController* controller)
{
  if (_stats      && _stats      != controller) delete _stats;
  if (_statistics && _statistics != statistics) delete _statistics;
  _statistics = statistics;
  _stats      = controller;
  if (_statistics)
  {
    Data data_stats(_statistics);

    ActionPointChanged.Emit(_actionPoints, data_stats["Statistics"]["Action Points"]);
    if (data_stats["Variables"]["Hit Points"].Nil())
      data_stats["Variables"]["Hit Points"] = data_stats["Statistics"]["Hit Points"].Value();
    SetHitPoints(data_stats["Variables"]["Hit Points"]);
    _obs_handler.Connect(_stats->HpChanged, *this, &ObjectCharacter::StatHpUpdate);
    cout << "Faction is named " << data_stats["Faction"].Value() << endl;
    if (data_stats["Faction"].NotNil())
      SetFaction(data_stats["Faction"].Value());
    else
      _faction = 0;
    GetDynamicObject()->nodePath.set_name(data_stats["Name"].Value());
  }
}

void ObjectCharacter::PlayEquipedItemAnimation(unsigned short it, const string& name)
{
  if (_equiped[it].equiped)
  {
    InventoryObject& item           = *(_equiped[it].equiped);
    Data             playerAnim     = item["actions"][_equiped[it].actionIt]["animations"]["player"][name];
    const string     playerAnimName = (playerAnim.Nil() ? ANIMATION_DEFAULT : playerAnim.Value());

    if (_equiped[it].graphics)
      _equiped[it].graphics->PlayAnimation(name);
    PlayAnimation(playerAnimName);
  }
}

InventoryObject* ObjectCharacter::GetEquipedItem(unsigned short it)
{
  return (_equiped[it].equiped);
}

void ObjectCharacter::SetEquipedItem(unsigned short it, InventoryObject* item, EquipedMode mode)
{
  if (_equiped[it].graphics)
    delete _equiped[it].graphics;
  _equiped[it].equiped->SetEquiped(false);
  _equiped[it].equiped  = item;
  _equiped[it].mode     = mode;
  _equiped[it].actionIt = 0;

  _equiped[it].graphics = item->CreateEquipedModel(_level->GetWorld());
  if (_equiped[it].graphics)
  {
    NodePath itemParentNode;

    switch (mode)
    {
      case EquipedMouth:
	itemParentNode = _equiped[it].jointMouth;
	break ;
      case EquipedMagic:
	itemParentNode = _equiped[it].jointHorn;
	break ;
      case EquipedBattleSaddle:
	itemParentNode = _equiped[it].jointBattleSaddle;
	break ;
    }
    _equiped[it].graphics->GetNodePath().reparent_to(itemParentNode);
  }
  
  item->SetEquiped(true);
  EquipedItemChanged.Emit(it, item);
  _inventory->ContentChanged.Emit();
}

void ObjectCharacter::UnequipItem(unsigned short it)
{
  SetEquipedItem(it, _equiped[it].default_);
}

void ObjectCharacter::ItemNextUseType(unsigned short it)
{
  if (_equiped[it].equiped)
  {
    Data             itemData   = *(_equiped[it].equiped);
    Data             actionData = itemData["actions"];
    unsigned char    action     = _equiped[it].actionIt;

    if (!(actionData.Nil()))
    {
      if (actionData.Count() <= (unsigned int)action + 1)
	_equiped[it].actionIt = 0;
      else
	_equiped[it].actionIt++;
      if (action != _equiped[it].actionIt)
	EquipedItemActionChanged.Emit(it, _equiped[it].equiped, _equiped[it].actionIt);
    }
  }
}

void ObjectCharacter::RestartActionPoints(void)
{
  _path.clear();
  if (_statistics)
  {
    Data stats(_statistics);

    _actionPoints = stats["Statistics"]["Action Points"];
  }
  else
    _actionPoints = 10;
  ActionPointChanged.Emit(_actionPoints, _actionPoints);
}

void ObjectCharacter::Run(float elapsedTime)
{
  if (!(IsInterrupted()))
  {
    Level::State state = _level->GetState();

    if (state == Level::Normal && _hitPoints > 0)
    {
      ReloadFunction(&_scriptMain);
      if (_scriptMain)
      {
        _script_context->Prepare(_scriptMain);
        _script_context->SetArgObject(0, this);
        _script_context->SetArgFloat(1, elapsedTime);
        _script_context->Execute();
      }
    }
    else if (state == Level::Fight)
    {
//       if (this != _level->GetPlayer())
//         cout << "Movement count: " << _path.size() << endl;
      ReloadFunction(&_scriptFight);
      if (_hitPoints <= 0 || _actionPoints == 0)
	_level->NextTurn();
      else if (!(IsMoving()) && _scriptFight) // replace with something more appropriate
      {
        ReloadFunction(&_scriptFight);
	_script_context->Prepare(_scriptFight);
	_script_context->SetArgObject(0, this);
	_script_context->Execute();
      }
    }
    if (_path.size() > 0)
      RunMovement(elapsedTime);
  }
  InstanceDynamicObject::Run(elapsedTime);
}

int                 ObjectCharacter::GetBestWaypoint(InstanceDynamicObject* object, bool farthest)
{
  Waypoint* self  = GetOccupiedWaypoint();
  Waypoint* other = object->GetOccupiedWaypoint();
  Waypoint* wp    = self;
  float     currentDistance;
  
  if (other)
  {
    currentDistance = wp->GetDistanceEstimate(*other);
    UnprocessCollisions();
    {
      list<Waypoint*> list = self->GetSuccessors(other);
      
      for_each(list.begin(), list.end(), [&wp, &currentDistance, other, farthest](Waypoint* waypoint)
      {
	float compDistance = waypoint->GetDistanceEstimate(*other);
	bool  comp         = (farthest ? currentDistance < compDistance : currentDistance > compDistance);

	if (comp)
	  wp = waypoint;
      });
    }
    ProcessCollisions();
  }
  return (wp->id);
}

int                 ObjectCharacter::GetFarthestWaypoint(InstanceDynamicObject* object)
{
  return (GetBestWaypoint(object, true));
}

int                 ObjectCharacter::GetNearestWaypoint(InstanceDynamicObject* object)
{
  return (GetBestWaypoint(object, false));
}

float               ObjectCharacter::GetDistance(InstanceDynamicObject* object)
{
    LPoint3 pos_1  = _object->nodePath.get_pos();
    LPoint3 pos_2  = object->GetDynamicObject()->nodePath.get_pos();
    float   dist_x = pos_1.get_x() - pos_2.get_x();
    float   dist_y = pos_1.get_y() - pos_2.get_y();

    return (SQRT(dist_x * dist_x + dist_y * dist_y));
}

unsigned short      ObjectCharacter::GetPathDistance(InstanceDynamicObject* object)
{
  unsigned short ret;
  
  object->UnprocessCollisions();
  ret = GetPathDistance(object->GetOccupiedWaypoint());
  object->ProcessCollisions();
  return (ret);
}

list<Waypoint>     ObjectCharacter::GetPath(Waypoint* waypoint)
{
  list<Waypoint>   path;
  
  UnprocessCollisions();
  _level->FindPath(path, *_waypointOccupied, *waypoint);
  if (path.size() > 0)
    path.erase(path.begin());
  if (path.size() > _actionPoints)
    path.resize(_actionPoints);
  ProcessCollisions();
  return (path);
}

unsigned short      ObjectCharacter::GetPathDistance(Waypoint* waypoint)
{
  list<Waypoint> path;
  
  UnprocessCollisions();
  _level->FindPath(path, *_waypointOccupied, *waypoint);
  if (path.size() > 0)
    path.erase(path.begin());
  ProcessCollisions();
  return (path.size());
}

void                ObjectCharacter::GoTo(unsigned int id)
{
  Waypoint*         wp = _level->GetWorld()->GetWaypointFromId(id);
  
  if (wp) GoTo(wp);
}

void                ObjectCharacter::GoTo(Waypoint* waypoint)
{
  ReachedDestination.DisconnectAll();
  _goToData.objective = 0;

  UnprocessCollisions();
  _path.clear();
  if (_waypointOccupied && waypoint)
  {
    if (!(_level->FindPath(_path, *_waypointOccupied, *waypoint)))
    {
      if (_level->GetPlayer() == this)
        _level->ConsoleWrite(i18n::T("No path."));
      cout << "No path" << endl;
    }
    else
      StartRunAnimation();
  }
  else
    cout << "Character doesn't have a waypointOccupied" << endl;
  ProcessCollisions();
  cout << _path.size() << endl;
}

void                ObjectCharacter::GoTo(InstanceDynamicObject* object, int max_distance)
{
  if (object == 0)
  {
    Script::Engine::ScriptError.Emit("Character::GoTo: NullPointer Error");
    return ;
  }
  ReachedDestination.DisconnectAll();
  _goToData              = object->GetGoToData(this);
  _goToData.max_distance = max_distance;

  UnprocessCollisions();
  object->UnprocessCollisions();

  _path.clear();
  if (_waypointOccupied && _goToData.nearest)
  {
    if (!(_level->FindPath(_path, *_waypointOccupied, *_goToData.nearest)))
    {
      if (_level->GetPlayer() == this)
        _level->ConsoleWrite("Can't reach.");
    }
    else
      StartRunAnimation();
    while (_goToData.min_distance && _path.size() > 1)
    {
      _path.erase(--(_path.end()));
      _goToData.min_distance--;
    }
  }

  ProcessCollisions();
  object->ProcessCollisions();
}

void                ObjectCharacter::GoToRandomWaypoint(void)
{
  if (_waypointOccupied)
  {
    _goToData.objective = 0;
    _path.clear();
    ReachedDestination.DisconnectAll();
    UnprocessCollisions();
    {
      list<Waypoint*>             wplist = _waypointOccupied->GetSuccessors(0);
      
      if (wplist.size() > 0)
      {
	int                       rit    = rand() % wplist.size();
	list<Waypoint*>::iterator it     = wplist.begin();

	for (it = wplist.begin() ; rit ; --rit, ++it);
	_path.push_back(**it);
	StartRunAnimation();
      }
    }
    ProcessCollisions();
  }
}

void                ObjectCharacter::TruncatePath(unsigned short max_size)
{
  if ((_path.size() > 0) && (*_path.begin()).id == (unsigned int)GetOccupiedWaypointAsInt())
    max_size++;
  if (_path.size() > max_size)
    _path.resize(max_size);
}

void                ObjectCharacter::StartRunAnimation(void)
{
  ReachedDestination.Connect(*this, &ObjectCharacter::StopRunAnimation);
  PlayAnimation("run", true);
}

void                ObjectCharacter::StopRunAnimation(InstanceDynamicObject*)
{
  if (_anim)
  {
    _animLoop = false;
    PlayIdleAnimation();
  }
}

void                ObjectCharacter::RunMovementNext(float elapsedTime)
{
  Waypoint* wp = _level->GetWorld()->GetWaypointFromId(_path.begin()->id);

  if (wp != _waypointOccupied)
    SetActionPoints(_actionPoints - 1);
  _waypointOccupied = wp;
  _level->GetWorld()->DynamicObjectSetWaypoint(*(GetDynamicObject()), *wp);

  // Has reached object objective, if there is one ?
  if (_goToData.objective)
  {
    if (_path.size() <= (unsigned int)_goToData.max_distance && HasLineOfSight(_goToData.objective))
    {
//      cout << "Reached destination" << endl;
      _path.clear();
      ReachedDestination.Emit(this);
      ReachedDestination.DisconnectAll();
      return ;
    }
  }

  _path.erase(_path.begin());  

  if (_path.size() > 0)
  {
    bool pathAvailable = true;
    // Check if the next waypoint is still accessible
    UnprocessCollisions();
    Waypoint::Arc* arc = _waypointOccupied->GetArcTo(_path.begin()->id);

    if (arc)
    {
      if (arc->observer)
      {
	if (arc->observer->CanGoThrough(0))
	  arc->observer->GoingThrough(this);
	else
	  pathAvailable = false;
      }
    }
    else
      pathAvailable = false;
    ProcessCollisions();
    // End check if the next waypoint is still accessible

    if (pathAvailable)
      RunMovement(elapsedTime);
    else
    {
      if (_goToData.objective)
	GoTo(_goToData.objective);
      else
      {
	Waypoint* dest = _level->GetWorld()->GetWaypointFromId((*(--(_path.end()))).id);
	GoTo(dest);
      }
      if (_path.size() == 0)
      {
	_level->ConsoleWrite("Path is obstructed");
	ReachedDestination.DisconnectAll();
      }
    }
  }
  else
  {
    ReachedDestination.Emit(this);
    ReachedDestination.DisconnectAll();
  }
}

LPoint3 NodePathSize(NodePath);

void                ObjectCharacter::RunMovement(float elapsedTime)
{
  Waypoint&         next = *(_path.begin());
  // TODO: Speed walking / running / combat
  float             combat_speed = OptionsManager::Get()["combat-speed"];
  float             max_speed = (_level->GetState() == Level::Fight && combat_speed > 0 ? combat_speed : 20.f) * elapsedTime;
  LPoint3           distance;
  float             max_distance;    
  LPoint3           speed, axis_speed, dest;
  LPoint3           pos = _object->nodePath.get_pos();
  int               dirX, dirY, dirZ;
  LPoint3           objective = next.nodePath.get_pos();

  //
  LPoint3 wp_size = NodePathSize(next.nodePath);

  float z = (objective.get_z() - wp_size.get_z()) + 0.25;
  objective.set_z(z + (_idle_size.get_z() / 2));
  //
  
  distance  = pos - objective;
  
  if (distance.get_x() == 0 && distance.get_y() == 0 && distance.get_z() == 0)
    RunMovementNext(elapsedTime);
  else
  {
    dirX = dirY = dirZ = 1;
    if (distance.get_x() < 0)
    {
      distance.set_x(-(distance.get_x()));
      dirX = -1;
    }
    if (distance.get_y() < 0)
    {
      distance.set_y(-(distance.get_y()));
      dirY = -1;
    }
    if (distance.get_z() < 0)
    {
      distance.set_z(-(distance.get_z()));
      dirZ = -1;
    }
    
    max_distance = (distance.get_x() > distance.get_y() ? distance.get_x() : distance.get_y());
    max_distance = (distance.get_z() > max_distance     ? distance.get_z() : max_distance);

    axis_speed.set_x(distance.get_x() / max_distance);
    axis_speed.set_y(distance.get_y() / max_distance);
    axis_speed.set_z(distance.get_z() / max_distance);
    if (max_speed > max_distance)
      max_speed = max_distance;
    speed.set_x(max_speed * axis_speed.get_x() * dirX);
    speed.set_y(max_speed * axis_speed.get_y() * dirY);
    speed.set_z(max_speed * axis_speed.get_z() * dirZ);

    dest = pos - speed;

    LookAt(dest);
    _object->nodePath.set_pos(dest);
  }
}

void                ObjectCharacter::LookAt(LVecBase3 pos)
{
   LVecBase3 rot;

   _object->nodePath.look_at(pos);
   rot = _object->nodePath.get_hpr();
   rot.set_x(rot.get_x() - 180);
   rot.set_y(-rot.get_y());
   _object->nodePath.set_hpr(rot);  
}

void                ObjectCharacter::LookAt(InstanceDynamicObject* object)
{
  LVecBase3 pos = object->GetNodePath().get_pos();

  LookAt(pos);
}


bool                ObjectCharacter::HasLineOfSight(InstanceDynamicObject* object)
{
  if (object == this)
    return (true);
  NodePath root  = _object->nodePath;
  NodePath other = object->GetNodePath();
  bool ret = true;

  LVecBase3 rot = root.get_hpr();
  LVector3  dir = root.get_relative_vector(other, other.get_pos() - root.get_pos());

  _losPath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
  _losRay->set_point_b(dir.get_x(), dir.get_y(), dir.get_z());
  _losTraverser.traverse(_level->GetWorld()->window->get_render());

  //_losPath.show();
  _losHandlerQueue->sort_entries();

  for (int i = 0 ; i < _losHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = _losHandlerQueue->get_entry(i);
    NodePath        node  = entry->get_into_node_path();

    if (root.is_ancestor_of(node))
      continue ;
    if (!(other.is_ancestor_of(node)))
      ret = false;
    break ;
  }
  return (ret);
}

void                ObjectCharacter::RunDeath()
{
  UnequipItem(0);
  UnequipItem(1);
  ResetInteractions();
  _interactions.push_back(Interaction("use", this, &ActionUse));

  GetNodePath().set_hpr(0, 0, 90);
  UnprocessCollisions();
  _hitPoints = 0;
}

void                ObjectCharacter::CallbackActionUse(InstanceDynamicObject*)
{
  _level->PlayerLoot(&(GetInventory()));
}

/*
 * Field of View
 */
# define FOV_TTL 5

Script::StdList<ObjectCharacter*>         ObjectCharacter::GetNearbyEnemies(void) const
{
  Script::StdList<ObjectCharacter*> ret;
  
  auto it  = _fovEnemies.begin();
  auto end = _fovEnemies.end();
  
  while (it != end)
  {
    //cout << "[" << it->enemy << "] FovEnemy: " << it->enemy->GetName() << endl;
    ret.push_back(it->enemy);
    it++;
  }
  return (ret);
}

Script::StdList<ObjectCharacter*> ObjectCharacter::GetNearbyAllies(void) const
{
  return (_fovAllies);
}

void     ObjectCharacter::CheckFieldOfView(void)
{
  if (_hitPoints <= 0 || _level->GetPlayer() == this)
    return ;
  CollisionTraverser fovTraverser;
  float              fovRadius = 45;
  
  // Prepare all the FOV data
  {
    list<FovEnemy>::iterator it = _fovEnemies.begin();

    while (it != _fovEnemies.end())
    {
      it->ttl--;
      if (it->ttl == 0)
	it = _fovEnemies.erase(it);
      else
	++it;
    }
    _fovAllies.clear();
    // Decrement TTL of nearby enemies
  }

  //Timer timer;
  
  _fovSphere->set_radius(fovRadius);
  _fovTraverser.traverse(_level->GetWorld()->window->get_render());

  //_fovNp.show();
    
  // Optimization of FOV
  //if (!(IsEnemy(_level->GetPlayer())) && _level->GetState() != Level::Fight)
  //  return ;
  
  for (unsigned short i = 0 ; i < _fovHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry*        entry  = _fovHandlerQueue->get_entry(i);
    NodePath               node   = entry->get_into_node_path();
    InstanceDynamicObject* object = _level->FindObjectFromNode(node);

    if (object && object != this)
    {
      ObjectCharacter* character = object->Get<ObjectCharacter>();

      if (character)
      {
	if (!(character->IsAlive()))
	  continue ;
	if      (IsAlly(character))
	  _fovAllies.push_back(character);
	else if (IsEnemy(character) && HasLineOfSight(character))
	{
	  list<FovEnemy>::iterator enemyIt = find(_fovEnemies.begin(), _fovEnemies.end(), character);

	  if (enemyIt != _fovEnemies.end())
	    enemyIt->ttl = FOV_TTL;
	  else
	    _fovEnemies.push_back(FovEnemy(character, FOV_TTL));
	}
      }
    }
  }
  if (_fovEnemies.size() > 0 && _level->GetState() != Level::Fight)
    _level->StartFight(this);
  //timer.Profile("Field of view");
}

/*
 * Script Communication
 */
void     ObjectCharacter::SendMessage(const string& str)
{
  if (_scriptSendMessage)
  {
    string cpy = str;

    _script_context->Prepare(_scriptSendMessage);
    _script_context->SetArgObject(0, &cpy);
    _script_context->Execute();
  }
}

int      ObjectCharacter::AskMorale(void)
{
  if (_scriptAskMorale)
  {
    _script_context->Prepare(_scriptAskMorale);
    _script_context->Execute();
    return (_script_context->GetReturnByte());
  }
  return (0);
}

void     ObjectCharacter::RequestAttack(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, _scriptRequestAttack);
}

void     ObjectCharacter::RequestHeal(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, _scriptRequestHeal);
}

void     ObjectCharacter::RequestFollow(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, _scriptRequestFollow);
}

void     ObjectCharacter::RequestStopFollowing(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, _scriptRequestStopFollowing);
}

void     ObjectCharacter::RequestCharacter(ObjectCharacter* f, ObjectCharacter* s, asIScriptFunction* func)
{
  if (func)
  {
    _script_context->Prepare(func);
    _script_context->SetArgObject(0, f);
    _script_context->SetArgObject(1, s);
    _script_context->Execute();
  }
}

/*
 * Diplomacy
 */
#include "gametask.hpp"
void     ObjectCharacter::SetFaction(const std::string& name)
{
  WorldDiplomacy& diplomacy = GameTask::CurrentGameTask->GetDiplomacy();

  _faction = diplomacy.GetFaction(name);
  cout << "Faction pointer for " << name << " is " << _faction << endl;
}

void     ObjectCharacter::SetFaction(unsigned int flag)
{
  WorldDiplomacy& diplomacy = GameTask::CurrentGameTask->GetDiplomacy();

  _faction = diplomacy.GetFaction(flag);
}

void     ObjectCharacter::SetAsEnemy(const ObjectCharacter* other, bool enemy)
{
  if (_faction && other->GetFaction() != 0)
  {
    WorldDiplomacy& diplomacy = GameTask::CurrentGameTask->GetDiplomacy();

    diplomacy.SetAsEnemy(enemy, _faction->flag, other->GetFaction());
  }
  else
  {
    if (enemy)
      _self_enemyMask &= other->GetFaction();
    else if (_self_enemyMask & other->GetFaction());
      _self_enemyMask -= other->GetFaction();
  }
}

bool     ObjectCharacter::IsEnemy(const ObjectCharacter* other) const
{
  //cout << "Calling IsEnemy" << endl;
  if (other->GetFaction() == 0 && _faction)
    return (other->IsEnemy(this));
  if (_faction)
  {
    //cout << "I haz faction: " << _faction->enemyMask << " (" << _faction->flag << ")" << endl;
    return (_faction->enemyMask & other->GetFaction());
  }
  //cout << "I haz no faction" << endl;
  return (_self_enemyMask & other->GetFaction());
}

bool     ObjectCharacter::IsAlly(const ObjectCharacter* other) const
{
  return (_faction && _faction->flag == other->GetFaction());
}

