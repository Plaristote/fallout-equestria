#include "level/script_zone.hpp"
#include "level/level.hpp"
#include <executor.hpp>

using namespace std;

ScriptZone* ScriptZone::Factory(const std::string& zone_name)
{
  if (Level::CurrentLevel != 0)
  {
    try
    {
      asIScriptContext*  context = AngelScript::ContextLock::Context();
      asIScriptModule*   module  = AngelScript::ContextLock::Module();
      Zones::Manager&    zones   = Level::CurrentLevel->GetZoneManager();
      Zones::Controller& zone    = zones.GetZone(zone_name);

      if (context && module)
      {
        ScriptZone* script = new ScriptZone(zone, context, module);

        return (script);
      }
    }
    catch (...)
    {
      // TODO proper exception handling
    }
  }
  return (0);
}

ScriptZone::ScriptZone(Zones::Controller& zone, asIScriptContext* context, asIScriptModule* module) :
  AngelScript::Object(context, module), zone(zone)
{
  effect_enabled = true;
  signals.Connect(zone.EnteredZone, [this](InstanceDynamicObject* object)
  {
    CallCallback("Callback", object);
  });
  signals.Connect(zone.ExitedZone, [this](InstanceDynamicObject* object)
  {
    CallCallback("Exit", object);
  });
  signals.Connect(zone.MovedWithinZone, [this](InstanceDynamicObject* object)
  {
    CallCallback("MovedWithinZone", object);
  });
}

ScriptZone::~ScriptZone()
{
  signals.DisconnectAll();
}

void ScriptZone::CallCallback(const std::string& callback, InstanceDynamicObject* object)
{
  if (IsDefined(callback))
  {
    AngelScript::Type<InstanceDynamicObject*> param(object);
    
    Call(callback, 1, &param);
  }
}

void ScriptZone::SetEnterCallback(const string &signature)
{
  asDefineMethod("Callback", signature);
}

void ScriptZone::SetExitCallback(const std::string& signature)
{
  asDefineMethod("Exit", signature);
}

void ScriptZone::Delete(void)
{
  Executor::ExecuteLater([this](void)
  {
    delete this;
  });
}
