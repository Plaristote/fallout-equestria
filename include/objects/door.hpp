#ifndef  OBJECT_DOOR_HPP
# define OBJECT_DOOR_HPP

# include "objectnode.hpp"

class LevelExitZone : public Waypoint::ArcObserver
{
public:
  LevelExitZone(Level* level, std::list<std::string> destinations) : _level(level)
  {
    ForEach(destinations, [this](std::string dest) { _destinations.push_back(dest); });
  }
  
  void               SetName(const std::string& name) { _name = name;   }
  const std::string& GetName(void) const              { return (_name); }
  
  bool CanGoThrough(unsigned char id) { return (true); }
  void GoingThrough(void* character);
  
  Observatory::Signal<void (void)>                            ExitZone;
  Observatory::Signal<void (const std::string&)>              GoToNextZone;
  Observatory::Signal<void (const std::vector<std::string>&)> SelectNextZone;

private:
  std::string              _name;
  Level*                   _level;
  std::vector<std::string> _destinations;
};

class ObjectDoor : public InstanceDynamicObject, public Waypoint::ArcObserver
{
public:
  ObjectDoor(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
  {
    _type   = ObjectTypes::Door;
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
  void     GoingThrough(void*);

private:
  bool _closed;
  bool _locked;
};

template<> struct ObjectType2Code<ObjectDoor>      { enum { Type = ObjectTypes::ObjectType::Door      }; };

#endif