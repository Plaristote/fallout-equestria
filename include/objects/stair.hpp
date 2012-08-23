#ifndef  STAIR_HPP
# define STAIR_HPP

# include "objectnode.hpp"

class ObjectStair : public InstanceDynamicObject, public Waypoint::ArcObserver
{
public:
  ObjectStair(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
    _type = ObjectTypes::Stair;
    object->lockedArcs;
  }

  ~ObjectStair(void)
  {
  }

private:
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectTypes::ObjectType::Stair     }; };

#endif
