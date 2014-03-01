#include "level/tasks/scripted_task.hpp"
#include "level/objects/instance_dynamic_object.hpp"

using namespace std;

ScriptedTask::ScriptedTask(const string& name, InstanceDynamicObject* target) : object("scripts/tasks/" + name + ".as"), name(name), target(target)
{
  object.asDefineMethod("Run", "void run(Task@)");
  data = new DataTree;
}

ScriptedTask::~ScriptedTask()
{
  if (data)
    delete data;
}

void ScriptedTask::Run(void)
{
  ScheduledTask::Run();
}

void ScriptedTask::Serialize(Utils::Packet& packet)
{
  std::string data_str;
  
  DataTree::Writers::StringJSON(Data(data), data_str);
  ScheduledTask::Serialize(packet);
  packet << data_str;
}

void ScriptedTask::Unserialize(Utils::Packet& packet)
{
  std::string data_str;
  
  ScheduledTask::Unserialize(packet);
  packet >> data_str;
  LoadDataFromString(data_str);
}

void ScriptedTask::LoadDataFromString(const string& str)
{
  DataTree* loaded_data = DataTree::Factory::StringJSON(str);

  if (loaded_data)
  {
    if (data)
      delete data;
    data = loaded_data;
  }
}
