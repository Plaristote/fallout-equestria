#ifndef  MOUSE_ZONE_HINT_HPP
# define MOUSE_ZONE_HINT_HPP

# include "mouse_waypoint_hint.hpp"
# include "level/zones/manager.hpp"

class Level;

class MouseZoneHint : public MouseWaypointHint
{
public:
  MouseZoneHint(Level& level);

  void            SetHoveredWaypoint(Waypoint*);
private:
  void            ExitZoneHint(Waypoint* waypoint);

  Zones::Manager& zone_manager;
};

#endif