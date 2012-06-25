#include "world.h"
#ifndef GAME_EDITOR
# include "globals.hpp"
#else
# define ABS(x) (x > 0 ? x : -x)
#endif

using namespace std;

unsigned char         gPathfindingUnitType = 0;
void*                 gPathfindingData     = 0;

World::World(WindowFramework* window)
{
    this->window       = window;
    rootWaypoints      = window->get_render().attach_new_node("waypoints");
    rootMapObjects     = window->get_render().attach_new_node("mapobjects");
    rootDynamicObjects = window->get_render().attach_new_node("dynamicobjects");
}

World::~World()
{
  ForEach(waypoints,      [](Waypoint& wp)      { wp.nodePath.remove_node(); });
  ForEach(objects,        [](MapObject& mo)     { mo.nodePath.remove_node(); });
  ForEach(dynamicObjects, [](DynamicObject& dy) { dy.nodePath.remove_node(); });
}

Waypoint* World::AddWayPoint(float x, float y, float z)
{
  NodePath nodePath = window->load_model(window->get_panda_framework()->get_models(), "misc/sphere");
  Waypoint  waypoint(nodePath);
  Waypoint* ptr;
  
  waypoint.nodePath.set_pos(x, y, z);
  waypoints.push_back(waypoint);
  nodePath.reparent_to(rootWaypoints);
  ptr = &(*(--(waypoints.end())));
  return (ptr);
}

