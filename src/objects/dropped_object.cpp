#include "objects/dropped_object.hpp"
#include "character.hpp"

ObjectNode* DroppedObject::Factory(WindowFramework* window, Tilemap& map, Characters&, Data data)
{
  InventoryObject* object    = new InventoryObject(data);

  return (new DroppedObject(window, map, *object));
}

DroppedObject::DroppedObject(WindowFramework* window, Tilemap& map, InventoryObject& object) :
  ObjectNode(window, map, object), _object(object)
{
  SetCollisionEnabled(false);
}

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
      c->ReachedCase.Connect(*this, &DroppedObject::InteractUse);
    else
      std::cout << "[GameConsole] Can't reach !" << std::endl;
  }
}