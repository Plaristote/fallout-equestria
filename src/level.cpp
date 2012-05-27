#include "globals.hpp"
#include "level.hpp"

#include "npc.hpp"

using namespace std;

struct LevelObjectLoader
{
  typedef ObjectNode* (*Loader)(WindowFramework*, Tilemap&, Characters&, Data);

  LevelObjectLoader(string name, Loader func) : name(name), func(func) {}

  string name;
  Loader func;
};

LevelObjectLoader objectLoaders[] = {
  LevelObjectLoader("door",      &Door::Factory),
  LevelObjectLoader("item",      &DroppedObject::Factory),
  LevelObjectLoader("character", &Character::Factory),
  LevelObjectLoader("npc",       &Npc::Factory)
};

Level* Level::CurrentLevel = 0;

Level::Level(WindowFramework* window, const std::string& filename) : _window(window), _mouse(window),
  _camera(window, window->get_camera_group()), _tilemap(window), _gameUi(window)
{
  CurrentLevel = this;

  _l18n = DataTree::Factory::JSON("data/l18n/english.json");

  ceilingCurrentTransparency = 1.f;

  ObjectNode::ActionUse.Connect(*this, &Level::CallbackActionUse);
  ObjectNode::ActionTalkTo.Connect(*this, &Level::CallbackActionTalkTo);
  _currentInteractMenu = 0;
  _currentRunningDialog = 0;

  DataTree* datafile = DataTree::Factory::ShinyLang("scenes/" + filename);
  DataTree* tilefile = DataTree::Factory::JSON("maps/" + filename + ".json");

  if (tilefile)
    _tilemap.Load(tilefile);
  else
    throw ("Can't load tilemap '" + filename + "'");

  if (datafile)
  {
    Data data(datafile);
    Data characters = data["characters"];

    for_each(characters.begin(), characters.end(), [this, window](Data objectData)
    {
      for (unsigned short i = 0 ; i < GET_ARRAY_SIZE(objectLoaders) /*objectLoaders.size()*/ ; ++i)
      {
        if (objectLoaders[i].name == objectData.Key())
        {
          _objects.push_back(objectLoaders[i].func(window, _tilemap, _characters, objectData));
          break ;
        }
      }
    });
  }
  else
    throw ("Can't load scene '" + filename + "'");
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
}

/*void Level::StartFight(void)
{
  Characters::iterator it  = _characters.begin();
  Characters::iteratir end = _characters.end();
}*/

AsyncTask::DoneStatus Level::do_task(void)
{
  // TEST FPS COUNTER
  /*{
    static Timer        fpsTimer;
    static unsigned int fpsCounter = 0;

    if (fpsTimer.GetElapsedTime() > 1.f)
    {
      cout << "[FPS Count]" << fpsCounter << endl;
      cout << "[NodeCount]" << _window->get_render().count_num_descendants() << endl;
      fpsTimer.Restart();
      fpsCounter = 0;
    }
    fpsCounter++;
  }*/

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

      //cout << "Dif -> " << dif.get_x() << ", " << dif.get_y() << ", " << dif.get_z() << endl;

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

  for_each(_objects.begin(), _objects.end(), [this, elapsedTime](ObjectNode* object)
  {
    object->Run(elapsedTime);
  });
  _tilemap.ResetPathfinding();
  TaskCeiling(elapsedTime);
  _timer.Restart();
  return AsyncTask::DS_cont;
}

void Level::TaskCeiling(float elapsedTime)
{
  MapElement::Position        playerPos = (*(_characters.begin()))->GetPosition();
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
  }
}

/*
 * Nodes Management
 */
ObjectNode* Level::FindObjectFromNode(NodePath node)
{
  Objects::iterator cur = _objects.begin();

  while (cur != _objects.end())
  {
    if ((*(*cur)) == node)
      return (*cur);
    ++cur;
  }
  return (0);
}

Character* Level::FindCharacterFromNode(NodePath node)
{
  Characters::iterator cur = _characters.begin();

  while (cur != _characters.end())
  {
    if ((*(*cur)) == node)
      return (*cur);
    ++cur;
  }
  return (0);
}

Character* Level::FindCharacterByName(const std::string& name)
{
  Characters::iterator cur = _characters.begin();

  while (cur != _characters.end())
  {
    if ((*(*cur)) == name)
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
  _mouseHovering = HoveringCase;
  _mouse.ButtonLeft.Connect(*this, &Level::MouseLeftClicked);
  _mouse.ButtonRight.Connect(*this, &Level::MouseRightClicked);
  _mouse.CaseHovered.Connect(*this, &Level::HoveredCase);
  _mouse.UnitHovered.Connect(*this, &Level::HoveredUnit);
  _mouse.ButtonMiddle.Connect(_camera, &SceneCamera::SwapCameraView);
}

void Level::HoveredUnit(NodePath node)
{
  _mouseHovering = HoveringUnit;
  _hoveredUnit   = node;
}

void Level::HoveredCase(int x, int y)
{
  _mouseHovering = HoveringCase;
  _hoveredCase.set_x(x);
  _hoveredCase.set_y(y);
}

void Level::MouseLeftClicked(void)
{
  cout << "Mouse Left Clicked" << endl;
  switch (_mouseState)
  {
    case MouseAction:
      cout << "Mouse Action" << endl;
      if (_mouseHovering      == HoveringUnit)
      {
        // Do something, or not...
      }
      else if (_mouseHovering == HoveringCase)
      {
        // Test stuff
        (*(_characters.begin()))->GoTo(_hoveredCase.get_x(), _hoveredCase.get_y());
      }
      break ;
    case MouseInteraction:
      cout << "Mouse Interaction" << endl;
      if (_mouseHovering == HoveringUnit)
      {
        cout << "Mouse Interaction : HoveringUnit" << endl;
        if (_currentInteractMenu)
          delete _currentInteractMenu;
        ObjectNode* currentHoveredObject = FindObjectFromNode(_hoveredUnit);

        if (currentHoveredObject)
        {
          cout << "Open contextual menu for unit" << endl;
          _currentInteractMenu = new InteractMenu(_window, *currentHoveredObject);
          _camera.SetEnabledScroll(false);
        }
        else
          cerr << "Error finding hovered unit" << endl;
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

// Interactions
void Level::CallbackActionUse(ObjectNode* object)
{
  object->InteractUse(*(_characters.begin()));
  CloseInteractMenu();
}

void Level::CallbackActionTalkTo(ObjectNode* object)
{
  const string& dialog = object->GetDialog();

  CloseInteractMenu();
  if (_currentRunningDialog)
    delete _currentRunningDialog;
  _currentRunningDialog = new DialogController(_window, _gameUi.GetContext(), dialog, _l18n);
  _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningDialog);
  _camera.SetEnabledScroll(false);
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
