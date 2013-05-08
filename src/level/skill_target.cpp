#include "level/skill_target.hpp"
#include "level/objectnode.hpp"
#include "level/character.hpp"

SkillTarget::SkillTarget(InstanceDynamicObject* self) : self(self)
{
  hook_use_skill = 0;
}

void SkillTarget::Initialize(const std::string& module_name, const std::string& filepath, asIScriptContext* context)
{
  _script_func_ptrs = {
    ScriptFuncPtr(&hook_use_skill, "void UseSkill(DynamicObject@, Character@, string)")
  };
  LoadScript(module_name, filepath, context);
}

void SkillTarget::UseSkill(ObjectCharacter* user, std::string skill)
{
  if (_script_context != 0)
  {
    ReloadFunction(&hook_use_skill);
    if (hook_use_skill)
    {
      _script_context->Prepare(hook_use_skill);
      _script_context->SetArgObject(0, self);
      _script_context->SetArgObject(1, user);
      _script_context->SetArgObject(2, &skill);
      _script_context->Execute();
      return ;
    }
  }
  user->ThatDoesNothing();
}
