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

  waypoints_ids.clear();
  for (; wpIt != wpEnd ; ++wpIt)
  {
    if ((find(waypoints_ids.begin(), waypoints_ids.end(), (*wpIt)->id)) == waypoints_ids.end())
      waypoints_ids.push_back((*wpIt)->id);
  }
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
  
  for (auto it = waypoints_ids.begin() ; it != waypoints_ids.end() ;)
  {
    Waypoint* waypoint = world->GetWaypointFromId(*it);
    
    if (waypoint && ((find(waypoints.begin(), waypoints.end(), waypoint) == waypoints.end())))
    {
      waypoints.push_back(waypoint);
      ++it;
    }
    else
      it = waypoints_ids.erase(it);
  }

}


