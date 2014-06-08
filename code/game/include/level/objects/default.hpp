#ifndef  OBJECT_DEFAULT_HPP
# define OBJECT_DEFAULT_HPP

# include "level/objects/instance_dynamic_object.hpp"
# include "level/inventory.hpp"

class ObjectDefault : public InstanceDynamicObject
{
public:
  ObjectDefault(Level* level, DynamicObject* object);

  virtual ~ObjectDefault()
  {
  }

  Inventory& GetInventory(void) { return (inventory); }

private:
  Inventory            inventory;
  AngelScript::Object* script;
};

template<> struct ObjectType2Code<ObjectDefault> { enum { Type = ObjectTypes::Default }; };

#endif
