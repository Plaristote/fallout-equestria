#include "objects/door.hpp"
#include <character.hpp>

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

    bool success = c->GoTo(positionToReach1.x, positionToReach1.y);

    if (!success)
      success    = c->GoTo(positionToReach2.x, positionToReach2.y);
    if (success)
      c->ReachedCase.Connect(*this, &Door::InteractUse);
    else
      std::cout << "[GameConsole] Can't reach" << std::endl;
  }
}

void  Door::ProcessCollision(Pathfinding* map)
{
  if (!_opened)
  {
    Pathfinding::Node&                node = map->GetNode(_mapPos.get_x(), _mapPos.get_y());
    Pathfinding::Node*                toDc;
    Pathfinding::Node::Arcs::iterator it;

    if (_vertical)
      toDc = &(map->GetNode(_mapPos.get_x(), _mapPos.get_y() - 1));
    else
      toDc = &(map->GetNode(_mapPos.get_x() - 1, _mapPos.get_y()));
    it = std::find(node.arcs.begin(), node.arcs.end(), toDc);

    if (it != node.arcs.end())
      WithdrawArc(node, *it);
  }
}
