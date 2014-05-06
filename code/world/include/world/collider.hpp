#ifndef  COLLIDER_HPP
# define COLLIDER_HPP

# include "globals.hpp"

struct Collider : public Utils::Serializable
{
  enum Type
  {
    NONE,
    MODEL,
    BOX,
    SPHERE
  };

  NodePath parent, node;
  Type     type;

  void InitializeCollider(Type type, LPoint3f position, LPoint3f scale, LPoint3f hpr);
  void Serialize(Utils::Packet&) const;
  void Unserialize(Utils::Packet&);
};

#endif
