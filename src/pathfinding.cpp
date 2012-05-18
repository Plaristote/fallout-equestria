#include "pathfinding.hpp"
#include <iostream>
#include <panda3d/pStatCollector.h>

# define MAX_PATHFINDING_ITERATIONS 500

using namespace std;

float Pathfinding::Heuristic(const Node& first, const Node& second)
{
  int dist_x = ABS(first.x - second.x);
  int dist_y = ABS(first.y - second.y);

  return (dist_x > dist_y ? dist_x : dist_y);
}

bool Pathfinding::FindPath(Character* pathseeker, std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y)
{
  Astar        astar;
  Node&        beg = GetNode(beg_x, beg_y);
  Node&        end = GetNode(dest_x, dest_y);
  unsigned int state;
  unsigned int max_iterations = 0;

  _currentPathseeker = pathseeker;
  astar.SetStartAndGoalStates(beg, end);
  while ((state = astar.SearchStep()) == Astar::Searching && max_iterations++ < MAX_PATHFINDING_ITERATIONS);

  if (state == Astar::Succeeded)
  {
    path = astar.GetSolution();
    return (true);
  }
  return (false);
}

void Pathfinding::ConnectNodes(Node& first, Node& second, float cost, Pathfinding::Node::Arc::Observer* observer)
{
  first.arcs.push_back(Pathfinding::Node::Arc(&second, cost, observer));
  second.arcs.push_back(Pathfinding::Node::Arc(&first, cost, observer));
}

void Pathfinding::DisconnectNodes(Node& first, Node& second)
{
  first.DeleteArc(second);
  second.DeleteArc(first);
}

void Pathfinding::DisconnectNode(Node& node)
{
  Node::Arcs::iterator cur = node.arcs.begin();
  Node::Arcs::iterator end = node.arcs.end();

  while (cur != end)
  {
    Node&                otherNode = *((*cur).first);
    Node::Arcs::iterator nodeIt    = std::find(otherNode.arcs.begin(), otherNode.arcs.end(), &node);

    otherNode.arcs.erase(nodeIt);
    cur = node.arcs.erase(cur);
  }
}

std::list<Pathfinding::Node*> Pathfinding::Node::GetSuccessors(Node* parent)
{
  std::list<Node*> successors;
  Arcs::iterator   it  = arcs.begin();
  Arcs::iterator   end = arcs.end();

  int successorsCount = 0;
  for (; it != end ; ++it)
  {
    Arc&  arc       = *it;
    Node* successor = arc.first;

    if (parent)
    {
      if (successor->x == parent->x && successor->y == parent->y)
        continue ;
    }
    if (arc.observer)
    {
      if (!(arc.observer->CanGoThrough(pf->_currentPathseeker)))
        continue ;
    }
    successorsCount++;
    successors.push_back(successor);
  }
  return (successors);
}

float      Pathfinding::Node::GetCost(Node& other) const
{
  Arcs::const_iterator it  = arcs.begin();
  Arcs::const_iterator end = arcs.end();

  for (; it != end ; ++it)
  {
    if ((*it).first->x == other.x && (*it).first->y == other.y)
      return ((*it).second);
  }
  return (1.f);
}

void       Pathfinding::Node::DeleteArc(const Node& other)
{
  Arcs::iterator it = std::find(arcs.begin(), arcs.end(), &other);

  if (it != arcs.end())
    arcs.erase(it);
}