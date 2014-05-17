#include "world/world.h"
#include <panda3d/collisionHandlerQueue.h>
#include <panda3d/collisionBox.h>
#include <panda3d/collisionSphere.h>
#include <panda3d/collisionRay.h>
#define CURRENT_BLOB_REVISION 13

using namespace std;

unsigned int          blob_revision = CURRENT_BLOB_REVISION;
World*                World::LoadingWorld = 0;
#ifndef GAME_EDITOR
bool                  world_is_game_save = false;
#endif

World::World(WindowFramework* window)
{
  this->window         = window;
  model_sphere         = window->load_model(window->get_panda_framework()->get_models(), std::string(MODEL_ROOT) + "misc/sphere.egg.pz");
  floors_node          = window->get_render().attach_new_node("floors");
  rootWaypoints        = window->get_render().attach_new_node("waypoints");
  rootMapObjects       = window->get_render().attach_new_node("mapobjects");
  rootDynamicObjects   = window->get_render().attach_new_node("dynamicobjects");
  rootLights           = window->get_render().attach_new_node("lights");
  rootParticleObjects  = window->get_render().attach_new_node("particleObjects");
  sunlight_enabled     = false;
#ifdef GAME_EDITOR
  lightSymbols         = window->get_render().attach_new_node("lightSymbols");
  do_compile_doors     = true;
  do_compile_waypoints = true;
#endif
  debug_pathfinding    = window->get_render().attach_new_node("debug_pathfinding");
  //rootWaypoints.set_collide_mask(ColMask::Waypoint);
}

World::~World()
{
  ForEach(waypoints,      [](Waypoint& wp)      { wp.nodePath.remove_node(); });
  ForEach(objects,        [](MapObject& mo)     { mo.nodePath.remove_node(); });
  ForEach(dynamicObjects, [](DynamicObject& dy) { dy.nodePath.remove_node(); });
  ForEach(lights,         [](WorldLight& wl)    { wl.Destroy();              });
  debug_pathfinding.remove_node();
  floors_node.remove_node();
}

void World::WriteToBam(const string &name)
{
  window->get_render().write_bam_file(name);
}

Waypoint* World::AddWayPoint(float x, float y, float z)
{
  NodePath  nodePath = rootWaypoints.attach_new_node("waypoint");
  Waypoint  waypoint(nodePath);
  Waypoint* ptr;

  model_sphere.instance_to(nodePath);
  waypoint.nodePath.set_pos(x, y, z);
  waypoints.push_back(waypoint);
  nodePath.reparent_to(rootWaypoints);
  ptr = &(*(--(waypoints.end())));
  return (ptr);
}

void World::DeleteWayPoint(Waypoint* toDel)
{
  {
    Waypoints::iterator it = find(waypoints.begin(), waypoints.end(), toDel);

    if (it != waypoints.end())
    {
      toDel->DisconnectAll();
      toDel->nodePath.remove_node();
      waypoints.erase(it);
    }
  }
  for_each(objects.begin(), objects.end(), [toDel](MapObject& object)
  {
    auto it = find(object.waypoints.begin(), object.waypoints.end(), toDel);

    if (it != object.waypoints.end())
      object.waypoints.erase(it);
  });
}

void   World::GetWaypointLimits(short currentFloor, LPoint3& upperRight, LPoint3& upperLeft, LPoint3& bottomLeft) const
{
  Waypoints::const_iterator it  = waypoints.begin();
  Waypoints::const_iterator end = waypoints.end();

  for (; it != end ; ++it)
  {
    const Waypoint& wp  = *it;
    LPoint3   pos = wp.nodePath.get_pos();

    if (wp.floor != currentFloor)
      continue ;
    bottomLeft.set_z(pos.get_z());
    if (pos.get_x() < upperLeft.get_x())
    {
      upperLeft.set_x(pos.get_x());
      bottomLeft.set_x(pos.get_x());
    }
    if (pos.get_x() > upperRight.get_x())
      upperRight.set_x(pos.get_x());
    if (pos.get_y() > upperLeft.get_y())
    {
      upperLeft.set_y(pos.get_y());
      upperRight.set_y(pos.get_y());
    }
    if (pos.get_y() < bottomLeft.get_y())
      bottomLeft.set_y(pos.get_y());
  }
  upperRight.set_z(bottomLeft.get_z());
  upperLeft.set_z(bottomLeft.get_z());
}

