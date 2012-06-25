#ifndef  LEVEL_HPP
# define LEVEL_HPP

# include <panda3d/cmath.h>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/directionalLight.h>

# include "timer.hpp"
# include "datatree.hpp"
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
# include "inventory_ui.hpp"

# include "world.h"

class Level
{
  float ceilingCurrentTransparency;
public:
  static Level* CurrentLevel;
  
  Level(WindowFramework* window, GameUi& gameUi, AsyncTask& task, Utils::Packet& data);

  // Saving/Loading
  void SetDataEngine(DataEngine* de) { _dataEngine = de; }
  void Save(Utils::Packet&);
  void Load(Utils::Packet&);

  ~Level();

  enum State
  {
    Normal,
    Fight,
    Interrupted
  };

  AsyncTask::DoneStatus   do_task(void);
  void                    SetState(State);
  State                   GetState(void) const { return (_state); }
  void                    SetInterrupted(bool);
  void                    TaskCeiling(float elapsedTime);
  
  // World Interactions
  bool                   FindPath(std::list<Waypoint>&, Waypoint&, Waypoint&);
  World*                 GetWorld(void)       { return (_world); }
  SceneCamera&           GetCamera(void)      { return (_camera); }
  ObjectCharacter*       GetCharacter(const std::string& name);
  ObjectCharacter*       GetPlayer(void);
  void                   UnprocessAllCollisions(void);
  void                   ProcessAllCollisions(void);

  void                   CloseInteractMenu(void);
  InstanceDynamicObject* FindObjectFromNode(NodePath node);
  InstanceDynamicObject* GetObject(const std::string& name);
  TimeManager&           GetTimeManager(void) { return (_timeManager);  }
  Data                   GetDataEngine(void)  { return (*_dataEngine);  }
  Data                   GetItems(void)       { return (_items);        }
  void                   ConsoleWrite(const std::string& str);

  void                   RemoveObject(InstanceDynamicObject* object);
  
  void                   CallbackExitZone(void);
  void                   CallbackGoToZone(const std::string& name);
  void                   CallbackSelectNextZone(const std::vector<std::string>& zones);
  const std::string&     GetNextZone(void) const;
  const std::string&     GetExitZone(void) const;
  void                   SetEntryZone(const std::string&);

  // Interaction Management
  void                   CallbackActionUse(InstanceDynamicObject* object);
  void                   CallbackActionTalkTo(InstanceDynamicObject* object);
  void                   CallbackActionUseObjectOn(InstanceDynamicObject* object);
  void                   CallbackActionTargetUse(unsigned short it);
  void                   SelectedUseObjectOn(InventoryObject* object);

  void                   ActionUse(ObjectCharacter* user, InstanceDynamicObject* target);
  void                   ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object);
  void                   ActionDropObject(ObjectCharacter* user, InventoryObject* object);
  void                   ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* object);

  void                   PendingActionTalkTo(InstanceDynamicObject* fromObject);
  void                   PendingActionUse(InstanceDynamicObject* fromObject);
  void                   PendingActionUseObjectOn(InstanceDynamicObject* fromObject);
  void                   PendingActionUseWeaponOn(InstanceDynamicObject* fromObject);

  // Interace Interactions
  void                   PlayerDropObject(InventoryObject*);
  void                   PlayerUseObject(InventoryObject*);
  void                   PlayerLoot(Inventory*);

  void                   PlayerEquipObject(unsigned short it, InventoryObject* object);

  Observatory::Signal<void (Inventory&)> SignalShelfOpened;

  // Fight Management
  void                   StartFight(ObjectCharacter* starter);
  void                   StopFight(void);
  void                   NextTurn(void);
  bool                   UseActionPoints(unsigned short ap);

  // Mouse Management
  enum MouseState
  {
    MouseAction,
    MouseInteraction,
    MouseTarget
  };

  void              SetMouseState(MouseState);
  void              MouseLeftClicked(void);
  void              MouseRightClicked(void);

  void              StartCombat(void);

  MouseState        _mouseState;

private:
  typedef std::list<InstanceDynamicObject*> InstanceObjects;
  typedef std::list<ObjectCharacter*>       Characters;
  typedef std::list<LevelExitZone*>         ExitZones;

  void              RunDaylight(void);
  void              MouseInit(void);
  
  Observatory::ObserverHandler obs;

  WindowFramework*  _window;
  GraphicsWindow*   _graphicWindow;
  AsyncTask&        _asyncTask;
  Mouse             _mouse;
  SceneCamera       _camera;
  Timer             _timer;
  TimeManager       _timeManager;
  State             _state;

  World*               _world;
  InstanceObjects      _objects;
  Characters           _characters;
  Characters::iterator _itCharacter;
  
  ExitZones            _exitZones;
  bool                 _exitingZone;
  std::string          _exitingZoneTo, _exitingZoneName;

  DirectionalLight* _sunLight;
  NodePath          _sunLightNode;

  enum UiIterator
  {
    UiItRunningDialog,
    UiItUseObjectOn,
    UiItLoot,
    UiItEquipMode,
    UiItNextZone,
    UiTotalIt
  };
  
  template<UiIterator it> void CloseRunningUi(void)
  {
    if (_currentUis[it])
      _currentUis[it]->Destroy();
    _camera.SetEnabledScroll(true);
    _mouseActionBlocked = false;
    SetInterrupted(false);
  }
  
  LevelUi           _levelUi;
  InteractMenu*     _currentInteractMenu;
  UiBase*           _currentUis[UiTotalIt];
  DialogController* _currentRunningDialog;
  UiUseObjectOn*    _currentUseObjectOn;
  UiLoot*           _currentUiLoot;
  bool              _mouseActionBlocked;

  DataEngine*       _dataEngine;
  DataTree*         _l18n;
  DataTree*         _items;
};

#endif