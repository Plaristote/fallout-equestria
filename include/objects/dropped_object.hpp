#ifndef  DROPPED_OBJECT_HPP
# define DROPPED_OBJECT_HPP

# include "objectnode.hpp"
# include "inventory.hpp"

class DroppedObject : public ObjectNode
{
public:
  DroppedObject(WindowFramework* window, Tilemap& map, InventoryObject& object);

  virtual void  InteractUse(Character* c);
  
private:
  InventoryObject& _object;
};

#endif