#include "character.hpp"
#include "scene_camera.hpp"

using namespace std;

bool Character::GoTo(int x, int y)
{
  bool success = true;

  // TODO Replace this with something moving the character to the closest case instead of the last one
  //      Or simply delete if future-me finds a better way.
  ForceClosestCase();

  _path.clear();

  //int          pf_depth = (fabs(float((_mapPos.get_x() - x))) + fabs(float(((_mapPos.get_y() - y)))));
  Pathfinding* pf       = _map.GeneratePathfinding(this/*, pf_depth*/);

  if (!(pf->FindPath(_path, _mapPos.get_x(), _mapPos.get_y(), x, y)))
  {
    cout << "Character didn't find any path between " << _mapPos.get_x() << "," << _mapPos.get_y() << " and " << x << "," << y << endl;
    success = false;
  }
  delete pf;
  return (success);
}

bool Character::CanReach(ObjectNode* node, int min_distance)
{
  float dist_x = ABS(node->GetPosition().x - GetPosition().x);
  float dist_y = ABS(node->GetPosition().y - GetPosition().y);

  return (dist_x <= min_distance && dist_y <= min_distance);
}

bool Character::TryToReach(ObjectNode* node, int min_distance)
{
  // TODO this isn't the real deal. It needs to get character to a certain distance of node, not at node.
  return (GoTo(node->GetPosition().x, node->GetPosition().y));
}

void Character::Run(float elapsedTime)
{
  if (!(_path.empty()))
    DoMovement(elapsedTime);
}

void Character::DoMovement(float elapsedTime)
{
  unsigned char     dir  = MotionNone;
  Pathfinding::Node next = *(_path.begin());

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
    pos.set_y(pos.get_y() + movement);
  else if (dir & MotionBottom)
    pos.set_y(pos.get_y() - movement);

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
  }

  _root.set_pos(pos);
}
