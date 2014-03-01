#include "level/objects/instance_dynamic_object.hpp"
#include "level/level.hpp"

using namespace std;
using namespace Sync;
using namespace ObjectTypes;

InstanceDynamicObject::InstanceDynamicObject(Level* level, DynamicObject* object) :
  AnimatedObject(level->GetWorld()->window),
  Interactions::Target(level, object),
  _object(object),
  tasks(this)
{
  _type                 = Other;
  _level                = level;
  idle_size             = NodePathSize(object->nodePath);
  waypoint_disconnected = object->lockedArcs;
  SetOccupiedWaypoint(object->waypoint);
  SetModelNameFromPath(object->strModel);
}

InstanceDynamicObject::GoToData   InstanceDynamicObject::GetGoToData(InstanceDynamicObject* character)
{
  GoToData         ret;

  ret.nearest      = GetOccupiedWaypoint();
  ret.objective    = this;
  ret.max_distance = 0;
  ret.min_distance = 0;
  return (ret);
}

void InstanceDynamicObject::AddTextBox(const std::string& text, unsigned short r, unsigned short g, unsigned short b, float timeout)
{
  _level->GetChatterManager().PushTextBox(this, text, r, g, b, timeout);
}

void InstanceDynamicObject::Unserialize(Utils::Packet& packet)
{
  char hasWaypoint;
  
  packet >> hasWaypoint;
  if (hasWaypoint == '1')
  {
    unsigned int waypointId;

    packet.operator>> <unsigned int>(waypointId);
    SetOccupiedWaypoint(_level->GetWorld()->GetWaypointFromId(waypointId));
  }
  tasks.Unserialize(packet);
}

void InstanceDynamicObject::Serialize(Utils::Packet& packet)
{
  if (HasOccupiedWaypoint())
  {
    packet << '1'; // has a waypointOccupied
    packet << GetOccupiedWaypoint()->id;
  }
  else
    packet << '0';
  tasks.Serialize(packet);
}
