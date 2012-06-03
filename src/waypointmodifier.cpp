#include "level.hpp"

using namespace std;

void         WaypointModifier::ProcessCollisions(void)
{
  if (_waypointOccupied != 0)
    WithdrawAllArcs(_waypointOccupied);
  for_each(_waypointDisconnected.begin(), _waypointDisconnected.end(), [this](std::pair<int, int> waypoints)
  {
    WithdrawArc(waypoints.first, waypoints.second);
  });
}

void         WaypointModifier::UnprocessCollisions(void)
{
  std::for_each(_withdrawedArcs.begin(), _withdrawedArcs.end(), [this](WithdrawedArc& arcs)
  {
    arcs.first->Connect(arcs.second);
    arcs.second->Connect(arcs.first);
  });
  _withdrawedArcs.clear();
}

void        WaypointModifier::WithdrawAllArcs(unsigned int id)
{
  Waypoint* waypoint = _level->GetWorld()->GetWaypointFromId(id);
  
  if (waypoint)
    WithdrawAllArcs(waypoint);
}

void        WaypointModifier::WithdrawAllArcs(Waypoint* waypoint)
{
  for_each(waypoint->arcs.begin(), waypoint->arcs.end(), [this, waypoint](Waypoint::Arc& arc)
  {
    _withdrawedArcs.push_back(WithdrawedArc(waypoint, arc.to));
  });
  waypoint->DisconnectAll();
}

void        WaypointModifier::WithdrawArc(unsigned int id1, unsigned int id2)
{
  Waypoint* first  = _level->GetWorld()->GetWaypointFromId(id1);
  Waypoint* second = _level->GetWorld()->GetWaypointFromId(id2);
  
  if (first && second)
    WithdrawArc(first, second);
}

void        WaypointModifier::WithdrawArc(Waypoint* first, Waypoint* second)
{
  first->Disconnect(second);
  second->Disconnect(first);
  _withdrawedArcs.push_back(WithdrawedArc(first, second));
}