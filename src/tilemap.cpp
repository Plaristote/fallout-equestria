#include "globals.hpp"
#include "tilemap.hpp"

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
#include <panda3d/geomVertexFormat.h>
template<class NodeType>
void Tilemap::LoadTiles(string tileType, Data map, LPoint3 posModificator, NodePath fatherGroup, vector<NodePath>& groups, vector<NodeType>& storage)
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

  // Loading Tiles
  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentTile)
    {
	  unsigned int tileId  = *currentTile;
      Tile tile;

      tile.position.set_x(x * (_scale * TILE_UNIT) + posModificator.get_x());
      tile.position.set_y(y * (_scale * TILE_UNIT) + posModificator.get_y());
      tile.position.set_z(posModificator.get_z());

      // The tile is defined
      if ((*currentTile).Value() != "-")
      {
        stringstream stream1, stream2;
		stream1 << x;
		stream2 << y;
        NodePath     newNode= NodePath("Tile" + stream1.str() + stream2.str());

		if (tileId <= _modelArchive.size()) {
			_modelArchive[tileId].instance_to(newNode);
			newNode.set_texture(_texmap);
		};

        // Set NodePath tags
        newNode.set_tag(tileType, "1");
        newNode.set_tag("pos_x", stream1.str());
        newNode.set_tag("pos_y", stream2.str());

        // Collision Mask
        newNode.set_collide_mask(CollideMask(Tiles));

        // Set Node transformations
        newNode.set_scale(_scale);
        newNode.set_pos(tile.position);

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

void Tilemap::LoadWalls(Data map)
{
	Data::iterator currentWall = map.begin();
	for (unsigned int   y = 0 ; y < _size.get_y() ; ++y) {
		for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentWall) {

			unsigned int tileId  = *currentWall;
			if (tileId == 0)
				continue;

			stringstream stream1, stream2;
			stream1 << x;
			stream2 << y;
			NodePath         newNode= NodePath("Wall" + stream1.str() + stream2.str());
			MapTile& tile = GetTile(x, y);

			if (tileId <= _modelArchive.size()) {
				_modelArchive[tileId].instance_to(newNode);
				newNode.reparent_to(_groundGroup[y + (x / _groupSize)]);
				newNode.set_texture(_texmap);
			}
			//if (horizontal)
			//{
			//  //newNode.set_hpr(0, 90, 0);
			//  // (_scale * (2 / 2) => WorldScale * (half the size of the wall)
			//  newNode.set_pos(x * (_scale * TILE_UNIT) - (_scale * (TILE_UNIT / 2)), /*(_size.get_y() - y)*/y * (_scale * TILE_UNIT), 0);
			//} else {
			//  //newNode.set_hpr(90, 90, 0);
			//  //newNode.set_pos(x * (_scale * TILE_UNIT),/* ((_size.get_y() - y)*/y * (_scale * TILE_UNIT)) + (_scale * (TILE_UNIT / 2)), 0);
			//	  newNode.set_pos(x * (_scale * TILE_UNIT), y * (_scale * TILE_UNIT) , 0);
			//}
			newNode.set_scale(_scale);
			newNode.set_tag("wall", "1");
			// Collision Mask
			newNode.set_collide_mask(CollideMask(Walls));
			newNode.set_pos(tile.position);
			cout << newNode.get_pos() << endl;

			/*(horizontal ? tile.hWall    : tile.vWall)    = newNode;
			(horizontal ? tile.hasHWall : tile.hasVWall) = true;*/
			tile.hWall = newNode;
			tile.vWall = newNode;
			tile.hasHWall = true;
			tile.hasVWall = true;

			newNode.reparent_to(tile.nodePath.get_parent());
		}
	}
}

void Tilemap::Load(Data data)
{
  Data tileset = data["tileset"];

  //data.Output();
  _size.set_x(data["size_x"]);
  _size.set_y(data["size_y"]);

  
  for(Data::iterator i= tileset.begin(); i!=tileset.end(); ++i) {
	  cout << tileset.Value() << " |||LOADING: " << MODEL_TILE_PATH + (*i).Value() << endl;
	  _modelArchive.push_back( _window->load_model(_window->get_panda_framework()->get_models(),MODEL_TILE_PATH + (*i).Value() ) );
  };

  //Load the texture archive (one per tilemap!)
  _texmap= TexturePool::load_texture(TEXTURE_TILE_PATH + data["texmap"].Value());
  //TODO: throw, if texture bad!

  if (!(data["group_size"].Nil()))
    _groupSize = data["group_size"];
  
  LoadTiles("tile",   data["tilemap"], LPoint3f(0, 0, 0),                         _groundNode,  _groundGroup,  _nodes);
  LoadTiles("ceiling",data["ceiling"], LPoint3f(0, 0, (_scale * CEILING_HEIGHT)), _ceilingNode, _ceilingGroup, _ceiling);
  
  /*LoadWalls(data["tileset"], data["hwalls"], true);
  LoadWalls(data["tileset"], data["vwalls"], false);*/
  LoadWalls(data["walls"]);
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