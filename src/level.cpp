#include "globals.hpp"
#include "level.hpp"
#include "astar.hpp"

#include "objects/door.hpp"
#include "objects/shelf.hpp"

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
  _state       = Normal;
  
  _gameUi.InterfaceOpened.Connect(*this, &Level::SetInterrupted);

  _l18n  = DataTree::Factory::JSON("data/l18n/english.json");
  _items = DataTree::Factory::JSON("data/objects.json");

  ceilingCurrentTransparency = 1.f;

  InstanceDynamicObject::ActionUse.Connect        (*this, &Level::CallbackActionUse);
  InstanceDynamicObject::ActionTalkTo.Connect     (*this, &Level::CallbackActionTalkTo);
  InstanceDynamicObject::ActionUseObjectOn.Connect(*this, &Level::CallbackActionUseObjectOn);
  _currentInteractMenu  = 0;
  _currentRunningDialog = 0;
  _currentUseObjectOn   = 0;
  _mouseActionBlocked   = false;

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

  file.open(fullpath.c_str(), ios::binary);
  if (file.is_open())
  {
    try
    {
      Utils::Packet* packet;
      long           begin, end;
      long           size;
      char*          raw;

      begin     = file.tellg();
      file.seekg(0, std::ios::end);
      end       = file.tellg();
      file.seekg(0, std::ios::beg);
      size      = end - begin;
      raw       = new char[size + 1];
      file.read(raw, size);
      file.close();
      raw[size] = 0;

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
        }
	break ;
      case DynamicObject::Door:
	instance = new ObjectDoor(this, &object);
	break ;
      case DynamicObject::Shelf:
	instance = new ObjectShelf(this, &object);
	break ;
      case DynamicObject::Locker:
	break ;
    }
    if (instance != 0)
      _objects.push_back(instance);
  });
  _itCharacter = _characters.end();

  _world->SetWaypointsVisible(false);

  InventoryObject* object = new InventoryObject(Data(_items)["Key"]);

  GetPlayer()->GetInventory().AddObject(object);
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
  Characters::iterator it  = _characters.begin();
  Characters::iterator end = _characters.end();

  for (; it != end ; ++it)
  {
    if ((*(*it)) == name)
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
}

void Level::SetInterrupted(bool set)
{
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
  (*_itCharacter)->RestartActionPoints();
  SetState(Fight);
}

void Level::StopFight(void)
{
  if (_state == Fight)
  {
    // Check if no hostiles are around
    SetState(Normal);
  }
}

void Level::NextTurn(void)
{
  if (_state != Fight)
    return ;
  std::cout << "Next Turn" << std::endl;
  if ((++_itCharacter) == _characters.end())
    _itCharacter = _characters.begin();
  (*_itCharacter)->RestartActionPoints();
}

