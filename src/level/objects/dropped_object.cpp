#include "level/objects/dropped_object.hpp"
#include "level/character.hpp"
#include "level/level.hpp"

using namespace std;

ObjectItem::ObjectItem(Level* level, DynamicObject* object, InventoryObject* item) : InstanceDynamicObject(level, object), _item(item)
{
  _type = ObjectTypes::Item;
  if (object->inventory.size() == 0)
  {
    string str_data;

    DataTree::Writers::StringJSON(*item, str_data);
    object->inventory.push_back(pair<string, int>(str_data, 1));
    object->key = item->Key();
  }
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
