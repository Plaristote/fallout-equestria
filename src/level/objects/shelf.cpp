#include "level/objects/shelf.hpp"
#include "level/level.hpp"
#include <algorithm>

using namespace std;
using namespace Observatory;

void ObjectShelf::LockWaypoints(bool locking)
{
  if (_object)
  {
    std::for_each(_object->lockedArcs.begin(), _object->lockedArcs.end(), [this, locking](std::pair<int, int> waypoints)
    {
      Waypoint*        waypoint1 = _level->GetWorld()->GetWaypointFromId(waypoints.first);
      Waypoint*        waypoint2 = _level->GetWorld()->GetWaypointFromId(waypoints.first);

      if (waypoint1 && waypoint2)
      {
	if (locking)
	  waypoint1->Disconnect(waypoint2);
	else
	  waypoint1->Connect(waypoint2);
      }
    });
  }
}

ObjectShelf::ObjectShelf(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  _type   = ObjectTypes::Shelf;
  LockWaypoints(true);
  _inventory.LoadInventory(object);
  _inventory.SetCapacity(450);
}

InstanceDynamicObject::GoToData ObjectShelf::GetGoToData(InstanceDynamicObject* character)
{
  Waypoint* waypoint = character->GetOccupiedWaypoint();
  GoToData  ret;

  ret.nearest      = 0;
  ret.max_distance = -1;
  ret.min_distance = 0;
  if (waypoint)
  {
    character->UnprocessCollisions();
    std::for_each(_object->lockedArcs.begin(), _object->lockedArcs.end(), [this, waypoint, &ret](std::pair<int, int> waypoints)
    {
      Waypoint* waypoint1 = _level->GetWorld()->GetWaypointFromId(waypoints.first);
      Waypoint* waypoint2 = _level->GetWorld()->GetWaypointFromId(waypoints.second);

      list<Waypoint> path;

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

void ObjectShelf::CallbackActionUse(InstanceDynamicObject* object)
{
  if (object == _level->GetPlayer())
    _level->PlayerLoot(&_inventory);
}
