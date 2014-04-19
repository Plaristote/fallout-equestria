#include "globals.hpp"
#include "level/level.hpp"
#include "astar.hpp"

#include "level/objects/door.hpp"
#include "level/objects/shelf.hpp"
#include <level/objects/locker.hpp>
#include <world/world_flatten.hpp>
#include <i18n.hpp>

#include "ui/alert_ui.hpp"
#include "ui/ui_loot.hpp"
#include "ui/ui_equip_mode.hpp"
#include <ui/loading_screen.hpp>
#include "options.hpp"
#include <mousecursor.hpp>

#include "loading_exception.hpp"

#define AP_COST_USE             2

using namespace std;

Level* Level::CurrentLevel = 0;
Level::Level(const std::string& name, WindowFramework* window, GameUi& gameUi, Utils::Packet& packet, TimeManager& tm) : window(window),
  camera(*this, window, window->get_camera_group()),
  time_manager(tm),
  main_script(name),
  level_ui(window, gameUi),
  mouse(*this, level_ui),
  player(*this),
  chatter_manager(window),
  combat(*this, characters),
  floors(*this),
  zones(*this)
{
  CurrentLevel  = this;
  level_state   = Normal;
  is_persistent = true;
  level_name    = name;
  sunlight      = 0;

  floors.EnableShowLowerFloors(true);

  hovered_path.SetRenderNode(window->get_render());

  obs.Connect(level_ui.InterfaceOpened, *this, &Level::SetInterrupted);

  timer.Restart();

  // WORLD LOADING
  LoadingScreen::AppendText("Loading World...");
  world = new World(window);  
  try
  {
    world->UnSerialize(packet);
    _light_iterator = world->lights.begin();
  }
  catch (unsigned int&)
  {
    throw LoadingException("Couldn't load the level's world");
  }

  cout << "Level Loading Step #6" << endl;
  if (world->sunlight_enabled)
  {
    LoadingScreen::AppendText("Celestial bodies detected.");
    InitializeSun();
  }

  LPoint3 upperLeft(0,0,0), upperRight(0,0,0), bottomLeft(0,0,0);
  LoadingScreen::AppendText("Setting up visual sensors...");
  world->GetWaypointLimits(0, upperRight, upperLeft, bottomLeft);
  camera.SetLimits((bottomLeft.get_x() - 50) * 1.25, (bottomLeft.get_y() - 50) * 1.25, (upperRight.get_x() + 50) * 1.25, (upperRight.get_y() + 50) * 1.25);

  LoadingScreen::AppendText("Processing topology...");
  ForEach(world->zones,          [this](Zone& zone)          { zones.RegisterZone(zone);  });
  LoadingScreen::AppendText("Analyzing surrounding objects...");
  ForEach(world->dynamicObjects, [this](DynamicObject& dobj) { InsertDynamicObject(dobj); });

  world->SetWaypointsVisible(false);
  
  player_halo.Initialize(window, world);
  
  LoadingScreen::AppendText("Flattening objects...");
  {
    //WorldFlattener flattener(*world);
    
    //flattener.Flatten();
  }

  /*
   * DIVIDE AND CONQUER WAYPOINTS
   */
  std::vector<Waypoint*>                      entries;
  
  for_each(world->waypoints.begin(), world->waypoints.end(), [&entries](Waypoint& wp) { entries.push_back(&wp); });
  world->waypoint_graph.SetHeuristic([](LPoint3f position1, LPoint3f position2) -> float
  {
    float xd = position2.get_x() - position1.get_x();
    float yd = position2.get_y() - position1.get_y();
    float zd = position2.get_z() - position1.get_z();

    return (SQRT(xd * xd + yd * yd + zd * zd));
  });
  world->waypoint_graph.Initialize(entries, [](const std::vector<Waypoint*>& entries) -> std::vector<LPoint3f>
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

Level::~Level()
{
  cout << "- Destroying Level" << endl;
  try
  {
    if (main_script.IsDefined("Finalize"))
      main_script.Call("Finalize");
  }
  catch (const AngelScript::Exception& exception)
  {
    AlertUi::NewAlert.Emit(std::string("Script crashed during Level destruction: ") + exception.what());
  }

  obs.DisconnectAll();
  player.UnsetPlayer();
  
  for_each(parties.begin(), parties.end(), [](Party* party)
  {
    if (party->GetName() != "player")
      delete party;
  });
  
  MouseCursor::Get()->SetHint("");
  window->get_render().clear_light();

  time_manager.ClearTasks(TASK_LVL_CITY);
  projectiles.CleanUp();
  ForEach(characters,  [](ObjectCharacter* obj)       { delete obj;        });
  ForEach(objects,     [](InstanceDynamicObject* obj) { delete obj;        });
  ForEach(parties,     [](Party* party)               { delete party;      });
  CurrentLevel = 0;
  if (sunlight) delete sunlight;
  zones.UnregisterAllZones();
  if (world)    delete world;
}

void Level::RefreshCharactersVisibility(void)
{
  ObjectCharacter*   player              = GetPlayer();
  
  if (player)
  {
    auto             detected_characters = player->GetFieldOfView().GetDetectedCharacters();

    for_each(characters.begin(), characters.end(),
            [this, detected_characters, player](ObjectCharacter* character)
    {
      if (character != player)
      {
        auto it = find(detected_characters.begin(), detected_characters.end(), character);

        character->SetVisible(it != detected_characters.end());
      }
    });
  }
}

void Level::InsertCharacter(ObjectCharacter* character)
{
  character->TeleportTo(character->GetOccupiedWaypoint());
  character->PlayIdleAnimation();
  character->GetFieldOfView().SetIntervalDurationInSeconds(3);
  character->GetFieldOfView().Launch();
  character->ProcessCollisions();
  characters.push_back(character);
}

void Level::InsertInstanceDynamicObject(InstanceDynamicObject* object)
{
  objects.push_back(object);
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
  {
    objects.push_back(instance);
    instance->ProcessCollisions();
  }
}

void Level::InitializeSun(void)
{
  sunlight = new Sunlight(*world, time_manager);
  sunlight->SetAsRepetitiveTask(true);
  sunlight->SetIntervalDuration(DateTime::Minutes(1));
  sunlight->Launch();
}

void Level::SetAsPlayerParty(Party&)
{
  if (GetPlayer() != 0)
  {
    player.SetPlayer(GetPlayer());
    if (main_script.IsDefined("Initialize"))
      main_script.Call("Initialize");
    camera.SetConfigurationFromLevelState();
    mouse.SetPlayer(GetPlayer());
    player_halo.SetTarget(GetPlayer());
  }
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
    DynamicObject*   world_object   = world->InsertDynamicObject(dynamic_object);
    ObjectCharacter* character      = new ObjectCharacter(this, world_object);

    member->LinkCharacter(character);
    characters.insert(characters.begin(), character);
  }
  GetZoneManager().InsertPartyInZone(party, zone_name);
  if (party.GetName() == "player")
    SetAsPlayerParty(party);
  parties.push_back(&party);
}

void Level::MatchPartyToExistingCharacters(Party& party)
{
  RunForPartyMembers(party, [](Party::Member* member, ObjectCharacter* character)
  {
    member->LinkCharacter(character);
  });
  if (party.GetName() == "player")
    SetAsPlayerParty(party);
  parties.push_back(&party);
}

void Level::RemovePartyFromLevel(Party& party)
{
  if (party.GetName() == "player")
    player.UnsetPlayer();
  RunForPartyMembers(party, [this](Party::Member* member, ObjectCharacter* character)
  {
    auto           character_it = find(characters.begin(), characters.end(), character);
    DynamicObject* world_object = character->GetDynamicObject();

    member->SaveCharacter(character);
    character->UnprocessCollisions();
    delete character;
    characters.erase(character_it);
    world->DeleteDynamicObject(world_object);
  });
  parties.remove(&party);
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

InstanceDynamicObject* Level::GetObject(const string& name)
{
  InstanceObjects::iterator it  = objects.begin();
  InstanceObjects::iterator end = objects.end();

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
  Characters::const_iterator it  = characters.begin();
  Characters::const_iterator end = characters.end();

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
  Characters::const_iterator it  = characters.begin();
  Characters::const_iterator end = characters.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)) == name)
      return (*it);
  }
  return (0);
}

