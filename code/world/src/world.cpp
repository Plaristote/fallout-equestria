#include "world/world.h"
#include <panda3d/collisionHandlerQueue.h>
#include <panda3d/collisionBox.h>
#include <panda3d/collisionSphere.h>

using namespace std;

unsigned int          blob_revision = 6;

World::World(WindowFramework* window)
{
  this->window         = window;
  model_sphere         = window->load_model(window->get_panda_framework()->get_models(), std::string(MODEL_ROOT) + "misc/sphere.egg.pz");
  floors_node          = window->get_render().attach_new_node("floors");
  rootWaypoints        = window->get_render().attach_new_node("waypoints");
  rootMapObjects       = window->get_render().attach_new_node("mapobjects");
  rootDynamicObjects   = window->get_render().attach_new_node("dynamicobjects");
  rootLights           = window->get_render().attach_new_node("lights");
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
  for (size_t it = currentSize ; it < newSize ; ++it)
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

  object.strModel   = model;
  object.strTexture = texture;
  object.nodePath   = window->get_render().attach_new_node(name);
  object.render     = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + model2);
  object.render.reparent_to(object.nodePath);
  if (texture != "")
  {
    object.texture    = TexturePool::load_texture(TEXT_ROOT + texture);
    if (object.texture)
      object.render.set_texture(object.texture);
  }
  object.render.set_name(name);
  object.nodePath.set_pos(x, y, z);
  object.waypoints_root = object.nodePath.attach_new_node("waypoints");
  MapObjectChangeFloor(object, 0);
  {
    LPoint3f scale = NodePathSize(object.render) / 2;

    object.InitializeCollider(MapObject::MODEL, LPoint3f(), scale, LPoint3f());
    object.nodePath.set_collide_mask(CollideMask(ColMask::Object));
    object.collision_node.set_collide_mask(CollideMask(ColMask::FovBlocker));
  }
  objects.push_back(object);
  return (&(*(--(objects.end()))));
}

void World::ObjectChangeFloor(MapObject& object, unsigned char floor, unsigned short type)
{
  if (floor != object.floor)
  {
    if (floors.size() <= floor) FloorResize(floor + 1);
    object.nodePath.reparent_to(floors[floor].get_child(type));
    object.floor = floor;
  }
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

    if (!(object.collision_node.is_empty()) && object.collision_node.is_ancestor_of(path))
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
  object.waypoint = 0;
  object.nodePath = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + object.strModel);
  if (object.nodePath.is_empty())
  {
    std::cerr << "[World::InsertDynamicObject] Could not load " << object.strModel << std::endl;
    return (0);
  }
  if (object.strTexture != "")
  {
    object.texture    = TexturePool::load_texture(TEXT_ROOT + object.strTexture);
    if (object.texture)
      object.nodePath.set_texture(object.texture);
  }
  object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
  dynamicObjects.insert(dynamicObjects.begin(), object);
  return (&(*dynamicObjects.begin()));
}