void World::DeleteWayPoint(Waypoint* toDel)
{
    Waypoints::iterator it = find(waypoints.begin(), waypoints.end(), toDel);

    if (it != waypoints.end())
    {
        toDel->DisconnectAll();
        toDel->nodePath.remove_node();
        waypoints.erase(it);
    }
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

Waypoint* World::GetWaypointFromId(unsigned int id)
{
    Waypoints::iterator it  = find(waypoints.begin(), waypoints.end(), id);

    if (it != waypoints.end())
      return ((&(*it)));
    return (0);
}

MapObject* World::AddMapObject(const string &name, const string &model, const string &texture, float x, float y, float z)
{
    MapObject object;
    string model2 = model;

    /*if (model2 == "lpip.egg")
      model2 = "horse.obj";*/
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
    object.nodePath.reparent_to(rootMapObjects);
    object.nodePath.set_collide_mask(CollideMask(ColMask::Object));
    objects.push_back(object);
    return (&(*(--(objects.end()))));
}

void World::DeleteMapObject(MapObject* ptr)
{
    DeleteObject(ptr, objects);
}

MapObject* World::GetMapObjectFromName(const string &name)
{
  return (GetObjectFromName(name, objects));
}

MapObject* World::GetMapObjectFromNodePath(NodePath path)
{
  return (GetObjectFromNodePath(path, objects));
}

// DYNAMIC OBJECTS
DynamicObject* World::AddDynamicObject(const string &name, DynamicObject::Type type, const string &model, const string &texture)
{
  DynamicObject object;

  object.type         = type;
  object.interactions = 0;
  object.strModel     = model;
  object.strTexture   = texture;
  object.nodePath     = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + model);
  if (texture != "")
  {
    object.texture    = TexturePool::load_texture(TEXT_ROOT + texture);
    if (object.texture)
      object.nodePath.set_texture(object.texture);
  }

  object.nodePath.set_name(name);
  object.nodePath.reparent_to(rootDynamicObjects);
  object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
  dynamicObjects.push_back(object);
  return (&(*(--(dynamicObjects.end()))));
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

Waypoint*      World::GetWaypointAt(LPoint2f point)
{
  Waypoint*    nearest =  0;
  float        min     = -1;

  Waypoints::iterator it  = waypoints.begin();
  Waypoints::iterator end = waypoints.end();

  for (; it != end ; ++it)
  {
      Waypoint& waypoint = *it;

      if ((waypoint.mouseBox.Intersects(point.get_x(), point.get_y())))
      {
        float x        = waypoint.mouseBox.left + waypoint.mouseBox.width  / 2;
        float y        = waypoint.mouseBox.top  + waypoint.mouseBox.height / 2;
        float distance;

        x        = ABS(x - point.get_x());
        y        = ABS(y - point.get_y());
        distance = (x < y ? x : y);
        if (min == -1 || distance < min)
        {
          min     = distance;
          nearest = &waypoint;
        }
      }
  }
  return (nearest);
}

// EXIT ZONES
void World::AddExitZone(const std::string& name)
{
  ExitZone exitZone;
  
  exitZone.name = name;
  exitZones.push_back(exitZone);
}

ExitZone* World::GetExitZoneByName(const std::string& name)
{
  ExitZones::iterator it = std::find(exitZones.begin(), exitZones.end(), name);
  
  if (it != exitZones.end())
    return (&(*it));
  return (0);
}

// WAYPOINTS

Waypoint::Waypoint(NodePath root)
{
    nodePath = root;
    nodePath.set_collide_mask(CollideMask(ColMask::Waypoint));
    nodePath.set_scale(2.f);
    nodePath.show();

    nodePath.set_transparency(TransparencyAttrib::M_alpha);
    nodePath.set_color(0, 0, 0, 0.5);
}

void                Waypoint::SetSelected(bool selected)
{
  if (selected)
    nodePath.set_color(0, 1.0, 0, 0.5);
  else
    nodePath.set_color(0, 0, 0, 0.5);
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

void Waypoint::SetMouseBox(void)
{
    Arcs::iterator it    = arcs.begin();
    Arcs::iterator end   = arcs.end();
    float          max_x = 0;
    float          max_y = 0;

    LVector3f      pos_a  = nodePath.get_pos();
    LVector3f      pos_b;

    for (; it != end ; ++it)
    {
      float dist_x, dist_y;

      pos_b  = (*it).to->nodePath.get_pos();
      dist_x = ABS(pos_a.get_x() - pos_b.get_x());
      dist_y = ABS(pos_a.get_y() - pos_b.get_y());
      if (dist_x > max_x) max_x = dist_x;
      if (dist_y > max_y) max_y = dist_y;
    }
    mouseBox.left   = pos_a.get_x() - max_x;
    mouseBox.width  = max_x * 2;
    mouseBox.top    = pos_a.get_y() - max_y;
    mouseBox.height = max_y * 2;
}

// WAYPOINTS ARCS
Waypoint::Arc::Arc(NodePath from, Waypoint* to) : to(to)
{
  observer = 0;
#ifdef WAYPOINT_DEBUG
  csegment = new CollisionSegment();
  node     = new CollisionNode("waypointArc");
  node->set_into_collide_mask(CollideMask(0));
  node->set_from_collide_mask(CollideMask(0));
  node->add_solid(csegment);
  csegment->set_point_a(0, 0, 0);
  nodePath = from.attach_new_node(node);
  nodePath.set_pos(0, 0, 0);
  nodePath.show();
  UpdateDirection();
#endif
}

void Waypoint::Arc::UpdateDirection(void)
{
    NodePath  other  = to->nodePath;
    NodePath  parent = nodePath.get_parent();
    LVecBase3 rot    = parent.get_hpr();
    LVector3  dir    = parent.get_relative_vector(other, other.get_pos() - parent.get_pos());

    nodePath.set_scale(1 / parent.get_scale().get_x());
    nodePath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
    csegment->set_point_b(dir);
}

void Waypoint::Arc::Destroy(void)
{
#ifdef WAYPOINT_DEBUG
    nodePath.remove_node();
#endif
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

// SERIALIZATION
void Waypoint::Unserialize(Utils::Packet &packet)
{
    float            posx, posy, posz;

    packet >> (int&)(id);
    packet >> posx >> posy >> posz;
    packet >> tmpArcs;

    nodePath.set_pos(posx, posy, posz);
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
  packet << arcs;
}

void MapObject::UnSerialize(WindowFramework* window, Utils::Packet& packet)
{
  string name;
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;

  packet >> name >> strModel >> strTexture;
  packet >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaleX >> scaleY >> scaleZ;

  nodePath   = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + strModel);
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
}

void MapObject::Serialize(Utils::Packet& packet)
{
  float  posX,   posY,   posZ;
  float  rotX,   rotY,   rotZ;
  float  scaleX, scaleY, scaleZ;
  string name = nodePath.get_name();

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
}

void DynamicObject::UnSerialize(World* world, Utils::Packet& packet)
{
    int  iType;
    char iLocked;
    int  iWaypoint;

    MapObject::UnSerialize(world->window, packet);
    packet >> iType >> interactions;
    type = (Type)iType;

    if (type == Character)
      packet >> script >> charsheet >> dialog;
    if (type == Door || type == Locker)
      packet >> iLocked >> key;
    locked = iLocked;

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
	    lockedArcs.push_back(std::pair<int, int>(id1, id2));
        }
    }

    // Add inventory serialization
}

