#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "objectnode.hpp"

class Door : public ObjectNode, public Pathfinding::Node::Arc::Observer
{
public:
  Door(WindowFramework* window, Tilemap& map, Data data);

  virtual void  InteractUse(Character* c);
  virtual void  ProcessCollision(Pathfinding* map);

  virtual bool  CanGoThrough(Character*);
  virtual bool  GoingThrough(Character*);

private:
  bool _opened;
  bool _vertical;
};

#endif