ObjectCharacter* Level::GetCharacter(const DynamicObject* object)
{
  Characters::iterator it  = characters.begin();
  Characters::iterator end = characters.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)).GetDynamicObject() == object)
      return (*it);
  }
  return (0);
}

ObjectCharacter* Level::GetPlayer(void)
{
  if (characters.size() == 0)
    return (0);
  return (characters.front());
}

void Level::SetState(State state)
{
  level_state = state;
  if (state == Normal)
    combat.Stop();
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
    SetState(combat.GetCurrentCharacter() == 0 ? Normal : Fight);
}

AsyncTask::DoneStatus Level::do_task(void)
{
  float elapsedTime = timer.GetElapsedTime();

  if (level_ui.GetContext()->GetHoverElement() == level_ui.GetContext()->GetRootElement())
    mouse.SetCursorFromState();
  else
    mouse.SetMouseState('i');

  bool use_fog_of_war = false;
  if (use_fog_of_war == false)
    RefreshCharactersVisibility();

  if (GetPlayer() == 0)
    return (AsyncTask::DS_cont);
  camera.SlideToHeight(GetPlayer()->GetDynamicObject()->nodePath.get_z());
  camera.Run(elapsedTime);  
  mouse.Run(elapsedTime);

  std::function<void (InstanceDynamicObject*)> run_object = [elapsedTime](InstanceDynamicObject* obj) { obj->Run(elapsedTime); };
  switch (level_state)
  {
    case Fight:
      ForEach(objects,    run_object);
      ForEach(characters, [elapsedTime](ObjectCharacter* character) { character->RunFade(elapsedTime); });
      // If projectiles are moving, run them. Otherwise, run the current character
      if (projectiles.size() > 0)
        projectiles.Run(elapsedTime);
      else
      {
        ObjectCharacter* combat_character = combat.GetCurrentCharacter();

        if (combat_character)
          run_object(combat_character);
        if (mouse.Hovering().hasWaypoint && mouse.GetState() == MouseEvents::MouseAction)
          hovered_path.DisplayHoveredPath(GetPlayer(), mouse);
      }
      break ;
    case Normal:
      projectiles.Run(elapsedTime);
      time_manager.AddElapsedSeconds(elapsedTime);
      ForEach(objects,    run_object);
      ForEach(characters, run_object);
      break ;
    case Interrupted:
      break ;
  }
  zones.Refresh();
  
  if (main_script.IsDefined("Run"))
  {
    AngelScript::Type<float> param_time(elapsedTime);

    main_script.Call("Run", 1, &param_time);
  }

  floors.SetCurrentFloorFromObject(GetPlayer());
  floors.RunFadingEffect(elapsedTime);
  chatter_manager.Run(elapsedTime, camera.GetNodePath());
  particle_manager.do_particles(ClockObject::get_global_clock()->get_dt());
  timer.Restart();
  return (exit.ReadyForNextZone() ? AsyncTask::DS_done : AsyncTask::DS_cont);
}

