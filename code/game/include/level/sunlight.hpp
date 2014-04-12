#ifndef  SUNLIGHT_HPP
# define SUNLIGHT_HPP

# include "world/world.h"
# include "scheduled_task.hpp"
# include "solar_orbit.hpp"
# include "timer.hpp"

class Sunlight : public ScheduledTask, private Sync::ObserverHandler
{
public:
  Sunlight(World& world, const TimeManager& time_manager);
  ~Sunlight();

  void         SetLightsOnModels(void);
  void         SetGroundLevel(unsigned short ground_level) { this->ground_level = ground_level; }
  void         SetNearFar(float distance_near, float distance_far);

protected:
  void         Run(void);
  
private:
  void         SetShadowCaster(void);
  unsigned int GetFilmSize(void);
  unsigned int GetShadowBufferSize(void);

  void         InitializeSun(void);
  void         InitializeAmbientLight(void);

  void         SetLightColorFromTime(void);
  void         SetSunlightPositionFromTime(void);

  World&               world;
  const TimeManager&   time_manager;
  PT(DirectionalLight) sunlight_node;
  NodePath             sunlight_nodepath;
  PT(AmbientLight)     ambient_node;
  NodePath             ambient_nodepath;
  unsigned short       ground_level;
  SolarOrbit           solar_orbit;
  Timer                time_since_last_run;
};

#endif
