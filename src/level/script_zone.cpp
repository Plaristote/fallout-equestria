#include "level/script_zone.hpp"
#include "level/level.hpp"
#include <executor.hpp>

using namespace std;

ScriptZone* ScriptZone::Factory(const std::string& zone_name, const std::string& callback)
{
  if (Level::CurrentLevel != 0)
  {
    asIScriptContext* context = AngelScript::ContextLock::Context();
    asIScriptModule*  module  = AngelScript::ContextLock::Module();
    LevelZone*        zone    = Level::CurrentLevel->GetZoneByName(zone_name);

    if (context && module && zone)
    {
      ScriptZone* script = new ScriptZone(*zone, context, module, callback);

      if (script->IsDefined("Callback"))
        return (script);
      delete script;
    }
  }
  return (0);
}

ScriptZone::ScriptZone(LevelZone& zone, asIScriptContext* context, asIScriptModule* module, const std::string& callback) :
  AngelScript::Object(context, module), zone(zone)
{
  effect_enable = true;
  asDefineMethod("Callback", callback);
  signals.Connect(zone.Entered, [this](InstanceDynamicObject* object)
  {
    if (IsDefined("Callback"))
    {
      AngelScript::Type<InstanceDynamicObject*> param(object);

      Call("Callback", 1, &param);
    }
  });
  signals.Connect(zone.Exited, [this](InstanceDynamicObject* object)
  {
    if (IsDefined("Exit"))
    {
      AngelScript::Type<InstanceDynamicObject*> param(object);

      Call("Exit", 1, &param);
    }
  });
}

ScriptZone::~ScriptZone()
{
  signals.DisconnectAll();
}

void ScriptZone::SetEffect(const std::string& effect, int time)
{
  std::function<void (void)> apply_effect = [this, effect, time](void)
  {
    try
    {
      AngelScript::Type<ScriptZone*> self(this);

      Call("Effect", 1, &self);
    }
    catch (const AngelScript::Exception&)
    {
    }
    if (effect_enable == true)
      SetEffect(effect, time);
  };

  asDefineMethod("Effect", "void " + effect + "(Zone@)");
  effect_enable = true;
  zone.AddEffect(apply_effect, time);
}

void ScriptZone::DisableEffect(void)
{
  effect_enable = false;
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
