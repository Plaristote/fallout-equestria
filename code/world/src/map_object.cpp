#include "world/world.h"
#include "world/map_object.hpp"
#include "serializer.hpp"
#ifdef GAME_EDITOR
# include "qpandaapplication.h"
#else
  extern PandaFramework* framework;
#endif

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
      solid_ptr = new CollisionBox(LPoint3f(0, 0, 0), 1, 1, 1);
      break ;
  case SPHERE:
      solid_ptr = new CollisionSphere(LPoint3(0, 0, 0), 1);
      break ;
  }
  node_ptr       = new CollisionNode("collision_node");
  collision_node = nodePath.attach_new_node(node_ptr);
  collision_node.set_pos(position);
  collision_node.set_scale(scale);
  collision_node.set_hpr(hpr);
  collision_node.set_collide_mask(CollideMask(ColMask::FovBlocker));
  node_ptr->add_solid(solid_ptr);
}

void MapObject::SetFloor(unsigned char floor)
{
  for_each(waypoints.begin(), waypoints.end(), [floor](Waypoint* waypoint)
  {
     waypoint->floor = floor;
  });
  for_each(children.begin(), children.end(), [floor](MapObject* child)
  {
    if (child->inherits_floor)
      child->SetFloor(floor);
  });
  this->floor = floor;
}

MapObject::~MapObject()
{
  ReparentTo(0);
}

void MapObject::ReparentTo(MapObject* object)
{
  if (parent_object)
    parent_object->children.erase(find(parent_object->children.begin(), parent_object->children.end(), this));
  parent_object   = object;
  if (object)
  {
    parent        = object->nodePath.get_name();
    object->children.push_back(this);
    if (object->floor != floor)
      SetFloor(object->floor);
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

void MapObject::SetModel(const std::string& model)
{
#ifdef GAME_EDITOR
  PandaFramework& panda_framework = QPandaApplication::Framework();
#else
  PandaFramework& panda_framework = *framework;
#endif

  if (!(render.is_empty()))
    render.remove_node();
  strModel = model;
  render   = panda_framework.get_window(0)->load_model(panda_framework.get_models(), MODEL_ROOT + strModel);
  render.set_name("render-" + nodePath.get_name());
  //SetCollideMaskOnSingleNodepath(render, ColMask::Render);
  render.set_collide_mask(CollideMask(ColMask::Render));
  render.reparent_to(nodePath);
  if (!(render.is_empty()))
    SetTexture(strTexture);
  else
    std::cerr << "[MapObject][SetModel] Could not load model " << strModel << " for object '" << name << '\'' << std::endl;
}

void MapObject::SetTexture(const std::string& new_texture)
{
  strTexture = new_texture;
  if (!(render.is_empty()) && strTexture != "")
  {
    texture    = TexturePool::load_texture(TEXT_ROOT + strTexture);
    if (texture)
      render.set_texture(texture);
    else
      std::cerr << "[MapObject][SetTexture] Could not load texture " << strTexture << " for object '" << name << '\'' << std::endl;
  }
}

/*
 * Serialization
 */
void MapObject::Unserialize(Utils::Packet& packet)
{
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;
  char   inherits_floor = true;

  world = World::LoadingWorld;
  packet >> name >> strModel >> strTexture;
  packet >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaleX >> scaleY >> scaleZ;
  packet >> floor;
  if (blob_revision >= 1)
    packet >> parent;
  if (blob_revision >= 10)
    packet >> inherits_floor;
  this->inherits_floor = inherits_floor;

  if (name == "")
    name = "name-not-found";

  if (world)
  {
    nodePath   = world->window->get_render().attach_new_node(name);
    nodePath.set_name(name);
    SetModel(strModel);
    nodePath.set_depth_offset(1);
    nodePath.set_two_sided(false);
    nodePath.set_hpr(rotX, rotY, rotZ);
    nodePath.set_scale(scaleX, scaleY, scaleZ);
    nodePath.set_pos(posX, posY, posZ);
    waypoints_root = nodePath.attach_new_node("waypoints");
    world->MapObjectChangeFloor(*this, floor);
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

  InitializeCollideMask();
}

void MapObject::UnserializeWaypoints(World* world, Utils::Packet& packet)
{
  vector<int> waypoint_ids;

  packet >> waypoint_ids;
  if (world)
  {
    for_each(waypoint_ids.begin(), waypoint_ids.end(), [this, world](int id)
    {
      Waypoint* waypoint = world->GetWaypointFromId(id);

      if (waypoint)
      {
        waypoint->floor       = this->floor;
        waypoint->parent_path = this->render;
        waypoints.push_back(waypoint);
  #ifdef GAME_EDITOR
        if (!(waypoint->nodePath.is_empty()))
          waypoint->nodePath.reparent_to(waypoints_root);
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
  packet << (char)inherits_floor;
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

void MapObject::InitializeCollideMask(void)
{
  int flag     = GetObjectCollideMask();
  int col_flag = ColMask::FovBlocker;

  if (collider == MapObject::MODEL)
    col_flag |= ColMask::CheckCollisionOnModel;
  if (waypoints.size() > 0)
    col_flag |= ColMask::WpPlane;
  nodePath.set_collide_mask(flag);
  collision_node.set_collide_mask(col_flag);
}

bool MapObject::IsCuttable(void) const
{
#ifndef GAME_EDITOR
  string name       = nodePath.get_name();
  string patterns[] = { "Wall", "wall", "Ceiling", "ceiling" };

  for (unsigned short i = 0 ; i < 4 ; ++i)
  {
    if (starts_with(name, patterns[i]))
      return (true);
  }
#endif
  return (false);
}
