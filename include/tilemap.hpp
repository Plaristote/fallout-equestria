#ifndef  TILEMAP_HPP
# define TILEMAP_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include "datatree.hpp"
# include "pathfinding.hpp"
# include "mapelement.hpp"
# include <algorithm>

# define WORLD_SCALE        1.f
# define DEFAULT_GROUP_SIZE 45
# define TILE_UNIT          8
# define CEILING_HEIGHT     10

enum MyCollisionMask
{
  Object    = 2,
  Tiles     = 4,
  Walls     = 8
};

class Tilemap
{
public:
  struct Tile
  {
    Tile(NodePath nodePath) : nodePath(nodePath), isDefined(true) {}
    Tile(void)              : isDefined(false)                    {}
    
    bool     isDefined;
    NodePath nodePath;
    LPoint3  position;
  };
  
  struct MapTile : public Tile
  {
    MapTile(NodePath nodePath) : Tile(nodePath) { hasHWall = false; hasVWall = false; }
    MapTile(void)              : Tile()         { hasHWall = false; hasVWall = false; }
    MapTile(const Tile& tile)  : Tile(tile)     { hasHWall = false; hasVWall = false; }

    void                operator=(const NodePath& toSet) { nodePath = toSet; hasHWall = false; hasVWall = false; }

    operator NodePath() const { return (nodePath); }

    NodePath            hWall, vWall;
    bool                hasHWall, hasVWall;
  };

  struct CeilingTile : public Tile
  {
    CeilingTile(NodePath nodePath) : Tile(nodePath) {}
    CeilingTile(void)              : Tile()         {}
    CeilingTile(const Tile& tile)  : Tile(tile)     {}
  };
  
  Tilemap(WindowFramework* window);
  ~Tilemap();

  void                          Load(Data data);

  LPoint2                       GetSize(void) const { return (_size); }
  const MapTile&                GetTile(unsigned int x, unsigned int y) const;
  MapTile&                      GetTile(unsigned int x, unsigned int y);
  const CeilingTile&            GetCeiling(unsigned int x, unsigned int y) const;
  void                          AddMapElement(MapElement* e) { _mapElements.push_back(e); }
  void                          DelMapElement(MapElement* e)
  {
    std::list<MapElement*>::iterator it = std::find(_mapElements.begin(), _mapElements.end(), e);

    if (it != _mapElements.end())
    {
      _mapElements.erase(it);
      e->UnprocessCollision(_pf);
    }
  }

  NodePath                      GetCeilingNode(void) const { return (_ceilingNode); }
  void                          SetCeilingTransparent(float value);

  Pathfinding*                  SetupPathfinding(MapElement*, int max_depth = 0);
  void                          SetdownPathfinding(MapElement*, int max_depth = 0) const;
  void                          ResetPathfinding(void);

private:
  template<class NodeType> void LoadTiles(string tileType, Data map, LPoint3 posModificator, NodePath fatherGroup, vector<NodePath>& groups, vector<NodeType>& storage);
  void                          LoadWalls(Data map);
  void                          LoadPathfinding(void);
  
  float                         _scale;
  unsigned                     _groupSize;
  WindowFramework*             _window;
  std::vector<MapTile>         _nodes;
  std::vector<CeilingTile>     _ceiling;

  // Used for grouping tiles before flattening them
  std::vector<NodePath>        _ceilingGroup;
  std::vector<NodePath>        _groundGroup;

  // Groups every ceiling group
  NodePath                     _ceilingNode;
  NodePath                     _groundNode;

  LPoint2                      _size;
  std::list<MapElement*>       _mapElements;

  // Pathfinding Context
  Pathfinding*                 _pf;
  bool                         _pfCollisionSet;

	//Holds texture map, for all the tiles inside this tilemap
	PT(Texture)				_texmap;

	//Stores all the models needed for the entire tilemap
	//(such that they are loaded from the disk only ONCE)
	std::vector<NodePath>		_modelArchive;
};

#endif