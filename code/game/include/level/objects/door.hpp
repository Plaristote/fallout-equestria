#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "level/objects/instance_dynamic_object.hpp"
# include "level/zones/passage_way.hpp"

class Lockable
{
public:
  Lockable(DynamicObject* o) : __object(o) {}
  Lockable(void) {}

  string         GetKeyName(void) const { return (__object->key);               }
  bool           IsLocked(void)   const { return (__object->locked);            }
  bool           IsOpen(void)     const { return (!_closed);                    }
  void           Unlock(void)           { __object->locked = !__object->locked; }

protected:
  bool           _closed;
private:
  DynamicObject* __object;
};

class ObjectDoor : public InstanceDynamicObject, public Lockable
{
public:
  ObjectDoor(Level* level, DynamicObject* object);  
  ~ObjectDoor();

  bool               HasOccupiedWaypoint(void) const { return (false);        }
  
  void               ProcessCollisions(void);
  void               UnprocessCollision(void);

  void               CallbackActionUse(InstanceDynamicObject* object);
  
private:
  void               InitializePassageWay(void);
  bool               CanGoThrough(InstanceDynamicObject*);
  void               GoingThrough(InstanceDynamicObject*);

  Zones::PassageWay* passage_way;
  bool               collision_enabled;
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectTypes::Door      }; };

#endif
