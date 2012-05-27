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

# include "dataengine.hpp"

# include "objectnode.hpp"
# include "objects/door.hpp"
# include "objects/dropped_object.hpp"
# include "character.hpp"

# include "gameui.hpp"
# include "dialog.hpp"

/*
 * Level
 */
class Level : public AsyncTask
{
  float ceilingCurrentTransparency;
public:
  static Level* CurrentLevel;
  
  Level(WindowFramework* window, const std::string& filename);

  ~Level()
  {
    std::for_each(_characters.begin(), _characters.end(), [](Character* character) { delete character; });
    AsyncTaskManager::get_global_ptr()->remove(this);
    CurrentLevel = 0;
    if (_currentRunningDialog)
      delete _currentRunningDialog;
    if (_currentInteractMenu)
      delete _currentInteractMenu;
    if (_l18n)
      delete _l18n;
  }

  DoneStatus       do_task(void);
  void             TaskCeiling(float elapsedTime);

  void             CloseInteractMenu(void);
  void             CloseRunningDialog(void);
  ObjectNode*      FindObjectFromNode(NodePath node);
  Character*       FindCharacterFromNode(NodePath node);
  Character*       FindCharacterByName(const std::string& name);
  Data             GetDataEngine(void) { return (_dataEngine); }

  // Interaction Management
  void             CallbackActionUse(ObjectNode* object);
  void             CallbackActionTalkTo(ObjectNode* object);

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

  void              StartCombat(void);

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
  DialogController* _currentRunningDialog;

  DataEngine        _dataEngine;
  DataTree*         _l18n;
};

#endif