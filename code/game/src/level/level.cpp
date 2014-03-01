#include "globals.hpp"
#include "level/level.hpp"
#include "astar.hpp"

#include "level/objects/door.hpp"
#include "level/objects/shelf.hpp"
#include <level/objects/locker.hpp>
#include <level/interactions/action_use_object.hpp>
#include <level/interactions/action_drop_object.hpp>
#include <i18n.hpp>

#include "ui/alert_ui.hpp"
#include "ui/ui_loot.hpp"
#include "ui/ui_equip_mode.hpp"
#include "options.hpp"
#include <mousecursor.hpp>
#include <boost/concept_check.hpp>
#include <boost/detail/container_fwd.hpp>

#define AP_COST_USE             2
#define WORLDTIME_TURN          10
#define WORLDTIME_DAYLIGHT_STEP 3


using namespace std;

Level* Level::CurrentLevel = 0;
Level::Level(const std::string& name, WindowFramework* window, GameUi& gameUi, Utils::Packet& packet, TimeManager& tm) : _window(window),
  camera(*this, window, window->get_camera_group()),
  mouse (*this, window),
  _timeManager(tm),
  _main_script(name),
  level_ui(window, gameUi),
  mouse_hint(*this, level_ui, mouse),
  _chatter_manager(window),
  floors(*this),
  zones(*this)
{
  cout << "Level Loading Step #1" << endl;
  CurrentLevel = this;
  _state       = Normal;
  _persistent  = true;
  _level_name  = name;
  _sunlight    = 0;

  floors.EnableShowLowerFloors(true);

  hovered_path.SetRenderNode(window->get_render());

  obs.Connect(level_ui.InterfaceOpened, *this, &Level::SetInterrupted);

  cout << "Level Loading Step #2" << endl;
  _items = DataTree::Factory::JSON("data/objects.json");

  cout << "Level Loading Step #3" << endl;
  _graphicWindow = _window->get_graphics_window();

  cout << "Level Loading Step #4" << endl;
  _timer.Restart();

  // WORLD LOADING
  cout << "Level Loading Step #5" << endl;
  _world = new World(window);  
  try
  {
    _world->UnSerialize(packet);
    _light_iterator = _world->lights.begin();
  }
  catch (unsigned int&)
  {
    std::cout << "Failed to load file" << std::endl;
  }

  cout << "Level Loading Step #6" << endl;
  if (_world->sunlight_enabled)
    InitializeSun();

  LPoint3 upperLeft, upperRight, bottomLeft;
  cout << "Level Loading Step #7" << endl;
  _world->GetWaypointLimits(0, upperRight, upperLeft, bottomLeft);
  camera.SetLimits(bottomLeft.get_x() - 50, bottomLeft.get_y() - 50, upperRight.get_x() - 50, upperRight.get_y() - 50);  

  cout << "Level Loading Step #8" << endl;
  ForEach(_world->entryZones, [this](EntryZone& zone) { zones.RegisterZone(zone); });
  ForEach(_world->exitZones,  [this](ExitZone& zone)  { zones.RegisterZone(zone); });
  
  ForEach(_world->dynamicObjects, [this](DynamicObject& dobj) { InsertDynamicObject(dobj); });
  _itCharacter = _characters.end();

  _world->SetWaypointsVisible(false);

  //loadingScreen->AppendText("Loading interface");
  obs.Connect(interactions.Use,         *this, &Level::CallbackActionUse);
  obs.Connect(interactions.TalkTo,      *this, &Level::CallbackActionTalkTo);
  obs.Connect(interactions.UseObjectOn, *this, &Level::CallbackActionUseObjectOn);
  obs.Connect(interactions.UseSkillOn,  *this, &Level::CallbackActionUseSkillOn);
  obs.Connect(interactions.UseSpellOn,  *this, &Level::CallbackActionUseSpellOn);
//obs.Connect(interactions.LookAt,      *this, &Level::CallbackActionLookAt);

  _task_metabolism = _timeManager.AddRepetitiveTask(TASK_LVL_CITY, DateTime::Hours(1));
  _task_metabolism->Interval.Connect(*this, &Level::RunMetabolism);  


  /*
   * DIVIDE AND CONQUER WAYPOINTS
   */
  std::vector<Waypoint*>                      entries;
  
  for_each(_world->waypoints.begin(), _world->waypoints.end(), [&entries](Waypoint& wp) { entries.push_back(&wp); });
  _world->waypoint_graph.SetHeuristic([](LPoint3f position1, LPoint3f position2) -> float
  {
    float xd = position2.get_x() - position1.get_x();
    float yd = position2.get_y() - position1.get_y();
    float zd = position2.get_z() - position1.get_z();

    return (SQRT(xd * xd + yd * yd + zd * zd));
  });
  _world->waypoint_graph.Initialize(entries, [](const std::vector<Waypoint*>& entries) -> std::vector<LPoint3f>
  {
    std::vector<LPoint3f> positions;
    
    {
      LPoint3f block_size;
      LPoint3f max_pos(0, 0, 0);
      LPoint3f min_pos(0, 0, 0);
      auto     it  = entries.begin();
      auto     end = entries.end();

      for (; it != end ; ++it)
      {
        LPoint3f pos = (*it)->GetPosition();

        if (pos.get_x() < min_pos.get_x()) { min_pos.set_x(pos.get_x()); }
        if (pos.get_y() < min_pos.get_y()) { min_pos.set_y(pos.get_y()); }
        if (pos.get_z() < min_pos.get_z()) { min_pos.set_z(pos.get_z()); }
        if (pos.get_x() > max_pos.get_x()) { max_pos.set_x(pos.get_x()); }
        if (pos.get_y() > max_pos.get_y()) { max_pos.set_y(pos.get_y()); }
        if (pos.get_z() > max_pos.get_z()) { max_pos.set_z(pos.get_z()); }
      }
      
      function<float (float, float)> distance = [](float min_pos, float max_pos) -> float
      {
        if (min_pos < 0 && max_pos > 0)
          return (ABS(min_pos - max_pos));
        return (ABS(max_pos) - ABS(min_pos));
      };
      
      block_size.set_x(distance(min_pos.get_x(), max_pos.get_x()));
      block_size.set_y(distance(min_pos.get_y(), max_pos.get_y()));
      block_size.set_z(distance(min_pos.get_z(), max_pos.get_z()));

      unsigned short block_count = ceil(entries.size() / 200.f);
      for (unsigned short i = 0 ; i < block_count ; ++i)
      {
        LPoint3f block_position;
        
        block_position.set_x(min_pos.get_x() + block_size.get_x() / block_count * i);
        block_position.set_y(min_pos.get_y() + block_size.get_y() / block_count * i);
        block_position.set_z(min_pos.get_z() + block_size.get_z() / block_count * i);
        positions.push_back(block_position);
      }
    }
    return (positions);
  });
  /*
   * END DIVIDE AND CONQUER
   */

  //window->get_render().set_shader_auto();
}

