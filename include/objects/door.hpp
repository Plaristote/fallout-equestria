#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "objectnode.hpp"
# include "character.hpp"

/*class Door : public ObjectNode, public Pathfinding::Node::Arc::Observer
{
public:
  static ObjectNode* Factory(WindowFramework*, Tilemap&, Characters&, Data);
  
  Door(WindowFramework* window, Tilemap& map, Data data);

  virtual void  InteractUse(Character* c);
  virtual void  ProcessCollision(Pathfinding* map);

  virtual bool  CanGoThrough(Character*);
  virtual bool  GoingThrough(Character*);

private:
  bool _opened;
  bool _vertical;
};*/

#endif