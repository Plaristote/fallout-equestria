#include "level/tasks/task_set.hpp"
#include <executor.hpp>

using namespace std;

TaskSet::~TaskSet()
{
  auto it = begin();
  
  while (it != end())
  {
    delete it->second;
    it = erase(it);
  }
}

ScriptedTask* TaskSet::PushTask(const string& name)
{
  ScriptedTask* task = GetTask(name);

  if (task == 0)
  {
    task = new ScriptedTask(name, target);
    insert(pair<string, ScriptedTask*>(name, task));
  }
  return (task);
}

void TaskSet::RemoveTask(const string& name)
{
  auto it = find(name);
  
  if (it != end())
  {
    ScriptedTask* task = it->second;

    // This needs to be delayed, in case the ScriptedTask itself is doing this
    Executor::ExecuteLater([task]() { delete task; });
    erase(it);
  }
}

ScriptedTask* TaskSet::GetTask(const string& name)
{
  try
  {
    return (at(name));
  }
  catch (...)
  {
  }
  return (0);
}

void TaskSet::Serialize(Utils::Packet& packet)
{
  int  count = size();
  auto it    = begin();
  auto last  = end();

  packet << count;
  for (; it != last ; ++it)
  {
    packet << it->first;
    it->second->Serialize(packet);
  }
}

void TaskSet::Unserialize(Utils::Packet& packet)
{
  int count;
  
  packet >> count;
  for (int i = 0 ; i < count ; ++i)
  {
    string        name;
    ScriptedTask* task;
    
    packet >> name;
    task = new ScriptedTask(name, target);
    insert(pair<string,ScriptedTask*>(name, task));
    task->Unserialize(packet);
  }
}
