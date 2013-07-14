#ifndef  SKILL_TARGET_HPP
# define SKILL_TARGET_HPP

# include "as_object.hpp"
# include <string>

class ObjectCharacter;
class InstanceDynamicObject;

class SkillTarget
{
public:
  SkillTarget(InstanceDynamicObject* self);
  void Initialize(const std::string& module_name, const std::string& filepath, asIScriptContext* context);
  void UseSkill(ObjectCharacter* user, std::string skill);  
private:
  InstanceDynamicObject* self;
  AngelScript::Object*   script;
};

#endif