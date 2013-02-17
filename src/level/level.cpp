#include "globals.hpp"
#include "level/level.hpp"
#include "astar.hpp"

#include "level/objects/door.hpp"
#include "level/objects/shelf.hpp"
#include <level/objects/locker.hpp>
#include <i18n.hpp>

#define AP_COST_USE             2
#define WORLDTIME_TURN          10
#define WORLDTIME_DAYLIGHT_STEP 3

// REQUIREMENT FOR THE WALL-EATING BALL OF WRATH
#include <panda3d/stencilAttrib.h>
#include <panda3d/colorBlendAttrib.h>
#include <panda3d/depthTestAttrib.h>
// END

using namespace std;

Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUse;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseObjectOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseSkillOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionTalkTo;

PT(DirectionalLight) sunlight;

Level* Level::CurrentLevel = 0;
#include <panda3d/cullFaceAttrib.h>
Level::Level(WindowFramework* window, GameUi& gameUi, Utils::Packet& packet, TimeManager& tm) : _window(window), _mouse(window),
  _camera(window, window->get_camera_group()), _timeManager(tm), _levelUi(window, gameUi)
{
  LoadingScreen* loadingScreen = new LoadingScreen(window, gameUi.GetContext());

  loadingScreen->FadeIn();
  CurrentLevel = this;
  _state       = Normal;
  _mouseState  = MouseAction;
  _persistent  = true;

  obs.Connect(_levelUi.InterfaceOpened, *this, &Level::SetInterrupted);

  loadingScreen->AppendText("-> Loading textual content");
  _items = DataTree::Factory::JSON("data/objects.json");

  _floor_lastWp = 0;
  ceilingCurrentTransparency = 1.f;

  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
    _currentUis[i] = 0;
  _currentRunningDialog = 0;
  _currentUseObjectOn   = 0;
  _currentUiLoot        = 0;
  _mouseActionBlocked   = false;

  _graphicWindow = _window->get_graphics_window();

  // TODO Implement map daylight/nodaylight system
  if (true)
  {
    _sunLight = new DirectionalLight("sun_light");
    sunlight = _sunLight;

    _sunLight->set_shadow_caster(true, 8192, 8192);
    _sunLight->get_lens()->set_near_far(10.f, 200.f);
    _sunLight->get_lens()->set_film_size(768);
    
    _sunLightNode = window->get_render().attach_new_node(_sunLight);
    _sunLightNode.set_pos(50.f, 50, 50.f);
    _sunLightNode.set_hpr(127, -31,  0);
    window->get_render().set_light(_sunLightNode);
    window->get_render().set_two_sided(false);

    TimeManager::Task* daylightTask = _timeManager.AddTask(TASK_LVL_CITY, true, 0, 1);
    daylightTask->Interval.Connect(*this, &Level::RunDaylight);
    RunDaylight();
  }

  window->get_render().set_shader_input("light", _sunLightNode);

  MouseInit();
  _timer.Restart();

  loadingScreen->AppendText("-> Loading World...");
  // WORLD LOADING
  _world = new World(window);  
  try
  {
    _world->UnSerialize(packet);
  }
  catch (unsigned int error)
  {
    loadingScreen->AppendText("/!\\ Failed to load world file");
    std::cout << "Failed to load file" << std::endl;
  }

  ForEach(_world->exitZones, [this](ExitZone& zone)
  {
    LevelExitZone* exitZone = new LevelExitZone(this, zone.destinations);
    
    exitZone->SetName(zone.name);
    ForEach(zone.waypoints, [exitZone](Waypoint* wp)
    {
      LevelExitZone* ez= exitZone; //MSVC2010: Captured variables lambda stuffs (compiler bug, I think) 
      ForEach(wp->arcs, [ez](Waypoint::Arc& arc)
      {
	arc.observer = ez;
      });
    });
    
    exitZone->ExitZone.Connect      (*this, &Level::CallbackExitZone);
    exitZone->GoToNextZone.Connect  (*this, &Level::CallbackGoToZone);
    exitZone->SelectNextZone.Connect(*this, &Level::CallbackSelectNextZone);
  });
  _exitingZone = false;

  ForEach(_world->dynamicObjects, [this](DynamicObject& dobj) { InsertDynamicObject(dobj); });
  _itCharacter = _characters.end();

  _world->SetWaypointsVisible(false);

  loadingScreen->AppendText("Loading interface");
  obs.Connect(InstanceDynamicObject::ActionUse,         *this, &Level::CallbackActionUse);
  obs.Connect(InstanceDynamicObject::ActionTalkTo,      *this, &Level::CallbackActionTalkTo);
  obs.Connect(InstanceDynamicObject::ActionUseObjectOn, *this, &Level::CallbackActionUseObjectOn);

   /*_world->AddLight(WorldLight::Directional, "toto");
   WorldLight* light    = _world->GetLightByName("toto");
   PT(DirectionalLight) slight = reinterpret_cast<DirectionalLight*>(&(*light->light));

   light->zoneSize = 1000;
   light->SetColor(255, 50, 50, 125);
   //slight->set_shadow_caster(true, 12, 12);
   //window->get_render().set_shader_input("light2", light->nodePath);
   light->nodePath.reparent_to(GetPlayer()->GetNodePath());
   _world->CompileLight(light);*/
 
//   PointLight* plight = dynamic_cast<PointLight*>(light->nodePath.node());
//   plight->get_lens()->set_near_far(1.f, 2.f);
//   plight->get_lens()->set_film_size(512);

  window->get_render().set_shader_auto();
  loadingScreen->AppendText("-- Done --");
  loadingScreen->FadeOut();
  loadingScreen->Destroy();
  delete loadingScreen;
}

