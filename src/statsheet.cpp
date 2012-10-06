#include "statsheet.hpp"

using namespace std;

/*
 * StatModel
 */
StatModel::StatModel(Data statsheet) : _statsheet(statsheet)
{
  _scriptContext = Script::Engine::Get()->CreateContext();
  LoadFunctions();
}

StatModel::~StatModel(void)
{
  if (_scriptContext) _scriptContext->Release();
}

void StatModel::ReloadFunction(asIScriptFunction** pointer)
{
  *pointer = 0;
  if (_scriptContext && _scriptModule)
  {
    ScriptFuncPtrs::iterator cur, end;

    for (cur = _script_func_ptrs.begin(), end = _script_func_ptrs.end() ; cur != end ; ++cur)
    {
      if (cur->first == pointer)
      {
	*pointer = _scriptModule->GetFunctionByDecl(cur->second.c_str());
	break ;
      }
    }
  }
}

void StatModel::LoadFunctions(void)
{
  _scriptUpdateAllValues = _scriptAvailableTraits = _scriptActivateTraits = _scriptAddExperience = _scriptAddSpecialPoint = _scriptIsReady = _scriptLevelUp = _scriptXpNextLevel = 0;
  if (_scriptContext)
  {
    _scriptModule = Script::Engine::LoadModule("special", "scripts/ai/special.as");
    if (_scriptModule)
    {
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAvailableTraits, "StringList AvailableTraits(Data)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptActivateTraits,  "bool ActivateTraits(Data, string, bool)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAddExperience,   "void AddExperience(Data, int)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAddSpecialPoint, "bool AddSpecialPoint(Data, string, int)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptXpNextLevel,     "int  XpNextLevel(Data)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptLevelUp,         "void LevelUp(Data)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptIsReady,         "bool IsReady(Data)"));
      _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptUpdateAllValues, "void UpdateAllValues(Data)"));
    }
  }  
}

unsigned short StatModel::GetLevel(void) const
{
  return (_statsheet["Variables"]["Level"]);
}

void           StatModel::LevelUp(void)
{
  _statsheet["Variables"]["Level"] = GetLevel() + 1;
  ReloadFunction(&_scriptLevelUp);
  if (_scriptLevelUp)
  {
    _scriptContext->Prepare(_scriptLevelUp);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
  }
  LevelUpped.Emit(GetLevel());
}

bool           StatModel::IsReady(void)
{
  bool         is_ready = true;

  ReloadFunction(&_scriptIsReady);
  if (_scriptIsReady)
  {
    _scriptContext->Prepare(_scriptIsReady);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
    is_ready = _scriptContext->GetReturnByte();
  }
  return (is_ready);
}

list<string>   StatModel::GetAvailableTraits(void)
{
  list<string> ret;

  ReloadFunction(&_scriptAvailableTraits);
  if (_scriptAvailableTraits)
  {
    _scriptContext->Prepare(_scriptAvailableTraits);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
    ret = *((list<string>*)_scriptContext->GetReturnObject());
  }
  return (ret);
}

void           StatModel::ToggleTrait(const string& trait)
{
  Data         dtrait    = _statsheet["Traits"][trait];
  bool         is_active = (!dtrait.Nil()) && dtrait == 1;

  ReloadFunction(&_scriptActivateTraits);
  if (_scriptActivateTraits)
  {
    string tmp = trait;
    
    //Script::Call(_scriptContext, _scriptActivateTraits, "OOb", &_statsheet, &tmp, !is_active);
    _scriptContext->Prepare(_scriptActivateTraits);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->SetArgObject(1, &tmp);
    _scriptContext->SetArgByte(2, !is_active);
    _scriptContext->Execute();
    if (_scriptContext->GetReturnByte())
      UpdateAllValues();
  }
}

