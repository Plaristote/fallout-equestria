#include "ui/ui_use_skill_on.hpp"
#include <list>
#include <string>

using namespace std;

UiUseSkillOn::UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target) : UiBase(window, context)
{

  _root = context->LoadDocument("data/skill_picker.rml");
  Initialize();
  if (_root)
  {
    StatController*    statistics = player->GetStatController();
    auto               skill_list = statistics->Model().GetUsableSkills(player == target);
    map<string, short> skill_map;

    for_each(skill_list.begin(), skill_list.end(), [&skill_map, &statistics](const std::string& skill)
    {
      pair<string, short> _pair(skill, statistics->Model().GetSkill(skill));
      
      skill_map.insert(_pair);
    });
    SetSkillList(skill_map);
  }
}

UiUseSkillOn::UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/skill_picker.rml");
  Initialize();
}

void UiUseSkillOn::Initialize(void)
{
  if (_root)
  {
    ToggleEventListener(true, "button_cancel", "click", EventCloseClicked);
    EventCloseClicked.EventReceived.Connect([this](Rocket::Core::Event&) { Hide(); Closed.Emit(); });
    EventSkillPicked.EventReceived.Connect([this](Rocket::Core::Event& event)
    {
      Rocket::Core::Variant* var   = event.GetCurrentElement()->GetAttribute("data-skill");
      std::string            skill = var->Get<Rocket::Core::String>().CString();

      Hide();
      SkillPicked.Emit(skill);
      Closed.Emit();
    });
  }
}

void UiUseSkillOn::SetSkillList(const map<string, short>& skill_list)
{
  Rocket::Core::Element* list = _root->GetElementById("skill-list");
  stringstream           stream;

  for_each(skill_list.begin(), skill_list.end(), [&stream](pair<string,short> skill)
  {
    stream << "<div class='item'><button id='pick-skill-" << skill.first << "' class='long_button skill-button' data-skill='" << skill.first << "'>" << skill.first << "</button><span id='pick-skill-" << skill.first << "-points' class='points skill-points'>" << skill.second << "</span></div>";
  });
  list->SetInnerRML(stream.str().c_str());
  for_each(skill_list.begin(), skill_list.end(), [this](pair<string,short> skill)
  {
    string id = "pick-skill-" + skill.first;

    ToggleEventListener(true, id, "click", EventSkillPicked);
  });
}

UiUseSkillOn::~UiUseSkillOn()
{
  ToggleEventListener(false, "button_cancel", "click", EventCloseClicked);
  for_each(skill_list.begin(), skill_list.end(), [this](string skill)
  {
    string id = "pick-skill-" + skill;
    
    ToggleEventListener(false, id, "click", EventSkillPicked);
  });
}

void UiUseSkillOn::Destroy(void)
{
  Hide();
}
 
