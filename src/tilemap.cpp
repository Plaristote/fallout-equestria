#include "tilemap.hpp"

# define MODEL_TILE_PATH    "models/plane.obj"
# define TEXTURE_TILE_PATH  "textures/tiles/"
# define TEXTURE_WALL_PATH  "textures/walls/"

using namespace std;

Tilemap::Tilemap(WindowFramework* window) : _window(window)
{
  _pf          = 0;
  _scale       = WORLD_SCALE;
  _groupSize   = DEFAULT_GROUP_SIZE;
  _groundNode  = window->get_render().attach_new_node("Ground  Main Node");
  _ceilingNode = window->get_render().attach_new_node("Ceiling Main Node");
}

Tilemap::~Tilemap()
{
  if (_pf) delete _pf;
}

template<class NodeType>
void Tilemap::LoadTiles(string tileType, Data tileset, Data map, LPoint3 posModificator, NodePath fatherGroup, vector<NodePath>& groups, vector<NodeType>& storage)
{
  vector<string> tilesSrc;
  Data::iterator currentTile = map.begin();

  // Loading Groups
  for (unsigned int   y = 0 ; y < _groupSize ; ++y)
  {
    for (unsigned int x = 0 ; x < _groupSize ; ++x)
    {
      NodePath groupNode;
      std::stringstream stream;

      stream << x << "," << y;
      groupNode = _window->get_render().attach_new_node("Ground Grouping Node " + stream.str());
      groups.push_back(groupNode);
    }
  }

  // Loading Tileset
  for_each(tileset.begin(), tileset.end(), [&tilesSrc](Data children) { tilesSrc.push_back(children.Value()); });

  // Loading Tiles
  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentTile)
    {
      Tile tile;

      tile.position.set_x(x                   * (_scale * TILE_UNIT) + posModificator.get_x());
      tile.position.set_y((_size.get_y() - y) * (_scale * TILE_UNIT) + posModificator.get_y());
      tile.position.set_z(posModificator.get_z());

      // The tile is defined
      if ((*currentTile).Value() != "-")
      {
        stringstream stream1, stream2;
        NodePath     newNode = _window->load_model(_window->get_render(), MODEL_TILE_PATH);
        unsigned int tileId  = *currentTile;
        Texture*     texture;

        // Set NodePath tags
        newNode.set_tag(tileType, "1");
        stream1 << x;
        newNode.set_tag("pos_x", stream1.str());
        stream2 << y;
        newNode.set_tag("pos_y", stream2.str());

        // Set Node transformations
        newNode.set_scale(_scale);
        newNode.set_hpr(0, 90, 0);
        newNode.set_pos(tile.position);

        // If the texture is defined in the tileset, apply it on the node
        if (tileId < tilesSrc.size())
        {
          texture = TexturePool::load_texture(TEXTURE_TILE_PATH + tilesSrc[tileId]);
          newNode.set_texture(texture);
        }

        // Add to corresponding group
        newNode.reparent_to(groups[y + (x / _groupSize)]);

        // Set the Tile object properly
        tile.nodePath  = newNode;
        tile.isDefined = true;
      }
      storage.push_back(tile);
    }
  }

  // Flatten each groups separatly
  std::for_each(groups.begin(), groups.end(), [fatherGroup](NodePath node)
  {
    node.flatten_strong();
    node.reparent_to(fatherGroup);
  });
}

void Tilemap::LoadWalls(Data wallset, Data map, bool horizontal)
{
  Data::iterator currentWall = map.begin();
  vector<string> wallsSrc;

  for_each(wallset.begin(), wallset.end(), [&wallsSrc](Data children) { wallsSrc.push_back(children.Value()); });

  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentWall)
    {
      unsigned int tileId  = *currentWall;

      if (tileId == 0) continue ;

      NodePath         newNode = _window->load_model(_window->get_render(), "models/wall.obj");
      Texture*         texture;

      newNode.set_scale(_scale);

      if (horizontal)
      {
        newNode.set_hpr(0, 90, 0);
        // (_scale * (2 / 2) => WorldScale * (half the size of the wall)
        newNode.set_pos(x * (_scale * TILE_UNIT) - (_scale * (TILE_UNIT / 2)), (_size.get_y() - y) * (_scale * TILE_UNIT), 0);
      }
      else
      {
        newNode.set_hpr(90, 90, 0);
        newNode.set_pos(x * (_scale * TILE_UNIT), ((_size.get_y() - y) * (_scale * TILE_UNIT)) + (_scale * (TILE_UNIT / 2)), 0);
      }

      if (--tileId < wallsSrc.size())
      {
        texture = TexturePool::load_texture(TEXTURE_WALL_PATH + wallsSrc[tileId]);
        newNode.set_texture(texture);
      }

      MapTile& tile = GetTile(x, y);

      (horizontal ? tile.hWall    : tile.vWall)    = newNode;
      (horizontal ? tile.hasHWall : tile.hasVWall) = true;

      newNode.reparent_to(tile.nodePath.get_parent());
    }
  }
}

