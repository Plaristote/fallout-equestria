#include "character.hpp"
#include "scene_camera.hpp"
#include <panda3d/nodePathCollection.h>

#include "level.hpp"
using namespace std;

void InstanceDynamicObject::ThatDoesNothing() { _level->ConsoleWrite("That does nothing"); }

ObjectCharacter::ObjectCharacter(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  _type         = Character;
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
  
  // Statistics
  _statistics = DataTree::Factory::JSON("data/charsheets/" + object->charsheet + ".json");
  if (_statistics)
  {
    _inventory.SetCapacity(GetStatistics()["Statistics"]["Carry Weight"]);
  }

  // Script
  _scriptContext = 0;
  _scriptModule  = 0;
  _scriptMain    = 0;
  if (object->script != "")
  {
    string prefixName = "IA_";
    string prefixPath = "scripts/";

    _scriptContext = Script::Engine::Get()->CreateContext();
    if (_scriptContext)
    {
      _scriptModule  = Script::Engine::LoadModule(prefixName + GetName(), prefixPath + object->script);
      if (_scriptModule)
        _scriptMain    = _scriptModule->GetFunctionByDecl("void main(Character@, float)");
    }
  }
}

void ObjectCharacter::Run(float elapsedTime)
{
  if (_scriptMain)
  {
    _scriptContext->Prepare(_scriptMain);
    _scriptContext->SetArgObject(0, this);
    _scriptContext->SetArgFloat(1, elapsedTime);
    _scriptContext->Execute();
  }
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
  if (path.size() > 0)
    path.erase(path.begin());
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
  if (object == 0)
  {
    std::cout << "GoTo: NULL pointer to object" << std::endl;
    return ;
  }
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
    while (_goToData.min_distance && _path.size() > 1)
    {
      _path.erase(--(_path.end()));
      _goToData.min_distance--;
    }
  }
  else
    _level->ConsoleWrite("Can't reach.");

  ProcessCollisions();
  object->ProcessCollisions();
}

void                ObjectCharacter::RunMovementNext(float elapsedTime)
{
  _waypointOccupied = _level->GetWorld()->GetWaypointFromId(_path.begin()->id);

  // Has reached object objective, if there is one ?
  if (_goToData.objective)
  {
    if (_path.size() <= _goToData.max_distance && HasLineOfSight(_goToData.objective))
    {
      _path.clear();
      ReachedDestination.Emit(this);
      ReachedDestination.DisconnectAll();
      return ;
    }
  }
  
  _path.erase(_path.begin());  

  if (_path.size() > 0)
  {
    bool pathAvailable = true;
    // Check if the next waypoint is still accessible
    UnprocessCollisions();
    Waypoint::Arc* arc = _waypointOccupied->GetArcTo(_path.begin()->id);

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
    ProcessCollisions();
    // End check if the next waypoint is still accessible

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
  {
    ReachedDestination.Emit(this);
    ReachedDestination.DisconnectAll();
  }
}

void                ObjectCharacter::RunMovement(float elapsedTime)
{
  //std::cout << "Running Movement" << std::endl;
  
  Waypoint&         next = *(_path.begin());
  // TODO: Speed walking / running / combat
  float             max_speed = 30.f * elapsedTime;
  LPoint3           distance;
  float             max_distance;    
  LPoint3           speed, axis_speed, dest;
  LPoint3           pos = _object->nodePath.get_pos();
  int               dirX, dirY, dirZ;

  distance  = pos - next.nodePath.get_pos();

  if (distance.get_x() == 0 && distance.get_y() == 0 && distance.get_z() == 0)
    RunMovementNext(elapsedTime);
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
  if (object == this)
    return (true);
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
