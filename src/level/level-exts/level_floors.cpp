#include "level/level.hpp"

using namespace std;

void Level::HidingFloor::SetNodePath(NodePath np)
{
  floor = np;
  floor.set_transparency(TransparencyAttrib::M_alpha);
}

void Level::HidingFloor::SetFadingIn(bool set)
{
  fadingIn = set;
  alpha    = fadingIn ? 1.f : 0.f;
  if (!fadingIn)
    floor.show();
}

void Level::HidingFloor::Run(float elapsedTime)
{
  alpha += (fadingIn ? -0.1f : 0.1f) * (elapsedTime * 10);
  done   = (fadingIn ? alpha <= 0.f : alpha >= 1.f);
  floor.set_alpha_scale(alpha);
  if (fadingIn && done)
    floor.hide();
}

void Level::FloorFade(bool in, NodePath floor)
{
  list<HidingFloor>::iterator it = std::find(_hidingFloors.begin(), _hidingFloors.end(), floor);
  HidingFloor                 hidingFloor;
  
  if (it == _hidingFloors.end() && (floor.is_hidden() ^ !in))
    return ;
  hidingFloor.SetNodePath(floor);
  hidingFloor.SetFadingIn(in);  
  if (it != _hidingFloors.end())
  {
    hidingFloor.ForceAlpha(it->Alpha());
    _hidingFloors.erase(it);
  }
  _hidingFloors.push_back(hidingFloor);
}

bool Level::IsInsideBuilding(unsigned char& floor)
{
  bool                      isInsideBuilding = false;
  PT(CollisionRay)          pickerRay;
  PT(CollisionNode)         pickerNode;
  NodePath                  pickerPath;
  CollisionTraverser        collisionTraverser;
  PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();
  
  pickerNode   = new CollisionNode("isInsideBuildingRay");
  pickerPath   = _window->get_render().attach_new_node(pickerNode);
  pickerRay    = new CollisionRay();
  pickerNode->add_solid(pickerRay);
  
  pickerPath.set_pos(GetPlayer()->GetNodePath().get_pos());
  pickerRay->set_origin(0, 0, 0);
  pickerRay->set_direction(0, 0, 10);
  
  collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
  collisionTraverser.traverse(_window->get_render());
  
  collisionHandlerQueue->sort_entries();
  
  for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry  = collisionHandlerQueue->get_entry(i);
    MapObject*      object = GetWorld()->GetMapObjectFromNodePath(entry->get_into_node_path());
    
    if (!object)
      object = GetWorld()->GetDynamicObjectFromNodePath(entry->get_into_node_path());
    if (object && object != GetPlayer()->GetDynamicObject())
    {
      isInsideBuilding = true;
      floor            = object->floor;
      break ;
    }
  }
  
  pickerPath.detach_node();
  return (isInsideBuilding);
}

void Level::SetCurrentFloor(unsigned char floor)
{
  bool          showLowerFloors  = true;
  unsigned char floorAbove       = 0;
  bool          isInsideBuilding = IsInsideBuilding(floorAbove);
  
  for (unsigned int it = 0 ; it < floor ; ++it)
    FloorFade(showLowerFloors, _world->floors[it]);
  
  for (unsigned int it = floor + 1 ; it < _world->floors.size() ; ++it)
    FloorFade(isInsideBuilding, _world->floors[it]);
  
  if (_world->floors.size() > floor)
    FloorFade(false, _world->floors[floor]);
  
  World::Waypoints::const_iterator cur, end;
  
  for (cur = _world->waypoints.begin(), end = _world->waypoints.end() ; cur != end ; ++cur)
  {
    if (cur->floor == floor)
    {
      _camera.SlideToHeight(cur->nodePath.get_pos().get_z());
      break ;
    }
  }
  
  LPoint3 upperLeft(0, 0, 0), upperRight(0, 0, 0), bottomLeft(0, 0, 0);
  _world->GetWaypointLimits(floor, upperRight, upperLeft, bottomLeft);
  //_camera.SetLimits(bottomLeft.get_x(), bottomLeft.get_y(), upperRight.get_x(), upperRight.get_y());
  _currentFloor = floor;
}

void Level::CheckCurrentFloor(float elapsedTime)
{
  ObjectCharacter* player = GetPlayer();
  
  if (player)
  {
    Waypoint*      wp;

    wp = player->GetDynamicObject()->waypoint;
    if (!wp)
    {
      cout << "[Level] Player doesn't have an occupied waypoint. Something went wrong somewhere" << endl;
      return ;
    }
    if (!_floor_lastWp || (wp != _floor_lastWp))
    {
      SetCurrentFloor(wp->floor);
      _floor_lastWp = wp;
    }
  }
  {
    std::list<HidingFloor>::iterator cur, end;

    for (cur = _hidingFloors.begin(), end = _hidingFloors.end() ; cur != end ;)
    {
      cur->Run(elapsedTime);
      if (cur->Done())
        cur = _hidingFloors.erase(cur);
      else
        ++cur;
    }
  }
}