#ifndef  SCRIPTED_ZONE_HPP
# define SCRIPTED_ZONE_HPP

# include "level/zones/manager.hpp"
# include "as_object.hpp"

class Level;
class Character;

class ScriptZone : public AngelScript::Object
{
public:
  ScriptZone(Zones::Controller& zone, asIScriptContext*, asIScriptModule*);
  ~ScriptZone();

  const std::string& GetZoneName(void) const;
  void SetExitCallback(const std::string& signature);
  void SetEnterCallback(const std::string& signature);
  void SetMovedWithinZoneCallback(const std::string& signature);
  void SetBlocked(bool blocked)                      { zone.SetZoneBlocked(blocked);   }
  void SetEnabled(bool enabled)                      { zone.SetEnabled(enabled);       }
  bool IsEnabled(void)                         const { return (zone.IsEnabled());      }
  bool IsInside(InstanceDynamicObject* object) const { return (zone.IsInZone(object)); }
  void AddDestination(const std::string& dest)       { zone.AddDestination(dest);      }
  void DelDestination(const std::string& dest)       { zone.DelDestination(dest);      }
  void Delete(void);

  static ScriptZone* Factory(const std::string& zone);

private:
  void                  CallCallback(const std::string& callback, InstanceDynamicObject*);

  Zones::Controller&    zone;
  Sync::ObserverHandler signals;
  bool                  effect_enabled;
};

#endif
