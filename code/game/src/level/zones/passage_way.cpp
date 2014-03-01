#include "level/zones/passage_way.hpp"

using namespace std;

bool Zones::PassageWay::ConnectsWaypoints(Waypoint* first, Waypoint* second)
{
  auto first_it  = find(waypoints.begin(), waypoints.end(), first);
  auto second_it = find(waypoints.begin(), waypoints.end(), second);

  return (first_it != waypoints.end() && second_it != waypoints.end());
}
