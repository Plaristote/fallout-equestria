#include "ui/stat_view_rocket.hpp"
#include "ui/gameui.hpp"

using namespace std;
using namespace Rocket;

StatViewRocket::~StatViewRocket()
{
  ToggleEventListener(false, "continue",               "click", DoneButton);
  ToggleEventListener(false, "cancel",                 "click", CancelButton);
  ToggleEventListener(false, "char-age-edit-ok",       "click", EventAgeChanged);
  ToggleEventListener(false, "char-name-edit-ok",      "click", EventNameChanged);
  ToggleEventListener(false, "char-gender-edit-ok",    "click", EventGenderChanged);
  ToggleEventListener(false, "edit-value-cursor-plus", "click", ButtonUp);
  ToggleEventListener(false, "edit-value-cursor-less", "click", ButtonDown);
  ToggleEventListener(false, "edit-value-cursor-less", "click", ButtonDown);
  ToggleEventListener(false, "special",                "click", EventSpecialClicked);
  ToggleEventListener(false, "body",                   "click", EventSkillClicked);
  ToggleEventListener(false, "char-name",              "click", EventGeneralClicked);
  ToggleEventListener(false, "char-age",               "click", EventGeneralClicked);
  ToggleEventListener(false, "char-gender",            "click", EventGeneralClicked);
  for_each(_traits.begin(), _traits.end(), [this](Core::Element* trait)
  { trait->RemoveEventListener("click", &EventTraitClicked); });
}

StatViewRocket::StatViewRocket(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context), _perks_dialog(window, context)
{
  root     = context->LoadDocument("data/charsheet.rml");

  if (root)
  {
    _perks_dialog.PerkSelected.Connect(UpdatePerkDescription, &Sync::Signal<void (const std::string&)>::Emit);

    ToggleEventListener(true, "continue", "click", DoneButton);
    ToggleEventListener(true, "cancel",   "click", CancelButton);
    DoneButton.EventReceived.Connect  (*this, &StatViewRocket::Accept);
    CancelButton.EventReceived.Connect(*this, &StatViewRocket::Cancel);
    EventDetails.EventReceived.Connect(*this, &StatViewRocket::DisplayDetails);

    // Perks Dialog
    _perks_dialog.PerkChoosen.Connect (PerkToggled, &Sync::Signal<void (const string&)>::Emit);

    // Edit Mode
    EventSpecialClicked.EventReceived.Connect(*this, &StatViewRocket::SpecialClicked);
    EventSkillClicked.EventReceived.Connect  (*this, &StatViewRocket::SkillClicked);
    EventGeneralClicked.EventReceived.Connect(*this, &StatViewRocket::GeneralClicked);

    EventAgeChanged.EventReceived.Connect   (*this, &StatViewRocket::UpdateAge);
    EventNameChanged.EventReceived.Connect  (*this, &StatViewRocket::UpdateName);
    EventGenderChanged.EventReceived.Connect(*this, &StatViewRocket::UpdateGender);

    EventTraitClicked.EventReceived.Connect (*this, &StatViewRocket::TraitClicked);
    
    {
      function<void (const Core::Event&)> functor = [this](const Core::Event&)
      {
        Core::Element*   element =  this->context->GetHoverElement();
        Core::Dictionary dictionary;
        Core::Event      event(element, "click", dictionary);

        DisplayDetails(event);
      };

      EventStatisticClicked.EventReceived.Connect(functor);
      EventPerkClicked.EventReceived.Connect(functor);
    }

    ToggleEventListener(true, "char-age-edit-ok",       "click", EventAgeChanged);
    ToggleEventListener(true, "char-name-edit-ok",      "click", EventNameChanged);
    ToggleEventListener(true, "char-gender-edit-ok",    "click", EventGenderChanged);
    ToggleEventListener(true, "edit-value-cursor-plus", "click", ButtonUp);
    ToggleEventListener(true, "edit-value-cursor-less", "click", ButtonDown);
    ToggleEventListener(true, "team-panel",             "click", PartyMemberClicked);
    ButtonUp.EventReceived.Connect  (*this, &StatViewRocket::StatMore);
    ButtonDown.EventReceived.Connect(*this, &StatViewRocket::StatLess);

    PartyMemberClicked.EventReceived.Connect([this](Rocket::Core::Event& event)
    {
      Core::Element* elem = this->context->GetHoverElement();

      while (elem != 0 && elem != event.GetCurrentElement() && elem->GetClassNames() != "party-member")
        elem = elem->GetParentNode();
      if (elem != 0 && elem->GetId() != "team-panel")
        SwapToPartyMember.Emit(elem->GetId().CString());
      else
        std::cout << "Unable to find the character clicked sir" << std::endl;
    });

    SetEditMode(Display);
    Translate();
  }
}

