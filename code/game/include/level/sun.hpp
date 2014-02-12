#ifndef  SUN_HPP
# define SUN_HPP

# include "world/world.h"

struct World;

class Circle
{
public:
  void     SetPosition(float x, float y) { this->cx = x; this->cy = y; }
  void     SetRadius(float radius)       { this->radius = radius;      }

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