#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "objectnode.hpp"
# include "inventory.hpp"
# include <panda3d/collisionSphere.h>
# include <panda3d/collisionTraverser.h>
# include <panda3d/collisionHandlerQueue.h>

# include <panda3d/pointLight.h>

class Character;

typedef std::list<Character*> Characters;

class Character : public ObjectNode
{
public:
  static ObjectNode* Factory(WindowFramework*, Tilemap&, Characters&, Data);
  
  Character(WindowFramework* window, Tilemap& map, Data data, Characters& chars);
  virtual void      Run(float elapsedTime);
  unsigned short    GoTo(int x, int y);
  bool              TryToReach(ObjectNode*, int min_distance = 0);
  bool              CanReach(ObjectNode*, int min_distance = 0);

  Inventory&        GetInventory(void)       { return (_inventory); }
  const Inventory&  GetInventory(void) const { return (_inventory); }
  
  Pathfinding::Node GetCurrentDestination(void) const;

  Observatory::Signal<void (Character*)> ReachedCase;

protected:
  bool              IsArcAccessible(int beg_x, int beg_y, int dest_x, int dest_y);

  typedef std::list<Pathfinding::Node> DirectionPath;

  Inventory     _inventory;
  DirectionPath _path;
  bool          _lookingForNewWay;
  Characters    _characters;
private:
  void DoMovement(float elapsedTime);


  PT(CollisionSphere)       _collisionFov;
  PT(CollisionNode)         _collisionNode;
  NodePath                  _collisionPath;
  CollisionTraverser        _collisionTraverser;
  PT(CollisionHandlerQueue) _collisionHandlerQueue;

  // Light
  PT(PointLight) _charLight;
  NodePath       _charLightNode;
};

#endif