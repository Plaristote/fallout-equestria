#include "level/level.hpp"
#include <panda3d/depthTestAttrib.h>

using namespace std;

void Level::DestroyCombatPath(void)
{
  for_each(_combat_path.begin(), _combat_path.end(), [](Waypoint& wp) { wp.nodePath.detach_node(); });
}

void Level::DisplayCombatPath(void)
{
  _last_combat_path = _mouse.Hovering().waypoint;
  DestroyCombatPath();
  if (*_itCharacter != GetPlayer())
    return ;
  _combat_path = GetPlayer()->GetPath(_world->GetWaypointFromNodePath(_mouse.Hovering().waypoint));
  for_each(_combat_path.begin(), _combat_path.end(), [this](Waypoint& wp)
  {
    NodePath sphere = _window->get_render().attach_new_node("combat_path_node");
    
    wp.nodePath.copy_to(sphere);
    wp.nodePath = sphere;
    wp.nodePath.set_transparency(TransparencyAttrib::M_alpha);
    wp.nodePath.set_color(0.5, 0.5, 0.5, 0.5);
    wp.nodePath.set_light_off();
    wp.nodePath.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
  });
}