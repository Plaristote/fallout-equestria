#ifndef  WORLD_MAP_OBJECT_HPP
# define WORLD_MAP_OBJECT_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <vector>
# include "serializer.hpp"

struct Waypoint;
struct World;

struct MapObject : public Utils::Serializable
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

  std::string   name;
  std::string   strModel;
  std::string   strTexture;
  std::string   parent;
  Collider      collider;

  void          SetName(const std::string&);
  void          SetFloor(unsigned char floor);
  void          ReparentTo(MapObject* object);

  void          UnserializeWaypoints(World*, Utils::Packet& packet);
  virtual void  Unserialize(Utils::Packet& packet);
  virtual void  Serialize(Utils::Packet& packet) const;
  static void   InitializeTree(World* world);
  void          InitializeCollider(Collider type, LPoint3f position, LPoint3f scale, LPoint3f hpr);
  
  bool          IsCuttable(void) const;
};


#endif
