#ifndef  CHATTER_MANAGER_HPP
# define CHATTER_MANAGER_HPP

# include <panda3d/fontPool.h>
# include "level/objects/instance_dynamic_object.hpp"

class ChatterManager
{
  struct Text
  {
    NodePath parent;
    NodePath text_node;
    float    timeout;
  };
  
  typedef std::list<Text> TextBoxes;
public:
  ChatterManager(WindowFramework* window);

  void PushTextBox(InstanceDynamicObject* object, const std::string& text, unsigned short r, unsigned short g, unsigned short b, float timeout = 5.f);  
  void Run(float elapsed_time, NodePath camera);
  
private:
  static void SetTextOffset(NodePath parent, NodePath node);
  
  WindowFramework* window;
  PT(TextFont)     font;
  TextBoxes        text_boxes;
};

#endif
