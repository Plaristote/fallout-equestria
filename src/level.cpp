#include "globals.hpp"
#include "level.hpp"
#include "astar.hpp"

#include "objects/door.hpp"
#include "objects/shelf.hpp"

#define AP_COST_USE             2
#define WORLDTIME_TURN          10
#define WORLDTIME_DAYLIGHT_STEP 3

using namespace std;

Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUse;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseObjectOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionUseSkillOn;
Observatory::Signal<void (InstanceDynamicObject*)> InstanceDynamicObject::ActionTalkTo;  

Level* Level::CurrentLevel = 0;

Level::Level(WindowFramework* window, AsyncTask& task, Utils::Packet& packet) : _window(window), _asyncTask(task), _mouse(window),
  _camera(window, window->get_camera_group()), _gameUi(window)
{
  AsyncTaskManager::get_global_ptr()->add(&_asyncTask);  
  CurrentLevel = this;
  _state       = Normal;
  
  _gameUi.InterfaceOpened.Connect(*this, &Level::SetInterrupted);

  _l18n  = DataTree::Factory::JSON("data/l18n/english.json");
  _items = DataTree::Factory::JSON("data/objects.json");

  ceilingCurrentTransparency = 1.f;

  InstanceDynamicObject::ActionUse.Connect        (*this, &Level::CallbackActionUse);
  InstanceDynamicObject::ActionTalkTo.Connect     (*this, &Level::CallbackActionTalkTo);
  InstanceDynamicObject::ActionUseObjectOn.Connect(*this, &Level::CallbackActionUseObjectOn);
  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
    _currentUis[i] = 0;
  _currentInteractMenu  = 0;
  _currentRunningDialog = 0;
  _currentUseObjectOn   = 0;
  _currentUiLoot        = 0;
  _mouseActionBlocked   = false;

  _graphicWindow = _window->get_graphics_window();

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
  try
  {
    _world->UnSerialize(packet);
  }
  catch (unsigned int error)
  {
    std::cout << "Failed to load file" << std::endl;
  }

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
//   GetPlayer()->SetEquipedItem(0, object);
//   GetPlayer()->UnequipItem(0);
  if (GetPlayer()->GetStatistics())
  {
    Data stats(GetPlayer()->GetStatistics());
    
    if (!(stats["Statistics"]["Action Points"].Nil()))
      _gameUi.GetMainBar().SetMaxAP(stats["Statistics"]["Action Points"]);
    GetPlayer()->ActionPointChanged.Connect(_gameUi.GetMainBar(), &GameMainBar::SetCurrentAP);
    GetPlayer()->EquipedItemChanged.Connect(_gameUi.GetMainBar(), &GameMainBar::SetEquipedItem);
    GetPlayer()->EquipedItemChanged.Emit(0, GetPlayer()->GetEquipedItem(0));
    GetPlayer()->EquipedItemChanged.Emit(1, GetPlayer()->GetEquipedItem(1));
  }
  _gameUi.GetMainBar().UseEquipedItem.Connect(*this, &Level::CallbackActionTargetUse);
  _gameUi.GetMainBar().CombatEnd.Connect     (*this, &Level::StopFight);
  _gameUi.GetMainBar().CombatPassTurn.Connect(*this, &Level::NextTurn);
  _gameUi.GetInventory().SetInventory(GetPlayer()->GetInventory());
  _gameUi.GetInventory().EquipItem.Connect   (*this, &Level::PlayerEquipObject);
  _gameUi.GetInventory().UnequipItem.Connect (*GetPlayer(), &ObjectCharacter::UnequipItem);
  _gameUi.GetInventory().DropObject.Connect  (*this, &Level::PlayerDropObject);
  _gameUi.GetInventory().UseObject.Connect   (*this, &Level::PlayerUseObject);
  
  TimeManager::Task* daylightTask = _timeManager.AddTask(true, 3);
  daylightTask->Interval.Connect(*this, &Level::RunDaylight);

  unsigned int taskIt = 0;
  std::for_each(_characters.begin(), _characters.end(), [this, &taskIt](ObjectCharacter* character)
  {
    if (character != GetPlayer())
    {
      TimeManager::Task* task = _timeManager.AddTask(true, 3);

      task->lastS += (taskIt % 3);
      ++taskIt;
      task->Interval.Connect(*character, &ObjectCharacter::CheckFieldOfView);
    }
  });
}

