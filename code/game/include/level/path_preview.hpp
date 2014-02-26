#ifndef  PATH_PREVIEW_HPP
# define PATH_PREVIEW_HPP

# include "globals.hpp"
# include "level/pathfinding.hpp"
# include "level/mouse.hpp"
# include <panda3d/nodePathCollection.h>

class InstanceDynamicObject;

class PathPreview : public Pathfinding::Path
{
public:
  ~PathPreview();
  
  void               SetColor(LVector4f color);
  void               SetRenderNode(NodePath render) { this->render = render; }

  void               DisplayPath(Waypoint* from, Waypoint* to);
  void               DisplayHoveredPath(InstanceDynamicObject* from_object, Mouse& mouse);

  void               Show(void);
  void               Hide(void);
  void               CreateDisplay(void);
  void               ClearDisplay(void);
  
private:
  NodePath           render;
  NodePathCollection path_render;
  LVector4f          color;
};

#endif