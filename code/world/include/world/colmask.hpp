#ifndef  WORLD_COLMASK_HPP
# define WORLD_COLMASK_HPP

namespace ColMask
{
  enum
  {
    None                  = 0,
    Waypoint              = 1,
    DynObject             = 2,
    Object                = 4,
    FovTarget             = 8,
    WpPlane               = 16,
    FovBlocker            = 32,
    CheckCollisionOnModel = 64,
    Render                = 128
  };
}

#endif
