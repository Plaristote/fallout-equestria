#include "level/tasks/scripted_task.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include "level/level.hpp"

using namespace std;

ScriptedTask::ScriptedTask(const string& name, InstanceDynamicObject* target) : object("scripts/tasks/" + name + ".as"), name(name), target(target)
{
  object.asDefineMethod("Start",    "void start(Task@)");
  object.asDefineMethod("Finalize", "void finalize(Task@");
  object.asDefineMethod("Run",      "void run(Task@)");
  object.asDefineMethod("NextTurn", "void next_turn(Task@)");
  data = new DataTree;
  Start();
}

ScriptedTask::~ScriptedTask()
{
  if (data)
    delete data;
}

void ScriptedTask::Run(void)
{
  if (object.IsDefined("Run"))
  {
    try
    {
      AngelScript::Type<ScriptedTask*> task(this);

      object.Call("Run", 1, &task);
    }
    catch (const std::exception& exception)
    {
      cout << "[AngelScript][ScriptTask][" << name << "][Run] script crashed: " << exception.what() << endl;
    }
  }
  ScheduledTask::Run();
}

void ScriptedTask::Finalize(void)
{
  if (object.IsDefined("Finalize"))
  {
    try
    {
      AngelScript::Type<ScriptedTask*> task(this);

      object.Call("Finalize", 1, &task);
    }
    catch (const std::exception& exception)
    {
      cout << "[AngelScript][ScriptTask][" << name << "][Finalize] script crashed: " << exception.what() << endl;
    }
  }
}

void ScriptedTask::Start(void)
{
  if (object.IsDefined("Start"))
  {
    try
    {
      AngelScript::Type<ScriptedTask*> task(this);

      object.Call("Start", 1, &task);
    }
    catch (const std::exception& exception)
    {
      cout << "[AngelScript][ScriptTask][" << name << "][Start] script crashed: " << exception.what() << endl;
    }
  }
}

void ScriptedTask::NextTurn(void)
{
  if (object.IsDefined("NextTurn"))
  {
    try
    {
      AngelScript::Type<ScriptedTask*> task(this);

      object.Call("NextTurn", 1, &task);
    }
    catch (const std::exception& exception)
    {
      cout << "[AngelScript][ScriptTask][" << name << "][NextTurn] script crashed: " << exception.what() << endl;
    }
  }
}

void ScriptedTask::AddGfx(const string &name, const string &joint)
{
  Gfx gfx;

  gfx.Initialize(name, joint);
  gfx.SetTarget(target);
  if (Level::CurrentLevel)
    Level::CurrentLevel->GetParticleManager().attach_particlesystem(gfx.particle_effect.GetParticleSystem());
  gfxs.push_back(gfx);
}

void ScriptedTask::DeleteGfx(const string &name)
{
  auto it = find(gfxs.begin(), gfxs.end(), name);

  if (it != gfxs.end())
  {
    if (Level::CurrentLevel)
      Level::CurrentLevel->GetParticleManager().remove_particlesystem(it->particle_effect.GetParticleSystem());
    gfxs.erase(it);
  }
}

void ScriptedTask::Serialize(Utils::Packet& packet)
{
  std::string data_str;
  
  DataTree::Writers::StringJSON(Data(data), data_str);
  ScheduledTask::Serialize(packet);
  packet << data_str << gfxs;
}

void ScriptedTask::Unserialize(Utils::Packet& packet)
{
  std::string data_str;
  
  ScheduledTask::Unserialize(packet);
  packet >> data_str >> gfxs;
  LoadDataFromString(data_str);
  for_each(gfxs.begin(), gfxs.end(), [this](Gfx& gfx) { gfx.SetTarget(target); });
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
