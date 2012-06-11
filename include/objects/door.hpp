#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "objectnode.hpp"

class ObjectDoor : public InstanceDynamicObject, public Waypoint::ArcObserver
{
public:
  ObjectDoor(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
    _type   = Door;
    _closed = true;
    _locked = object->locked;
    ObserveWaypoints(true);
  }
  
  ~ObjectDoor()
  {
    ObserveWaypoints(false);
  }

  string   GetKeyName() const { return (_object->key); }
  void     Unlock(void)   { _locked = !_locked; }
  bool     IsLocked(void) { return (_locked);   }
  bool     IsOpen(void)   { return (!_closed);  }
  
  void     CallbackActionUse(InstanceDynamicObject* object);
  GoToData GetGoToData(InstanceDynamicObject* character);  
  void     ObserveWaypoints(bool doObserver);
  
  void     ProcessCollisions(void) {}

  bool     CanGoThrough(unsigned char id);
  void     GoingThrough(void);

private:
  bool _closed;
  bool _locked;
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectType::Door      }; };

#endif