void Level::RefreshCharactersVisibility(void)
{
  ObjectCharacter* player              = GetPlayer();
  auto             detected_characters = player->GetFieldOfView().GetDetectedCharacters();

  for_each(_characters.begin(), _characters.end(),
           [this, detected_characters, player](ObjectCharacter* character)
  {
    if (character != player)
    {
      auto it = find(detected_characters.begin(), detected_characters.end(), character);

      character->SetVisible(it != detected_characters.end());
    }
  });
}

void Level::InsertCharacter(ObjectCharacter* character)
{
  character->GetFieldOfView().SetIntervalDurationInSeconds(3);
  character->GetFieldOfView().Launch();
  _characters.push_back(character);
}

void Level::InsertInstanceDynamicObject(InstanceDynamicObject* object)
{
  _objects.push_back(object);
}

void Level::InsertDynamicObject(DynamicObject& object)
{
  InstanceDynamicObject* instance = 0;

  switch (object.type)
  {
    case DynamicObject::Character:
      InsertCharacter(new ObjectCharacter(this, &object));
      break ;
    case DynamicObject::Door:
      instance = new ObjectDoor(this, &object);
      break ;
    case DynamicObject::Shelf:
      instance = new ObjectShelf(this, &object);
      break ;
    case DynamicObject::Item:
    {
      DataTree*        item_data = DataTree::Factory::StringJSON(object.inventory.front().first);
      InventoryObject* item;

      item_data->key = object.key;
      item           = new InventoryObject(item_data);
      instance       = new ObjectItem(this, &object, item);
      delete item_data;
      break ;
    }
    case DynamicObject::Locker:
      instance = new ObjectLocker(this, &object);
      break ;
    default:
    {
      stringstream stream;

      stream << "Inserted unimplemented dynamic object type (" << object.type << ')';
      AlertUi::NewAlert.Emit(stream.str());
    }
  }
  cout << "Added an instance => " << instance << endl;
  if (instance != 0)
    _objects.push_back(instance);
}