void           StatModel::SetExperience(unsigned short e)
{
  unsigned short currentXp = _statsheet["Variables"]["Experience"];
  unsigned short nextLevel = 0;

  _statsheet["Variables"]["Experience"] = e;
  ReloadFunction(&_scriptAddExperience);
  if (_scriptAddExperience)
  {
    _scriptContext->Prepare(_scriptAddExperience);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->SetArgDWord(1, e - currentXp);
    _scriptContext->Execute();
  }
  else
    _statsheet["Variables"]["Experience"] = currentXp + e;
  ReloadFunction(&_scriptXpNextLevel);
  if (_scriptXpNextLevel)
  {
    _scriptContext->Prepare(_scriptXpNextLevel);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
    nextLevel = _scriptContext->GetReturnDWord();
  }
  if (nextLevel && e >= nextLevel)
    LevelUp();
}

void           StatModel::SetStatistic(const std::string& stat, short value)
{
  _statsheet["Statistics"][stat] = value;
}

void           StatModel::SetSpecial(const std::string& stat, short value)
{
  short  currentValue = _statsheet["Special"][stat];
  bool   sendSignal   = true;

  ReloadFunction(&_scriptAddSpecialPoint);
  if (_scriptAddSpecialPoint)
  {
    string stat_      = stat;

    _scriptContext->Prepare(_scriptAddSpecialPoint);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->SetArgObject(1, &stat_);
    _scriptContext->SetArgDWord(2, (value - currentValue));
    _scriptContext->Execute();
    sendSignal = _scriptContext->GetReturnByte();
  }
  else
    _statsheet["Special"][stat] = value;
  if (sendSignal)
  {
    SpecialChanged.Emit(stat, _statsheet["Special"][stat]);
    UpdateAllValues();
  }
}

bool          StatModel::UpdateAllValues(void)
{
  ReloadFunction(&_scriptUpdateAllValues);
  if (_scriptUpdateAllValues)
  {
    _scriptContext->Prepare(_scriptUpdateAllValues);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
    
    std::for_each(_statsheet["Special"].begin(), _statsheet["Special"].end(), [this](Data value)
    { SpecialChanged.Emit(value.Key(), value); });
    
    std::for_each(_statsheet["Statistics"].begin(), _statsheet["Statistics"].end(), [this](Data value)
    { StatisticChanged.Emit(value.Key(), value); });

    std::for_each(_statsheet["Skills"].begin(), _statsheet["Skills"].end(), [this](Data value)
    { SkillChanged.Emit(value.Key(), value); });
    
    return (true);
  }
  return (false);
}

unsigned short StatModel::GetXpNextLevel(void)
{
  ReloadFunction(&_scriptXpNextLevel);
  if (_scriptXpNextLevel)
  {
    _scriptContext->Prepare(_scriptXpNextLevel);
    _scriptContext->SetArgObject(0, &_statsheet);
    _scriptContext->Execute();
    return (_scriptContext->GetReturnDWord());
  }
  return (0);
}

void           StatModel::SetSkill(const std::string& stat, short value)
{
  Data           skill_points   = _statsheet["Variables"]["Skill Points"];
  short          v_skill_points = skill_points;
  Data           skill          = _statsheet["Skills"][stat];
  short          current_value  = skill;
  
  v_skill_points += (current_value - value);
  if (v_skill_points >= 0)
  {
    skill        = value;
    skill_points = v_skill_points;
    SkillChanged.Emit(stat, value);
  }
}

std::string    StatModel::GetStatistic(const std::string& stat) const
{
  return (_statsheet["Statistics"][stat]);
}

short          StatModel::GetSpecial(const std::string& stat)   const
{
  return (_statsheet["Special"][stat]);
}

short          StatModel::GetSkill(const std::string& stat)     const
{
  return (_statsheet["Skills"][stat]);
}

vector<string> StatModel::GetStatKeys(Data stats) const
{
  vector<string> keys;
  
  for_each(stats.begin(), stats.end(), [&keys](Data field) { keys.push_back(field.Key()); });
  return (keys);
}

/*
 * StatController
 */
StatController::StatController(Data statsheet) : _model(statsheet), _view(0)
{
  _model.SpecialChanged.Connect  (*this,   &StatController::SpecialChanged);
  _model.SkillChanged.Connect    (*this,   &StatController::SkillChanged);
  _model.StatisticChanged.Connect(*this,   &StatController::StatisticChanged);
  _model.LevelUpped.Connect      (*this,   &StatController::LevelChanged);
  _model.LevelUpped.Connect      (LevelUp, &Observatory::Signal<void (unsigned short)>::Emit);
}

