#ifndef  DROPPED_OBJECT_HPP
# define DROPPED_OBJECT_HPP

# include "level/objectnode.hpp"
# include "level/inventory.hpp"
# include "level/character.hpp"

class ObjectItem : public InstanceDynamicObject
{
public:
  ObjectItem(Level* level, DynamicObject* object, InventoryObject* item);

  void     CallbackActionUse(InstanceDynamicObject* object);
  void     ProcessCollisions(void) {}

private:
  InventoryObject*  _item;
};

template<> struct ObjectType2Code<ObjectItem> { enum { Type = ObjectTypes::Item }; };

#endif