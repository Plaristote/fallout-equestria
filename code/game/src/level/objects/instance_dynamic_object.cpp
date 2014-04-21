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
  data_store            = new DataTree;
  idle_size             = NodePathSize(object->nodePath);
  waypoint_disconnected = object->lockedArcs;
  SetModelNameFromPath(object->strModel);
  SetOccupiedWaypoint(object->waypoint);
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
