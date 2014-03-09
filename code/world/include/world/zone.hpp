#ifndef  WORLD_ZONE_HPP
# define WORLD_ZONE_HPP

#include "waypoint.hpp"

struct Zone : public Utils::Serializable
{
  bool operator==(const std::string& comp)     const { return (name == comp); }

  bool Contains(unsigned int id) const;

  bool Contains(Waypoint* wp)                  const { return (Contains(wp->id));                                                                }
  bool HasDestination(const std::string& name) const { return (std::find(destinations.begin(), destinations.end(), name) != destinations.end()); }
  void AddDestination(const std::string& name)       { if (!(HasDestination(name))) { destinations.push_back(name); }                            }
  void DelDestination(const std::string& name)       { destinations.erase(std::find(destinations.begin(), destinations.end(), name));            }
  
  void Serialize(Utils::Packet& packet) const;
  void Unserialize(Utils::Packet& packet);

  std::string              name;
  std::list<Waypoint*>     waypoints;
  std::vector<std::string> destinations;
};

#endif