void StatViewRocket::SetReputation(const std::string& faction, int reputation)
{
  std::string reputation_level;

  if (reputation < 0)
    reputation_level = "bad";
  else
    reputation_level = "good";
  SetFieldValue("Reputation", faction, reputation_level);
}

void StatViewRocket::SetPartyMembers(const std::vector<std::string>& members)
{
  Rocket::Core::Element* team_panel = root->GetElementById("team-panel");
  
  if (team_panel)
  {
    std::stringstream rml;
    
    for_each(members.begin(), members.end(), [&rml](std::string member)
    {
      rml << "<div class='party-member' id='" << member << "'>";
      rml << "<img src='../textures/avatars/" << member << ".png'><br />";
      rml << "<span class='party-member-name'>" << member << "</span>";
      rml << "</div>";
    });
    team_panel->SetInnerRML(rml.str().c_str());
  }
}

void StatViewRocket::UpdateAge(Core::Event& event)
{
  Core::Element* age_field = root->GetElementById("char-age-value");
  
  if (age_field)
  {
    Core::Variant* var = age_field->GetAttribute("value");
    
    if (var)
    {
      stringstream   stream;
      string         str_age = var->Get<Core::String>().CString();
      unsigned short age;

      stream << str_age;
      stream >> age;
      if (age > 15)
        AgeChanged.Emit(age);
    }
  }
  event.GetCurrentElement()->GetParentNode()->SetProperty("display", "none");
}

void StatViewRocket::UpdateName(Core::Event& event)
{
  Core::Element* name_field = root->GetElementById("char-name-value");
  
  if (name_field)
  {
    Core::Variant* var = name_field->GetAttribute("value");
    
    if (var)
    {
      string name = var->Get<Core::String>().CString();
      
      InformationChanged.Emit("name", name);
    }
  }
  event.GetCurrentElement()->GetParentNode()->SetProperty("display", "none");
}

void StatViewRocket::UpdateGender(Core::Event& event)
{
  Core::Element* gender_edit   = root->GetElementById("char-gender-edit");
  Core::Element* gender_male   = root->GetElementById("char-gender-option-male");
  Core::Element* gender_female = root->GetElementById("char-gender-option-female");
  
  if (gender_male && gender_female)
  {
    Core::String   is_male, is_female;
    Core::Variant* var_male   = gender_male->GetAttribute("checked");
    Core::Variant* var_female = gender_female->GetAttribute("checked");
    string         result     = "male";

    if (var_male)   is_male   = var_male->Get<Core::String>();
    if (var_female) is_female = var_female->Get<Core::String>();

    if ((!var_male && var_female) || is_female == "true") result = "female";
    InformationChanged.Emit("gender", result);
  }
  if (gender_edit)
    gender_edit->SetProperty("display", "none");
}

void        StatViewRocket::StatUpdate(Core::Event& event, string& ret_type, string& ret_stat)
{
  Core::Element* cursor    = root->GetElementById("edit-value-cursor");

  if (cursor)
  {
    Core::Element* element = cursor->GetParentNode();
    Core::String   stat, type;
    Core::Variant* var_type;

    element->GetInnerRML(stat);
    element->GetChild(0)->GetInnerRML(stat);
    var_type = element->GetAttribute("data-type");
    if (var_type) type = var_type->Get<Core::String>();
    var_type = element->GetAttribute("data-key");
    if (var_type) stat = var_type->Get<Core::String>();
    ret_stat = stat.CString();
    ret_type = type.CString();
  }
}

void        StatViewRocket::StatMore(Core::Event& event)
{
  string    type, stat;
  
  StatUpdate(event, type, stat);
  StatUpped.Emit(type, stat);
}