void Tilemap::Load(Data data)
{
  Data tileset = data["tileset"];
  Data tilemap = data["tilemap"];

  //data.Output();
  _size.set_x(data["size_x"]);
  _size.set_y(data["size_y"]);

  if (!(data["group_size"].Nil()))
    _groupSize = data["group_size"];
  
  LoadTiles("tile",    tileset, data["tilemap"], LPoint3f(0, 0, 0),                        _groundNode,  _groundGroup,  _nodes);
  LoadTiles("ceiling", tileset, data["ceiling"], LPoint3f(0, 0, (_scale * TILE_UNIT * 2)), _ceilingNode, _ceilingGroup, _ceiling);

  LoadWalls(data["wallset"], data["hwalls"], true);
  LoadWalls(data["wallset"], data["vwalls"], false);
  cout << "Tilemap loading done" << endl;

  LoadPathfinding();
  cout << "Pathfinding loading done" << endl;
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

const Tilemap::CeilingTile& Tilemap::GetCeiling(unsigned int x, unsigned int y) const
{
  unsigned int it;

  it = (y * _size.get_x()) + x;
  if (it < _ceiling.size())
    return (_ceiling[it]);
  return (_ceiling[0]);
}

void Tilemap::LoadPathfinding()
{
  std::vector<Pathfinding::Node> pfNodes(_size.get_x() * _size.get_y());

  _pf = new Pathfinding(pfNodes, _size.get_x(), _size.get_y());
  for (unsigned int   x = 0 ; x < _size.get_x() ; ++x)
  {
    for (unsigned int y = 0 ; y < _size.get_y() ; ++y)
    {
      Pathfinding::Node& node = _pf->GetNode(x, y);
      MapTile            tile = GetTile(x, y);

      node.x = x;
      node.y = y;

      if (x != 0 && !tile.hasHWall) // To the left
        Pathfinding::ConnectNodes(node, _pf->GetNode(x - 1, y), 1.f);
      if (y != 0 && !tile.hasVWall) // To the top
        Pathfinding::ConnectNodes(node, _pf->GetNode(x, y - 1), 1.f);
      if ((x != 0) && (y != 0) &&
          (!tile.hasHWall) && (!tile.hasVWall) && (!(GetTile(x - 1, y).hasVWall))) // To the top and left
        Pathfinding::ConnectNodes(node, _pf->GetNode(x - 1, y - 1), 1.1f);
      if ((y != 0) && (x + 1 != _size.get_x()) && (!tile.hasVWall))// To the top and right
      {
        MapTile right    = GetTile(x + 1, y);

        if ((!right.hasHWall) && (!right.hasVWall))
          Pathfinding::ConnectNodes(node, _pf->GetNode(x + 1, y - 1), 1.1f);
      }
    }
  }
}

Pathfinding* Tilemap::SetupPathfinding(MapElement* pathseeker, int max_depth)
{
  if (!_pfCollisionSet)
  {
    std::for_each(_mapElements.begin(), _mapElements.end(), [this](MapElement* element)
    { element->ProcessCollision(_pf); });
    _pfCollisionSet = true;
  }
  pathseeker->UnprocessCollision(_pf);
  return (_pf);
}

void         Tilemap::SetdownPathfinding(MapElement* pathseeker, int max_depth) const
{
  pathseeker->ProcessCollision(_pf);
}

void Tilemap::ResetPathfinding(void)
{
  if (_pfCollisionSet)
  {
    std::for_each(_mapElements.begin(), _mapElements.end(), [this](MapElement* element)
    { element->UnprocessCollision(_pf); });
    _pfCollisionSet = false;
  }
}

//
// CEILING
//
void Tilemap::SetCeilingTransparent(float value)
{
  _ceilingNode.set_transparency(value == 1.f ? TransparencyAttrib::M_none : TransparencyAttrib::M_alpha);
  _ceilingNode.set_color(1, 1, 1, value);
}