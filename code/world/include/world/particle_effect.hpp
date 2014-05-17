#ifndef  PARTICLE_EFFECT_HPP
# define PARTICLE_EFFECT_HPP

# include "globals.hpp"
# include <panda3d/pointParticleFactory.h>
# include <panda3d/zSpinParticleFactory.h>
# include <panda3d/baseParticleEmitter.h>
# include <panda3d/baseParticleRenderer.h>
# include <panda3d/particleSystem.h>
# include "datatree.hpp"
# define  AddException(type_name, message) \
  struct type_name : public std::exception { public: const char* what() const throw() { return (message); } };

struct ParticleFactoryComponent
{
  void InitializePointParticleFactory(Data data);
  void InitializeZSpinParticleFactory(Data data);

  PT(BaseParticleFactory)   particle_factory;
};

struct ParticleEmitterComponent
{
  void InitializeBoxEmitter(Data data);
  void InitializeDiscEmitter(Data data);
  void InitializeLineEmitter(Data data);
  void InitializePointEmitter(Data data);
  void InitializeRectangleEmitter(Data data);
  void InitializeRingEmitter(Data data);
  void InitializeSphereSurfaceEmitter(Data data);
  void InitializeSphereVolumeEmitter(Data data);
  void InitializeTangeantRingEmitter(Data data);

  PT(BaseParticleEmitter)   particle_emitter;
};

struct ParticleRendererComponent
{
  void InitializeGeomRenderer(Data data);
  void InitializeLineRenderer(Data data);
  void InitializePointRenderer(Data data);
  void InitializeSparkleRenderer(Data data);
  void InitializeSpriteRenderer(Data data);

  PT(BaseParticleRenderer)  particle_renderer;
};

class ParticleEffect : protected ParticleFactoryComponent, protected ParticleEmitterComponent, protected ParticleRendererComponent
{
public:
  AddException(UnknownFactoryType,  "Unknown factory type")
  AddException(UnknownEmitterType,  "Unknown emitter type")
  AddException(UnknownRendererType, "Unknown renderer type")

  ParticleEffect();

  void LoadConfiguration(const std::string& filepath);
  void LoadConfiguration(Data configuration);
  void ReparentTo(NodePath nodePath);

  PT(ParticleSystem) GetParticleSystem(void) const { return (particle_system); }

private:

  PT(ParticleSystem) particle_system;

  struct Initializer
  {
    typedef void (ParticleEffect::*Loader)(Data);
    Initializer(const std::string name, Loader method) : name(name), method(method) {}
    const std::string name;
    Loader            method;
  };

  static Initializer emitter_initializers[];
  static Initializer renderer_initializers[];
};

#endif