void        StatViewRocket::StatLess(Core::Event& event)
{
  string    type, stat;

  StatUpdate(event, type, stat);
  StatDowned.Emit(type, stat);
}

void StatViewRocket::SpecialClicked(Core::Event& event)
{
  Core::Element* cursor = root->GetElementById("edit-value-cursor");

  DisplayDetails(event);
  if (cursor)
  {
    Core::Element* current = context->GetHoverElement();

    cursor->SetProperty("display", "block");
    current = current->GetParentNode();
    if (cursor->GetParentNode() != current && current->GetClassNames() == "special-group")
    {
      cursor->GetParentNode()->RemoveChild(cursor);
      current->AppendChild(cursor);
    }
  }
}

void StatViewRocket::DisplayDetails(Core::Event& event)
{
  cout << "[CMAP] DisplayDetails" << endl;
  Core::Element* element   = context->GetHoverElement();
  Rocket::Core::String t;
  element->GetParentNode()->GetInnerRML(t);
  cout << element->GetClassNames().CString() << " -> " << t.CString() << endl;
  Core::Variant* var_type  = element->GetAttribute("data-details-type");
  Core::Variant* var_value = element->GetAttribute("data-details-value");

  if (var_type && var_value)
  {
    string type  = var_type->Get<Core::String>().CString();
    string value = var_value->Get<Core::String>().CString();

    UpdateDetails.Emit(type, value);
  }
}

void StatViewRocket::SetDetails(const string& icon, const string& title, const string& text)
{
  Core::Element* elem_icon  = root->GetElementById("details-icon");
  Core::Element* elem_title = root->GetElementById("details-title");
  Core::Element* elem_text  = root->GetElementById("details-text");

  if (elem_icon)  elem_icon->SetInnerRML(Core::String("<img src='") + icon.c_str() + "'/>");
  if (elem_title) elem_title->SetInnerRML(title.c_str());
  if (elem_text)  elem_text->SetInnerRML(text.c_str());
}

void StatViewRocket::SkillClicked(Core::Event& event)
{
  Core::Element* current = context->GetHoverElement();

  while (current && current->GetClassNames() != "skill-datagrid")
    current = current->GetParentNode();
  if (current)
  {
    DisplayDetails(event);
    if (_editMode == StatView::Create)
    {
      Core::Variant* var = current->GetAttribute("data-key");
      Core::String   str = (var ? var->Get<Core::String>() : "");

      ToggleSkillAffinity.Emit(str.CString());
    }
    else if (_editMode == StatView::Update)
    {
      Core::Element* cursor = root->GetElementById("edit-value-cursor");
      
      if (cursor)
      {
        cursor->SetProperty("display", "block");
        cursor->GetParentNode()->RemoveChild(cursor);
        current->AppendChild(cursor);
      }
    }
  }
}

void StatViewRocket::GeneralClicked(Rocket::Core::Event& event)
{
  Core::String   id     = event.GetCurrentElement()->GetId();
  Core::Element* name   = root->GetElementById("char-name-edit");
  Core::Element* age    = root->GetElementById("char-age-edit");
  Core::Element* gender = root->GetElementById("char-gender-edit");

  name->SetProperty  ("display", "none");
  age->SetProperty   ("display", "none");
  gender->SetProperty("display", "none");
  if      (id == "char-name")   name->SetProperty  ("display", "block");
  else if (id == "char-age")    age->SetProperty   ("display", "block");
  else if (id == "char-gender") gender->SetProperty("display", "block");
}

