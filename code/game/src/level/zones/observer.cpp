#include "level/zones/observer.hpp"

using namespace std;

Zones::Observer::Observer(Waypoint* to_observe) : waypoint(to_observe)
{
  for_each(waypoint->arcs.begin(), waypoint->arcs.end(), [this](Waypoint::Arc& arc)
  {
    arc.observer = this;
  });
  for_each(waypoint->arcs_withdrawed.begin(), waypoint->arcs_withdrawed.end(), [this](std::pair<Waypoint::Arc, unsigned short>& arc)
  {
    arc.first.observer = this;
  });
}

Zones::Observer::~Observer()
{
  for_each(waypoint->arcs.begin(), waypoint->arcs.end(), [this](Waypoint::Arc& arc)
  {
    if (arc.observer == this)
      arc.observer = 0;
  });
  for_each(waypoint->arcs_withdrawed.begin(), waypoint->arcs_withdrawed.end(), [this](std::pair<Waypoint::Arc, unsigned short>& arc)
  {
    if (arc.first.observer == this)
      arc.first.observer = 0;
  });
}

bool Zones::Observer::CanGoThrough(Waypoint* from, Waypoint* to, void* _object)
{
  InstanceDynamicObject* object         = reinterpret_cast<InstanceDynamicObject*>(_object);
  bool                   can_go_through = true;
  
  for_each(passage_ways.begin(), passage_ways.end(), [from, to, object, &can_go_through](PassageWay* entry)
  {
    if (entry->ConnectsWaypoints(from, to))
      can_go_through = can_go_through && entry->CanGoThrough(object);
  });
  return (can_go_through);
}

void Zones::Observer::GoingThrough(Waypoint* from, Waypoint* to, void* _object)
{
  InstanceDynamicObject* object = reinterpret_cast<InstanceDynamicObject*>(_object);

  if (object)
  {
    GoingThroughZones(object);
    GoingThroughPassageways(from, to, object);
  }
}

void Zones::Observer::GoingThroughPassageways(Waypoint* from, Waypoint* to, InstanceDynamicObject* object)
{
  for_each(passage_ways.begin(), passage_ways.end(), [object, from, to](PassageWay* entry)
  {
    if (entry->ConnectsWaypoints(from, to))
      entry->ObjectGoingThrough.Emit(object);
  });
}

void Zones::Observer::GoingThroughZones(InstanceDynamicObject* object)
{
  for_each(zones.begin(), zones.end(), [object](Controller* entry)
  {
    entry->ObjectMovesWithinZone(object);
  });
}