void Level::InsertCharacter(ObjectCharacter* character)
{
  static unsigned short task_it = 0;
  TimeManager::Task*    task    = _timeManager.AddTask(TASK_LVL_CITY, true, 3);

  task->lastS += (task_it % 2);
  task->Interval.Connect(*character, &ObjectCharacter::CheckFieldOfView);  
  ++task_it;
  _characters.push_back(character);
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

void Level::InitPlayer(void)
{
  if (!(GetPlayer()->GetStatistics().Nil()))
  {
    Data stats(GetPlayer()->GetStatistics());
    
    if (!(stats["Statistics"]["Action Points"].Nil()))
      _levelUi.GetMainBar().SetMaxAP(stats["Statistics"]["Action Points"]);
  }
  obs_player.Connect(GetPlayer()->HitPointsChanged,         _levelUi.GetMainBar(), &GameMainBar::SetCurrentHp);
  obs_player.Connect(GetPlayer()->ActionPointChanged,       _levelUi.GetMainBar(), &GameMainBar::SetCurrentAP);
  obs_player.Connect(GetPlayer()->EquipedItemActionChanged, _levelUi.GetMainBar(), &GameMainBar::SetEquipedItemAction);
  obs_player.Connect(GetPlayer()->EquipedItemChanged,       _levelUi.GetMainBar(), &GameMainBar::SetEquipedItem);
  _levelUi.GetMainBar().EquipedItemNextAction.Connect(*GetPlayer(), &ObjectCharacter::ItemNextUseType);
  _levelUi.GetMainBar().UseEquipedItem.Connect       (*this, &Level::CallbackActionTargetUse);
  _levelUi.GetMainBar().CombatEnd.Connect            (*this, &Level::StopFight);
  _levelUi.GetMainBar().CombatPassTurn.Connect       (*this, &Level::NextTurn);
  obs.Connect(_levelUi.GetInventory().EquipItem,   *this,        &Level::PlayerEquipObject);
  obs.Connect(_levelUi.GetInventory().UnequipItem, *GetPlayer(), &ObjectCharacter::UnequipItem);
  obs.Connect(_levelUi.GetInventory().DropObject,  *this,        &Level::PlayerDropObject);
  obs.Connect(_levelUi.GetInventory().UseObject,   *this,        &Level::PlayerUseObject);
  
  _levelUi.GetMainBar().SetEquipedItem(0, GetPlayer()->GetEquipedItem(0));
  _levelUi.GetMainBar().SetEquipedItem(1, GetPlayer()->GetEquipedItem(1));
  
  //
  // The wall-eating ball of wrath
  //
  PandaNode*        node;
  CPT(RenderAttrib) atr1 = StencilAttrib::make(1, StencilAttrib::SCF_not_equal, StencilAttrib::SO_keep, StencilAttrib::SO_keep,    StencilAttrib::SO_keep,    1, 1, 0);
  CPT(RenderAttrib) atr2 = StencilAttrib::make(1, StencilAttrib::SCF_always,    StencilAttrib::SO_zero, StencilAttrib::SO_replace, StencilAttrib::SO_replace, 1, 0, 1);

  _player_halo = _window->load_model(_window->get_panda_framework()->get_models(), "misc/sphere");
  _player_halo.set_scale(5);
  _player_halo.set_bin("background", 0);
  _player_halo.set_depth_write(0);
  _player_halo.reparent_to(_window->get_render());
  node        = _player_halo.node();
  node->set_attrib(atr2);
  node->set_attrib(ColorBlendAttrib::make(ColorBlendAttrib::M_add));
  for_each(_world->floors.begin(), _world->floors.end(), [node, atr1](NodePath floor)
  {
    NodePath map_objects = floor.get_child(0);
    
    for (unsigned short i = 0 ; i < map_objects.get_num_children() ; ++i)
    {
      NodePath child = map_objects.get_child(i);

      if (child.get_name() == "Terrain")
        continue ;
      if (child.node() != node && (child.get_name().substr(0, 6) != "Ground"))
        child.set_attrib(atr1);
    }
  });  
  
  //_world->CompileLight(_world->GetLightByName("toto"));
}

void Level::InsertParty(PlayerParty& party)
{
  cout << "[Level] Insert Party" << endl;
  PlayerParty::DynamicObjects::reverse_iterator it, end;

  // Inserting progressively the last of PlayerParty's character at the beginning of the characters list
  // Player being the first of PlayerParty's list, it will also be the first of Level's character list.
  it  = party.GetObjects().rbegin();
  end = party.GetObjects().rend();
  for (; it != end ; ++it)
  {
    DynamicObject*   object    = _world->InsertDynamicObject(**it);
    ObjectCharacter* character = new ObjectCharacter(this, object);

    _characters.insert(_characters.begin(), character);
    // Replace the Party DynamicObject pointer to the new one
    delete *it;
    *it = object;
  }
  party.SetHasLocalObjects(false);
}

void Level::FetchParty(PlayerParty& party)
{
  cout << "[Level] Fetch Party" << endl;
  PlayerParty::DynamicObjects::iterator it  = party.GetObjects().begin();
  PlayerParty::DynamicObjects::iterator end = party.GetObjects().end();

  cout << "Debug: Fetch Party" << endl;
  for (; it != end ; ++it)
  {
    Characters::iterator cit    = _characters.begin();
    Characters::iterator cend   = _characters.end();

    cout << "Fetching character: " << (*it)->nodePath.get_name() << endl;
    (*it)->nodePath.set_name(GetPlayer()->GetName());
    cout << "Fetching character: " << (*it)->nodePath.get_name() << endl;
    while (cit != cend)
    {
      ObjectCharacter* character = *cit;

      cout << "--> Comparing with " << (*cit)->GetDynamicObject()->nodePath.get_name() << endl;
      if (character->GetDynamicObject()->nodePath.get_name() == (*it)->nodePath.get_name())
      {
        cout << "--> SUCCESS" << endl;
        *it = character->GetDynamicObject();
        break ;
      }
      ++cit;
    }
    cout << "My primary function is failure..." << endl;
  }
  //*it = GetPlayer()->GetDynamicObject();
  party.SetHasLocalObjects(false);
}

// Takes the party's characters out of the map.
// Backups their DynamicObject in PlayerParty because World is going to remove them anyway.
void Level::StripParty(PlayerParty& party)
{
  PlayerParty::DynamicObjects::iterator it  = party.GetObjects().begin();
  PlayerParty::DynamicObjects::iterator end = party.GetObjects().end();
  
  obs_player.DisconnectAll(); // Character is gonna get deleted: we must disconnect him.
  cout << "Debug: Strip Party" << endl;
  for (; it != end ; ++it)
  {
    DynamicObject*       backup = new DynamicObject;
    Characters::iterator cit    = _characters.begin();
    Characters::iterator cend   = _characters.end();

    *backup = **it;
    cout << "Stripping character: " << (*it)->nodePath.get_name() << endl;
    while (cit != cend)
    {
      ObjectCharacter* character = *cit;

      cout << "--> Comparing with: " << (*cit)->GetDynamicObject()->nodePath.get_name() << endl;
      if (character->GetDynamicObject() == *it)
      {
        cout << "--> SUCCESS" << endl;
        character->UnprocessCollisions();
        character->NullifyStatistics();
	delete character;
	_characters.erase(cit);
	_world->DeleteDynamicObject(*it);
	break ;
      }
      ++cit;
    }
    cout << "My primary function is failure..." << endl;
    *it = backup;
  }
  party.SetHasLocalObjects(true);
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
  _levelUi.GetInventory().SetInventory(*inventory);
  player->EquipedItemChanged.Emit(0, player->GetEquipedItem(0));
  player->EquipedItemChanged.Emit(1, player->GetEquipedItem(1));  
}

Level::~Level()
{
  _player_halo.remove_node();
  
  _timeManager.ClearTasks(TASK_LVL_CITY);
  obs.DisconnectAll();
  obs_player.DisconnectAll();
  ForEach(_objects,   [](InstanceDynamicObject* obj) { delete obj;  });
  ForEach(_exitZones, [](LevelExitZone* zone)        { delete zone; });
  CurrentLevel = 0;
  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
  {
    if (_currentUis[i] != 0)
      delete _currentUis[i];
  }
  if (_world) delete _world;
  if (_items) delete _items;
  CurrentLevel = 0;
}

bool Level::FindPath(std::list<Waypoint>& path, Waypoint& from, Waypoint& to)
{
  AstarPathfinding<Waypoint>        astar;
  int                               max_iterations = 0;
  AstarPathfinding<Waypoint>::State state;
  
  astar.SetStartAndGoalStates(from, to);
  while ((state = astar.SearchStep()) == AstarPathfinding<Waypoint>::Searching && max_iterations++ < 500);

  if (state == AstarPathfinding<Waypoint>::Succeeded)
  {
    path = astar.GetSolution();
    return (true);
  }
  return (false);  
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
  return (_characters.front());
}

void Level::SetState(State state)
{
  _state = state;
  if (state == Normal)
    _itCharacter = _characters.end();
  if (state != Fight)
  {
    DestroyCombatPath();
    ToggleCharacterOutline(false);
  }
}

void Level::SetInterrupted(bool set)
{
  for (int i = 0 ; i < UiTotalIt ; ++i)
  {
    if (_currentUis[i] && _currentUis[i]->IsVisible())
      set = true;
  }
  
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
  _levelUi.GetMainBar().SetEnabledAP(true);
  (*_itCharacter)->RestartActionPoints();
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
      list<ObjectCharacter*> listEnemies = (*it)->GetNearbyEnemies();

      if (listEnemies.size() > 0 && (*it)->IsAlive())
      {
	ConsoleWrite("You can't leave combat mode if enemies are nearby");
	return ;
      }
    }
    if (_mouseState == MouseTarget)
      SetMouseState(MouseAction);
    SetState(Normal);
    _levelUi.GetMainBar().SetEnabledAP(false);
  }
}

