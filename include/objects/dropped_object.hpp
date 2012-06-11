#ifndef  DROPPED_OBJECT_HPP
# define DROPPED_OBJECT_HPP

# include "objectnode.hpp"
# include "inventory.hpp"
# include "character.hpp"

class ObjectItem : public InstanceDynamicObject
{
public:
  ObjectItem(Level* level, DynamicObject* object, InventoryObject* item);

  void     CallbackActionUse(InstanceDynamicObject* object);
  void     ProcessCollisions(void) {}

private:
  InventoryObject*  _item;
};

template<> struct ObjectType2Code<ObjectItem> { enum { Type = ObjectType::Item }; };

#endif