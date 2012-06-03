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

# include "world.h"

/*
 * WARNING This needs to move. C'est pas l'auspice ici.
 */
class Level;

struct WaypointModifier
{
  virtual void ProcessCollisions(void);
  void         UnprocessCollisions(void);
protected:
  void         WithdrawAllArcs(unsigned int id);
  void         WithdrawAllArcs(Waypoint* waypoint);
  void         WithdrawArc(unsigned int id1, unsigned int id2);
  void         WithdrawArc(Waypoint* first, Waypoint* second);

  Level*                          _level;
  Waypoint*                       _waypointOccupied;
  std::list<std::pair<int, int> > _waypointDisconnected;

private:
  typedef std::pair<Waypoint*, Waypoint*> WithdrawedArc;
  typedef std::list<WithdrawedArc>        WithdrawedArcs;

  WithdrawedArcs                          _withdrawedArcs;
};

class InstanceDynamicObject : public WaypointModifier
{
public:
  InstanceDynamicObject(Level* level, DynamicObject* object) : _object(object)
  {
    _level                = level;
    _waypointDisconnected = object->lockedArcs;
    _waypointOccupied     = object->waypoint;
  }

  virtual ~InstanceDynamicObject() {}

  virtual void       Run(float elapsedTime) {};

  bool               operator==(const std::string& name) const { return (GetName() == name);            }
  std::string        GetName(void)                       const { return (_object->nodePath.get_name()); }

protected:
  DynamicObject* _object;
};

class ObjectDoor : public InstanceDynamicObject
{
public:
  ObjectDoor(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
  }

private:
};

class ObjectCharacter : public InstanceDynamicObject
{
public:
  ObjectCharacter(Level* level, DynamicObject* object);

  void Run(float elapsedTime);
  void                GoTo(unsigned int id);
  void                GoTo(Waypoint* waypoint);

private:
  void                RunMovement(float elapsedTime);

  std::list<Waypoint> _path;
};


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
    //std::for_each(_characters.begin(), _characters.end(), [](Character* character) { delete character; });
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
  
  // World Interactions
  bool             FindPath(std::list<Waypoint>&, Waypoint&, Waypoint&);
  World*           GetWorld(void)       { return (_world); }
  //const World*     GetWorld(void) const { return (_world); }

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

  void              MouseLeftClicked(void);
  void              MouseRightClicked(void);

  void              StartCombat(void);

  MouseState        _mouseState;

private:
  typedef std::list<ObjectNode*> Objects;
  typedef std::list<InstanceDynamicObject*> InstanceObjects;
  typedef std::list<ObjectCharacter*>       Characters;

  void              MouseInit(void);

  WindowFramework*  _window;
  GraphicsWindow*   _graphicWindow;
  Mouse             _mouse;
  SceneCamera       _camera;
  Timer             _timer;
  
  World*            _world;
  InstanceObjects   _objects;
  Characters        _characters;

  DirectionalLight* _sunLight;
  NodePath          _sunLightNode;

  GameUi            _gameUi;
  InteractMenu*     _currentInteractMenu;
  DialogController* _currentRunningDialog;

  DataEngine        _dataEngine;
  DataTree*         _l18n;
};

#endif