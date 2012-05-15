#ifndef  PATHFINDING_HPP
# define PATHFINDING_HPP

# include <math.h>
# include <list>
# include <vector>
# include <algorithm>
# include <iostream>

class Pathfinding
{
public:
  struct Node
  {
    Node(int x, int y) : x(x), y(y), distance(0) {}
    Node(void)         : distance(0)             {}
    Node(const Node& node);

    void             DeleteArc(const Node& other)
    {
      Arcs::iterator it = std::find(arcs.begin(), arcs.end(), &other);

      if (it != arcs.end())
      {
        std::cout << "Disconnected" << std::endl;
        arcs.erase(it);
      }
    }

    struct Arc : public std::pair<Node*, float>
    {
      Arc(Node* f, float s) : std::pair<Node*, float>(f, s) {}
      bool operator==(const Node* comp) const { return (first == comp); }
    };

    typedef std::list<Arc>          Arcs;

    int              x;
    int              y;
    Arcs             arcs;
    float            distance;
  };

  Pathfinding(void) {}
  
  Pathfinding(const std::vector<Node>& nodes, int sizex, int sizey)
  {
    _nodes = nodes;
    _sizex = sizex;
    _sizey = sizey;
  }
  
  // Default max depth is twice the distance between the two points
  bool FindPath(std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y)
  {
    int def_depth = (fabs(float((beg_x - dest_x))) + fabs(float(((beg_y - dest_y))))) * 2;

    return (FindPath(path, beg_x, beg_y, dest_x, dest_y, def_depth + 1));
  }

  bool FindPath(std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y, int max_depth);
  void EvaluateNode(std::list<Node> path, Node& current_node, float current_depth, int max_depth);
  bool IsInPath(const std::list<Node>& path, Node& node) const;

  Node& GetNode(int x, int y)
  {
    if (x < 0 || y < 0 || x >= _sizex || y >= _sizey)
      throw "Pathfinding::GetNode: Index out of bounds";
    return (_nodes[y * _sizex + x]);
  }

  void  GetSize(int& x, int& y) const { x = _sizex; y = _sizey; }

  void  DisconnectNodes(int x1, int y1, int x2, int y2) { DisconnectNodes(GetNode(x1, y1), GetNode(x2, y2)); }
  void  DisconnectNodes(Node& first, Node& second)
  {
    first.DeleteArc(second);
    second.DeleteArc(first);
  }

  void  DisconnectNode(int x, int y) { DisconnectNode(GetNode(x, y)); }
  void  DisconnectNode(Node& node)
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

private:
  std::vector<Node> _nodes;
  int               _sizex, _sizey;
  float             _currentBestPath;
  Node*             _currentDestNode;
};


#endif