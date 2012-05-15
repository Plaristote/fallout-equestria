#include "tilemap.hpp"

using namespace std;

Tilemap::Tilemap(WindowFramework* window) : _window(window)
{
  _scale = WORLD_SCALE;
}

// WARNING This method is exceedingly long because I'm lazy.
// TODO    If future-me isn't as lazy, it should be possible to at least reduce it by half. At least.
void Tilemap::Load(Data data)
{
  Data tileset = data["tileset"];
  Data tilemap = data["tilemap"];

  //data.Output();
  _size.set_x(data["size_x"]);
  _size.set_y(data["size_y"]);

  cout << "Tilemap size " << _size.get_x() << "," << _size.get_y() << endl;

  // TILEMAP
  vector<string> tilesSrc;

  for_each(tileset.begin(), tileset.end(), [&tilesSrc](Data children) { tilesSrc.push_back(children.Value()); });

  Data::iterator currentTile = tilemap.begin();

  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x)
    {
      NodePath     newNode = _window->load_model(_window->get_render(), "models/plane.obj");
      unsigned int tileId  = *currentTile;
      Texture*     texture;

      newNode.set_scale(_scale);
      newNode.set_hpr(0, 90, 0);
      newNode.set_pos(x * (_scale * 2), (_size.get_y() - y) * (_scale * 2), 0);
      if (tileId < tilesSrc.size())
      {
        texture = TexturePool::load_texture("textures/tiles/" + tilesSrc[tileId]);
        newNode.set_texture(texture);
      }
      _nodes.push_back(newNode);
      ++currentTile;

      // Set NodePath tags
      std::stringstream stream1, stream2;

      newNode.set_tag("tile", "1");
      stream1 << x;
      newNode.set_tag("pos_x", stream1.str());
      stream2 << y;
      newNode.set_tag("pos_y", stream2.str());
    }
  }

  // CEILING
  Data ceiling = data["ceiling"];

  _ceilingNode = _window->get_render().attach_new_node("Ceiling Node");
  currentTile = ceiling.begin();
  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x)
    {
      if ((*currentTile).Value() != "-")
      {
        NodePath     newNode = _window->load_model(_window->get_render(), "models/plane.obj");
        unsigned int tileId  = *currentTile;
        Texture*     texture;

        newNode.set_scale(_scale);
        newNode.set_hpr(0, 90, 0);
        newNode.set_pos(x * (_scale * 2), (_size.get_y() - y) * (_scale * 2), (_scale * 4));
        if (tileId < tilesSrc.size())
        {
          texture = TexturePool::load_texture("textures/tiles/" + tilesSrc[tileId]);
          newNode.set_texture(texture);
        }
        newNode.reparent_to(_ceilingNode);
        _ceiling.push_back(CeilingTile(newNode));

//         CeilingTile& added = GetCeiling(x, y);
// 
//         if (x > 0)
//         {
//           CeilingTile& otherTile = GetCeiling(x - 1, y);
//           if (otherTile.isCeiling)
//           {
//             added.connectedTo.push_back(&otherTile);
//             otherTile.connectedTo.push_back(&added);
//           }
//         }
//         if (y > 0)
//         {
//           CeilingTile& otherTile = GetCeiling(x, y - 1);
//           if (otherTile.isCeiling)
//           {
//             added.connectedTo.push_back(&otherTile);
//             otherTile.connectedTo.push_back(&added);
//           }
//         }
        

        // Set NodePath tags
        std::stringstream stream1, stream2;

        newNode.set_tag("ceiling", "1");
        stream1 << x;
        newNode.set_tag("pos_x", stream1.str());
        stream2 << y;
        newNode.set_tag("pos_y", stream2.str());
      }
      else
        _ceiling.push_back(CeilingTile());
      ++currentTile;
    }
  }

  // WALLS
  Data wallset = data["wallset"];
  Data hwalls  = data["hwalls"];
  Data vwalls  = data["vwalls"];

  vector<string> wallsSrc;

  for_each(wallset.begin(), wallset.end(), [&wallsSrc](Data children) { wallsSrc.push_back(children.Value()); });

  Data::iterator currentWall = hwalls.begin();

  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentWall)
    {
      unsigned int tileId  = *currentWall;

      if (tileId == 0) continue ;

      NodePath         newNode = _window->load_model(_window->get_render(), "models/wall.obj");
      Texture*         texture;

      newNode.set_scale(_scale);
      newNode.set_hpr(0, 90, 0);
      // (_scale * (2 / 2) => WorldScale * (half the size of the wall)
      newNode.set_pos(x * (_scale * 2) - (_scale * (2 / 2)), (_size.get_y() - y) * (_scale * 2), 0);
      if (--tileId < wallsSrc.size())
      {
        texture = TexturePool::load_texture("textures/walls/" + wallsSrc[tileId]);
        newNode.set_texture(texture);
      }
      MapTile& tile = GetTile(x, y);
      tile.hWall    = newNode;
      tile.hasHWall = true;
    }
  }

  currentWall = vwalls.begin();

  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentWall)
    {
      unsigned int tileId  = *currentWall;

      if (tileId == 0) continue ;

      NodePath     newNode = _window->load_model(_window->get_render(), "models/wall.obj");
      Texture*     texture;

      newNode.set_scale(_scale);
      newNode.set_hpr(90, 90, 0);
      newNode.set_pos(x * (_scale * 2), ((_size.get_y() - y) * (_scale * 2)) + (_scale * (2 / 2)), 0);
      if (--tileId < wallsSrc.size())
      {
        texture = TexturePool::load_texture("textures/walls/" + wallsSrc[tileId]);
        newNode.set_texture(texture);
      }
      MapTile& tile = GetTile(x, y);
      tile.vWall    = newNode;
      tile.hasVWall = true;
    }
  }
}

