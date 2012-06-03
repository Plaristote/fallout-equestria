#ifndef  PATHFINDING_HPP
# define PATHFINDING_HPP

# include "astar.hpp"

class Character;

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
      struct Observer
      {
        virtual bool CanGoThrough(Character*) = 0;
        virtual bool GoingThrough(Character*) = 0;
      };

      Arc(Node* f, float s, Observer* o = 0) : std::pair<Node*, float>(f, s) { observer = o; }
      bool operator==(const Node* comp) const { return (first == comp); }

      Observer* observer;
    };

    typedef std::list<Arc>          Arcs;
    
    Node(int x, int y)      : x(x), y(y) {}
    Node(void)              : x(0), y(0) {}
    Node(const Node& other) : x(other.x), y(other.y)
    {
      arcs = other.arcs;
    }

    bool       operator==(const Node& other) const
    {
      return (other.x == x && other.y == y);
    }

    void             DeleteArc(const Node& other);
    float            GetCost(Node& other) const;
    std::list<Node*> GetSuccessors(Node* parent);
    float            GoalDistanceEstimate(Node& goal)
    {
      return (Pathfinding::Heuristic(*this, goal));
    }

    int          x;
    int          y;
    Arcs         arcs;
    Pathfinding* pf;
  };

  Pathfinding(const std::vector<Node>& nodes, int sizex, int sizey)
  {
    _nodes = nodes;
    _sizex = sizex;
    _sizey = sizey;
    std::for_each(_nodes.begin(), _nodes.end(), [this](Node& node) { node.pf = this; });
  }

  bool         FindPath(Character* pathSeeker, std::list<Node>& path, int beg_x, int beg_y, int dest_x, int dest_y);

  Node&        GetNode(int x, int y)
  {
    if (x < 0 || y < 0 || x >= _sizex || y >= _sizey)
      return (_nodes[0]);
    return (_nodes[y * _sizex + x]);
  }

  void         GetSize(int& x, int& y) const { x = _sizex; y = _sizey; }

  static void  ConnectNodes(Node& first, Node& second, float cost = 1.f, Pathfinding::Node::Arc::Observer* observer = 0);
  void         DisconnectNodes(int x1, int y1, int x2, int y2) { DisconnectNodes(GetNode(x1, y1), GetNode(x2, y2)); }
  static void  DisconnectNodes(Node& first, Node& second);
  void         DisconnectNode(int x, int y) { DisconnectNode(GetNode(x, y)); }
  static void  DisconnectNode(Node& node);

  static float Heuristic(const Node& first, const Node& second);

private:
  std::vector<Node> _nodes;
  int               _sizex, _sizey;
  Character*        _currentPathseeker;
};

#endif