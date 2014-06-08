#include "world/particle_effect.hpp"
#include <panda3d/geomParticleRenderer.h>
#include <panda3d/lineParticleRenderer.h>
#include <panda3d/pointParticleRenderer.h>
#include <panda3d/sparkleParticleRenderer.h>
#include <panda3d/spriteParticleRenderer.h>
#define RENDERER_INITIALIZERS_COUNT 5
#include <panda3d/boxEmitter.h>
#include <panda3d/discEmitter.h>
#include <panda3d/lineEmitter.h>
#include <panda3d/pointEmitter.h>
#include <panda3d/rectangleEmitter.h>
#include <panda3d/sphereSurfaceEmitter.h>
#include <panda3d/sphereVolumeEmitter.h>
#include <panda3d/ringEmitter.h>
#define EMITTER_INITIALIZERS_COUNT  8
#include "world/world.h"

ParticleEffect::Initializer ParticleEffect::emitter_initializers[] = {
  Initializer("box",            &ParticleEffect::InitializeBoxEmitter),
  Initializer("disc",           &ParticleEffect::InitializeDiscEmitter),
  Initializer("line",           &ParticleEffect::InitializeLineEmitter),
  Initializer("point",          &ParticleEffect::InitializePointEmitter),
  Initializer("rectangle",      &ParticleEffect::InitializeRectangleEmitter),
  Initializer("sphere-surface", &ParticleEffect::InitializeSphereSurfaceEmitter),
  Initializer("sphere-volume",  &ParticleEffect::InitializeSphereVolumeEmitter),
  Initializer("ring",           &ParticleEffect::InitializeRingEmitter)
};

ParticleEffect::Initializer ParticleEffect::renderer_initializers[] = {
  Initializer("geom",           &ParticleEffect::InitializeGeomRenderer),
  Initializer("line",           &ParticleEffect::InitializeLineRenderer),
  Initializer("point",          &ParticleEffect::InitializePointRenderer),
  Initializer("sparkle",        &ParticleEffect::InitializeSparkleRenderer),
  Initializer("sprite",         &ParticleEffect::InitializeSpriteRenderer)
};

using namespace std;

ParticleEffect::ParticleEffect()
{
}

void ParticleEffect::ReparentTo(NodePath nodePath)
{
  if (!particle_system.is_null())
  {
    {
      NodePath floor_node = nodePath;
#ifdef GAME_EDITOR
      while (floor_node.has_parent())
        floor_node = floor_node.get_parent();
#else
      while (floor_node.has_parent() && !(starts_with(floor_node.get_name(), "floor-")))
        floor_node = floor_node.get_parent();
#endif
      particle_system->set_render_parent(floor_node);
    }
    particle_system->set_spawn_render_node_path(nodePath);
  }
}

string ParticleEffect::GetParentName(void) const
{
  if (!particle_system.is_null())
  {
    NodePath spawn_node = particle_system->get_spawn_render_node_path();

    return (spawn_node.is_empty() ? "" : spawn_node.get_name());
  }
  return ("");
}

void ParticleEffect::LoadConfiguration(const string& filepath)
{
  DataTree* data = DataTree::Factory::JSON(filepath);

  if (data)
  {
    LoadConfiguration(data);
    delete data;
  }
}

