#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "level/objectnode.hpp"

class Lockable
{
public:
  Lockable(DynamicObject* o) : __object(o) {}
  Lockable(void) {}

  string         GetKeyName(void) const { return (__object->key);              }
  bool           IsLocked(void)   const { return (__object->locked);           }
  bool           IsOpen(void)     const { return (!_closed);                  }
  void           Unlock(void)           { __object->locked = !__object->locked; }

protected:
  bool           _closed;
private:
  DynamicObject* __object;
};

class ObjectDoor : public InstanceDynamicObject, public Lockable, public Waypoint::ArcObserver
{
public:
  ObjectDoor(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object), Lockable(object)
  {
    _type   = ObjectTypes::Door;
    _closed = true;
    ObserveWaypoints(true);
  }
  
  ~ObjectDoor()
  {
    ObserveWaypoints(false);
  }
  
  void     ProcessCollisions(void);

  string   GetKeyName() const { return (_object->key); }
  
  void     CallbackActionUse(InstanceDynamicObject* object);
  GoToData GetGoToData(InstanceDynamicObject* character);
  void     ObserveWaypoints(bool doObserver);
  
  bool     CanGoThrough(unsigned char id);
  void     GoingThrough(void*);
  
  std::list<std::pair<int, int> > _workaround_wp_disconnected;
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectTypes::Door      }; };

#endif
