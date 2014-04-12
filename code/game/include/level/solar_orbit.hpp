#ifndef  SOLAR_ORBIT_HPP
# define SOLAR_ORBIT_HPP

# include "world/world.h"

struct World;

class SolarOrbit
{
public:
  void     SetPosition(float x, float y) { this->cx = x; this->cy = y; }
  void     SetRadius(float radius)       { this->radius = radius;      }
  float    GetRadius(void) const         { return (radius);            }

  LPoint3f GetPosition(void) const
  {
    return (LPoint3f(cx, cy, 0));
  }

  LPoint2f PointAtAngle(float angle);
  void     SetFromWorld(World* world);

private:
  float cx, cy, radius;
};

#endif
