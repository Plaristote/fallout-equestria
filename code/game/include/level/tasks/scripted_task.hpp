#ifndef  SCRIPTED_TASK_HPP
# define SCRIPTED_TASK_HPP

# include "globals.hpp"
# include "scheduled_task.hpp"
# include "serializer.hpp"
# include "datatree.hpp"
# include "as_object.hpp"

class InstanceDynamicObject;

class ScriptedTask : public ScheduledTask
{
public:
  ScriptedTask(const std::string& name, InstanceDynamicObject* target);
  ~ScriptedTask();

  const std::string&     GetName(void)   const { return (name);       }
  Data                   GetData(void)         { return (Data(data)); }
  InstanceDynamicObject* GetTarget(void) const { return (target);     }

  void                   Serialize(Utils::Packet&);
  void                   Unserialize(Utils::Packet&);

private:
  void                   Run(void);
  void                   LoadDataFromString(const std::string&);
  
  AngelScript::Object    object;
  const std::string      name;
  DataTree*              data;
  InstanceDynamicObject* target;
};

#endif