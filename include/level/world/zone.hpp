#ifndef  WORLD_ZONE_HPP
# define WORLD_ZONE_HPP

#include "waypoint.hpp"

struct Zone
{
  bool operator==(const std::string& comp) { return (name == comp); }

  bool Contains(unsigned int id) const
  {
    auto      it = waypoints.begin(), end = waypoints.end();

    for (; it != end ; ++it)
    {
      if ((*it)->id == id)
        return (true);
    }
    return (false);
  }

  bool Contains(Waypoint* wp) const { return (Contains(wp->id)); }

  std::string          name;
  std::list<Waypoint*> waypoints;
};

struct ExitZone : public Zone
{
  std::list<std::string> destinations;
};

typedef Zone EntryZone;

#endif
