#include "level/objects/door.hpp"
#include "level/character.hpp"
#include "level/level.hpp"

void ObjectDoor::ProcessCollisions(void)
{
  if (_waypointOccupied->arcs.begin()->observer == 0)
    ObserveWaypoints(true);
}

void ObjectDoor::ObserveWaypoints(bool doObserver)
{
  _waypointDisconnected = _object->lockedArcs;
  std::cout << "Observe Waypoints " << _waypointDisconnected.size() << " (" << doObserver << ')' << std::endl;
  std::for_each(_waypointOccupied->arcs.begin(), _waypointOccupied->arcs.end(), [this, doObserver](Waypoint::Arc& arc)
  {
    arc.observer = (doObserver ? this : 0);
  });
  std::for_each(_waypointDisconnected.begin(), _waypointDisconnected.end(), [this, doObserver](std::pair<int, int> waypoints)
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

  ret.nearest      = _waypointOccupied;
  ret.max_distance = -1;
  ret.min_distance = 0;
  if (waypoint)
  {
    character->UnprocessCollisions();
    std::for_each(_waypointDisconnected.begin(), _waypointDisconnected.end(), [this, waypoint, &ret](std::pair<int, int> waypoints)
    {
      Waypoint* waypoint1 = _level->GetWorld()->GetWaypointFromId(waypoints.first);
      Waypoint* waypoint2 = _level->GetWorld()->GetWaypointFromId(waypoints.second);

      std::list<Waypoint> path;

      if (waypoint1 && (_level->FindPath(path, *waypoint, *waypoint1)))
      {
        if (ret.max_distance > (int)path.size() || ret.max_distance == -1)
        {
          ret.nearest      = waypoint1;
          ret.max_distance = path.size();
        }
      }

      if (waypoint2 && (_level->FindPath(path, *waypoint, *waypoint2)))
      {
        if (ret.max_distance > (int)path.size() || ret.max_distance == -1)
        {
          ret.nearest      = waypoint2;
          ret.max_distance = path.size();
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
    AnimationEnded.DisconnectAll();
    AnimationEnded.Connect([this](InstanceDynamicObject*) { _closed = !_closed; });
    PlayAnimation(_closed ? "open" : "close");
  }
  else
    _level->ConsoleWrite(i18n::T("It's locked"));
}











