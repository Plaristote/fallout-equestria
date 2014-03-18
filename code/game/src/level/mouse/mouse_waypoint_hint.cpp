#include "level/mouse/mouse_waypoint_hint.hpp"
#include "world/map_object.hpp"
#include "world/world.h"

using namespace std;

MouseWaypointHint::MouseWaypointHint(Level& level, WindowFramework* window) : MouseEvents(level, window), window(window)
{
  light    = new PointLight("hovered_waypoint_hint");
  light->set_color(LColor(0, 1, 0, 1));
  light->set_attenuation(LPoint3f(1, 1, 0.3));
  nodepath = window->get_render().attach_new_node(light);
  nodepath.hide();
}

MouseWaypointHint::~MouseWaypointHint()
{
  if (!(waypoint_surface.is_empty()))
  {
    waypoint_surface.set_light_off(nodepath);
    waypoint_surface.clear_light(nodepath);
  }
  nodepath.remove_node();
}

void MouseWaypointHint::SetLightOnWaypointSurface(Waypoint* waypoint)
{
  if (!waypoint || waypoint->nodePath != waypoint_hovered)
  {
    NodePath new_surface;

    if (waypoint && !waypoint->parent_path.is_empty())
      new_surface = waypoint->parent_path;
    if (new_surface != waypoint_surface)
    {
      if (!(waypoint_surface.is_empty()))
      {
        waypoint_surface.set_light_off(nodepath);
        waypoint_surface.clear_light(nodepath);
      }
      if (!new_surface.is_empty())
      {
        waypoint_surface = waypoint->parent_path;
        waypoint_surface.set_light(nodepath, 99);
      }
      else
        waypoint_surface = NodePath();
    }
  }
}

void MouseWaypointHint::SetHoveredWaypoint(Waypoint* waypoint)
{
  SetLightOnWaypointSurface(waypoint);
  if (waypoint)
  {
    if (waypoint_hovered.is_empty())
      nodepath.set_pos(waypoint->nodePath.get_pos());
    waypoint_hovered = waypoint->nodePath;
    nodepath.show();
  }
  else
  {
    waypoint_hovered = NodePath();
    nodepath.hide();
  }
}

void MouseWaypointHint::Run(float elapsed_time)
{
  if (!waypoint_hovered.is_empty() && waypoint_hovered.get_pos() != nodepath.get_pos())
    MoveLightToWaypoint(elapsed_time);
  Mouse::Run();
}

void MouseWaypointHint::MoveLightToWaypoint(float elapsed_time)
{
  LPoint3f distance = nodepath.get_pos() - waypoint_hovered.get_pos();

  if (distance.get_x() < 1 && distance.get_y() < 1 && distance.get_z() < 1)
    nodepath.set_pos(waypoint_hovered.get_pos());
  else
    nodepath.set_pos(nodepath.get_pos() - (distance * (elapsed_time * 10)));
}
