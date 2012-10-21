#ifndef  LOCKER_HPP
# define LOCKER_HPP

# include "door.hpp"
# include "shelf.hpp"

class ObjectLocker : public ObjectShelf, public Lockable
{
public:
  ObjectLocker(Level* level, DynamicObject* object);

  virtual void CallbackActionUse(InstanceDynamicObject*);
  string       GetKeyName() const { return (_object->key); }

private:
  void         PendingActionOpen(InstanceDynamicObject*);
};

template<> struct ObjectType2Code<ObjectLocker>      { enum { Type = ObjectTypes::ObjectType::Locker      }; };

#endif
