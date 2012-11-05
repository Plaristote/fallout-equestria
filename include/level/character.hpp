#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "globals.hpp"
# include "objectnode.hpp"
# include "inventory.hpp"
# include "timer.hpp"
# include "statsheet.hpp"
# include <panda3d/collisionRay.h>
# include <panda3d/collisionSegment.h>
# include <panda3d/collisionSphere.h>
# include <panda3d/collisionTraverser.h>
# include <panda3d/collisionHandlerQueue.h>
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
  struct Faction
  {
    unsigned int flag;
    unsigned int enemyMask;
  };

  class Diplomacy
  {
  public:
    void     SetEnemyMask(unsigned int m) { _enemyMask = m;     }
    void     SetFaction(Faction* faction) { _faction = faction; }
    Faction* GetFaction(void) const       { return (_faction);  }
    bool     IsEnemyWith(Diplomacy& other) const;
    bool     IsAlly(Diplomacy& other)      const;
    void     SetAsEnemy(Diplomacy& other, bool enemy = true);

  private:
    Faction*     _faction;
    unsigned int _enemyMask;
  };
  
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

  Observatory::Signal<void (InstanceDynamicObject*)>         ReachedDestination;
  Observatory::Signal<void (unsigned short, unsigned short)> ActionPointChanged;
  Observatory::Signal<void (short)>                          HitPointsChanged;
  Observatory::Signal<void>                                  CharacterDied;

  virtual GoToData   GetGoToData(InstanceDynamicObject* character)
  {
    GoToData         ret;

    ret.nearest      = _waypointOccupied;
    ret.objective    = this;
    ret.max_distance = 0;
    ret.min_distance = 1;
    return (ret);
  }
  
  void                SetInventory(Inventory* inventory)
  {
    if (inventory)
    {
      if (_inventory) delete _inventory;
      _inventory = inventory;
    }
  }
  
  void                ProcessCollisions() { if (_hitPoints > 0) InstanceDynamicObject::ProcessCollisions(); }

  void                Run(float elapsedTime);
  void                LookAt(LVecBase3);
  void                LookAt(InstanceDynamicObject*);
  void                GoTo(unsigned int id);
  void                GoTo(Waypoint* waypoint);
  void                GoTo(InstanceDynamicObject* object, int max_distance = 0);
  void                GoToRandomWaypoint(void);
  void                TruncatePath(unsigned short max_length);
  std::list<Waypoint> GetPath(Waypoint* waypoint);
  unsigned short      GetPathDistance(Waypoint* waypoint);
  unsigned short      GetPathDistance(InstanceDynamicObject* object);
  float               GetDistance(InstanceDynamicObject* object);
  int                 GetBestWaypoint(InstanceDynamicObject* object, bool farthest);
  int                 GetNearestWaypoint(InstanceDynamicObject* object);
  int                 GetFarthestWaypoint(InstanceDynamicObject* object);
  bool                HasLineOfSight(InstanceDynamicObject* object);
  bool                IsMoving(void) const      { return (_path.size());          }
  bool                IsAlive(void) const       { return (_hitPoints > 0);        }
  bool                IsInterrupted(void) const { return (AnimationEnded.ObserverCount() > 0); }
  Inventory&          GetInventory(void)        { return (*_inventory);           }
  Data                GetStatistics(void)       { return (_statistics ? Data(_statistics) : Data()); }
  StatController*     GetStatController(void)   { return (_stats);                }
  Diplomacy&          GetDiplomacy(void)        { return (_diplomacy);            }

  unsigned short      GetActionPoints(void) const        { return (_actionPoints); }
  void                SetActionPoints(unsigned short ap)
  {
    _actionPoints = ap;
    if (_statistics)
      ActionPointChanged.Emit(_actionPoints, Data(_statistics)["Statistics"]["Action Points"]);
  }
  void                RestartActionPoints(void);
  
  short               GetHitPoints(void) const        { return (_hitPoints); }
  void                SetHitPoints(short hp);
  void                StatHpUpdate(short);
  
  short               GetArmorClass(void) const        { return (_armorClass); }
  void                SetArmorClass(short ac)          { _armorClass = ac; ArmorClassChanged.Emit(_armorClass); }
  void                RestartArmorClass(void)          { _armorClass -= _tmpArmorClass; _tmpArmorClass = 0;     }
  void                SetBonusAC(short ac)             { _armorClass += ac; _tmpArmorClass += ac;               }
  Observatory::Signal<void (short)>          ArmorClassChanged;

  void                PlayEquipedItemAnimation(unsigned short it, const std::string& name);
  void                SetEquipedItem(unsigned short it, InventoryObject* object, EquipedMode mode = EquipedMouth);
  InventoryObject*    GetEquipedItem(unsigned short it);
  unsigned char       GetequipedAction(unsigned short it) const { return (_equiped[it].actionIt); }
  void                UnequipItem(unsigned short it);
  void                ItemNextUseType(unsigned short it);
  Observatory::Signal<void (unsigned short, InventoryObject*)>                EquipedItemChanged;
  Observatory::Signal<void (unsigned short, InventoryObject*, unsigned char)> EquipedItemActionChanged;

  void                PushBuff(Data, ObjectCharacter* caster);
  void                DelBuff(CharacterBuff* buff);

  void                CheckFieldOfView(void);
  void                SetAsEnemy(ObjectCharacter*, bool);
  bool                IsEnemy(ObjectCharacter*) const;
  bool                IsAlly(ObjectCharacter*)  const;
  
  Script::StdList<ObjectCharacter*> GetNearbyEnemies(void) const;
  Script::StdList<ObjectCharacter*> GetNearbyAllies(void)  const;
  
  // Script Communication Tools
  void                RequestAttack(ObjectCharacter* attack, ObjectCharacter* from);
  void                RequestHeal(ObjectCharacter* heal, ObjectCharacter* from);
  void                RequestFollow(ObjectCharacter* follow, ObjectCharacter* from);
  void                RequestStopFollowing(ObjectCharacter* follow, ObjectCharacter* from);
  int                 AskMorale(void);
  void                SendMessage(const std::string&);
  