void Level::NextTurn(void)
{
  if (*_itCharacter == GetPlayer())
    StopFight();
  if (_state != Fight)
    return ;
  if (_itCharacter != _characters.end())
  {
    cout << "Playing animation idle" << endl;
    (*_itCharacter)->PlayAnimation("idle");
  }
  if ((++_itCharacter) == _characters.end())
  {
    _itCharacter = _characters.begin();
    _timeManager.AddElapsedTime(WORLDTIME_TURN);
  }
  if (_itCharacter != _characters.end())
    (*_itCharacter)->RestartActionPoints();
  else
    cout << "[FATAL ERROR][Level::NextTurn] Character Iterator points to nothing (n_characters = " << _characters.size() << ")" << endl;
}

void Level::RunDaylight(void)
{
  //cout << "Run Day Light" << endl;
  
  LVecBase4f color_steps[6] = {
    LVecBase4f(0.2, 0.2, 0.5, 1), // 00h00
    LVecBase4f(0.2, 0.2, 0.3, 1), // 04h00
    LVecBase4f(0.7, 0.7, 0.5, 1), // 08h00
    LVecBase4f(1.0, 1.0, 1.0, 1), // 12h00
    LVecBase4f(1.0, 0.8, 0.8, 1), // 16h00
    LVecBase4f(0.4, 0.4, 0.6, 1)  // 20h00
  };

  int it = _timeManager.GetHour() / 4;
  
  LVecBase4f to_set(0, 0, 0, 0);
  LVecBase4f dif = (it == 5 ? color_steps[0] : color_steps[it + 1]) - color_steps[it];
  
  // dif / 5 * (it % 4) -> dif / 100 * (20 * (it % 4)) is this more clear ? I hope it is.
  to_set += color_steps[it] + (dif / 5 * (it % 4));
  _sunLight->set_color(to_set);
}

extern void* mypointer;

AsyncTask::DoneStatus Level::do_task(void)
{ 
  float elapsedTime = _timer.GetElapsedTime();
  
  // TEST Mouse Cursor automatic change while hovering interfaces
  if (_levelUi.GetContext()->GetHoverElement() == _levelUi.GetContext()->GetRootElement())
    SetMouseState(_mouseState);
  else
    _mouse.SetMouseState('i');
  // TEST END

  // TEST Transparent Ball of Wrath
  if (!(_player_halo.is_empty()))
  {
    _player_halo.set_pos(GetPlayer()->GetDynamicObject()->nodePath.get_pos());
    _player_halo.set_hpr(GetPlayer()->GetDynamicObject()->nodePath.get_hpr());
  }
  // TEST End
  
  //RunDaylight(); // Quick workaround for the daylight task not working
  _camera.Run(elapsedTime);  
  //_timeManager.ExecuteTasks();
  
  switch (_state)
  {
    case Fight:
      ForEach(_objects,    [elapsedTime]      (InstanceDynamicObject* object)
      {
	object->Run(elapsedTime);
	object->UnprocessCollisions();
	object->ProcessCollisions();
      });
      for_each(_characters.begin(), _characters.end(), [this, elapsedTime](ObjectCharacter* character)
      {
	if (character == (*_itCharacter))
	  character->Run(elapsedTime);
        character->UnprocessCollisions();
        character->ProcessCollisions();
      });
      _mouse.ClosestWaypoint(_world, _currentFloor);
      if (_mouse.Hovering().hasWaypoint && _mouse.Hovering().waypoint != _last_combat_path && _mouseState == MouseAction)
        DisplayCombatPath();
      break ;
    case Normal:
      _timeManager.AddElapsedSeconds(elapsedTime);
      ForEach(_objects,    [elapsedTime](InstanceDynamicObject* object)
      {
	object->Run(elapsedTime);
	object->UnprocessCollisions();
	object->ProcessCollisions();
      });
      ForEach(_characters, [elapsedTime](ObjectCharacter* character)
      {
        character->Run(elapsedTime);
        character->UnprocessCollisions();
        character->ProcessCollisions();
      });
      break ;
    case Interrupted:
      break ;
  }
  
  CheckCurrentFloor(elapsedTime);  
  _mouse.Run();
  _timer.Restart();
  return (_exitingZone ? AsyncTask::DS_done : AsyncTask::DS_cont);
}

