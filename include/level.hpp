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
  Waypoint*    GetOccupiedWaypoint(void) const { return (_waypointOccupied); }

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
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUse;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUseObjectOn;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUseSkillOn;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionTalkTo;  
  
  struct Interaction
  {
    Interaction(const std::string& name, InstanceDynamicObject* instance, Observatory::Signal<void (InstanceDynamicObject*)>* signal)
    : name(name), instance(instance), signal(signal) {}

    std::string                                         name;
    InstanceDynamicObject*                              instance;
    Observatory::Signal<void (InstanceDynamicObject*)>* signal;
  };
  typedef std::list<Interaction> InteractionList;
  
  struct GoToData
  {
    Waypoint*              nearest;
    InstanceDynamicObject* objective;
    int                    max_distance;
  };
  
  InstanceDynamicObject(Level* level, DynamicObject* object) : _object(object)
  {
    _level                = level;
    _waypointDisconnected = object->lockedArcs;
    _waypointOccupied     = object->waypoint;
    
    if (object->interactions & Interactions::TalkTo)
      _interactions.push_back(Interaction("talk_to",    this, &ActionTalkTo));
    if (object->interactions & Interactions::Use)
      _interactions.push_back(Interaction("use",        this, &ActionUse));
    if (object->interactions & Interactions::UseObject)
      _interactions.push_back(Interaction("use_object", this, &ActionUseObjectOn));
    if (object->interactions & Interactions::UseSkill)
      _interactions.push_back(Interaction("use_skill",  this, &ActionUseSkillOn));
  }

  virtual ~InstanceDynamicObject() {}

  virtual void       Run(float elapsedTime) {};

  bool               operator==(NodePath np)             const { return (_object->nodePath.is_ancestor_of(np)); }
  bool               operator==(const std::string& name) const { return (GetName() == name);                    }
  std::string        GetName(void)                       const { return (_object->nodePath.get_name());         }
  NodePath           GetNodePath(void)                   const { return (_object->nodePath);                    }
  InteractionList&   GetInteractions(void)                     { return (_interactions);                        }
  const std::string& GetDialog(void)                     const { return (_object->dialog);                      }
  
  virtual GoToData   GetGoToData(InstanceDynamicObject* character)
  {
    GoToData         ret;

    ret.nearest      = _waypointOccupied;
    ret.objective    = this;
    ret.max_distance = 0;
    return (ret);
  }
  
  InstanceDynamicObject*   pendingActionOn;

protected:
  DynamicObject*           _object;

private:
  InteractionList          _interactions;
};

class ObjectDoor : public InstanceDynamicObject, public Waypoint::ArcObserver
{
public:
  ObjectDoor(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
    _locked = object->locked;
    ObserveWaypoints(true);
  }
  
  ~ObjectDoor()
  {
    ObserveWaypoints(false);
  }
  
  void ObserveWaypoints(bool doObserver);
  
  void ProcessCollisions(void) {}

  bool CanGoThrough(unsigned char id);
  void GoingThrough(void);

private:
  bool _closed;
  bool _locked;
};

class ObjectCharacter : public InstanceDynamicObject
{
public:
  ObjectCharacter(Level* level, DynamicObject* object);

  Observatory::Signal<void (InstanceDynamicObject*)> ReachedDestination;
  
  void                Run(float elapsedTime);
  void                GoTo(unsigned int id);
  void                GoTo(Waypoint* waypoint);
  void                GoTo(InstanceDynamicObject* object, int max_distance = 0);
  unsigned short      GetPathDistance(Waypoint* waypoint);
  unsigned short      GetPathDistance(InstanceDynamicObject* object);
  bool                HasLineOfSight(InstanceDynamicObject* object);

private:
  void                RunMovement(float elapsedTime);

  std::list<Waypoint> _path;
  GoToData            _goToData;
  
  // Line of Sight Tools
  NodePath                  _losPath;
  PT(CollisionRay)          _losRay;
  PT(CollisionNode)         _losNode;
  PT(CollisionHandlerQueue) _losHandlerQueue;
  CollisionTraverser        _losTraverser;
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

  void                   CloseInteractMenu(void);
  void                   CloseRunningDialog(void);
  InstanceDynamicObject* FindObjectFromNode(NodePath node);
  Data                   GetDataEngine(void) { return (_dataEngine); }
  void                   ConsoleWrite(const std::string& str);

  // Interaction Management
  void             CallbackActionUse(InstanceDynamicObject* object);
  void             CallbackActionTalkTo(InstanceDynamicObject* object);

  void             PendingActionTalkTo(InstanceDynamicObject* fromObject);

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