void StatViewRocket::SetEditMode(EditMode mode)
{
  const char*    createElems[]  = { "continue", "cancel", "special-points-title", 0 };
  const char*    updateElems[]  = { "continue", "cancel", "experience", "skill-points-title", 0 };
  const char*    displayElems[] = { "cancel",   "experience", 0 };
  const char**   toShow         = 0;
  const char**   elemArrays[]   = { createElems, updateElems, displayElems, 0 };
  Core::Element* cursor         = root->GetElementById("edit-value-cursor");
  Core::Element* special        = root->GetElementById("special");
  Core::Element* skill          = root->GetElementById("body");
  Core::Element* traits_create  = root->GetElementById("panel-traits-create");
  Core::Element* traits         = root->GetElementById("panel-traits-default");
  Core::Element* perks          = root->GetElementById("panel-perks");
  Core::Element* statistics     = root->GetElementById("statistics");

  Core::Element* name   = root->GetElementById("char-name");
  Core::Element* age    = root->GetElementById("char-age");
  Core::Element* gender = root->GetElementById("char-gender");

  if (cursor)     cursor->SetProperty("display", "none");
  if (special)    special->RemoveEventListener   ("click", &EventSpecialClicked);
  if (skill)      skill->RemoveEventListener     ("click", &EventSkillClicked);
  if (perks)      perks->RemoveEventListener     ("click", &EventPerkClicked);
  if (statistics) statistics->RemoveEventListener("click", &EventStatisticClicked);

  if (name)       name->RemoveEventListener      ("click", &EventGeneralClicked);
  if (age)        age->RemoveEventListener       ("click", &EventGeneralClicked);
  if (gender)     gender->RemoveEventListener    ("click", &EventGeneralClicked);

  if (perks)      perks->AddEventListener        ("click", &EventPerkClicked);
  if (statistics) statistics->AddEventListener   ("click", &EventStatisticClicked);
  switch (mode)
  {
    case Create:
      if (special) special->AddEventListener("click", &EventSpecialClicked);
      if (name)    name->AddEventListener   ("click", &EventGeneralClicked);
      if (age)     age->AddEventListener    ("click", &EventGeneralClicked);
      if (gender)  gender->AddEventListener ("click", &EventGeneralClicked);
      if (skill)   skill->AddEventListener  ("click", &EventSkillClicked);
      if (traits_create) traits_create->SetProperty("display", "block");
      if (traits)        traits->SetProperty       ("display", "none");
      toShow = createElems;
      break ;
    case Update:
      if (skill) skill->AddEventListener  ("click", &EventSkillClicked);
      if (traits_create) traits_create->SetProperty("display", "none");
      if (traits)        traits->SetProperty       ("display", "block");
      toShow = updateElems;
      break ;
    case Display:
      if (traits_create) traits_create->SetProperty("display", "none");
      if (traits)        traits->SetProperty       ("display", "block");
      toShow = displayElems;
      break ;
  }
  for (unsigned int ii = 0 ; elemArrays[ii] != 0 ; ++ii)
  {
    const char** elemArray = elemArrays[ii];

    if (elemArray == toShow) continue ;
    for (unsigned int i = 0 ; elemArray[i] != 0 ; ++i)
    {
      Core::Element* elem = root->GetElementById(elemArray[i]);

      if (elem) elem->SetProperty("display", "none");
    }
  }
  for (unsigned int i = 0 ; toShow[i] != 0 ; ++i)
  {
    Core::Element* elem = root->GetElementById(toShow[i]);

    if (elem) elem->SetProperty("display", "block");
  }
  _editMode = mode;
}

void StatViewRocket::SetInformation(const std::string& name, const std::string& value)
{
  Core::Element* element;
  string         id = "char-" + underscore(name);
  
  element = root->GetElementById(id.c_str());
  if (element)
    element->SetInnerRML(value.c_str());
}

void StatViewRocket::SetInformation(const std::string& name, short value)
{
  stringstream strValue;
  
  strValue << value;
  SetInformation(name, strValue.str());
}

void StatViewRocket::SetFieldValue(const std::string& category, const std::string& key, const std::string& value)
{
  Core::Element* element;
  string         strId;

  strId = underscore(category) + "-value-" + underscore(key);
  if ((element = root->GetElementById(strId.c_str())))
    element->SetInnerRML(value.c_str());
  else
  {
    if (category == "Kills")
    {
      stringstream rml;
      Core::String old_rml;

      if ((element = root->GetElementById("panel-kills")))
      {
        element->GetInnerRML(old_rml);
        rml << "<datagrid>";
        rml << "<col width='80%'><span class='kills-key' i18n='" << key << "'>" << i18n::T(key) << "</span></col>";
        rml << "<col width='20%'><span class='kills-value' id='" << strId << "'>" << value << "</span></col>";
        rml << "</datagrid>";
        element->SetInnerRML(old_rml + rml.str().c_str());
      }
    }
    else if (category == "Reputation")
    {
      stringstream rml;
      Core::String old_rml;

      if ((element = root->GetElementById("panel-reputation")))
      {
        element->GetInnerRML(old_rml);
        rml << "<datagrid>";
        rml << "<col width='80%'><span class='reputation-key' i18n='" << key << "'>" << i18n::T(key) << "</span></col>";
        rml << "<col width='20%'><span class='reputation-value' id='" << strId << "'>" << i18n::T(value) << "</span></col>";
        rml << "</datagrid>";
        element->SetInnerRML(old_rml + rml.str().c_str());
      }
    }
    cout << "[Warning] Element '" << strId << "' should exist but doesn't" << endl;
  }
}

