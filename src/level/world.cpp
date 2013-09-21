#include "is_game_editor.h"
#ifdef GAME_EDITOR
# include "world.h"
#else
# include "level/world.h"
#include <dices.hpp>
#endif
#include <panda3d/collisionHandlerQueue.h>

using namespace std;

unsigned int          blob_revision = 3;

unsigned char         gPathfindingUnitType = 0;
void*                 gPathfindingData     = 0;

NodePath World::model_sphere;
NodePath debug_pathfinding;

LPoint3 NodePathSize(NodePath np)
{
  LPoint3 min_point, max_point;
  LPoint3 ret;

  np.calc_tight_bounds(min_point, max_point);
  ret.set_x(max_point.get_x() - min_point.get_x());
  ret.set_y(max_point.get_y() - min_point.get_y());
  ret.set_z(max_point.get_z() - min_point.get_z());
  return (ret);
}

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
  NodePath nodePath = rootWaypoints.attach_new_node("waypoint");
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

LPlane World::GetWaypointPlane(short currentFloor) const
{
  LPoint3 upperRight(0, 0, 0), upperLeft(0, 0, 0), bottomLeft(0, 0, 0);

  GetWaypointLimits(currentFloor, upperRight, upperLeft, bottomLeft);
  return (LPlane(upperRight, upperLeft, bottomLeft));
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
  if (waypoints.size() > id && id > 0)
  {
    Waypoint& wp = waypoints[id - 1];
    
    if (wp.id != id)
      cout << "/!\\ RETARD ALERT ! Wrong waypoint (" << wp.id << ", looking for " << id << ')' << endl;
    return (&waypoints[id - 1]);
  }
  cout  << "Wp size -> " << waypoints.size() << ", looking for " << id << endl;
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
  int currentSize = floors.size();

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

  object.strModel   = model;
  object.strTexture = texture;
  object.nodePath   = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + model2);
  if (texture != "")
  {
    object.texture    = TexturePool::load_texture(TEXT_ROOT + texture);
    if (object.texture)
      object.nodePath.set_texture(object.texture);
  }

  object.nodePath.set_name(name);
  object.nodePath.set_pos(x, y, z);
  object.waypoints_root = object.nodePath.attach_new_node("waypoints");

  MapObjectChangeFloor(object, 0);

  object.nodePath.set_collide_mask(CollideMask(ColMask::Object));
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
          it = objects.erase(it);
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
  PT(CollisionNode)         colNode       = new CollisionNode("compileLightSphere");
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

void WorldLight::ReparentTo(World* world)
{
  parent_type = Type_None;
  parent_i    = 0;
  nodePath.reparent_to(world->rootLights);
#ifdef GAME_EDITOR
  //symbol.reparent_to(world->lightSymbols);
  nodePath.reparent_to(world->rootLights);
#endif
}

void WorldLight::SetEnabled(bool set_enabled)
{
  std::function<void (NodePath)> set_light;
  std::function<void (NodePath)> unset_light;

  enabled       = set_enabled;
  set_light     = [this](NodePath object) { object.set_light(nodePath); };
  unset_light   = [this](NodePath object) { object.set_light_off(nodePath); object.clear_light(nodePath); };
  for_each(enlightened.begin(), enlightened.end(), enabled ? set_light : unset_light);
}

void WorldLight::Destroy(void)
{
  SetEnabled(false);
  nodePath.detach_node();
#ifdef GAME_EDITOR
  symbol.detach_node();
#endif
}

// WAYPOINTS

Waypoint::Waypoint(NodePath root)
{
  selected = false;
  nodePath = root;
  //nodePath.set_collide_mask(CollideMask(ColMask::Waypoint));
  nodePath.set_scale(2.f);
  nodePath.show();

  nodePath.set_transparency(TransparencyAttrib::M_alpha);
  nodePath.set_color(0, 0, 0, 0.5);

  floor    = 0;
}

void                Waypoint::SetSelected(bool selected)
{
  this->selected = selected;
  if (selected)
    nodePath.set_color(0, 1.0, 0, 0.5);
  else
    nodePath.set_color(0, 0, 0, 0.5);
  SetArcsVisible(selected);
}

