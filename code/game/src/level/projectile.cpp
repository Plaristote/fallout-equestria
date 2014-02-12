#include "level/projectile.hpp"

using namespace std;

Projectile::Projectile(World* world, NodePath parent, NodePath target, Data data) : node_path(parent.attach_new_node("projectile")), target(target)
{
  Data light_data = data["light"];

  enlightened     = world->floors_node;
  if (data["model"].NotNil())
  {
    node_path.detach_node();
    node_path     = world->window->load_model(parent, data["model"].Value());
    node_path.look_at(target);
  }
  if (data["speed"].NotNil())
  {
    is_moving     = true;
    speed         = data["speed"];
  }
  else
    is_moving     = false;
  if (light_data.NotNil())
  {
    light         = new PointLight("projectile-light");
    light_node    = node_path.attach_new_node(light);
    enlightened.set_light(light_node, 8);
    SetAttenuation(0, 0, light_data["attenuation"] || 1);
    SetColor(light_data["red"]   || 255,
             light_data["green"] || 255,
             light_data["blue"]  || 255, 1);
  }
}

Projectile::~Projectile(void)
{
  enlightened.set_light_off(light_node);
  enlightened.clear_light(light_node);
  light_node.remove_node();
  node_path.remove_node();
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
  light->set_color(color);
}

bool Projectile::HasReachedDestination(void) const
{
  return (!is_moving || (node_path.get_pos() == target.get_pos(node_path.get_parent())));
}

bool Projectile::HasExpired(void) const
{
  if (is_moving && !(HasReachedDestination()))
    return (false);
  return (time_left <= 0);
}

void Projectile::Run(float elapsed_time)
{
  if (HasReachedDestination())
  {
    float factor;

    time_left -= elapsed_time;
    factor     = time_left / timeout;
    if (!(light.is_null()))
    {
      LColor current_color = color;

      current_color.set_x(color.get_x() * factor);
      current_color.set_y(color.get_y() * factor);
      current_color.set_z(color.get_z() * factor);
      light->set_color(current_color);
    }
  }
  else
  {
    LPoint3f objective    = target.get_pos(node_path.get_parent());
    LPoint3f position     = node_path.get_pos();
    LPoint3f distance     = position - objective;
    float    max_distance = max(max(abs(distance.get_x()), abs(distance.get_y())), abs(distance.get_z()));
    LPoint3f movement;

    if (max_distance <= speed)
    {
      node_path.set_pos(objective);
      node_path.hide();
      HitsTarget.Emit();
    }
    else
    {
      movement.set_x(-(speed * (distance.get_x() / max_distance)));
      movement.set_y(-(speed * (distance.get_y() / max_distance)));
      movement.set_z(-(speed * (distance.get_z() / max_distance)));
      node_path.set_pos(node_path.get_pos() + movement);
    }
  }
}
