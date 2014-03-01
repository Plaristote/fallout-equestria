#include "level/graphical_effect.hpp"
#include "level/objects/instance_dynamic_object.hpp"

using namespace std;

GraphicalEffect::GraphicalEffect(WindowFramework* window, Data data): AnimatedObject(window)
{
  SetModel    (data["model"]);
  SetColor    (data["color"]);
  SetPosition (data["position"]);
  SetRotation (data["rotation"]);
  SetScale    (data["scale"]);
  SetAnimation(data["animation"]);
}

GraphicalEffect::~GraphicalEffect()
{
  if (!root.is_empty())
    root.remove_node();
}

void GraphicalEffect::SetOnObject(InstanceDynamicObject* object)
{
  root.reparent_to(object->GetNodePath());
}


void GraphicalEffect::SetModel(Data model)
{
  std::string path = MODEL_ROOT + model.Value();
  
  SetModelNameFromPath(path);
  root = _window->load_model(_window->get_panda_framework()->get_models(), path);
}

void GraphicalEffect::SetTexture(Data texture)
{
  if (!root.is_empty() && texture.NotNil())
  {
  }
}

void GraphicalEffect::SetAnimation(Data animation)
{
  if (!root.is_empty() && animation["name"].NotNil())
  {
    PlayAnimation(animation["name"].Value(), animation["loop"].Value() == "1");
  }
}

void GraphicalEffect::SetColor(Data color)
{
  if (!root.is_empty() && color.NotNil())
  {
    float red   = color["red"];
    float green = color["green"];
    float blue  = color["blue"];
    
    if (color["alpha"].NotNil())
    {
      root.set_transparency(TransparencyAttrib::M_alpha);
      root.set_color(red, green, blue, color["alpha"]);
    }
    else
      root.set_color(red, green, blue);
  }
}

void GraphicalEffect::SetPosition(Data position)
{
  if (!root.is_empty() && position.NotNil())
  {
    LPoint3f current_position = root.get_pos();
    
    if (position["x"].NotNil())
      current_position.set_x(position["x"]);
    if (position["y"].NotNil())
      current_position.set_y(position["y"]);
    if (position["x"].NotNil())
      current_position.set_z(position["z"]);
    root.set_pos(current_position);
  }
}

void GraphicalEffect::SetRotation(Data rotation)
{
  if (!root.is_empty() && rotation.NotNil())
  {
    LPoint3f current_rotation = root.get_hpr();

    if (rotation["x"].NotNil())
      current_rotation.set_x(rotation["x"]);
    if (rotation["y"].NotNil())
      current_rotation.set_y(rotation["y"]);
    if (rotation["x"].NotNil())
      current_rotation.set_z(rotation["z"]);
    root.set_hpr(current_rotation);
  }
}

void GraphicalEffect::SetScale(Data scale)
{
  if (!root.is_empty() && scale.NotNil())
  {
    if (scale["x"].NotNil() && scale["y"].NotNil() && scale["z"].NotNil())
      root.set_scale(scale["x"], scale["y"], scale["x"]);
    else
      root.set_scale((float)scale);
  }
}