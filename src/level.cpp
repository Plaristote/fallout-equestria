#include "globals.hpp"
#include "level.hpp"

#include "npc.hpp"

using namespace std;

Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUse;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseObjectOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseSkillOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionTalkTo;  

Level* Level::CurrentLevel = 0;

Level::Level(WindowFramework* window, const std::string& filename) : _window(window), _mouse(window),
  _camera(window, window->get_camera_group()), _gameUi(window)
{
  CurrentLevel = this;

  _l18n = DataTree::Factory::JSON("data/l18n/english.json");

  ceilingCurrentTransparency = 1.f;

  InstanceDynamicObject::ActionUse.Connect   (*this, &Level::CallbackActionUse);
  InstanceDynamicObject::ActionTalkTo.Connect(*this, &Level::CallbackActionTalkTo);
  _currentInteractMenu = 0;
  _currentRunningDialog = 0;

  _graphicWindow = _window->get_graphics_window();
  AsyncTaskManager::get_global_ptr()->add(this);

  _sunLight = new DirectionalLight("sun_light");

  _sunLight->set_color(LVecBase4f(0.8, 0.8, 0.8, 1));
  //_sunLight->set_shadow_caster(true, 512, 512);
  _sunLight->get_lens()->set_near_far(1.f, 2.f);
  _sunLight->get_lens()->set_film_size(512);

  _sunLightNode = window->get_render().attach_new_node(_sunLight);
  _sunLightNode.set_hpr(-30, -80, 0);
  window->get_render().set_light(_sunLightNode);
  window->get_render().set_shader_input("light", _sunLightNode);

  MouseInit();
  _timer.Restart();
  
  // WORLD LOADING
  _world = new World(window);  
  std::ifstream file;
  std::string   fullpath = "maps/" + filename + ".blob";

  file.open(fullpath.c_str());
  if (file.is_open())
  {
    try
    {
      long  begin, end;
      long  size;
      char* raw;

      begin     = file.tellg();
      file.seekg(0, std::ios::end);
      end       = file.tellg();
      file.seekg(0, std::ios::beg);
      size      = end - begin;
      raw       = new char[size + 1];
      file.read(raw, size);
      file.close();
      raw[size] = 0;

      Utils::Packet* packet;

      packet = new Utils::Packet(raw, size);

      _world->UnSerialize(*packet);

      delete   packet;
      delete[] raw;
    }
    catch (unsigned int error)
    {
      std::cout << "Failed to load file" << std::endl;
    }
  }
  else
    std::cout << "Blob file not found" << std::endl;
  
  World::DynamicObjects::iterator it  = _world->dynamicObjects.begin();
  World::DynamicObjects::iterator end = _world->dynamicObjects.end();

  for_each(it, end, [this](DynamicObject& object)
  {
    InstanceDynamicObject* instance = 0;
    
    switch (object.type)
    {
      case DynamicObject::Character:
        {
	  ObjectCharacter* character = new ObjectCharacter(this, &object);
	  
	  _characters.push_back(character);
	  instance = character;
        }
	break ;
      case DynamicObject::Door:
	instance = new ObjectDoor(this, &object);
	break ;
      case DynamicObject::Shelf:
	break ;
      case DynamicObject::Locker:
	break ;
    }
    if (instance != 0)
      _objects.push_back(instance);
  });
  
  _world->SetWaypointsVisible(false);
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

AsyncTask::DoneStatus Level::do_task(void)
{
  float elapsedTime = _timer.GetElapsedTime();

  // TEST SUNLIGHT MOONLIGHT
  {
    static Timer         lightTimer;
    static unsigned char dayStep    = 1;
    static unsigned int  stepLength = 10;

    LVecBase4f           colorSteps[3];

    colorSteps[0] = LVecBase4f(0.2, 0.2, 0.2, 1);
    colorSteps[1] = LVecBase4f(0.8, 0.8, 0.8, 1);
    colorSteps[2] = LVecBase4f(0.6, 0.3, 0.3, 1);

    if (lightTimer.GetElapsedTime() < stepLength)
    {
      LVecBase4f toSet = _sunLight->get_color();
      LVecBase4f dif   = colorSteps[(dayStep == 2 ? 0 : dayStep + 1)] - colorSteps[dayStep];

      toSet += ((dif * elapsedTime) / stepLength);
      _sunLight->set_color(toSet);
    }
    else
    {
      cout << "Next step" << endl;
      dayStep++;
      if (dayStep >= 3)
        dayStep = 0;
      _sunLight->set_color(colorSteps[dayStep]);
      lightTimer.Restart();
    }
  }
  
  _mouse.Run();
  _camera.Run(elapsedTime);

  for_each(_objects.begin(), _objects.end(),       [elapsedTime](InstanceDynamicObject* object) { object->Run(elapsedTime); });
  for_each(_characters.begin(), _characters.end(), [](ObjectCharacter* character)               { character->UnprocessCollisions(); });
  for_each(_characters.begin(), _characters.end(), [](ObjectCharacter* character)               { character->ProcessCollisions(); });
  //_tilemap.ResetPathfinding();
  TaskCeiling(elapsedTime);
  _timer.Restart();
  return AsyncTask::DS_cont;
}

void Level::TaskCeiling(float elapsedTime)
{
  /*MapElement::Position        playerPos = (*(_characters.begin()))->GetPosition();
  const Tilemap::CeilingTile& ceiling   =_tilemap.GetCeiling(playerPos.x, playerPos.y);

  if (ceiling.isDefined && ceilingCurrentTransparency >= 0.f)
  {
    if (ceilingCurrentTransparency >= 1.f)
      ceilingCurrentTransparency = 1.f;
    ceilingCurrentTransparency -= 1.f * elapsedTime;
    _tilemap.SetCeilingTransparent(ceilingCurrentTransparency);
  }
  else if (!ceiling.isDefined && ceilingCurrentTransparency <= 1.f)
  {
    ceilingCurrentTransparency += 1.f * elapsedTime;
    _tilemap.SetCeilingTransparent(ceilingCurrentTransparency);
  }*/
}

/*
 * Nodes Management
 */
InstanceDynamicObject* Level::FindObjectFromNode(NodePath node)
{
  InstanceObjects::iterator cur = _objects.begin();
  
  while (cur != _objects.end())
  {
    if ((**cur) == node)
      return (*cur);
    ++cur;
  }
  return (0);
}

/*
 * Level Mouse
 */
void Level::MouseInit(void)
{
  _mouseState    = MouseAction;
  _mouse.ButtonLeft.Connect  (*this,   &Level::MouseLeftClicked);
  _mouse.ButtonRight.Connect (*this,   &Level::MouseRightClicked);
  _mouse.ButtonMiddle.Connect(_camera, &SceneCamera::SwapCameraView);
}

void Level::MouseLeftClicked(void)
{
  const MouseHovering& hovering = _mouse.Hovering();
  
  cout << "Mouse Left Clicked" << endl;
  switch (_mouseState)
  {
    case MouseAction:
      cout << "Mouse Action" << endl;
      if (hovering.hasDynObject && false)
      {
        // Do something, or not...
      }
      else if (hovering.hasWaypoint)
      {
	Waypoint* toGo = _world->GetWaypointFromNodePath(hovering.waypoint);

	if (toGo)
	{
	  if (_characters.size() > 0)
	    _characters.front()->GoTo(toGo);
	}
      }
      break ;
    case MouseInteraction:
      cout << "Mouse Interaction" << endl;
      if (hovering.hasDynObject)
      {
	InstanceDynamicObject* object = FindObjectFromNode(hovering.dynObject);

	if (_currentInteractMenu)
	  delete _currentInteractMenu;
	if (object && object->GetInteractions().size() != 0)
	{
	  _currentInteractMenu = new InteractMenu(_window, *object);	  
	  _camera.SetEnabledScroll(false);
	}
      }
      break ;
  }
}

void Level::MouseRightClicked(void)
{
  cout << "Changed mouse state" << endl;
  CloseInteractMenu();
  _mouseState = (_mouseState == MouseAction ? MouseInteraction : MouseAction);
}

/*
 * InteractMenu
 */
void Level::CloseInteractMenu(void)
{
  if (_currentInteractMenu)
  {
    delete _currentInteractMenu;
    _currentInteractMenu = 0;
  }
  _camera.SetEnabledScroll(true);
}

void Level::ConsoleWrite(const string& str)
{
  _gameUi.GetMainBar().AppendToConsole(str);
}

// Interactions
void Level::CallbackActionUse(InstanceDynamicObject* object)
{
  //object->InteractUse(*(_characters.begin()));
  CloseInteractMenu();
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  CloseInteractMenu();
  if ((_characters.front()->HasLineOfSight(object)) && _characters.front()->GetPathDistance(object) <= 3)
  {
    string dialog = object->GetDialog();

    if (_currentRunningDialog)
      delete _currentRunningDialog;
    _currentRunningDialog = new DialogController(_window, _gameUi.GetContext(), dialog, _l18n);
    _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningDialog);
    _camera.SetEnabledScroll(false);
  }
  else
  {
    _characters.front()->GoTo(object, 3);
    _characters.front()->ReachedDestination.Connect(*this, &Level::CallbackActionTalkTo);
  }
}

void Level::CloseRunningDialog(void)
{
  if (_currentRunningDialog)
  {
    _currentRunningDialog->Destroy();
    //delete _currentRunningDialog;
    _currentRunningDialog = 0;
  }
  _camera.SetEnabledScroll(true);
}