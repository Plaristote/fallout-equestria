#ifndef  ZONES_PASSAGE_WAY_HPP
# define ZONES_PASSAGE_WAY_HPP

# include "world/waypoint.hpp"
# include "observatory.hpp"
# include <functional>

class InstanceDynamicObject;

namespace Zones
{
  class Manager;
  
  class PassageWay
  {
    friend class Manager;
  public:
    Sync::Signal<void (InstanceDynamicObject*)>  ObjectGoingThrough;
    std::function<bool (InstanceDynamicObject*)> CanGoThrough;
    bool                                         ConnectsWaypoints(Waypoint* first, Waypoint* second);
  private:
    std::vector<Waypoint*>                       waypoints;
  };
}

#endif