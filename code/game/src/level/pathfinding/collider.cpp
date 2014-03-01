#include "level/level.hpp"

using namespace std;
using namespace Pathfinding;

void         Collider::ProcessCollisions(void)
{
  if (waypoint_occupied != 0)
    WithdrawAllArcs(waypoint_occupied);
  /*for_each(waypoint_disconnected.begin(), waypoint_disconnected.end(), [this](std::pair<int, int> waypoints)
  {
    WithdrawArc(waypoints.first, waypoints.second);
  });*/
}

void         Collider::UnprocessCollisions(void)
{
  std::for_each(withdrawed_arcs.begin(), withdrawed_arcs.end(), [this](WithdrawedArc& arcs)
  {
    Waypoint::Arcs::iterator it;

    arcs.first->UnwithdrawArc(arcs.second, arcs.observer);
    //arcs.second->UnwithdrawArc(arcs.first, arcs.observer);
  });
  withdrawed_arcs.clear();
}

Path Collider::GetPathTowardsObject(Collider* character)
{
  Pathfinding::Path path;

  if (HasOccupiedWaypoint())
  {
    character->UnprocessCollisions();
    UnprocessCollisions();
    path.FindPath(character->GetOccupiedWaypoint(), GetOccupiedWaypoint());
    path.StripFirstWaypointFromList();
    path.StripLastWaypointFromList();
    ProcessCollisions();
    character->ProcessCollisions();
  }
  return (path);
}

void        Collider::WithdrawAllArcs(Waypoint* waypoint)
{
  for_each(waypoint->arcs_withdrawed.begin(), waypoint->arcs_withdrawed.end(), [this, waypoint](std::pair<Waypoint::Arc, unsigned short>& withdrawable)
  {
    Waypoint::Arc* arc = waypoint->GetArcTo(withdrawable.first.to->id);

    if (arc)
      withdrawable.first.observer = arc->observer;
    withdrawed_arcs.push_back(Collider::WithdrawedArc(waypoint, withdrawable.first.to, withdrawable.first.observer));
    waypoint->WithdrawArc(withdrawable.first.to);
  });
}

void        Collider::WithdrawArc(Waypoint* first, Waypoint* second)
{
  Waypoint::Arc*         arc      = first->GetArcTo(second->id);
  Waypoint::ArcObserver* observer = (arc ? arc->observer : 0);

  if (observer == 0)
  {
    std::pair<Waypoint::Arc, unsigned short>* withdrawable = first->GetWithdrawable(second);
    
    if (withdrawable)
      observer = withdrawable->first.observer;
  }
  first->WithdrawArc(second);
  //second->WithdrawArc(first);
  withdrawed_arcs.push_back(WithdrawedArc(first, second, observer));
}

void        Collider::SetOccupiedWaypoint(Waypoint* wp)
{
#ifndef GAME_EDITOR
  if (waypoint_occupied)
  {
    auto     it  = waypoint_occupied->lights.begin();
    auto     end = waypoint_occupied->lights.end();
    NodePath np  = GetNodePath();

    for (; it != end ; ++it)
      np.set_light_off((*it)->light->as_node());
  }
  waypoint_occupied = wp;
  if (wp)
  {
    auto     it  = waypoint_occupied->lights.begin();
    auto     end = waypoint_occupied->lights.end();
    NodePath np  = GetNodePath();

    for (; it != end ; ++it)
      np.set_light((*it)->light->as_node());
  }
#else
  waypoint_occupied = wp;
#endif
}
