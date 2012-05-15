#include "mapelement.hpp"
#include "pathfinding.hpp"

using namespace std;

void    MapElement::UnocccupyPosition(int x, int y)
{
  Positions::iterator cur = _occupedNodes.begin();
  Positions::iterator end = _occupedNodes.end();

  while (cur != end)
  {
    if ((*cur).get_x() == x && (*cur).get_y() == y)
      cur = _occupedNodes.erase(cur);
    else
      ++cur;
  }
}

void    MapElement::ProcessCollision(Pathfinding* map)
{
  if (_collisionEnabled)
  {
    Positions::const_iterator it  = _occupedNodes.begin();
    Positions::const_iterator end = _occupedNodes.end();

    for (; it != end ; ++it)
      map->DisconnectNode(_mapPos.get_x() + (*it).get_x(), _mapPos.get_y() + (*it).get_y());
  }
}
