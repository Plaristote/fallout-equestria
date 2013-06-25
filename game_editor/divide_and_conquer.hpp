#ifndef  DIVIDE_AND_CONQUER
# define DIVIDE_AND_CONQUER

# include <functional>
# include <vector>
# include <algorithm>

namespace DivideAndConquer
{
  template<typename T, typename POS>
  class Graph
  {
  public:
    typedef  std::vector<POS>                                  Positions;
    typedef  std::function<Positions (const std::vector<T*>&)> Initializer;
    typedef  std::function<float (POS, POS)>                   Heuristic;
    typename std::vector<T*>                                   Entries;
    
    struct NodeException : public std::exception
    {
      virtual const char* what(void) const throw() { return ("Empty branch in the graph's node tree"); }
    };
    
  //private:
    struct Node
    {
      virtual ~Node()
      {
        std::for_each(nodes.begin(), nodes.end(), [](Node* children) { delete children; });
      }
      
      virtual const std::vector<T*>& GetEntries(POS position_from, Heuristic heuristic) const
      {
        return (FindBestNode(position_from, heuristic)->GetEntries(position_from, heuristic));
      }
      
      const Node*   FindBestNode(POS position_from, Heuristic heuristic) const
      {
        auto        it                = nodes.begin(), end = nodes.end();
        const Node* best_candidate    = 0;
        float       best_score;
        
        for (; it != end ; ++it)
        {
          const Node* candidate_node  = *it;
          float       candidate_score = heuristic(position_from, (*it)->position);
          
          if (!best_candidate || candidate_score < best_score)
          {
            best_candidate            = candidate_node;
            best_score                = candidate_score;
          }
        }
        if (!best_candidate)
          throw new NodeException();
        return (best_candidate);
      }
      
      Node*   FindBestNode(POS position_from, Heuristic heuristic)
      {
        auto        it                = nodes.begin(), end = nodes.end();
        Node*       best_candidate    = 0;
        float       best_score        = 0;
        
        for (; it != end ; ++it)
        {
          Node*       candidate_node  = *it;
          float       candidate_score = heuristic(position_from, (*it)->position);
          
          if (!best_candidate || candidate_score < best_score)
          {
            best_candidate            = candidate_node;
            best_score                = candidate_score;
          }
        }
        if (!best_candidate)
          throw new NodeException();
        return (best_candidate);
      }
      
      void                Initialize(Heuristic heuristic, Initializer initializer, unsigned short max_per_node)
      {
        Positions         positions = initializer(entries);
        bool              last_node = entries.size() <= max_per_node;
        
        // Generating all the needed node
        std::for_each(positions.begin(), positions.end(), [this, last_node](POS position)
        {
          Node* node;
          
          node = last_node ? new FinalNode : new Node;
          node->position = position;
          nodes.push_back(node);
        });
        
        // Dispatching the entries in the closest nodes
        std::for_each(entries.begin(),   entries.end(),   [this, heuristic](T* item)
        {
          Node* node = FindBestNode(item->GetPosition(), heuristic);
          
          node->entries.push_back(item);
        });
        
        // If we're not at the last level, keep Initialiazing deeper until we are
        if (!last_node)
        {
          for (unsigned int it = 0 ; it < nodes.size() ; ++it)
          {
            Node* node = nodes[it];
            
            if (node->entries.size() != entries.size())
              node->Initialize(heuristic, initializer, max_per_node);
            else
            {
              delete node;
              nodes[it] = new FinalNode;
              nodes[it]->entries = entries;
            }
          }
        }
      }
      
      POS                position;
      std::vector<Node*> nodes;
      std::vector<T*>    entries;
    };
    
    struct FinalNode : public Node
    {
      virtual const std::vector<T*>& GetEntries(POS position_from, Heuristic heuristic) const
      {
        return (this->entries);
      }
    };
    
  public:

    void               SetHeuristic(Heuristic heuristic) { this->heuristic = heuristic; }

    void               Initialize(const std::vector<T*>& entries, Initializer initializer, unsigned short max_per_node = 100)
    {
      node_tree.entries = entries;
      node_tree.Initialize(heuristic, initializer, max_per_node);
    }
    
    T*                 GetClosest(POS position)
    {
      Node* node       = node_tree.FindBestNode(position, heuristic);
      T*    best       = 0;
      float best_score = 0;

      std::for_each(node->entries.begin(), node->entries.end(), [this, &best, &best_score, position](T* entry)
      {
        float score = heuristic(position, entry->GetPosition());
        
        if (!best || best_score > score)
        {
          best       = entry;
          best_score = score;
        }
      });
      return (best);
    }

  //private:
    Heuristic          heuristic;
    Node               node_tree;
  };
}

#endif
