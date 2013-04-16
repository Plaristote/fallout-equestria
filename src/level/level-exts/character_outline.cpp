#include "level/level.hpp"
#include <panda3d/depthTestAttrib.h>

using namespace std;

void Level::ToggleCharacterOutline(bool toggle)
{
  for_each(_characters.begin(), _characters.end(), [this, toggle](ObjectCharacter* character)
  {
    NodePath original = character->GetDynamicObject()->nodePath;
    NodePath outline;
    
    if (character == GetPlayer()) return ;
           if (toggle)
           {
             if (!(character->IsAlive())) return ;
           outline = original.copy_to(original);
    outline.set_name("outline");
    outline.set_texture_off();
    outline.set_light_off();
    outline.set_attrib(RenderModeAttrib::make(RenderModeAttrib::M_wireframe));
    outline.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
    outline.set_transparency(TransparencyAttrib::M_alpha);
    if (character->IsAlly(GetPlayer()))
    { outline.set_color(0, 255, 0, 0.5); }
    else
    { outline.set_color(255, 0, 0, 0.5); }
    outline.reparent_to(original);
    outline.set_pos(0, 0, 0);
    outline.set_hpr(0, 0, 0);
    
    NodePathCollection effects = outline.find_all_matches("graphical_effect");
    
    for(unsigned short i = 0 ; i < effects.get_num_paths() ; ++i)
      effects.get_path(i).detach_node();
           }
           else
           {
             outline = original.find("outline");
    if (!(outline.is_empty()))
    { outline.detach_node(); }
           }
  });
}