void StatViewRocket::SetFieldValue(const std::string& category, const std::string& key, short value)
{
  stringstream   strValue;

  strValue << value;
  SetFieldValue(category, key, strValue.str());
  
  if (category == "Special")
  {
    string         comm = "Very bad";
    string         id   = "special-commt-" + underscore(key);
    Core::Element* elem = root->GetElementById(id.c_str());
    
    if (elem)
    {
      if (value > 2)
        comm = "Bad";
      if (value > 4)
        comm = "Average";
      if (value > 6)
        comm = "Good";
      if (value > 8)
        comm = "Great";
      if (value > 9)
        comm = "Heroic";
      elem->SetInnerRML(comm.c_str());
    }
  }
}

void StatViewRocket::SetIdValue(const std::string& id, short value)
{
  Core::Element* element = root->GetElementById(id.c_str());
  
  if (element)
  {
    stringstream rml;
    
    rml << value;
    element->SetInnerRML(rml.str().c_str());
  }
}

void StatViewRocket::SetIdValue(const std::string& id, const std::string& value)
{
  Core::Element* element = root->GetElementById(id.c_str());
  
  if (element)
    element->SetInnerRML(value.c_str());
}

void StatViewRocket::SetExperience(unsigned int xp, unsigned short lvl, unsigned int next_level)
{
  if (root)
  {
    Core::Element* element = root->GetElementById("experience");
    
    if (element)
    {
      stringstream rml;
      
      rml << "<p class='current-level-label'>Level: <span id='level'>" << lvl << "</span></p>";
      rml << "<p class='current-xp-label'>Experience: <span id='current-xp'>" << xp << "</span></p>\n";
      rml << "<p class='next-level-label'>Next level: <span id='next-level'>" << next_level << "</span></p>\n";
      element->SetInnerRML(rml.str().c_str());
    }
  }
}

void StatViewRocket::TraitClicked(Core::Event& event)
{
  Core::Element* element = event.GetCurrentElement();
  
  if (element && _editMode == StatView::Create)
  {
    string trait = humanize(element->GetId().CString());

    TraitToggled.Emit(trait);
  }
}

void StatViewRocket::SetSkillAffinity(const string& skill, bool active)
{
  if (root)
  {
    string         elem_id = "skill-datagrid-" + underscore(skill);
    Core::Element* elem    = root->GetElementById(elem_id.c_str());

    if (elem)
      elem->SetProperty("color", (active ? "yellow" : "white"));
  }
}

void StatViewRocket::SetTraitActive(const string& trait, bool active)
{
  if (root)
  {
    // Create Traits Interface
    {
      string         elem_id = "text-" + underscore(trait);
      Core::Element* elem    = root->GetElementById(elem_id.c_str());

      if (elem)
        elem->SetProperty("color", (active ? "yellow" : "white"));
    }
    // Display Traits Interface
    {
      string         elem_id = "display-trait-" + underscore(trait);
      Core::Element* elem    = root->GetElementById(elem_id.c_str());
      
      if (elem)
        elem->SetProperty("display", active ? "block" : "none");
    }
  }
}

