#include "level/objects/character.hpp"
#include <panda3d/nodePathCollection.h>
#include "circular_value_set.hpp"

#include "level/level.hpp"
#include <options.hpp>
#include <dices.hpp>
#include <iterator>
#include "gametask.hpp"

#define DEFAULT_WEAPON_1 "hooves"
#define DEFAULT_WEAPON_2 "buck"

using namespace std;

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) :
  CharacterVisibility(level, object),
  line_of_sight(*level->GetWorld(), _window->get_render(), GetNodePath()),
  field_of_view(*level, *this)
{
  Data     items      = GameTask::CurrentGameTask->GetItemIndex(); 
  string   defEquiped[2];
  NodePath body_node  = object->nodePath.find("**/+Character");

  _flags              = 0;
  _inventory          = new Inventory;
  _character          = dynamic_cast<Character*>(body_node.node());
  current_action      = 0;

  SetMovementAnimation("run");

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

  SetCollideMaskOnSingleNodepath(_object->nodePath, ColMask::DynObject | ColMask::FovTarget);
  _object->nodePath.set_collide_mask(ColMask::DynObject);

  // Faction
  _faction        = 0;
  _self_enemyMask = 0;
  RefreshStatistics();
  
  // Script
  if (object->script == "")
    object->script = "general_pony";

  {
    string prefixPath  = "scripts/ai/";

    script = new AngelScript::Object(prefixPath + object->script + ".as");
    skill_target.Initialize(prefixPath + object->script + ".as", script->GetContext());
    SetupScript(script);
  }
  
  // Inventory
  _inventory->LoadInventory(_object);

  // Equiped Items
  defEquiped[0]    = DEFAULT_WEAPON_1;
  defEquiped[1]    = DEFAULT_WEAPON_2;
  active_object    = 0;
  active_object_it = 0;

  const char* default_weapons[] = { "DefaultWeapon1", "DefaultWeapon2" };
  for (unsigned short i = 0 ; i < 2 ; ++i)
  {
    if (script->IsDefined(default_weapons[i]))
      defEquiped[i] = *(string*)(script->Call(default_weapons[i]));
  }

  for (int i = 0 ; i < 2 ; ++i)
  {
    if (items[defEquiped[i]].Nil())
      continue ;
    _equiped[i].default_ = new InventoryObject(items[defEquiped[i]]);
    _equiped[i].equiped  = _equiped[i].default_;
    _equiped[i].equiped->SetEquiped(this, true);
    _equiped[i].mode     = EquipedMouth;
    _inventory->AddObject(_equiped[i].equiped);
  }
  
  // Animations (HAIL MICROSOFT)
  string anims[] = { "idle", "walk", "run", "use" };
  for (unsigned int i = 0; i<GET_ARRAY_SIZE(anims); i++)
    LoadAnimation(anims[i]);

  // FLOORS
  ChangedFloor.Connect([this](unsigned char floor)
  {
    World* world = _level->GetWorld();

    GetNodePath().reparent_to(world->floors[floor]);
  });
  if (HasOccupiedWaypoint())
    ChangedFloor.Emit(GetOccupiedWaypoint()->floor);
}

void ObjectCharacter::SetupScript(AngelScript::Object* script)
{
  script->asDefineMethod("main",                 "void   main(Character@, float)");
  script->asDefineMethod("combat",               "void   combat(Character@)");
  script->asDefineMethod("RequestStopFollowing", "void   RequestStopFollowing(Character@, Character@)");
  script->asDefineMethod("RequestFollow",        "void   RequestFollow(Character@, Character@)");
  script->asDefineMethod("RequestHeal",          "void   RequestHeal(Character@, Character@)");
  script->asDefineMethod("AskMorale",            "int    AskMorale(Character@)");
  script->asDefineMethod("SendMessage",          "void   ReceiveMessage(string)");
  script->asDefineMethod("Load",                 "void   Load(Serializer@)");
  script->asDefineMethod("Save",                 "void   Save(Serializer@)");
  script->asDefineMethod("DefaultWeapon1",       "string default_weapon_1()");
  script->asDefineMethod("DefaultWeapon2",       "string default_weapon_2()");
  Target::SetupScript(script);
}

void ObjectCharacter::ActionTalkTo(ObjectCharacter* user)
{
  bool open_dialog = _object->dialog != "";

  if (script && script->IsDefined("TalkTo"))
  {
    AngelScript::Type<ObjectCharacter*> param(user);
    
    open_dialog = open_dialog && (bool)(script->Call("TalkTo", 1, &param));
  }
  if (open_dialog)
  {
    DialogController* controller = _level->GetLevelUi().OpenUiDialog(this);
    
    controller->StartBarter.Connect([this, user, controller](ObjectCharacter*)
    {
      bool can_barter = true;
      
      if (script && script->IsDefined("CanBarter"))
        can_barter = script->Call("CanBarter");
      if (can_barter)
        _level->GetLevelUi().OpenUiBarter(user, this);
      else
        controller->SetCurrentNpcText(i18n::T("[This individual does not want to barter with you.]"));
    });
  }
}