void ParticleEffect::LoadConfiguration(Data data)
{
  string factory = data["factory"].Or(std::string("point"));

  if (factory == "point")
    InitializePointParticleFactory(data);
  else if (factory == "zspin")
    InitializeZSpinParticleFactory(data);
  else
    throw UnknownFactoryType();

  // LoadEmitterConfiguration(Data data)
  {
    string emitter = data["emitter"]["type"].Or(string("sphere-volume"));

    for (unsigned int i = 0 ; i < EMITTER_INITIALIZERS_COUNT ; ++i)
    {
      if (emitter == emitter_initializers[i].name)
      {
        (this->*emitter_initializers[i].method)(data["emitter"]);
        break ;
      }
    }
    if (particle_emitter.is_null())
      throw UnknownEmitterType();

    // SetEmissionType(string)
    {
      string emission_type = data["emitter"]["emission-type"].Or(string("radiate"));

      if (emission_type == "radiate")
        particle_emitter->set_emission_type(BaseParticleEmitter::ET_RADIATE);
      else if (emission_type == "explicit")
        particle_emitter->set_emission_type(BaseParticleEmitter::ET_EXPLICIT);
      else
        particle_emitter->set_emission_type(BaseParticleEmitter::ET_CUSTOM);
    }

    particle_emitter->set_amplitude(data["emitter"]["amplitude"].Or(10));
    particle_emitter->set_amplitude_spread(data["emitter"]["amplitude-spread"].Or(0));
    if (data["emitter"]["force"].NotNil())
      particle_emitter->set_offset_force(LVector3f(data["emitter"]["force"]["x"].Or(0),
                                                   data["emitter"]["force"]["y"].Or(0),
                                                   data["emitter"]["force"]["z"].Or(0)));
    if (data["emitter"]["explicit-launch-vector"].NotNil())
      particle_emitter->set_explicit_launch_vector(LVector3f(data["emitter"]["explicit-launch-vector"]["x"].Or(0),
                                                             data["emitter"]["explicit-launch-vector"]["y"].Or(0),
                                                             data["emitter"]["explicit-launch-vector"]["z"].Or(0)));
    if (data["emitter"]["radiate-origin"].NotNil())
      particle_emitter->set_radiate_origin(LVector3f(data["emitter"]["radiate-origin"]["x"].Or(0),
                                                     data["emitter"]["radiate-origin"]["y"].Or(0),
                                                     data["emitter"]["radiate-origin"]["z"].Or(0)));

  }
  // LoadRendererConfiguration
  {
    string renderer_type = data["renderer"]["type"].Or(string("point"));
    Data   alpha_mode    = data["renderer"]["alpha-mode"];

    for (unsigned int i = 0 ; i < RENDERER_INITIALIZERS_COUNT ; ++i)
    {
      if (renderer_type == renderer_initializers[i].name)
      {
        (this->*(renderer_initializers[i].method))(data["renderer"]);
        break ;
      }
    }
    if (particle_renderer.is_null())
      throw UnknownRendererType();
    if (data["renderer"]["alpha"].NotNil())
      particle_renderer->set_user_alpha(data["renderer"]["alpha"]);
    particle_renderer->set_ignore_scale(data["renderer"]["ignore-scale"] == 1);
    if (alpha_mode.NotNil())
    {
      if (alpha_mode == "none")
        particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_NONE);
      else if (alpha_mode == "out")
        particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_OUT);
      else if (alpha_mode == "in")
        particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_IN);
      else if (alpha_mode == "in-out")
        particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_IN_OUT);
      else if (alpha_mode == "user")
        particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_USER);
    }
    else
      particle_renderer->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_NONE);
  }

  // LoadParticleSystem
  {
    particle_system = new ParticleSystem();
    particle_system->set_pool_size(data["pool-size"].Or(100));
    particle_system->set_birth_rate(data["birth-rate"].Or(.1f));
    particle_system->set_litter_size(data["litter-size"].Or(10));
    particle_system->set_litter_spread(data["litter-spread"].Or(0));
    particle_system->set_local_velocity_flag(data["local-velocity"] == 1);
    particle_system->set_system_grows_older_flag(data["grows-older"] == 1);
    particle_system->set_system_lifespan(data["lifespan"].Or(3));
    particle_system->set_active_system_flag(data["active"] == 1);

    particle_system->set_factory(particle_factory);
    particle_system->set_renderer(particle_renderer);
    particle_system->set_emitter(particle_emitter);
  }
}

/*
 * ParticleRendererComponent
 */
void ParticleRendererComponent::InitializePointRenderer(Data data)
{
  PT(PointParticleRenderer) point_renderer = new PointParticleRenderer();

  particle_renderer = point_renderer;
}