void StatViewRocket::SetTraits(list<string> traits)
{
  if (root)
  {
    Core::Element* create_element  = root->GetElementById("panel-traits-create");
    Core::Element* display_element = root->GetElementById("panel-traits-default");

    stringstream create_rml, display_rml;

    for_each(traits.begin(), traits.end(), [this, &create_rml, &display_rml](const string trait)
    {
      string details = "data-details-type='traits' data-details-value='" + underscore(trait) + "'";
      
      create_rml  << "<div class='traits-row'><button id='" << underscore(trait) << "' class='small_button'>&nbsp;</button>";
      create_rml  << "<span class='text-trait' id='trait-" << underscore(trait) << "' " << details << " >" << i18n::T(trait) << "</span><br /></div>";
      display_rml << "<span class='text-trait' id='display-trait-" << underscore(trait) << "' " << details << "  style='display:none;'>";
      display_rml << i18n::T(trait) << "</span><br />";
    });
    if (create_element)  create_element->SetInnerRML(create_rml.str().c_str());
    if (display_element) display_element->SetInnerRML(display_rml.str().c_str());

    _traits.clear();
    for_each(traits.begin(), traits.end(), [this](const string trait)
    {
      string button_id = underscore(trait);
      string display_text_id = "display-trait-" + button_id;
      string text_id         = "trait-" + button_id;

      ToggleEventListener(true, button_id,       "click", EventTraitClicked);
      ToggleEventListener(true, display_text_id, "click", EventDetails);
      ToggleEventListener(true, text_id,         "click", EventDetails);
      _traits.push_back(root->GetElementById(button_id.c_str()));
    });
  }
}

void StatViewRocket::SetCategoryFields(const std::string& category, const std::vector<std::string>& keys)
{
  if (root)
  {
    Core::Element* element = root->GetElementById(underscore(category).c_str());
    stringstream   rml;

    if (element)
    {
      unsigned short topX = 0;

      for (unsigned int i = 0 ; i < keys.size() ; ++i)
      {
        string underscored  = underscore(keys[i]);
        string details_data = "data-details-type='" + underscore(category) + "' data-details-value='" + underscore(keys[i]) + '\'';

        if (category == "Special")
        {
          rml << "<p class='special-group' data-type='Special' data-key='" << keys[i] << "' style='top: " << topX << "px;'>\n";
          rml << "  <p class='special-key' " << details_data << ">" << keys[i] << "</p>\n";
          rml << "  <p class='special-value console-value' id='special-value-" << underscored << "'>0</p>\n";
          rml << "  <p class='special-commt console-value' id='special-commt-" << underscored << "'>Great</p>\n";
          rml << "</p>\n\n";
          topX += 40;
        }
        else if (category == "Statistics")
        {
          rml << "<datagrid  class='statistics-datagrid'>\n";
          rml << "  <col width='80%'><span class='statistics-key' " << details_data << ">" << i18n::T(keys[i]) << "</span></col>\n";
          rml << "  <col width='15%'><span class='statistics-value' id='statistics-value-" << underscored << "'></span></col>\n";
          rml << "</datagrid>\n\n";
        }
        else if (category == "Skills")
        {
          rml << "<div id='skill-datagrid-" << underscored << "' class='skill-datagrid' data-type='Skills' data-key='" << keys[i] << "'>\n";
          rml << "  <div class='skill-key' " << details_data << ">" << i18n::T(keys[i]) << "</div>\n";
          rml << "  <div class='skill-percent'>%</div>";
          rml << "  <div class='skill-value' id='skills-value-" << underscored << "'>0</div>\n";
          rml << "</div>\n\n";
        }
      }
      element->SetInnerRML(rml.str().c_str());
    }
  }
}

void StatViewRocket::Show(void)
{
  if (_editMode == StatView::Update)
    MakeBackup.Emit();
  UiBase::Show();
  if (_n_perks > 0)
    _perks_dialog.Show();
}

void StatViewRocket::Hide(void)
{
  _perks_dialog.Hide();
  UiBase::Hide();  
}

void StatViewRocket::SetPerks(list<string> perks)
{
  if (root)
  {
    Core::Element* panel_perks = root->GetElementById("panel-perks");
    stringstream   rml;

    for_each(perks.begin(), perks.end(), [&rml](const string& perk)
    {
      string details_data = "data-details-type='perks' data-details-value='" + underscore(perk) + '\'';

      rml << "- <span " << details_data << '>' << perk << "</span><br />" << endl;
    });
    panel_perks->SetInnerRML(rml.str().c_str());
  }
}