void Level::InitializeSun(void)
{
  _sunlight = new Sunlight(*_world, _timeManager);
  _sunlight->SetAsRepetitiveTask(true);
  _sunlight->SetIntervalDuration(DateTime::Minutes(1));
  _sunlight->Launch();
}

void Level::InitializePlayer(void)
{
  if (!(GetPlayer()->GetStatistics().Nil()))
  {
    Data stats(GetPlayer()->GetStatistics());
    
    if (!(stats["Statistics"]["Action Points"].Nil()))
      level_ui.GetMainBar().SetMaxAP(stats["Statistics"]["Action Points"]);
  }
  {
    Interactions::InteractionList& interactions_on_player = GetPlayer()->GetInteractions();

    interactions_on_player.clear();
    interactions_on_player.push_back(Interactions::Interaction("use_object", GetPlayer(), &(interactions.UseObjectOn)));
    interactions_on_player.push_back(Interactions::Interaction("use_skill",  GetPlayer(), &(interactions.UseSkillOn)));
    interactions_on_player.push_back(Interactions::Interaction("use_magic",  GetPlayer(), &(interactions.UseSpellOn)));
  }
  
  level_ui.GetMainBar().SetStatistics(GetPlayer()->GetStatController());
  level_ui.GetMainBar().OpenSkilldex.Connect([this]() { interactions.UseSkillOn.Emit(GetPlayer()); });
  level_ui.GetMainBar().OpenSpelldex.Connect([this]() { interactions.UseSpellOn.Emit(0);           });

  obs_player.Connect(GetPlayer()->EquipedItemActionChanged, level_ui.GetMainBar(),   &GameMainBar::SetEquipedItemAction);
  obs_player.Connect(GetPlayer()->EquipedItemChanged,       level_ui.GetMainBar(),   &GameMainBar::SetEquipedItem);
  obs_player.Connect(GetPlayer()->EquipedItemChanged,       level_ui.GetInventory(), &GameInventory::SetEquipedItem);
  level_ui.GetMainBar().EquipedItemNextAction.Connect(*GetPlayer(), &ObjectCharacter::ItemNextUseType);
  level_ui.GetMainBar().UseEquipedItem.Connect       (*this, &Level::CallbackActionTargetUse);
  level_ui.GetMainBar().CombatEnd.Connect            ([this](void)
  {
    StopFight();
    if (_state == Fight)
      ConsoleWrite("You can't leave combat mode if enemies are nearby.");
  });
  level_ui.GetMainBar().CombatPassTurn.Connect       (*this, &Level::NextTurn);

  obs.Connect(level_ui.GetInventory().EquipItem,   [this](const std::string& target, unsigned int slot, InventoryObject* object)
  {
    if (target == "equiped")
      PlayerEquipObject(slot, object);
  });
  obs.Connect(level_ui.GetInventory().UnequipItem, [this](const std::string& target, unsigned int slot)
  {
    if (target == "equiped")
      GetPlayer()->UnequipItem(slot);
  });

  obs.Connect(level_ui.GetInventory().DropObject,  *this,        &Level::PlayerDropObject);
  obs.Connect(level_ui.GetInventory().UseObject,   *this,        &Level::PlayerUseObject);

  for (unsigned short it = 0 ; it < 2 ; ++it) // For every equiped item slot
  {
    level_ui.GetMainBar().SetEquipedItem(it, GetPlayer()->GetEquipedItem(it));
    level_ui.GetInventory().SetEquipedItem(it, GetPlayer()->GetEquipedItem(it));
  }

  player_halo.SetTarget(GetPlayer());
  GetPlayer()->GetFieldOfView().Launch();
  target_outliner.UsePerspectiveOfCharacter(GetPlayer());

  //
  // Initializing Main Script
  //
  if (_main_script.IsDefined("Initialize"))
    _main_script.Call("Initialize");
}

