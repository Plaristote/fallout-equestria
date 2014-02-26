#ifndef  PROJECTILE_HPP
# define PROJECTILE_HPP

# include "globals.hpp"
# include <panda3d/pointLight.h>
# include <panda3d/pointParticleFactory.h>
# include <panda3d/sphereVolumeEmitter.h>
# include <panda3d/pointParticleRenderer.h>
# include <panda3d/particleSystem.h>
# include "world/world.h"
# include "datatree.hpp"
# include "observatory.hpp"

class ObjectCharacter;
class InventoryObject;

class Projectile
{
public:
  class Set : public std::list<Projectile*>
  {
  public:
    void Run(float elapsed_time);
    void CleanUp(void);
  private:
  };  
  
  Projectile(World* world, NodePath parent, NodePath target, Data data);
  ~Projectile(void);

  static Projectile* Factory(World*, ObjectCharacter*, InventoryObject*);

  NodePath GetNodePath(void) const { return (node_path); }

  bool     HasExpired(void) const;
  bool     HasReachedDestination(void) const;
  void     SetTimeout(float timeout);
  void     SetColor(float red, float green, float blue, float alpha);
  void     SetAttenuation(float a, float b, float c);
  void     Run(float elapsed_time);
  
  Sync::Signal<void> HitsTarget;

private:
  NodePath           node_path, light_node, enlightened, target;
  bool               is_moving;
  float              speed;
  PT(ParticleSystem) system;
  PT(PointLight)     light;
  float              timeout;
  float              time_left;
  LColor             color;
};

#endif