#ifndef  ZONES_OBSERVER_HPP
# define ZONES_OBSERVER_HPP

# include "world/waypoint.hpp"
# include "level/zones/controller.hpp"

namespace Zones
{
  struct Observer : public Waypoint::ArcObserver
  {
    Observer(Waypoint* to_observe);
    ~Observer();

    bool                     operator==(Waypoint* waypoint) const { return (this->waypoint == waypoint); }
    bool                     operator==(Controller* entry)  const { return (std::find(zones.begin(), zones.end(), entry) != zones.end()); }

    bool                     CanGoThrough(unsigned char);
    void                     GoingThrough(void* object);

    Waypoint*                waypoint;
    std::vector<Controller*> zones;
  };
}

#endif