private:
  void                RunMovement(float elapsedTime);
  void                RunMovementNext(float elaspedTime);
  void                RunDeath(void);
  
  void                StartRunAnimation(void);
  void                StopRunAnimation(InstanceDynamicObject*);
  
  void                RequestCharacter(ObjectCharacter*, ObjectCharacter*, asIScriptFunction*);
  
  void                CallbackActionUse(InstanceDynamicObject* object);
  
  Observatory::ObserverHandler _obs_handler;

  PT(Character)             _character;
  std::list<Waypoint>       _path;
  GoToData                  _goToData;

  DataTree*                 _statistics;
  StatController*           _stats;
  Diplomacy                 _diplomacy;
  unsigned short            _actionPoints;
  short                     _hitPoints, _armorClass, _tmpArmorClass;
  

  // Inventory and Equiped Items
  struct ItemEquiped
  {
    ItemEquiped(void) : equiped(0), default_(0), graphics(0), actionIt(0) {}
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

  // Line of Sight Tools
  NodePath                  _losPath;
  PT(CollisionSegment)      _losRay;
  PT(CollisionNode)         _losNode;
  PT(CollisionHandlerQueue) _losHandlerQueue;
  CollisionTraverser        _losTraverser;
  
  // Field of view
  PT(CollisionSphere)       _fovTargetSphere;
  PT(CollisionNode)         _fovTargetNode;
  NodePath                  _fovTargetNp;
  
  PT(CollisionSphere)       _fovSphere;
  PT(CollisionNode)         _fovNode;
  NodePath                  _fovNp;
  PT(CollisionHandlerQueue) _fovHandlerQueue;
  CollisionTraverser        _fovTraverser;
  
  std::list<FovEnemy>         _fovEnemies;
  std::list<ObjectCharacter*> _fovAllies;
  
  // Script
  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction* _scriptMain;
  asIScriptFunction* _scriptFight;

  asIScriptFunction *_scriptRequestAttack, *_scriptRequestHeal, *_scriptRequestFollow, *_scriptRequestStopFollowing;
  asIScriptFunction *_scriptAskMorale;
  asIScriptFunction* _scriptSendMessage;
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectTypes::ObjectType::Character }; };

#endif