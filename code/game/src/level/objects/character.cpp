#include "level/objects/character.hpp"
#include <panda3d/nodePathCollection.h>
#include "circular_value_set.hpp"

#include "level/level.hpp"
#include <options.hpp>
#include <dices.hpp>
#include <iterator>
#include "gametask.hpp"

using namespace std;

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) :
  CharacterVisibility(level, object),
  line_of_sight(*level->GetWorld(), _window->get_render(), GetNodePath()),
  field_of_view(*level, *this), equipment(*this)
{
  NodePath body_node   = object->nodePath.find("**/+Character");
  Inventory* inventory = new Inventory;

  _type                = ObjectTypes::Character;
  _inventory           = 0;
  _flags               = 0;
  _character           = dynamic_cast<Character*>(body_node.node());
  current_action       = 0;

  SetMovementAnimation("run");

  SetCollideMaskOnSingleNodepath(_object->nodePath, ColMask::DynObject | ColMask::FovTarget);
  _object->nodePath.set_collide_mask(ColMask::DynObject);

  // Faction
  _faction        = 0;
  _self_enemyMask = 0;
  
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
  {
    Data statistics = GetStatistics();

    if (statistics["Inventory"].NotNil())
      inventory->LoadInventory(statistics["Inventory"]);
    else
    {
      inventory->LoadInventory(_object);
      statistics["Inventory"].Remove();
    }
    SetInventory(inventory);
  }

  RefreshStatistics();
  
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
  cout << "ActionUse character" << endl;
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

    if (_inventory)
    {
      _inventory->UnequipAllItems();
      delete _inventory;
    }
    _inventory = inventory;
    equipment.SetInventory(inventory);
    inventory->SetCapacity(statistics["Statistics"]["Carry Weight"]);
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
  {
    delete current_action;
    current_action = 0;
  }
  if (script)
  {
    delete script;
    script = 0;
  }
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
  EquipedItem* equiped_item = equipment.GetEquipedItem("equiped", it);

  if (equiped_item && equiped_item->item)
  {
    InventoryObject& item           = *(equiped_item->item);
    Data             animation      = item["actions"][equiped_item->current_action]["animations"];
    Data             playerAnim     = animation["player"][name];
    const string     playerAnimName = (playerAnim.Nil() ? ANIMATION_DEFAULT : playerAnim.Value());

    //
    // Character's animation
    //
    if (playerAnim.NotNil())
    {
      EquipModes& equip_modes = GetLevel()->GetEquipModes();
      string      anim_name   = equip_modes.GetNameForMode(equiped_item->slot->mode);

      anim_name += '-' + playerAnimName;
      PlayAnimation(anim_name);
    }
    else
      PlayAnimation(playerAnimName);
  }
}


NodePath ObjectCharacter::Equipment::GetEquipedItemNode(const std::string& slot, unsigned short slot_number)
{
  EquipedItem* equiped_item = GetEquipedItem(slot, slot_number);

  if (equiped_item != 0 && equiped_item->render != 0)
    return (equiped_item->render->GetNodePath());
  return (NodePath());
}

void ObjectCharacter::RefreshEquipment(void)
{
  /*for (short i = 0 ; i < 2 ; ++i)
  {
    EquipedItemChanged.Emit      (i, _equiped[i].equiped);
    EquipedItemActionChanged.Emit(i, _equiped[i].equiped, _equiped[i].actionIt);
  }*/
}

void ObjectCharacter::ItemNextUseType(unsigned short it)
{
  EquipedItem* equiped_item = equipment.GetEquipedItem("equiped", it);

  if (equiped_item && equiped_item->item)
  {
    cout << "Equiped item on slot " << it << " is " << equiped_item->item->GetName() << endl;
    Data             itemData   = *(equiped_item->item);
    Data             actionData = itemData["actions"];
    unsigned char    action     = equiped_item->current_action;

    if (!(actionData.Nil()))
    {
      if (actionData.Count() <= (unsigned int)action + 1)
        equiped_item->current_action = 0;
      else
        equiped_item->current_action++;
      if (action != equiped_item->current_action)
        EquipedItemActionChanged.Emit(it, equiped_item->item, equiped_item->current_action);
    }
  }
}

bool ObjectCharacter::IsInterrupted() const
{
  if (HasOccupiedWaypoint())
  {
    LPoint3f target = GetOccupiedWaypoint()->nodePath.get_pos();
    LPoint3f pos    = GetNodePath().get_pos();

    if (target.get_x() != pos.get_x() && target.get_y() != pos.get_y())
      return (true);
  }
  return (AnimationEndForObject.ObserverCount() > 0);
}

void ObjectCharacter::Run(float elapsedTime)
{
  //Timer profile;
  Level::State state = _level->GetState();

  CharacterVisibility::Run(elapsedTime);
  field_of_view.MarkForUpdate();
  if (!(IsInterrupted()))
  {
    try
    {
      if (state == Level::Normal && GetHitPoints() > 0 && script->IsDefined("main"))
        RunRegularBehaviour(elapsedTime);
      else if (state == Level::Fight)
        RunCombatBehaviour(elapsedTime);
    }
    catch (AngelScript::Exception exception)
    {
      GameConsole::Get().WriteOn("[Character][" + GetName() + "] " + exception.what());
      if (state == Level::Fight)
        _level->GetCombat().NextTurn();
    }
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
  if (_inventory)
    _inventory->UnequipAllItems();
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
