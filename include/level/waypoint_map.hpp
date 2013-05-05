#ifndef  WAYPOINTMAP_HPP
# define WAYPOINTMAP_HPP

# include "level/world.h"
# include <panda3d/geomVertexData.h>
# include <panda3d/geomPoints.h>
# include <panda3d/geom.h>
# include <panda3d/geomNode.h>

class WaypointMap
{
public:
  WaypointMap(WindowFramework* window) : window(window) {}

  bool             Initialize(const std::vector<Waypoint>& waypoints, unsigned char floor);
  NodePath         GetNodePath(void) const { return (waypoint_map); }
  
private:
  WindowFramework* window;
  PT(GeomNode)     geom_node;
  NodePath         waypoint_map;
};

#endif