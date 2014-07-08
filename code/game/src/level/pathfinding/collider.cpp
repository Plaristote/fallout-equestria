#include "level/level.hpp"

using namespace std;
using namespace Pathfinding;

void      Pathfinding::Collider::ProcessCollisions(void)
{
  if (collision_processed == false && waypoint_occupied != 0)
  {
    collision_processed = true;
    WithdrawAllArcs(waypoint_occupied);
  }
}

void      Pathfinding::Collider::UnprocessCollisions(void)
{
  collision_processed = false;
  std::for_each(withdrawed_arcs.begin(), withdrawed_arcs.end(), [this](WithdrawedArc& arcs)
  {
    Waypoint::Arcs::iterator it;

    arcs.first->UnwithdrawArc(arcs.second);
    arcs.second->UnwithdrawArc(arcs.first);
  });
  withdrawed_arcs.clear();
}

Path      Pathfinding::Collider::GetPathTowardsObject(Collider* character)
{
  Pathfinding::Path path;

  if (HasOccupiedWaypoint())
  {
    character->UnprocessCollisions();
    UnprocessCollisions();
    path.FindPath(character->GetOccupiedWaypoint(), GetOccupiedWaypoint());
    path.StripFirstWaypointFromList();
    if (!can_be_walked_on)
      path.StripLastWaypointFromList();
    ProcessCollisions();
    character->ProcessCollisions();
  }
  else
    cout << "Target has no waypoint" << endl;
  return (path);
}

void      Pathfinding::Collider::WithdrawAllArcs(Waypoint* waypoint)
{
  for_each(waypoint->arcs_withdrawed.begin(), waypoint->arcs_withdrawed.end(), [this, waypoint](std::pair<Waypoint::Arc, unsigned short>& withdrawable)
  {
    WithdrawArc(withdrawable.first.from, withdrawable.first.to);
  });
}

void      Pathfinding::Collider::WithdrawArc(Waypoint* first, Waypoint* second)
{
  first->WithdrawArc(second);
  second->WithdrawArc(first);
  withdrawed_arcs.push_back(WithdrawedArc(first, second));
}

void      Pathfinding::Collider::SetOccupiedWaypoint(Waypoint* wp)
{
#ifndef GAME_EDITOR
  if (wp != waypoint_occupied)
  {
    if ((!waypoint_occupied && wp) || (wp && waypoint_occupied && waypoint_occupied->floor != wp->floor))
      ChangedFloor.Emit(wp->floor);
    collision_processed = false;
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
      {
        cout << "Setting light " << (*it)->nodePath.get_name() << endl;
        np.set_light((*it)->light->as_node());
      }
//      waypoint_occupied->nodePath.reparent_to(Level::CurrentLevel->GetWorld()->window->get_render());
//      waypoint_occupied->nodePath.show();
    }
  }
#else
  waypoint_occupied = wp;
#endif
}

Waypoint* Pathfinding::Collider::GetClosestWaypointFrom(Collider* object, bool closest)
{
  Waypoint* self  = GetOccupiedWaypoint();
  Waypoint* other = object->GetOccupiedWaypoint();
  Waypoint* wp    = self;
  float     currentDistance;
  
  if (other)
  {
    currentDistance = wp->GetDistanceEstimate(*other);
    UnprocessCollisions();
    {
      list<Waypoint*> list = self->GetSuccessors(other);
      
      cout << "BestWaypoint choices: " << list.size() << endl;
      for_each(list.begin(), list.end(), [&wp, &currentDistance, other, closest](Waypoint* waypoint)
      {
        float compDistance = waypoint->GetDistanceEstimate(*other);
        bool  comp         = (closest == false ? currentDistance < compDistance : currentDistance > compDistance);

        if (comp)
          wp = waypoint;
      });
    }
    ProcessCollisions();
  }
  cout << self->id << " versus " << wp->id << endl;
  return (wp);
}
