#ifndef  LEVEL_HPP
# define LEVEL_HPP

# include <panda3d/cmath.h>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/directionalLight.h>

# include "playerparty.hpp"

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
# include "soundmanager.hpp"

class Level
{
public:
  static Level* CurrentLevel;
  
  Level(WindowFramework* window, GameUi& gameUi, Utils::Packet& data, TimeManager& tm);
  
  void InitSun(void);
  void InitPlayer(void);

  // Saving/Loading
  void SetDataEngine(DataEngine* de)   { _dataEngine = de; }
  void SetPlayerInventory(Inventory*);
  void SaveUpdateWorld(void);
  void Save(Utils::Packet&);
  void Load(Utils::Packet&);
  
  void InsertParty(PlayerParty& party);
  void FetchParty(PlayerParty& party);
  void StripParty(PlayerParty& party);

  ~Level();

  enum State
  {
    Normal,
    Fight,
    Interrupted
  };

  AsyncTask::DoneStatus   do_task(void);
  void                    SetPersistent(bool set)  { _persistent = set;    }
  bool                    IsPersistent(void) const { return (_persistent); }
  void                    SetState(State);
  State                   GetState(void) const { return (_state); }
  void                    SetInterrupted(bool);
  void                    DisplayCombatPath(void);
  void                    DestroyCombatPath(void);
  
  // World Interactions
  bool                   FindPath(std::list<Waypoint>&, Waypoint&, Waypoint&);
  World*                 GetWorld(void)       { return (_world); }
  SceneCamera&           GetCamera(void)      { return (_camera); }
  ObjectCharacter*       GetCharacter(const std::string& name);
  ObjectCharacter*       GetCharacter(const DynamicObject*);
  ObjectCharacter*       GetPlayer(void);
  void                   UnprocessAllCollisions(void);
  void                   ProcessAllCollisions(void);
  void                   RefreshCharactersVisibility(void);

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
  void                   CallbackCancelSelectZone(void);
  const std::string&     GetNextZone(void) const;
  const std::string&     GetExitZone(void) const;
  void                   SetEntryZone(Party&, const std::string&);
  void                   SetEntryZone(ObjectCharacter*, const std::string&);
  bool                   MoveCharacterTo(ObjectCharacter*, Waypoint* wp);
  bool                   MoveCharacterTo(ObjectCharacter*, unsigned int wp_id);

  // Interaction Management
  void                   CallbackActionBarter(ObjectCharacter*);
  void                   CallbackActionUse(InstanceDynamicObject* object);
  void                   CallbackActionTalkTo(InstanceDynamicObject* object);
  void                   CallbackActionUseObjectOn(InstanceDynamicObject* object);
  void                   CallbackActionUseSkillOn(InstanceDynamicObject* object);
  void                   CallbackActionTargetUse(unsigned short it);

  void                   ActionUse(ObjectCharacter* user, InstanceDynamicObject* target);
  void                   ActionUseObject(ObjectCharacter* user, InventoryObject* object, unsigned char actionIt);
  void                   ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object, unsigned char actionIt);
  void                   ActionUseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill);
  void                   ActionDropObject(ObjectCharacter* user, InventoryObject* object);
  void                   ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* object, unsigned char actionIt);

  // Interace Interactions
  void                   PlayerDropObject(InventoryObject*);
  void                   PlayerUseObject(InventoryObject*);
  void                   PlayerLoot(Inventory*);
  void                   PlayerLootWithScript(Inventory*, InstanceDynamicObject*, asIScriptContext*, const std::string& script_path);

  void                   PlayerEquipObject(unsigned short it, InventoryObject* object);
  void                   PlayerEquipObject(const std::string& target, unsigned int slot, InventoryObject* object);

  Sync::Signal<void (Inventory&)> SignalShelfOpened;

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

  void               SetMouseState(MouseState);
  void               MouseLeftClicked(void);
  void               MouseRightClicked(void);
  void               MouseWheelUp(void);
  void               MouseWheelDown(void);
  void               MouseSuccessRateHint(void);

  void               StartCombat(void);

  MouseState         _mouseState;

  // Misc
  void               SetName(const std::string& name) { _level_name = name;   }
  const std::string& GetName(void) const              { return (_level_name); }
  void               SpawnEnemies(const std::string& type, unsigned short quantity, unsigned short n_spawn);
  bool               IsWaypointOccupied(unsigned int id) const;
  ISampleInstance*   PlaySound(const std::string& name);

  Sync::ObserverHandler obs;