void DynamicObject::Serialize(Utils::Packet& packet)
{
    MapObject::Serialize(packet);

    int  iType     = (int)type;
    char iLocked   = locked;
    int  iWaypoint = 0;

    packet << iType << interactions;

    if (type == Character)
      packet << script << charsheet << dialog;
    if (type == Door || type == Locker)
      packet << iLocked << key;

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

    // Add inventory serialization
}

void           World::UnSerialize(Utils::Packet& packet)
{
  // Waypoints
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      NodePath sphere = window->load_model(window->get_panda_framework()->get_models(), "misc/sphere");
      Waypoint waypoint(sphere);

      waypoint.Unserialize(packet);
      sphere.reparent_to(rootWaypoints);
      waypoints.push_back(waypoint);
    }

    Waypoints::iterator it, end;

    for (it = waypoints.begin(), end = waypoints.end() ; it != end ; ++it)
    {
      (*it).UnserializeLoadArcs(this);
      (*it).SetMouseBox();
    }
  }

  // MapObjects
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      MapObject object;

      object.UnSerialize(window, packet);
      object.nodePath.reparent_to(rootMapObjects);
      object.nodePath.set_collide_mask(CollideMask(ColMask::Object));
      objects.push_back(object);
    }
  }

  // DynamicObjects
  {
    int size;

    packet >> size;
    for (int it = 0 ; it < size ; ++it)
    {
      DynamicObject object;

      object.UnSerialize(this, packet);
      object.nodePath.reparent_to(rootDynamicObjects);
      object.nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
      dynamicObjects.push_back(object);
    }
  }
  
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
}

void           World::Serialize(Utils::Packet& packet)
{
  // Compile Step
  CompileWaypoints();

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
      }
      size = waypoints.size();
      packet << size;
      for (it = waypoints.begin() ; it != end ; ++it)
	(*it).Serialize(packet);
  }

  // MapObjects
  {
      int size = objects.size();

      packet << size;
      MapObjects::iterator it  = objects.begin();
      MapObjects::iterator end = objects.end();

      for (; it != end ; ++it) { (*it).Serialize(packet); }
  }

  CompileDoors();

  // DynamicObjects
  {
      int size = dynamicObjects.size();

      packet << size;
      DynamicObjects::iterator it  = dynamicObjects.begin();
      DynamicObjects::iterator end = dynamicObjects.end();

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
}

// MAP COMPILING
void           World::CompileWaypoints(void)
{
    Waypoints::iterator it  = waypoints.begin();
    Waypoints::iterator end = waypoints.end();

    for (; it != end ; ++it)
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
            LPoint3           tmp = other.get_pos() - parent.get_pos();
            LPoint3           dir    = parent.get_relative_vector(other, tmp);

            PT(CollisionNode) cnode  = new CollisionNode("compileWaypointsNode");
            //cnode->set_into_collide_mask(ColMask::Object);
            cnode->set_from_collide_mask(CollideMask(ColMask::Object));
            np = (*it).nodePath.attach_new_node(cnode);

            PT(CollisionSegment) ctube  = new CollisionSegment(LPoint3(0, 0, 0),
                                                         dir/*,
                                                         2.f*/);
            cnode->add_solid(ctube);

            //np.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
            //np.set_pos(0, 0, 0);

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
    }
}

void World::CompileDoors(void)
{
    Waypoints::iterator it  = waypoints.begin();
    Waypoints::iterator end = waypoints.end();

    for (; it != end ; ++it)
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
    }
}
