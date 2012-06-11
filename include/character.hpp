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
  unsigned short      GetPathDistance(Waypoint* waypoint);
  unsigned short      GetPathDistance(InstanceDynamicObject* object);
  bool                HasLineOfSight(InstanceDynamicObject* object);
  bool                IsMoving(void) const { return (_path.size()); }
  Inventory&          GetInventory(void)   { return (_inventory);   }
  Data                GetStatistics(void)  { return (_statistics);  }

private:
  void                RunMovement(float elapsedTime);
  void                RunMovementNext(float elaspedTime);

  std::list<Waypoint>       _path;
  GoToData                  _goToData;
  
  Inventory                 _inventory;
  DataTree*                 _statistics;
  
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