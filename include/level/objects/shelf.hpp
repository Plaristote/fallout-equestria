#ifndef  OBJECT_SHELF_HPP
# define OBJECT_SHELF_HPP

# include "level/objectnode.hpp"
# include "level/inventory.hpp"

class ObjectShelf : public InstanceDynamicObject
{
public:
  ObjectShelf(Level* level, DynamicObject* object);

  ~ObjectShelf()
  {
    LockWaypoints(false);
  }

  void         LockWaypoints(bool);
  GoToData     GetGoToData(InstanceDynamicObject*);
  Inventory&   GetInventory(void)      { return (_inventory); }

  virtual void CallbackActionUse(InstanceDynamicObject*);
private:
  Inventory   _inventory;
};

template<> struct ObjectType2Code<ObjectShelf>      { enum { Type = ObjectTypes::ObjectType::Shelf }; };

#endif