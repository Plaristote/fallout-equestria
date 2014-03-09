#include "world/world.h"
#include "world/map_object.hpp"
#include "serializer.hpp"

using namespace std;

extern unsigned int blob_revision;

void MapObject::InitializeTree(World *world)
{
  std::function<void (MapObject&)> find_parents = [world](MapObject& object)
  {
    if (object.parent != "")
    {
      MapObject*     parent = world->GetMapObjectFromName(object.parent);

      if (parent == 0)
        parent = world->GetDynamicObjectFromName(object.parent);
      if (parent)
        object.nodePath.reparent_to(parent->nodePath);
    }
  };

  for_each(world->objects.begin(),        world->objects.begin(),        [find_parents](MapObject&     object) { find_parents(object); });
  for_each(world->dynamicObjects.begin(), world->dynamicObjects.begin(), [find_parents](DynamicObject& object) { find_parents(object); });
}

void MapObject::InitializeCollider(Collider type, LPoint3f position, LPoint3f scale, LPoint3f hpr)
{
  PT(CollisionNode)  node_ptr;
  PT(CollisionSolid) solid_ptr;

  collider = type;
  switch (type)
  {
  default:
  case NONE:
    return ;
  case MODEL:
  case BOX:
      cout << "Creationg box collision" << endl;
      solid_ptr = new CollisionBox(LPoint3f(0, 0, 0), 1, 1, 1);
      break ;
  case SPHERE:
      cout << "Creating collision sphere" << endl;
      solid_ptr = new CollisionSphere(LPoint3(0, 0, 0), 1);
      break ;
  }
  node_ptr       = new CollisionNode("collision_node");
  collision_node = nodePath.attach_new_node(node_ptr);
  collision_node.set_pos(position);
  collision_node.set_scale(scale);
  collision_node.set_hpr(hpr);
  node_ptr->add_solid(solid_ptr);
}

void MapObject::SetFloor(unsigned char floor)
{
  std::for_each(waypoints.begin(), waypoints.end(), [floor](Waypoint* waypoint)
  {
     waypoint->floor = floor;
  });
  this->floor = floor;
}

void MapObject::ReparentTo(MapObject* object)
{
  if (object)
  {
    parent = object->nodePath.get_name();
    nodePath.reparent_to(object->nodePath);
  }
  else
    parent = "";
}

void MapObject::SetName(const std::string& name)
{
  this->name = name;
  if (!(nodePath.is_empty()))
    nodePath.set_name(name);
}

/*
 * Serialization
 */
void MapObject::Unserialize(Utils::Packet& packet)
{
  World* world = World::LoadingWorld;
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;

  packet >> name >> strModel >> strTexture;
  packet >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaleX >> scaleY >> scaleZ;
  packet >> floor;
  if (blob_revision >= 1)
    packet >> parent;

  if (world)
  {
    nodePath   = world->window->get_render().attach_new_node(name);
    render     = world->window->load_model(world->window->get_panda_framework()->get_models(), MODEL_ROOT + strModel);
    render.reparent_to(nodePath);
    if (render.is_empty())
    {
      std::cerr << "[World][Unserialize] Could not load model " << strModel << " for object '" << name << '\'' << std::endl;
    }
    else
    {
      nodePath.set_depth_offset(1);
      nodePath.set_two_sided(false);
      if (strTexture != "")
      {
        texture    = TexturePool::load_texture(TEXT_ROOT + strTexture);
        if (texture)
          render.set_texture(texture);
      }
      render.set_name(name);
      nodePath.set_hpr(rotX, rotY, rotZ);
      nodePath.set_scale(scaleX, scaleY, scaleZ);
      nodePath.set_pos(posX, posY, posZ);
      waypoints_root = nodePath.attach_new_node("waypoints");
    }
  }

  if (blob_revision >= 2)
    UnserializeWaypoints(world, packet);
  if (blob_revision >= 4)
  {
    unsigned char _collider;

    packet >> _collider;
    if (_collider != NONE && (blob_revision >= 5 || _collider != MODEL))
      packet >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaleX >> scaleY >> scaleZ;
    else if (_collider == MODEL)
    {
      LPoint3f scale = NodePathSize(render) / 2;

      scaleX = scale.get_x();
      scaleY = scale.get_y();
      scaleZ = scale.get_z();
    }
    InitializeCollider((Collider)_collider, LPoint3f(posX, posY, posZ), LPoint3f(scaleX, scaleY, scaleZ), LPoint3f(rotX, rotY, rotZ));
  }
  else
  {
    LPoint3f scale = NodePathSize(render) / 2;

    collider = MODEL;
    InitializeCollider(collider, LPoint3f(0, 0, 0), scale, LPoint3f(0, 0, 0));
  }
}

void MapObject::UnserializeWaypoints(World* world, Utils::Packet& packet)
{
  vector<int> waypoint_ids;

  packet >> waypoint_ids;
  if (world)
  {
    for_each(waypoint_ids.begin(), waypoint_ids.end(), [this, world](int id)
    {
      Waypoint* wp = world->GetWaypointFromId(id);

      if (wp)
      {
        wp->floor = this->floor;
        waypoints.push_back(wp);
  #ifdef GAME_EDITOR
        if (!(wp->nodePath.is_empty()))
          wp->nodePath.reparent_to(waypoints_root);
  #endif
      }
    });
  }
}

void MapObject::Serialize(Utils::Packet& packet) const
{
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;

  posX   = nodePath.get_pos().get_x();
  posY   = nodePath.get_pos().get_y();
  posZ   = nodePath.get_pos().get_z();
  rotX   = nodePath.get_hpr().get_x();
  rotY   = nodePath.get_hpr().get_y();
  rotZ   = nodePath.get_hpr().get_z();
  scaleX = nodePath.get_scale().get_x();
  scaleY = nodePath.get_scale().get_y();
  scaleZ = nodePath.get_scale().get_z();
  packet << name << strModel << strTexture;
  packet << posX << posY << posZ << rotX << rotY << rotZ << scaleX << scaleY << scaleZ;
  packet << floor;
  packet << parent; // Revision #1
  {
    std::vector<int> waypoint_ids;

    waypoint_ids.resize(waypoints.size());
    for_each(waypoints.begin(), waypoints.end(), [&waypoint_ids](Waypoint* wp)
    {
      waypoint_ids.push_back(wp->id);
    });
    packet << waypoint_ids;
  } // #Revision2
  {
    unsigned char _collider = (unsigned char)collider;

    packet << _collider;
    if (collider != NONE)
    {
      posX = collision_node.get_pos().get_x(); rotX = collision_node.get_hpr().get_x(); scaleX = collision_node.get_scale().get_x();
      posY = collision_node.get_pos().get_y(); rotY = collision_node.get_hpr().get_y(); scaleY = collision_node.get_scale().get_y();
      posZ = collision_node.get_pos().get_z(); rotZ = collision_node.get_hpr().get_z(); scaleZ = collision_node.get_scale().get_z();

      packet << posX << posY << posZ << rotX << rotY << rotZ << scaleX << scaleY << scaleZ;
    }
  } // #Revision4/5
}
