#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "globals.hpp"
# include "level/objectnode.hpp"
# include "level/inventory.hpp"
# include "time_manager.hpp"
# include "cmap/statcontroller.hpp"
# include "level/diplomacy.hpp"
# include "level/characters/line_of_sight.hpp"
# include "level/characters/field_of_view.hpp"
# include "level/characters/metabolism.hpp"
# include "level/pathfinding.hpp"
# include <panda3d/character.h>
# include <panda3d/pointLight.h>

class ObjectCharacter;

class CharacterBuff
{
public:
  CharacterBuff(Level*, ObjectCharacter* character, Data buff);
  CharacterBuff(Level* l);

  bool               operator==(const std::string& comp) const { return (_name == comp); }
  const std::string& GetName(void)                       const { return (_name);         }

  void Begin(ObjectCharacter* from, TimeManager::Task* task = 0);
  void End(void);

  void Load(Level*, ObjectCharacter*, Utils::Packet&);
  void Save(Utils::Packet&);

private:
  void               Initialize(Level*, ObjectCharacter*, Data buff);
  
  Data               _buff;
  
  TimeManager&       _timeManager;
  ObjectCharacter*   _character;
  unsigned short     _duration;
  TimeManager::Task* _task;
  std::string        _name;
  asIScriptContext*  _context;
  asIScriptModule*   _module;
  asIScriptFunction* _begin;
  asIScriptFunction* _end;

  NodePath           _graphicalEffect;
};

class ObjectCharacter : public InstanceDynamicObject
{
public:
  struct FovEnemy
  {
    FovEnemy(ObjectCharacter* enemy, unsigned char ttl) : enemy(enemy), ttl(ttl) {}
    operator ObjectCharacter*() const { return (enemy); }
    bool             operator==(ObjectCharacter* comp) const { return (enemy == comp); }
    ObjectCharacter* enemy;
    unsigned char    ttl;
  };
  
  ObjectCharacter(Level* level, DynamicObject* object);
  ~ObjectCharacter(void);
  
  void Load(Utils::Packet&);
  void Save(Utils::Packet&);

  void SetStatistics(DataTree* stats, StatController* statsController);
  void NullifyStatistics(void);

  Sync::Signal<void (InstanceDynamicObject*)>         ReachedDestination;
  Sync::Signal<void (unsigned short, unsigned short)> ActionPointChanged;
  Sync::Signal<void (short)>                          HitPointsChanged;
  Sync::Signal<void>                                  CharacterDied;

  virtual GoToData     GetGoToData(InstanceDynamicObject* character)
  {
    GoToData         ret;

    ret.nearest      = _waypointOccupied;
    ret.objective    = this;
    ret.max_distance = 0;
    ret.min_distance = 1;
    return (ret);
  }

  void                     SetInventory(Inventory* inventory);

  void                     ProcessCollisions(void)         { if (_hitPoints > 0) InstanceDynamicObject::ProcessCollisions();     }
  bool                     HasOccupiedWaypoint(void) const { return (_hitPoints > 0 && WaypointModifier::HasOccupiedWaypoint()); }

  void                     Run(float elapsedTime);
  void                     RunEffects(float elapsedTime);
  void                     SetRunning(bool running) { _running = running; }
  void                     LookAt(LVecBase3);
  void                     LookAt(InstanceDynamicObject*);
  void                     TeleportTo(unsigned int id);
  void                     TeleportTo(Waypoint* waypoint);
  void                     GoTo(unsigned int id);
  void                     GoTo(Waypoint* waypoint);
  void                     GoTo(InstanceDynamicObject* object, int max_distance = 0);
  void                     GoTo(LPoint3f position);
  void                     GoToRandomWaypoint(void);
  void                     TruncatePath(unsigned short max_length);
  unsigned short           GetPathDistance(Waypoint* waypoint);
  unsigned short           GetPathDistance(InstanceDynamicObject* object);
  float                    GetDistance(const InstanceDynamicObject* object) const;
  int                      GetBestWaypoint(InstanceDynamicObject* object, bool farthest);
  int                      GetNearestWaypoint(InstanceDynamicObject* object);
  int                      GetFarthestWaypoint(InstanceDynamicObject* object);
  Pathfinding::Path&       GetPath(void)              { return (_path);                 }
  const Pathfinding::Path& GetPath(void) const        { return (_path);                 }
  bool                     HasLineOfSight(InstanceDynamicObject* object);
  bool                     IsMoving(void) const       { return (_path.Size() > 0);      }
  bool                     IsAlive(void) const        { return (_hitPoints > 0);        }
  bool                     IsInterrupted(void) const  { return (AnimationEnded.ObserverCount() > 0); }
  Inventory&               GetInventory(void)         { return (*_inventory);           }
  const Inventory&         GetInventory(void) const   { return (*_inventory); }
  Data                     GetStatistics(void)        { return (_statistics ? Data(_statistics) : Data()); }
  StatController*          GetStatController(void)    { return (_stats);                }
  const StatController*    GetStatController(void) const { return (_stats); }
  const std::string        GetFactionName(void) const { return (_faction ? _faction->name : ""); }
  unsigned int             GetFaction(void) const     { return (_faction ? _faction->flag : 0);  }