bool                Waypoint::operator==(const Waypoint& other) const
{
  return (nodePath == other.nodePath);
}

bool                Waypoint::operator==(const Waypoint* other) const { return (*this == *other); }

Waypoint::Arcs::iterator Waypoint::ConnectUnsafe(Waypoint* other)
{
  arcs.push_back(Arc(nodePath, other));
  return (--(arcs.end()));
}

Waypoint::Arcs::iterator Waypoint::Connect(Waypoint* other)
{
  Arcs::iterator  it = find(arcs.begin(), arcs.end(), other);

  if (it == arcs.end())
  {
    arcs.push_back(Arc(nodePath, other));
    other->arcs.push_back(Arc(other->nodePath, this));
    return (--(arcs.end()));
  }
  return (it);
}

Waypoint::Arcs::iterator Waypoint::Disconnect(Waypoint* other)
{
  Arcs::iterator itSelf  = find(arcs.begin(), arcs.end(), other);
  Arcs::iterator itOther = find(other->arcs.begin(), other->arcs.end(), this);

  if (itSelf  != arcs.end())
  {
    itSelf->Destroy();
    itSelf = arcs.erase(itSelf);
  }
  if (itOther != other->arcs.end())
  {
    itOther->Destroy();
    other->arcs.erase(itOther);
  }
  return (itSelf);
}

void                Waypoint::DisconnectAll(void)
{
  Arcs::iterator it = arcs.begin();

  while (it != arcs.end())
  {
    Disconnect(it->to);
    it = arcs.begin();
  }
}

Waypoint::Arc*      Waypoint::GetArcTo(unsigned int id)
{
  Arcs::iterator it = arcs.begin();

  while (it != arcs.end())
  {
    if ((*it).to->id == id)
      return (&(*it));
    ++it;
  }
  return (0);
}

float               Waypoint::GetDistanceEstimate(const Waypoint& other) const
{
  LPoint3 pos_1  = nodePath.get_pos();
  LPoint3 pos_2  = other.nodePath.get_pos();
  float   dist_x = pos_1.get_x() - pos_2.get_x();
  float   dist_y = pos_1.get_y() - pos_2.get_y();

  return (SQRT(dist_x * dist_x + dist_y * dist_y));
}

list<Waypoint*> Waypoint::GetSuccessors(Waypoint* parent)
{
  list<Waypoint*> successors;

  Arcs::iterator it  = arcs.begin();
  Arcs::iterator end = arcs.end();

  for (; it != end ; ++it)
  {
      Arc& arc = *it;

      if (parent == arc.to)
        continue ;
      if (arc.observer && arc.observer->CanGoThrough(gPathfindingUnitType) == false)
        continue ;
      successors.push_back(arc.to);
  }
  return (successors);
}

void Waypoint::PositionChanged()
{
  Arcs::iterator it  = arcs.begin();
  Arcs::iterator end = arcs.end();

  for (; it != end ; ++it)
  {
      (*it).UpdateDirection();
      (*it).to->UpdateArcDirection(this);
  }
}

void Waypoint::UpdateArcDirection(Waypoint* to)
{
  Arcs::iterator it = find(arcs.begin(), arcs.end(), to);

  if (it != arcs.end())
    (*it).UpdateDirection();
}

#ifdef GAME_EDITOR
# define WAYPOINT_DEBUG
#endif

// WAYPOINTS ARCS
Waypoint::Arc::Arc(NodePath from, Waypoint* to) : from(from), to(to)
{
  observer = 0;
}

Waypoint::Arc::Arc(const Waypoint::Arc& arc) : from(arc.from), to(arc.to)
{
  csegment = arc.csegment;
  observer = arc.observer;
}

Waypoint::Arc::~Arc()
{
#ifdef WAYPOINT_DEBUG
  if (!(nodePath.is_empty()))
    nodePath.detach_node();
#endif
}

