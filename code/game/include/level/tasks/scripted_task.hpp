#ifndef  SCRIPTED_TASK_HPP
# define SCRIPTED_TASK_HPP

# include "globals.hpp"
# include "scheduled_task.hpp"
# include "serializer.hpp"
# include "datatree.hpp"
# include "as_object.hpp"
# include "world/particle_effect.hpp"

class InstanceDynamicObject;

class ScriptedTask : public ScheduledTask
{
  struct Gfx : public Utils::Serializable
  {
    void Initialize(const std::string& name, const std::string& joint = "");
    void SetTarget(InstanceDynamicObject* object);
    void Serialize(Utils::Packet& packet) const;
    void Unserialize(Utils::Packet& packet);

    bool operator==(const std::string& name) const { return (this->name == name); }
    std::string    name;
    std::string    joint;
    ParticleEffect particle_effect;
  };

  typedef std::vector<Gfx> GraphicsEffects;

public:
  ScriptedTask(const std::string& name, InstanceDynamicObject* target);
  ~ScriptedTask();

  const std::string&     GetName(void)   const { return (name);       }
  Data                   GetData(void)         { return (Data(data)); }
  InstanceDynamicObject* GetTarget(void) const { return (target);     }

  void                   AddGfx(const std::string& name, const std::string& joint = "");
  void                   DeleteGfx(const std::string& name);

  void                   Serialize(Utils::Packet&);
  void                   Unserialize(Utils::Packet&);

private:
  void                   Run(void);
  void                   Start(void);
  void                   Finalize(void);
  void                   LoadDataFromString(const std::string&);
  
  AngelScript::Object    object;
  const std::string      name;
  DataTree*              data;
  InstanceDynamicObject* target;
  GraphicsEffects        gfxs;
};

#endif
