#include "level/objects/instance_dynamic_object.hpp"
#include "level/level.hpp"

using namespace std;
using namespace Sync;
using namespace ObjectTypes;

InstanceDynamicObject::InstanceDynamicObject(Level* level, DynamicObject* object) :
  ObjectVisibility(level->GetWorld()->window),
  Interactions::Target(level, object),
  _object(object),
  tasks(this)
{
  _type                 = Other;
  _level                = level;
  _flags                = 0;
  data_store            = new DataTree;
  idle_size             = NodePathSize(object->nodePath);
  waypoint_disconnected = object->lockedArcs;
  SetModelNameFromPath(object->strModel);
  SetOccupiedWaypoint(object->waypoint);
  ObjectVisibility::Initialize();
}

InstanceDynamicObject::~InstanceDynamicObject()
{
  if (data_store)
    delete data_store;
}

void InstanceDynamicObject::AddTextBox(const std::string& text, unsigned short r, unsigned short g, unsigned short b, float timeout)
{
  _level->GetChatterManager().PushTextBox(this, text, r, g, b, timeout);
}

ISampleInstance* InstanceDynamicObject::PlaySound(const string& name)
{
  return (_level->PlaySound(_object->sound_pack + '/' + name));
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
  UnserializeDataStore(packet);
  ObjectVisibility::Unserialize(packet);
}

void InstanceDynamicObject::Serialize(Utils::Packet& packet)
{
  Waypoint* waypoint = GetOccupiedWaypoint();
  
  if (waypoint != 0)
  {
    packet << '1'; // has a waypointOccupied
    packet << waypoint->id;
  }
  else
    packet << '0';
  tasks.Serialize(packet);
  SerializeDataStore(packet);
  ObjectVisibility::Serialize(packet);
}

void InstanceDynamicObject::SerializeDataStore(Utils::Packet& packet)
{
  string data_store_json;
  
  DataTree::Writers::StringJSON(data_store, data_store_json);
  packet << data_store_json;
}

void InstanceDynamicObject::UnserializeDataStore(Utils::Packet& packet)
{
  string data_store_json;

  packet >> data_store_json;
  if (data_store)
    delete data_store;
  data_store = DataTree::Factory::StringJSON(data_store_json);
}

float GetDistance(LPoint3f pos_1, LPoint3f pos_2)
{
  float   dist_x = pos_1.get_x() - pos_2.get_x();
  float   dist_y = pos_1.get_y() - pos_2.get_y();

  return (SQRT(dist_x * dist_x + dist_y * dist_y));
}

float               InstanceDynamicObject::GetDistance(const InstanceDynamicObject* object) const
{
  if (object != this)
  {
    LPoint3 pos_1 = _object->nodePath.get_pos(_level->GetWindow()->get_render());
    LPoint3 pos_2 = object->GetNodePath().get_pos(_level->GetWindow()->get_render());

    return (::GetDistance(pos_1, pos_2));
  }
  return (0);
}

list<InstanceDynamicObject*> InstanceDynamicObject::GetObjectsInRadius(float radius) const
{
  LPoint3f position = GetDynamicObject()->nodePath.get_pos(_level->GetWindow()->get_render());

  return (_level->GetObjectsInRadius(position, radius));
}