void Waypoint::Arc::SetVisible(bool set)
{
#ifdef WAYPOINT_DEBUG
  if (set && nodePath.is_empty())
  {
    csegment = new CollisionSegment();
    node     = new CollisionNode("waypointArc");
    node->set_into_collide_mask(CollideMask(0));
    node->set_from_collide_mask(CollideMask(0));
    node->add_solid(csegment);
    csegment->set_point_a(0, 0, 0);
    nodePath = from.attach_new_node(node);
    nodePath.set_pos(0, 0, 0);
    UpdateDirection();
    nodePath.show();
  }
  else
  {
    nodePath.remove_node();
  }
#endif
}

void Waypoint::Arc::UpdateDirection(void)
{
  if (!(nodePath.is_empty()))
  {
    NodePath  other  = to->nodePath;
    NodePath  parent = nodePath.get_parent();
    LVecBase3 rot    = parent.get_hpr();
    LVector3  dir    = parent.get_relative_vector(other, other.get_pos() - parent.get_pos());

    nodePath.set_scale(1 / parent.get_scale().get_x());
    nodePath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
    csegment->set_point_b(dir);
  }
}

void Waypoint::Arc::Destroy(void)
{
#ifdef WAYPOINT_DEBUG
  if (!(nodePath.is_empty()))
    nodePath.detach_node();
#endif
}

void Waypoint::SetArcsVisible(bool set)
{
  for_each(arcs.begin(), arcs.end(), [set](Waypoint::Arc& arc)
  {
    arc.SetVisible(set);
  });
}

void Waypoint::WithdrawArc(Waypoint* other)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to->id == other->id)
    {
      if (GetArcTo(arc.to->id))
        Disconnect(arc.to);
      (*it).second++;
      break ;
    }
  }
}

void Waypoint::UnwithdrawArc(Waypoint* other, ArcObserver* observer)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to == other)
    {
      if ((*it).second != 0)
        (*it).second--;
      if ((*it).second == 0)
      {
        std::pair<Waypoint::Arc, unsigned short>* withdrawable = arc.to->GetWithdrawable(this);

        if (!withdrawable)
          break ;
        if (withdrawable && withdrawable->second == 0)
        {
          std::list<Waypoint::Arc>::iterator it = Connect(arc.to);
        
          it->observer = observer;
        }
      }
      break ;
    }
  }
}

std::pair<Waypoint::Arc, unsigned short>* Waypoint::GetWithdrawable(Waypoint* other)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to->id == other->id)
      return (&(*it));
  }
  return (0);
}

Waypoint* Waypoint::GetRandomWaypoint(void) const
{
  if (arcs_withdrawed.size() > 0)
  {
    ArcsWithdrawed::const_iterator it  = arcs_withdrawed.begin();
    unsigned int                   i   = Dices::Throw(arcs_withdrawed.size() - 1);

    std::advance(it, i);
    return ((*it).first.to);
  }
  return (0);
}

/* MySqrt */
float my_sqrt(const float x)
{
  union { int a; float x; } u;

  u.x = x;
  u.a = (1 << 29) + (u.a >> 1) - (1 << 22);
  u.x = (u.x + (x / u.x));
  return (u.x * 0.5f);
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

// SERIALIZATION
void Waypoint::Unserialize(Utils::Packet &packet)
{
  float            posx, posy, posz;

  packet >> (int&)(id);
  packet >> posx >> posy >> posz;
  packet >> floor;
  packet >> tmpArcs;

  nodePath.set_pos(posx, posy, posz);
}

void Waypoint::LoadArcs(void)
{
  std::for_each(arcs.begin(), arcs.end(), [this](Arc& arc)
  {
    arcs_withdrawed.push_back(std::pair<Arc, unsigned short>(arc, 0));
  });
}

void Waypoint::UnserializeLoadArcs(World* world)
{
  vector<int>::iterator it  = tmpArcs.begin();
  vector<int>::iterator end = tmpArcs.end();

  for (; it != end ; ++it)
  {
    Waypoint* waypoint = world->GetWaypointFromId((*it));

    if (waypoint)
      Connect(waypoint);
  }
  LoadArcs();
  tmpArcs.clear();
}

void Waypoint::Serialize(Utils::Packet &packet)
{
  int              id = this->id;
  float            posx, posy, posz;
  vector<int>      arcs;

  posx = nodePath.get_x();
  posy = nodePath.get_y();
  posz = nodePath.get_z();

  Arcs::iterator it  = this->arcs.begin();
  Arcs::iterator end = this->arcs.end();

  for (; it != end ; ++it)
    arcs.push_back((*it).to->id);

  packet << id;
  packet << posx << posy << posz;
  packet << floor;
  packet << arcs;
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

void MapObject::UnSerialize(World* world, Utils::Packet& packet)
{
  string name;
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;

  packet >> name >> strModel >> strTexture;
  packet >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaleX >> scaleY >> scaleZ;
  packet >> floor;
  if (blob_revision >= 1)
    packet >> parent;
  if (blob_revision >= 2)
    UnserializeWaypoints(world, packet);

  nodePath   = world->window->load_model(world->window->get_panda_framework()->get_models(), MODEL_ROOT + strModel);
  if (nodePath.is_empty())
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
        nodePath.set_texture(texture);
    }
    nodePath.set_name(name);
    nodePath.set_hpr(rotX, rotY, rotZ);
    nodePath.set_scale(scaleX, scaleY, scaleZ);
    nodePath.set_pos(posX, posY, posZ);
    waypoints_root = nodePath.attach_new_node("waypoints");
  }
}