Waypoint* World::GetWaypointClosest(LPoint3 pos_1, unsigned char floor)
{
  Waypoints::iterator it        = waypoints.begin();
  Waypoints::iterator end       = waypoints.end();
  Waypoint*           best      = 0;
  float               bestScore = 0;

  for (; it != end ; ++it)
  {
    if ((*it).floor != floor) continue ;
    LPoint3 pos_2  = (*it).nodePath.get_pos();
    float   dist_x = pos_1.get_x() - pos_2.get_x();
    float   dist_y = pos_1.get_y() - pos_2.get_y();
    float   score  = SQRT(dist_x * dist_x + dist_y * dist_y);

    if (score <= bestScore || (bestScore == 0 && best == 0))
    {
      best      = &(*it);
      bestScore = score;
    }
  }
  return (best);
}

Waypoint* World::GetWaypointFromNodePath(NodePath path)
{
  Waypoints::iterator it  = waypoints.begin();
  Waypoints::iterator end = waypoints.end();

  for (; it != end ; ++it)
  {
      if ((*it).nodePath.is_ancestor_of(path))
    return (&(*it));
  }
  return (0);
}

#ifndef GAME_EDITOR
Waypoint* World::GetWaypointFromId(unsigned int id)
{
  if (waypoints.size() >= id && id > 0)
  {
    Waypoint& wp = waypoints[id - 1];

    if (wp.id != id)
      cout << "/!\\ RETARD ALERT ! Wrong waypoint (" << wp.id << ", looking for " << id << ')' << endl;
    return (&waypoints[id - 1]);
  }
  return (0);
}
#else
Waypoint* World::GetWaypointFromId(unsigned int id)
{
  Waypoints::iterator it = std::find(waypoints.begin(), waypoints.end(), id);

  if (it != waypoints.end())
    return (&(*it));
  return (0);
}
#endif

void World::FloorResize(int newSize)
{
  size_t currentSize = floors.size();

  floors.resize(newSize);
  for (int it = currentSize ; it < newSize ; ++it)
  {
    std::stringstream stream;

    stream << "floor-" << it;
    floors[it] = floors_node.attach_new_node(stream.str());
    floors[it].attach_new_node("mapobjects");
    floors[it].attach_new_node("dynamicobjects");
    floors[it].attach_new_node("lights");
    floors[it].show();
  }
}

MapObject* World::AddMapObject(const string &name, const string &model, const string &texture, float x, float y, float z)
{
  MapObject object;
  string    model2 = model;

  object.name       = name;
  object.strModel   = model;
  object.strTexture = texture;
  object.nodePath   = window->get_render().attach_new_node(name);
  object.SetModel(model2);
  object.nodePath.set_pos(x, y, z);
  object.waypoints_root = object.nodePath.attach_new_node("waypoints");
  MapObjectChangeFloor(object, 0);
  {
    LPoint3f scale = NodePathSize(object.render) / 2;

    object.collider.parent = object.nodePath;
    object.collider.InitializeCollider(Collider::MODEL, LPoint3f(0,0,0), scale, LPoint3f(0,0,0));
    object.InitializeCollideMask();
  }
  objects.push_back(object);
  return (&(*(--(objects.end()))));
}

void World::ObjectChangeFloor(MapObject& object, unsigned char floor, unsigned short type)
{
  if (floors.size() <= floor) FloorResize(floor + 1);
  object.nodePath.reparent_to(floors[floor].get_child(type));
  object.floor = floor;
}

void World::MapObjectChangeFloor(MapObject& object, unsigned char floor)
{
  ObjectChangeFloor(object, floor, 0);
}

void World::DynamicObjectChangeFloor(DynamicObject& object, unsigned char floor)
{
  ObjectChangeFloor(object, floor, 1);
}

void World::DeleteMapObject(MapObject* ptr)
{
  if (ptr)
  {
    // Children waypoints removal
    {
      MapObject::Waypoints::iterator it;

      while ((it = ptr->waypoints.begin()) != ptr->waypoints.end())
        DeleteWayPoint(*it);
    }
    // Children mapobjects removal
    {
      auto it = objects.begin();

      while (it != objects.end())
      {
        if (it->parent == ptr->nodePath.get_name())
        {
          DeleteMapObject(&(*it));
          it = objects.begin();
        }
        else
          ++it;
      }
    }
    DeleteObject(ptr, objects);
  }
}

