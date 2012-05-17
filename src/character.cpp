#include "character.hpp"
#include "scene_camera.hpp"

using namespace std;

bool Character::IsArcAccessible(int beg_x, int beg_y, int dest_x, int dest_y)
{
  if (beg_x == dest_x && dest_y == dest_y)
    return (true);
  bool success = false;

  Pathfinding*      pf    = _map.SetupPathfinding(this, 1);
  Pathfinding::Node node1 = pf->GetNode(beg_x,  beg_y);
  Pathfinding::Node node2 = pf->GetNode(dest_x, dest_y);

  Pathfinding::Node::Arcs::iterator it, end;

  for (it = node1.arcs.begin(), end = node1.arcs.end() ; it != end ; ++it)
  {
    if ((*it).first->x == node2.x && (*it).first->y == node2.y)
    {
      success = true;
      break ;
    }
  }
  _map.SetdownPathfinding(this, 1);
  return (success);
}

extern PT(ClockObject) globalClock;

bool Character::GoTo(int x, int y)
{
  float ftime = globalClock->get_real_time();
  bool success = true;

  ForceClosestCase();

  _path.clear();

  Pathfinding* pf       = _map.SetupPathfinding(this);

  if (!(pf->FindPath(_path, _mapPos.get_x(), _mapPos.get_y(), x, y)))
  {
    cout << "Character didn't find any path between " << _mapPos.get_x() << "," << _mapPos.get_y() << " and " << x << "," << y << endl;
    success = false;
  }
  _map.SetdownPathfinding(this);
  float etime = globalClock->get_real_time();
  cout << "Pathfinding time: " << (etime - ftime) << endl;
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
    GoTo(dest.x, dest.y);
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
