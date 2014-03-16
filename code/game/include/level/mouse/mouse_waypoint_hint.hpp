#ifndef  MOUSE_WAYPOINT_HINT_HPP
# define MOUSE_WAYPOINT_HINT_HPP

# include "globals.hpp"
# include <panda3d/windowFramework.h>
# include <panda3d/pointLight.h>
# include "world/waypoint.hpp"
# include "mouse_events.hpp"

class Level;

class MouseWaypointHint : public MouseEvents
{
public:
  MouseWaypointHint(Level& level, WindowFramework* window);
  ~MouseWaypointHint();

  void             Run(float elapsed_time);
  void             SetHoveredWaypoint(Waypoint*);
  
private:
  void             SetLightOnWaypointSurface(Waypoint*);
  void             MoveLightToWaypoint(float elapsed_time);

  WindowFramework* window;
  PT(PointLight)   light;
  NodePath         nodepath;
  NodePath         waypoint_hovered;
  NodePath         waypoint_surface;
};


#endif