void Level::SetAsPlayerParty(Party&)
{
  InitializePlayer();
  camera.SetConfigurationFromLevelState();
}

void Level::InsertParty(Party& party, const std::string& zone_name)
{
  auto party_members = party.GetPartyMembers();
  auto it            = party_members.rbegin();
  auto end           = party_members.rend();
  
  for (; it != end ; ++it)
  {
    Party::Member*   member         = *it;
    DynamicObject&   dynamic_object = member->GetDynamicObject();
    DynamicObject*   world_object   = _world->InsertDynamicObject(dynamic_object);
    ObjectCharacter* character      = new ObjectCharacter(this, world_object);

    _world->InsertDynamicObject(dynamic_object);
    member->LinkCharacter(character);
    _characters.insert(_characters.begin(), character);
  }
  GetZoneManager().InsertPartyInZone(party, zone_name);
  if (party.GetName() == "player")
    SetAsPlayerParty(party);
}

void Level::MatchPartyToExistingCharacters(Party& party)
{
  RunForPartyMembers(party, [](Party::Member* member, ObjectCharacter* character)
  {
    member->LinkCharacter(character);
  });
  if (party.GetName() == "player")
    SetAsPlayerParty(party);
}

void Level::RemovePartyFromLevel(Party& party)
{
  if (party.GetName() == "player")
    obs_player.DisconnectAll();
  RunForPartyMembers(party, [this](Party::Member* member, ObjectCharacter* character)
  {
    auto character_it = find(_characters.begin(), _characters.end(), character);

    member->SaveCharacter(character);
    character->UnprocessCollisions();
    delete character;
    _characters.erase(character_it);
  });
}

void Level::RunForPartyMembers(Party& party, function<void (Party::Member*,ObjectCharacter*)> callback)
{
  auto party_members = party.GetPartyMembers();
  auto it            = party_members.begin();
  auto end           = party_members.end();
  
  for (; it != end ; ++it)
  {
    Party::Member* member         = *it;
    DynamicObject& dynamic_object = member->GetDynamicObject();
    CharacterList  matches        = FindCharacters([&dynamic_object](ObjectCharacter* character) -> bool
    {
      return (character->GetDynamicObject()->name == dynamic_object.name);
    });
    
    if (matches.size() == 0)
      AlertUi::NewAlert.Emit("Couldn't match party member '" + dynamic_object.name + "' to anyone in the level.");
    else
    {
      if (matches.size() > 1)
        AlertUi::NewAlert.Emit("There was more than one match when matching party character '" + dynamic_object.name + "' to the level's characters.");
      callback(member, *matches.begin());
    }
  }
}

void Level::SetPlayerInventory(Inventory* inventory)
{
  ObjectCharacter* player = GetPlayer();

  if (!inventory)
  {
    cout << "Using map's inventory" << endl;
    inventory = &(player->GetInventory());
  }
  else
  {  player->SetInventory(inventory);       }
  level_ui.GetInventory().SetInventory(*inventory);
  player->EquipedItemChanged.Emit(0, player->GetEquipedItem(0));
  player->EquipedItemChanged.Emit(1, player->GetEquipedItem(1));
}

Level::~Level()
{
  cout << "- Destroying Level" << endl;
  try
  {
    if (_main_script.IsDefined("Finalize"))
      _main_script.Call("Finalize");
  }
  catch (const AngelScript::Exception& exception)
  {
	AlertUi::NewAlert.Emit(std::string("Script crashed during Level destruction: ") + exception.what());
  }
  MouseCursor::Get()->SetHint("");
  _window->get_render().clear_light();

  _timeManager.ClearTasks(TASK_LVL_CITY);
  obs.DisconnectAll();
  obs_player.DisconnectAll();
  _projectiles.CleanUp();
  ForEach(_objects,     [](InstanceDynamicObject* obj) { delete obj;        });
  ForEach(_parties,     [](Party* party)               { delete party;      });
  zones.UnregisterAllZones();
  CurrentLevel = 0;
  if (_sunlight) delete _sunlight;
  if (_world)    delete _world;
  if (_items)    delete _items;
  cout << "-> Done." << endl;
}

