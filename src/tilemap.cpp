#include "globals.hpp"
#include "tilemap.hpp"
#include "timer.hpp"

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
	Timer t2= Timer();
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
  t2.Profile("Group loading");

  // Loading Tiles
  for (unsigned int   y = 0 ; y < _size.get_y() ; ++y)
  {
    for (unsigned int x = 0 ; x < _size.get_x() ; ++x, ++currentTile)
    {
	  unsigned int tileId  = *currentTile;
      Tile tile;

	  tile.rotation= 0; //For the time being!!! TODO TODO
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

		//Load information from the archive, and store a pointer to the Prototype
		if (tileId <= _tileArchive.size()) {
			tile.SetPrototype(&_tileArchive[tileId]);
			_tileArchive[tileId].model.instance_to(newNode);
			newNode.set_texture(_texmap);
		} else
			throw "Tile not in archive!";

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
  t2.Profile("Tile loading");

  // Flatten each groups separatly
  std::for_each(groups.begin(), groups.end(), [fatherGroup](NodePath node)
  {
    node.flatten_strong();
    node.reparent_to(fatherGroup);
  });
  t2.Profile("Flattening");
}

void Tilemap::Load(Data data)
{
	//Start the profiling timer
	Timer t1= Timer();

  Data tileset = data["tileset"];

  //data.Output();
  _size.set_x(data["size_x"]);
  _size.set_y(data["size_y"]);
  if (!data["group_size"].Nil())
	_groupSize = data["group_size"];

  //Load our tile prototypes
  //TODO: Place all the tile+texmap declarations into a SEPARATE FILE!!! (independent from the map file)
  for(Data::iterator i= tileset.begin(); i!=tileset.end(); ++i) {
	  TilePrototype tp;
	  tp.model= _window->load_model(_window->get_panda_framework()->get_models(),MODEL_TILE_PATH + (*i)["model"].Value() );
	  //Sequentially load the access points
	  int k= 0;
	  for(Data::iterator j= (*i)["access"].begin(); j!=(*i)["access"].end(); ++j, k++)
		  tp.access[k]= (*j);
	  _tileArchive.push_back( tp );
  };

  //Load the texture archive (one per tilemap!)
  _texmap= TexturePool::load_texture(TEXTURE_TILE_PATH + data["texmap"].Value());
  //TODO: throw, if texture bad!

  t1.Profile("Archive loading");
  
  LoadTiles("tile",   data["maptile"], LPoint3f(0, 0, 0),                         _groundNode,  _groundGroup,  _nodes);
  t1.Profile("Tile loading");

  LoadTiles("ceiling",data["ceiling"], LPoint3f(0, 0, (_scale * CEILING_HEIGHT)), _ceilingNode, _ceilingGroup, _ceiling);
  t1.Profile("Ceiling loading");
  cout << "Tilemap loading done" << endl;

  LoadPathfinding();
  t1.Profile("Pathfinding loading");
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

	  //Horizontal+Vertical traverse
	  if (x != 0 && tile.Access(TilePrototype::left))
		  if (GetTile(x-1,y  ).Access(TilePrototype::right))
			Pathfinding::ConnectNodes(node, _pf->GetNode(x-1, y  ), 1.0f);

	  if (y != 0 && tile.Access(TilePrototype::up))
		  if (GetTile(x,y-1).Access(TilePrototype::down))
			Pathfinding::ConnectNodes(node, _pf->GetNode(x  , y-1), 1.0f);

	  //Diagonal traverse
	  //Ensures that characters don't take shortcuts across diagonals when there a solid column/etc. in the way
	  if (x != 0 && y != 0 && tile.Access(TilePrototype::upleft))
		  if (GetTile(x-1,y-1).Access(TilePrototype::downright))
			  if (GetTile(x  ,y-1).Access(TilePrototype::downleft))
				  if (GetTile(x-1,y  ).Access(TilePrototype::upright))
					Pathfinding::ConnectNodes(node, _pf->GetNode(x-1, y-1), 1.4f);

	  if (y != 0 && tile.Access(TilePrototype::upright))
		  if (GetTile(x+1,y-1).Access(TilePrototype::downleft))
			  if (GetTile(x  ,y-1).Access(TilePrototype::upleft))
				  if (GetTile(x+1,y  ).Access(TilePrototype::downright))
					Pathfinding::ConnectNodes(node, _pf->GetNode(x+1, y-1), 1.4f);

	  //if (x != 0 && y != 0 && tile.Access(TilePrototype::upleft))
		 // if (GetTile(x-1,y-1).Access(TilePrototype::downright))
			//Pathfinding::ConnectNodes(node, _pf->GetNode(x-1, y-1), 1.4f);

	  //if (y != 0 && tile.Access(TilePrototype::upright))
		 // if (GetTile(x+1,y-1).Access(TilePrototype::downleft))
			//Pathfinding::ConnectNodes(node, _pf->GetNode(x+1, y-1), 1.4f);
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