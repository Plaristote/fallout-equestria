#ifndef  LEVEL_HPP
# define LEVEL_HPP

# include <panda3d/cmath.h>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/directionalLight.h>
# include <panda3d/particleSystemManager.h>

# include "playerparty.hpp"

# include "timer.hpp"
# include "time_manager.hpp"
# include "datatree.hpp"
# include "level_camera.hpp"
# include "mouse.hpp"
# include "interact_menu.hpp"

# include "dataengine.hpp"

# include "objectnode.hpp"
# include "objects/door.hpp"
# include "objects/dropped_object.hpp"
# include "objects/character.hpp"

# include "ui/level_ui.hpp"
# include "dialog.hpp"
# include "chatter_manager.hpp"
# include "ui/inventory_ui.hpp"

# include "world/world.h"
# include "sunlight.hpp"
# include "projectile.hpp"
# include "soundmanager.hpp"
# include "main_script.hpp"
# include "script_zone.hpp"
# include "executor.hpp"

# include "mouse_hint.hpp"
# include "visibility_halo.hpp"
# include "floors.hpp"
# include "characters/object_outline.hpp"
# include "path_preview.hpp"
# include "zones/manager.hpp"
# include "equip_modes.hpp"
# include "mouse_events.hpp"

# include <functional>

class Level
{
  friend class Party;
public:
  struct Exit
  {
    bool        ToWorldmap(void)       const { return (level == "worldmap"); }
    bool        ReadyForNextZone(void) const { return (level == "");         }

    std::string level;
    std::string zone;
  };
  
  typedef std::vector<ObjectCharacter*> CharacterList;
  
  static Level* CurrentLevel;
  
  Level(const std::string& name, WindowFramework* window, GameUi& gameUi, Utils::Packet& data, TimeManager& tm);
  
  void InitializeSun(void);
  void InitializePlayer(void);

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
  
  // World Interactions
  World*                 GetWorld(void)       { return (_world); }
  LevelUi&               GetLevelUi(void)     { return (level_ui); }
  MouseEvents&           GetMouse(void)       { return (mouse); }
  Floors&                GetFloors(void)      { return (floors); }
  TargetOutliner&        GetTargetOutliner(void) { return (target_outliner); }
  Zones::Manager&        GetZoneManager(void) { return (zones); }
  LevelCamera&           GetCamera(void)      { return (camera); }
  ObjectCharacter*       GetCharacter(const std::string& name);
  ObjectCharacter*       GetCharacter(const DynamicObject*);
  CharacterList          FindCharacters(std::function<bool (ObjectCharacter*)> = [](ObjectCharacter*) { return (true); }) const;
  ObjectCharacter*       GetPlayer(void);
  void                   UnprocessAllCollisions(void);
  void                   ProcessAllCollisions(void);
  void                   RefreshCharactersVisibility(void);
  unsigned char          GetCurrentFloor(void) const { return (floors.GetCurrentFloor()); }
  Exit&                  GetExit(void)       { return (exit); }
  const Exit&            GetExit(void) const { return (exit); }

  InstanceDynamicObject* FindObjectFromNode(NodePath node);
  InstanceDynamicObject* GetObject(const std::string& name);
  TimeManager&           GetTimeManager(void)    { return (_timeManager);      }
  ParticleSystemManager& GetParticleManager(void){ return (_particle_manager); }
  ChatterManager&        GetChatterManager(void) { return (_chatter_manager);  }
  PathPreview&           GetHoveredPath(void)    { return (hovered_path);      }
  Data                   GetDataEngine(void)     { return (*_dataEngine);      }
  Data                   GetItems(void)          { return (_items);            }
  void                   ConsoleWrite(const std::string& str);

  void                   RemoveObject(InstanceDynamicObject* object);


  // Interaction Management
  void                   CallbackActionBarter(ObjectCharacter*);
  void                   CallbackActionUse(InstanceDynamicObject* object);
  void                   CallbackActionTalkTo(InstanceDynamicObject* object);
  void                   CallbackActionUseObjectOn(InstanceDynamicObject* object);
  void                   CallbackActionUseSkillOn(InstanceDynamicObject* object);
  void                   CallbackActionUseSpellOn(InstanceDynamicObject* object);
  void                   CallbackActionTargetUse(unsigned short it);

  void                   ActionUse(ObjectCharacter* user, InstanceDynamicObject* target);
  void                   ActionUseObject(ObjectCharacter* user, InventoryObject* object, unsigned char actionIt);
  void                   ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object, unsigned char actionIt);
  void                   ActionUseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill);
  void                   ActionUseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill);
  void                   ActionDropObject(ObjectCharacter* user, InventoryObject* object);
  void                   ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* object, unsigned char actionIt);

  // Interace Interactions
  void                   PlayerDropObject(InventoryObject*);
  void                   PlayerUseObject(InventoryObject*);
  void                   PlayerLoot(Inventory*);
  void                   PlayerLootWithScript(Inventory*, InstanceDynamicObject*, asIScriptContext*, const std::string& script_path);

  void                   PlayerEquipObject(unsigned short it, InventoryObject* object);
  void                   PlayerEquipObject(const std::string& target, unsigned int slot, InventoryObject* object);

  // Fight Management
  void                   StartCombat(void);
  void                   StartFight(ObjectCharacter* starter);
  void                   StopFight(void);
  void                   NextTurn(void);
  bool                   UseActionPoints(unsigned short ap);
  ObjectCharacter*       GetCurrentFightPlayer(void) const { return (_itCharacter != _characters.end() ? *_itCharacter : 0); }

  // Misc
  void               SetName(const std::string& name) { _level_name = name;   }
  const std::string& GetName(void) const              { return (_level_name); }
  void               SpawnEnemies(const std::string& type, unsigned short spawn_zone_number);
  bool               IsWaypointOccupied(unsigned int id) const;
  ISampleInstance*   PlaySound(const std::string& name);
  void               InsertProjectile(Projectile* projectile) { _projectiles.push_back(projectile); }

  Sync::ObserverHandler obs;
private:
  typedef std::list<InstanceDynamicObject*> InstanceObjects;
  typedef std::list<ObjectCharacter*>       Characters;
  typedef std::list<Party*>                 Parties;
  
  void              RunMetabolism(void);
  
  void              InsertDynamicObject(DynamicObject&);
  void              InsertCharacter(ObjectCharacter*);
  
  Sync::ObserverHandler obs_player;

  std::string           _level_name;
  WindowFramework*      _window;
  GraphicsWindow*       _graphicWindow;
  LevelCamera           camera;
  MouseEvents           mouse;
  Timer                 _timer;
  TimeManager&          _timeManager;
  MainScript            _main_script;
  State                 _state;
  bool                  _persistent;

  LevelUi               level_ui;
  MouseHint             mouse_hint;
  World*                _world;
  ParticleSystemManager _particle_manager;
  SoundManager          _sound_manager;
  ChatterManager        _chatter_manager;
  Projectile::Set       _projectiles;
  InstanceObjects       _objects;
  Characters            _characters;
  Characters::iterator  _itCharacter;
  Characters::iterator  _currentCharacter;
  Parties               _parties;
  VisibilityHalo        player_halo;
  Sunlight*             _sunlight;
  Floors                floors;
  TargetOutliner        target_outliner;
  PathPreview           hovered_path;
  Zones::Manager        zones;
  EquipModes            equip_modes;
  Exit                  exit;

  World::WorldLights::iterator _light_iterator;

  TimeManager::Task*    _task_metabolism;

  DataEngine*           _dataEngine;
  DataTree*             _items;
};

#endif