InstanceDynamicObject* Level::GetObject(const string& name)
{
  InstanceObjects::iterator it  = _objects.begin();
  InstanceObjects::iterator end = _objects.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)) == name)
      return (*it);
  }
  return (0);
}

Level::CharacterList Level::FindCharacters(function<bool (ObjectCharacter*)> selector) const
{
  CharacterList              list;
  Characters::const_iterator it  = _characters.begin();
  Characters::const_iterator end = _characters.end();

  for (; it != end ; ++it)
  {
    ObjectCharacter* character = *it;
    
    if (selector(character))
      list.push_back(character);
  }
  return (list);
}

ObjectCharacter* Level::GetCharacter(const string& name)
{
  Characters::const_iterator it  = _characters.begin();
  Characters::const_iterator end = _characters.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)) == name)
      return (*it);
  }
  return (0);
}

ObjectCharacter* Level::GetCharacter(const DynamicObject* object)
{
  Characters::iterator it  = _characters.begin();
  Characters::iterator end = _characters.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)).GetDynamicObject() == object)
      return (*it);
  }
  return (0);
}

ObjectCharacter* Level::GetPlayer(void)
{
  if (_characters.size() == 0)
    return (0);
  return (_characters.front());
}

void Level::SetState(State state)
{
  _state = state;
  if (state == Normal)
    _itCharacter = _characters.end();
  if (state != Fight)
  {
    hovered_path.Hide();
    target_outliner.DisableOutline();
  }
  camera.SetEnabledScroll(state != Interrupted);
  camera.SetConfigurationFromLevelState();
}

void Level::SetInterrupted(bool set)
{
  if (set == false && level_ui.HasOpenedWindows())
    set = true;
  
  if (set)
    SetState(Interrupted);
  else
  {
    if (_itCharacter == _characters.end())
      SetState(Normal);
    else
      SetState(Fight);
  }
}

void Level::StartFight(ObjectCharacter* starter)
{
  _itCharacter = std::find(_characters.begin(), _characters.end(), starter);
  if (_itCharacter == _characters.end())
  { 
    cout << "[FATAL ERROR][Level::StartFight] Unable to find starting character" << endl;
    if (_characters.size() < 1)
    {
      cout << "[FATAL ERROR][Level::StartFight] Can't find a single character" << endl;
      return ;
    }
    _itCharacter = _characters.begin();
  }
  level_ui.GetMainBar().SetEnabledAP(true);
  {
    ObjectCharacter* current_fighter = *_itCharacter;

    current_fighter->SetActionPoints(current_fighter->GetMaxActionPoints());
  }
  if (_state != Fight)
    ConsoleWrite("You are now in combat mode.");
  SetState(Fight);
}

void Level::StopFight(void)
{
  if (_state == Fight)
  {
    Characters::iterator it  = _characters.begin();
    Characters::iterator end = _characters.end();
    
    for (; it != end ; ++it)
    {
      if (!((*it)->IsAlly(GetPlayer())))
      {
        list<ObjectCharacter*> listEnemies = (*it)->GetNearbyEnemies();

        if (!(listEnemies.empty()) && (*it)->IsAlive())
          return ;
      }
    }
    if (mouse.GetState() == MouseEvents::MouseTarget)
      mouse.SetState(MouseEvents::MouseAction);
    ConsoleWrite("Combat ended.");
    SetState(Normal);
    level_ui.GetMainBar().SetEnabledAP(false);
  }
}

void Level::NextTurn(void)
{
  if (_state != Fight || _currentCharacter != _itCharacter)
  {
    cout << "cannot go to next turn" << endl;
    return ;
  }
  if (*_itCharacter == GetPlayer())
    StopFight();
  if (_itCharacter != _characters.end())
  {
    cout << "Playing animation idle" << endl;
    (*_itCharacter)->PlayAnimation("idle");
  }
  if ((++_itCharacter) == _characters.end())
  {
    _itCharacter = _characters.begin();
    (*_itCharacter)->GetFieldOfView().RunCheck();
    _timeManager.AddElapsedTime(WORLDTIME_TURN);
  }
  if (_itCharacter != _characters.end())
  {
    ObjectCharacter* current_fighter = *_itCharacter;
    
    current_fighter->SetActionPoints(current_fighter->GetMaxActionPoints());
  }
  else
    cout << "[FATAL ERROR][Level::NextTurn] Character Iterator points to nothing (n_characters = " << _characters.size() << ")" << endl;
  camera.SetConfigurationFromLevelState();
}

