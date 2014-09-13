#include "world/world.h"
#include "world/light.hpp"
#include "world/map_object.hpp"
#include "world/dynamic_object.hpp"
#ifdef GAME_EDITOR
# include "qpandaapplication.h"
#endif

using namespace std;

extern unsigned int blob_revision;
string get_nodepath_path(NodePath);

void WorldLight::SetColor(float r, float g, float b, float a)
{
  LColor color(r, g, b, a);

  light->set_color(color);
}

LVecBase3f WorldLight::GetAttenuation(void) const
{
  switch (type)
  {
    case Point:
    {
      PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

      return (point_light->get_attenuation());
    }
    case Spot:
    {
      PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

      return (spot_light->get_attenuation());
    }
    default:
        break;
  }
  return (LVecBase3f(0, 0, 0));
}

void WorldLight::SetAttenuation(float a, float b, float c)
{
  switch (type)
  {
    case Point:
    {
      PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

      point_light->set_attenuation(LVecBase3(a, b, c));
      break ;
    }
    case Spot:
    {
      PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

      spot_light->set_attenuation(LVecBase3(a, b, c));
      break ;
    }
    default:
      break ;
  }
}

void WorldLight::SetFrustumVisible(bool set_visible)
{
  PT(Camera) camera;

  switch (type)
  {
    case Point:
    {
      PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

      camera = point_light;
      break ;
    }
    case Directional:
    {
      PT(DirectionalLight) directional_light = reinterpret_cast<DirectionalLight*>(light.p());

      camera = directional_light;
      break ;
    }
    case Spot:
    {
      PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

      camera = spot_light;
      break ;
    }
    case Ambient:
      break ;
  }
  if (!(camera.is_null()))
  {
    if (set_visible)
      camera->show_frustum();
    else
      camera->hide_frustum();
  }
}

