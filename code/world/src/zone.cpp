#include "world/zone.hpp"
#include "world/world.h"

using namespace std;

bool Zone::Contains(unsigned int id) const
{
  auto      it = waypoints.begin(), end = waypoints.end();

  for (; it != end ; ++it)
  {
    if ((*it)->id == id)
      return (true);
  }
  return (false);
}

void Zone::Serialize(Utils::Packet& packet) const
{
  std::list<int> waypointsId;
  auto           wpIt  = waypoints.begin();
  auto           wpEnd = waypoints.end();

  for (; wpIt != wpEnd ; ++wpIt)
    waypointsId.push_back((*wpIt)->id);
  packet << name;
  packet << destinations;
  packet << waypointsId;
}

void Zone::Unserialize(Utils::Packet& packet)
{
  World*    world = World::LoadingWorld;
  list<int> waypointsId;
  
  packet >> name;
  packet >> destinations;
  packet >> waypointsId;
  
  for (auto it = waypointsId.begin() ; it != waypointsId.end() ; ++it)
  {
    Waypoint* waypoint = world->GetWaypointFromId(*it);
    
    if (waypoint)
      waypoints.push_back(waypoint);
  }
}


