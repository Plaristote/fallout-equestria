#include "level/interactions/target.hpp"
#include "level/level.hpp"

using namespace std;

Interactions::Target::Target(Level* level, DynamicObject* object) : skill_target((InstanceDynamicObject*)this), level(level), script(0)
{
  SetInteractionsFromDynamicObject(object);
}

void Interactions::Target::SetupScript(AngelScript::Object* script)
{
  this->script = script;
  script->asDefineMethod("LookAt", "bool look_at(DynamicObject@, Character@)");
  script->asDefineMethod("Use",    "void use(DynamicObject@, Character@)");
  script->asDefineMethod("TalkTo", "bool talk_to(DynamicObject@, Character@)");
}

void Interactions::Target::ActionUseSkill(ObjectCharacter* user, const std::string& skill)
{
  skill_target.UseSkill(user, skill);
}

void Interactions::Target::ActionLookAt(InstanceDynamicObject* user)
{
  if (IsPlayer(user))
  {
    bool script_success = false;

    if (script && script->IsDefined("LookAt"))
    {
      AngelScript::Type<InstanceDynamicObject*> param_self(static_cast<InstanceDynamicObject*>(this));
      AngelScript::Type<ObjectCharacter*>       param_player(user->Get<ObjectCharacter>());

      script_success = script->Call("LookAt", 2, &param_self, &param_player);
    }
    if (!script_success)
    {
      const InstanceDynamicObject* self = static_cast<const InstanceDynamicObject*>(this);

      if (self->Get<const ObjectCharacter>())
        DisplayMessage(i18n::T("You see ") + i18n::T(self->Get<const ObjectCharacter>()->GetStatController()->GetData()["Name"].Value()));
      else
        DisplayMessage(i18n::T("You see ") + i18n::T(GetName()));
    }
  }
}

void Interactions::Target::ActionUse(InstanceDynamicObject* user)
{
  cout << "ActionUse instancedynamicobject" << endl;
  if (script && script->IsDefined("Use"))
  {
    AngelScript::Type<InstanceDynamicObject*> param_self(static_cast<InstanceDynamicObject*>(this));
    AngelScript::Type<ObjectCharacter*>       param_player(user->Get<ObjectCharacter>());

    script->Call("Use", 2, &param_self, &param_player);
  }
  else
    ThatDoesNothing(user);
}

bool Interactions::Target::TryToStartConversation(ObjectCharacter* user)
{
  InstanceDynamicObject* self = static_cast<InstanceDynamicObject*>(this);
  bool                   open_dialog = self->GetDynamicObject()->dialog != "";

  cout << "Dialog: " << self->GetDynamicObject()->dialog << endl;
  if (script && script->IsDefined("TalkTo"))
  {
    AngelScript::Type<InstanceDynamicObject*> param_self(self);
    AngelScript::Type<ObjectCharacter*>       param_player(user);

    open_dialog = open_dialog && (bool)(script->Call("TalkTo", 2, &param_self, &param_player));
    cout << "open_dialog: " << open_dialog << endl;
  }
  return (open_dialog);
}

void Interactions::Target::ActionTalkTo(ObjectCharacter* user)
{
  cout << "ActionTalkTo" << endl;
  if (TryToStartConversation(user))
    Level::CurrentLevel->GetLevelUi().OpenUiDialog(static_cast<InstanceDynamicObject*>(this));
  else
    ThatDoesNothing(user);
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
  level->GetLevelUi().GetMainBar().AppendToConsole(message);
}

void Interactions::Target::SetInteractionsFromDynamicObject(DynamicObject* object)
{
  Interactions::Player&  player_interactions = level->GetInteractions();
  InstanceDynamicObject* self                = (InstanceDynamicObject*)this;

  ClearInteractions();
  if (object->interactions & Interactions::TalkTo)
    interactions.push_back(Interaction("talk_to",    self, &player_interactions.TalkTo));
  if (object->interactions & Interactions::Use)
    interactions.push_back(Interaction("use",        self, &player_interactions.Use));
  if (object->interactions & Interactions::LookAt)
    interactions.push_back(Interaction("look",       self, &player_interactions.LookAt));
  if (object->interactions & Interactions::UseObject)
    interactions.push_back(Interaction("use_object", self, &player_interactions.UseObjectOn));
  if (object->interactions & Interactions::UseSkill)
    interactions.push_back(Interaction("use_skill",  self, &player_interactions.UseSkillOn));
  if (object->interactions & Interactions::UseSpell)
    interactions.push_back(Interaction("use_magic",  self, &player_interactions.UseSpellOn));
}

void Interactions::Target::AddInteraction(const std::string& name, Interactions::Trigger& trigger)
{
  DelInteraction(name);
  interactions.push_back(Interactions::Interaction(name, static_cast<InstanceDynamicObject*>(this), &trigger));
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