Level::~Level()
{
  AsyncTaskManager::get_global_ptr()->remove(&_asyncTask);
  std::for_each(_objects.begin(), _objects.end(), [](InstanceDynamicObject* obj) { delete obj; });
  CurrentLevel = 0;
  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
  {
    if (_currentUis[i] != 0)
      delete _currentUis[i];
  }
  if (_currentInteractMenu)
    delete _currentInteractMenu;
  if (_l18n)
    delete _l18n;
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
  _gameUi.GetMainBar().SetEnabledAP(true);
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
      list<ObjectCharacter::FovEnemy> listEnemies = (*it)->GetNearbyEnemies();

      if (listEnemies.size() > 0 && (*it)->IsAlive())
      {
	ConsoleWrite("You can't leave combat mode if enemies are nearby");
	return ;
      }
    }
    SetState(Normal);
    _gameUi.GetMainBar().SetEnabledAP(false);
  }
}

void Level::NextTurn(void)
{
  if (_state != Fight)
    return ;
  std::cout << "Next Turn" << std::endl;
  (*_itCharacter)->PlayAnimation("idle");
  if ((++_itCharacter) == _characters.end())
  {
    _itCharacter = _characters.begin();
    _timeManager.AddElapsedTime(WORLDTIME_TURN);
  }
  (*_itCharacter)->RestartActionPoints();
}

void Level::RunDaylight(void)
{
  static Timer         lightTimer;
  static unsigned char dayStep    = 1;
  static unsigned int  stepLength = 60;

  LVecBase4f           colorSteps[3];

  colorSteps[0] = LVecBase4f(0.2, 0.2, 0.2, 1);
  colorSteps[1] = LVecBase4f(0.8, 0.8, 0.8, 1);
  colorSteps[2] = LVecBase4f(0.6, 0.3, 0.3, 1);

  if (lightTimer.GetElapsedTime() < stepLength)
  {
    LVecBase4f toSet = _sunLight->get_color();
    LVecBase4f dif   = colorSteps[(dayStep == 2 ? 0 : dayStep + 1)] - colorSteps[dayStep];

    toSet += ((dif * WORLDTIME_DAYLIGHT_STEP) / stepLength);
    _sunLight->set_color(toSet);
  }
  else
  {
    cout << "Daylight Next step" << endl;
    dayStep++;
    if (dayStep >= 3)
      dayStep = 0;
    _sunLight->set_color(colorSteps[dayStep]);
    lightTimer.Restart();
  }
}

AsyncTask::DoneStatus Level::do_task(void)
{ 
  float elapsedTime = _timer.GetElapsedTime();

  _mouse.Run();
  _camera.Run(elapsedTime);
  _timeManager.ExecuteTasks();

  switch (_state)
  {
    case Fight:
      for_each(_objects.begin(), _objects.end(),       [elapsedTime](InstanceDynamicObject* object) { object->Run(elapsedTime); });
      (*_itCharacter)->Run(elapsedTime);
      break ;
    case Normal:
      _timeManager.AddElapsedSeconds(elapsedTime);
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
  _timer.Restart();
  return (AsyncTask::DS_cont);
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
  _mouseState = state;
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
    case MouseTarget:
      cout << "Mouse Target" << endl;
      if (hovering.hasDynObject)
      {
	InstanceDynamicObject* dynObject = FindObjectFromNode(hovering.dynObject);
	
	if (dynObject)
	{
	  ObjectCharacter*     player    = GetPlayer();
	  InventoryObject*     item      = player->pendingActionObject;

	  if ((*item)["combat"] == "1")
	  {
	    ObjectCharacter*   target = dynObject->Get<ObjectCharacter>();

	    if (!target)
	      return ;
	    ActionUseWeaponOn(player, target, item);
	  }
	  else
	    ActionUseObjectOn(player, dynObject, item);
	}
      }
  }
}

void Level::MouseRightClicked(void)
{
  cout << "Changed mouse state" << endl;
  CloseInteractMenu();
  SetMouseState(_mouseState == MouseInteraction || _mouseState == MouseTarget ? MouseAction : MouseInteraction);
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
  if (GetState() == Fight)
    return ;
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
    
    _currentRunningDialog = new DialogController(_window, _gameUi.GetContext(), dialog, _l18n);
    _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningUi<UiItRunningDialog>);
    _mouseActionBlocked = true;
    _camera.SetEnabledScroll(false);
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
  InventoryObject* object;
  
  CloseInteractMenu();
  if (_currentUis[UiItUseObjectOn])
    delete _currentUis[UiItUseObjectOn];
  _currentUseObjectOn = new UiUseObjectOn(_window, _gameUi.GetContext(), GetPlayer()->GetInventory());
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
  CloseInteractMenu();
  if (_currentUis[UiItLoot])
    delete _currentUis[UiItLoot];
  _currentUiLoot = new UiLoot(_window, _gameUi.GetContext(), GetPlayer()->GetInventory(), *inventory);
  _currentUiLoot->Done.Connect(*_currentUiLoot, &UiLoot::Destroy);
  _currentUiLoot->Done.Connect(*this, &Level::CloseRunningUi<UiItLoot>);
  _mouseActionBlocked = true;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
  _currentUis[UiItLoot] = _currentUiLoot;
}

