#ifndef  SCRIPT_ZONE_HPP
# define SCRIPT_ZONE_HPP

# include "level/level_zone.hpp"
# include "as_object.hpp"

class Level;
class Character;

class ScriptZone : public AngelScript::Object
{
public:
  ScriptZone(LevelZone& zone, asIScriptContext*, asIScriptModule*);
  ~ScriptZone();

  void SetExitCallback(const std::string& signature);
  void SetEnterCallback(const std::string& signature);
  void SetEnabled(bool enabled) { zone.SetEnabled(enabled);  }
  bool IsEnabled(void) const    { return (zone.IsEnabled()); }
  bool IsInside(InstanceDynamicObject* object) const { return (zone.IsInside(object)); }
  void Delete(void);

  void SetEffect(const std::string&, int);
  void DisableEffect(void);

  static ScriptZone* Factory(const std::string& zone);

private:
  LevelZone&            zone;
  Sync::ObserverHandler signals;
  bool                  effect_enabled;
};

#endif