void StatController::SpecialChanged(const string& stat, short value)
{
  if (_view)
  {
    _view->SetFieldValue("Special", stat, value);
    _view->SetIdValue("special-points", _model.GetSpecialPoints());
  }
}

void StatController::SkillChanged(const string& stat, short value)
{
  if (_view)
  {
    _view->SetFieldValue("Skills", stat, value);
    _view->SetIdValue("skill-points", _model.GetSkillPoints());
  }
}

void StatController::StatisticChanged(const string& stat, short value)
{
  if (_view)
    _view->SetFieldValue("Statistics", stat, value);
}

void StatController::TraitToggled(const string& trait)
{
  _model.ToggleTrait(trait);
}

void StatController::LevelChanged(unsigned short lvl)
{
  _view->SetIdValue("level", lvl);
  _view->SetIdValue("next-level", _model.GetXpNextLevel());
  if (_model.GetSkillPoints() > 0)
    _view->SetEditMode(StatView::Update);
}

void StatController::AddExperience(unsigned short exp)
{
  _model.SetExperience(_model.GetExperience() + exp);
  _view->SetIdValue("current-xp", exp);
}

void StatController::UpSpecial(const std::string& stat)
{
  SetSpecial(stat, _model.GetSpecial(stat) + 1);
}

void StatController::DownSpecial(const std::string& stat)
{
  SetSpecial(stat, _model.GetSpecial(stat) - 1);
}

void StatController::SetSpecial(const std::string& stat, short value)
{
  _model.SetSpecial(stat, value);
}

void StatController::SetStatistic(const std::string& stat, short value)
{
  _model.SetStatistic(stat, value);
}

void StatController::UpSkill(const std::string& stat)
{
  SetSkill(stat, _model.GetSkill(stat) + 1);
}

void StatController::DownSkill(const std::string& stat)
{
  SetSkill(stat, _model.GetSkill(stat) - 1);
}

void StatController::SetSkill(const std::string& stat, short value)
{
  _model.SetSkill(stat, value);
}

void StatController::AcceptChanges(void)
{
  StatView::EditMode editMode = _view->GetEditMode();

  if (!(_model.IsReady()))
  {
    cout << "[StatController] Thou art not ready" << endl;
    // TODO popup a dialog saying that you're not ready
    return ;
  }
  cout << "[StatController] Thou art ready" << endl;
  if (_model.GetSpecialPoints() > 0)
    _view->SetEditMode(StatView::Create);
  else if (_model.GetSkillPoints() > 0)
    _view->SetEditMode(StatView::Update);
  else
    _view->SetEditMode(StatView::Display);
  _view->Hide();
}

void StatController::CancelChanges(void)
{
  _view->Hide();
}

