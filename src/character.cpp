#include "character.hpp"
#include "scene_camera.hpp"
#include <panda3d/nodePathCollection.h>

#include "level.hpp"
using namespace std;

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  // Line of sight tools
  _losNode      = new CollisionNode("losRay");
  _losNode->set_from_collide_mask(CollideMask(ColMask::Object | ColMask::DynObject));
  _losPath      = object->nodePath.attach_new_node(_losNode);
  _losRay       = new CollisionRay();
  _losRay->set_origin(0, 0, 0);
  _losRay->set_direction(-10, 0, 0);
  _losPath.set_pos(0, -0.5, 0);
  //_losPath.show();
  _losNode->add_solid(_losRay);
  _losHandlerQueue = new CollisionHandlerQueue();
  _losTraverser.add_collider(_losPath, _losHandlerQueue);  
}

void ObjectCharacter::Run(float elapsedTime)
{
  if (_path.size() > 0)
    RunMovement(elapsedTime);
}

unsigned short      ObjectCharacter::GetPathDistance(InstanceDynamicObject* object)
{
  unsigned short ret;
  
  object->UnprocessCollisions();
  ret = GetPathDistance(object->GetOccupiedWaypoint());
  object->ProcessCollisions();
  return (ret);
}

unsigned short      ObjectCharacter::GetPathDistance(Waypoint* waypoint)
{
  std::list<Waypoint> path;
  
  UnprocessCollisions();
  _level->FindPath(path, *_waypointOccupied, *waypoint);
  ProcessCollisions();
  return (path.size());
}

void                ObjectCharacter::GoTo(unsigned int id)
{
  Waypoint*         wp = _level->GetWorld()->GetWaypointFromId(id);
  
  if (wp) GoTo(wp);
}

void                ObjectCharacter::GoTo(Waypoint* waypoint)
{
  ReachedDestination.DisconnectAll();
  _goToData.objective = 0;

  UnprocessCollisions();
  _path.clear();
  if (_waypointOccupied && waypoint)
  {
    if (!(_level->FindPath(_path, *_waypointOccupied, *waypoint)))
      _level->ConsoleWrite("No path.");
  }
  else
    cout << "Character doesn't have a waypointOccupied" << endl;
  ProcessCollisions();
}

void                ObjectCharacter::GoTo(InstanceDynamicObject* object, int max_distance)
{
  ReachedDestination.DisconnectAll();
  _goToData              = object->GetGoToData(this);
  _goToData.max_distance = max_distance;

  UnprocessCollisions();
  object->UnprocessCollisions();

  _path.clear();
  if (_waypointOccupied && _goToData.nearest)
  {
    if (!(_level->FindPath(_path, *_waypointOccupied, *_goToData.nearest)))
      _level->ConsoleWrite("Can't reach.");
  }

  ProcessCollisions();
  object->ProcessCollisions();
}

