#ifndef  OBJECT_SHELF_HPP
# define OBJECT_SHELF_HPP

# include "level/objects/instance_dynamic_object.hpp"
# include "level/inventory.hpp"

class ObjectShelf : public InstanceDynamicObject
{
public:
  ObjectShelf(Level* level, DynamicObject* object);

  ~ObjectShelf()
  {
  }

  Inventory&   GetInventory(void)      { return (_inventory); }

  virtual void ActionUse(InstanceDynamicObject*);
private:
  Inventory   _inventory;
};

template<> struct ObjectType2Code<ObjectShelf>      { enum { Type = ObjectTypes::Shelf }; };

#endif