void Level::CallbackActionTargetUse(unsigned short it)
{
  ObjectCharacter* player = GetPlayer();
  InventoryObject* object = player->GetEquipedItem(it);
  
  if (object)
  {
    if ((*object)["combat"].Value() == "1" && _state != Fight)
      StartFight(player);
    player->pendingActionObject = object;
    SetMouseState(MouseTarget);
  }
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
      const string toOutput = item->UseOn(object->Get<ObjectCharacter>(), object->pendingActionOn);

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
    std::cout << "AnimationEnded. Observers: " << object->AnimationEnded.ObserverCount() << std::endl;
    object->PlayAnimation("use");
  }
  else
  {
    std::cout << "PendingAvtionUseAnimationDone" << std::endl;
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
    CloseRunningUi<UiItUseObjectOn>();
}

void Level::ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item)
{
  if (!(user->pendingAnimationDone))
  {
    if (user == target && target == GetPlayer())
    {
      ConsoleWrite("Stop hitting yourself !");
      return ;
    }

    user->LookAt(target);

    if (target->GetDistance(user) > (float)((*item)["range"]))
      ConsoleWrite("Out of range");
    else if (!(user->HasLineOfSight(target)))
      ConsoleWrite("No line of sight");
    else
    {
      unsigned int equipedIt = 0;

      user->pendingActionObject = item;
      user->pendingActionOn     = target;
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
    string output;
    
    output = (item->UseAsWeapon(user, target));
    MouseRightClicked();
    ConsoleWrite(output);
  }
}

void Level::PendingActionUseWeaponOn(InstanceDynamicObject* fromObject)
{
  std::cout << "PendingActionUseWeaponOn" << std::endl;
  ActionUseWeaponOn(fromObject->Get<ObjectCharacter>(),
		    fromObject->pendingActionOn->Get<ObjectCharacter>(),
		    fromObject->pendingActionObject);
}

void Level::PlayerEquipObject(unsigned short it, InventoryObject* object)
{
  bool canWeildMouth        = object->CanWeild(GetPlayer(), EquipedMouth);
  bool canWeildMagic        = object->CanWeild(GetPlayer(), EquipedMagic);
  bool canWeildBattleSaddle = object->CanWeild(GetPlayer(), EquipedBattleSaddle);
  int  canWeildTotal        = (canWeildMouth ? 1 : 0) + (canWeildMagic ? 1 : 0) + (canWeildBattleSaddle ? 1 : 0);

  if (canWeildTotal >= 2)
  {
    UiEquipMode* ui = new UiEquipMode(_window, _gameUi.GetContext(), it, object);

    if (_currentUis[UiItEquipMode])
      delete _currentUis[UiItEquipMode];
    _currentUis[UiItEquipMode] = ui;
    ui->Closed.Connect(*this, &Level::CloseRunningUi<UiItEquipMode>);
    ui->EquipModeSelected.Connect(*GetPlayer(), &ObjectCharacter::SetEquipedItem);

    if (!canWeildMouth)        ui->DisableMode(EquipedMouth);
    if (!canWeildMagic)        ui->DisableMode(EquipedMagic);
    if (!canWeildBattleSaddle) ui->DisableMode(EquipedBattleSaddle);
  }
  else if (canWeildTotal)
  {
    GetPlayer()->SetEquipedItem(it, object, (canWeildMouth ? EquipedMouth :
                                            (canWeildMagic ? EquipedMagic : EquipedBattleSaddle)));
  }
  else
    ConsoleWrite("You can't equip " + object->GetName());
}

void Level::PlayerDropObject(InventoryObject* object)
{
  ActionDropObject(GetPlayer(), object);
}

void Level::PlayerUseObject(InventoryObject* object)
{
  ActionUseObjectOn(GetPlayer(), GetPlayer(), object);
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
  user->GetInventory().DelObject(object);
  waypoint = user->GetOccupiedWaypoint();
  item     = new ObjectItem(this, object->CreateDynamicObject(_world), object);
  item->SetOccupiedWaypoint(waypoint);
  item->GetDynamicObject()->nodePath.set_pos(waypoint->nodePath.get_pos());
  _objects.push_back(item);
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