void                ObjectCharacter::RunMovement(float elapsedTime)
{
  cout << "lol wtf ?" << endl;
  Waypoint&         next = *(_path.begin());
  // TODO: Speed walking / running / combat
  float             max_speed = 6.f * elapsedTime;
  LPoint3           distance;
  float             max_distance;    
  LPoint3           speed, axis_speed, dest;
  LPoint3           pos = _object->nodePath.get_pos();
  int               dirX, dirY, dirZ;

  distance  = pos - next.nodePath.get_pos();

  if (distance.get_x() == 0 && distance.get_y() == 0 && distance.get_z() == 0)
  {
    // Has reached object objective, if there is one ?
    if (_goToData.objective)
    {
      if (_path.size() <= _goToData.max_distance && HasLineOfSight(_goToData.objective))
      {
	_path.clear();
	ReachedDestination.Emit(this);
	return ;
      }
    }

    _waypointOccupied = _level->GetWorld()->GetWaypointFromId(_path.begin()->id);
    _path.erase(_path.begin());

    if (_path.size() > 0)
    {
      bool pathAvailable = true;
      // Check if the next waypoint is still accessible
      UnprocessCollisions();
      Waypoint::Arc* arc = _waypointOccupied->GetArcTo(_path.begin()->id);
      ProcessCollisions();
      
      if (arc)
      {
	if (arc->observer)
	{
	  if (arc->observer->CanGoThrough(0))
	    arc->observer->GoingThrough();
	  else
	    pathAvailable = false;
	}
      }
      else
	pathAvailable = false;

      if (pathAvailable)
        RunMovement(elapsedTime);
      else
      {
	if (_goToData.objective)
	  GoTo(_goToData.objective);
	else
	{
	  Waypoint* dest = _level->GetWorld()->GetWaypointFromId((*(--(_path.end()))).id);
	  GoTo(dest);
	}
	if (_path.size() == 0)
	{
	  _level->ConsoleWrite("Path is obstructed");
	  ReachedDestination.DisconnectAll();
	}
      }
    }
    else
      ReachedDestination.Emit(this);
  }
  else
  {
    dirX = dirY = dirZ = 1;
    if (distance.get_x() < 0)
    {
      distance.set_x(-(distance.get_x()));
      dirX = -1;
    }
    if (distance.get_y() < 0)
    {
      distance.set_y(-(distance.get_y()));
      dirY = -1;
    }
    if (distance.get_z() < 0)
    {
      distance.set_z(-(distance.get_z()));
      dirZ = -1;
    }

    max_distance = (distance.get_x() > distance.get_y() ? distance.get_x() : distance.get_y());
    max_distance = (distance.get_z() > max_distance     ? distance.get_z() : max_distance);
    
    axis_speed.set_x(distance.get_x() / max_distance);
    axis_speed.set_y(distance.get_y() / max_distance);
    axis_speed.set_z(distance.get_z() / max_distance);
    if (max_speed > max_distance)
      max_speed = max_distance;
    speed.set_x(max_speed * axis_speed.get_x() * dirX);
    speed.set_y(max_speed * axis_speed.get_y() * dirY);
    speed.set_z(max_speed * axis_speed.get_z() * dirZ);

    dest = pos - speed;

    _object->nodePath.set_pos(dest);
  }
}

bool                ObjectCharacter::HasLineOfSight(InstanceDynamicObject* object)
{
  NodePath root  = _object->nodePath;
  NodePath other = object->GetNodePath();
  bool ret = true;

  LVecBase3 rot = root.get_hpr();
  LVector3  dir = root.get_relative_vector(other, other.get_pos() - root.get_pos());

  _losPath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
  _losRay->set_direction(dir.get_x(), dir.get_y(), dir.get_z());
  _losTraverser.traverse(_level->GetWorld()->window->get_render());

  _losHandlerQueue->sort_entries();

  for (unsigned int i = 0 ; i < _losHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = _losHandlerQueue->get_entry(i);
    NodePath        node  = entry->get_into_node_path();

    if (root.is_ancestor_of(node))
      continue ;
    if (!(other.is_ancestor_of(node)))
      ret = false;
    break ;
  }
  return (ret);
}

/*
 * WARNING The following is LEGACY and will disapear soon
 */

