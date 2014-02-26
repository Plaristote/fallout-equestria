#ifndef  LEVEL_PATHFINDING_HPP
# define LEVEL_PATHFINDING_HPP

# include "globals.hpp"
# include "world/waypoint.hpp"
# include <functional>
# include <list>

class World;

namespace Pathfinding
{
  class Path
  {
    friend class Pathfinding;
  public:
    Path(Waypoint* from, Waypoint* to) : from(from), to(to)
    {
    }
    
    Path(void) : from(0), to(0)
    {
    }

    bool                 FindPath(Waypoint* from, Waypoint* to);
    void                 StripFirstWaypointFromList(void);
    void                 StripLastWaypointFromList(void);
    unsigned int         Size(void) const { return (waypoints.size());    }
    Waypoint&            Front(void)      { return (*waypoints.begin());  }
    Waypoint&            Last(void)       { return (*waypoints.rbegin()); }
    void                 Clear(void);
    void                 Truncate(unsigned int max_size);
    
    void Save();
    void Load(World*);
    
  protected:
    void                 ForeachWaypoint(std::function<void (Waypoint&)>);

    Waypoint*            from;
    Waypoint*            to;
    std::list<Waypoint>  waypoints;
  };
}

#endif