MapObject* World::GetObjectFromName(const string& name)
{
  MapObject* result = GetObjectFromName(name, objects);

  return (result != 0 ? result : GetObjectFromName(name, dynamicObjects));
}

MapObject* World::GetMapObjectFromName(const string &name)
{
  return (GetObjectFromName(name, objects));
}

MapObject* World::GetMapObjectFromNodePath(NodePath path)
{
  return (GetObjectFromNodePath(path, objects));
}

MapObject* World::GetMapObjectFromCollisionNode(NodePath path)
{
  auto it  = objects.begin();
  auto end = objects.end();

  for (; it != end ; ++it)
  {
    MapObject& object = *it;

    if (!(object.collider.node.is_empty()) && object.collider.node.is_ancestor_of(path))
      return (&object);
  }
  return (0);
}

void World::ReparentObject(MapObject *object, MapObject *new_parent)
{
  if (new_parent)
    object->ReparentTo(new_parent);
  else
  {
    object->parent = "";
    object->nodePath.reparent_to(floors_node);
  }
}

void World::ReparentObject(MapObject* object, const std::string& name)
{
  MapObject*     map_object = GetMapObjectFromName(name);
  DynamicObject* dyn_object = (map_object == 0 ? GetDynamicObjectFromName(name) : 0);

  if (map_object)
    ReparentObject(object, map_object);
  else if (dyn_object)
    ReparentObject(object, dyn_object);
}

// DYNAMIC OBJECTS
DynamicObject* World::InsertDynamicObject(DynamicObject& object)
{
  cout << "INSERTING OBJECT " << object.name << " with model " << object.strModel << " and texture " << object.strTexture << endl;
  cout << "Its floor is " << object.floor << endl;
  object.waypoint = 0;
  object.nodePath = window->get_render().attach_new_node(object.name);
  object.SetModel(object.strModel);
  if (object.render.is_empty())
    return (0);
  object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
  dynamicObjects.insert(dynamicObjects.begin(), object);
  DynamicObjectChangeFloor(*dynamicObjects.begin(), object.floor);
  return (&(*dynamicObjects.begin()));
}

DynamicObject* World::AddDynamicObject(const string &name, DynamicObject::Type type, const string &model, const string &texture)
{
  DynamicObject  object;
  DynamicObject* ptr;

  object.name         = name;
  object.type         = type;
  object.interactions = 0;
  object.strModel     = model;
  object.strTexture   = texture;
  ptr                 = InsertDynamicObject(object);
  if (ptr && !(ptr->nodePath.is_empty()))
    ptr->nodePath.set_name(name);
  return (ptr);
}

void World::DeleteDynamicObject(DynamicObject* ptr)
{
  DeleteObject(ptr, dynamicObjects);
}

DynamicObject* World::GetDynamicObjectFromName(const string &name)
{
  return (GetObjectFromName(name, dynamicObjects));
}

DynamicObject* World::GetDynamicObjectFromNodePath(NodePath path)
{
  return (GetObjectFromNodePath(path, dynamicObjects));
}

// ENTRY ZONES
void World::AddZone(const std::string& name)
{
  Zone zone;

  zone.name = name;
  zones.push_back(zone);
}

void World::DeleteZone(const std::string& name)
{
  Zones::iterator it = std::find(zones.begin(), zones.end(), name);

  if (it != zones.end())
    zones.erase(it);
}

Zone* World::GetZoneByName(const std::string& name)
{
  Zones::iterator it = std::find(zones.begin(), zones.end(), name);

  if (it != zones.end())
    return (&(*it));
  return (0);
}

// Lights
void World::AddLight(WorldLight::Type type, const std::string& name)
{
  lights.push_back(WorldLight(type, WorldLight::Type_None, rootLights, name));
#ifdef GAME_EDITOR
  lights.rbegin()->symbol.reparent_to(rootLights);
#endif
}

void World::AddLight(WorldLight::Type type, const std::string& name, MapObject* parent)
{
  AddLight(type, name);
  lights.rbegin()->ReparentTo(parent);
}

void World::AddLight(WorldLight::Type type, const std::string& name, DynamicObject* parent)
{
  AddLight(type, name);
  lights.rbegin()->ReparentTo(parent);
}

