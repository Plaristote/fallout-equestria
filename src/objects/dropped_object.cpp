#include "objects/dropped_object.hpp"
#include "character.hpp"

DroppedObject::DroppedObject(WindowFramework* window, Tilemap& map, InventoryObject& object) :
  ObjectNode(window, map, object), _object(object)
{
  SetCollisionEnabled(false);
}

#define ABS(x) ((x) > 0 ? (x) : -(x))

void  DroppedObject::InteractUse(Character* c)
{
  if (c->CanReach(this, 0))
  {
    c->GetInventory().AddObject(&_object);
    _root.remove_node();
  }
  else
  {
    if (c->TryToReach(this))
      std::cout << "[GameConsole] Can't reach !" << std::endl;
    else
    {
      // TODO create a signal for teh character for when he's done moving, connect this method to it
    }
  }
}