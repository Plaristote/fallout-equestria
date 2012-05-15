#include "pathfinding.hpp"
#include <iostream>

using namespace std;

int loopCount;

bool Pathfinding::FindPath(std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y, int max_depth)
{
  Node& dest = GetNode(dest_x, dest_y);
  Node& beg  = GetNode(beg_x,  beg_y);

//   cout << "Pathfinding " << beg_x << "," << beg_y << " and " << dest_x << "," << dest_y << endl;
  // Explore every possible path and give a score to each node corresponding to
  // the least costly path from said node to the destination
  cout << "Start evaluating node" << endl;
  loopCount = 0;
  _currentDestNode = &beg;
  _currentBestPath = 0;
  EvaluateNode(std::list<Node>(), dest, 1, max_depth);
  cout << "Done evaluating node (complexity factor: " << loopCount << " - yes it's too damn high)" << endl;

  // If at least one path has been found, rebuild by searching least costly nodes
  if (beg.distance != 0)
  {
    Node& current_node = beg;

//     std::cout << "Begin node " << current_node.x << "," << current_node.y << " has weight " << current_node.distance << std::endl;
    do
    {
      Node::Arcs::iterator cur     = current_node.arcs.begin();
      Node::Arcs::iterator end     = current_node.arcs.end();
      Node*                closest = 0;

      for (; cur != end ; ++cur)
      {
        Node::Arc arc = *cur;

        if ((closest == 0 or closest->distance > arc.first->distance) and arc.first->distance != 0)
          closest = arc.first;
//         std::cout << " - Checking node " << (*cur)->x << "," << (*cur)->y << " with weight " << (*cur)->distance << std::endl;
      }
      if (closest == 0)
        return (false);
      current_node = *closest;
//       cout << "Adding node " << current_node.x << "," << current_node.y << " with weight " << current_node.distance << std::endl;
      path.push_back(current_node);
    } while (current_node.x != dest.x or current_node.y != dest.y);
    return (true);
  }
  return (false);
}

Pathfinding::Node::Node(const Node& cpy)
{
  if (&cpy == this) return ;
  x        = cpy.x;
  y        = cpy.y;
  arcs     = cpy.arcs;
  distance = cpy.distance;
}

void Pathfinding::EvaluateNode(std::list<Node> path, Node& current_node, float current_depth, int max_depth)
{
  if (current_depth == max_depth * 3)
  {
//     cout << "Reached the depth limit" << endl;
    return ;
  }

  // We already have a better path than this
  if (_currentDestNode->distance != 0 && _currentDestNode->distance < current_depth)
    return;

  if (current_node.distance == 0 or current_node.distance > current_depth)
  {
    loopCount += 1;
    current_node.distance = current_depth;

    std::list<Node>      nextPath = path;
    Node::Arcs::iterator cur      = current_node.arcs.begin();
    Node::Arcs::iterator end      = current_node.arcs.end();

//     cout << "current_node coord " << current_node.x << "," << current_node.y << endl;
    nextPath.push_back(current_node);
    for (; cur != end ; ++cur)
    {
      Node::Arc arc = *cur;
      
      if (IsInPath(path, *(arc.first)))
        continue ;
//       cout << "Next level" << endl;
      EvaluateNode(nextPath, *(arc.first), current_depth + (arc.second), max_depth);
//       cout << "Done with this level" << endl;
    }
  }
//   else
//     cout << "This node " << current_node.x << "," << current_node.y << " is already rated better" << endl;
}

bool Pathfinding::IsInPath(const std::list<Node>& path, Node& node) const
{
  std::list<Node>::const_iterator cur = path.begin();
  std::list<Node>::const_iterator end = path.end();

  for (; cur != end ; ++cur)
  {
    if (node.x == (*cur).x and node.y == (*cur).y)
      return (true);
  }
  return (false);
}