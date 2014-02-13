#ifndef  UI_USE_SPELL_ON_HPP
# define UI_USE_SPELL_ON_HPP

# include "globals.hpp"
# include "ui_use_skill_on.hpp"

class UiUseSpellOn : public UiUseSkillOn
{
public:
  UiUseSpellOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target);
private:
};

#endif

