#include "level/objects/door.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"
#include <level/pathfinding/path.hpp>

void ObjectDoor::ProcessCollisions(void)
{
  if (GetOccupiedWaypoint()->arcs.begin()->observer == 0)
    ObserveWaypoints(true);
}

void ObjectDoor::ObserveWaypoints(bool doObserver)
{
  Waypoint* waypoint_occupied = GetOccupiedWaypoint();
  
  waypoint_disconnected = _object->lockedArcs;
  std::for_each(waypoint_occupied->arcs.begin(), waypoint_occupied->arcs.end(), [this, doObserver](Waypoint::Arc& arc)
  {
    arc.observer = (doObserver ? this : 0);
  });
  std::for_each(waypoint_disconnected.begin(), waypoint_disconnected.end(), [this, doObserver](std::pair<int, int> waypoints)
  {
    Waypoint*        waypoint = _level->GetWorld()->GetWaypointFromId(waypoints.first);

    if (waypoint)
    {
      Waypoint::Arc* arc1 = waypoint->GetArcTo(waypoints.second);
      Waypoint::Arc* arc2 = 0;

      if (arc1)
      {
        arc1->observer = (doObserver ? this : 0);
        arc2           = arc1->to->GetArcTo(waypoints.first);
      }
      if (arc2)
        arc2->observer = (doObserver ? this : 0);
    }
  });
}

InstanceDynamicObject::GoToData ObjectDoor::GetGoToData(InstanceDynamicObject* character)
{
  Waypoint* waypoint = character->GetOccupiedWaypoint();
  GoToData  ret;

  ret.nearest      = GetOccupiedWaypoint();
  ret.max_distance = -1;
  ret.min_distance = 0;
  if (waypoint)
  {
    character->UnprocessCollisions();
    std::for_each(waypoint_disconnected.begin(), waypoint_disconnected.end(), [this, waypoint, &ret](std::pair<int, int> waypoints)
    {
      Waypoint*         waypoint1 = _level->GetWorld()->GetWaypointFromId(waypoints.first);
      Waypoint*         waypoint2 = _level->GetWorld()->GetWaypointFromId(waypoints.second);
      Pathfinding::Path path;

      if (path.FindPath(waypoint, waypoint1))
      {
        if (ret.max_distance > path.Size() || ret.max_distance == -1)
        {
          ret.nearest      = &path.Last();
          ret.max_distance = path.Size();
        }
      }

      if (path.FindPath(waypoint, waypoint2))
      {
        if (ret.max_distance > path.Size() || ret.max_distance == -1)
        {
          ret.nearest      = &path.Last();
          ret.max_distance = path.Size();
        }
      }
    });
    character->ProcessCollisions();
  }

  ret.objective    = this;
  ret.max_distance = 0;
  return (ret);
}

bool ObjectDoor::CanGoThrough(unsigned char id)
{
  if (_closed)
    return (id != 0);
  return (true);
}

void ObjectDoor::GoingThrough(void*)
{
  if (_closed)
    PlayAnimation("open");
  _closed = false;
}

void ObjectDoor::CallbackActionUse(InstanceDynamicObject* object)
{
  if (!IsLocked())
  {
    AnimationEndForObject.DisconnectAll();
    AnimationEndForObject.Connect([this](AnimatedObject*) { _closed = !_closed; });
    PlayAnimation(_closed ? "open" : "close");
  }
  else
    _level->ConsoleWrite(i18n::T("It's locked"));
}

