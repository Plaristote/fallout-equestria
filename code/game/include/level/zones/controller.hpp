#ifndef  ZONES_CONTROLLER_HPP
# define ZONES_CONTROLLER_HPP

# include "level/objectnode.hpp"

namespace Zones
{
  typedef Sync::Signal<void (InstanceDynamicObject*)> Event;

  class Manager;
  class Observer;

  class Controller
  {
    friend class Zones::Manager;
    friend class Zones::Observer;

    struct Resident
    {
      Resident(InstanceDynamicObject* object) : object(object), waypoint(object->GetOccupiedWaypoint()) {}

      bool operator==(InstanceDynamicObject* object) const { return (this->object == object);                    }
      bool HasMoved(void)                            const { return (object->GetOccupiedWaypoint() == waypoint); }

      InstanceDynamicObject* object;
      Waypoint*              waypoint;
    };

    typedef std::list<Resident>      Residents;
  public:
    Controller(Zone& zone);

    void         SetManager(Manager* manager) { this->manager = manager; }
    bool         operator==(const std::string& name) const { return (zone.name == name);            }
    bool         IsExitZone(void)                    const { return (zone.destinations.size() > 0); }
    bool         IsEnabled(void)                     const { return (enabled);                      }
    bool         IsInZone(InstanceDynamicObject*)    const;
    void         Refresh(void);
    void         SetEnabled(bool);
    void         DisableZone(void);
    void         AddDestination(const std::string& name) { zone.AddDestination(name); }
    void         DelDestination(const std::string& name) { zone.DelDestination(name); }
    void         GoFromHereTo(const std::string& destination);
    void         InsertObject(InstanceDynamicObject*);

    Event        EnteredZone;
    Event        MovedWithinZone;
    Event        ExitedZone;

  protected:
    void         InsertObjectOnWaypoint(InstanceDynamicObject*,Waypoint*);
    void         ObjectMovesWithinZone(InstanceDynamicObject*);
    void         ExitingZone(InstanceDynamicObject*);
    void         RegisterResident(InstanceDynamicObject*);
    void         InsertResident(Resident);
    void         RefreshResident(Resident&);

  private:
    Manager*     manager;
    Zone&        zone;
    Residents    residents;
    bool         enabled;
  };  
}

#endif