void ParticleRendererComponent::InitializeGeomRenderer(Data data)
{
  PT(GeomParticleRenderer) geom_renderer = new GeomParticleRenderer();

  particle_renderer = geom_renderer;
}

void ParticleRendererComponent::InitializeLineRenderer(Data data)
{
  PT(LineParticleRenderer) line_renderer = new LineParticleRenderer();

  particle_renderer = line_renderer;
}

void ParticleRendererComponent::InitializeSparkleRenderer(Data data)
{
  PT(SparkleParticleRenderer) sparkle_renderer = new SparkleParticleRenderer;

  sparkle_renderer->set_birth_radius(data["birth-radius"].Or(10.f));
  sparkle_renderer->set_death_radius(data["death-radius"].Or(50.f));
  sparkle_renderer->set_life_scale(data["life-scale"] == 1 ? SparkleParticleRenderer::SP_SCALE : SparkleParticleRenderer::SP_NO_SCALE);
  sparkle_renderer->set_center_color(LColor(data["center-color"]["r"].Or(1.f), data["center-color"]["g"].Or(1.f), data["center-color"]["b"].Or(1.f), data["center-color"]["a"].Or(0.f)));
  sparkle_renderer->set_edge_color(LColor(data["edge-color"]["r"].Or(1.f), data["edge-color"]["g"].Or(1.f), data["edge-color"]["b"].Or(1.f), data["center-color"]["a"].Or(0.f)));
  particle_renderer = sparkle_renderer;
}

void ParticleRendererComponent::InitializeSpriteRenderer(Data data)
{
  PT(SpriteParticleRenderer) sprite_renderer = new SpriteParticleRenderer;

  sprite_renderer->set_x_scale_flag(data["final-x-scale"].NotNil() || data["initial-x-scale"].NotNil());
  sprite_renderer->set_y_scale_flag(data["final-y-scale"].NotNil() || data["initial-y-scale"].NotNil());
  sprite_renderer->set_final_x_scale(data["final-x-scale"].Or(1.f));
  sprite_renderer->set_final_y_scale(data["final-y-scale"].Or(1.f));
  sprite_renderer->set_initial_x_scale(data["initial-x-scale"].Or(1.f));
  sprite_renderer->set_initial_y_scale(data["initial-y-scale"].Or(1.f));
  {
    string      texture_name = data["texture"].Or(string("magic-particule"));
    PT(Texture) texture = TexturePool::load_texture(TEXT_ROOT + texture_name);

    if (texture)
      sprite_renderer->set_texture(texture);
    else
      std::cerr << "[ParticleObject] Could not load texture " << texture_name << endl;
  }
  sprite_renderer->set_size(data["width"].Or(5.f), data["height"].Or(5.f));
  // TODO: implement anims ?
  particle_renderer = sprite_renderer;
}

/*
 * ParticleEmitterComponent
 */
void ParticleEmitterComponent::InitializeBoxEmitter(Data data)
{
  PT(BoxEmitter) box_emitter    = new BoxEmitter();
  Data           data_max_bound = data["max-bound"];
  Data           data_min_bound = data["min-bound"];

  if (data_max_bound.NotNil())
    box_emitter->set_max_bound(LVector3f(data_max_bound["x"].Or(0.f), data_max_bound["y"].Or(0.f), data_max_bound["z"].Or(0.f)));
  if (data_min_bound.NotNil())
    box_emitter->set_min_bound(LVector3f(data_min_bound["x"].Or(0.f), data_min_bound["y"].Or(0.f), data_min_bound["z"].Or(0.f)));
  particle_emitter = box_emitter;
}