AsyncTask::DoneStatus Level::do_task(void)
{
  // FPS COUNTER NO PSTAT
  {
    static Timer          timer;
    static unsigned short fps = 0;

    if (timer.GetElapsedTime() > 1.f)
    {
      cout << "[FPS] " << fps << endl;
      fps = 0;
      timer.Restart();
    }
    fps++;
  }
  
  float elapsedTime = _timer.GetElapsedTime();

  // TEST SUNLIGHT MOONLIGHT
  /*{
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
  }*/
  
  _mouse.Run();
  _camera.Run(elapsedTime);

  switch (_state)
  {
    case Fight:
      for_each(_objects.begin(), _objects.end(),       [elapsedTime](InstanceDynamicObject* object) { object->Run(elapsedTime); });
      (*_itCharacter)->Run(elapsedTime);
      break ;
    case Normal:
      for_each(_objects.begin(), _objects.end(),       [elapsedTime](InstanceDynamicObject* object) { object->Run(elapsedTime); });
      for_each(_characters.begin(), _characters.end(), [elapsedTime](ObjectCharacter* character)
      {
	character->Run(elapsedTime);
	character->UnprocessCollisions();
	character->ProcessCollisions();
      });
      break ;
    case Interrupted:
      break ;
  }
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
  if (_mouseActionBlocked || _state == Interrupted)
    return ;
  switch (_mouseState)
  {
    case MouseAction:
      cout << "Mouse Action" << endl;
      if (_currentInteractMenu)
	return ;
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
	    GetPlayer()->GoTo(toGo);
	}
      }
      break ;
    case MouseInteraction:
      cout << "Mouse Interaction" << endl;
      if (hovering.hasDynObject)
      {
	InstanceDynamicObject* object = FindObjectFromNode(hovering.dynObject);

	if (_currentInteractMenu)
	{
	  delete _currentInteractMenu;
	  _currentInteractMenu = 0;
	}
	if (object && object->GetInteractions().size() != 0)
	{
	  _currentInteractMenu = new InteractMenu(_window, *object);	  
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
  CloseInteractMenu();
  ActionUse(GetPlayer(), object);
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  CloseInteractMenu();
  if ((GetPlayer()->HasLineOfSight(object)) && GetPlayer()->GetPathDistance(object) <= 3)
  {
    string dialog = object->GetDialog();

    if (_currentRunningDialog)
      delete _currentRunningDialog;
    _currentRunningDialog = new DialogController(_window, _gameUi.GetContext(), dialog, _l18n);
    _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningDialog);
    _mouseActionBlocked = true;
    _camera.SetEnabledScroll(false);
    SetInterrupted(true);
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
  InventoryObject* object;
  
  CloseInteractMenu();
  if (_currentUseObjectOn)
    delete _currentUseObjectOn;
  _currentUseObjectOn = new UiUseObjectOn(_window, _gameUi.GetContext(), GetPlayer()->GetInventory());
  _currentUseObjectOn->ActionCanceled.Connect(*this, &Level::CloseUseObjectOn);
  _currentUseObjectOn->ObjectSelected.Connect(*this, &Level::SelectedUseObjectOn);
  _mouseActionBlocked = true;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
  GetPlayer()->pendingActionOn = target;
}

void Level::SelectedUseObjectOn(InventoryObject* object)
{
  ActionUseObjectOn(GetPlayer(), GetPlayer()->pendingActionOn, object);
}

void Level::PendingActionTalkTo(InstanceDynamicObject* object)
{
  CallbackActionTalkTo(object->pendingActionOn);
}

void Level::PendingActionUseObjectOn(InstanceDynamicObject* object)
{
  const string toOutput = object->pendingActionObject->UseOn(object->Get<ObjectCharacter>(), object->pendingActionOn);
  
  ConsoleWrite(toOutput);
}

void Level::PendingActionUse(InstanceDynamicObject* object)
{
  object->pendingActionOn->CallbackActionUse(object);
}

void Level::ActionUse(ObjectCharacter* user, InstanceDynamicObject* target)
{
  if (!user || !target)
    return ;
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(*this, &Level::PendingActionUse);
  user->pendingActionOn = target;
}

void Level::ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object)
{
  if (!object || !target || !user)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObjectOn] Aborted: NullPointer Error</span>");
    return ;
  }

  user->GoTo(target, 0);
  user->ReachedDestination.Connect(*this, &Level::PendingActionUseObjectOn);
  user->pendingActionOn     = target;
  user->pendingActionObject = object;
  if (user == GetPlayer())
    CloseUseObjectOn();
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
  user->GetInventory().DelObject(object);
  waypoint = user->GetOccupiedWaypoint();
  item     = new ObjectItem(this, object->CreateDynamicObject(_world), object);
  item->SetOccupiedWaypoint(waypoint);
  item->GetDynamicObject()->nodePath.set_pos(waypoint->nodePath.get_pos());
  _objects.push_back(item);
}

void Level::CloseRunningDialog(void)
{
  if (_currentRunningDialog)
    _currentRunningDialog->Destroy();
  _camera.SetEnabledScroll(true);
  _mouseActionBlocked = false;
  SetInterrupted(false);
}

void Level::CloseUseObjectOn(void)
{
  if (_currentUseObjectOn)
    _currentUseObjectOn->Destroy();
  _camera.SetEnabledScroll(true);
  _mouseActionBlocked = false;
  SetInterrupted(false);
}