#ifndef  WORLD_ZONE_HPP
# define WORLD_ZONE_HPP

#include "waypoint.hpp"

struct Zone
{
  bool operator==(const std::string& comp)     const { return (name == comp); }

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

  bool Contains(Waypoint* wp)                  const { return (Contains(wp->id));                                                                }
  bool HasDestination(const std::string& name) const { return (std::find(destinations.begin(), destinations.end(), name) != destinations.end()); }
  void AddDestination(const std::string& name)       { if (!(HasDestination(name))) { destinations.push_back(name); }                            }
  void DelDestination(const std::string& name)       { destinations.erase(std::find(destinations.begin(), destinations.end(), name));            }

  std::string              name;
  std::list<Waypoint*>     waypoints;
  std::vector<std::string> destinations;
};

struct ExitZone : public Zone
{
};

typedef Zone EntryZone;

#endif
