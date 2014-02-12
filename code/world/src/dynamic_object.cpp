#include "world/world.h"
#include "world/dynamic_object.hpp"

using namespace std;

extern unsigned int blob_revision;

/*
 * Serialization
 */
void DynamicObject::UnSerialize(World* world, Utils::Packet& packet)
{
  int  iType;
  char iLocked;
  int  iWaypoint;

  MapObject::UnSerialize(world, packet);
  packet >> iType >> interactions;
  type = (Type)iType;

  if      (type == Character)
    packet >> script >> charsheet >> dialog;
  else if (type == Door || type == Locker)
  {
    packet >> iLocked >> key;
    locked = iLocked != 0;
  }
  else if (type == Item)
    packet >> key;

  packet >> iWaypoint;
  waypoint = world->GetWaypointFromId(iWaypoint);

  // Blocked Arcs
  {
      int size;

      packet >> size;
      for (int i = 0 ; i < size ; ++i)
      {
        int id1, id2;

        packet >> id1 >> id2;
        auto it1 = find(lockedArcs.begin(), lockedArcs.end(), std::pair<int, int>(id1, id2));
        auto it2 = find(lockedArcs.begin(), lockedArcs.end(), std::pair<int, int>(id2, id1));
        if (it1 == lockedArcs.end() && it2 == lockedArcs.end())
        {  lockedArcs.push_back(std::pair<int, int>(id1, id2)); }
      }
  }

  // Inventory serialization
  {
      int size;

      packet >> size;
      for (int i = 0 ; i < size ; ++i)
      {
          string jsonSrc;
          int    quantity;

          packet >> jsonSrc >> quantity;
          inventory.push_back(std::pair<std::string, int>(jsonSrc, quantity));
      }
  }
}

void DynamicObject::Serialize(Utils::Packet& packet)
{
    MapObject::Serialize(packet);

    int  iType     = (int)type;
    char iLocked   = locked;
    int  iWaypoint = 0;

    packet << iType << interactions;

    if      (type == Character)
      packet << script << charsheet << dialog;
    else if (type == Door || type == Locker)
      packet << iLocked << key;
    else if (type == Item)
      packet << key;

    if (waypoint)
      iWaypoint = waypoint->id;
    packet << iWaypoint;

    // Blocked Arcs
    {
        list<pair<int, int> >::iterator it  = lockedArcs.begin();
        list<pair<int, int> >::iterator end = lockedArcs.end();
        int size = lockedArcs.size();

        packet << size;
        for (; it != end ; ++it)
          packet << (*it).first << (*it).second;
    }

    // Inventory serialization
    {
      list<pair<string, int> >::iterator it   = inventory.begin();
      list<pair<string, int> >::iterator end  = inventory.end();
      int                                size = inventory.size();

      packet << size;
      for (; it != end ; ++it)
        packet << (*it).first << (*it).second;
    }
}
