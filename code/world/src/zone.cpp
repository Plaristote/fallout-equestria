#include "world/zone.hpp"
#include "world/world.h"

using namespace std;

bool Zone::Contains(unsigned int id) const
{
  auto      it = waypoints_ids.begin(), end = waypoints_ids.end();

  for (; it != end ; ++it)
  {
    if ((*it) == id)
      return (true);
  }
  return (false);
}

void Zone::Serialize(Utils::Packet& packet) const
{
#ifdef GAME_EDITOR
  auto           wpIt  = waypoints.begin();
  auto           wpEnd = waypoints.end();

  for (; wpIt != wpEnd ; ++wpIt)
    waypointsId.push_back((*wpIt)->id);
#endif
  packet << name;
  packet << destinations;
  packet << waypoints_ids;
}

void Zone::Unserialize(Utils::Packet& packet)
{
  World*    world = World::LoadingWorld;
  
  packet >> name;
  packet >> destinations;
  packet >> waypoints_ids;
  
  for (auto it = waypoints_ids.begin() ; it != waypoints_ids.end() ; ++it)
  {
    Waypoint* waypoint = world->GetWaypointFromId(*it);
    
    if (waypoint)
      waypoints.push_back(waypoint);
  }
}