InstanceDynamicObject* Level::FindObjectFromNode(NodePath node)
{
  function<bool (InstanceDynamicObject*)> condition = [node](InstanceDynamicObject* object) -> bool { return (*object == node); };
  auto result_object = find_if(objects.begin(), objects.end(), condition);
  
  if (result_object != objects.end())
    return (*result_object);
  else
  {
    auto result_character = find_if(characters.begin(), characters.end(), condition);
    
    if (result_character != characters.end())
      return (*result_character);
  }
  return (0);
}

void                   Level::RemoveObject(InstanceDynamicObject* object)
{
  InstanceObjects::iterator it = std::find(objects.begin(), objects.end(), object);
  
  if (it != objects.end())
  {
    world->DeleteDynamicObject((*it)->GetDynamicObject());
    delete (*it);
    objects.erase(it);
  }
}

void                   Level::UnprocessAllCollisions(void)
{
  ForEach(objects,    [](InstanceDynamicObject* object) { object->UnprocessCollisions(); });
  ForEach(characters, [](ObjectCharacter*       object) { object->UnprocessCollisions(); });
}

void                   Level::ProcessAllCollisions(void)
{
  ForEach(objects,    [](InstanceDynamicObject* object) { object->ProcessCollisions(); });
  ForEach(characters, [](ObjectCharacter*       object) { object->ProcessCollisions(); });
}

