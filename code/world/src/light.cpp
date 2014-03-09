#include "world/world.h"
#include "world/light.hpp"
#include "world/map_object.hpp"
#include "world/dynamic_object.hpp"

using namespace std;

extern unsigned int blob_revision;

void WorldLight::Initialize(void)
{
  switch (type)
  {
    default:
    case Point:
    {
      PT(PointLight) pLight = new PointLight(name);

      pLight->set_shadow_caster(true, 12, 12);
      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Directional:
    {
      PT(DirectionalLight) pLight = new DirectionalLight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Ambient:
    {
      PT(AmbientLight) pLight = new AmbientLight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
    case Spot:
    {
      PT(Spotlight) pLight = new Spotlight(name);

      light    = pLight;
      nodePath = parent.attach_new_node(pLight);
    }
      break ;
  }
#ifdef GAME_EDITOR
/*  if (!(World::model_sphere.is_empty()))
    World::model_sphere.instance_to(symbol);
  else
    cout << "The horror ! Model spehre is unavailable" << endl;*/
#endif
}

void WorldLight::ReparentTo(World* world)
{
  parent_type = Type_None;
  parent_i    = 0;
  nodePath.reparent_to(world->rootLights);
#ifdef GAME_EDITOR
  //symbol.reparent_to(world->lightSymbols);
  nodePath.reparent_to(world->rootLights);
#endif
}

void WorldLight::ReparentTo(DynamicObject* object)
{
  ReparentTo((MapObject*)object);
  parent_type = Type_DynamicObject;
}

void WorldLight::ReparentTo(MapObject* object)
{
  parent_type = Type_MapObject;
  parent      = object->nodePath;
  parent_i    = object;
  nodePath.reparent_to(parent);
#ifdef GAME_EDITOR
  symbol.reparent_to(parent);
#endif
}


void WorldLight::SetEnabled(bool set_enabled)
{
  std::function<void (NodePath)> set_light;
  std::function<void (NodePath)> unset_light;

  enabled       = set_enabled;
  set_light     = [this](NodePath object) { object.set_light(nodePath); };
  unset_light   = [this](NodePath object) { object.set_light_off(nodePath); object.clear_light(nodePath); };
  for_each(enlightened.begin(), enlightened.end(), enabled ? set_light : unset_light);
}

void WorldLight::Destroy(void)
{
  SetEnabled(false);
  nodePath.detach_node();
#ifdef GAME_EDITOR
  symbol.detach_node();
#endif
}

/*
 * Serialization
 */
void WorldLight::Unserialize(Utils::Packet& packet)
{
  World*    world = World::LoadingWorld;
  float     r, g, b, a;
  float     pos_x, pos_y, pos_z;
  float     hpr_x, hpr_y, hpr_z;
  string    parent_name;
  char      tmp_enabled, _type, _ptype;

  parent = world->window->get_render();
  packet >> name >> tmp_enabled >> zoneSize;
  packet >> _type >> _ptype;
  cout << "[World] Loading light " << name << endl;
  enabled     = tmp_enabled != 0;
  type        = (WorldLight::Type)_type;
  parent_type = (WorldLight::ParentType)_ptype;
  if (parent_type != Type_None)
    packet >> parent_name;
  packet >> r >> g >> b >> a;
  packet >> pos_x >> pos_y >> pos_z;
  packet >> hpr_x >> hpr_y >> hpr_z;
  switch (parent_type)
  {
    case Type_MapObject:
      ReparentTo(world->GetMapObjectFromName(parent_name));
      break ;
    case Type_DynamicObject:
      ReparentTo(world->GetDynamicObjectFromName(parent_name));
      break ;
    case Type_None:
      parent   = world->rootLights;
      parent_i = 0;
      break ;
  }
  cout << "Light type = " << (int)type << endl;
  Initialize();
  if (blob_revision >= 3)
  {
    float     attenuation[3];

    packet >> attenuation[0] >> attenuation[1] >> attenuation[2];
    SetAttenuation(attenuation[0], attenuation[1], attenuation[2]);
  }
  SetColor(r, g, b, a);
  if (!(nodePath.is_empty()))
  {
    nodePath.set_pos(LVecBase3(pos_x, pos_y, pos_z));
    nodePath.set_hpr(LVecBase3(hpr_x, hpr_y, hpr_z));
  }
#ifdef GAME_EDITOR
  if (!(symbol.is_empty()))
    symbol.reparent_to(world->lightSymbols);
#endif
}

void WorldLight::Serialize(Utils::Packet& packet) const
{
  LColor     color  = light->get_color();
  char       _type  = type;
  char       _ptype = parent_type;
  LVecBase3f attenuation = GetAttenuation();

  packet << name << (char)enabled << zoneSize << _type << _ptype;
  if (parent_i)
    packet << parent_i->nodePath.get_name();
  packet << (float)color.get_x() << (float)color.get_y() << (float)color.get_z() << (float)color.get_w();
  packet << (float)nodePath.get_x() << (float)nodePath.get_y() << (float)nodePath.get_z();
  packet << (float)nodePath.get_hpr().get_x() << (float)nodePath.get_hpr().get_y() << (float)nodePath.get_hpr().get_z();
  packet << (float)attenuation.get_x() << (float)attenuation.get_y() << (float)attenuation.get_z();
}
