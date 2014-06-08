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
# include "mouse/mouse.hpp"
# include "interact_menu.hpp"

# include "dataengine.hpp"

# include "objects/instance_dynamic_object.hpp"
# include "objects/door.hpp"
# include "objects/dropped_object.hpp"
# include "objects/character.hpp"
# include "objects/default.hpp"

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

# include "mouse/mouse_hint.hpp"
# include "visibility_halo.hpp"
# include "floors.hpp"
# include "characters/object_outline.hpp"
# include "path_preview.hpp"
# include "zones/manager.hpp"
# include "equip_modes.hpp"
# include "mouse/mouse_events.hpp"
# include "level/interactions.hpp"
# include "level/combat.hpp"
# include "level/player.hpp"

# include <functional>

class Level
{
  friend class Party;
public:
  typedef std::vector<ObjectCharacter*> CharacterList;

  struct Exit
  {
    bool                  ToWorldmap(void)       const { return (level == "worldmap"); }
    bool                  ReadyForNextZone(void) const { return (level != "");         }

    std::string           level, zone;
  };
  
  static Level*           CurrentLevel;
  
  Level(const std::string& name, WindowFramework* window, GameUi& gameUi, Utils::Packet& data, TimeManager& tm);
  
  void                    InitializeSun(void);
  // Saving/Loading
  void                    SetDataEngine(DataEngine* de)   { data_engine = de; }
  void                    SetPlayerInventory(Inventory*);
  void                    Serialize(Utils::Packet&);
  void                    Unserialize(Utils::Packet&);
  
  void                    SetAsPlayerParty(Party& party);
  void                    InsertParty(Party& party, const std::string& entry_zone);
  void                    MatchPartyToExistingCharacters(Party& party);
  void                    RemovePartyFromLevel(Party& party);
  void                    RunForPartyMembers(Party& party, std::function<void (Party::Member*,ObjectCharacter*)>);

  ~Level();

  enum State
  {
    Normal,
    Fight,
    Interrupted
  };

  AsyncTask::DoneStatus   do_task(void);
  void                    SetPersistent(bool set)  { is_persistent = set;    }
  bool                    IsPersistent(void) const { return (is_persistent); }
  void                    SetState(State);
  State                   GetState(void) const { return (level_state); }
  void                    SetInterrupted(bool);
  
  // World Interactions
  WindowFramework*       GetWindow(void)         { return (window); }
  World*                 GetWorld(void)          { return (world); }
  Sunlight*              GetSunlight(void)       { return (sunlight); }
  LevelUi&               GetLevelUi(void)        { return (level_ui); }
  MouseEvents&           GetMouse(void)          { return (mouse); }
  Floors&                GetFloors(void)         { return (floors); }
  TargetOutliner&        GetTargetOutliner(void) { return (target_outliner); }
  VisibilityHalo&        GetPlayerHalo(void)     { return (player_halo);     }
  Zones::Manager&        GetZoneManager(void)    { return (zones); }
  LevelCamera&           GetCamera(void)         { return (camera); }
  SoundManager&          GetSoundManager(void)   { return (sound_manager); }
  ObjectCharacter*       GetCharacter(const std::string& name);
  ObjectCharacter*       GetCharacter(const DynamicObject*);
  CharacterList          FindCharacters(std::function<bool (ObjectCharacter*)> = [](ObjectCharacter*) { return (true); }) const;
  ObjectCharacter*       GetPlayer(void);
  void                   UnprocessAllCollisions(void);
  void                   ProcessAllCollisions(void);
  void                   RefreshCharactersVisibility(void);
  unsigned char          GetCurrentFloor(void) const { return (floors.GetCurrentFloor()); }
  Exit&                  GetExit(void)         { return (exit);   }
  const Exit&            GetExit(void)   const { return (exit);   }
  Combat&                GetCombat(void)       { return (combat); }
  const Combat&          GetCombat(void) const { return (combat); }

  InstanceDynamicObject* FindObjectFromNode(NodePath node);
  InstanceDynamicObject* GetObject(const std::string& name);
  TimeManager&           GetTimeManager(void)    { return (time_manager);      }
  ParticleSystemManager& GetParticleManager(void){ return (particle_manager); }
  ChatterManager&        GetChatterManager(void) { return (chatter_manager);  }
  PathPreview&           GetHoveredPath(void)    { return (hovered_path);      }
  Data                   GetDataEngine(void)     { return (*data_engine);      }

  void                   RemoveObject(InstanceDynamicObject* object);

  // Interace Interactions
  Interactions::Player&  GetInteractions() { return (player);       }
  EquipModes&            GetEquipModes()   { return (equip_modes);  }

  void                   PlayerEquipObject(unsigned short it, InventoryObject* object);
  void                   PlayerEquipObject(const std::string& target, unsigned int slot, InventoryObject* object);


  // Misc
  void               SetName(const std::string& name) { level_name = name;   }
  const std::string& GetName(void) const              { return (level_name); }
  bool               IsWaypointOccupied(unsigned int id) const;
  bool               CanGoThroughWaypoint(InstanceDynamicObject* object, unsigned int id) const;
  ISampleInstance*   PlaySound(const std::string& name);

  void               InsertProjectile(Projectile* projectile) { projectiles.push_back(projectile); }
  void               InsertDynamicObject(DynamicObject&);
  void               InsertInstanceDynamicObject(InstanceDynamicObject*);
  void               InsertCharacter(ObjectCharacter*);
  
  Sync::ObserverHandler obs;
private:
  void               BackupInventoriesToDynamicObjects(void);
  void               SerializeParties(Utils::Packet&);
  void               UnserializeParties(Utils::Packet&);
  
  typedef std::list<InstanceDynamicObject*> InstanceObjects;
  typedef std::list<ObjectCharacter*>       Characters;
  typedef std::list<Party*>                 Parties;
  
  std::string           level_name;
  WindowFramework*      window;
  DataEngine*           data_engine;
  LevelCamera           camera;
  Timer                 timer;
  TimeManager&          time_manager;
  MainScript            main_script;
  State                 level_state;
  bool                  is_persistent;

  LevelUi               level_ui;
  MouseHint             mouse;
  Player                player;
  World*                world;
  ParticleSystemManager particle_manager;
  SoundManager          sound_manager;
  ChatterManager        chatter_manager;
  Projectile::Set       projectiles;
  InstanceObjects       objects;
  Characters            characters;
  Parties               parties;
  Combat                combat;
  VisibilityHalo        player_halo;
  Sunlight*             sunlight;
  Floors                floors;
  TargetOutliner        target_outliner;
  PathPreview           hovered_path;
  Zones::Manager        zones;
  EquipModes            equip_modes;
  Exit                  exit;
};

#endif