void World::DeleteLight(const std::string& name)
{
  WorldLights::iterator it = std::find(lights.begin(), lights.end(), name);

  if (it != lights.end())
  {
    it->nodePath.detach_node();
    lights.erase(it);
  }
}

void World::DeleteParticleObject(const string& name)
{
  auto it = find(particleObjects.begin(), particleObjects.end(), name);

  if (it != particleObjects.end())
    particleObjects.erase(it);
}

WorldLight* World::GetLightByName(const std::string& name)
{
  WorldLights::iterator it = std::find(lights.begin(), lights.end(), name);

  cout << "Getting light by name... light count: " << lights.size() << endl;
  if (it != lights.end())
    return (&(*it));
  return (0);
}

ParticleObject* World::GetParticleObjectByName(const string& name)
{
  auto it = std::find(particleObjects.begin(), particleObjects.end(), name);

  if (it != particleObjects.end())
    return (&(*it));
  return (0);
}

string get_nodepath_path(NodePath np)
{
  NodePath parent = np.get_parent();

  if (parent.is_empty())
    return (np.get_name());
  return (get_nodepath_path(parent) + '/' + np.get_name());
}

void        World::CompileLight(WorldLight* light, unsigned char colmask)
{
  PT(CollisionSphere)       colSphere = new CollisionSphere(light->nodePath.get_x(),
                               light->nodePath.get_y(),
                               light->nodePath.get_z(),
                               light->zoneSize);
  PT(CollisionNode)         colNode       = new CollisionNode("compileLightSphere");
  NodePath                  colNp         = rootLights.attach_new_node(colNode);
  PT(CollisionHandlerQueue) handlerQueue  = new CollisionHandlerQueue();
  CollisionTraverser        traverser;
  list<NodePath>::iterator  it            = light->enlightened.begin();
  list<NodePath>::iterator  end           = light->enlightened.end();

  if (light->collider.type == Collider::NONE)
  {
    colNode->add_solid(colSphere);
    light->collider.node = colNp;
    light->collider.node.reparent_to(light->nodePath);
  }
  else
    colNp = light->collider.node;
  colNode = reinterpret_cast<CollisionNode*>(light->collider.node.node());
  cout << colNp.get_scale().get_x() << ", " << colNp.get_scale().get_y() << ", " << colNp.get_scale().get_z() << endl;
  traverser.add_collider(colNp, handlerQueue);

  // Reseting the objects corresponding to the collision mask
#ifdef GAME_EDITOR
  while (it != end)
  {
    NodePath     np     = *it;
    unsigned int npmask = np.get_collide_mask().get_word();

    if (npmask & colmask)
    {
      np.set_light_off(light->nodePath);
      it = light->enlightened.erase(it);
    }
    else
      ++it;
  }
#endif

  // Detecting the new collisions with colmask, and setting the light
  colNode->set_into_collide_mask(colmask);
  colNode->set_from_collide_mask(colmask);
  traverser.traverse(window->get_render());

  cout << "Waypoint count:   " << waypoints.size() << endl;
  cout << "Light '" << light->name << "' collisions: " << handlerQueue->get_num_entries() << endl;
  handlerQueue->output(cout);

  if (colmask & ColMask::Waypoint)
  {
    auto it  = waypoints.begin();
    auto end = waypoints.end();

    for (; it != end ; ++it)
    {
      if (it->GetDistanceEstimate(light->nodePath.get_pos()) <= light->collider.node.get_scale(window->get_render()).get_x())
        it->lights.push_back(light);
    }
  }

  string last_path;

  for (unsigned int i = 0 ; i < handlerQueue->get_num_entries() ; ++i)
  {
    NodePath        node  = handlerQueue->get_entry(i)->get_into_node_path();

    if (node.is_empty())
      continue ;

    { // Avoid doublons
      string current_path = get_nodepath_path(node);
      if (current_path == last_path)
        continue ;
      last_path = current_path;
    }

    Waypoint*      waypoint  = (colmask & ColMask::Waypoint ? GetWaypointFromNodePath(node) : 0);
    MapObject*     object    = (!waypoint && (colmask & ColMask::Object) ? GetMapObjectFromNodePath(node) : 0);
    DynamicObject* dynObject = (object ? 0 : GetDynamicObjectFromNodePath(node));

    if (waypoint)
      waypoint->lights.push_back(light);
    else if (object || dynObject)
    {
      if (dynObject != 0)
        object = dynObject;
      cout << "Enlightening object " << object->name << endl;
      list<NodePath>::iterator alreadyRegistered;

      node = (object ? object->render : dynObject->render);
      alreadyRegistered = find(light->enlightened.begin(), light->enlightened.end(), node);
      if (alreadyRegistered == light->enlightened.end())
      {
        light->enlightened.push_back(node);
        node.set_light(light->nodePath, light->priority);
      }
    }
  }

  std::for_each(light->enlightened_index.begin(), light->enlightened_index.end(), [this, &light](const string& object_name)
  {
    MapObject* object = GetObjectFromName(object_name);

    if (object)
    {
      light->enlightened.push_back(object->nodePath);
      object->nodePath.set_light(light->nodePath, light->priority);
    }
  });

  //cout << "Number of enlightened objects -> " << light->enlightened.size() << endl;

  //colNp.detach_node();
}

