#ifndef  OBJECT_NODE_HPP
# define OBJECT_NODE_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include "datatree.hpp"
# include "tilemap.hpp"
# include "observatory.hpp"

class Character;

class ObjectNode : public MapElement
{
public:
  static Observatory::Signal<void (ObjectNode*)> ActionUse;
  static Observatory::Signal<void (ObjectNode*)> ActionUseObjectOn;
  static Observatory::Signal<void (ObjectNode*)> ActionUseSkillOn;
  static Observatory::Signal<void (ObjectNode*)> ActionTalkTo;

  struct Interaction
  {
    std::string                              name;
    ObjectNode*                              objectNode;
    Observatory::Signal<void (ObjectNode*)>* triggered;
  };
  typedef std::list<Interaction> Interactions;
  
  ObjectNode(WindowFramework* window, Tilemap& map, Data data);
  ObjectNode(WindowFramework* window, Tilemap& map) : _window(window), _map(map)
  {
    _map.AddMapElement(this);
  }

  ~ObjectNode()
  {
    _map.DelMapElement(this);
  }

  bool          operator==(NodePath comp) const  { return (_root == comp);                    }
  bool          HasUnitId(const std::string& id) { return (_root.get_tag("character") == id); }

  virtual void  Run(float elapsedTime);

  void          ForceCurrentCase(int x, int y);
  void          ForceClosestCase(void);

  Interactions& GetInteractions(void) { return (_interactions); }
  virtual void  InteractUse(Character* c)    { InteractDoesNothing(c); }
  virtual void  InteractTalkTo(Character* c) { InteractDoesNothing(c); }

protected:
  void          InteractDoesNothing(Character* c) { std::cout << "[GameLog] That does nothing" << std::endl; }
  void          LoadInteractions(Data);

  WindowFramework* _window;
  Tilemap&         _map;
  NodePath         _root;

  Interactions _interactions;
};

#endif