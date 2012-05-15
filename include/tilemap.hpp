#ifndef  TILEMAP_HPP
# define TILEMAP_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include "datatree.hpp"
# include "pathfinding.hpp"
# include "mapelement.hpp"
# include <algorithm>

# define WORLD_SCALE 2.f

class Tilemap
{
public:
  struct MapTile
  {
    MapTile(NodePath nodePath) : nodePath(nodePath) { hasHWall = false; hasVWall = false; }
    void                operator=(const NodePath& toSet) { nodePath = toSet; hasHWall = false; hasVWall = false; }
    operator NodePath() const { return (nodePath); }

    NodePath            nodePath;
    NodePath            hWall, vWall;
    bool                hasHWall, hasVWall;
  };

  struct CeilingTile
  {
    CeilingTile(NodePath nodePath) : isCeiling(true), nodePath(nodePath) {}
    CeilingTile(void)              : isCeiling(false)                    {}

    bool                    isCeiling;
    NodePath                nodePath;
  };
  
  Tilemap(WindowFramework* window);

  void                  Load(Data data);

  LPoint2               GetSize(void) const { return (_size); }
  const MapTile&        GetTile(unsigned int x, unsigned int y) const;
  MapTile&              GetTile(unsigned int x, unsigned int y);
  CeilingTile&          GetCeiling(unsigned int x, unsigned int y);
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
  float                    _scale;
  WindowFramework*         _window;
  std::vector<MapTile>     _nodes;
  std::vector<CeilingTile> _ceiling;
  NodePath                 _ceilingNode;
  LPoint2                  _size;
  std::list<MapElement*>   _mapElements;
};

#endif