void Level::DestroyCombatPath(void)
{
  for_each(_combat_path.begin(), _combat_path.end(), [](Waypoint& wp) { wp.nodePath.detach_node(); });
}

void Level::DisplayCombatPath(void)
{
  _last_combat_path = _mouse.Hovering().waypoint;
  DestroyCombatPath();
  _combat_path = GetPlayer()->GetPath(_world->GetWaypointFromNodePath(_mouse.Hovering().waypoint));
  for_each(_combat_path.begin(), _combat_path.end(), [this](Waypoint& wp)
  {
    NodePath sphere = _window->load_model(_window->get_panda_framework()->get_models(), "misc/sphere");

    sphere.set_pos(wp.nodePath.get_pos());
    sphere.reparent_to(_window->get_render());
    wp.nodePath = sphere;
    wp.nodePath.set_transparency(TransparencyAttrib::M_alpha);
    wp.nodePath.set_color(0.5, 0.5, 0.5, 0.5);
    wp.nodePath.set_light_off();
    wp.nodePath.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
  });
}

void Level::TaskCeiling(float elapsedTime)
{
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

/*
 * Level Mouse
 */
void Level::MouseInit(void)
{
  SetMouseState(MouseAction);
  _mouse.ButtonLeft.Connect  (*this,   &Level::MouseLeftClicked);
  _mouse.ButtonRight.Connect (*this,   &Level::MouseRightClicked);
  _mouse.ButtonMiddle.Connect(_camera, &SceneCamera::SwapCameraView);
}

void Level::SetMouseState(MouseState state)
{
  if (state != _mouseState)
  {
    DestroyCombatPath();
    _mouseState = state;
    ToggleCharacterOutline(_state == Level::State::Fight && _mouseState == MouseTarget && *_itCharacter == GetPlayer());
  }
  switch (state)
  {
    case MouseAction:
      _mouse.SetMouseState('a');
      break ;
    case MouseInteraction:
      _mouse.SetMouseState('i');
      break ;
    case MouseTarget:
      _mouse.SetMouseState('t');
      break ;
  }
}

void Level::MouseLeftClicked(void)
{
  const MouseHovering& hovering = _mouse.Hovering();

  if (_mouseActionBlocked || _state == Interrupted)
    return ;
  if (_levelUi.GetContext()->GetHoverElement() != _levelUi.GetContext()->GetRootElement())
    return ;
  switch (_mouseState)
  {
    case MouseAction:
      if (_currentUis[UiItInteractMenu] && _currentUis[UiItInteractMenu]->IsVisible())
	return ;
      else
      {
	Waypoint* toGo;

	_mouse.ClosestWaypoint(_world, _currentFloor);
	if (hovering.hasWaypoint)
	{
	  toGo = _world->GetWaypointFromNodePath(hovering.waypoint);

	  if (toGo && _characters.size() > 0)
	    GetPlayer()->GoTo(toGo);
	}
      }
      break ;
    case MouseInteraction:
      if (hovering.hasDynObject)
      {
	InstanceDynamicObject* object = FindObjectFromNode(hovering.dynObject);

	if (_currentUis[UiItInteractMenu] && _currentUis[UiItInteractMenu]->IsVisible())
	  CloseRunningUi<UiItInteractMenu>();
	if (object && object->GetInteractions().size() != 0)
	{
	  if (_currentUis[UiItInteractMenu])
	    delete _currentUis[UiItInteractMenu];
	  _currentUis[UiItInteractMenu] = new InteractMenu(_window, _levelUi.GetContext(), *object);
	  _camera.SetEnabledScroll(false);
	}
	else
	{
	  if (!object)
	    cout << "Object does not exist" << endl;
	  else if (object->GetInteractions().size() == 0)
	    cout << "Object has no interactions" << endl;
	  else
	    cout << "This is impossible" << endl;
	}
      }
      break ;
    case MouseTarget:
      std::cout << "Mouse Target" << std::endl;
      if (hovering.hasDynObject)
      {
	InstanceDynamicObject* dynObject = FindObjectFromNode(hovering.dynObject);
	
	std::cout << "HasDynObject" << std::endl;
	if (dynObject)
	{
	  ObjectCharacter*     player    = GetPlayer();
	  InventoryObject*     item      = player->pendingActionObject;
	  unsigned char        actionIt  = player->pendingActionObjectActionIt;

	  if ((*item)["actions"][actionIt]["combat"] == "1")
	  {
	    ObjectCharacter*   target = dynObject->Get<ObjectCharacter>();

	    if (!target)
	      return ;
	    ActionUseWeaponOn(player, target, item, actionIt);
	  }
	  else
	    ActionUseObjectOn(player, dynObject, item, actionIt);
	}
      }
  }
}

void Level::MouseRightClicked(void)
{
  CloseRunningUi<UiItInteractMenu>();
  SetMouseState(_mouseState == MouseInteraction || _mouseState == MouseTarget ? MouseAction : MouseInteraction);
}

void Level::ConsoleWrite(const string& str)
{
  _levelUi.GetMainBar().AppendToConsole(str);
}

// Interactions
void Level::CallbackActionUse(InstanceDynamicObject* object)
{
  CloseRunningUi<UiItInteractMenu>();
  ActionUse(GetPlayer(), object);
}

void Level::CallbackActionBarter(ObjectCharacter* character)
{
  cout << "CallbackActionBarter" << endl;
  UiBarter* ui_barter;

  CloseRunningUi<UiItBarter>();
  if (_currentUis[UiItBarter])
    delete _currentUis[UiItBarter];
  ui_barter = new UiBarter(_window, _levelUi.GetContext(), GetPlayer(), character);
  ui_barter->BarterEnded.Connect(*this, &Level::CloseRunningUi<UiItBarter>);
  ui_barter->Show();
  _currentUis[UiItBarter] = ui_barter;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  CloseRunningUi<UiItInteractMenu>();
  if (GetState() == Fight)
  {
    ConsoleWrite(i18n::T("Can't talk during fight."));
    return ;
  }
  if ((GetPlayer()->HasLineOfSight(object)) && GetPlayer()->GetPathDistance(object) <= 3)
  {
    string dialog = object->GetDialog();

    if (_currentUis[UiItRunningDialog])
      delete _currentUis[UiItRunningDialog];

    // Player must look at its target
    GetPlayer()->LookAt(object);
    // If target is a character, then it must look back
    ObjectCharacter* talkTo = object->Get<ObjectCharacter>();
    if (talkTo)
      talkTo->LookAt(GetPlayer());
    
    _currentRunningDialog = new DialogController(_window, _levelUi.GetContext(), talkTo, i18n::GetDialogs());
    _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningUi<UiItRunningDialog>);
    _currentRunningDialog->DialogEnded.Connect(_levelUi.GetMainBar(), &UiBase::Show);
    _currentRunningDialog->StartBarter.Connect(*this, &Level::CallbackActionBarter);
    _mouseActionBlocked = true;
    _camera.SetEnabledScroll(false);
    _levelUi.GetMainBar().Hide();
    SetInterrupted(true);
    _currentUis[UiItRunningDialog] = _currentRunningDialog;
  }
  else
  {
    GetPlayer()->GoTo(object, 3);
    GetPlayer()->ReachedDestination.Connect(*this, &Level::PendingActionTalkTo);
    GetPlayer()->pendingActionOn = object;
  }
}

