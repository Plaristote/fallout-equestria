#ifndef  ZONES_OBSERVER_HPP
# define ZONES_OBSERVER_HPP

# include "world/waypoint.hpp"
# include "level/zones/controller.hpp"
# include "level/zones/passage_way.hpp"

class InstanceDynamicObject;

namespace Zones
{
  struct Observer : public Waypoint::ArcObserver
  {
    Observer(Waypoint* to_observe);
    ~Observer();

    bool                     operator==(Waypoint* waypoint) const { return (this->waypoint == waypoint); }
    bool                     operator==(Controller* entry)  const { return (std::find(zones.begin(), zones.end(), entry) != zones.end()); }

    bool                     CanGoThrough(Waypoint* from, Waypoint* to, void* object);
    void                     GoingThrough(Waypoint* from, Waypoint* to, void* object);
    
    bool                     HasObservers(void)             const { return (zones.size() + passage_ways.size() > 0); }
    void                     AddObserver(Controller* controller)  { zones.push_back(controller);         }
    void                     AddObserver(PassageWay* passage_way) { passage_ways.push_back(passage_way); }

    Waypoint*                waypoint;
    std::vector<Controller*> zones;
    std::vector<PassageWay*> passage_ways;
    
  private:
    void                    GoingThroughZones(InstanceDynamicObject* object);
    void                    GoingThroughPassageways(Waypoint* from, Waypoint* to, InstanceDynamicObject* object);
  };
}

#endif