void ParticleEmitterComponent::InitializeDiscEmitter(Data data)
{
  PT(DiscEmitter) disc_emitter = new DiscEmitter();

  disc_emitter->set_cubic_lerping(data["cubic-lerbing"] == 1);
  disc_emitter->set_inner_angle(data["inner-angle"].Or(0.f));
  disc_emitter->set_inner_magnitude(data["inner-magnitude"].Or(2.f));
  disc_emitter->set_outer_angle(data["outer-angle"].Or(100.f));
  disc_emitter->set_outer_magnitude(data["outer-magnitude"].Or(1.f));
  disc_emitter->set_radius(data["radius"].Or(2.f));
  particle_emitter = disc_emitter;
}

void ParticleEmitterComponent::InitializeLineEmitter(Data data)
{
  PT(LineEmitter) line_emitter = new LineEmitter();

  particle_emitter = line_emitter;
}
void ParticleEmitterComponent::InitializePointEmitter(Data data)
{
  PT(PointEmitter) point_emitter = new PointEmitter();

  if (data["position"].NotNil())
    point_emitter->set_location(LVector3f(data["position"]["x"].Or(0.f),
                                          data["position"]["y"].Or(0.f),
                                          data["position"]["z"].Or(0.f)));
  particle_emitter = point_emitter;
}
void ParticleEmitterComponent::InitializeRectangleEmitter(Data data)
{
  PT(RectangleEmitter) rectangle_emitter = new RectangleEmitter();

  if (data["max-bound"].NotNil())
    rectangle_emitter->set_max_bound(LVector2f(data["max-bound"]["x"].Or(10.f), data["max-bound"]["y"].Or(10.f)));
  else if (data["min-bound"].NotNil())
    rectangle_emitter->set_min_bound(LVector2f(data["min-bound"]["x"].Or(0.f),  data["min-bound"]["y"].Or(0.f)));
  particle_emitter = rectangle_emitter;
}

void ParticleEmitterComponent::InitializeSphereSurfaceEmitter(Data data)
{
  PT(SphereSurfaceEmitter) sphere_surface_emitter = new SphereSurfaceEmitter();

  sphere_surface_emitter->set_radius(data["radius"].Or(10.f));
  particle_emitter = sphere_surface_emitter;
}

void ParticleEmitterComponent::InitializeSphereVolumeEmitter(Data data)
{
  PT(SphereVolumeEmitter) sphere_volume_emitter = new SphereVolumeEmitter();

  sphere_volume_emitter->set_radius(data["radius"].Or(10.f));
  particle_emitter = sphere_volume_emitter;
}

void ParticleEmitterComponent::InitializeRingEmitter(Data data)
{
  PT(RingEmitter) ring_emitter = new RingEmitter();

  ring_emitter->set_angle(data["angle"].Or(0.f));
  ring_emitter->set_radius(data["radius"].Or(10.f));
  ring_emitter->set_radius_spread(data["radius-spread"].Or(0.f));
  ring_emitter->set_uniform_emission(data["uniform-emission"].Or(10));
  particle_emitter = ring_emitter;
}

/*
 * ParticleFactoryComponent
 */
void ParticleFactoryComponent::InitializePointParticleFactory(Data data)
{
  PT(PointParticleFactory) particle_factory = new PointParticleFactory();

  this->particle_factory = particle_factory;
}

void ParticleFactoryComponent::InitializeZSpinParticleFactory(Data data)
{
  PT(ZSpinParticleFactory) particle_factory = new ZSpinParticleFactory();

  this->particle_factory = particle_factory;
  if (data["angular-velocity"].NotNil())
  {
    particle_factory->enable_angular_velocity(data["spin-enabled"] == 1 || data["spin-enabled"].Nil());
    particle_factory->set_angular_velocity(data["angular-velocity"]);
    particle_factory->set_angular_velocity_spread(data["angular-velocity-spread"].Or(0.f));
  }
  else
  {
    particle_factory->set_initial_angle(data["initial-angle"].Or(0.f));
    particle_factory->set_initial_angle_spread(data["initial-angle-spread"].Or(0.f));
    particle_factory->set_final_angle(data["final-angle"].Or(100.f));
    particle_factory->set_final_angle_spread(data["final-angle-spread"].Or(0.f));
  }
}

