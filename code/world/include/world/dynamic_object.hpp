#ifndef  WORLD_DYNAMIC_OBJECT_HPP
# define WORLD_DYNAMIC_OBJECT_HPP

# include "map_object.hpp"
# include <list>

struct DynamicObject : public MapObject
{
  DynamicObject() : waypoint(0), type(Shelf), interactions(0), locked(false)
  {
  }

  void Unserialize(Utils::Packet& packet);
  void Serialize(Utils::Packet& packet) const;

  enum Type
  {
    Door,
    Shelf,
    Locker,
    Character,
    Item
  };

  Waypoint*   waypoint;
  Type        type;
  // Interactions
  char        interactions;
  std::string dialog;
  // All
  std::string script;
  std::string sound_pack;
  // Character
  std::string charsheet;


  // Door / Locker
  bool                            locked;
  std::string                     key; // Also used to store item names for DynamicObject::Item
  std::list<std::pair<int, int> > lockedArcs;

  // Shelf / Character
  std::list<std::pair<std::string, int> > inventory;
};


#endif
