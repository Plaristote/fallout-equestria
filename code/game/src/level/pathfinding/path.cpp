#include "level/pathfinding/path.hpp"
#include "world/world.h"
#include "astar.hpp"
#include <panda3d/collisionNode.h>

using namespace std;

bool Pathfinding::Path::FindPath(Waypoint* from, Waypoint* to)
{
  AstarPathfinding<Waypoint>        astar;
  int                               max_iterations = 0;
  AstarPathfinding<Waypoint>::State state;

  Clear();
  this->from = from;
  this->to   = to;
  astar.SetStartAndGoalStates(*from, *to);
  while ((state = astar.SearchStep()) == AstarPathfinding<Waypoint>::Searching && max_iterations++ < 250);

  if (state == AstarPathfinding<Waypoint>::Succeeded)
  {
    waypoints = astar.GetSolution();
    contains_valid_path = true;
  }
  else
    contains_valid_path = false;
  return (contains_valid_path);
}

void Pathfinding::Path::Clear(void)
{
  waypoints.clear();
  from = to = 0;
}

void Pathfinding::Path::ForeachWaypoint(function<void (Waypoint&)> callback)
{
  for_each(waypoints.begin(), waypoints.end(), callback);
}

void Pathfinding::Path::StripFirstWaypointFromList(void)
{
  if (waypoints.size() > 0)
  {
    Waypoint& current_waypoint = *waypoints.begin();

    if (!(current_waypoint.nodePath.is_empty()))
      current_waypoint.nodePath.detach_node();
    waypoints.erase(waypoints.begin());
    from = waypoints.size() == 0 ? 0 : &(*waypoints.begin());
  }
}

void Pathfinding::Path::StripLastWaypointFromList(void)
{
  if (waypoints.size() > 0)
  {
    Waypoint& current_waypoint = *waypoints.rbegin();
    
    if (!(current_waypoint.nodePath.is_empty()))
      current_waypoint.nodePath.detach_node();
    waypoints.erase(--(waypoints.end()));
  }
}

void Pathfinding::Path::Truncate(unsigned int max_size)
{
  if (waypoints.size() > max_size)
    waypoints.resize(max_size);
}

void Pathfinding::Path::Serialize(Utils::Packet& packet)
{
  std::vector<int> waypoint_ids;
  
  for_each(waypoints.begin(), waypoints.end(), [&waypoint_ids](Waypoint& waypoint) { waypoint_ids.push_back(waypoint.id); });
  packet << waypoint_ids;
}

void Pathfinding::Path::Unserialize(World* world, Utils::Packet& packet)
{
  std::vector<int> waypoint_ids;
  
  from = to = 0;
  waypoints.clear();
  packet >> waypoint_ids;
  for_each(waypoint_ids.begin(), waypoint_ids.end(), [this, world](int waypoint_id)
  {
    Waypoint* waypoint = world->GetWaypointFromId(waypoint_id);
    
    if (waypoint)
    {
      if (from == 0)
        from = waypoint;
      to     = waypoint;
      waypoints.push_back(*waypoint);
    }
  });
}