void StatViewRocket::SetAvailablePerks(list<string> perks)
{
  _perks_dialog.SetAvailablePerks(perks);
  if (_n_perks <= 0)
    _perks_dialog.Hide();
}

void StatViewRocket::SetPerkDescription(const string& icon, const string& description)
{
  _perks_dialog.SetPerkDescription(icon, description);
}

/*
 * PerksDialog
 */
StatViewRocket::PerksDialog::PerksDialog(WindowFramework* w, Core::Context* c) : UiBase(w, c)
{
  root = c->LoadDocument("data/perks_menu.rml");
  if (root)
  {
    ToggleEventListener(true, "cancel", "click", Cancel);
    ToggleEventListener(true, "select", "click", ChoosePerk);
    DblClickPerk.EventReceived.Connect(*this, &PerksDialog::CallbackDblClickPerk);
    Cancel.EventReceived.Connect      (*this, &PerksDialog::CallbackCancel);
    SelectPerk.EventReceived.Connect  (*this, &PerksDialog::SetSelectedPerk);
    ChoosePerk.EventReceived.Connect  (*this, &PerksDialog::CallbackChoosePerk);
    Translate();
  }
  else
    cerr << "[StatViewRocket][PerksPicker] Missing RML Template" << endl;
}

StatViewRocket::PerksDialog::~PerksDialog()
{
  ToggleEventListener(false, "cancel", "click", Cancel);
  ToggleEventListener(false, "select", "click", ChoosePerk);
  ClearPerksButtons();
}

void StatViewRocket::PerksDialog::CallbackCancel(Core::Event&)
{
  Hide();
}

void StatViewRocket::PerksDialog::SetSelectedPerk(Core::Event& event)
{
  Core::Element* element   = event.GetCurrentElement();
  Core::Variant* variant   = element->GetAttribute("data-perk");
  string         data_perk = variant->Get<Core::String>().CString();
  
  _selected_perk = data_perk;
  PerkSelected.Emit(humanize(data_perk));
}

void StatViewRocket::PerksDialog::CallbackChoosePerk(Core::Event&)
{
  PerkChoosen.Emit(humanize(_selected_perk));
}

void StatViewRocket::PerksDialog::CallbackDblClickPerk(Core::Event& event)
{
  SetSelectedPerk(event);
  CallbackChoosePerk(event);
}

void StatViewRocket::PerksDialog::ClearPerksButtons(void)
{
  for_each(_perks_buttons.begin(), _perks_buttons.end(), [this](Core::Element* element)
  {
    element->RemoveEventListener("click",    &SelectPerk);
    element->RemoveEventListener("dblclick", &DblClickPerk);
  });
  _perks_buttons.clear();
}

void StatViewRocket::PerksDialog::SetAvailablePerks(list<string> perks)
{
  if (_perks_buttons.size())
    ClearPerksButtons();
  if (root)
  {
    Core::Element* element = root->GetElementById("perks-selector");
    
    if (element)
    {
      stringstream rml;
      
      for_each(perks.begin(), perks.end(), [this, &rml](const string& perk)
      {
        rml << "- <button id='perk-picker-" << underscore(perk) << "' data-perk='" << underscore(perk) << "'>" << perk << "</button><br />" << endl;
      });
      element->SetInnerRML(rml.str().c_str());
      for_each(perks.begin(), perks.end(), [this, element](const string& perk)
      {
        string         id          = "perk-picker-" + underscore(perk);
        Core::Element* perk_picker = element->GetElementById(id.c_str());

        if (perk_picker)
        {
          perk_picker->AddEventListener("click",    &SelectPerk);
          perk_picker->AddEventListener("dblclick", &DblClickPerk);
          _perks_buttons.push_back(perk_picker);
        }
        else
          cerr << "[StatViewRocket][PerksPicker] Fatal Error" << endl;
      });
    }
  }
}

void StatViewRocket::PerksDialog::SetPerkDescription(const string& icon, const string& description)
{
  cout << "Set Perk Description" << endl;
  if (root)
  {
    cout << "Has root" << endl;
    Core::Element* element = root->GetElementById("perks-description");
    
    if (element)
      element->SetInnerRML(description.c_str());
  }
}