private:
  typedef std::list<InstanceDynamicObject*> InstanceObjects;
  typedef std::list<ObjectCharacter*>       Characters;
  typedef std::list<LevelExitZone*>         ExitZones;
  
  void              SetupCamera(void);

  void              RunDaylight(void);
  void              RunMetabolism(void);
  void              MouseInit(void);
  void              ToggleCharacterOutline(bool);
  
  void              InsertDynamicObject(DynamicObject&);
  void              InsertCharacter(ObjectCharacter*);
  
  Sync::ObserverHandler obs_player;

  std::string          _level_name;
  WindowFramework*     _window;
  GraphicsWindow*      _graphicWindow;
  Mouse                _mouse;
  SceneCamera          _camera;
  Timer                _timer;
  TimeManager&         _timeManager;
  State                _state;
  bool                 _persistent;

  World*               _world;
  SoundManager         _sound_manager;
  InstanceObjects      _objects;
  Characters           _characters;
  Characters::iterator _itCharacter;
  Characters::iterator _currentCharacter;
  NodePath             _player_halo;
  
  ExitZones            _exitZones;
  bool                 _exitingZone;
  std::string          _exitingZoneTo, _exitingZoneName;

  PT(DirectionalLight) _sunLight;
  NodePath             _sunLightNode;
  PT(AmbientLight)     _sunLightAmbient;
  NodePath             _sunLightAmbientNode;
  World::WorldLights::iterator _light_iterator;
  
  TimeManager::Task*   _task_daylight;
  TimeManager::Task*   _task_metabolism;

  enum UiIterator
  {
    UiItInteractMenu,
    UiItRunningDialog,
    UiItUseObjectOn,
    UiItUseSkillOn,
    UiItLoot,
    UiItEquipMode,
    UiItNextZone,
    UiItBarter,
    UiTotalIt
  };
  
  template<UiIterator it> void CloseRunningUi(void)
  {
    if (_currentUis[it])
      _currentUis[it]->Destroy();
    for (short i = 0 ; i < UiTotalIt ; ++i)
    {
      if (_currentUis[it] != 0 && _currentUis[it]->IsVisible())
        return ;
    }
    _mouseActionBlocked = false;
    _camera.SetEnabledScroll(true);
    SetInterrupted(false);
  }

  LevelUi           _levelUi;
  UiBase*           _currentUis[UiTotalIt];
  DialogController* _currentRunningDialog;
  UiUseObjectOn*    _currentUseObjectOn;
  UiLoot*           _currentUiLoot;
  bool              _mouseActionBlocked;

  DataEngine*       _dataEngine;
  DataTree*         _items;

  /*
   * Combat Path Shower
   */
  std::list<Waypoint> _combat_path;
  NodePath            _last_combat_path;

  /*
   * Floor Management
   */
  class HidingFloor
  {
    NodePath floor;
    bool     fadingIn, done;
    float    alpha;  
  public:
    HidingFloor() : done(false) {}
    
    bool  operator==(NodePath np) const { return (floor == np); }
    bool  Done(void)              const { return (done);        }
    float Alpha(void)             const { return (alpha);       }
    void  ForceAlpha(float _alpha)      { alpha = _alpha;       }
    void  SetNodePath(NodePath np);
    void  SetFadingIn(bool set);
    void  Run(float elapsedTime);
  };

  std::list<HidingFloor> _hidingFloors;

  unsigned char     _currentFloor;
  Waypoint*         _floor_lastWp;
  
  bool              IsInsideBuilding(unsigned char& floor);
  void              CheckCurrentFloor(float elapsedTime);
  void              SetCurrentFloor(unsigned char floor);
  void              FloorFade(bool in, NodePath floor);
};

#endif