void World::DynamicObjectSetWaypoint(DynamicObject& object, Waypoint& waypoint)
{
  if (object.waypoint == 0 || object.waypoint->floor != waypoint.floor)
  {
    DynamicObjectChangeFloor(object, waypoint.floor);
    object.nodePath.set_alpha_scale(floors[waypoint.floor].get_color_scale().get_w());
  }
  object.waypoint = &waypoint;
}

void World::SetWaypointsVisible(bool v)
{
  if (v)
  {
    rootWaypoints.show();
    std::for_each(objects.begin(), objects.end(), [](MapObject& object)
    {
      object.waypoints_root.show();
    });
  }
  else
  {
    rootWaypoints.hide();
    std::for_each(objects.begin(), objects.end(), [](MapObject& object)
    {
      object.waypoints_root.hide();
    });
  }
}

void           World::SetMapObjectsVisible(bool v)
{
  if (v)
  {
    rootMapObjects.show();
    for (unsigned int i = 0 ; i < floors.size() ; ++i)
      floors[i].get_child(0).show();
  }
  else
  {
    rootMapObjects.hide();
    for (unsigned int i = 0 ; i < floors.size() ; ++i)
     floors[i].get_child(0).hide();
  }
}

void           World::SetDynamicObjectsVisible(bool v)
{
  if (v)
  {
    rootDynamicObjects.show();
    for (unsigned int i = 0 ; i < floors.size() ; ++i)
      floors[i].get_child(1).show();
  }
  else
  {
    rootDynamicObjects.hide();
    for (unsigned int i = 0 ; i < floors.size() ; ++i)
      floors[i].get_child(1).hide();
  }
}

/*
 * World
 */
