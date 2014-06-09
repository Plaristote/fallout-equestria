#include "world/world.h"
#include "world/dynamic_object.hpp"
#ifndef GAME_EDITOR
  extern bool world_is_game_save;
#endif
using namespace std;

extern unsigned int blob_revision;

/*
 * Serialization
 */
void DynamicObject::Unserialize(Utils::Packet& packet)
{
  World* world = World::LoadingWorld;
  int    iType;
  char   iLocked;
  int    iWaypoint;

  MapObject::Unserialize(packet);
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
  if (world)
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

void DynamicObject::Serialize(Utils::Packet& packet) const
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
        list<pair<int, int> >::const_iterator it  = lockedArcs.begin();
        list<pair<int, int> >::const_iterator end = lockedArcs.end();
        int size = lockedArcs.size();

        packet << size;
        for (; it != end ; ++it)
          packet << (*it).first << (*it).second;
    }

    // Inventory serialization
    {
      list<pair<string, int> >::const_iterator it   = inventory.begin();
      list<pair<string, int> >::const_iterator end  = inventory.end();
      int                                      size = inventory.size();

      packet << size;
      for (; it != end ; ++it)
        packet << (*it).first << (*it).second;
    }
}

void DynamicObject::ReparentToFloor(World *world, unsigned char floor)
{
  // TODO Fix inherits_floor feature for dynamic objects ?
/*#ifndef GAME_EDITOR
  LPoint3f current_pos   = nodePath.get_pos(world->window->get_render());
  LPoint3f current_hpr   = nodePath.get_hpr(world->window->get_render());
  LPoint3f current_scale = nodePath.get_scale(world->window->get_render());
#endif

  world->DynamicObjectChangeFloor(*this, floor);
#ifndef GAME_EDITOR
  if (!world_is_game_save)
  {
    nodePath.set_pos(world->window->get_render(), current_pos);
    nodePath.set_hpr(world->window->get_render(), current_hpr);
    nodePath.set_scale(world->window->get_render(), current_scale);
  }
#endif*/
}
