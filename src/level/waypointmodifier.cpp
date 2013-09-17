#include "level/level.hpp"

using namespace std;

void         WaypointModifier::ProcessCollisions(void)
{
  PStatCollector collector("Level:Waypoints:ProcessCollisions");

  /*if (_collision_processed == 0)
  {*/
    collector.start();
    if (_waypointOccupied != 0)
      WithdrawAllArcs(_waypointOccupied);
    for_each(_waypointDisconnected.begin(), _waypointDisconnected.end(), [this](std::pair<int, int> waypoints)
    {
      WithdrawArc(waypoints.first, waypoints.second);
    });
    collector.stop();
  /*}
  _collision_processed++;
  cout << this << " col proc count '" << _collision_processed << "' (" << (this == _level->GetPlayer()) << endl;*/
}

void         WaypointModifier::UnprocessCollisions(void)
{
  PStatCollector collector("Level:Waypoints:UnprocessCollisions");
  Timer timer;

  /*if (_collision_processed)
  {*/
    collector.start();
    std::for_each(_withdrawedArcs.begin(), _withdrawedArcs.end(), [this](WithdrawedArc& arcs)
    {
      Waypoint::Arcs::iterator it;

      arcs.first->UnwithdrawArc(arcs.second, arcs.observer);
      //arcs.second->UnwithdrawArc(arcs.first, arcs.observer);
    });
    _withdrawedArcs.clear();
    collector.stop();
  /*}
  _collision_processed--;*/
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
  //second->WithdrawArc(first);
  _withdrawedArcs.push_back(WithdrawedArc(first, second, observer));
}

void        WaypointModifier::SetOccupiedWaypoint(Waypoint* wp)
{
#ifndef GAME_EDITOR
  if (_waypointOccupied)
  {
    auto     it  = _waypointOccupied->lights.begin();
    auto     end = _waypointOccupied->lights.end();
    NodePath np  = GetNodePath();

    for (; it != end ; ++it)
      np.set_light_off((*it)->light->as_node());
  }
  cout << "SET OCCUPIED WAYPOINT FOR " << this << ": " << wp << endl;
  _waypointOccupied = wp;
  if (wp)
  {
    auto     it  = _waypointOccupied->lights.begin();
    auto     end = _waypointOccupied->lights.end();
    NodePath np  = GetNodePath();

    for (; it != end ; ++it)
      np.set_light((*it)->light->as_node());
  }
#else
  _waypointOccupied = wp;
#endif
}