DynamicObject* World::AddDynamicObject(const string &name, DynamicObject::Type type, const string &model, const string &texture)
{
  DynamicObject  object;
  DynamicObject* ptr;

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

// EXIT ZONES
void World::AddExitZone(const std::string& name)
{
  ExitZone exitZone;

  exitZone.name = name;
  exitZones.push_back(exitZone);
}

void World::DeleteExitZone(const std::string& name)
{
  ExitZones::iterator it = std::find(exitZones.begin(), exitZones.end(), name);

  if (it != exitZones.end())
    exitZones.erase(it);
}

ExitZone* World::GetExitZoneByName(const std::string& name)
{
  ExitZones::iterator it = std::find(exitZones.begin(), exitZones.end(), name);

  if (it != exitZones.end())
    return (&(*it));
  return (0);
}

bool      World::IsInExitZone(unsigned int id) const
{
  auto it = exitZones.begin(), end = exitZones.end();

  for (; it != end ; ++it)
  {
    const ExitZone& exit_zone = *it;

    if (exit_zone.Contains(id))
      return (true);
  }
  return (false);
}

// ENTRY ZONES
void World::AddEntryZone(const std::string& name)
{
  EntryZone entryZone;

  entryZone.name = name;
  entryZones.push_back(entryZone);
}

void World::DeleteEntryZone(const std::string& name)
{
  EntryZones::iterator it = std::find(entryZones.begin(), entryZones.end(), name);

  if (it != entryZones.end())
    entryZones.erase(it);
}

EntryZone* World::GetEntryZoneByName(const std::string& name)
{
  EntryZones::iterator it = std::find(entryZones.begin(), entryZones.end(), name);

  if (it != entryZones.end())
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

WorldLight* World::GetLightByName(const std::string& name)
{
  WorldLights::iterator it = std::find(lights.begin(), lights.end(), name);

  cout << "Getting light by name... light count: " << lights.size() << endl;
  if (it != lights.end())
    return (&(*it));
  return (0);
}

void        World::CompileLight(WorldLight* light, unsigned char colmask)
{
  PT(CollisionSphere)       colSphere = new CollisionSphere(light->nodePath.get_x(),
                               light->nodePath.get_y(),
                               light->nodePath.get_z(),
                               light->zoneSize);
  PT(CollisionNode)         colNode       = new_CollisionNode("compileLightSphere");
  NodePath                  colNp         = rootLights.attach_new_node(colNode);
  PT(CollisionHandlerQueue) handlerQueue  = new CollisionHandlerQueue();
  CollisionTraverser        traverser;
  list<NodePath>::iterator  it            = light->enlightened.begin();
  list<NodePath>::iterator  end           = light->enlightened.end();

  colNode->add_solid(colSphere);
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
  traverser.traverse(floors_node);
  for (unsigned short i = 0 ; i < handlerQueue->get_num_entries() ; ++i)
  {
    NodePath        node  = handlerQueue->get_entry(i)->get_into_node_path();

    if (node.is_empty())
      continue ;

    Waypoint*      waypoint  = (colmask & ColMask::Waypoint ? GetWaypointFromNodePath(node) : 0);
    MapObject*     object    = (!waypoint && (colmask & ColMask::Object) ? GetMapObjectFromNodePath(node) : 0);
    DynamicObject* dynObject = (object ? 0 : GetDynamicObjectFromNodePath(node));

    if (waypoint)
      waypoint->lights.push_back(light);
    else if (object || dynObject)
    {
      list<NodePath>::iterator alreadyRegistered;

      node = (object ? object->nodePath : dynObject->nodePath);
      alreadyRegistered = find(light->enlightened.begin(), light->enlightened.end(), node);
      if (alreadyRegistered == light->enlightened.end())
      {
        light->enlightened.push_back(node);
        node.set_light(light->nodePath, 7);
      }
    }
  }

  //cout << "Number of enlightened objects -> " << light->enlightened.size() << endl;

  colNp.detach_node();
}

void World::DynamicObjectSetWaypoint(DynamicObject& object, Waypoint& waypoint)
{
  object.waypoint = &waypoint;
  if (object.waypoint->floor != waypoint.floor)
  {
    if (floors.size() > waypoint.floor)
      object.nodePath.set_alpha_scale(floors[waypoint.floor].get_color_scale().get_w());
    DynamicObjectChangeFloor(object, waypoint.floor);
  }
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
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      MapObject     object;
      unsigned char floor;

      object.UnSerialize(this, packet);
      floor        = object.floor;
      object.floor = (floor == 0 ? 1 : 0); // This has to be done, or MapObjectChangeFloor won't execute
      MapObjectChangeFloor(object, floor);
      {
        int flag     = ColMask::Object;
        int col_flag = ColMask::FovBlocker;

        if (object.waypoints.size() > 0)
          flag     |= ColMask::WpPlane;
        if (object.collider == MapObject::MODEL)
          col_flag |= ColMask::CheckCollisionOnModel;
        object.nodePath.set_collide_mask(CollideMask(flag));
        object.collision_node.set_collide_mask(CollideMask(col_flag));
      }
      objects.push_back(object);
    }
  }

    cout << "Unserialize dynamic objects" << endl;
  // DynamicObjects
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      DynamicObject object;
      unsigned char floor;

      object.UnSerialize(this, packet);
      floor        = object.floor;
      object.floor = (floor == 0 ? 1 : 0);
      DynamicObjectChangeFloor(object, floor);
      object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
      dynamicObjects.push_back(object);
    }
  }

    cout << "Unserialize lights" << endl;
  // Lights
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      WorldLight light(window->get_render());

      light.UnSerialize(this, packet);
      lights.push_back(light);
    }
  }

    cout << "Unserialize zones" << endl;
  // ExitZones
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      ExitZone       zone;
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
      exitZones.push_back(zone);
    }
  }

  // EntryZone
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      EntryZone      zone;
      std::list<int> waypointsId;

      packet >> zone.name;
      packet >> waypointsId;
      std::list<int>::iterator begin = waypointsId.begin();
      std::list<int>::iterator end   = waypointsId.end();

      for (; begin != end ; ++begin)
      {
        Waypoint* wp = GetWaypointFromId(*begin);

        if (wp)
          zone.waypoints.push_back(wp);
      }
      entryZones.push_back(zone);
    }
  }

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

    cout << "Compiling lights" << endl;
  // Post-loading stuff
