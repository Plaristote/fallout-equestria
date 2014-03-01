#ifndef  ZONES_MANAGER_HPP
# define ZONES_MANAGER_HPP

# include "level/zones/controller.hpp"
# include "level/zones/observer.hpp"
# include "level/zones/passage_way.hpp"
# include "playerparty.hpp"

class Level;

namespace Zones
{
  /*class ManagerBase
  {
  };
  
  class ZoneManager : public virtual ManagerBase
  {
  };
  
  class PassageWayManager : public virtual ManagerBase
  {
  };
  
  class Manager : public ZoneManager, public PassageWayManager
  {
    
  };*/
  
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
    
    PassageWay*              RegisterPassageway(const std::vector<Waypoint*>&);
    void                     UnregisterPassageway(PassageWay*);
    void                     UnregisterAllPassageways(void);

    void                     InsertPartyInZone (Party& party,           const std::string&);
    void                     InsertObjectInZone(InstanceDynamicObject*, const std::string&);

  private:
    void                     InitializeZoneObservers(Controller&);
    void                     InitializePassagewayObservers(PassageWay&);
    Observer*                InitializeObserver(Waypoint*);
    Observer*                FindObserver(Waypoint* waypoint) const;

    Level&                   level;
    std::vector<Controller*> zones;
    std::vector<Observer*>   observers;
    std::vector<PassageWay*> passage_ways;
  };
}

#endif