// CollideMask characterCollideMask(Object);
// 
// ObjectNode* Character::Factory(WindowFramework* window, Tilemap& tilemap, Characters& characters, Data data)
// {
//   Character* character = new Character(window, tilemap, data, characters);
// 
//   characters.push_back(character);
//   return (character);
// }
// 
// bool Character::IsArcAccessible(int beg_x, int beg_y, int dest_x, int dest_y)
// {
//   if (beg_x == dest_x && beg_y == dest_y)
//     return (true);
//   bool success = false;
// 
//   Pathfinding*      pf    = _map.SetupPathfinding(this, 1);
//   Pathfinding::Node node1 = pf->GetNode(beg_x,  beg_y);
//   Pathfinding::Node node2 = pf->GetNode(dest_x, dest_y);
// 
//   Pathfinding::Node::Arcs::iterator it, end;
// 
//   for (it = node1.arcs.begin(), end = node1.arcs.end() ; it != end ; ++it)
//   {
//     if ((*it).first->x == node2.x && (*it).first->y == node2.y)
//     {
//       if ((*it).observer)
//         success = (*it).observer->GoingThrough(this);
//       else
//         success = true;
//       break ;
//     }
//   }
//   _map.SetdownPathfinding(this, 1);
//   return (success);
// }
// 
// extern PT(ClockObject) globalClock;
// 
// unsigned short Character::GoTo(int x, int y)
// {
//   cout << "Executing GoTo" << endl;
//   float          ftime    = globalClock->get_real_time();
//   unsigned short solution = 0;
// 
//   if (_lookingForNewWay == false)
//     ReachedCase.DisconnectAll();
//   ForceClosestCase();
// 
//   _path.clear();
// 
//   Pathfinding* pf       = _map.SetupPathfinding(this);
// 
//   if (!(pf->FindPath(this, _path, _mapPos.get_x(), _mapPos.get_y(), x, y)))
//   {
//     cout << "Character didn't find any path between " << _mapPos.get_x() << "," << _mapPos.get_y() << " and " << x << "," << y << endl;
//     solution = 0;
//   }
//   else
//   {
//     cout << "Character found a path between " << _mapPos.get_x() << "," << _mapPos.get_y() << " and " << x << "," << y << endl;
//     solution = _path.size();
//   }
//   _map.SetdownPathfinding(this);
//   float etime = globalClock->get_real_time();
//   cout << "Pathfinding time: " << (etime - ftime) << endl;
//   return (solution);
// }
// 
// bool Character::CanReach(ObjectNode* node, int min_distance)
// {
//   float dist_x = ABS(node->GetPosition().x - GetPosition().x);
//   float dist_y = ABS(node->GetPosition().y - GetPosition().y);
// 
//   return (dist_x <= min_distance && dist_y <= min_distance);
// }
// 
// bool Character::TryToReach(ObjectNode* node, int min_distance)
// {
//   // TODO this isn't the real deal. It needs to get character to a certain distance of node, not at node.
//   return (GoTo(node->GetPosition().x, node->GetPosition().y));
// }
// 
// Character::Character(WindowFramework* window, Tilemap& map, Data data, Characters& chars) : ObjectNode(window, map, data), _characters(chars)
// {
//   _lookingForNewWay = false;
//   _root.set_name(data["name"].Value());
//   _root.set_collide_mask(0);
// 
//   _selfSphere     = new CollisionSphere(0, 0, 0, 5.f);
//   _selfSphereNode = new CollisionNode("characterRange");
//   _selfSphereNP   = _root.attach_new_node(_selfSphereNode);
//   _selfSphereNode->set_into_collide_mask(characterCollideMask);
//   _selfSphereNode->set_from_collide_mask(0);
//   _selfSphereNode->add_solid(_selfSphere);
// 
//   _selfSphereNP.show();
// 
// 
//   _collisionNode         = new CollisionNode("characterRange");
//   _collisionPath         = _root.attach_new_node(_collisionNode);
//   _collisionNode->set_from_collide_mask(characterCollideMask);
//   _collisionNode->set_into_collide_mask(0);
//   _collisionFov          = new CollisionSphere(0, 0, 0, 0.f);
//   _collisionNode->add_solid(_collisionFov);
//   _collisionHandlerQueue = new CollisionHandlerQueue();
//   _collisionTraverser.add_collider(_collisionPath, _collisionHandlerQueue);
// 
//   /*PT(CollisionVisualizer) colVisualizer;
//   PandaNode* pn = colVisualizer;
// 
//   _root.attach_new_node(pn);*/
//   
//   _collisionPath.show();

  /*_charLight = new PointLight("Light" + data["name"].Value());
  _charLight->set_color(LColor(0.8, 0.8, 0.8, 1));
  _charLight->set_attenuation(LVecBase3(0, 0, 0.1));
  _charLightNode = _window->get_render().attach_new_node(_charLight);
  _charLightNode.reparent_to(_root);
  _charLightNode.set_pos(0, 0, 0.01);
  _window->get_render().set_light(_charLightNode);*//*

  // Line of sight tools
  _losNode      = new CollisionNode("losRay");
  _losNode->set_from_collide_mask(CollideMask(Object | Walls));
  _losPath      = _root.attach_new_node(_losNode);
  _losRay       = new CollisionRay();
  _losRay->set_origin(0, 0, 0);
  _losRay->set_direction(-10, 0, 0);
  _losPath.set_pos(0, -0.5, 0);
  //_losPath.show();
  _losNode->add_solid(_losRay);
  _losHandlerQueue = new CollisionHandlerQueue();
  _losTraverser.add_collider(_losPath, _losHandlerQueue);
}

bool Character::HasLineOfSight(Character* other)
{
  bool ret = true;

  LVecBase3 rot = _root.get_hpr();
  LVector3  dir = _root.get_relative_vector(other->_root, other->_root.get_pos() - _root.get_pos());

  _losPath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
  _losRay->set_direction(dir.get_x(), dir.get_y(), dir.get_z());
  _losTraverser.traverse(_window->get_render());

  _losHandlerQueue->sort_entries();

  for (unsigned int i = 0 ; i < _losHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = _losHandlerQueue->get_entry(i);
    NodePath        node  = entry->get_into_node_path();

    if (_root.is_ancestor_of(node))
      continue ;
    if (!(other->_root.is_ancestor_of(node)))
      ret = false;
    break ;
  }
  return (ret);
}

void Character::Run(float elapsedTime)
{
  if (!(_path.empty()))
    DoMovement(elapsedTime);

  // TEST
  // Update Field of View
  if (_timerFov.GetElapsedTime() > 5.f)
  {
    _timerFov.Restart();
    
    Characters _fieldOfView;
    _fieldOfView.clear();

    _collisionFov->set_radius(50.f);
    //std::cout << "Range test initialized    in " << _timerFov.GetElapsedTime() << std::endl;
    _collisionTraverser.traverse(_window->get_render());
    //std::cout << "Range test executed       in " << _timerFov.GetElapsedTime() << std::endl;
    _collisionFov->set_radius(0.f);
    for(unsigned int i = 0 ; i < _collisionHandlerQueue->get_num_entries() ; i++)
    {
      CollisionEntry*      entry = _collisionHandlerQueue->get_entry(i);
      NodePath             into  = entry->get_into_node_path();

      if (_root.is_ancestor_of(into))
        continue ;
      
      Characters::iterator it    = Character::Find(_characters, into);

      if (it != _characters.end())
      {
        if (Character::Find(_fieldOfView, into) == _fieldOfView.end())
          _fieldOfView.push_back(*it);
      }
    }
    for_each(_fieldOfView.begin(), _fieldOfView.end(), [this](Character* character)
    {
      bool lineOfSight = HasLineOfSight(character);
    });
  }
}

Pathfinding::Node Character::GetCurrentDestination(void) const
{
  if (_path.size())
  {
    DirectionPath::const_iterator last = --(_path.end());

    return (*last);
  }
  return (Pathfinding::Node());
}

void Character::DoMovement(float elapsedTime)
{
  unsigned char     dir  = MotionNone;
  Pathfinding::Node next = *(_path.begin());

  if (!(IsArcAccessible(_mapPos.get_x(), _mapPos.get_y(), next.x, next.y)))
  {
    Pathfinding::Node dest = GetCurrentDestination();
    
    ForceCurrentCase(_mapPos.get_x(), _mapPos.get_y());
    _lookingForNewWay = true;
    GoTo(dest.x, dest.y);
    _lookingForNewWay = false;
    return ;
  }
  
  if      (_mapPos.get_x() > next.x)
    dir |= MotionLeft;
  else if (_mapPos.get_x() < next.x)
    dir |= MotionRight;
  if      (_mapPos.get_y() > next.y)
    dir |= MotionTop;
  else if (_mapPos.get_y() < next.y)
    dir |= MotionBottom;

  LPoint3 pos      = _root.get_pos();
  float   movement = 6.f * elapsedTime;

  if (dir & MotionLeft)
    pos.set_x(pos.get_x() - movement);
  else if (dir & MotionRight)
    pos.set_x(pos.get_x() + movement);
  if (dir & MotionTop)
    pos.set_y(pos.get_y() - movement);
  else if (dir & MotionBottom)
    pos.set_y(pos.get_y() + movement);

  Tilemap::MapTile& tile    = _map.GetTile(next.x, next.y);
  LPoint3           tilePos = tile.position;

  if (ABS(tilePos.get_x() - pos.get_x()) <= movement)
    pos.set_x(tilePos.get_x());
  if (ABS(tilePos.get_y() - pos.get_y()) <= movement)
    pos.set_y(tilePos.get_y());

  if (tilePos.get_x() == pos.get_x() && tilePos.get_y() == pos.get_y())
  {
    _mapPos.set_x(next.x);
    _mapPos.set_y(next.y);
    _path.erase(_path.begin());
    if (!(_path.size()))
    {
      std::cout << "ReachedCase" << std::endl;
      ReachedCase.Emit(this);
    }
  }

  _root.set_pos(pos);
}*/
