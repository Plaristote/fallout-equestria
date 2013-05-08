#ifndef  SKILL_TARGET_HPP
# define SKILL_TARGET_HPP

# include "scriptable.hpp"
# include <string>

class ObjectCharacter;
class InstanceDynamicObject;

class SkillTarget : public Scriptable
{
public:
  SkillTarget(InstanceDynamicObject* self);
  void Initialize(const std::string& module_name, const std::string& filepath, asIScriptContext* context);
  void UseSkill(ObjectCharacter* user, std::string skill);  
private:
  InstanceDynamicObject* self;
  asIScriptFunction*     hook_use_skill;
};

#endif