#include "level/projectile.hpp"

using namespace std;

Projectile::Projectile(World* world, NodePath parent) : node_path(parent)
{
  enlightened = world->floors_node;
  light       = new PointLight("projectile-light");
  light_node  = node_path.attach_new_node(light);
  enlightened.set_light(light_node, 7);
  SetAttenuation(0, 0, 1);
  SetColor(255, 255, 255, 1);
}

Projectile::~Projectile(void)
{
  enlightened.set_light_off(light_node);
  enlightened.clear_light(light_node);
  light_node.remove_node();
}

void Projectile::SetTimeout(float timeout)
{
  this->timeout = timeout;
  time_left     = timeout;
}

void Projectile::SetAttenuation(float a, float b, float c)
{
  light->set_attenuation(LPoint3f(a, b, c));
}

void Projectile::SetColor(float red, float green, float blue, float alpha)
{
  color = LColor(red, green, blue, alpha);
}

void Projectile::Run(float elapsed_time)
{
  float factor;
  
  time_left -= elapsed_time;
  factor     = time_left / timeout;
  {
    LColor current_color = color;
    
    current_color.set_x(color.get_x() * factor);
    current_color.set_y(color.get_y() * factor);
    current_color.set_z(color.get_z() * factor);
    light->set_color(current_color);
  }
}
