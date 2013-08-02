#include "level/skill_target.hpp"
#include "level/objectnode.hpp"
#include "level/character.hpp"

SkillTarget::SkillTarget(InstanceDynamicObject* self) : self(self)
{
  script = 0;
}

SkillTarget::~SkillTarget()
{
  if (script != 0)
    delete script;
}

void SkillTarget::Initialize(const std::string& filepath, asIScriptContext* context)
{
  if (context != 0)
    script = new AngelScript::Object(context, filepath);
  else
    script = new AngelScript::Object(filepath);
  script->asDefineMethod("UseSkill", "bool UseSkill(DynamicObject@, Character@, string)");
}

void SkillTarget::UseSkill(ObjectCharacter* user, std::string skill)
{
  if (script != 0 && script->IsDefined("UseSkill"))
  {
    AngelScript::Type<InstanceDynamicObject*> param_self(self);
    AngelScript::Type<ObjectCharacter*>       param_user(user);
    AngelScript::Type<std::string*>           param_skill(&skill);
    bool                                      has_effect;

    has_effect = script->Call("UseSkill", 3, &param_self, &param_user, &param_skill);
    if (has_effect)
      return ;
  }
  user->ThatDoesNothing();
}