void WorldLight::Initialize(void)
{
  switch (type)
  {
    default:
    case Point:
    {
      PT(PointLight) pLight = new PointLight(name);

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
  InitializeShadowCaster();
#ifdef GAME_EDITOR
  {
    WindowFramework* window = QPandaApplication::Framework().get_window(0);

    symbol = window->load_model(QPandaApplication::Framework().get_models(), std::string(MODEL_ROOT) + "misc/sphere.egg.pz");
    symbol.reparent_to(nodePath);
  }
#endif
}

void WorldLight::InitializeShadowCaster()
{
  switch (type)
  {
    case Point:
    {
      PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

      point_light->set_shadow_caster(true, shadow_settings.buffer_size[0], shadow_settings.buffer_size[1]);
      point_light->get_lens()->set_near_far(shadow_settings.distance_near, shadow_settings.distance_far);
      point_light->get_lens()->set_film_size(shadow_settings.film_size);
    }
    case Directional:
    {
      PT(DirectionalLight) directional_light = reinterpret_cast<DirectionalLight*>(light.p());

      directional_light->set_shadow_caster(true, shadow_settings.buffer_size[0], shadow_settings.buffer_size[1]);
      directional_light->get_lens()->set_near_far(shadow_settings.distance_near, shadow_settings.distance_far);
      directional_light->get_lens()->set_film_size(shadow_settings.film_size);
    }
    case Spot:
    {
      PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

      spot_light->set_shadow_caster(true, shadow_settings.buffer_size[0], shadow_settings.buffer_size[1]);
      spot_light->get_lens()->set_near_far(shadow_settings.distance_near, shadow_settings.distance_far);
      spot_light->get_lens()->set_film_size(shadow_settings.film_size);
    }
    default:
      break ;
  }
}

void WorldLight::ReparentTo(World* world)
{
  parent_type = Type_None;
  parent_i    = 0;
  nodePath.reparent_to(world->rootLights);
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

void WorldLight::InitializeEnlightenedObjects(World* world)
{
  DiscardEnlightenedObjects();
  for_each(enlightened_index.begin(), enlightened_index.end(), [this, &world](const EnlightenedObjectSettings& settings)
  {
    MapObject* object = world->GetObjectFromName(settings.name);

    AddEnlightenedObject(object, settings.priority, settings.inherited_property);
  });
}

void WorldLight::AddEnlightenedObject(MapObject* object, unsigned int priority, bool propagate_to_children)
{
  if (object)
  {
    enlightened.push_back(object->render);
    object->SetLight(this, true);
    if (propagate_to_children)
    {
      std::for_each(object->children.begin(), object->children.end(), [this, priority](MapObject* child)
      {
        AddEnlightenedObject(child, priority, true);
      });
    }
  }
}

void WorldLight::DiscardEnlightenedObjects(void)
{
  if (enabled)
  {
    SetEnabled(false);
    enlightened.clear();
    SetEnabled(true);
  }
  else
    enlightened.clear();
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
  enabled     = (bool)tmp_enabled;
  type        = (WorldLight::Type)_type;
  parent_type = (WorldLight::ParentType)_ptype;
  if (parent_type != Type_None)
    packet >> parent_name;
  packet >> priority;
  packet >> r >> g >> b >> a;
  packet >> pos_x >> pos_y >> pos_z;
  packet >> hpr_x >> hpr_y >> hpr_z;
  switch (parent_type)
  {
    case Type_MapObject:
      {
        MapObject* object = world->GetMapObjectFromName(parent_name);

        if (object)
          ReparentTo(object);
        else
          ReparentTo(world);
      }
      break ;
    case Type_DynamicObject:
      {
        DynamicObject* object = world->GetDynamicObjectFromName(parent_name);

        if (object)
          ReparentTo(object);
        else
          ReparentTo(world);
      }
      break ;
    case Type_None:
      ReparentTo(world);
      break ;
  }
  cout << "Light type = " << (int)type << endl;
  Initialize();
  {
    float     attenuation[3];

    packet >> attenuation[0] >> attenuation[1] >> attenuation[2];
    SetAttenuation(attenuation[0], attenuation[1], attenuation[2]);

    if (CastsShadows())
      packet >> shadow_settings;
  }
  SetColor(r, g, b, a);
  if (!(nodePath.is_empty()))
  {
    nodePath.set_pos(LVecBase3(pos_x, pos_y, pos_z));
    nodePath.set_hpr(LVecBase3(hpr_x, hpr_y, hpr_z));
  }
  if (blob_revision >= 12)
  {
    collider.parent = nodePath;
    packet >> collider;
  }
  else
  {
    collider.parent = nodePath;
    collider.type   = Collider::NONE;
  }
  if (blob_revision >= 15)
    packet >> enlightened_index;
  else if (blob_revision >= 12)
  {
    std::vector<std::string> tmp;
    packet >> tmp;
    for_each(tmp.begin(), tmp.end(), [this](std::string name)
    { enlightened_index.push_back(WorldLight::EnlightenedObjectSettings(name)); });
  }
  collider.node.set_collide_mask(0);
}

void WorldLight::ShadowSettings::Serialize(Utils::Packet& packet) const
{
  packet << buffer_size[0] << buffer_size[1];
  packet << distance_near << distance_far;
  packet << film_size;
}

void WorldLight::ShadowSettings::Unserialize(Utils::Packet& packet)
{
  packet >> buffer_size[0] >> buffer_size[1];
  packet >> distance_near >> distance_far;
  packet >> film_size;
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
  packet << priority;
  packet << (float)color.get_x() << (float)color.get_y() << (float)color.get_z() << (float)color.get_w();
  packet << (float)nodePath.get_x() << (float)nodePath.get_y() << (float)nodePath.get_z();
  packet << (float)nodePath.get_hpr().get_x() << (float)nodePath.get_hpr().get_y() << (float)nodePath.get_hpr().get_z();
  packet << (float)attenuation.get_x() << (float)attenuation.get_y() << (float)attenuation.get_z();
  if (CastsShadows())
    packet << shadow_settings;
  packet << collider;
  packet << enlightened_index;
}

void WorldLight::LightCollider::SetLightOnCollidingObjects(World *world, WorldLight *light)
{
  if (light->collider.type == Collider::SPHERE)
  {
    NodePath                  collision_node = light->collider.node;
    CollisionTraverser        traverser;
    PT(CollisionHandlerQueue) handler_queue = new CollisionHandlerQueue();
    string                    last_path;

    if (collision_node.node() == 0) return ;
    traverser.add_collider(collision_node, handler_queue);
    traverser.traverse(world->window->get_render());

    for (int i = 0 ; i < handler_queue->get_num_entries() ; ++i)
    {
      NodePath   collision_entry = handler_queue->get_entry(i)->get_into_node_path();
      string     path            = get_nodepath_path(collision_entry);
      MapObject* object;

      if (path == last_path)
        continue ;
      last_path = path;
      object    = world->GetObjectFromNodePath(collision_entry);
      if (object)
        light->AddEnlightenedObject(object, light->priority, false);
    }
  }
}
