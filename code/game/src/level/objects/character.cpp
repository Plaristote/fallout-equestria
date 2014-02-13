#include "level/objects/character.hpp"
#include "level/scene_camera.hpp"
#include <panda3d/nodePathCollection.h>

#include "level/level.hpp"
#include <options.hpp>
#include <dices.hpp>

#define DEFAULT_WEAPON_1 "hooves"
#define DEFAULT_WEAPON_2 "buck"

using namespace std;

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  Data     items      = _level->GetItems();  
  string   defEquiped[2];
  NodePath body_node  = object->nodePath.find("**/+Character");

  _running            = true;
  _rotating           = false;
  _fading_in          = _fading_off = false;
  _flags              = 0;
  _goToData.objective = 0;  
  _inventory          = new Inventory;
  _character          = dynamic_cast<Character*>(body_node.node());

  if (_character && _character->get_bundle(0))
  {
    PT(CharacterJointBundle) bodyBundle = _character->get_bundle(0);

    //HAIL MICROSOFT
    string listJoints[] = { "Horn", "Mouth", "BattleSaddle" };
    for (unsigned int i = 0; i<GET_ARRAY_SIZE(listJoints); i++)
    {
      for (unsigned short it = 0 ; it < 2 ; ++it)
      {
        stringstream       jointName;
        stringstream       npName;
        PT(CharacterJoint) joint;
        NodePath           tmp;

        //jointName << "attach_"  << listJoints[i] << "_" << (it + 1);
        //npName    << "equiped_" << listJoints[i] << "_" << (it + 1);
        jointName << "Horn"; // TODO Get models with the proper joints
        npName    << "equiped_" << listJoints[i] << "_" << (it + 1);
        joint     = _character->find_joint(jointName.str());

        if (joint)
        {
          tmp     = body_node.attach_new_node(npName.str());
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
    }
  }


  _type         = ObjectTypes::Character;
  _actionPoints = 0;

  SetCollideMaskOnSingleNodepath(_object->nodePath, ColMask::DynObject | ColMask::FovTarget);
  _object->nodePath.set_collide_mask(ColMask::DynObject);

  // Line of sight tools
  _losNode      = new_CollisionNode("losRay");
  _losNode->set_from_collide_mask(CollideMask(ColMask::FovBlocker | ColMask::FovTarget));
  _losNode->set_into_collide_mask(0);
  _losPath      = _window->get_render().attach_new_node(_losNode);
//  _losPath      = object->nodePath.attach_new_node(_losNode);
  _losRay       = new CollisionSegment();
  _losRay->set_point_a(0, 0, 0);
  _losRay->set_point_b(-10, 0, 0);
  _losPath.set_pos(0, 0, 0);
  //_losPath.show();
  _losNode->add_solid(_losRay);
  _losHandlerQueue = new CollisionHandlerQueue();
  _losTraverser.add_collider(_losPath, _losHandlerQueue);  

  // Fov tools
  _fovNeedsUpdate   = true;
  _fovTargetNode    = new_CollisionNode("fovTargetSphere");
  _fovTargetNode->set_from_collide_mask(CollideMask(0));
  _fovTargetNode->set_into_collide_mask(CollideMask(ColMask::FovTarget));
  _fovTargetSphere  = new CollisionSphere(0, 0, 0, 2.5f);
  _fovTargetNp      = _object->nodePath.attach_new_node(_fovTargetNode);
  _fovTargetNode->add_solid(_fovTargetSphere);
  //_fovTargetNp.show();
  
  _fovNode   = new_CollisionNode("fovSphere");
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
  _stats        = 0;
  std::string savepath = "saves";
  _statistics   = DataTree::Factory::JSON(savepath + "/stats-" + object->charsheet + ".json");
  if (_statistics == 0)
    _statistics = DataTree::Factory::JSON("data/charsheets/" + object->charsheet + ".json");
  if (_statistics)
  {
    Data stats  = GetStatistics();

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
  if (object->script == "")
    object->script = "general_pony";
  {
    string prefixPath  = "scripts/ai/";

    _script = new AngelScript::Object(prefixPath + object->script + ".as");
    _skill_target.Initialize(prefixPath + object->script + ".as", _script->GetContext());
    _script->asDefineMethod("main",                 "void   main(Character@, float)");
    _script->asDefineMethod("combat",               "void   combat(Character@)");
    _script->asDefineMethod("RequestStopFollowing", "void   RequestStopFollowing(Character@, Character@)");
    _script->asDefineMethod("RequestFollow",        "void   RequestFollow(Character@, Character@)");
    _script->asDefineMethod("RequestHeal",          "void   RequestHeal(Character@, Character@)");
    _script->asDefineMethod("AskMorale",            "int    AskMorale(Character@)");
    _script->asDefineMethod("SendMessage",          "void   ReceiveMessage(string)");
    _script->asDefineMethod("Load",                 "void   Load(Serializer@)");
    _script->asDefineMethod("Save",                 "void   Save(Serializer@)");
    _script->asDefineMethod("DefaultWeapon1",       "string default_weapon_1()");
    _script->asDefineMethod("DefaultWeapon2",       "string default_weapon_2()");

    const char* default_weapons[] = { "DefaultWeapon1", "DefaultWeapon2" };
    for (unsigned short i = 0 ; i < 2 ; ++i)
    {
      if (_script->IsDefined(default_weapons[i]))
        defEquiped[i] = *(string*)(_script->Call(default_weapons[i]));
    }
  }
  
  // Inventory
  _inventory->LoadInventory(_object);

  // Equiped Items
  defEquiped[0]    = DEFAULT_WEAPON_1;
  defEquiped[1]    = DEFAULT_WEAPON_2;
  active_object    = 0;
  active_object_it = 0;
  
  for (int i = 0 ; i < 2 ; ++i)
  {
    if (items[defEquiped[i]].Nil())
      continue ;
    _equiped[i].default_ = new InventoryObject(items[defEquiped[i]]);
    _equiped[i].equiped  = _equiped[i].default_;
    _equiped[i].equiped->SetEquiped(this, true);
    _inventory->AddObject(_equiped[i].equiped);
  }
  
  // Animations (HAIL MICROSOFT)
  string anims[] = { "idle", "walk", "run", "use" };
  for (unsigned int i = 0; i<GET_ARRAY_SIZE(anims); i++)
    LoadAnimation(anims[i]);

  // Others
  CharacterDied.Connect(*this, &ObjectCharacter::RunDeath);

  GetNodePath().set_transparency(TransparencyAttrib::M_alpha);
}

void ObjectCharacter::SetActionPoints(unsigned short ap)
{
  if (_level->GetState() == Level::Fight)
  {
    _actionPoints = ap;
    if (_statistics)
      ActionPointChanged.Emit(_actionPoints, Data(_statistics)["Statistics"]["Action Points"]);
  }
}

unsigned short ObjectCharacter::GetActionPoints(void) const
{
  if (_level->GetState() != Level::Fight)
    return (Data(_statistics)["Statistics"]["Action Points"]);
  return (_actionPoints);
}

void ObjectCharacter::SetInventory(Inventory* inventory)
{
  if (inventory)
  {
    if (_inventory) delete _inventory;
    _inventory = inventory;
    _obs_handler.Connect(_inventory->ContentChanged, *this, &ObjectCharacter::RefreshEquipment);
    _obs_handler.Connect(_inventory->EquipedItem,    [this](const string& target, unsigned short int slot, InventoryObject* object)
    {
      if (target == "equiped")
      {
        SetEquipedItem(slot, object, (EquipedMode)_inventory->GetEquipedMode(target, slot));
      }
    });
    _obs_handler.Connect(_inventory->UnequipedItem, [this](const string& target, unsigned short int slot, InventoryObject* object)
    {
      if (target == "unequiped")
      {
        UnequipItem(slot);
      }
    });
    _inventory->InitializeSlots();
    if (_statistics)
    {
      Data statistics(_statistics);

      inventory->SetCapacity(statistics["Statistics"]["Carry Weight"]);
    }
  }
}

void ObjectCharacter::SetFurtive(bool do_set)
{
  if (do_set)
  {
    GetNodePath().set_color(0, 0, 0, 0.5);
    AddFlag(1);
  }
  else
  {
    GetNodePath().set_color(0, 0, 0, 1);
    DelFlag(1);
  }
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

void ObjectCharacter::PlayEquipedItemAnimation(unsigned short it, const string& name, InstanceDynamicObject* target)
{
  if (_equiped[it].equiped)
  {
    InventoryObject& item           = *(_equiped[it].equiped);
    Data             animation      = item["actions"][_equiped[it].actionIt]["animations"];
    Data             playerAnim     = animation["player"][name];
    const string     playerAnimName = (playerAnim.Nil() ? ANIMATION_DEFAULT : playerAnim.Value());

    //
    // Character's animation
    //
    if (playerAnim.NotNil())
    {
      switch (_equiped[it].mode)
      {
        case EquipedBattleSaddle:
          PlayAnimation("saddle-" + playerAnimName);
        case EquipedMagic:
          PlayAnimation("magic-" + playerAnimName);
        case EquipedMouth:
          PlayAnimation("mouth-" + playerAnimName);
          break ;
      }
    }
    else
      PlayAnimation(playerAnimName);
  }
}

InventoryObject* ObjectCharacter::GetEquipedItem(unsigned short it)
{
  return (_equiped[it].equiped);
}

NodePath ObjectCharacter::GetEquipedItemNode(unsigned short it)
{
  if (_equiped[it].graphics)
    return (_equiped[it].graphics->GetNodePath());
  return (NodePath());
}

void ObjectCharacter::SetEquipedItem(unsigned short it, InventoryObject* item, EquipedMode mode)
{
  if (_equiped[it].equiped == item && _equiped[it].mode == mode)
  {
    cout << "Item " << item << " already equiped in this mode" << endl;
    return ;
  }
  cout << "SetEquipedItem called" << endl;
  // New system
  //_inventory->SetEquipedItem("equiped", it, item, mode);

  // Old system
  if (_equiped[it].graphics)
  {
    delete _equiped[it].graphics;
    _equiped[it].graphics = 0;
  }
  if (_equiped[it].equiped)
    _equiped[it].equiped->SetEquiped(this, false);
  _equiped[it].equiped  = item;
  _equiped[it].mode     = mode;
  _equiped[it].actionIt = 0;

  if (!item)
  {
    if (_equiped[it].default_ == 0)
      return ;
    item = _equiped[it].default_;
  }

  _equiped[it].graphics = item->CreateEquipedModel(_level->GetWorld());
  cout << "SetEquipedItem: Start" << endl;
  if (_equiped[it].graphics)
  {
    NodePath itemParentNode;

    cout << "SetEquipedItem: has graphics" << endl;
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
    cout << "SetEquipedItem: done" << endl;
  }
  
  item->SetEquiped(this, true);
  EquipedItemChanged.Emit(it, item);
  _inventory->ContentChanged.Emit();
}

void ObjectCharacter::RefreshEquipment(void)
{
  for (short i = 0 ; i < 2 ; ++i)
  {
    EquipedItemChanged.Emit      (i, _equiped[i].equiped);
    EquipedItemActionChanged.Emit(i, _equiped[i].equiped, _equiped[i].actionIt);
  }
}

void ObjectCharacter::UnequipItem(unsigned short it)
{
  _inventory->SetEquipedItem("equiped", it, 0);
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

void ObjectCharacter::Fading(void)
{
  LColor color = GetNodePath().get_color();

  if (_fading_off)
  {
    if (color.get_w() > 0)
      color.set_w(color.get_w() - 0.05);
    else
      _fading_off = false;
  }
  else if (_fading_in)
  {
    if (HasFlag(1))
    {
      if (color.get_w() < 1)
        color.set_w(color.get_w() + 0.05);
      else
        _fading_in = false;
    }
    else
    {
      if (color.get_w() < 0.5)
        color.set_w(color.get_w() + 0.05);
      else
        _fading_in = false;
    }
    if (_fading_in == false) // Quick hack for characters not fading back in completely
      color.set_w(255);
  }
  GetNodePath().set_color(color);
  if (color.get_w() == 0)
    GetNodePath().hide();
  else if (GetNodePath().is_hidden())
    GetNodePath().show();
}

void ObjectCharacter::RunEffects(float elapsedTime)
{
  if (_fading_in || _fading_off)
    Fading();
  if (_rotating && _rotation_goal != _object->nodePath.get_hpr().get_x())
    RunRotate(elapsedTime);
}

void ObjectCharacter::Run(float elapsedTime)
{
  PStatCollector collector_ai("Level:Characters:AI");
  Timer profile;
  
  _fovNeedsUpdate = true;
  if (!(IsInterrupted()))
  {
    Level::State state = _level->GetState();

    if (_fading_in || _fading_off)
      Fading();
    if (state == Level::Normal && _hitPoints > 0)
    {
      if (_script->IsDefined("main"))
      {
        collector_ai.start();
        AngelScript::Type<ObjectCharacter*> self(this);
        AngelScript::Type<float>            p_time(elapsedTime);
        
        _script->Call("main", 2, &self, &p_time);
        collector_ai.stop();
      }
    }
    else if (state == Level::Fight)
    {
      if (_hitPoints <= 0 || _actionPoints == 0)
	_level->NextTurn();
      else if (!(IsMoving()) && _script->IsDefined("combat")) // TODO replace with something more appropriate
      {
        collector_ai.start();
        AngelScript::Type<ObjectCharacter*> self(this);
        unsigned int                        ap_before = _actionPoints;

        _script->Call("combat", 1, &self);
        collector_ai.stop();
        if (ap_before == _actionPoints && !IsInterrupted() && !IsMoving()) // If stalled, skip turn
        {
          cout << "Character " << GetName() << " is stalling" << endl;
          _level->NextTurn();
        }
      }
    }
    if (_path.size() > 0)
      RunMovement(elapsedTime);
  }
  InstanceDynamicObject::Run(elapsedTime);
  //profile.Profile("Level:Characters:AI");
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
      
      cout << "BestWaypoint choices: " << list.size() << endl;
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
  cout << self->id << " versus " << wp->id << endl;
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

  ret = GetPathDistance(object->GetOccupiedWaypoint());
  return (ret);
}

list<Waypoint>     ObjectCharacter::GetPath(Waypoint* waypoint)
{
  list<Waypoint>   path;
  
  UnprocessCollisions();
  _level->FindPath(path, *_waypointOccupied, *waypoint);
  if (!(path.empty()))
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
  if (!(path.empty()))
    path.erase(path.begin());
  ProcessCollisions();
  return (path.size());
}

void                ObjectCharacter::GoTo(LPoint3f position)
{
  Waypoint* waypoint = _level->GetWorld()->waypoint_graph.GetClosest(position);
  
  if (waypoint)
    GoTo(waypoint);
}

void                ObjectCharacter::GoTo(unsigned int id)
{
  Waypoint*         wp = _level->GetWorld()->GetWaypointFromId(id);
  
  if (wp) GoTo(wp);
}

void                ObjectCharacter::DebugPathfinding(void)
{
  NodePath debug_root = _window->get_render().find("debug_pathfinding");
  NodePath debug      = debug_root.find(GetName());

  if (debug.is_empty())
    debug = debug_root.attach_new_node(GetName());
  for (unsigned short i = 0 ; i < debug.get_num_children() ; ++i)
    debug.get_child(i).remove_node();
  for_each(_path.begin(), _path.end(), [this, debug](Waypoint& wp)
  {
    NodePath np = debug.attach_new_node("debug_pathfinding_node");

    wp.nodePath.copy_to(np);
  });
}

void                ObjectCharacter::TeleportTo(unsigned int id)
{
  TeleportTo(_level->GetWorld()->GetWaypointFromId(id));
}

void                ObjectCharacter::TeleportTo(Waypoint* waypoint)
{
  if (waypoint)
  {
    LPoint3  wp_size  = NodePathSize(waypoint->nodePath);
    LPoint3f position = waypoint->nodePath.get_pos();
    float    z        = (position.get_z() - wp_size.get_z()) + 0.25;

    position.set_z(z + (_idle_size.get_z() / 2));
    GetNodePath().set_pos(position);
    SetOccupiedWaypoint(waypoint);
  }
}

void                ObjectCharacter::GoTo(Waypoint* waypoint)
{
  PStatCollector collector("Level:Characters:Pathfinding");
  Waypoint*      start_from = _waypointOccupied;

  collector.start();
  ReachedDestination.DisconnectAll();
  _goToData.objective = 0;
  UnprocessCollisions();
  if (_path.size() > 0)
  {
    //cout << "SETTING START FROM FOR SOME REASON" << endl;
    start_from = _level->GetWorld()->GetWaypointFromId(_path.front().id);
  }
  _path.clear();
  if (start_from && waypoint)
  {
    if (!(_level->FindPath(_path, *start_from, *waypoint)))
    {
      if (_level->GetPlayer() == this)
        _level->ConsoleWrite(i18n::T("No path."));
    }
    else
    {
      if (OptionsManager::Get()["debug"]["pathfinding"] == 1)
        DebugPathfinding();
      StartRunAnimation();
    }
  }
  else
    cout << "Character " << GetName() << " doesn't have a waypointOccupied" << endl;
  ProcessCollisions();
  collector.stop();
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

  object->UnprocessCollisions();
  GoTo(_goToData.nearest);
  object->ProcessCollisions();

  while (_goToData.min_distance && _path.size() > 1)
  {
    _path.erase(--(_path.end()));
    _goToData.min_distance--;
  }
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
      
      if (!(wplist.empty()))
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
  PlayAnimation(_running ? "run" : "walk", true);
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

  if (wp != _waypointOccupied && _level->GetState() == Level::Fight)
    SetActionPoints(_actionPoints - 1);
  
  // If the next waypoint is already occupied by someone else,
  // do not swap waypoints: instead, find another path.
  if (_waypointOccupied->id != wp->id && _level->IsWaypointOccupied(wp->id))
  {
    GoTo(_level->GetWorld()->GetWaypointFromId((*(--(_path.end()))).id));
    return ;
  }
  SetOccupiedWaypoint(wp);

  // Has reached object objective, if there is one ?
  if (_goToData.objective)
  {
    if (_path.size() <= (unsigned int)_goToData.max_distance && HasLineOfSight(_goToData.objective))
    {
      _path.clear();
      ReachedDestination.Emit(this);
      ReachedDestination.DisconnectAll();
      return ;
    }
  }

  _path.erase(_path.begin());  
  while (_path.size() > 0 && _path.front().id == _waypointOccupied->id)
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

    if (!pathAvailable)
    {
      //cout << "Path is not available" << endl;
      if (_goToData.objective)
	GoTo(_goToData.objective);
      else
      {
	Waypoint* dest = _level->GetWorld()->GetWaypointFromId((*(--(_path.end()))).id);
	GoTo(dest);
      }
      if (_path.size() == 0)
	ReachedDestination.DisconnectAll();
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
  Timer             profile;
  PStatCollector    collector("Level:Characters:Movement"); collector.start();
  Waypoint&         next      = *(_path.begin());
  LPoint3           pos       = _object->nodePath.get_pos();
  LPoint3           objective = next.nodePath.get_pos();
  float             max_speed;
  LPoint3           distance;
  float             max_distance;    
  LPoint3           speed, axis_speed, dest;
  int               dirX, dirY, dirZ;

  if (_level->GetState() == Level::Fight)
    max_speed = OptionsManager::Get()["combat-speed"];
  else
    max_speed = _running ? 20.f : 8.f;
  max_speed  *= elapsedTime;  
  
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
  collector.stop();
  //profile.Profile("Level:Characters:Movement");
}

void                ObjectCharacter::RunRotate(float elapsedTime)
{
  LVecBase3 rot    = _object->nodePath.get_hpr();
  float     factor = elapsedTime * 500;

  if ((ABS(_rotation_goal - rot.get_x())) < factor)
  {
    rot.set_x(_rotation_goal);
    _rotating = false;
  }
  else if (_rotation_goal > rot.get_x())
    rot.set_x(rot.get_x() + factor);
  else if (_rotation_goal < rot.get_x())
    rot.set_x(rot.get_x() - factor);
  _object->nodePath.set_hpr(rot);
}

void                ObjectCharacter::LookAt(LVecBase3 pos)
{
   LVecBase3 rot;
   float     backup;

   backup = _object->nodePath.get_hpr().get_x();
   _object->nodePath.look_at(pos);
   rot = _object->nodePath.get_hpr();
   rot.set_x(rot.get_x() - 180);
   rot.set_y(-rot.get_y());
   _rotation_goal = rot.get_x();
   rot.set_x(backup);
   _rotating      = true;
   _object->nodePath.set_hpr(rot);
}

void                ObjectCharacter::LookAt(InstanceDynamicObject* object)
{
  LVecBase3 pos = object->GetNodePath().get_pos();

  LookAt(pos);
}

bool                ObjectCharacter::HasLineOfSight_CheckModel(NodePath node)
{
  MapObject*                map_object    = _level->GetWorld()->GetMapObjectFromCollisionNode(node);
  
  if (map_object)
  {
    CollisionTraverser        model_traverser;
    PT(CollisionHandlerQueue) handler_queue = new CollisionHandlerQueue();

    model_traverser.add_collider(_losPath, handler_queue);
    model_traverser.traverse(map_object->render);
    if (handler_queue->get_num_entries() > 0)
      return (false);
  }
  return (true);
}

bool                ObjectCharacter::HasLineOfSight(InstanceDynamicObject* object)
{
  bool        ret   = true;

  if (object != this)
  {
    Timer     profiler;
    NodePath  root  = _object->nodePath;
    NodePath  other = object->GetNodePath();
    LVecBase3 rot   = root.get_hpr();
    LVector3  rpos  = root.get_pos();
    LVector3  dir   = root.get_relative_vector(other, other.get_pos() - rpos);

    _losPath.set_hpr(0, 0, 0);
    _losRay->set_point_a(rpos.get_x(), rpos.get_y(), rpos.get_z() + 4.f);
    _losRay->set_point_b(other.get_x(), other.get_y(), other.get_z() + 4.f);
    _losTraverser.traverse(_level->GetWorld()->floors_node);
    _losHandlerQueue->sort_entries();
    for (int i = 0 ; i < _losHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry = _losHandlerQueue->get_entry(i);
      NodePath        node  = entry->get_into_node_path();
      unsigned int    mask  = node.get_collide_mask().get_word();

      if (mask & ColMask::FovBlocker)
      {
        if (mask & ColMask::CheckCollisionOnModel && ((ret = HasLineOfSight_CheckModel(node)) == true))
          continue ;
        else
          ret = false;
      }
      else if (other.is_ancestor_of(node))
        ret = true;
      else
        continue ;
      break ;
    }
    profiler.Profile("HasLineOfSight");
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

void                ObjectCharacter::CallbackActionUse(InstanceDynamicObject* user)
{
  if (user == _level->GetPlayer())
    _level->PlayerLootWithScript(&(GetInventory()), this, _script->GetContext(), "scripts/ai/" + _object->script + ".as");
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

void     ObjectCharacter::SetEnemyDetected(ObjectCharacter* character)
{
  list<FovEnemy>::iterator enemyIt  = find(_fovEnemies.begin(), _fovEnemies.end(), character);

  cout << "[FOV] " << character->GetName() << " detected" << endl;
  if (enemyIt != _fovEnemies.end())
    enemyIt->ttl = FOV_TTL;
  else
    _fovEnemies.push_back(FovEnemy(character, FOV_TTL));
}

void     ObjectCharacter::CheckFieldOfView(void)
{
  if (_hitPoints <= 0 || !_fovNeedsUpdate)
    return ;
  Timer profile;
  cout << "Checking Field of View for " << GetName() << endl;
  PStatCollector       collector("Level:Characters:FieldOfView");
  short                perception          = (GetStatController() != 0) ? GetStatController()->Model().GetSpecial("PER") : 5;
  float                fovRadius           = 20 + (perception * 5);
  NodePath             self_node           = GetNodePath();
  Level::CharacterList detected_characters = _level->FindCharacters([this, fovRadius](ObjectCharacter* character) -> bool
  {
    return (character != this && character->GetDistance(this) < fovRadius);
  });
  
  collector.start();
  _fovNeedsUpdate = false;
  // Prepare all the FOV data
  {
    list<FovEnemy>::iterator it = _fovEnemies.begin();

    // Decrement TTL of nearby enemies
    while (it != _fovEnemies.end())
    {
      it->ttl--;
      cout << "[FOV] TTL left is '" << (int)it->ttl << '\'' << endl;
      if (it->ttl == 0)
	it = _fovEnemies.erase(it);
      else
	++it;
    }
    _fovAllies.clear();
  }

  for (unsigned int it = 0 ; it < detected_characters.size() ; ++it)
  {
    ObjectCharacter* character = detected_characters[it];

    if (character)
    {
      if (!(character->IsAlive()))
        continue ;
      if      (IsAlly(character))
        _fovAllies.push_back(character);
      else if (IsEnemy(character) && HasLineOfSight(character))
      {
        bool                     detected = true;

        if (character->HasFlag(1))
        {
          short sneak      = character->GetStatController()->Model().GetSkill("Sneak");
          short value      = sneak - (perception * 3);

          if (!(Dices::Throw(100) >= (value > 95 ? 95 : value)))
            detected = false;
        }
        if (detected)
          SetEnemyDetected(character);
        else
          cout << "[FOV] " << character->GetName() << " remains undetected" << endl;
      }
      else
        cout << "[FOV] " << character->GetName() << " is out of my line of sight" << endl;
    }
  }

  if (_fovEnemies.size() > 0 && _level->GetState() != Level::Fight)
    _level->StartFight(this);
  if (this == _level->GetPlayer())
    _level->RefreshCharactersVisibility();
  collector.stop();
  profile.Profile("/!\\ CheckFieldOfView");
}

/*
 * Script Communication
 */
void     ObjectCharacter::SendMessage(string& str)
{
  if (_script->IsDefined("SendMessage"))
  {
    AngelScript::Type<ObjectCharacter*> self(this);
    AngelScript::Type<std::string*>     message(&str);
    
    _script->Call("SendMessage", 2, &self, &message);
  }
}

int      ObjectCharacter::AskMorale(void)
{
  if (_script->IsDefined("AskMorale"))
  {
    AngelScript::Type<ObjectCharacter*> self(this);
    
    return (_script->Call("AskMorale", 1, &self));
  }
  return (0);
}

void     ObjectCharacter::RequestAttack(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, "RequestAttack");
}

void     ObjectCharacter::RequestHeal(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, "RequestHeal");
}

void     ObjectCharacter::RequestFollow(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, "RequestFollow");
}

void     ObjectCharacter::RequestStopFollowing(ObjectCharacter* f, ObjectCharacter* s)
{
  RequestCharacter(f, s, "RequestStopFollowing");
}

void     ObjectCharacter::RequestCharacter(ObjectCharacter* f, ObjectCharacter* s, const std::string& func)
{
  if (_script->IsDefined(func))
  {
    AngelScript::Type<ObjectCharacter*> self(f);
    AngelScript::Type<ObjectCharacter*> buddy(s);
    
    _script->Call(func, 2, &self, &buddy);
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
  cout << "SetAsEnemy" << endl;
  if (_faction && other->GetFaction() != 0)
  {
    WorldDiplomacy& diplomacy = GameTask::CurrentGameTask->GetDiplomacy();

    diplomacy.SetAsEnemy(enemy, _faction->flag, other->GetFaction());
  }
  else
  {
    cout << "Using self enemy mask: " << other->GetFactionName() << ':' << other->GetFaction() << endl;
    if (enemy)
      _self_enemyMask |= other->GetFaction();
    else if (_self_enemyMask & other->GetFaction())
      _self_enemyMask -= other->GetFaction();
    cout << "Self enemy mask: " << _self_enemyMask << endl;
  }
}

bool     ObjectCharacter::IsEnemy(const ObjectCharacter* other) const
{
  if (other->GetFaction() == 0 && _faction)
    return (other->IsEnemy(this));
  if (_faction)
    return ((_faction->enemyMask & other->GetFaction()) != 0);
  return ((_self_enemyMask & other->GetFaction()) != 0);
}

bool     ObjectCharacter::IsAlly(const ObjectCharacter* other) const
{
  return (_faction && _faction->flag == other->GetFaction());
}

