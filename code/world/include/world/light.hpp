#ifndef  WORLD_LIGHT_HPP
# define WORLD_LIGHT_HPP

# include "globals.hpp"
# include <panda3d/pointLight.h>
# include <panda3d/directionalLight.h>
# include <panda3d/ambientLight.h>
# include <panda3d/spotlight.h>
# include "serializer.hpp"
# include "collider.hpp"

struct World;
struct MapObject;
struct DynamicObject;

namespace Utils
{
  template<typename A, typename B>
  struct Pair : public std::pair<A, B>, public Utils::Serializable
  {
    void Serialize(Utils::Packet& packet) const { packet << this->first << this->second; }
    void Unserialize(Utils::Packet& packet)     { packet >> this->first >> this->second; }
  };
}

struct WorldLight : public Utils::Serializable
{
  struct EnlightenedObjectSettings : public Utils::Serializable
  {
    EnlightenedObjectSettings() : priority(1), inherited_property(true) {}
    EnlightenedObjectSettings(const std::string& name) : name(name), priority(1), inherited_property(true) {}
    EnlightenedObjectSettings(const std::string& name, unsigned short priority, bool inherited_property) : name(name), priority(priority), inherited_property(inherited_property) {}

    bool operator==(const std::string& name) const { return (this->name == name); }

    void Serialize(Utils::Packet& packet) const { packet << name << priority << inherited_property; }
    void Unserialize(Utils::Packet& packet)     { packet >> name >> priority >> inherited_property; }

    std::string    name;
    unsigned short priority;
    bool           inherited_property;
  };

  typedef std::vector<EnlightenedObjectSettings> EnlightenedObjects;

  struct ShadowSettings : public Utils::Serializable
  {
    ShadowSettings()
    {
      buffer_size[0] = 256;
      buffer_size[1] = 256;
      distance_near  = 1.f;
      distance_far   = 30.f;
      film_size      = 1024;
    }

    void Serialize(Utils::Packet&) const;
    void Unserialize(Utils::Packet&);

    unsigned int buffer_size[2];
    float        distance_near, distance_far;
    unsigned int film_size;
  };

  struct LightCollider : public Collider
  {
    void SetLightOnCollidingObjects(World* world, WorldLight* light);
  };


  enum Type
  {
    Point,
    Directional,
    Ambient,
    Spot
  };

  enum ParentType
  {
    Type_None,
    Type_MapObject,
    Type_DynamicObject
  };

  WorldLight(Type type, ParentType ptype, NodePath parent, const std::string& name) : name(name), type(type), parent_type(ptype), parent(parent), parent_i(0)
  {
    Initialize();
  }

  WorldLight(NodePath parent) : parent(parent), parent_i(0) {}

  WorldLight() : parent_i(0) {}
  
  void       Initialize(void);
  void       InitializeShadowCaster(void);
  void       SetEnabled(bool);
  void       Destroy(void);
  bool       CastsShadows(void) const { return (type == Point || type == Spot); }

  LColor     GetColor(void) const { return (light->get_color()); }
  void       SetColor(float r, float g, float b, float a);
  LVecBase3f GetAttenuation(void) const;
  void       SetAttenuation(float a, float b, float c);
  void       SetPosition(LPoint3 position) { nodePath.set_pos(position); }
  void       SetFrustumVisible(bool);

  bool       operator==(const std::string& comp) { return (name == comp); }

  void       Unserialize(Utils::Packet& packet);
  void       Serialize(Utils::Packet& packet) const;

  void       ReparentTo(World* world);
  void       ReparentTo(DynamicObject* object);
  void       ReparentTo(MapObject* object);
  MapObject* Parent(void) const { return (parent_i); }

  void       InitializeEnlightenedObjects(World*);
  void       AddEnlightenedObject(MapObject*, unsigned int priority, bool propagate_to_children);
  void       DiscardEnlightenedObjects(void);

  std::string              name;
  NodePath                 nodePath;
  Type                     type;
  ParentType               parent_type;
  PT(Light)                light;
  Lens*                    lens;
  float                    zoneSize; // DEPRECATED
  unsigned char            priority;
  bool                     enabled;
  ShadowSettings           shadow_settings;
  LightCollider            collider;
  std::list<NodePath>      enlightened;
  EnlightenedObjects       enlightened_index;
#ifdef GAME_EDITOR
  NodePath                 symbol;
#endif
private:
  NodePath                 parent;
  MapObject*               parent_i;
};


#endif
