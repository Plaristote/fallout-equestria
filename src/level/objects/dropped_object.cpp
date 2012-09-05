#include "level/objects/dropped_object.hpp"
#include "level/character.hpp"
#include "level/level.hpp"

ObjectItem::ObjectItem(Level* level, DynamicObject* object, InventoryObject* item) : InstanceDynamicObject(level, object), _item(item)
{
  _type = ObjectTypes::Item;
}

void ObjectItem::CallbackActionUse(InstanceDynamicObject* user)
{
  ObjectCharacter* character = user->Get<ObjectCharacter>();
  
  if (character)
  {
    Inventory& inventory = character->GetInventory();
    
    if (inventory.CanCarry(_item))
    {
      inventory.AddObject(_item);
      _level->RemoveObject(this);
    }
    else
      _level->ConsoleWrite(character->GetName() + " can't carry this");
  }
}
