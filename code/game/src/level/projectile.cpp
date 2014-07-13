#include "level/projectile.hpp"

using namespace std;

Projectile::Projectile(World* world, NodePath parent, NodePath target, Data data) : node_path(parent.attach_new_node("projectile")), target(target)
{
  Data light_data = data["light"];

  //enlightened     = world->floors_node;
  enlightened     = world->window->get_render();
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
    light            = new PointLight("projectile-light");
    light_node       = node_path.attach_new_node(light);
    base_attenuation = light_data["attenuation"].Or(1);
    enlightened.set_light(light_node, 7);
    SetAttenuation(0, 0, base_attenuation);
    SetColor(light_data["red"]   || 255,
             light_data["green"] || 255,
             light_data["blue"]  || 255, 1);
  }
  SetTimeout(data["timeout"].Or(1));
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
  if (!(light.is_null()))
    light->set_attenuation(LPoint3f(a, b, c));
}

void Projectile::SetColor(float red, float green, float blue, float alpha)
{
  if (!(light.is_null()))
  {
    color = LColor(red, green, blue, alpha);
    light->set_color(color);
  }
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
    float factor, attenuation;

    time_left -= elapsed_time;
    factor     = time_left / timeout;
    attenuation = base_attenuation / factor;
    SetAttenuation(0, 0, (attenuation < 0 ? 1000 : attenuation));
  }
  else
  {
    LPoint3f objective    = target.get_pos(node_path.get_parent());
    LPoint3f position     = node_path.get_pos();
    LPoint3f distance     = position - objective;
    float    max_distance = max(max(abs((long)distance.get_x()), abs((long)distance.get_y())), abs((long)distance.get_z()));
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

void Projectile::Set::Run(float elapsed_time)
{
  auto it   = begin();

  while (it != end())
  {
    Projectile* projectile = *it;
    
    if (projectile->HasExpired())
    {
      delete projectile;
      it = erase(it);
    }
    else
    {
      projectile->Run(elapsed_time);
      ++it;
    }
  }
}

void Projectile::Set::CleanUp(void)
{
  auto it = begin();
  
  while (it != end())
  {
    delete *it;
    it = erase(it);
  }
}