void StatController::SetView(StatView* view)
{
  vector<string> specials, skills, statistics;

  if (_view)
    _viewObservers.DisconnectAll();
  _view      = view;
  specials   = _model.GetSpecials();
  skills     = _model.GetSkills();
  statistics = _model.GetStatistics();
  
  _view->SetCategoryFields("Special",    specials);
  _view->SetCategoryFields("Skills",     skills);
  _view->SetCategoryFields("Statistics", statistics);

  for_each(specials.begin(), specials.end(), [this](const string& key)
  { _view->SetFieldValue("Special", key, _model.GetSpecial(key)); });

  for_each(skills.begin(), skills.end(), [this](const string& key)
  { _view->SetFieldValue("Skills", key, _model.GetSkill(key)); });

  for_each(statistics.begin(), statistics.end(), [this](const string& key)
  { _view->SetFieldValue("Statistics", key, _model.GetStatistic(key)); });

  _view->SetInformation("Name",   _model.GetName());
  _view->SetInformation("Age",    _model.GetAge());
  _view->SetInformation("Race",   _model.GetRace());
  _view->SetInformation("Gender", _model.GetGender());
  
  _view->SetIdValue("special-points", _model.GetSpecialPoints());
  _view->SetIdValue("skill-points",   _model.GetSkillPoints());
  
  _view->SetExperience(_model.GetExperience(), _model.GetLevel(), _model.GetXpNextLevel());

  _viewObservers.Connect(_view->StatDowned,         *this, &StatController::ViewStatDowned);
  _viewObservers.Connect(_view->StatUpped,          *this, &StatController::ViewStatUpped);
  _viewObservers.Connect(_view->InformationChanged, *this, &StatController::InformationChanged);
  _viewObservers.Connect(_view->AgeChanged,         *this, &StatController::AgeChanged);
  _viewObservers.Connect(_view->TraitToggled,       *this, &StatController::TraitToggled);
  _viewObservers.Connect(_view->Accepted,           *this, &StatController::AcceptChanges);
  _viewObservers.Connect(_view->Canceled,           *this, &StatController::CancelChanges);

  _view->SetTraits(_model.GetAvailableTraits());
  
  if (_model.GetSpecialPoints() > 0)
    _view->SetEditMode(StatView::Create);
  else if (_model.GetSkillPoints() > 0)
    _view->SetEditMode(StatView::Update);
  else
    _view->SetEditMode(StatView::Display);

  /*list<string> perks = _model.GetAvailablePerks();
  
  for_each(perks.begin(), perks.end(), [](string perk)
  {
    cout << "Available Perk: " << perk << endl;
  });*/
}

void StatController::InformationChanged(const string& info, const string& value)
{
  if (info == "name")
  {
    _model.SetName(value);
    if (_view)
      _view->SetInformation(info, value);
  }
  else if (info == "gender")
  {
    _model.SetGender(value);
    if (_view)
      _view->SetInformation(info, _model.GetGender());
  }
}

void StatController::AgeChanged(unsigned char age)
{
  _model.SetAge(age);
  if (_view)
    _view->SetInformation("age", (unsigned short)age);
}

void StatController::ViewStatUpped(const string& type, const string& stat)
{
  if      (type == "Special") UpSpecial(stat);
  else if (type == "Skills")  UpSkill(stat);
}

void StatController::ViewStatDowned(const string& type, const string& stat)
{
  if      (type == "Special") DownSpecial(stat);
  else if (type == "Skills")  DownSkill(stat);
}

/*
 * StatViewRocket
 */
using namespace Rocket;

static list<string> split(const std::string& str, char c = ' ')
{
  list<string> ret;
  short        last_sep = -1;
  short        i;

  for (i = 0 ; str[i] ; ++i)
  {
    if (str[i] == c)
    {
      if (i != 0 && str[i - 1] != c)
        ret.push_back(str.substr(last_sep + 1, i - (last_sep + 1)));
      last_sep = i;
    }
  }
  if (last_sep != i && i > 0 && str[i - 1] != c)
    ret.push_back(str.substr(last_sep + 1));
  return (ret);
}

Data DataGetFromPathRec(Data data, list<string> array)
{
  if (array.size() != 0)
  {
    string key = (*array.begin());
    
    array.erase(array.begin());
    return (DataGetFromPathRec(data[key], array));
  }
  return (data);
}

Data DataGetFromPath(Data data, const std::string& path)
{
  list<string> array = split(path, '/');
  
  return (DataGetFromPathRec(data, array));
}

list<string> StatModel::GetAvailablePerks(void)
{
  list<string> perks;
  DataTree*    file = DataTree::Factory::JSON("data/perks.json");

  if (file)
  {
    { // dataPerks needs to get out of the heap before file is deleted
      Data dataPerks(file);
      
      for_each(dataPerks.begin(), dataPerks.end(), [this, &perks](Data perk)
      {
	Data           requirements = perk["Requirements"];
	bool           do_add       = true;
	Data::iterator it           = requirements.begin();
	Data::iterator end          = requirements.end();
	
	for (; it != end ; ++it)
	{
	  Data         requirement = *it;
	  string       comp        = requirement["Comp"];
	  short        value       = requirement["Value"];
	  Data         data_check  = DataGetFromPath(_statsheet, requirement.Key());
	  short        to_check    = data_check;

	  if      (comp == "==") do_add = data_check.Value() == requirement["Value"].Value();
	  else if (comp == ">=") do_add = to_check >= value;
	  else if (comp == "<=") do_add = to_check <= value;
	  else if (comp == ">")  do_add = to_check >  value;
	  else if (comp == "<")  do_add = to_check <  value;
	  if (!do_add)
	    break ;
	}
	if (do_add)
	  perks.push_back(perk.Key());
      });
    }
    delete file;
  }
  return (perks);
}