void Level::CallbackActionUseObjectOn(InstanceDynamicObject* target)
{
  CloseRunningUi<UiItInteractMenu>();
  if (_currentUis[UiItUseObjectOn])
    delete _currentUis[UiItUseObjectOn];
  std::cout << "CallbackActionUseObjectOn" << std::endl;
  _currentUseObjectOn = new UiUseObjectOn(_window, _levelUi.GetContext(), GetPlayer()->GetInventory());
  _currentUseObjectOn->ActionCanceled.Connect(*this, &Level::CloseRunningUi<UiItUseObjectOn>);
  _currentUseObjectOn->ObjectSelected.Connect(*this, &Level::SelectedUseObjectOn);
  _mouseActionBlocked = true;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
  GetPlayer()->pendingActionOn = target;
  _currentUis[UiItUseObjectOn] = _currentUseObjectOn;
}

void Level::PlayerLoot(Inventory* inventory)
{
  if (!inventory)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[PlayerLoot] Aborted: NullPointer Error</span>");
    return ;
  }
  CloseRunningUi<UiItInteractMenu>();
  if (_currentUis[UiItLoot])
    delete _currentUis[UiItLoot];
  _currentUiLoot = new UiLoot(_window, _levelUi.GetContext(), GetPlayer()->GetInventory(), *inventory);
  _currentUiLoot->Done.Connect(*_currentUiLoot, &UiLoot::Destroy);
  _currentUiLoot->Done.Connect(*this, &Level::CloseRunningUi<UiItLoot>);
  _mouseActionBlocked = true;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
  _currentUis[UiItLoot] = _currentUiLoot;
}

void Level::CallbackActionTargetUse(unsigned short it)
{
  ObjectCharacter* player   = GetPlayer();
  InventoryObject* object   = player->GetEquipedItem(it);
  unsigned char    actionIt = player->GetequipedAction(it);

  std::cout << "ActionTargetUse" << std::endl;
  if (object)
  {
    if ((*object)["actions"][actionIt]["targeted"] == "1")
    {
      if ((*object)["actions"][actionIt]["combat"].Value() == "1" && _state != Fight)
	StartFight(player);
      player->pendingActionObject         = object;
      player->pendingActionObjectActionIt = actionIt;
      SetMouseState(MouseTarget);
    }
    else
      ActionUseObject(player, object, actionIt);
  }
}

void Level::SelectedUseObjectOn(InventoryObject* object)
{
  ActionUseObjectOn(GetPlayer(), GetPlayer()->pendingActionOn, object, 0); // Action is default action: zero
}

void Level::PendingActionTalkTo(InstanceDynamicObject* object)
{
  CallbackActionTalkTo(object->pendingActionOn);
}

void Level::PendingActionUseObjectOn(InstanceDynamicObject* object)
{
  if (!object->pendingAnimationDone)
  {
    ObjectCharacter* user      = object->Get<ObjectCharacter>();
    unsigned short   equipedIt = 0;
    
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(*this, &Level::PendingActionUseObjectOn);
    if (user->GetEquipedItem(0) == user->pendingActionObject)
      equipedIt = 0;
    else
      equipedIt = 1;
    user->PlayEquipedItemAnimation(equipedIt, "use");
  }
  else
  {
    InventoryObject* item        = object->pendingActionObject;
    Data             dataUseCost = (*item)["use_cost"];
    unsigned short   useCost     = AP_COST_USE;

    if (!(dataUseCost.Nil()))
      useCost = dataUseCost;
    object->Get<ObjectCharacter>()->LookAt(object->pendingActionOn);
    if (UseActionPoints(useCost))
    {
      const string toOutput = item->UseOn(object->Get<ObjectCharacter>(), object->pendingActionOn, object->pendingActionObjectActionIt);

      ConsoleWrite(toOutput);
    }
    SetMouseState(MouseAction);
  }
}

void Level::PendingActionUse(InstanceDynamicObject* object)
{
  if (!object->pendingAnimationDone)
  {
    object->AnimationEnded.DisconnectAll();    
    object->AnimationEnded.Connect(*this, &Level::PendingActionUse);
    object->PlayAnimation("use");
  }
  else
  {
    object->Get<ObjectCharacter>()->LookAt(object->pendingActionOn);
    if (!(UseActionPoints(AP_COST_USE)))
      return ;
    object->pendingActionOn->CallbackActionUse(object);
    SetMouseState(MouseAction);
  }
}

void Level::ActionUse(ObjectCharacter* user, InstanceDynamicObject* target)
{
  if (!user || !target)
    return ;
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(*this, &Level::PendingActionUse);
  user->pendingActionOn = target;
}

void Level::ActionUseObject(ObjectCharacter* user, InventoryObject* object, unsigned char actionIt)
{
  if (!user || !object)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObject] Aborted: NullPointer Error</span>");
    return ;
  }
  if ((*object)["actions"].Count() <= actionIt)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObject] Invalid action iterator</span>");
    return ;
  }

  std::cout << "ActionUseObject" << std::endl;
  // Use the object naow