void           World::UnSerialize(Utils::Packet& packet)
{
  LoadingWorld = this;
  if (blob_revision >= 1)
    packet >> blob_revision;
  cout << "Blob revision is  " << blob_revision << endl;

  cout << "Unserialize waypoints" << endl;
  // Waypoints
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      NodePath sphere = rootWaypoints.attach_new_node("waypoint");
      Waypoint waypoint(sphere);

      model_sphere.instance_to(sphere);
      waypoint.Unserialize(packet);
      if (!sphere.is_empty())
        sphere.reparent_to(rootWaypoints);
      waypoints.push_back(waypoint);
    }

    Waypoints::iterator it, end;

    for (it = waypoints.begin(), end = waypoints.end() ; it != end ; ++it)
    {
      (*it).UnserializeLoadArcs(this);
    }
  }

    cout << "Unserialize map objects" << endl;
  // MapObjects
  if (blob_revision < 11)
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      MapObject     object;

      packet >> object;
      objects.push_back(object);
    }
  }
  else
    packet >> objects;

    cout << "Unserialize dynamic objects" << endl;
  // DynamicObjects
  if (blob_revision < 11)
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      DynamicObject object;
      unsigned char floor;

      packet >> object;
      floor        = object.floor;
      object.floor = (floor == 0 ? 1 : 0);
      DynamicObjectChangeFloor(object, floor);
      object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
      dynamicObjects.push_back(object);
    }
  }
  else
    packet >> dynamicObjects;

    cout << "Unserialize lights" << endl;
  // Lights
  if (blob_revision >= 7)
    packet >> lights;
  else
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      WorldLight light(window->get_render());

      light.Unserialize(packet);
      lights.push_back(light);
    }
  }

  cout << "Unserialize particle objects" << endl;
  if (blob_revision >= 13)
    packet >> particleObjects;

  cout << "Unserialize zones" << endl;
  if (blob_revision >= 7)
    packet >> zones;
  else // Revisions < 7 used EntryZone and ExitZone types
  {
    // ExitZones
    {
      int size;

      packet >> size;
      for (int it = 0 ; it < size ; ++it)
      {
        Zone       zone;
        std::list<int> waypointsId;

        packet >> zone.name;
        packet >> zone.destinations;
        packet >> waypointsId;

        std::list<int>::iterator begin = waypointsId.begin();
        std::list<int>::iterator end   = waypointsId.end();

        for (; begin != end ; ++begin)
        {
          Waypoint* wp = GetWaypointFromId(*begin);

        if (wp)
          zone.waypoints.push_back(wp);
        }
        zones.push_back(zone);
      }
    }

    // EntryZone
    {
      int size;

      packet >> size;
      for (int it = 0 ; it < size ; ++it)
      {
        Zone      zone;
        std::list<int> waypointsId;

        packet >> zone.name;
        packet >> waypointsId;
        std::list<int>::iterator begin = waypointsId.begin();
        std::list<int>::iterator end   = waypointsId.end();

        for (; begin != end ; ++begin)
        {
          Waypoint* wp = GetWaypointFromId(*begin);

          if (wp)
          {
            zone.waypoints.push_back(wp);
            zone.waypoints_ids.push_back(wp->id);
          }
        }
        zones.push_back(zone);
      }
    }
  } // End compatibility with Revision < 7

  cout << "Unserialize sunlight" << endl;
  {
    char serialize_sunlight_enabled;

    packet >> serialize_sunlight_enabled;
    sunlight_enabled = serialize_sunlight_enabled != 0;
  }

    cout << "Solving branch relations" << endl;
  /*
   * Solving branching relations between MapObjects
   */
  UpdateMapTree();

#ifdef GAME_EDITOR
  if (blob_revision >= 6)
  {
    std::for_each(objects.begin(), objects.end(), [this](MapObject& object)
    {
      unsigned int i = 0;

      for (; i < object.waypoints.size() ; ++i)
      {
        Waypoint* wp = object.waypoints[i];
        wp->nodePath.set_pos(window->get_render(), wp->nodePath.get_pos());
      }
    });
  }
#endif

#ifndef GAME_EDITOR
  //if (blob_revision < 8)
    CompileWaypointsFloorAbove();
#endif

    cout << "Compiling lights" << endl;
  // Post-loading stuff
#ifndef GAME_EDITOR
  for_each(lights.begin(), lights.end(), [this](WorldLight& light) { CompileLight(&light, ColMask::Object | ColMask::DynObject | ColMask::Waypoint); });
#else
  for_each(lights.begin(), lights.end(), [this](WorldLight& light) { CompileLight(&light, ColMask::Object | ColMask::DynObject); });
#endif
  LoadingWorld = 0;
}

void           World::UpdateMapTree(void)
{
  std::function<void (NodePath, std::string)> set_relations = [this, &set_relations](NodePath parent, std::string solving_for)
  {
    // Solving for MapObjects
    {
      auto it  = objects.begin();
      auto end = objects.end();

      for (; it != end ; ++it)
      {
        if (it->nodePath == parent)
          continue ;
        //std::cout << "Solving for: '" << solving_for << "'. Current item: '" << it->nodePath.get_name() << '\'' << std::endl;
        if (it->parent == solving_for)
        {
          if (solving_for != "" && !parent.is_empty())
          {
            it->nodePath.reparent_to(parent);
#ifndef GAME_EDITOR
            if (!it->inherits_floor)
            {
              LPoint3f current_pos = it->nodePath.get_pos(window->get_render());
              LPoint3f current_hpr = it->nodePath.get_hpr(window->get_render());
              LPoint3f current_scale = it->nodePath.get_scale(window->get_render());
              MapObjectChangeFloor((*it), it->floor);
              if (!world_is_game_save)
              {
                it->nodePath.set_pos(window->get_render(), current_pos);
                it->nodePath.set_hpr(window->get_render(), current_hpr);
                it->nodePath.set_scale(window->get_render(), current_scale);
              }
            }
#endif
          }
          else
          {
            if (floors.size() <= it->floor)
              FloorResize(it->floor + 1);
            it->nodePath.reparent_to(floors[it->floor]);
          }
        }
      }
    }

    // Solving for DynamicObjects
    {
      auto it  = dynamicObjects.begin();
      auto end = dynamicObjects.end();

      for (; it != end ; ++it)
      {
        if (it->nodePath == parent)
          continue ;
        if (it->parent == solving_for)
        {
          if (solving_for != "")
            it->nodePath.reparent_to(parent);
          else
          {
            if (floors.size() <= it->floor)
              FloorResize(it->floor + 1);
            it->nodePath.reparent_to(floors[it->floor]);
          }
        }
      }
    }
  };

  std::for_each(objects.begin(), objects.end(), [this, &set_relations](MapObject& object)
  { set_relations(object.nodePath, object.nodePath.get_name()); });
  std::for_each(dynamicObjects.begin(), dynamicObjects.end(), [this, &set_relations](DynamicObject& object)
  { set_relations(object.nodePath, object.nodePath.get_name()); });
  set_relations(floors_node, "");
}