static string humanize(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if (i == 0 || str[i - 1] == '_')
      ret += str[i] - 'a' + 'A';
    else if (str[i] == '_')
      ret += ' ';
    else
      ret += str[i];
  }
  return (ret);
}

static string underscore(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if      (str[i] >= 'A' && str[i] <= 'Z')
      ret += str[i] - 'A' + 'a';
    else if (str[i] == ' ')
      ret += '_';
    else
      ret += str[i];
  }
  return (ret);
}

StatViewRocket::StatViewRocket(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  _root     = context->LoadDocument("data/charsheet.rml");

  if (_root)
  {
    Rocket::Core::Element* button_continue = _root->GetElementById("continue");
    Rocket::Core::Element* button_cancel   = _root->GetElementById("cancel");

    if (button_continue) button_continue->AddEventListener("click", &DoneButton);
    if (button_cancel)   button_cancel->AddEventListener  ("click", &CancelButton);

    DoneButton.EventReceived.Connect  (*this, &StatViewRocket::Accept);
    CancelButton.EventReceived.Connect(*this, &StatViewRocket::Cancel);

    // Edit Mode
    EventSpecialClicked.EventReceived.Connect(*this, &StatViewRocket::SpecialClicked);
    EventSkillClicked.EventReceived.Connect  (*this, &StatViewRocket::SkillClicked);
    EventGeneralClicked.EventReceived.Connect(*this, &StatViewRocket::GeneralClicked);

    EventAgeChanged.EventReceived.Connect   (*this, &StatViewRocket::UpdateAge);
    EventNameChanged.EventReceived.Connect  (*this, &StatViewRocket::UpdateName);
    EventGenderChanged.EventReceived.Connect(*this, &StatViewRocket::UpdateGender);
    
    EventTraitClicked.EventReceived.Connect (*this, &StatViewRocket::TraitClicked);
    
    Core::Element* age_edit_ok    = _root->GetElementById("char-age-edit-ok");
    Core::Element* name_edit_ok   = _root->GetElementById("char-name-edit-ok");
    Core::Element* gender_edit_ok = _root->GetElementById("char-gender-edit-ok");
    
    if (age_edit_ok)    age_edit_ok->AddEventListener   ("click", &EventAgeChanged);
    if (name_edit_ok)   name_edit_ok->AddEventListener  ("click", &EventNameChanged);
    if (gender_edit_ok) gender_edit_ok->AddEventListener("click", &EventGenderChanged);

    Core::Element* cursor_plus = _root->GetElementById("edit-value-cursor-plus");
    Core::Element* cursor_less = _root->GetElementById("edit-value-cursor-less");

    if (cursor_plus && cursor_less)
    {
      cursor_plus->AddEventListener("click", &ButtonUp);
      cursor_less->AddEventListener("click", &ButtonDown);
      ButtonUp.EventReceived.Connect  (*this, &StatViewRocket::StatMore);
      ButtonDown.EventReceived.Connect(*this, &StatViewRocket::StatLess);
    }
    
    SetEditMode(Display);
  }
}

void StatViewRocket::UpdateAge(Core::Event& event)
{
  Core::Element* age_field = _root->GetElementById("char-age-value");
  
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
  Core::Element* name_field = _root->GetElementById("char-name-value");
  
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
  Core::Element* gender_male   = _root->GetElementById("char-gender-option-male");
  Core::Element* gender_female = _root->GetElementById("char-gender-option-female");
  
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
  event.GetCurrentElement()->GetParentNode()->SetProperty("display", "none");
}

