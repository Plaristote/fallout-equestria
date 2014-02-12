#ifndef  WORLD_LIGHT_HPP
# define WORLD_LIGHT_HPP

# include "globals.hpp"
# include <panda3d/pointLight.h>
# include <panda3d/directionalLight.h>
# include <panda3d/ambientLight.h>
# include <panda3d/spotlight.h>
# include "serializer.hpp"

struct World;
struct MapObject;
struct DynamicObject;

struct WorldLight
{
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

  void   SetEnabled(bool);
  void   Destroy(void);

  LColor GetColor(void) const
  {
    return (light->get_color());
  }

  void   SetColor(float r, float g, float b, float a)
  {
    LColor color(r, g, b, a);

    light->set_color(color);
  }

  LVecBase3f GetAttenuation(void) const
  {
    switch (type)
    {
      case Point:
      {
        PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

        return (point_light->get_attenuation());
      }
      case Spot:
      {
        PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

        return (spot_light->get_attenuation());
      }
      default:
          break;
    }
    return (LVecBase3f(0, 0, 0));
  }

  void   SetAttenuation(float a, float b, float c)
  {
    switch (type)
    {
      case Point:
      {
        PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

        point_light->set_attenuation(LVecBase3(a, b, c));
        break ;
      }
      case Spot:
      {
        PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

        spot_light->set_attenuation(LVecBase3(a, b, c));
        break ;
      }
      default:
        break ;
    }
  }

  void   SetPosition(LPoint3 position)
  {
      nodePath.set_pos(position);
#ifdef GAME_EDITOR
      symbol.set_pos(position);
#endif
  }

  bool operator==(const std::string& comp) { return (name == comp); }

  void UnSerialize(World*, Utils::Packet& packet);
  void Serialize(Utils::Packet& packet);

  void ReparentTo(World* world);
  void ReparentTo(DynamicObject* object);
  void ReparentTo(MapObject* object);

  MapObject*  Parent(void) const
  {
      return (parent_i);
  }

  std::string name;
  NodePath    nodePath;
  Type        type;
  ParentType  parent_type;
  PT(Light)   light;
  Lens*       lens;
  float       zoneSize;
  bool        enabled;

  std::list<NodePath> enlightened;
#ifdef GAME_EDITOR
  NodePath    symbol;
#endif
private:
  void Initialize(void);
  NodePath    parent;
  MapObject*  parent_i;
};


#endif