void           World::Serialize(Utils::Packet& packet, std::function<void (const std::string&, float)> progress_callback)
{
  // Compile Step
# ifdef GAME_EDITOR
  if (do_compile_waypoints)
    CompileWaypoints(progress_callback);
  auto it = waypoints.begin();
  auto end = waypoints.end();

  while (it != end)
  {
    if (it->nodePath.get_parent() == rootWaypoints)
    {
      auto current = it;

      ++it;
      DeleteWayPoint(&(*current));
    }
    else
      ++it;
  }

  CompileWaypointsFloorAbove();
# endif

  packet << (unsigned int)CURRENT_BLOB_REVISION; // #blob revision

  // Waypoints
  {
    Waypoints::iterator it  = waypoints.begin();
    Waypoints::iterator end = waypoints.end();
    unsigned int        id  = 0;
    int                 size;

    cout << "Serializing " << waypoints.size() << " waypoints." << endl;
    while (it != end)
    {
      if ((*it).arcs.size() == 0)
        it = waypoints.erase(it);
      else
      {
        (*it).id = ++id;
        ++it;
      }
      progress_callback("Serializing Waypoints: ", (float)id / waypoints.size() * 100.f);
    }
    size = waypoints.size();
    packet << size;
    for (it = waypoints.begin() ; it != end ; ++it)
    (*it).Serialize(this, packet);
  }

#ifdef GAME_EDITOR
  if (do_compile_doors)
    CompileDoors(progress_callback);
#endif

  packet << objects << dynamicObjects << lights << particleObjects << zones;

  {
    char serialize_sunlight_enabled = sunlight_enabled;

    packet << serialize_sunlight_enabled;
  }
  progress_callback("Done serializing", 100);
}

// MAP COMPILING
void           World::CompileWaypointsFloorAbove()
{
  for_each(waypoints.begin(), waypoints.end(), [this](Waypoint& waypoint)
  {
    PT(CollisionRay)          pickerRay;
    PT(CollisionNode)         pickerNode;
    NodePath                  pickerPath;
    CollisionTraverser        collisionTraverser;
    PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();

    pickerNode   = new CollisionNode("isInsideBuildingRay");
    pickerPath   = window->get_render().attach_new_node(pickerNode);
    pickerRay    = new CollisionRay();
    pickerNode->add_solid(pickerRay);
    pickerNode->set_from_collide_mask(CollideMask(ColMask::FovBlocker));

    pickerPath.set_pos(waypoint.nodePath.get_pos());
    pickerPath.set_hpr(0, 0, 0);
    pickerRay->set_direction(0, 0, 10);

    collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
    collisionTraverser.traverse(floors_node);

    collisionHandlerQueue->sort_entries();

    waypoint.floor_above = waypoint.floor;

    for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry  = collisionHandlerQueue->get_entry(i);
      MapObject*      object = GetMapObjectFromNodePath(entry->get_into_node_path());

      if (!object)
      {
        DynamicObject* dynamic_object = GetDynamicObjectFromNodePath(entry->get_into_node_path());

        switch (dynamic_object->type)
        {
          case DynamicObject::Shelf:
          case DynamicObject::Door:
          case DynamicObject::Locker:
            object = dynamic_object;
          default:
            break ;
        }
      }
      if (object && object->floor > waypoint.floor)
        waypoint.floor_above = object->floor;
    }
    if (waypoint.suggested_floor_above > waypoint.floor &&
        (waypoint.floor_above > waypoint.suggested_floor_above || waypoint.floor_above == waypoint.floor))
      waypoint.floor_above = waypoint.suggested_floor_above;
    pickerPath.detach_node();
  });
}

