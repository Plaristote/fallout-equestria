#ifndef  ZONES_MANAGER_HPP
# define ZONES_MANAGER_HPP

# include "level/zones/controller.hpp"
# include "level/zones/observer.hpp"
# include "playerparty.hpp"

class Level;

namespace Zones
{
  class Manager
  {
    friend class Controller;
  public:
    Manager(Level& level);
    ~Manager();

    bool                     ZoneExists(const std::string& name) const;
    const Controller&        GetZone(const std::string& name)    const;
    Controller&              GetZone(const std::string& name);
    void                     RegisterZone(Zone& zone);
    void                     UnregisterZone(const std::string& name);
    void                     UnregisterAllZones(void);
    void                     Refresh(void);

    void                     InsertPartyInZone (Party& party,           const std::string&);
    void                     InsertObjectInZone(InstanceDynamicObject*, const std::string&);

  private:
    void                     InitializeZoneObservers(Controller&);
    Observer*                FindObserver(Waypoint* waypoint) const;

    Level&                   level;
    std::vector<Controller*> zones;
    std::vector<Observer*>   observers;
  };
}

#endif