//   if (!(user->pendingAnimationDone))
//   {
//     user->pendingActionObject         = object;
//     user->pendingActionObjectActionIt = actionIt;
//     user->AnimationEnded.Connect(*this, &Level::PendingActionUseWeaponOn);
//     user->PlayEquipedItemAnimation(user->GetEquipedItem(0) == object ? 0 : 1);
//   }
}

void Level::ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object, unsigned char actionIt)
{
  if (!object || !target || !user)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObjectOn] Aborted: NullPointer Error</span>");
    return ;
  }
  if ((*object)["actions"].Count() <= actionIt)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObject] Invalid action iterator</span>");
    return ;
  }
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(*this, &Level::PendingActionUseObjectOn);
  user->pendingActionOn             = target;
  user->pendingActionObject         = object;
  user->pendingActionObjectActionIt = actionIt;
  if (user == GetPlayer())
    CloseRunningUi<UiItUseObjectOn>();
}

struct XpFetcher
{
  XpFetcher(ObjectCharacter* killer, ObjectCharacter* target) : killer(killer), target(target)
  {
    observerId     = target->CharacterDied.Connect(*this, &XpFetcher::CharacterDied);
    character_died = false;
  }
  
  ~XpFetcher(void)
  {
    target->CharacterDied.Disconnect(observerId);
  }

  void CharacterDied(void)
  {
    character_died = true;
  }
  
  void Execute(void)
  {
    Data            stats      = target->GetStatistics();
    Data            xp_reward;
    StatController* controller = killer->GetStatController();

    if (stats.NotNil())
      xp_reward = stats["Variable"]["XpReward"];
    if (controller)
    {
      if (xp_reward.Nil())
        controller->AddExperience(1001);
      else
        controller->AddExperience(xp_reward);
      controller->AddKill(stats["Race"].Value());
    }
  }

  ObjectCharacter         *killer, *target;
  Observatory::ObserverId observerId;
  bool                    character_died;
};

void Level::ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char actionIt)
{
  if (!(user->pendingAnimationDone))
  {
    if (user == target && target == GetPlayer())
    {
      ConsoleWrite("Stop hitting yourself !");
      return ;
    }

    user->LookAt(target);

    if (target->GetDistance(user) > (float)((*item)["actions"][actionIt]["range"]))
      ConsoleWrite("Out of range");
    else if (!(user->HasLineOfSight(target)))
      ConsoleWrite("No line of sight");
    else
    {
      unsigned int equipedIt            = 0;

      user->pendingActionObject         = item;
      user->pendingActionOn             = target;
      user->pendingActionObjectActionIt = actionIt;
      user->AnimationEnded.DisconnectAll();
      user->AnimationEnded.Connect(*this, &Level::PendingActionUseWeaponOn);
      if (user->GetEquipedItem(0))
	equipedIt = 0;
      else if (user->GetEquipedItem(1))
	equipedIt = 1;
      user->PlayEquipedItemAnimation(equipedIt, "attack");
    }
  }
  else
  {
    XpFetcher xpFetcher(user, target);
    string    output;
    
    user->SetAsEnemy(target, true);
    output = (item->UseAsWeapon(user, target, user->pendingActionObjectActionIt));
    MouseRightClicked();
    ConsoleWrite(output);
    if (xpFetcher.character_died)
    {
      xpFetcher.Execute();
      if (user->GetStatController() && target->GetStatController())
      {
        string race  = target->GetStatistics()["Race"].Value();
        Data   kills = user->GetStatistics()["Kills"];

        if (kills[race].Nil())
          kills[race] = 0;
        else
          kills[race] = (unsigned int)kills[race] + 1;
      }
    }
  }
}

void Level::PendingActionUseWeaponOn(InstanceDynamicObject* fromObject)
{
  std::cout << "PendingActionUseWeaponOn" << std::endl;
  ActionUseWeaponOn(fromObject->Get<ObjectCharacter>(),
		    fromObject->pendingActionOn->Get<ObjectCharacter>(),
		    fromObject->pendingActionObject,
		    fromObject->pendingActionObjectActionIt);
}

void Level::PlayerEquipObject(unsigned short it, InventoryObject* object)
{
  cout << "PlayerEquipObject #1" << endl;
  bool canWeildMouth        = object->CanWeild(GetPlayer(), EquipedMouth);
  bool canWeildMagic        = object->CanWeild(GetPlayer(), EquipedMagic);
  bool canWeildBattleSaddle = object->CanWeild(GetPlayer(), EquipedBattleSaddle);
  int  canWeildTotal        = (canWeildMouth ? 1 : 0) + (canWeildMagic ? 1 : 0) + (canWeildBattleSaddle ? 1 : 0);

  cout << "PlayerEquipObject #2" << endl;
  if (canWeildTotal >= 2)
  {
    cout << "PlayerEquipObject #2.1" << endl;
    UiEquipMode* ui = new UiEquipMode(_window, _levelUi.GetContext(), it, object);
    cout << "PlayerEquipObject #2.2" << endl;

    if (_currentUis[UiItEquipMode])
      delete _currentUis[UiItEquipMode];
    cout << "PlayerEquipObject #2.3" << endl;
    _currentUis[UiItEquipMode] = ui;
  cout << "PlayerEquipObject #2.4" << endl;
    ui->Closed.Connect(*this, &Level::CloseRunningUi<UiItEquipMode>);
  cout << "PlayerEquipObject #2.5" << endl;
    ui->EquipModeSelected.Connect(*GetPlayer(), &ObjectCharacter::SetEquipedItem);
  cout << "PlayerEquipObject #2.6" << endl;

    if (!canWeildMouth)        ui->DisableMode(EquipedMouth);
    if (!canWeildMagic)        ui->DisableMode(EquipedMagic);
    if (!canWeildBattleSaddle) ui->DisableMode(EquipedBattleSaddle);
  cout << "PlayerEquipObject #2.7" << endl;
  }
  else if (canWeildTotal)
  {
    GetPlayer()->SetEquipedItem(it, object, (canWeildMouth ? EquipedMouth :
                                            (canWeildMagic ? EquipedMagic : EquipedBattleSaddle)));
  }
  else
    ConsoleWrite("You can't equip " + object->GetName());
  cout << "PlayerEquipObject #3" << endl;  
}

void Level::PlayerDropObject(InventoryObject* object)
{
  ActionDropObject(GetPlayer(), object);
}

