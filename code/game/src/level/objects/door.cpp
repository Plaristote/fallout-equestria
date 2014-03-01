#include "level/objects/door.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"
#include <level/pathfinding/path.hpp>

ObjectDoor::ObjectDoor(Level* level, DynamicObject* object): InstanceDynamicObject(level, object), Lockable(object)
{
  _type             = ObjectTypes::Door;
  _closed           = true;
  collision_enabled = true;
  InitializePassageWay();
}

ObjectDoor::~ObjectDoor()
{
  if (passage_way)
  {
    Zones::Manager& zone_manager = _level->GetZoneManager();

    zone_manager.UnregisterPassageway(passage_way);
  }
}

void ObjectDoor::ProcessCollisions(void)
{
  collision_enabled = true;
}

void ObjectDoor::UnprocessCollision(void)
{
  collision_enabled = false;
}

void ObjectDoor::InitializePassageWay(void)
{
  Zones::Manager&   zone_manager      = _level->GetZoneManager();
  Waypoint*         waypoint_occupied = GetOccupiedWaypoint();
  vector<Waypoint*> to_observe;

  // From occupied waypoint, if one was specified  
  if (waypoint_occupied)
  {
    for_each(waypoint_occupied->arcs.begin(), waypoint_occupied->arcs.end(), [this, &to_observe](const Waypoint::Arc& arc)
    {
      to_observe.push_back(arc.to);
    });
  }

  // From disconnected list
  for_each(_object->lockedArcs.begin(), _object->lockedArcs.end(), [this, &to_observe](const std::pair<int, int>& waypoint_ids)
  {
    World*    world         = _level->GetWorld();
    Waypoint* waypoint_from = world->GetWaypointFromId(waypoint_ids.first);
    Waypoint* waypoint_to   = world->GetWaypointFromId(waypoint_ids.second);

    if (find(to_observe.begin(), to_observe.end(), waypoint_from) == to_observe.end())
      to_observe.push_back(waypoint_from);
    if (find(to_observe.begin(), to_observe.end(), waypoint_to)   == to_observe.end())
      to_observe.push_back(waypoint_to);
  });
  passage_way = zone_manager.RegisterPassageway(to_observe);
  passage_way->CanGoThrough = [this](InstanceDynamicObject* object) -> bool { return (CanGoThrough(object)); };
  passage_way->ObjectGoingThrough.Connect(*this, &ObjectDoor::GoingThrough);
}

bool ObjectDoor::CanGoThrough(InstanceDynamicObject* object)
{
  if (collision_enabled)
  {
    if (_closed)
      return (object != _level->GetPlayer());
  }
  return (true);
}

void ObjectDoor::GoingThrough(InstanceDynamicObject* object)
{
  if (collision_enabled)
  {
    if (_closed)
      PlayAnimation("open");
    _closed = false;
  }
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
