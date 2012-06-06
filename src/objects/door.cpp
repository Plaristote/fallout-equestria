#include "objects/door.hpp"
#include <character.hpp>

#include "level.hpp"

void ObjectDoor::ObserveWaypoints(bool doObserver)
{
  _waypointDisconnected = _object->lockedArcs;
  std::cout << "Observe Waypoints " << _waypointDisconnected.size() << std::endl;
  std::for_each(_waypointDisconnected.begin(), _waypointDisconnected.end(), [this, doObserver](std::pair<int, int> waypoints)
  {
    std::cout << "TROLOLOOOOL" << std::endl;
    Waypoint*        waypoint = _level->GetWorld()->GetWaypointFromId(waypoints.first);

    if (waypoint)
    {
      Waypoint::Arc* arc1 = waypoint->GetArcTo(waypoints.second);
      Waypoint::Arc* arc2 = 0;

      if (arc1)
      {
	arc1->observer = (doObserver ? this : 0);
	arc2           = arc1->to->GetArcTo(waypoints.first);
      }
      if (arc2)
	arc2->observer = (doObserver ? this : 0);
    }
  });
}

bool ObjectDoor::CanGoThrough(unsigned char id)
{
  if (_closed)
  {
    std::cout << "Returning value != 0" << std::endl;
    return (id != 0);
  }
  return (true);
}

void ObjectDoor::GoingThrough(void)
{
  _closed = false;
}

/*
 * WARNING The following is legacy and will have to go away. On est pas à la soupe populaire.
 */

/*ObjectNode* Door::Factory(WindowFramework* window, Tilemap& map, Characters&, Data data)
{
  return (new Door(window, map, data));
}

Door::Door(WindowFramework* window, Tilemap& map, Data data) : ObjectNode(window, map, data)
{
  LPoint3 current_pos = _root.get_pos();

  _opened = false;

  float scale = data["scale"];

  if (data["vertical"].Value() == "1" || data["horizontal"].Value() == "0")
  {
    _root.set_hpr(90, 90, 0);
    _root.set_pos(current_pos.get_x(), current_pos.get_y() + (WORLD_SCALE * (2 / 2)), 0);
    _vertical = true;
  }
  else
  {
    _root.set_hpr(0, 90, 0);
    _root.set_pos(current_pos.get_x() - (WORLD_SCALE * (2 / 2)), current_pos.get_y(), 0);
    _vertical = false;
  }
  SetCollisionEnabled(data["opened"].Value() == "1");
}

void  Door::InteractUse(Character* c)
{
  Position position = c->GetPosition();
  bool     canReach = false;

  if (_vertical)
    canReach = (position.y == _mapPos.y || position.y == _mapPos.y - 1) && (position.x >= _mapPos.x - 1 && position.x <= _mapPos.x + 1);
  else
    canReach = (position.x == _mapPos.x || position.x == _mapPos.x - 1) && (position.y >= _mapPos.y - 1 && position.y <= _mapPos.y + 1);

  if (canReach)
  {
    cout << "[GameConsole] You opened a door" << endl;
    _opened = !_opened;
    this->SetCollisionEnabled(!_opened);
  }
  else
  {
    Position positionToReach1, positionToReach2;

    positionToReach1 = _mapPos;
    if (_vertical)
    {
      positionToReach2.y = _mapPos.y - 1;
      positionToReach2.x = _mapPos.x;
    }
    else
    {
      positionToReach2.y = _mapPos.y;
      positionToReach2.x = _mapPos.x - 1;
    }

    unsigned short len1 = c->GoTo(positionToReach1.x, positionToReach1.y);
    unsigned short len2 = c->GoTo(positionToReach2.x, positionToReach2.y);

    if (len1 == 0 && len2 == 0)
      std::cout << "[GameConsole] Can't reach" << std::endl;
    else
    {
      if ((len1 < len2 && len1 != 0) || len2 == 0)
        c->GoTo(positionToReach1.x, positionToReach1.y);
      c->ReachedCase.Connect(*this, &Door::InteractUse);
    }
  }
}

void  Door::ProcessCollision(Pathfinding* map)
{
  if (!_opened)
  {
    Pathfinding::Node&                node = map->GetNode(_mapPos.get_x(), _mapPos.get_y());
    Pathfinding::Node*                toDc;
    Pathfinding::Node::Arcs::iterator it1;
    Pathfinding::Node::Arcs::iterator it2;

    if (_vertical)
      toDc = &(map->GetNode(_mapPos.get_x(), _mapPos.get_y() - 1));
    else
      toDc = &(map->GetNode(_mapPos.get_x() - 1, _mapPos.get_y()));
    it1 = std::find(node.arcs.begin(), node.arcs.end(), toDc);
    it2 = std::find(toDc->arcs.begin(), toDc->arcs.end(), &node);

    if (it1 != node.arcs.end())
      (*it1).observer = this;
    if (it2 != toDc->arcs.end())
      (*it2).observer = this;
    //WithdrawArc(node, *it);
  }
}

bool  Door::CanGoThrough(Character*)
{
  if (!_opened)
  {
    // If Character points to the Player, return false
    // Else if the door is locked and the character hasn't the key, return false
    return (false);
  }
  return (true);
}

bool  Door::GoingThrough(Character* character)
{
  bool can = CanGoThrough(character);

  if (!_opened && can)
  {
    // TODO: if just closed, open the door. If locked, open and close the door.
    _opened = true;
  }
  return (can);
}*/