void Level::RunMetabolism(void)
{
  for_each(_characters.begin(), _characters.end(), [this](ObjectCharacter* character)
  {
    if (character != GetPlayer() && character->GetHitPoints() > 0)
    {
      StatController* controller = character->GetStatController();

      controller->RunMetabolism();
    }
  });
}

AsyncTask::DoneStatus Level::do_task(void)
{
  float elapsedTime = _timer.GetElapsedTime();

  if (level_ui.GetContext()->GetHoverElement() == level_ui.GetContext()->GetRootElement())
    mouse.SetCursorFromState();
  else
    mouse.SetMouseState('i');

  player_halo.Run();

  bool use_fog_of_war = false;
  if (use_fog_of_war == false)
    RefreshCharactersVisibility();

  camera.SlideToHeight(GetPlayer()->GetDynamicObject()->nodePath.get_z());
  camera.Run(elapsedTime);  

  mouse_hint.Run();

  std::function<void (InstanceDynamicObject*)> run_object = [elapsedTime](InstanceDynamicObject* obj)
  {
    obj->Run(elapsedTime);
    obj->UnprocessCollisions();
    obj->ProcessCollisions();
  };
  switch (_state)
  {
    case Fight:
      ForEach(_objects, run_object);
      // If projectiles are moving, run them. Otherwise, run the current character
      if (_projectiles.size() > 0)
        _projectiles.Run(elapsedTime);
      else
      {
        _currentCharacter = _itCharacter; // Keep a character from askin NextTurn several times
        if (_itCharacter != _characters.end())
          run_object(*_itCharacter);
        if (mouse.Hovering().hasWaypoint && mouse.GetState() == MouseEvents::MouseAction)
          hovered_path.DisplayHoveredPath(GetPlayer(), mouse);
      }
      break ;
    case Normal:
      _projectiles.Run(elapsedTime);
      _timeManager.AddElapsedSeconds(elapsedTime);
      ForEach(_objects,    run_object);
      ForEach(_characters, run_object);
      break ;
    case Interrupted:
      break ;
  }
  //ForEach(_characters, [elapsedTime](ObjectCharacter* character) { character->RunEffects(elapsedTime); });
  zones.Refresh();
  
  if (_main_script.IsDefined("Run"))
  {
    AngelScript::Type<float> param_time(elapsedTime);

    _main_script.Call("Run", 1, &param_time);
  }

  floors.SetCurrentFloorFromObject(GetPlayer());
  floors.RunFadingEffect(elapsedTime);
  _chatter_manager.Run(elapsedTime, camera.GetNodePath());
  _particle_manager.do_particles(ClockObject::get_global_clock()->get_dt());
  mouse.Run();
  _timer.Restart();
  return (exit.ReadyForNextZone() ? AsyncTask::DS_done : AsyncTask::DS_cont);
}

/*
 * Nodes Management
 */
InstanceDynamicObject* Level::FindObjectFromNode(NodePath node)
{
  {
    InstanceObjects::iterator cur = _objects.begin();
    
    while (cur != _objects.end())
    {
      if ((**cur) == node)
	return (*cur);
      ++cur;
    }
  }
  {
    Characters::iterator      cur = _characters.begin();
    
    while (cur != _characters.end())
    {
      if ((**cur) == node)
	return (*cur);
      ++cur;
    }
  }
  return (0);
}

void                   Level::RemoveObject(InstanceDynamicObject* object)
{
  InstanceObjects::iterator it = std::find(_objects.begin(), _objects.end(), object);
  
  if (it != _objects.end())
  {
    _world->DeleteDynamicObject((*it)->GetDynamicObject());
    delete (*it);
    _objects.erase(it);
  }
}

