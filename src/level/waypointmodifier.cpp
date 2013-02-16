#include "level/level.hpp"

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
  //static float average = 0;
  Timer timer;

  std::for_each(_withdrawedArcs.begin(), _withdrawedArcs.end(), [this](WithdrawedArc& arcs)
  {
    Waypoint::Arcs::iterator it;

    arcs.first->UnwithdrawArc(arcs.second, arcs.observer);
    arcs.second->UnwithdrawArc(arcs.first, arcs.observer);
//     it = arcs.first->ConnectUnsafe(arcs.second);
//     it->observer = arcs.observer;
//     it = arcs.second->ConnectUnsafe(arcs.first);
//     it->observer = arcs.observer;
  });
  _withdrawedArcs.clear();

  /*if (average == 0)
    average = timer.GetElapsedTime();
  else
    average = (average + timer.GetElapsedTime()) / 2;*/
}

void        WaypointModifier::WithdrawAllArcs(unsigned int id)
{
  Waypoint* waypoint = _level->GetWorld()->GetWaypointFromId(id);
  
  if (waypoint)
    WithdrawAllArcs(waypoint);
}

void        WaypointModifier::WithdrawAllArcs(Waypoint* waypoint)
{
  for_each(waypoint->arcs_withdrawed.begin(), waypoint->arcs_withdrawed.end(), [this, waypoint](std::pair<Waypoint::Arc, unsigned short>& withdrawable)
  {
    Waypoint::Arc* arc = waypoint->GetArcTo(withdrawable.first.to->id);

    if (arc)
      withdrawable.first.observer = arc->observer;
    _withdrawedArcs.push_back(WaypointModifier::WithdrawedArc(waypoint, withdrawable.first.to, withdrawable.first.observer));
    waypoint->WithdrawArc(withdrawable.first.to);
  });
  //waypoint->DisconnectAll();
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
  Waypoint::Arc*         arc      = first->GetArcTo(second->id);
  Waypoint::ArcObserver* observer = (arc ? arc->observer : 0);

  if (observer == 0)
  {
    std::pair<Waypoint::Arc, unsigned short>* withdrawable = first->GetWithdrawable(second);
    
    if (withdrawable)
      observer = withdrawable->first.observer;
  }
  first->WithdrawArc(second);
  second->WithdrawArc(first);
  _withdrawedArcs.push_back(WithdrawedArc(first, second, observer));
//   if (arc)
//   {
//     Waypoint::ArcObserver* observer = arc->observer;
// 
//     first->Disconnect(second);
//     second->Disconnect(first);
//     _withdrawedArcs.push_back(WithdrawedArc(first, second, observer));
//   }
}

void        WaypointModifier::SetOccupiedWaypoint(Waypoint* wp)
{
  _waypointOccupied = wp;
}
