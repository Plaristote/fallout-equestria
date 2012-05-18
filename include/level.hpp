#ifndef  LEVEL_HPP
# define LEVEL_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/directionalLight.h>

# include "timer.hpp"
# include "datatree.hpp"
# include "tilemap.hpp"
# include "scene_camera.hpp"
# include "mouse.hpp"
# include "interact_menu.hpp"

# include "objectnode.hpp"
# include "objects/door.hpp"
# include "objects/dropped_object.hpp"
# include "character.hpp"

/*
 * GameUi
 */
# include <panda3d/pgVirtualFrame.h>
# include <panda3d/rocketRegion.h>
# include <Rocket/Core.h>
# include <panda3d/orthographicLens.h>

class GameUi
{
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;
public:
  GameUi(WindowFramework* window) : _window(window)
  {
    /*Rocket::Core::Context*         context;
    Rocket::Core::ElementDocument* document;

    _rocket = RocketRegion::make("rocket ui", window->get_graphics_output());
    _ih     = new RocketInputHandler();
    window->get_mouse().attach_new_node(_ih);
    _rocket->set_input_handler(_ih);
    _rocket->set_active(true);
    context  = _rocket->get_context();
    document = context->LoadDocument("ui/ingame.rml");

    NodePath node = window->get_pixel_2d();

    node.reparent_to(window->get_aspect_2d());

    if (document != 0)
    {
      document->Show();
      cout << "Loaded RML document" << endl;
    }
    else
      cout << "Failed to load UI" << endl;

    Camera*  camera = new Camera("ui_camera");
    NodePath camera2d(camera);

    Lens*   lens   = new OrthographicLens();
    lens->set_film_size(2, 2);
    lens->set_near_far(-1000, 1000);
    camera->set_lens(lens);

    camera2d.reparent_to(window->get_aspect_2d());
    _rocket->set_camera(camera2d);*/

//     DisplayRegion* region = _rocket;
//
//     node = window->get_aspect_2d().attach_new_node(&((PandaNode)(*region)));


    /*window->set_perpixel(true);
    NodePath windowNode = window->get_pixel_2d();

    Texture*     texture = TexturePool::load_texture("textures/ingame-ui/main-bar.png");

    PGFrameStyle style;
    _mainBar = new PGVirtualFrame("GameUi MainBar");

    //_mainBar->setup(1.696f, 0.202f);
    //_mainBar->setup(2.f, 0.3f);
    _mainBar->setup(400, 100);

    style = _mainBar->get_frame_style(0);
    style.set_type(PGFrameStyle::T_flat);
    style.set_texture(texture);

    _mainBar->set_frame_style(0, style);
    _mainBar->set_frame_style(1, style);
    _mainBar->set_frame_style(2, style);
    _mainBar->set_frame_style(3, style);

    windowNode.attach_new_node(_mainBar);
    NodePath defButNp = window->get_pixel_2d().attach_new_node(_mainBar);

    defButNp.set_pos(10.f, 0, -400.f);*/
  }

private:
  WindowFramework*   _window;
  PT(PGVirtualFrame) _mainBar;
};

/*
 * Level
 */
class Level : public AsyncTask
{
public:
	Level(WindowFramework* window, const std::string& filename);
	~Level() {};

	DoneStatus       do_task(void) { return DS_cont; };
  void             TaskCeiling(float elapsedTime);

  void             CloseInteractMenu(void);
  ObjectNode*      FindObjectFromNode(NodePath node);
  Character*       FindCharacterFromNode(NodePath node);

  // Interaction Management
  void             CallbackActionUse(ObjectNode* object);

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

  void              HoveredUnit(NodePath node);
  void              HoveredCase(int x, int y);

  void              MouseLeftClicked(void);
  void              MouseRightClicked(void);

  MouseState        _mouseState;
  MouseHovering     _mouseHovering;
  NodePath          _hoveredUnit;
  LPoint2           _hoveredCase;

private:
  typedef std::list<ObjectNode*> Objects;

  void              MouseInit(void);
  
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

  GameUi            _gameUi;
  InteractMenu*     _currentInteractMenu;
};

//HAIL MICROSOFT
Level::Level(WindowFramework* window, const std::string& filename) : _mouse(window), _camera(window,window->get_camera_group()), _tilemap(window), _gameUi(window)
{};

#endif