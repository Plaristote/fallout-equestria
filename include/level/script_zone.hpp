#ifndef  SCRIPT_ZONE_HPP
# define SCRIPT_ZONE_HPP

# include "level/level_zone.hpp"
# include "as_object.hpp"

class Level;

class ScriptZone : public AngelScript::Object
{
public:
  ScriptZone(LevelZone& zone, asIScriptContext*, asIScriptModule*, const std::string& callback);
  ~ScriptZone();

  void SetExitCallback(const std::string& signature);
  void SetEnabled(bool enabled) { zone.SetEnabled(enabled);  }
  bool IsEnabled(void) const    { return (zone.IsEnabled()); }
  bool IsInside(InstanceDynamicObject* object) const { return (zone.IsInside(object)); }
  void Delete(void);

  static ScriptZone* Factory(const std::string& zone, const std::string& callback);

private:
  LevelZone&            zone;
  Sync::ObserverHandler signals;
};

#endif