bool Level::CanGoThroughWaypoint(InstanceDynamicObject* object, unsigned int id) const
{
  InstanceObjects::const_iterator it_object;
  Characters::const_iterator      it_character;

  for (it_object = objects.begin() ; it_object != objects.end() ; ++it_object)
  {
    if (*it_object != object    && id == ((*it_object)->GetOccupiedWaypointAsInt())    && !(*it_object)->CanGoThrough(object))
      return (false);
  }
  for (it_character = characters.begin() ; it_character != characters.end() ; ++it_character)
  {
    if (*it_character == object && id == ((*it_character)->GetOccupiedWaypointAsInt()) && !(*it_character)->CanGoThrough(object))
      return (false);
  }
  return (true);
}

bool Level::IsWaypointOccupied(unsigned int id) const
{
  InstanceObjects::const_iterator it_object;
  Characters::const_iterator      it_character;
  
  for (it_object = objects.begin() ; it_object != objects.end() ; ++it_object)
  {
    if ((*it_object)->HasOccupiedWaypoint() && id == ((*it_object)->GetOccupiedWaypointAsInt()))
      return (true);
  }
  for (it_character = characters.begin() ; it_character != characters.end() ; ++it_character)
  {
    if ((*it_character)->HasOccupiedWaypoint() && id == ((*it_character)->GetOccupiedWaypointAsInt()))
      return (true);
  }
  return (false);
}

ISampleInstance* Level::PlaySound(const string& name)
{
  if (sound_manager.Require(name))
  {
    ISampleInstance* instance = sound_manager.CreateInstance(name);

    instance->Start();
    return (instance);
  }
  return (0);
}

void Level::BackupInventoriesToDynamicObjects(void)
{
  //
  // This is saving the inventories in the DynamicObject structure, thus they will be saved
  // in the World part of the save file. This information consequently needs to be processed before everything else.
  //
  for_each(characters.begin(), characters.end(), [this](ObjectCharacter* character)
  {
    Inventory& inventory = character->GetInventory();

    inventory.SaveInventory(character->GetDynamicObject());
  });
  for_each(objects.begin(), objects.end(), [this](InstanceDynamicObject* object)
  {
    ObjectShelf* shelf = object->Get<ObjectShelf>();

    if (!shelf) { shelf = object->Get<ObjectLocker>(); }
    if (shelf)
    {
      Inventory& inventory = shelf->GetInventory();

      inventory.SaveInventory(shelf->GetDynamicObject());
    }
  });
}

void Level::SerializeParties(Utils::Packet& packet)
{
  unsigned int party_count = parties.size();
  
  packet << party_count;
  for_each(parties.begin(), parties.end(), [&packet](Party* party)
  {
    party->Serialize(packet);
  });
}

void Level::UnserializeParties(Utils::Packet& packet)
{
  unsigned int party_count;
  
  packet >> party_count;
  for (unsigned int i = 0 ; i < party_count ; ++i)
  {
    Party* party = new Party;
    
    party->Unserialize(packet);
    MatchPartyToExistingCharacters(*party);
  }
}

void Level::Serialize(Utils::Packet& packet)
{
  BackupInventoriesToDynamicObjects();
  UnprocessAllCollisions();
  {
    LoadingScreen::AppendText("Recording topology...");
    GetWorld()->Serialize(packet);
    packet << (char)(combat.GetCurrentCharacter() != 0 ? State::Fight : State::Normal);
    LoadingScreen::AppendText("Recording demographic data...");
    for_each(objects.begin(),    objects.end(),    [&packet](InstanceDynamicObject* object) { object->Serialize(packet);    });
    for_each(characters.begin(), characters.end(), [&packet](ObjectCharacter* character)    { character->Serialize(packet); });
    combat.Serialize(packet);
  }
  ProcessAllCollisions();
}

void Level::Unserialize(Utils::Packet& packet)
{
  char tmpState;

  packet >> tmpState;
  level_state = (State)tmpState;
  for_each(objects.begin(),    objects.end(),    [&packet](InstanceDynamicObject* object) { object->Unserialize(packet);    });
  cout << "objects unserialized" << endl;
  for_each(characters.begin(), characters.end(), [&packet](ObjectCharacter* character)    { character->Unserialize(packet); });
  cout << "characters unserialized" << endl;
  combat.Unserialize(packet);
}
