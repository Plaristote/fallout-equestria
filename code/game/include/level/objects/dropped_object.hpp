#ifndef  DROPPED_OBJECT_HPP
# define DROPPED_OBJECT_HPP

# include "level/objects/instance_dynamic_object.hpp"
# include "level/inventory.hpp"

class ObjectItem : public InstanceDynamicObject
{
public:
  ObjectItem(Level* level, DynamicObject* object, InventoryObject* item);

  bool              HasOccupiedWaypoint(void) const { return (has_waypoint); }
  Pathfinding::Path GetPathTowardsObject(Pathfinding::Collider* character);
  void              ActionUse(InstanceDynamicObject* object);
  void              ProcessCollisions(void) {}

private:
  InventoryObject*  _item;
  bool              has_waypoint;
};

template<> struct ObjectType2Code<ObjectItem> { enum { Type = ObjectTypes::Item }; };

#endif
