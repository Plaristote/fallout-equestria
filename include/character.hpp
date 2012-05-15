#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "objectnode.hpp"
# include "inventory.hpp"

class Character : public ObjectNode
{
public:
  Character(WindowFramework* window, Tilemap& map, Data data) : ObjectNode(window, map, data) {}

  virtual void     Run(float elapsedTime);
  bool             GoTo(int x, int y);
  bool             TryToReach(ObjectNode*, int min_distance = 0);
  bool             CanReach(ObjectNode*, int min_distance = 0);

  Inventory&       GetInventory(void)       { return (_inventory); }
  const Inventory& GetInventory(void) const { return (_inventory); }

protected:
  typedef std::list<Pathfinding::Node> DirectionPath;

  Inventory     _inventory;
  DirectionPath _path;
private:
  void DoMovement(float elapsedTime);
};

#endif