void Level::PlayerUseObject(InventoryObject* object)
{
  ActionUseObjectOn(GetPlayer(), GetPlayer(), object, 0); // Default action is 0
}

void Level::ActionDropObject(ObjectCharacter* user, InventoryObject* object)
{
  ObjectItem* item;
  Waypoint*   waypoint;

  if (!user || !object)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionDropObject] Aborted: NullPointer Error</span>");
    return ;
  }
  if (!(UseActionPoints(AP_COST_USE)))
    return ;
  waypoint = user->GetOccupiedWaypoint();
  if (waypoint)
  {
    user->GetInventory().DelObject(object);
    item     = new ObjectItem(this, object->CreateDynamicObject(_world), object);
    item->SetOccupiedWaypoint(waypoint);
    _world->DynamicObjectSetWaypoint(*(item->GetDynamicObject()), *waypoint);
    item->GetDynamicObject()->nodePath.set_pos(waypoint->nodePath.get_pos());
    _objects.push_back(item);
  }
  else
    cerr << "User has no waypoint, thus the object can't be dropped" << endl;
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

/*
 * Exit Zone Stuff
 */
void Level::CallbackExitZone(void)
{
  if (_currentUis[UiItNextZone])
    _currentUis[UiItNextZone]->Destroy();
  _exitingZone   = true;
}

void Level::CallbackGoToZone(const string& nextZone)
{
  LevelExitZone* zone = reinterpret_cast<LevelExitZone*>(GetPlayer()->GetOccupiedWaypoint()->arcs.front().observer);  
  
  if (_currentUis[UiItNextZone])
    _currentUis[UiItNextZone]->Destroy();
  _exitingZone       = true;
  _exitingZoneTo     = nextZone;
  if (zone)
    _exitingZoneName = zone->GetName();
}

void Level::CallbackSelectNextZone(const vector<string>& nextZoneChoices)
{
  if (_currentUis[UiItNextZone] && _currentUis[UiItNextZone]->IsVisible())
    return ;
  else
  {
    UiNextZone* uiNextZone = new UiNextZone(_window, _levelUi.GetContext(), nextZoneChoices);

    if (_currentUis[UiItNextZone])
      delete _currentUis[UiItNextZone];
    _camera.SetEnabledScroll(false);
    SetInterrupted(true);
    _currentUis[UiItNextZone] = uiNextZone;
    uiNextZone->Cancel.Connect          (*this, &Level::CallbackCancelSelectZone);
    uiNextZone->NextZoneSelected.Connect(*this, &Level::CallbackGoToZone);
  }
}

void Level::CallbackCancelSelectZone()
{
  CloseRunningUi<UiItNextZone>();
}

const string& Level::GetNextZone(void) const
{
  return (_exitingZoneTo);
}

const string& Level::GetExitZone(void) const
{
  return (_exitingZoneName);
}

void Level::SpawnEnemies(const std::string& type, unsigned short quantity, unsigned short n_spawn)
{
  Party          spawn_party;
  stringstream   entry_zone;
  Data           random_party = (*_dataEngine)["random-encounters"][type];
  unsigned short i            = 0; 

  entry_zone << "spawn_" << n_spawn;
  for (unsigned short ii = 0 ; ii < random_party.Count() ; ++ii)
  {
    string         critter_type = random_party[ii].Key();
    unsigned short same_critter = 0;
    
    for (; same_critter < (unsigned short)random_party[ii] && i < quantity ; ++same_critter, ++i)
    {
      Data           data         = (*_dataEngine)["bestiary"][critter_type];
      DynamicObject* object;
      string         str_model    = data["model"].Value();
      string         str_texture  = data["texture"].Value();
      
      object = _world->AddDynamicObject(type, DynamicObject::Character, str_model, str_texture);
      object->strModel  = str_model;
      object->strTexture= str_texture;
      object->charsheet = data["charsheet"].Value();
      object->dialog    = data["dialog"].Value();
      object->script    = data["script"].Value();
      InsertCharacter(new ObjectCharacter(this, object));
      spawn_party.Join(object);
      
      ObjectCharacter* tmp = (*_characters.rbegin());
      tmp->PlayAnimation("Run");
      tmp->SetAsEnemy(GetPlayer(), true); // TODO remove that when enemies flag loading is fixed      
    }
  }
  SetEntryZone(spawn_party, entry_zone.str());
}

