#ifndef  PARTICLE_OBJECT_HPP
# define PARTICLE_OBJECT_HPP

# include "particle_effect.hpp"
# include "map_object.hpp"
# include "dynamic_object.hpp"

class ParticleObject : public ParticleEffect, public Utils::Serializable
{
public:
  enum ParentType
  {
    NONE,
    MAP_OBJECT,
    DYNAMIC_OBJECT
  };

  ParticleObject() : parent_type(NONE)
  {}

  bool               operator==(const std::string& name) const { return (this->name == name); }
  const std::string& GetParticleEffectName(void)         const { return (particle_effect_name); }
  const std::string& GetName(void)                       const { return (name); }
  void               SetName(const std::string& name)          { this->name = name; }

  void SetParticleEffect(const std::string& name)
  {
    particle_effect_name = name;
    LoadConfiguration("data/particle-effects/" + name);
  }

  void ReparentTo(MapObject* map_object)
  {
    if (map_object != 0)
    {
      parent_name = map_object->name;
      parent_type = MAP_OBJECT;
      ParticleEffect::ReparentTo(map_object->nodePath);
    }
    else
    {
      parent_name = "";
      parent_type = NONE;
    }
  }

  void ReparentTo(DynamicObject* dyn_object)
  {
    ReparentTo((MapObject*)dyn_object);
    if (dyn_object != 0)
      parent_type = DYNAMIC_OBJECT;
  }

  void Serialize(Utils::Packet& packet) const
  {
    packet << name << particle_effect_name << parent_name << (char)parent_type;
  }

  void Unserialize(Utils::Packet& packet)
  {
    char parent_type;

    packet >> name >> particle_effect_name >> parent_name >> parent_type;
    this->parent_type = (ParentType)parent_type;
  }

  std::string parent_name;
  ParentType  parent_type;
private:
  std::string name;
  std::string particle_effect_name;
};
#endif