void                   Level::UnprocessAllCollisions(void)
{
  ForEach(_objects,    [](InstanceDynamicObject* object) { object->UnprocessCollisions(); });
  ForEach(_characters, [](ObjectCharacter*       object) { object->UnprocessCollisions(); });
}

void                   Level::ProcessAllCollisions(void)
{
  ForEach(_objects,    [](InstanceDynamicObject* object) { object->ProcessCollisions(); });
  ForEach(_characters, [](ObjectCharacter*       object) { object->ProcessCollisions(); });
}

void Level::ConsoleWrite(const string& str)
{
  level_ui.GetMainBar().AppendToConsole(str);
}

void Level::PlayerLootWithScript(Inventory* inventory, InstanceDynamicObject* target, asIScriptContext* context, const std::string& filepath)
{
  if (inventory && target && context)
  {
    UiLoot* ui_loot = level_ui.OpenUiLoot(&GetPlayer()->GetInventory(), inventory);

    ui_loot->SetScriptObject(GetPlayer(), target, context, filepath);
  }
}

void Level::PlayerLoot(Inventory* inventory)
{
  if (!inventory)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[PlayerLoot] Aborted: NullPointer Error</span>");
    return ;
  }
  level_ui.OpenUiLoot(&GetPlayer()->GetInventory(), inventory);
}

void Level::PlayerEquipObject(unsigned short it, InventoryObject* object)
{
  equip_modes.SearchForUserOnItemWithSlot(GetPlayer(), object, "equiped");
  if (equip_modes.HasOptions())
  {
    auto player_set_equiped_item = [this, it, object](unsigned char mode)
    {
      GetPlayer()->GetInventory().SetEquipedItem("equiped", it, object, mode);
    };
    
    if (equip_modes.HasChoice())
    {
      UiEquipMode* ui = level_ui.OpenUiEquipMode();

      equip_modes.Foreach([ui](unsigned char mode, const string& name)
      {
        ui->AddOption(mode, name);
      });
      ui->EquipModeSelected.Connect(player_set_equiped_item);
      ui->Initialize();
    }
    else
    {
      equip_modes.Foreach([player_set_equiped_item](unsigned char mode, const string&) { player_set_equiped_item(mode); });
    }
  }
  else
    ConsoleWrite(i18n::T("You can't equip " + i18n::T(object->GetName())));
}

void Level::PlayerEquipObject(const std::string& target, unsigned int slot, InventoryObject* object)
{
  if (target == "equiped")
    PlayerEquipObject(slot, object);
  else
  {
  }
}

void Level::PlayerDropObject(InventoryObject* object)
{
  Interactions::Actions::DropObject::Factory(GetPlayer(), object);
}

void Level::PlayerUseObject(InventoryObject* object)
{
  Interactions::Actions::UseObject::Factory(GetPlayer(), object, 0); // Default action is 0
}

bool Level::UseActionPoints(unsigned short ap)
{
  if (_state == Fight)
  {
    ObjectCharacter& character = (**_itCharacter);
    unsigned short   charAp    = character.GetActionPoints();

    if (charAp >= ap)
      character.SetActionPoints(charAp - ap);
    else
    {
      if (&character == GetPlayer())
        ConsoleWrite("Not enough action points");
      return (false);
    }
  }
  return (true);
}

bool Level::IsWaypointOccupied(unsigned int id) const
{
  InstanceObjects::const_iterator it_object;
  Characters::const_iterator      it_character;
  
  for (it_object = _objects.begin() ; it_object != _objects.end() ; ++it_object)
  {
    if ((*it_object)->HasOccupiedWaypoint() && (int)id == ((*it_object)->GetOccupiedWaypointAsInt()))
      return (true);
  }
  for (it_character = _characters.begin() ; it_character != _characters.end() ; ++it_character)
  {
    if ((*it_character)->HasOccupiedWaypoint() && (int)id == ((*it_character)->GetOccupiedWaypointAsInt()))
      return (true);
  }
  return (false);
}

ISampleInstance* Level::PlaySound(const string& name)
{
  if (_sound_manager.Require(name))
  {
    ISampleInstance* instance = _sound_manager.CreateInstance(name);

    instance->Start();
    return (instance);
  }
  return (0);
}