void        StatViewRocket::StatUpdate(Core::Event& event, string& ret_type, string& ret_stat)
{
  Core::Element* cursor    = _root->GetElementById("edit-value-cursor");

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
  Core::Element* cursor = _root->GetElementById("edit-value-cursor");

  if (cursor)
  {
    Core::Element* current = _context->GetHoverElement();
    
    cursor->SetProperty("display", "block");
    current = current->GetParentNode();
    if (cursor->GetParentNode() != current && current->GetClassNames() == "special-group")
    {
      cursor->GetParentNode()->RemoveChild(cursor);
      current->AppendChild(cursor);
    }
  }
}

void StatViewRocket::SkillClicked(Core::Event& event)
{
  Core::Element* cursor = _root->GetElementById("edit-value-cursor");
  
  if (cursor)
  {
    Core::Element* current = _context->GetHoverElement();
    
    while (current && current->GetClassNames() != "skill-datagrid")
      current = current->GetParentNode();
    if (current)
    {
      cursor->SetProperty("display", "block");
      cursor->GetParentNode()->RemoveChild(cursor);
      current->AppendChild(cursor);
    }
  }
}

void StatViewRocket::GeneralClicked(Rocket::Core::Event& event)
{
  Core::String   id     = event.GetCurrentElement()->GetId();
  Core::Element* name   = _root->GetElementById("char-name-edit");
  Core::Element* age    = _root->GetElementById("char-age-edit");
  Core::Element* gender = _root->GetElementById("char-gender-edit");

  name->SetProperty  ("display", "none");
  age->SetProperty   ("display", "none");
  gender->SetProperty("display", "none");
  if      (id == "char-name")   name->SetProperty  ("display", "block");
  else if (id == "char-age")    age->SetProperty   ("display", "block");
  else if (id == "char-gender") gender->SetProperty("display", "block");
}

void StatViewRocket::SetEditMode(EditMode mode)
{
  const char*    createElems[]  = { "continue", "special-points-title", 0 };
  const char*    updateElems[]  = { "continue", "cancel", "experience", "skill-points-title", 0 };
  const char*    displayElems[] = { "cancel",   "experience", 0 };
  const char**   toShow;
  const char**   elemArrays[]   = { createElems, updateElems, displayElems, 0 };
  Core::Element* cursor  = _root->GetElementById("edit-value-cursor");
  Core::Element* special = _root->GetElementById("special");
  Core::Element* skill   = _root->GetElementById("body");
  
  Core::Element* name   = _root->GetElementById("char-name");
  Core::Element* age    = _root->GetElementById("char-age");
  Core::Element* gender = _root->GetElementById("char-gender");

  if (cursor)  cursor->SetProperty("display", "none");
  if (special) special->RemoveEventListener("click", &EventSpecialClicked);
  if (skill)   skill->RemoveEventListener  ("click", &EventSkillClicked);
  
  if (name)    name->RemoveEventListener  ("click", &EventGeneralClicked);
  if (age)     age->RemoveEventListener   ("click", &EventGeneralClicked);
  if (gender)  gender->RemoveEventListener("click", &EventGeneralClicked);

  switch (mode)
  {
    case Create:
      if (special) special->AddEventListener("click", &EventSpecialClicked);
      if (name)    name->AddEventListener   ("click", &EventGeneralClicked);
      if (age)     age->AddEventListener    ("click", &EventGeneralClicked);
      if (gender)  gender->AddEventListener ("click", &EventGeneralClicked);
      toShow = createElems;
      break ;
    case Update:
      if (skill) skill->AddEventListener  ("click", &EventSkillClicked);
      toShow = updateElems;
      break ;
    case Display:
      toShow = displayElems;
      break ;
  }
  for (unsigned int ii = 0 ; elemArrays[ii] != 0 ; ++ii)
  {
    const char** elemArray = elemArrays[ii];

    if (elemArray == toShow) continue ;
    for (unsigned int i = 0 ; elemArray[i] != 0 ; ++i)
    {
      Core::Element* elem = _root->GetElementById(elemArray[i]);

      if (elem) elem->SetProperty("display", "none");
    }
  }
  for (unsigned int i = 0 ; toShow[i] != 0 ; ++i)
  {
    Core::Element* elem = _root->GetElementById(toShow[i]);

    if (elem) elem->SetProperty("display", "block");
  }

  _editMode = mode;
}

