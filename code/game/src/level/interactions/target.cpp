#include "level/interactions/target.hpp"
#include "level/level.hpp"

using namespace std;

Interactions::Target::Target(Level* level, DynamicObject* object) : skill_target((InstanceDynamicObject*)this), level(level)
{
  SetInteractionsFromDynamicObject(object);
}

void Interactions::Target::ActionUseSkill(ObjectCharacter* user, const std::string& skill)
{
  skill_target.UseSkill(user, skill);
}

void Interactions::Target::ActionLookAt(InstanceDynamicObject* user)
{
  if (IsPlayer(user))
    DisplayMessage(i18n::T("You see ") + i18n::T(GetName()));
}

void Interactions::Target::ThatDoesNothing(InstanceDynamicObject* user)
{
  if (IsPlayer(user))
    DisplayMessage(i18n::T("That does nothing"));
}

bool Interactions::Target::IsPlayer(InstanceDynamicObject* user) const
{
  return (user == level->GetPlayer());
}

void Interactions::Target::DisplayMessage(const string& message)
{
  level->ConsoleWrite(message);
}

void Interactions::Target::SetInteractionsFromDynamicObject(DynamicObject* object)
{
  Interactions::Player&  player_interactions = level->GetInteractions();
  InstanceDynamicObject* self                = (InstanceDynamicObject*)this;

  if (object->interactions & Interactions::TalkTo)
    interactions.push_back(Interaction("talk_to",    self, &player_interactions.TalkTo));
  if (object->interactions & Interactions::Use)
    interactions.push_back(Interaction("use",        self, &player_interactions.Use));
  if (object->interactions & Interactions::UseObject)
    interactions.push_back(Interaction("use_object", self, &player_interactions.UseObjectOn));
  if (object->interactions & Interactions::UseSkill)
    interactions.push_back(Interaction("use_skill",  self, &player_interactions.UseSkillOn));
  if (object->interactions & Interactions::UseSpell)
    interactions.push_back(Interaction("use_magic",  self, &player_interactions.UseSpellOn));
  if (object->interactions & Interactions::LookAt)
    interactions.push_back(Interaction("use_magic",  self, &player_interactions.LookAt));
}

void Interactions::Target::AddInteraction(const std::string& name, Interactions::Trigger& trigger)
{
  DelInteraction(name);
  interactions.push_back(Interactions::Interaction(name, reinterpret_cast<InstanceDynamicObject*>(this), &trigger));
}

void Interactions::Target::DelInteraction(const std::string& name)
{
  auto it = find(interactions.begin(), interactions.end(), name);
  
  if (it != interactions.end())
    interactions.erase(it);
}

void Interactions::Target::ClearInteractions(void)
{
  interactions.clear();
}
