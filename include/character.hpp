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
  bool                IsMoving(void) const { return (_path.size()); }
  Inventory&          GetInventory(void)   { return (_inventory);   }
  Data                GetStatistics(void)  { return (_statistics);  }

  unsigned short      GetActionPoints(void) const        { return (_actionPoints); }
  void                SetActionPoints(unsigned short ap) { _actionPoints = ap; ActionPointChanged.Emit(_actionPoints); }
  void                RestartActionPoints(void);
  Observatory::Signal<void (unsigned short)> ActionPointChanged;
  
  void                SetEquipedItem(unsigned short it, InventoryObject* object);
  InventoryObject*    GetEquipedItem(unsigned short it);
  void                UnequipItem(unsigned short it);
  Observatory::Signal<void (unsigned short, InventoryObject*)> EquipedItemChanged;

private:
  void                RunMovement(float elapsedTime);
  void                RunMovementNext(float elaspedTime);

  std::list<Waypoint>       _path;
  GoToData                  _goToData;

  Inventory                 _inventory;
  InventoryObject*          _equiped[2];
  InventoryObject*          _defEquiped[2];
  DataTree*                 _statistics;

  unsigned short            _actionPoints;
  
  // Line of Sight Tools
  NodePath                  _losPath;
  PT(CollisionRay)          _losRay;
  PT(CollisionNode)         _losNode;
  PT(CollisionHandlerQueue) _losHandlerQueue;
  CollisionTraverser        _losTraverser;
  
  // Script
  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction* _scriptMain;
  asIScriptFunction* _scriptFight;
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectType::Character }; };

#endif