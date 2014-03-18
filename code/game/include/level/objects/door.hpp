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

  bool               HasOccupiedWaypoint(void) const;
  bool               CanGoThrough(InstanceDynamicObject*) const;
  
  void               ProcessCollisions(void);
  void               UnprocessCollisions(void);
  
  void               Open(void)  { SetOpened(true);  }
  void               Close(void) { SetOpened(false); }
  void               SetOpened(bool set_open);

  void               ActionUse(InstanceDynamicObject* object);
  
private:
  void               InitializePassageWay(void);
  void               GoingThrough(InstanceDynamicObject*);
  bool               IsWayBlocked(void);

  Zones::PassageWay* passage_way;
  bool               collision_enabled;
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectTypes::Door      }; };

#endif
