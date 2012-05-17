#ifndef  TILEMAP_HPP
# define TILEMAP_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include "datatree.hpp"
# include "pathfinding.hpp"
# include "mapelement.hpp"
# include <algorithm>

# define WORLD_SCALE        2.f
# define DEFAULT_GROUP_SIZE 45
# define TILE_UNIT          2

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

  void                  Load(Data data);

  LPoint2               GetSize(void) const { return (_size); }
  const MapTile&        GetTile(unsigned int x, unsigned int y) const;
  MapTile&              GetTile(unsigned int x, unsigned int y);
  const CeilingTile&    GetCeiling(unsigned int x, unsigned int y) const;
  void                  AddMapElement(MapElement* e) { _mapElements.push_back(e); }
  void                  DelMapElement(MapElement* e)
  {
    std::list<MapElement*>::iterator it = std::find(_mapElements.begin(), _mapElements.end(), e);

    if (it != _mapElements.end())
      _mapElements.erase(it);
  }

  NodePath              GetCeilingNode(void) const { return (_ceilingNode); }
  void                  SetCeilingTransparent(float value);
  Pathfinding*          GeneratePathfinding(MapElement*, int max_depth = 0) const;

private:
  template<class NodeType>
  void                  LoadTiles(string tileType, Data tileset, Data map, LPoint3 posModificator, NodePath fatherGroup, vector<NodePath>& groups, vector<NodeType>& storage);

  void                  LoadWalls(Data wallset, Data map, bool horizontal);
  
  float                    _scale;
  unsigned                 _groupSize;
  WindowFramework*         _window;
  std::vector<MapTile>     _nodes;
  std::vector<CeilingTile> _ceiling;

  // Used for grouping tiles before flattening them
  std::vector<NodePath>    _ceilingGroup;
  std::vector<NodePath>    _groundGroup;

  // Groups every ceiling group
  NodePath                 _ceilingNode;
  NodePath                 _groundNode;

  LPoint2                  _size;
  std::list<MapElement*>   _mapElements;
};

#endif