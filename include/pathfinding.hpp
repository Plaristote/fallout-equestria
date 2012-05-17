#ifndef  PATHFINDING_HPP
# define PATHFINDING_HPP

# include "astar.hpp"

# define ABS(x) ((x) > 0 ? (x) : -(x))

class Pathfinding
{
public:
  struct Node;
private:
  typedef AstarPathfinding<Pathfinding::Node> Astar;
  typedef Astar::Node                AstarNode;
  typedef std::vector<AstarNode*>             AstarNodes;
public:

  struct Node
  {
    struct Arc : public std::pair<Node*, float>
    {
      Arc(Node* f, float s) : std::pair<Node*, float>(f, s) {}
      bool operator==(const Node* comp) const { return (first == comp); }
    };

    typedef std::list<Arc>          Arcs;
    
    Node(int x, int y)      : x(x), y(y) {}
    Node(void)              : x(0), y(0) {}
    Node(const Node& other) : x(other.x), y(other.y)
    {
      arcs = other.arcs;
    }

    Node&      operator=(const Node& other)
    {
      if (this != &other)
      {
        x    = other.x;
        y    = other.y;
        arcs = other.arcs;
      }
      return (*this);
    }

    bool       operator==(const Node& other) const
    {
      return (other.x == x && other.y == y);
    }

    float      GetCost(Node& other) const
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

    std::list<Node*> GetSuccessors(Node* parent);
    float      GoalDistanceEstimate(Node& goal)
    {
      return (Pathfinding::Heuristic(*this, goal));
    }

    void       DeleteArc(const Node& other)
    {
      Arcs::iterator it = std::find(arcs.begin(), arcs.end(), &other);

      if (it != arcs.end())
        arcs.erase(it);
    }
    
    int          x;
    int          y;
    Arcs         arcs;
  };

  Pathfinding(const std::vector<Node>& nodes, int sizex, int sizey)
  {
    _nodes = nodes;
    _sizex = sizex;
    _sizey = sizey;
  }

  static float Heuristic(const Node& first, const Node& second)
  {
    int dist_x = ABS(first.x - second.x);
    int dist_y = ABS(first.y - second.y);

    return (dist_x > dist_y ? dist_x : dist_y);
  }

  bool FindPath(std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y);
  
  Node& GetNode(int x, int y)
  {
    if (x < 0 || y < 0 || x >= _sizex || y >= _sizey)
      throw "Pathfinding::GetNode: Index out of bounds";
    return (_nodes[y * _sizex + x]);
  }

  void  GetSize(int& x, int& y) const { x = _sizex; y = _sizey; }

  static void  ConnectNodes(Node& first, Node& second, float cost = 1.f)
  {
    first.arcs.push_back(Pathfinding::Node::Arc(&second, cost));
    second.arcs.push_back(Pathfinding::Node::Arc(&first, cost));
  }

  void         DisconnectNodes(int x1, int y1, int x2, int y2) { DisconnectNodes(GetNode(x1, y1), GetNode(x2, y2)); }
  static void  DisconnectNodes(Node& first, Node& second)
  {
    first.DeleteArc(second);
    second.DeleteArc(first);
  }

  void         DisconnectNode(int x, int y) { DisconnectNode(GetNode(x, y)); }
  static void  DisconnectNode(Node& node)
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
};

#endif