#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "globals.hpp"
# include "objectnode.hpp"
# include "inventory.hpp"
# include "timer.hpp"
# include <panda3d/collisionRay.h>
# include <panda3d/collisionSphere.h>
# include <panda3d/collisionTraverser.h>
# include <panda3d/collisionHandlerQueue.h>

# include <panda3d/pointLight.h>

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
  
  ObjectCharacter(Level* level, DynamicObject* object);

  Observatory::Signal<void (InstanceDynamicObject*)> ReachedDestination;
  
  virtual GoToData   GetGoToData(InstanceDynamicObject* character)
  {
    GoToData         ret;

    ret.nearest      = _waypointOccupied;
    ret.objective    = this;
    ret.max_distance = 0;
    ret.min_distance = 1;
    return (ret);
  }
  
  void                ProcessCollisions() { if (_hitPoints > 0) InstanceDynamicObject::ProcessCollisions(); }

  void                Run(float elapsedTime);
  void                GoTo(unsigned int id);
  void                GoTo(Waypoint* waypoint);
  void                GoTo(InstanceDynamicObject* object, int max_distance = 0);
  void                GoToRandomWaypoint(void);
  void                TruncatePath(unsigned short max_length);
  unsigned short      GetPathDistance(Waypoint* waypoint);
  unsigned short      GetPathDistance(InstanceDynamicObject* object);
  float               GetDistance(InstanceDynamicObject* object);
  int                 GetBestWaypoint(InstanceDynamicObject* object, bool farthest);
  int                 GetNearestWaypoint(InstanceDynamicObject* object);
  int                 GetFarthestWaypoint(InstanceDynamicObject* object);
  bool                HasLineOfSight(InstanceDynamicObject* object);
  bool                IsMoving(void) const { return (_path.size());   }
  bool                IsAlive(void) const  { return (_hitPoints > 0); }
  Inventory&          GetInventory(void)   { return (_inventory);     }
  Data                GetStatistics(void)  { return (_statistics);    }
  Diplomacy&          GetDiplomacy(void)   { return (_diplomacy);     }

  unsigned short      GetActionPoints(void) const        { return (_actionPoints); }
  void                SetActionPoints(unsigned short ap) { _actionPoints = ap; ActionPointChanged.Emit(_actionPoints); }
  void                RestartActionPoints(void);
  Observatory::Signal<void (unsigned short)> ActionPointChanged;
  
  short               GetHitPoints(void) const        { return (_hitPoints); }
  void                SetHitPoints(short hp)
  {
    std::cout << "HitPoints are now " << hp << std::endl;
    _hitPoints = hp;
    HitPointsChanged.Emit(_hitPoints);
    if (hp <= 0)
      CharacterDied.Emit();
  }
  Observatory::Signal<void (unsigned short)> HitPointsChanged;
  Observatory::Signal<void>                  CharacterDied;
  
  short               GetArmorClass(void) const        { return (_armorClass); }
  void                SetArmorClass(short ac)          { _armorClass = ac; ArmorClassChanged.Emit(_armorClass); }
  void                RestartArmorClass(void)          { _armorClass -= _tmpArmorClass; _tmpArmorClass = 0;     }
  void                SetBonusAC(short ac)             { _armorClass += ac; _tmpArmorClass += ac;               }
  Observatory::Signal<void (short)>          ArmorClassChanged;
  
  void                SetEquipedItem(unsigned short it, InventoryObject* object);
  InventoryObject*    GetEquipedItem(unsigned short it);
  void                UnequipItem(unsigned short it);
  Observatory::Signal<void (unsigned short, InventoryObject*)> EquipedItemChanged;
  
  void                CheckFieldOfView(void);

  void                SetAsEnemy(ObjectCharacter*, bool);
  bool                IsEnemy(ObjectCharacter*) const;
  bool                IsAlly(ObjectCharacter*)  const;

private:
  void                RunMovement(float elapsedTime);
  void                RunMovementNext(float elaspedTime);
  void                RunDeath(void);
  
  void                StartRunAnimation(void);
  void                StopRunAnimation(InstanceDynamicObject*);
  
  void                CallbackActionUse(InstanceDynamicObject* object);

  std::list<Waypoint>       _path;
  GoToData                  _goToData;

  Inventory                 _inventory;
  InventoryObject*          _equiped[2];
  InventoryObject*          _defEquiped[2];
  DataTree*                 _statistics;
  Diplomacy                 _diplomacy;

  unsigned short            _actionPoints;
  short                     _hitPoints, _armorClass, _tmpArmorClass;

  // Line of Sight Tools
  NodePath                  _losPath;
  PT(CollisionRay)          _losRay;
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
  
  struct FovEnemy
  {
    FovEnemy(ObjectCharacter* enemy, unsigned char ttl) : enemy(enemy), ttl(ttl) {}
    bool operator==(ObjectCharacter* comp) const { return (enemy == comp); }
    ObjectCharacter* enemy;
    unsigned char    ttl;
  };
  
  std::list<FovEnemy>         _fovEnemies;
  std::list<ObjectCharacter*> _fovAllies;
  
  // Script
  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction* _scriptMain;
  asIScriptFunction* _scriptFight;
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectType::Character }; };

#endif