  unsigned short           GetActionPoints(void) const;
  void                     SetActionPoints(unsigned short ap);
  void                     RestartActionPoints(void);
  
  short                    GetHitPoints(void) const        { return (_hitPoints); }
  void                     SetHitPoints(short hp);
  void                     StatHpUpdate(short);
  
  short                 GetArmorClass(void) const        { return (_armorClass); }
  void                  SetArmorClass(short ac)          { _armorClass = ac; ArmorClassChanged.Emit(_armorClass); }
  void                  RestartArmorClass(void)          { _armorClass -= _tmpArmorClass; _tmpArmorClass = 0;     }
  void                  SetBonusAC(short ac)             { _armorClass += ac; _tmpArmorClass += ac;               }
  Sync::Signal<void (short)>          ArmorClassChanged;

  void                  SetFurtive(bool do_set);
  void                  SetVisible(bool do_set)           { _fading_in = do_set == true; _fading_off = !_fading_in; }
  void                  AddFlag(unsigned char flag)       { _flags |= flag; }
  void                  DelFlag(unsigned char flag)       { if (HasFlag(flag)) { _flags -= flag; } }
  bool                  HasFlag(unsigned char flag) const { return ((_flags & flag) != 0); }

  void                  PlayEquipedItemAnimation(unsigned short it, const std::string& name, InstanceDynamicObject* target);
  void                  RefreshEquipment(void);
  void                  SetEquipedItem(unsigned short it, InventoryObject* object, EquipedMode mode = EquipedMouth);
  InventoryObject*      GetEquipedItem(unsigned short it);
  NodePath              GetEquipedItemNode(unsigned short it);
  unsigned char         GetequipedAction(unsigned short it) const { return (_equiped[it].actionIt); }
  void                  UnequipItem(unsigned short it);
  void                  ItemNextUseType(unsigned short it);
  Sync::Signal<void (unsigned short, InventoryObject*)>                EquipedItemChanged;
  Sync::Signal<void (unsigned short, InventoryObject*, unsigned char)> EquipedItemActionChanged;

  void                  PushBuff(Data, ObjectCharacter* caster);
  void                  DelBuff(CharacterBuff* buff);

  FieldOfView&          GetFieldOfView(void)       { return (field_of_view); }
  const FieldOfView&    GetFieldOfView(void) const { return (field_of_view); }
  void                  SetFaction(const std::string&);
  void                  SetFaction(unsigned int flag);
  void                  SetAsEnemy(const ObjectCharacter*, bool);
  bool                  IsEnemy(const ObjectCharacter*) const;
  bool                  IsAlly(const ObjectCharacter*)  const;
  
  Script::StdList<ObjectCharacter*> GetNearbyEnemies(void) const;
  Script::StdList<ObjectCharacter*> GetNearbyAllies(void)  const;
  
  // Script Communication Tools
  void                  RequestAttack(ObjectCharacter* attack, ObjectCharacter* from);
  void                  RequestHeal(ObjectCharacter* heal, ObjectCharacter* from);
  void                  RequestFollow(ObjectCharacter* follow, ObjectCharacter* from);
  void                  RequestStopFollowing(ObjectCharacter* follow, ObjectCharacter* from);
  int                   AskMorale(void);
  void                  SendMessage(std::string&);
  
private:
  void                  RunRotate(float elapsedTime);
  void                  RunMovement(float elapsedTime);
  void                  RunMovementNext(float elaspedTime);
  void                  RunDeath(void);
  void                  Fading(void);
  
  void                StartRunAnimation(void);
  void                StopRunAnimation(InstanceDynamicObject*);
  
  void                RequestCharacter(ObjectCharacter*, ObjectCharacter*, const std::string& func);

  void                CallbackActionUse(InstanceDynamicObject* object);
  void                DebugPathfinding(void);
  
  Sync::ObserverHandler          _obs_handler;

  PT(Character)                  _character;
  Pathfinding::Path              _path;
  GoToData                       _goToData;

  DataTree*                      _statistics;
  StatController*                _stats;
  const WorldDiplomacy::Faction* _faction;
  unsigned int                   _self_enemyMask;
  unsigned short                 _actionPoints;
  bool                           _running;
  short                          _hitPoints, _armorClass, _tmpArmorClass;
  unsigned char                  _flags;
  bool                           _fading_off, _fading_in, _rotating;
  float                          _rotation_goal;

  public:
    InventoryObject*             active_object;
    unsigned short               active_object_it;
  private:

  // Inventory and Equiped Items
  struct ItemEquiped
  {
    ItemEquiped(void) : actionIt(0), equiped(0), default_(0), graphics(0) {}
    unsigned char                  actionIt;
    EquipedMode                    mode;
    InventoryObject*               equiped;
    InventoryObject*               default_;
    InventoryObject::EquipedModel* graphics;
    NodePath                       jointHorn, jointBattleSaddle, jointMouth;
  };

  Inventory*                _inventory;
  ItemEquiped               _equiped[2];
  std::list<CharacterBuff*> _buffs;

  LineOfSight               line_of_sight;
  FieldOfView               field_of_view;
  
  // Script
  AngelScript::Object*      _script;
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectTypes::Character }; };

#endif
