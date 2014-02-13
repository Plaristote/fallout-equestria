#ifndef  UI_USE_SKILL_ON_HPP
# define UI_USE_SKILL_ON_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"
# include "level/objects/character.hpp"
# include <map>

class UiUseSkillOn : public UiBase
{
public:
  UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target);
  virtual ~UiUseSkillOn();

  void Destroy(void);

  Sync::Signal<void ()>                   Closed;
  Sync::Signal<void (const std::string&)> SkillPicked;
protected:
  UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context);

  void                   SetSkillList(const std::map<std::string, short>& list);
  void                   Initialize(void);
private:
  RocketListener         EventCloseClicked, EventSkillPicked;
  std::string            skill_picked;
  std::list<std::string> skill_list;
};

#endif