void StatViewRocket::SetInformation(const std::string& name, const std::string& value)
{
  Core::Element* element;
  string         id = "char-" + underscore(name);
  
  element = _root->GetElementById(id.c_str());
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
  if ((element = _root->GetElementById(strId.c_str())))
    element->SetInnerRML(value.c_str());
  else
    cout << "[Warning] Element '" << strId << "' should exist but doesn't" << endl;
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
    Core::Element* elem = _root->GetElementById(id.c_str());
    
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
  Core::Element* element = _root->GetElementById(id.c_str());
  
  if (element)
  {
    stringstream rml;
    
    rml << value;
    element->SetInnerRML(rml.str().c_str());
  }
}

void StatViewRocket::SetIdValue(const std::string& id, const std::string& value)
{
  Core::Element* element = _root->GetElementById(id.c_str());
  
  if (element)
    element->SetInnerRML(value.c_str());
}

void StatViewRocket::SetExperience(unsigned short xp, unsigned short lvl, unsigned short next_level)
{
  if (_root)
  {
    Core::Element* element = _root->GetElementById("experience");
    
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

void StatViewRocket::SetTraits(list<string> traits)
{
  if (_root)
  {
    Core::Element* element = _root->GetElementById("panel-traits");
    
    if (element)
    {
      stringstream rml;
      
      for_each(traits.begin(), traits.end(), [&rml](const string trait)
      {
	rml << "<button id='" << underscore(trait) << "'>O</button>";
	rml << "<span class='text-trait' id='text-" << underscore(trait) << "'>" << trait << "</span><br />";
      });
      element->SetInnerRML(rml.str().c_str());
      
      for_each(traits.begin(), traits.end(), [this](const string trait)
      {
	Core::Element* traitButton = _root->GetElementById(underscore(trait).c_str());

	if (traitButton)
	  traitButton->AddEventListener("click", &EventTraitClicked);
      });
    }
  }
}

void StatViewRocket::SetCategoryFields(const std::string& category, const std::vector<std::string>& keys)
{
  if (_root)
  {
    Core::Element* element = _root->GetElementById(underscore(category).c_str());
    stringstream   rml;

    if (element)
    {
      unsigned short topX = 0;

      for (unsigned int i = 0 ; i < keys.size() ; ++i)
      {
	string underscored = underscore(keys[i]);

	if (category == "Special")
	{
	  rml << "<p class='special-group' data-type='Special' data-key='" << keys[i] << "' style='top: " << topX << "px;'>\n";
          rml << "  <p class='special-key'>" << keys[i] << "</p>\n";
	  rml << "  <p class='special-value' id='special-value-" << underscored << "'>0</p>\n";
	  rml << "  <p class='special-commt' id='special-commt-" << underscored << "'>Great</p>\n";
          rml << "</p>\n\n";
	  topX += 40;
	}
	else if (category == "Statistics")
	{
	  rml << "<datagrid>\n";
          rml << "  <col width='80%'><span class'statistics-key'>" << keys[i] << "</span></col>\n";
          rml << "  <col width='20%'><span class'statistics-value' id='statistics-value-" << underscored << "'>0</span></col>\n";
          rml << "</datagrid>\n\n";
	}
	else if (category == "Skills")
	{
	  rml << "<datagrid class='skill-datagrid' data-type='Skills' data-key='" << keys[i] << "'>\n";
          rml << "  <col width='80%'><span class='skill-key'>" << keys[i] << "</span></col>\n";
          rml << "  <col width='20%'><span class='skill-value' id='skills-value-" << underscored << "'>0</span>%</col>\n";
          rml << "</datagrid>\n\n";
	}
      }
      element->SetInnerRML(rml.str().c_str());
    }
  }
}