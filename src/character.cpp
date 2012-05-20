#include "character.hpp"
#include "scene_camera.hpp"

using namespace std;

ObjectNode* Character::Factory(WindowFramework* window, Tilemap& tilemap, Characters& characters, Data data)
{
  Character* character = new Character(window, tilemap, data, characters);

  characters.push_back(character);
  return (character);
}

bool Character::IsArcAccessible(int beg_x, int beg_y, int dest_x, int dest_y)
{
  if (beg_x == dest_x && beg_y == dest_y)
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
      if ((*it).observer)
        success = (*it).observer->GoingThrough(this);
      else
        success = true;
      break ;
    }
  }
  _map.SetdownPathfinding(this, 1);
  return (success);
}

extern PT(ClockObject) globalClock;

unsigned short Character::GoTo(int x, int y)
{
  float          ftime    = globalClock->get_real_time();
  unsigned short solution = 0;

  if (_lookingForNewWay == false)
    ReachedCase.DisconnectAll();
  ForceClosestCase();

  _path.clear();

  Pathfinding* pf       = _map.SetupPathfinding(this);

  if (!(pf->FindPath(this, _path, _mapPos.get_x(), _mapPos.get_y(), x, y)))
  {
    cout << "Character didn't find any path between " << _mapPos.get_x() << "," << _mapPos.get_y() << " and " << x << "," << y << endl;
    solution = 0;
  }
  else
    solution = _path.size();
  _map.SetdownPathfinding(this);
  float etime = globalClock->get_real_time();
  cout << "Pathfinding time: " << (etime - ftime) << endl;
  return (solution);
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

Character::Character(WindowFramework* window, Tilemap& map, Data data, Characters& chars) : ObjectNode(window, map, data), _characters(chars)
{
  _lookingForNewWay = false;
  _root.set_name(data["name"].Value());

  _collisionHandlerQueue = new CollisionHandlerQueue();
  _collisionFov          = new CollisionSphere(_root.get_x(), _root.get_y(), _root.get_z(), 1500.f);
  _collisionNode         = new CollisionNode("FovSphere" + data["name"].Value());
  _collisionPath         = NodePath(_collisionNode);
  _collisionNode->add_solid(_collisionFov);
  _collisionTraverser.add_collider(_collisionPath, _collisionHandlerQueue);

  _charLight = new PointLight("Light" + data["name"].Value());
  _charLight->set_color(LColor(0.8, 0.8, 0.8, 1));
  _charLight->set_attenuation(LVecBase3(0, 0, 0.1));
  _charLightNode = _window->get_render().attach_new_node(_charLight);
  _charLightNode.reparent_to(_root);
  _charLightNode.set_pos(0, 0, 0.01);
  _window->get_render().set_light(_charLightNode);
}

void Character::Run(float elapsedTime)
{
  if (!(_path.empty()))
    DoMovement(elapsedTime);

  _collisionFov->set_center(_root.get_pos());
  _collisionTraverser.traverse(_window->get_render());
  for(unsigned int i = 1; i<= _collisionHandlerQueue->get_num_entries();i++)
  {
    CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);
    cout<<entry;
  }

  //PT(CollisionSegment) segment = new CollisionSegment();
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
    if (!(_path.size()))
    {
      std::cout << "ReachedCase" << std::endl;
      ReachedCase.Emit(this);
    }
  }

  _root.set_pos(pos);
}