void           World::CompileWaypoints(ProgressCallback progress_callback)
{
    unsigned int        i   = 0;
    Waypoints::iterator it  = waypoints.begin();
    Waypoints::iterator end = waypoints.end();

    for (; it != end ; ++it, ++i)
    {
        Waypoint::Arcs::iterator itArc  = (*it).arcs.begin();
        Waypoint::Arcs::iterator endArc = (*it).arcs.end();

        while (itArc != endArc)
        {
            NodePath          np;
            NodePath          other  = (*itArc).to->nodePath;
            NodePath          parent = (*it).nodePath;
            LVecBase3         rot    = parent.get_hpr();
            other.get_pos();
            parent.get_pos();
            LPoint3           tmp    = other.get_pos() - parent.get_pos();
            LPoint3           dir    = parent.get_relative_vector(other, tmp);

            PT(CollisionNode) cnode  = new CollisionNode("compileWaypointsNode");
            //cnode->set_into_collide_mask(ColMask::Object);
            cnode->set_from_collide_mask(CollideMask(ColMask::Object));
            np = (*it).nodePath.attach_new_node(cnode);

            PT(CollisionSegment) ctube  = new CollisionSegment(LPoint3(0, 0, 0),
                                                         dir);
            cnode->add_solid(ctube);

            np.set_scale(1 / parent.get_scale().get_x());

            PT(CollisionHandlerQueue) handlerQueue = new CollisionHandlerQueue();
            CollisionTraverser        traverser;

            traverser.add_collider(np, handlerQueue);

            traverser.traverse(window->get_render());
            handlerQueue->sort_entries();
            if ((handlerQueue->get_num_entries()) > 0)
              itArc = (*it).Disconnect((*itArc).to);
            else
              ++itArc;
            //np.show();
            np.remove_node();
        }
        progress_callback("Compiling Waypoints: ", (float)i / waypoints.size() * 100.f);
    }
}

void World::CompileDoors(ProgressCallback progress_callback)
{
  unsigned int        i   = 0;
  Waypoints::iterator it  = waypoints.begin();
  Waypoints::iterator end = waypoints.end();

  for (; it != end ; ++it, ++i)
  {
    Waypoint::Arcs::iterator itArc  = (*it).arcs.begin();
    Waypoint::Arcs::iterator endArc = (*it).arcs.end();

    while (itArc != endArc)
    {
      NodePath          np;
      NodePath          other  = (*itArc).to->nodePath;
      NodePath          parent = (*it).nodePath;
      LVecBase3         rot    = parent.get_hpr();
      PT(CollisionTube) ctube  = new CollisionTube(LPoint3(0, 0, 0),
                                                    parent.get_relative_vector(other, other.get_pos() - parent.get_pos()),
                                                    2.f);
      PT(CollisionNode) cnode  = new CollisionNode("compileWaypointsNode");
      cnode->set_into_collide_mask(CollideMask(ColMask::DynObject));
      cnode->set_from_collide_mask(CollideMask(ColMask::None));
      cnode->add_solid(ctube);

      np.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
      np.set_pos(0, 0, 0);
      np.set_scale(1 / parent.get_scale().get_x());

      np = (*it).nodePath.attach_new_node(cnode);

      PT(CollisionHandlerQueue) handlerQueue = new CollisionHandlerQueue();
      CollisionTraverser        traverser;

      traverser.add_collider(np, handlerQueue);
      traverser.traverse(window->get_render());
      for (int i = 0 ; i < handlerQueue->get_num_entries() ; ++i)
      {
        CollisionEntry* entry  = handlerQueue->get_entry(i);
        NodePath        into   = entry->get_into_node_path();
        DynamicObject*  object = GetDynamicObjectFromNodePath(into);

        if (object->type != DynamicObject::Character)
          object->lockedArcs.push_back(pair<int, int>((*it).id, (*itArc).to->id));
      }
      ++itArc;
      np.remove_node();
    }
    progress_callback("Compiling Doors: ", (float)i / waypoints.size() * 100.f);
  }
}