void Level::SetEntryZone(Party& player_party, const std::string& name)
{
  EntryZone* zone               = _world->GetEntryZoneByName(name);

  if (!zone && _world->entryZones.size() > 0)
    zone = &(_world->entryZones.front());
  else if (!zone)
  {
    cout << "[Map Error] This map has no entry zones. Generating a fake entry zone." << endl;  
    _world->AddEntryZone("FakeEntryZone");
    zone = _world->GetEntryZoneByName("FakeEntryZone");
    for (unsigned int i = 1 ; i < 10 ; ++i)
    {
      Waypoint* wp = _world->GetWaypointFromId(i);

      if (wp)
        zone->waypoints.push_back(wp);
      else
	break ;
    }
  }
  if (zone)
  {
    cout << "[Level][SetEntryZone] Inserting characters into entry zone '" << zone->name << "'" << endl;
    auto party_it  = player_party.GetObjects().begin();
    auto party_end = player_party.GetObjects().end();
    
    for (; party_it != party_end ; ++party_it)
    {
      cout << "[Level][SetEntryZone] Trying to insert character " << (*party_it)->nodePath.get_name() << endl;
      list<Waypoint*>::iterator it  = zone->waypoints.begin();
      list<Waypoint*>::iterator end = zone->waypoints.end();

      for (; it != end ; ++it)
      {
	// TODO if something. Maybe this ? ->
        if (!(IsWaypointOccupied((*it)->id)))
	{
	  ObjectCharacter* character = GetCharacter(*party_it);

	  if (character)
	  {
	    cout << "[Level][SetEntryZone][" << character->GetName() << " is now on waypoint " << (*it)->id << endl;
	    (*party_it)->waypoint = *it;
	    (*party_it)->floor    = -1;
	    (*party_it)->nodePath.set_alpha_scale(1.f);
	    (*party_it)->nodePath.show();
	    character->SetOccupiedWaypoint(*it);
	    _world->DynamicObjectChangeFloor(*character->GetDynamicObject(), (*it)->floor);
	    character->GetNodePath().set_pos((*it)->nodePath.get_pos());
	    character->TruncatePath(0);
	  }
	  break ;
	}
      }
    }
  }
  _exitingZone = false;
  _camera.CenterCameraInstant(GetPlayer()->GetNodePath().get_pos());
  _camera.FollowObject(GetPlayer());
  _floor_lastWp = 0;
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

/*
 * Floors
 */
void Level::HidingFloor::SetNodePath(NodePath np)
{
  floor = np;
  floor.set_transparency(TransparencyAttrib::M_alpha);
}

void Level::HidingFloor::SetFadingIn(bool set)
{
  fadingIn = set;
  alpha    = fadingIn ? 1.f : 0.f;
  if (!fadingIn)
    floor.show();
}

void Level::HidingFloor::Run(float elapsedTime)
{
  alpha += (fadingIn ? -0.1f : 0.1f) * (elapsedTime * 10);
  done   = (fadingIn ? alpha <= 0.f : alpha >= 1.f);
  floor.set_alpha_scale(alpha);
  if (fadingIn && done)
    floor.hide();
}

void Level::FloorFade(bool in, NodePath floor)
{
  list<HidingFloor>::iterator it = std::find(_hidingFloors.begin(), _hidingFloors.end(), floor);
  HidingFloor                 hidingFloor;

  if (it == _hidingFloors.end() && (floor.is_hidden() ^ !in))
    return ;
  hidingFloor.SetNodePath(floor);
  hidingFloor.SetFadingIn(in);  
  if (it != _hidingFloors.end())
  {
    hidingFloor.ForceAlpha(it->Alpha());
    _hidingFloors.erase(it);
  }
  _hidingFloors.push_back(hidingFloor);
}

bool Level::IsInsideBuilding(unsigned char& floor)
{
  bool                      isInsideBuilding = false;
  PT(CollisionRay)          pickerRay;
  PT(CollisionNode)         pickerNode;
  NodePath                  pickerPath;
  CollisionTraverser        collisionTraverser;
  PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();
  
  pickerNode   = new CollisionNode("isInsideBuildingRay");
  pickerPath   = _window->get_render().attach_new_node(pickerNode);
  pickerRay    = new CollisionRay();
  pickerNode->add_solid(pickerRay);
  
  pickerPath.set_pos(GetPlayer()->GetNodePath().get_pos());
  pickerRay->set_origin(0, 0, 0);
  pickerRay->set_direction(0, 0, 10);
  
  collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
  collisionTraverser.traverse(_window->get_render());
  
  collisionHandlerQueue->sort_entries();

  for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry  = collisionHandlerQueue->get_entry(i);
    MapObject*      object = GetWorld()->GetMapObjectFromNodePath(entry->get_into_node_path());

    if (!object)
      object = GetWorld()->GetDynamicObjectFromNodePath(entry->get_into_node_path());
    if (object && object != GetPlayer()->GetDynamicObject())
    {
      isInsideBuilding = true;
      floor            = object->floor;
      break ;
    }
  }

  pickerPath.detach_node();
  return (isInsideBuilding);
}

void Level::SetCurrentFloor(unsigned char floor)
{
  bool          showLowerFloors  = true;
  unsigned char floorAbove       = 0;
  bool          isInsideBuilding = IsInsideBuilding(floorAbove);

  for (unsigned int it = 0 ; it < floor ; ++it)
    FloorFade(showLowerFloors, _world->floors[it]);

  for (unsigned int it = floor + 1 ; it < _world->floors.size() ; ++it)
    FloorFade(isInsideBuilding, _world->floors[it]);
  
  if (_world->floors.size() > floor)
    FloorFade(false, _world->floors[floor]);

  World::Waypoints::const_iterator cur, end;

  for (cur = _world->waypoints.begin(), end = _world->waypoints.end() ; cur != end ; ++cur)
  {
    if (cur->floor == floor)
    {
      _camera.SlideToHeight(cur->nodePath.get_pos().get_z());
      break ;
    }
  }
  
  LPoint3 upperLeft(0, 0, 0), upperRight(0, 0, 0), bottomLeft(0, 0, 0);
  _world->GetWaypointLimits(floor, upperRight, upperLeft, bottomLeft);
  //_camera.SetLimits(bottomLeft.get_x(), bottomLeft.get_y(), upperRight.get_x(), upperRight.get_y());
  _currentFloor = floor;
}

void Level::CheckCurrentFloor(float elapsedTime)
{
  ObjectCharacter* player = GetPlayer();

  if (player)
  {
    Waypoint*      wp;

    wp = player->GetDynamicObject()->waypoint;
    if (!wp)
    {
      cout << "[Level] Player doesn't have an occupied waypoint. Something went wrong somewhere" << endl;
      return ;
    }
    if (!_floor_lastWp || (wp != _floor_lastWp))
    {
      SetCurrentFloor(wp->floor);
      _floor_lastWp = wp;
    }
  }

  std::list<HidingFloor>::iterator cur, end;

  for (cur = _hidingFloors.begin(), end = _hidingFloors.end() ; cur != end ;)
  {
    cur->Run(elapsedTime);
    if (cur->Done())
      cur = _hidingFloors.erase(cur);
    else
      ++cur;
  }
}

void Level::ToggleCharacterOutline(bool toggle)
{
  for_each(_characters.begin(), _characters.end(), [this, toggle](ObjectCharacter* character)
  {
    NodePath original = character->GetDynamicObject()->nodePath;
    NodePath outline;

    if (character == GetPlayer()) return ;
    if (toggle)
    {
      if (!(character->IsAlive())) return ;
      outline = original.copy_to(original);
      outline.set_name("outline");
      outline.set_texture_off();
      outline.set_light_off();
      outline.set_attrib(RenderModeAttrib::make(RenderModeAttrib::M_wireframe));
      outline.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
      outline.set_transparency(TransparencyAttrib::M_alpha);
      if (character->IsAlly(GetPlayer()))
      { outline.set_color(0, 255, 0, 0.5); }
      else
      { outline.set_color(255, 0, 0, 0.5); }
      outline.reparent_to(original);
      outline.set_pos(0, 0, 0);
      outline.set_hpr(0, 0, 0);
      
      NodePathCollection effects = outline.find_all_matches("graphical_effect");
      
      for(unsigned short i = 0 ; i < effects.get_num_paths() ; ++i)
	effects.get_path(i).detach_node();
    }
    else
    {
      outline = original.find("outline");
      if (!(outline.is_empty()))
      { outline.detach_node(); }
    }
  });
}