const Tilemap::MapTile& Tilemap::GetTile(unsigned int x, unsigned int y) const
{
  NodePath     toRet;
  unsigned int it;

  it = (y * _size.get_x()) + x;
  if (it < _nodes.size())
    return (_nodes[it]);
  return (_nodes[0]);
}

Tilemap::MapTile& Tilemap::GetTile(unsigned int x, unsigned int y)
{
  unsigned int it;

  it = (y * _size.get_x()) + x;
  if (it < _nodes.size())
    return (_nodes[it]);
  return (_nodes[0]);
}

Tilemap::CeilingTile& Tilemap::GetCeiling(unsigned int x, unsigned int y)
{
  unsigned int it;

  it = (y * _size.get_x()) + x;
  if (it < _ceiling.size())
    return (_ceiling[it]);
  return (_ceiling[0]);
}

// WARNING This is too long as well. Optimize please.
Pathfinding* Tilemap::GeneratePathfinding(MapElement* pathseeker, int max_depth) const
{
  std::vector<Pathfinding::Node>  pfNodes(_size.get_x() * _size.get_y());
  Pathfinding&                    pf = *(new Pathfinding(pfNodes, _size.get_x(), _size.get_y()));

  // if no max_depth specified, build a pathfinder for the whole map  
  if (max_depth == 0)
    max_depth = (_size.get_x() > _size.get_y() ? _size.get_x() : _size.get_y());

  // Calculate which part of the map must be built into the pathfinder
  MapElement::Position seekerPos = pathseeker->GetPosition();
  int beg_x = seekerPos.x - max_depth;
  int end_x = seekerPos.x + max_depth + 1; // + 1 because loop will stop at -1
  int beg_y = seekerPos.y - max_depth;
  int end_y = seekerPos.y + max_depth + 1; // same here

  if (beg_x <  0)             beg_x = 0;
  if (end_x >= _size.get_x()) end_x = _size.get_x();
  if (beg_y <  0)             beg_y = 0;
  if (end_y >= _size.get_y()) end_y = _size.get_y();

  cout << "Only checking collision for zone (" << beg_x << "," << beg_y << ") to (" << end_x << "," << end_y << ")" << endl;

  // Add every needed arc for a given zone of the map to the pathfinder
  for (unsigned int x = beg_x ; x < end_x ; ++x)
  {
    for (unsigned int y = beg_y ; y < end_y ; ++y)
    {
      Pathfinding::Node& node = pf.GetNode(x, y);
      MapTile            tile = GetTile(x, y);

      node.x = x;
      node.y = y;
      // To the left
      if (x != 0 && !tile.hasHWall)
      {
        MapTile            left     = GetTile(x - 1, y);
        Pathfinding::Node& leftNode = pf.GetNode(x - 1, y);

        leftNode.arcs.push_back(Pathfinding::Node::Arc(&node, 1));
        node.arcs.push_back(Pathfinding::Node::Arc(&leftNode, 1));
      }

      // To the top
      if (y != 0 && !tile.hasVWall)
      {
        MapTile             top     = GetTile(x, y - 1);
        Pathfinding::Node&  topNode = pf.GetNode(x, y - 1);

        topNode.arcs.push_back(Pathfinding::Node::Arc(&node, 1));
        node.arcs.push_back(Pathfinding::Node::Arc(&topNode, 1));
      }

      // To the top and left
      if (x != 0 and y != 0 and !tile.hasHWall and !tile.hasVWall and !(GetTile(x - 1, y).hasVWall))
      {
        MapTile             topLeft     = GetTile(x - 1, y - 1);
        Pathfinding::Node&  topLeftNode = pf.GetNode(x - 1, y - 1);

        topLeftNode.arcs.push_back(Pathfinding::Node::Arc(&node, 1));
        node.arcs.push_back(Pathfinding::Node::Arc(&topLeftNode, 1.1));
      }

      // To the top and right
      if (y != 0 and x + 1 != _size.get_x() and !tile.hasVWall)
      {
        MapTile right    = GetTile(x + 1, y);

        if (!right.hasHWall and !right.hasVWall)
        {
          Pathfinding::Node& topRightNode = pf.GetNode(x + 1, y - 1);

          topRightNode.arcs.push_back(Pathfinding::Node::Arc(&node, 1));
          node.arcs.push_back(Pathfinding::Node::Arc(&topRightNode, 1.1));
        }
      }
    }
  }

  // Check all the MapElement for their own collisions
  // TODO Optimize by checking if the MapElement is part of the computed zone.
  //      This will require BoundingBoxes.
  std::for_each(_mapElements.begin(), _mapElements.end(), [&pf, pathseeker](MapElement* element)
  {
    if (element != pathseeker)
      element->ProcessCollision(&pf);
  });
  return (&pf);
}

//
// CEILING
//
void Tilemap::SetCeilingTransparent(float value)
{
  _ceilingNode.set_transparency(value == 1.f ? TransparencyAttrib::M_none : TransparencyAttrib::M_alpha);
  _ceilingNode.set_color(1, 1, 1, value);
}