#ifndef GAME_EDITOR
  for_each(lights.begin(), lights.end(), [this](WorldLight& light) { CompileLight(&light, ColMask::Object | ColMask::Waypoint | ColMask::DynObject); });
#else
  for_each(lights.begin(), lights.end(), [this](WorldLight& light) { CompileLight(&light, ColMask::Object | ColMask::DynObject); });
#endif
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
# endif

  packet << (unsigned int)6; // #blob revision

  // Waypoints
  {
    Waypoints::iterator it  = waypoints.begin();
    Waypoints::iterator end = waypoints.end();
    unsigned int        id  = 0;
    int                 size;

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

  // MapObjects
  {
    MapObjects::iterator it  = objects.begin();
    MapObjects::iterator end = objects.end();

    while (it != end)
    {
      if (it->nodePath.is_empty())
        it = objects.erase(it);
      else
        ++it;
    }
    packet << ((int)objects.size());
    for (it = objects.begin() ; it != end ; ++it) { (*it).Serialize(packet); }
  }
#ifdef GAME_EDITOR
  if (do_compile_doors)
    CompileDoors(progress_callback);
#endif

  // DynamicObjects
  {
    DynamicObjects::iterator it  = dynamicObjects.begin();
    DynamicObjects::iterator end = dynamicObjects.end();

    while (it != end)
    {
      if (it->nodePath.is_empty())
        it = dynamicObjects.erase(it);
      else
        ++it;
    }
    packet << ((int)dynamicObjects.size());
    for (it = dynamicObjects.begin() ; it != end ; ++it) { (*it).Serialize(packet); }
  }

  // WorldLights
  {
    int size = lights.size();

    packet << size;
    WorldLights::iterator it  = lights.begin();
    WorldLights::iterator end = lights.end();

    for (; it != end ; ++it) { (*it).Serialize(packet); }
  }

  // ExitZones
  {
    ExitZones::iterator it   = exitZones.begin();
    ExitZones::iterator end  = exitZones.end();
    int                 size = exitZones.size();

    packet << size;
    for (; it != end ; ++it)
    {
      ExitZone&                      zone = *it;
      std::list<int>                 waypointsId;
      std::list<Waypoint*>::iterator wpIt  = zone.waypoints.begin();
      std::list<Waypoint*>::iterator wpEnd = zone.waypoints.end();

      for (; wpIt != wpEnd ; ++wpIt)
        waypointsId.push_back((*wpIt)->id);
      packet << zone.name;
      packet << zone.destinations;
      packet << waypointsId;
    }
  }

  // EntryZone
  {
    EntryZones::iterator it   = entryZones.begin();
    EntryZones::iterator end  = entryZones.end();
    int                  size = entryZones.size();

    packet << size;
    for (; it != end ; ++it)
    {
      EntryZone&                     zone  = *it;
      std::list<int>                 waypointsId;
      std::list<Waypoint*>::iterator wpIt  = zone.waypoints.begin();
      std::list<Waypoint*>::iterator wpEnd = zone.waypoints.end();

      for (; wpIt != wpEnd ; ++wpIt)
        waypointsId.push_back((*wpIt)->id);
      packet << zone.name;
      packet << waypointsId;
    }
  }

  {
    char serialize_sunlight_enabled = sunlight_enabled;

    packet << serialize_sunlight_enabled;
  }
}

// MAP COMPILING
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

            PT(CollisionNode) cnode  = new_CollisionNode("compileWaypointsNode");
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
      PT(CollisionNode) cnode  = new_CollisionNode("compileWaypointsNode");
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
