#include "pathfinding.hpp"
#include <iostream>

using namespace std;

bool Pathfinding::FindPath(std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y)
{
  Astar        astar;
  Node&        beg = GetNode(beg_x, beg_y);
  Node&        end = GetNode(dest_x, dest_y);
  unsigned int state;

  astar.SetStartAndGoalStates(beg, end);
  while ((state = astar.SearchStep()) == Astar::Searching);

  if (state == Astar::Succeeded)
  {
    path = astar.GetSolution();
    cout << "Solution steps " << astar.GetStepCount() << endl;
    return (true);
  }
  cout << "Didn't find correct path" << endl;
  return (false);
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
    successorsCount++;
    successors.push_back(successor);
  }
  std::cout << "Case " << x << "/" << y << " has " << successorsCount << " successors" << std::endl;
  return (successors);
}
