#include "level/objects/shelf.hpp"
#include "level/level.hpp"
#include <algorithm>

using namespace std;
using namespace Sync;

ObjectShelf::ObjectShelf(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  _type   = ObjectTypes::Shelf;
  _inventory.LoadInventory(object);
  _inventory.SetCapacity(450);
}

void ObjectShelf::CallbackActionUse(InstanceDynamicObject* object)
{
  if (object == _level->GetPlayer())
    _level->GetInteractions().ActionLoot(&_inventory);
}
