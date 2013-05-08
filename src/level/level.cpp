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

Sync::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUse;
Sync::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseObjectOn;
Sync::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseSkillOn;
Sync::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionTalkTo;

PT(DirectionalLight) workaround_sunlight;

class Circle
{
public:
  void     SetPosition(float x, float y) { this->cx = x; this->cy = y; }
  void     SetRadius(float radius) { this->radius = radius; }

  LPoint3f GetPosition(void) const
  {
    return (LPoint3f(cx, cy, 0));
  }
  
  LPoint2f PointAtAngle(float angle)
  {
    LPoint2f ret;

    ret.set_x(cx + radius * cos(angle));
    ret.set_y(cy + radius * sin(angle));
    return (ret);
  }

  void    SetFromWorld(World* world)
  {
    LPoint3 upper_left, bottom_left, upper_right;

    world->GetWaypointLimits(0, upper_right, upper_left, bottom_left);
    
    cx     = (bottom_left.get_x() + upper_right.get_x()) / 2;
    cy     = (bottom_left.get_y() + upper_right.get_y()) / 2;
    radius = upper_right.get_x() - cx;
  }

private:
  float cx, cy, radius;
};

Circle solar_circle;

#include "options.hpp"
#include <mousecursor.hpp>
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

  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
    _currentUis[i] = 0;
  _currentRunningDialog = 0;
  _currentUseObjectOn   = 0;
  _currentUiLoot        = 0;
  _mouseActionBlocked   = false;

  _graphicWindow = _window->get_graphics_window();

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
  
  // TODO Implement map daylight/nodaylight system
  if (true)
    InitSun();

  LPoint3 upperLeft, upperRight, bottomLeft;
  _world->GetWaypointLimits(0, upperRight, upperLeft, bottomLeft);
  _camera.SetLimits(bottomLeft.get_x() - 50, bottomLeft.get_y() - 50, upperRight.get_x() - 50, upperRight.get_y() - 50);  

  ForEach(_world->exitZones, [this](ExitZone& zone)
  {
    LevelExitZone* exitZone = new LevelExitZone(this, zone.destinations);
    
    cout << "Registering ExitZone '" << zone.name << '\'' << endl;
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
  obs.Connect(InstanceDynamicObject::ActionUseSkillOn,  *this, &Level::CallbackActionUseSkillOn);

  _task_metabolism = _timeManager.AddTask(TASK_LVL_CITY, true, 0, 0, 1);
  _task_metabolism->Interval.Connect(*this, &Level::RunMetabolism);  
  
  
  /*
   * DIVIDE AND CONQUER WAYPOINTS
   */
  std::vector<Waypoint*>                      entries;
  
  for_each(_world->waypoints.begin(), _world->waypoints.end(), [&entries](Waypoint& wp) { entries.push_back(&wp); });
  _world->waypoint_graph.SetHeuristic([](LPoint3f position1, LPoint3f position2) -> float
  {
    float   dist_x = position1.get_x() - position2.get_x();
    float   dist_y = position1.get_y() - position2.get_y();
    
    return (ABS(SQRT(dist_x * dist_x + dist_y * dist_y)));
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

      unsigned short block_count = 100;
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

void Level::InitSun(void)
{
  if (workaround_sunlight.is_null())
    workaround_sunlight   = new DirectionalLight("sun_light");
  _sunLight = workaround_sunlight;
  //_sunLight = new DirectionalLight("sun_light");

  _sunLightAmbient = new AmbientLight("sun_light_ambient");
  _sunLightAmbientNode = _window->get_render().attach_new_node(_sunLightAmbient);
  _window->get_render().set_light(_sunLightAmbientNode, 5);

  unsigned int shadow_caster_buffer = 128;
  unsigned int film_size            = 128;

  unsigned int graphics_quality     = OptionsManager::Get()["graphics-quality"];

  while (graphics_quality--)
  {
    shadow_caster_buffer *= 2;
    film_size            += 128;
  }

  _sunLight->set_shadow_caster(true, shadow_caster_buffer, shadow_caster_buffer);
  _sunLight->get_lens()->set_near_far(10.f, 1200.f);
  _sunLight->get_lens()->set_film_size(film_size);

  _sunLightNode = _window->get_render().attach_new_node(_sunLight);
  _sunLightNode.set_pos(150.f, 50, 50.f);
  _sunLightNode.set_hpr(127, -31,  0);
  _window->get_render().set_light(_sunLightNode, 6);
  _window->get_render().set_two_sided(false);

  _task_daylight   = _timeManager.AddTask(TASK_LVL_CITY, true, 0, 1);
  _task_daylight->Interval.Connect(*this, &Level::RunDaylight);

  solar_circle.SetFromWorld(_world);

  RunDaylight();
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
  SetupCamera();
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

    cout << "Stripping character: " << (*it)->nodePath.get_name() << endl;
    //NodePath np;
    //(**it).nodePath = np; // Nullify the nodepath first, for some reason.
    *backup = **it;
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
  _sunLightAmbientNode.remove_node();
  _sunLightNode.remove_node();
  _player_halo.remove_node();
  _window->get_render().clear_light();

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
  while ((state = astar.SearchStep()) == AstarPathfinding<Waypoint>::Searching && max_iterations++ < 50);

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
  _camera.SetEnabledScroll(state != Interrupted);
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

      if (!(listEnemies.empty()) && (*it)->IsAlive())
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
  if (_state != Fight || _currentCharacter != _itCharacter)
    return ;
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
    _timeManager.AddElapsedTime(WORLDTIME_TURN);
  }
  if (_itCharacter != _characters.end())
    (*_itCharacter)->RestartActionPoints();
  else
    cout << "[FATAL ERROR][Level::NextTurn] Character Iterator points to nothing (n_characters = " << _characters.size() << ")" << endl;
  SetupCamera();
}

void Level::SetupCamera(void)
{
  if (_state == Fight)
  {
    if  (*_itCharacter != GetPlayer())
    {
      if (OptionsManager::Get()["camera"]["fight"]["focus-enemies"].Value() == "1")
        _camera.FollowObject(*_itCharacter);
      else
        _camera.StopFollowingNodePath();
    }
    else
    {
      if (OptionsManager::Get()["camera"]["fight"]["focus-self"].Value() == "1")
        _camera.FollowObject(*_itCharacter);
      else
        _camera.StopFollowingNodePath();
    }
  }
  else
  {
    if (OptionsManager::Get()["camera"]["focus-self"].Value() == "1")
      _camera.FollowObject(GetPlayer());
  }
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

void Level::RunDaylight(void)
{
  //cout << "Running daylight task" << endl;
  if (_sunLightNode.is_empty())
    return ;
  
  LVecBase4f color_steps[6] = {
    LVecBase4f(0.2, 0.2, 0.5, 1), // 00h00
    LVecBase4f(0.2, 0.2, 0.3, 1), // 04h00
    LVecBase4f(0.7, 0.7, 0.5, 1), // 08h00
    LVecBase4f(1.0, 1.0, 1.0, 1), // 12h00
    LVecBase4f(1.0, 0.8, 0.8, 1), // 16h00
    LVecBase4f(0.4, 0.4, 0.6, 1)  // 20h00
  };
  
  int   current_hour    = _task_daylight->lastH + _task_daylight->timeH;
  int   current_minute  = _task_daylight->lastM + _task_daylight->timeM;
  int   current_second  = _task_daylight->lastS + _task_daylight->timeS;
  int   it              = current_hour / 4;
  float total_seconds   = 60 * 60 * 4;
  float elapsed_seconds = current_second + (current_minute * 60) + ((current_hour - (it * 4)) * 60 * 60);

  LVecBase4f to_set(0, 0, 0, 0);
  LVecBase4f dif = (it == 5 ? color_steps[0] : color_steps[it + 1]) - color_steps[it];

  // dif / 5 * (it % 4) -> dif / 100 * (20 * (it % 4)) is this more clear ? I hope it is.
  to_set += color_steps[it] + (dif / total_seconds * elapsed_seconds);
  _sunLight->set_color(to_set);
  to_set.set_w(0.1);
  _sunLightAmbient->set_color(to_set / 3);
  //cout << "Sunlight color [" << _timeManager.GetHour() << "]->[" << it << "]: " << to_set.get_x() << "," << to_set.get_y() << "," << to_set.get_z() << endl;

  // Angle va de 0/180 de 8h/20h à 20h/8h
  float    step  = (current_hour) % 12 + (current_minute / 60.f);
  float    angle = ((180.f / 120.f) / 12.f) * step;
  LPoint2f pos   = solar_circle.PointAtAngle(angle);

  //cout << "Sun Step: " << step << endl;
  //cout << "Sun Position is (" << pos.get_x() << "," << pos.get_y() << ')' << endl;
  _sunLightNode.set_z(pos.get_x());
  _sunLightNode.set_y(pos.get_y());
  _sunLightNode.look_at(solar_circle.GetPosition());
}

AsyncTask::DoneStatus Level::do_task(void)
{ 
  float elapsedTime = _timer.GetElapsedTime();

  if (_levelUi.GetContext()->GetHoverElement() == _levelUi.GetContext()->GetRootElement())
    SetMouseState(_mouseState);
  else
    _mouse.SetMouseState('i');

  // TEST Transparent Ball of Wrath
  if (!(_player_halo.is_empty()))
  {
    _player_halo.set_pos(GetPlayer()->GetDynamicObject()->nodePath.get_pos());
    _player_halo.set_hpr(GetPlayer()->GetDynamicObject()->nodePath.get_hpr());
  }
  // TEST End

  _camera.Run(elapsedTime);  
  _mouse.ClosestWaypoint(_world, _currentFloor);
  if (_mouse.Hovering().hasWaypoint)
  {
    unsigned int waypoint_id = _mouse.Hovering().waypoint_ptr->id;
    
    if (_world->IsInExitZone(waypoint_id))
      MouseCursor::Get()->SetHint("exit");
    else
      MouseCursor::Get()->SetHint("");
  }
  else
    MouseCursor::Get()->SetHint("nowhere");

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
      _currentCharacter = _itCharacter; // Keep a character from askin NextTurn several times
      if (_itCharacter != _characters.end())
        run_object(*_itCharacter);
      if (_mouse.Hovering().hasWaypoint && _mouse.Hovering().waypoint != _last_combat_path && _mouseState == MouseAction)
        DisplayCombatPath();
      break ;
    case Normal:
      _timeManager.AddElapsedSeconds(elapsedTime);
      ForEach(_objects,    run_object);
      ForEach(_characters, run_object);
      break ;
    case Interrupted:
      break ;
  }
  
  CheckCurrentFloor(elapsedTime);  
  _mouse.Run();
  _timer.Restart();
  return (_exitingZone ? AsyncTask::DS_done : AsyncTask::DS_cont);
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
	_mouse.ClosestWaypoint(_world, _currentFloor);
	if (hovering.hasWaypoint)
	{
	  Waypoint* toGo = _world->GetWaypointFromNodePath(hovering.waypoint);

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
	  InventoryObject*     item      = player->active_object;
	  unsigned char        actionIt  = player->active_object_it;

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
    ObjectItem* item;

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
  if (!((_currentUis[UiItNextZone] && _currentUis[UiItNextZone]->IsVisible())))
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
