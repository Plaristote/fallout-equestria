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

class Character;

typedef std::list<Character*> Characters;

class Character : public ObjectNode
{
public:
  static ObjectNode*          Factory(WindowFramework*, Tilemap&, Characters&, Data);

  template<class T>
  static Characters::iterator Find(Characters& list, T compare)
  {
    Characters::iterator it  = list.begin();
    Characters::iterator end = list.end();

    for (; it != end ; ++it)
    {
      if ((**it) == compare)
        break ;
    }
    return (it);
  }

  Character(WindowFramework* window, Tilemap& map, Data data, Characters& chars);
  virtual ~Character() {}

  bool               operator==(NodePath comp)           const { return (_root == comp || _root.is_ancestor_of(comp)); }
  bool               operator==(const std::string& name) const { return (_root.get_name() == name);                    }

  virtual void       Run(float elapsedTime);
  unsigned short     GoTo(int x, int y);
  bool               TryToReach(ObjectNode*, int min_distance = 0);
  bool               CanReach(ObjectNode*, int min_distance = 0);
  bool               HasLineOfSight(Character*);
  bool               IsMoving(void) { return (!(_path.empty())); }

  std::string        GetName(void) const      { return (_root.get_name()); }
  Inventory&         GetInventory(void)       { return (_inventory);       }
  const Inventory&   GetInventory(void) const { return (_inventory);       }
  
  Pathfinding::Node  GetCurrentDestination(void) const;

  Observatory::Signal<void (Character*)> ReachedCase;

  void               DebugShowCollisionSphere(bool set) { (set ? _selfSphereNP.show() : _selfSphereNP.hide()); }

protected:
  bool              IsArcAccessible(int beg_x, int beg_y, int dest_x, int dest_y);

  typedef std::list<Pathfinding::Node> DirectionPath;

  Inventory     _inventory;
  DirectionPath _path;
  bool          _lookingForNewWay;
  Characters&   _characters;
private:
  void DoMovement(float elapsedTime);

  PT(CollisionSphere)       _selfSphere;
  PT(CollisionNode)         _selfSphereNode;
  NodePath                  _selfSphereNP;

  PT(CollisionSphere)       _collisionFov;
  PT(CollisionNode)         _collisionNode;
  NodePath                  _collisionPath;
  CollisionTraverser        _collisionTraverser;
  PT(CollisionHandlerQueue) _collisionHandlerQueue;
  Timer                     _timerFov;

  PT(CollisionRay)          _losRay;
  PT(CollisionNode)         _losNode;
  NodePath                  _losPath;
  CollisionTraverser        _losTraverser;
  PT(CollisionHandlerQueue) _losHandlerQueue;

  // Light
  PT(PointLight) _charLight;
  NodePath       _charLightNode;
};

#endif