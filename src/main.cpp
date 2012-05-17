#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>

#include <panda3d/texturePool.h>
#include <panda3d/directionalLight.h>

PandaFramework framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
NodePath camera;

#include "datatree.hpp"
#include "tilemap.hpp"
#include "scene_camera.hpp"
#include "mouse.hpp"
#include "interact_menu.hpp"

#include "objects/door.hpp"
#include "objects/dropped_object.hpp"

using namespace std;

class Timer
{
public:
  Timer(void)
  {
    _globalClock = ClockObject::get_global_clock();
    _lastTime    = _globalClock->get_real_time();
  }

  double          GetElapsedTime(void) { return (_globalClock->get_real_time() - _lastTime); }
  void            Restart(void)        { _lastTime = _globalClock->get_real_time();          }
  
private:
  double          _lastTime;
  PT(ClockObject) _globalClock;
};

#include "objectnode.hpp"
#include "character.hpp"

class Scene : public AsyncTask
{
public:
  Scene(WindowFramework* window, const std::string& filename);
  ~Scene();

  DoneStatus       do_task(void);

  void        CloseInteractMenu(void)
  {
    if (_currentInteractMenu)
    {
      delete _currentInteractMenu;
      _currentInteractMenu = 0;
    }
    _camera.SetEnabledScroll(true);
  }

  ObjectNode* FindObjectFromNode(NodePath node)
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
  
  Character* FindCharacterFromNode(NodePath node)
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

  // Interaction Management
  void CallbackActionUse(ObjectNode* object)
  {
    object->InteractUse(*(_characters.begin()));
    CloseInteractMenu();
  }

  // Mouse Management
  enum MouseState
  {
    MouseAction,
    MouseInteraction,
  };

  enum MouseHovering
  {
    HoveringUnit,
    HoveringCase
  };

  void             MouseInit(void)
  {
    _mouseState    = MouseAction;
    _mouseHovering = HoveringCase;
    _mouse.ButtonLeft.Connect(*this, &Scene::MouseLeftClicked);
    _mouse.ButtonRight.Connect(*this, &Scene::MouseRightClicked);
    _mouse.CaseHovered.Connect(*this, &Scene::HoveredCase);
    _mouse.UnitHovered.Connect(*this, &Scene::HoveredUnit);
  }
  
  void             HoveredUnit(NodePath node)
  {
    _mouseHovering = HoveringUnit;
    _hoveredUnit   = node;
  }

  void             HoveredCase(int x, int y)
  {
    _mouseHovering = HoveringCase;
    _hoveredCase.set_x(x);
    _hoveredCase.set_y(y);
  }

  void             MouseLeftClicked(void)
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

  void             MouseRightClicked(void)
  {
    cout << "Changed mouse state" << endl;
    CloseInteractMenu();
    _mouseState = (_mouseState == MouseAction ? MouseInteraction : MouseAction);
  }

  MouseState       _mouseState;
  MouseHovering    _mouseHovering;
  NodePath         _hoveredUnit;
  LPoint2          _hoveredCase;

private:
  typedef std::list<Character*>  Characters;
  typedef std::list<ObjectNode*> Objects;

  WindowFramework*  _window;
  GraphicsWindow*   _graphicWindow;
  Mouse             _mouse;
  SceneCamera       _camera;
  Timer             _timer;

  Tilemap           _tilemap;
  Characters        _characters;
  Objects           _objects;

  DirectionalLight* _sunLight;
  NodePath          _sunLightNode;

  InteractMenu*     _currentInteractMenu;
};

struct LevelObjectLoader
{
  //typedef function<ObjectNode*(WindowFramework*, Tilemap&, Data)> Loader;

  LevelObjectLoader(string name, Loader func) : name(name), func(func) {}
  
  string name;
  Loader func;
};

#include <panda3d/pStatClient.h>

float ceilingCurrentTransparency;

Scene::Scene(WindowFramework* window, const std::string& filename) : _window(window), _mouse(window),
  _camera(window, window->get_camera_group()), _tilemap(window)
{
  ceilingCurrentTransparency = 1.f;

  if (!(PStatClient::connect("localhost", 5185)))
    cout << "Can't connect to PStat client" << endl;
  
  ObjectNode::ActionUse.Connect(*this, &Scene::CallbackActionUse);
  _currentInteractMenu = 0;

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
      /* Temporary Character loading */
      ObjectNode* toAdd;

      if (objectData.Key() == "door")
      {
        toAdd = new Door(window, _tilemap, objectData);
      }
      else if (objectData.Key() == "horse")
      {
        Character* character = new Character(window, _tilemap, objectData);

        _characters.push_back(character);
        toAdd = character;
      }
      else if (objectData.Key() == "item")
      {
        InventoryObject* object    = new InventoryObject(objectData);
        DroppedObject*   character = new DroppedObject(window, _tilemap, *object);

        toAdd = character;
      }

      _objects.push_back(toAdd);
    });
  }
  else
    throw ("Can't load scene '" + filename + "'");
  _graphicWindow = _window->get_graphics_window();
  AsyncTaskManager::get_global_ptr()->add(this);

  _sunLight = new DirectionalLight("sun_light");

  _sunLight->set_color(LVecBase4f(0.8, 0.8, 0.8, 1));
  _sunLight->set_shadow_caster(true, 512, 512);

  _sunLightNode = window->get_render().attach_new_node(_sunLight);
  _sunLightNode.set_hpr(-30, -60, 0);
  window->get_render().set_light(_sunLightNode);
  window->get_render().set_shader_input("light", _sunLightNode);

  MouseInit();
}

Scene::~Scene()
{
  for_each(_characters.begin(), _characters.end(), [](Character* character) { delete character; });
  AsyncTaskManager::get_global_ptr()->remove(this);
}

AsyncTask::DoneStatus Scene::do_task(void)
{
  // FPS COUNTER
  {
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
  }
  
  float elapsedTime = _timer.GetElapsedTime();

  _mouse.Run();
  _camera.Run(elapsedTime);

  for_each(_objects.begin(), _objects.end(), [this, elapsedTime](ObjectNode* object)
  {
    object->Run(elapsedTime);
  });

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

  _timer.Restart();
  return AsyncTask::DS_cont;  
}

#include <panda3d/graphicsStateGuardianBase.h>

int main(int argc, char *argv[])
{
    //open a new window framework
  framework.open_framework(argc, argv);
    //set the window title to My Panda3D Window
  framework.set_window_title("Fallout Equestria");
    //open the window
  WindowFramework *window = framework.open_window();
  
  window->enable_keyboard();
  
  NodePath root  = window->get_render();

  camera = window->get_camera_group();

  camera.set_pos(0, 0, 75);
  camera.set_hpr(0, -60, 0);

  camera.set_hpr(-40, -40, 0);

  Scene scene(window, "test");

  window->get_render().set_shader_auto();


    //do the main loop, equal to run() in python
  framework.main_loop();
    //close the window framework
  framework.close_framework();
  return (0);
}