void ObjectCharacter::ActionUse(InstanceDynamicObject* user)
{
  if (user == _level->GetPlayer())
  {
    if (IsAlive())
      _level->GetInteractions().ActionLootWithScript(&(GetInventory()), this, script->GetContext(), "scripts/ai/" + _object->script + ".as");
    else
      _level->GetInteractions().ActionLoot(&(GetInventory()));
  }
}

void ObjectCharacter::SetInventory(Inventory* inventory)
{
  if (inventory)
  {
    Data statistics = GetStatistics();

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
    _inventory->SetCapacity(statistics["Statistics"]["Carry Weight"]);
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
  if (current_action)
    delete current_action;
}

void ObjectCharacter::RefreshStatistics(void)
{
  Data statistics = GetStatistics();
  
  CharacterStatistics::RefreshStatistics();
  _inventory->SetCapacity(statistics["Statistics"]["Carry Weight"]);
  field_of_view.SetIntervalDurationFromStatistics();
  if (statistics["Faction"].NotNil())
    SetFaction(statistics["Faction"].Value());
  else
    _faction = 0;

  GetStatController()->Died.Connect(*this, &ObjectCharacter::RunDeath);
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

void ObjectCharacter::Run(float elapsedTime)
{
  //Timer profile;
  Level::State state = _level->GetState();

  CharacterVisibility::Run(elapsedTime);
  field_of_view.MarkForUpdate();
  if (!(IsInterrupted()))
  {
    if (state == Level::Normal && GetHitPoints() > 0 && script->IsDefined("main"))
      RunRegularBehaviour(elapsedTime);
    else if (state == Level::Fight)
      RunCombatBehaviour(elapsedTime);
  }
  //profile.Profile("Level:Characters:AI");
}

void ObjectCharacter::RunRegularBehaviour(float elapsedTime)
{
  PStatCollector                      collector_ai("Level:Characters:AI");
  AngelScript::Type<ObjectCharacter*> self(this);
  AngelScript::Type<float>            p_time(elapsedTime);

  collector_ai.start();
  script->Call("main", 2, &self, &p_time);
  collector_ai.stop();
}

void ObjectCharacter::RunCombatBehaviour(float)
{
  if (IsMoving() && GetActionPoints() == 0)
  {
    NodePath waypoint_nodepath = GetOccupiedWaypoint()->nodePath;
    NodePath nodepath          = GetDynamicObject()->nodePath;

    if (nodepath.get_x() == waypoint_nodepath.get_x() && nodepath.get_y() == waypoint_nodepath.get_y())
      _level->GetCombat().NextTurn();
  }
  else if (!IsBusy())
    {
      PStatCollector collector_ai("Level:Characters:AI");
      bool           idle_during_fights = (this != _level->GetPlayer() && !script->IsDefined("combat"));

      if (GetHitPoints() <= 0 || GetActionPoints() == 0 || idle_during_fights)
        _level->GetCombat().NextTurn();
      else if (script->IsDefined("combat"))
      {
        AngelScript::Type<ObjectCharacter*> self(this);
        unsigned int                        ap_before = GetActionPoints();

        cout << "Calling AI" << endl;
        collector_ai.start();
        script->Call("combat", 1, &self);
        cout << "End Calling AI" << endl;
        collector_ai.stop();
        if (ap_before == GetActionPoints() && !IsBusy()) // If stalled, skip turn
        {
          cout << "Character " << GetName() << " is stalling" << endl;
          _level->GetCombat().NextTurn();
        }
        else if (IsMoving())
          cout << "Character " << GetName() << " is moving" << endl;
        else
          cout << "Character " << GetName() << " is playing" << endl;
      }
    }
}

bool ObjectCharacter::IsBusy(void) const
{
  return (IsMoving() || PlayingAnimationName() != "idle");
}

/*
void                ObjectCharacter::GoToRandomWaypoint(void)
{
  if (HasOccupiedWaypoint())
  {
    _goToData.objective = 0;
    _path.Clear();
    ReachedDestination.DisconnectAll();
    UnprocessCollisions();
    {
      list<Waypoint*>             wplist = GetOccupiedWaypoint()->GetSuccessors(0);
      
      if (!(wplist.empty()))
      {
	int                       rit    = rand() % wplist.size();
	list<Waypoint*>::iterator it     = wplist.begin();

	for (it = wplist.begin() ; rit ; --rit, ++it);
        // TODO reimplement that with the new pathfinding system
	//_path.push_back(**it);
	StartRunAnimation();
      }
    }
    ProcessCollisions();
  }
}*/

float               ObjectCharacter::GetDistance(const InstanceDynamicObject* object) const
{
  if (object != this)
  {
    LPoint3 pos_1  = _object->nodePath.get_pos();
    LPoint3 pos_2  = object->GetNodePath().get_pos();
    float   dist_x = pos_1.get_x() - pos_2.get_x();
    float   dist_y = pos_1.get_y() - pos_2.get_y();

    return (SQRT(dist_x * dist_x + dist_y * dist_y));
  }
  return (0);
}

bool                ObjectCharacter::HasLineOfSight(InstanceDynamicObject* object)
{
  return (line_of_sight.HasLineOfSight(object));
}

void                ObjectCharacter::RunDeath()
{
  cout << "Running death" << endl;
  cout << "- unequipping items" << endl;
  UnequipItem(0);
  UnequipItem(1);
  cout << "- clearing interactions" << endl;
  ClearInteractions();
  cout << "- adding looting interaction" << endl;
  AddInteraction("use", _level->GetInteractions().Use);

  GetNodePath().set_hpr(0, 0, 90);
  UnprocessCollisions();
  cout << "- etting hit points if superior to 0" << endl;
  if (GetHitPoints() > 0)
    SetHitPoints(0);
  cout << "Death ran" << endl;
}

bool ObjectCharacter::IsPlayer(void) const
{
  return (_level->GetPlayer() == this);
}

/*
 * Field of View
 */
Script::StdList<ObjectCharacter*>         ObjectCharacter::GetNearbyEnemies(void) const
{
  Script::StdList<ObjectCharacter*> ret;
  auto                              detection = field_of_view.GetDetectedEnemies();

  std::copy(detection.begin(), detection.end(), std::back_inserter(ret));
  return (ret);
}

Script::StdList<ObjectCharacter*> ObjectCharacter::GetNearbyAllies(void) const
{
  Script::StdList<ObjectCharacter*> ret;
  auto fovEnemies = field_of_view.GetDetectedAllies();
  auto it         = fovEnemies.begin();
  auto end        = fovEnemies.end();

  while (it != end)
  {
    //cout << "[" << it->enemy << "] FovEnemy: " << it->enemy->GetName() << endl;
    ret.push_back(*it);
    it++;
  }
  return (ret);
}

/*
 * Script Communication
 */
void     ObjectCharacter::SendMessage(string& str)
{
  if (script->IsDefined("SendMessage"))
  {
    AngelScript::Type<ObjectCharacter*> self(this);
    AngelScript::Type<std::string*>     message(&str);
    
    script->Call("SendMessage", 2, &self, &message);
  }
}

int      ObjectCharacter::AskMorale(void)
{
  if (script->IsDefined("AskMorale"))
  {
    AngelScript::Type<ObjectCharacter*> self(this);
    
    return (script->Call("AskMorale", 1, &self));
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
  if (script->IsDefined(func))
  {
    AngelScript::Type<ObjectCharacter*> self(f);
    AngelScript::Type<ObjectCharacter*> buddy(s);
    
    script->Call(func, 2, &self, &buddy);
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

void     ObjectCharacter::SetAsEnemy(ObjectCharacter* other, bool enemy)
{
  cout << "SetAsEnemy: " << GetName() << " -> " << other->GetName() << endl;
  if (_faction && other->GetFaction() != 0)
  {
    WorldDiplomacy& diplomacy = GameTask::CurrentGameTask->GetDiplomacy();

    cout << "Factions are now enemies: " << GetFactionName() << " -> " << other->GetFactionName() << endl;
    diplomacy.SetAsEnemy(enemy, _faction->flag, other->GetFaction());
  }
  else if (other->GetFaction() != 0)
  {
    cout << "Using self enemy mask: " << other->GetFactionName() << ':' << other->GetFaction() << endl;
    if (enemy)
      _self_enemyMask |= other->GetFaction();
    else if (_self_enemyMask & other->GetFaction())
      _self_enemyMask -= other->GetFaction();
  }
  else if (GetFaction() != 0)
    other->SetAsEnemy(this, enemy);
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

void ObjectCharacter::Unserialize(Utils::Packet& packet)
{
  packet >> _self_enemyMask >> _flags;
  CharacterVisibility::Unserialize(packet);
  if (GetHitPoints() <= 0)
    RunDeath();
}

void ObjectCharacter::Serialize(Utils::Packet& packet)
{
  packet << _self_enemyMask << _flags;
  CharacterVisibility::Serialize(packet);
}
