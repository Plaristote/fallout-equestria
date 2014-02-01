#include "is_game_editor.h"
#ifdef GAME_EDITOR
# include "world.h"
#else
# include "level/world.h"
#endif
#include <panda3d/nodePathCollection.h>

float my_sqrt(const float x)
{
  union { int a; float x; } u;

  u.x = x;
  u.a = (1 << 29) + (u.a >> 1) - (1 << 22);
  u.x = (u.x + (x / u.x));
  return (u.x * 0.5f);
}

LPoint3f operator*(LPoint3f p1, LPoint3f p2)
{
  LPoint3f result;

  result.set_x(p1.get_x() * p2.get_x());
  result.set_y(p1.get_y() * p2.get_y());
  result.set_z(p1.get_z() * p2.get_z());
  return (result);
}

void SetCollideMaskOnSingleNodepath(NodePath np, unsigned short collide_mask)
{
  NodePathCollection col = np.get_children();

  np.set_collide_mask(CollideMask(collide_mask));
  for (int n = 0 ; n < col.size() ; ++n)
  {
    NodePath cnp = col.get_path(n);

    cnp.set_collide_mask(CollideMask(0));
  }
}

LPoint3 NodePathSize(NodePath np)
{
  LPoint3 min_point, max_point;
  LPoint3 ret;

  np.calc_tight_bounds(min_point, max_point);
  ret.set_x(max_point.get_x() - min_point.get_x());
  ret.set_y(max_point.get_y() - min_point.get_y());
  ret.set_z(max_point.get_z() - min_point.get_z());
  return (ret);
}