void MapObject::UnserializeWaypoints(World* world, Utils::Packet& packet)
{
  vector<int> waypoint_ids;

  packet >> waypoint_ids;
  for_each(waypoint_ids.begin(), waypoint_ids.end(), [this, world](int id)
  {
    Waypoint* wp = world->GetWaypointFromId(id);

    if (wp)
    {
      waypoints.push_back(wp);
#ifdef GAME_EDITOR
      if (!(wp->nodePath.is_empty()))
        wp->nodePath.reparent_to(waypoints_root);
#endif
    }
  });
}

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

void MapObject::Serialize(Utils::Packet& packet)
{
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;
  string name;
  
  name   = nodePath.get_name();
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
}

void DynamicObject::UnSerialize(World* world, Utils::Packet& packet)
{
  int  iType;
  char iLocked;
  int  iWaypoint;

  MapObject::UnSerialize(world, packet);
  cout << "ma bite #1" << endl;
  packet >> iType >> interactions;
  cout << "ma bite #2" << endl;
  type = (Type)iType;

  if      (type == Character)
    packet >> script >> charsheet >> dialog;
  else if (type == Door || type == Locker)
    packet >> iLocked >> key;
  else if (type == Item)
    packet >> key;
  cout << "ma bite #3" << endl;
  locked = iLocked != 0;

  packet >> iWaypoint;
  cout << "ma bite #4" << endl;
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
  cout << "ma bite #5" << endl;

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
  cout << "ma bite #6" << endl;
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

/*
 * WorldLights
 */
void WorldLight::Initialize(void)
{
  switch (type)
  {
    default:
    case Point:
    {
      PT(PointLight) pLight = new PointLight(name);

      pLight->set_shadow_caster(true, 12, 12);
      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Directional:
    {
      PT(DirectionalLight) pLight = new DirectionalLight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Ambient:
    {
      PT(AmbientLight) pLight = new AmbientLight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Spot:
    {
      PT(Spotlight) pLight = new Spotlight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
  }
#ifdef GAME_EDITOR
  if (!(World::model_sphere.is_empty()))
    World::model_sphere.instance_to(symbol);
  else
    cout << "The horror ! Model spehre is unavailable" << endl;
#endif
}

void WorldLight::UnSerialize(World* world, Utils::Packet& packet)
{
  float     r, g, b, a;
  float     pos_x, pos_y, pos_z;
  float     hpr_x, hpr_y, hpr_z;
  string    parent_name;
  char      tmp_enabled, _type, _ptype;

  packet >> name >> tmp_enabled >> zoneSize;
  packet >> _type >> _ptype;
  cout << "[World] Loading light " << name << endl;
  enabled     = tmp_enabled != 0;
  type        = (WorldLight::Type)_type;
  parent_type = (WorldLight::ParentType)_ptype;
  if (parent_type != Type_None)
    packet >> parent_name;
  packet >> r >> g >> b >> a;
  packet >> pos_x >> pos_y >> pos_z;
  packet >> hpr_x >> hpr_y >> hpr_z;
  switch (parent_type)
  {
    case Type_MapObject:
      ReparentTo(world->GetMapObjectFromName(parent_name));
      break ;
    case Type_DynamicObject:
      ReparentTo(world->GetDynamicObjectFromName(parent_name));
      break ;
    case Type_None:
      parent   = world->rootLights;
      parent_i = 0;
      break ;
  }
  cout << "Light type = " << (int)type << endl;
  Initialize();
  if (blob_revision >= 3)
  {
    float     attenuation[3];

    packet >> attenuation[0] >> attenuation[1] >> attenuation[2];
    SetAttenuation(attenuation[0], attenuation[1], attenuation[2]);
  }
  SetColor(r, g, b, a);
  if (!(nodePath.is_empty()))
  {
    nodePath.set_pos(LVecBase3(pos_x, pos_y, pos_z));
    nodePath.set_hpr(LVecBase3(hpr_x, hpr_y, hpr_z));
  }
#ifdef GAME_EDITOR
  if (!(symbol.is_empty()))
    symbol.reparent_to(world->lightSymbols);
#endif
}

void WorldLight::Serialize(Utils::Packet& packet)
{
  LColor     color  = light->get_color();
  char       _type  = type;
  char       _ptype = parent_type;
  LVecBase3f attenuation = GetAttenuation();

  packet << name << (char)enabled << zoneSize << _type << _ptype;
  if (parent_i)
    packet << parent_i->nodePath.get_name();
  packet << (float)color.get_x() << (float)color.get_y() << (float)color.get_z() << (float)color.get_w();
  packet << (float)nodePath.get_x() << (float)nodePath.get_y() << (float)nodePath.get_z();
  packet << (float)nodePath.get_hpr().get_x() << (float)nodePath.get_hpr().get_y() << (float)nodePath.get_hpr().get_z();
  packet << (float)attenuation.get_x() << (float)attenuation.get_y() << (float)attenuation.get_z();
}

/*
 * World
 */
void           World::UnSerialize(Utils::Packet& packet)
{
  cout << "Blob revision was " << blob_revision << endl;
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
      //waypoint.nodePath.remove_node();
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
      object.nodePath.set_collide_mask(CollideMask(ColMask::Object));
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
          std::cout << "Solving for: '" << solving_for << "'. Current item: '" << it->nodePath.get_name() << '\'' << std::endl;
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

#ifndef GAME_EDITOR
  // Setting waypoint positions
  {
    std::for_each(objects.begin(), objects.end(), [this](MapObject& object)
    {
      auto it  = object.waypoints.begin();
      auto end = object.waypoints.end();

      for (; it != end ; ++it)
      {
        Waypoint* wp = *it;

        wp->nodePath.set_pos(wp->nodePath.get_pos() + object.nodePath.get_pos(window->get_render()));
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

#ifndef GAME_EDITOR
void           World::Serialize(Utils::Packet& packet)
#else
void           World::Serialize(Utils::Packet& packet, std::function<void (const std::string&, float)> progress_callback)
#endif
{
  // Compile Step
# ifdef GAME_EDITOR
  if (do_compile_waypoints)
    CompileWaypoints(progress_callback);
# endif

  packet << (unsigned int)3; // #blob revision

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
#ifdef GAME_EDITOR
      progress_callback("Serializing Waypoints: ", (float)id / waypoints.size() * 100.f);
#endif
    }
    size = waypoints.size();
    packet << size;
    for (it = waypoints.begin() ; it != end ; ++it)
  (*it).Serialize(packet);
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
# ifdef GAME_EDITOR
  if (do_compile_doors)
    CompileDoors(progress_callback);
# endif

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

            PT(CollisionNode) cnode  = new CollisionNode("compileWaypointsNode");
            //cnode->set_into_collide_mask(ColMask::Object);
            cnode->set_from_collide_mask(CollideMask(ColMask::Object));
            np = (*it).nodePath.attach_new_node(cnode);

            PT(CollisionSegment) ctube  = new CollisionSegment(LPoint3(0, 0, 0),
                                                         dir/*,
                                                         2.f*/);
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
