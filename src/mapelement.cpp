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
    {
      Pathfinding::Node&                nodeFrom = map->GetNode(_mapPos.get_x() + (*it).get_x(), _mapPos.get_y() + (*it).get_y());
      Pathfinding::Node::Arcs::iterator arcIt    = nodeFrom.arcs.begin();
      Pathfinding::Node::Arcs::iterator arcEnd   = nodeFrom.arcs.end();
      WithdrawedArc                     withdrawed;

      withdrawed.from     = &nodeFrom;
      while (arcIt != arcEnd)
      {
        WithdrawArc(nodeFrom, *arcIt);
        arcIt = nodeFrom.arcs.begin();
      }
    }
  }
}

void MapElement::UnprocessCollision(Pathfinding* map)
{
  while (_withdrawedArcs.begin() != _withdrawedArcs.end())
  {
    WithdrawedArc withdrawed = *(_withdrawedArcs.begin());

    map->ConnectNodes(*(withdrawed.from), *(withdrawed.to), withdrawed.weigth);
    _withdrawedArcs.erase(_withdrawedArcs.begin());
  }
}

void MapElement::WithdrawArc(Pathfinding::Node& node, Pathfinding::Node::Arc arc)
{
  WithdrawedArc                     withdrawed;

  withdrawed.from   = &node;
  withdrawed.to     = arc.first;
  withdrawed.weigth = arc.second;
  Pathfinding::DisconnectNodes(node, *(withdrawed.to));
  _withdrawedArcs.push_back(withdrawed);
}