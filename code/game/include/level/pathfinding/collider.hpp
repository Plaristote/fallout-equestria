#ifndef  WAYPOINT_MODIFIER_HPP
# define WAYPOINT_MODIFIER_HPP

# include "world/waypoint.hpp"

class Level;

namespace Pathfinding
{
  class Collider
  {
    struct WithdrawedArc
    {
      WithdrawedArc(Waypoint* first, Waypoint* second, Waypoint::ArcObserver* observer) : first(first), second(second), observer(observer) {}
      Waypoint              *first, *second;
      Waypoint::ArcObserver *observer;
    };
    
    typedef std::list<WithdrawedArc>        WithdrawedArcs;

  public:
    Collider() : waypoint_occupied(0)
    {}

    virtual NodePath GetNodePath()                  const = 0;

    virtual void     ProcessCollisions(void);
    void             UnprocessCollisions(void);
    virtual bool     HasOccupiedWaypoint(void)      const { return (waypoint_occupied != 0); }
    int              GetOccupiedWaypointAsInt(void) const { return (waypoint_occupied ? waypoint_occupied->id : 0);  }
    Waypoint*        GetOccupiedWaypoint(void)      const { return (waypoint_occupied);      }
    void             SetOccupiedWaypoint(Waypoint* wp);

  protected:
    void             WithdrawAllArcs(Waypoint* waypoint);
    void             WithdrawArc(Waypoint* first, Waypoint* second);

    std::list<std::pair<int, int> >         waypoint_disconnected;
  private:
    Waypoint*                               waypoint_occupied;
    WithdrawedArcs                          withdrawed_arcs;
  };
}

#endif