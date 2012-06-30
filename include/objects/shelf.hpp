#ifndef  OBJECT_SHELF_HPP
# define OBJECT_SHELF_HPP

# include "objectnode.hpp"
# include "inventory.hpp"

class ObjectShelf : public InstanceDynamicObject
{
public:
  ObjectShelf(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
    _type   = ObjectTypes::Shelf;
    LockWaypoints(true);
    _inventory.LoadInventory(object);
  }
  
  ~ObjectShelf()
  {
    LockWaypoints(false);
  }

  void        ProcessCollisions(void) {}
  void        LockWaypoints(bool);
  GoToData    GetGoToData(InstanceDynamicObject*);
  Inventory&  GetInventory(void)      { return (_inventory); }
  
  void        CallbackActionUse(InstanceDynamicObject*);

private:
  Inventory   _inventory;
};

template<> struct ObjectType2Code<ObjectShelf>      { enum { Type = ObjectTypes::ObjectType::Shelf }; };

#endif