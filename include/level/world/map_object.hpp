#ifndef  WORLD_MAP_OBJECT_HPP
# define WORLD_MAP_OBJECT_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <vector>
# include "serializer.hpp"

struct Waypoint;
struct World;

struct MapObject
{
  typedef std::vector<Waypoint*> Waypoints;

  enum Collider
  {
    NONE,
    MODEL,
    BOX,
    SPHERE
  };

  NodePath      nodePath, render, collision_node;
  PT(Texture)   texture;
  unsigned char floor;
  Waypoints     waypoints;
  NodePath      waypoints_root;

  std::string   strModel;
  std::string   strTexture;
  std::string   parent;
  Collider      collider;

  void          SetFloor(unsigned char floor);
  void          ReparentTo(MapObject* object);

  void          UnSerialize(World* world, Utils::Packet& packet);
  void          UnserializeWaypoints(World*, Utils::Packet& packet);
  void          Serialize(Utils::Packet& packet);
  static void   InitializeTree(World* world);
  void          InitializeCollider(Collider type, LPoint3f position, LPoint3f scale, LPoint3f hpr);
};


#endif
