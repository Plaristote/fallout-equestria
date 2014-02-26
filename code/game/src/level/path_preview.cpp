#include "level/path_preview.hpp"
#include "level/objectnode.hpp"
#include <panda3d/depthTestAttrib.h>

using namespace std;

PathPreview::~PathPreview()
{
  ClearDisplay();
  Hide();
}

void PathPreview::DisplayHoveredPath(InstanceDynamicObject* starting_point, Mouse& mouse)
{
  Waypoint* from             = starting_point->GetOccupiedWaypoint();
  Waypoint* hovered_waypoint = mouse.Hovering().waypoint_ptr;
  
  if (from && hovered_waypoint)
  {
    starting_point->UnprocessCollisions();
    DisplayPath(from, hovered_waypoint);
    starting_point->ProcessCollisions();
  }
  else
    Hide();
}

void PathPreview::DisplayPath(Waypoint* from, Waypoint* to)
{
  if (from != this->from || to != this->to)
    FindPath(from, to);
  if (Size() > 0)
  {
    CreateDisplay();
    Show();
  }
}

void PathPreview::Show()
{
  path_render.reparent_to(render);
  path_render.show();
}

void PathPreview::Hide()
{
  path_render.detach();
}

void PathPreview::CreateDisplay(void)
{
  ClearDisplay();
  ForeachWaypoint([this](Waypoint& waypoint)
  {
    NodePath nodepath = render.attach_new_node("path_preview_node");

    waypoint.nodePath.copy_to(nodepath);
    nodepath.reparent_to(render);
    nodepath.set_color(color);
    nodepath.set_light_off();
    nodepath.set_transparency(TransparencyAttrib::M_alpha);
    nodepath.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
    nodepath.show();
    path_render.add_path(nodepath);
  });
}

void PathPreview::ClearDisplay(void)
{
  while (path_render.size())
  {
    NodePath nodepath = path_render.get_path(0);
    
    path_render.remove_path(nodepath);
    nodepath.remove_node();
  }
}

void PathPreview::SetColor(LVector4f color)
{
  this->color = color;
  ForeachWaypoint([this, color](Waypoint& waypoint)
  {
    if (!(waypoint.nodePath.is_empty()))
      waypoint.nodePath.set_color(color);
  });
}
