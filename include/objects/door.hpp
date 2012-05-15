#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "objectnode.hpp"

class Door : public ObjectNode
{
public:
  Door(WindowFramework* window, Tilemap& map, Data data);

  virtual void  InteractUse(Character* c);

  virtual void  ProcessCollision(Pathfinding* map);

private:
  bool _opened;
  bool _vertical;
};

#endif