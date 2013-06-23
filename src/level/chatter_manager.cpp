#include "level/chatter_manager.hpp"

LPoint3 NodePathSize(NodePath np);

using namespace std;

ChatterManager::ChatterManager(WindowFramework* window) : window(window)
{
  font = FontPool::load_font("data/fallout.ttf");
}

void ChatterManager::SetTextOffset(NodePath parent, NodePath node)
{
  LPoint3f size      = NodePathSize(parent);
  LPoint3f self_size = NodePathSize(node);
  LPoint3f pos       = parent.get_pos();

  pos.set_z(pos.get_z() + size.get_z() + (self_size.get_z()));
  node.set_pos(pos);
}

void ChatterManager::PushTextBox(InstanceDynamicObject* object, const string& text, unsigned short r, unsigned short g, unsigned short b, float timeout)
{
  PT(TextNode) text_node = new TextNode("textnode");
  
  text_node->set_font(font);
  text_node->set_wordwrap(15);
  text_node->set_text(text);
  text_node->set_align(TextNode::A_center);
  text_node->set_frame_as_margin(0.2, 0.2, 0.1, 0.1);
  text_node->set_frame_color(0.25, 0.50, 1, 0.35);
  text_node->set_card_color(0, 0, 0, 0.35);
  text_node->set_card_decal(true);
  text_node->set_card_as_margin(0, 0, 0, 0);
  {
    Text text_box;

    text_box.parent    = object->GetNodePath();
    text_box.text_node = window->get_render().attach_new_node(text_node);
    text_box.timeout   = timeout;
    text_box.text_node.set_color(r, g, b);
    SetTextOffset(text_box.parent, text_box.text_node);
    text_box.text_node.set_scale(2.f);
    text_boxes.push_back(text_box);
  }
}

void ChatterManager::Run(float elapsed_time)
{
  auto it = text_boxes.begin();
  
  while (it != text_boxes.end())
  {
    it->timeout -= elapsed_time;
    // Fading out during the last second
    if (it->timeout <= 1)
    {
      it->text_node.set_transparency(TransparencyAttrib::M_alpha);
      it->text_node.set_alpha_scale(it->timeout);      
    }
    // Removing the text item when they timeout
    if (it->timeout <= 0)
    {
      it->text_node.remove_node();
      it = text_boxes.erase(it);
    }
    // Updating the position of still running text boxes
    else
    {
      SetTextOffset(it->parent, it->text_node);
      ++it;
    }
  }
}