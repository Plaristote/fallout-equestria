#include "level/solar_orbit.hpp"

LPoint2f SolarOrbit::PointAtAngle(float angle)
{
  LPoint2f ret;

  ret.set_x(cx + radius * cos(angle));
  ret.set_y(cy + radius * sin(angle));
  return (ret);
}

void    SolarOrbit::SetFromWorld(World* world)
{
  LPoint3 upper_left(0, 0, 0);
  LPoint3 bottom_left(0, 0, 0);
  LPoint3 upper_right(0, 0, 0);
  
  world->GetWaypointLimits(0, upper_right, upper_left, bottom_left);
  
  cx     = (bottom_left.get_x() + upper_right.get_x()) / 2;
  cy     = (bottom_left.get_y() + upper_right.get_y()) / 2;
  radius = upper_right.get_x() - cx;
}
