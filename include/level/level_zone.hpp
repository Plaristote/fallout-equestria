#ifndef  LEVEL_ZONE_HPP
# define LEVEL_ZONE_HPP

# include "level/objectnode.hpp"

class LevelZone : public Waypoint::ArcObserver
{
  typedef std::list<InstanceDynamicObject*> Residents;
public:
  LevelZone(Level* level, Zone& zone);
  virtual ~LevelZone();

  void                     SetName(const std::string& name) { _name    = name;    }
  const std::string&       GetName(void) const              { return (_name);     }
  void                     SetEnabled(bool enabled)         { _enabled = enabled; }
  bool                     IsEnabled(void) const            { return (_enabled);  }
  bool                     IsInside(InstanceDynamicObject* object) const;

  bool                     CanGoThrough(unsigned char)      { return (true);  }
  virtual void             GoingThrough(void* object);
  void                     Refresh(void);

  Sync::Signal<void (InstanceDynamicObject*)> Entered;
  Sync::Signal<void (InstanceDynamicObject*)> Exited;
  
protected:
  Residents   _objects;
  std::string _name;
  Level*      _level;
  Zone&       _zone;
  bool        _enabled;
};

class LevelExitZone : public LevelZone
{
public:
  LevelExitZone(Level* level, Zone& zone, std::list<std::string> destinations);
  virtual ~LevelExitZone(void) {}

  void                     GoingThrough(void* character);
  
  Sync::Signal<void (void)>                            ExitZone;
  Sync::Signal<void (const std::string&)>              GoToNextZone;
  Sync::Signal<void (const std::vector<std::string>&)> SelectNextZone;
  
private:
  void                     GoingThroughLocal(void* character);
  void                     GoingThroughExit(void* character);
  
  std::vector<std::string> _destinations;
};

#endif