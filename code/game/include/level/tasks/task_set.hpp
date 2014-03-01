#ifndef  TASK_SET_HPP
# define TASK_SET_HPP

# include "level/tasks/scripted_task.hpp"
# include <map>

class InstanceDynamicObject;

class TaskSet : public std::map<std::string, ScriptedTask*>
{
public:
  TaskSet(InstanceDynamicObject* target) : target(target) {}
  ~TaskSet();
 
  ScriptedTask* PushTask(const std::string& name);
  void          RemoveTask(const std::string& name);
  ScriptedTask* GetTask(const std::string& name);
  void          Serialize(Utils::Packet&);
  void          Unserialize(Utils::Packet&);

private:
  iterator     Find(const std::string& name);

  InstanceDynamicObject* target;
};

#endif