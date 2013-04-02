#include "statsheet.hpp"
#include <i18n.hpp>
#include <gameui.hpp>
#include <gametask.hpp>

using namespace std;

/*
 * StatModel
 */
StatModel::StatModel(Data statsheet) : _statsheet(statsheet)
{
  _script_context = 0;
  LoadFunctions();
}

StatModel::~StatModel(void)
{
  if (_script_context) _script_context->Release();
}

void StatModel::Backup(void)
{
  _statsheet_backup.Remove();
  _statsheet_backup = Data();
  _statsheet_backup.Duplicate(_statsheet);
}

static void RestoreData(Data restoring, Data to_restore)
{
  unsigned int it = 0;

  while (it < restoring.Count())
  {
    bool need_delete = false;
    {
      Data cur1 = restoring[it];
      Data cur2 = to_restore[cur1.Key()];

      if (cur2.Nil())
	need_delete = true;
      else
      {
	if (cur1.Count() > 0)
	  RestoreData(cur1, cur2);
	else
	  cur1 = cur2.Value();
      }
    }
    if (need_delete)
      restoring[it].Remove();
    else
      ++it;
  }
}

void StatModel::RestoreBackup(void)
{
  cout << "Restoring backup" << endl;
  RestoreData(_statsheet, _statsheet_backup);
  cout << "Done" << endl;
  UpdateAllValues();
}

Scriptable::Scriptable(void) : _script_context(0), _script_module(0)
{
}

Scriptable::~Scriptable(void)
{
  if (_script_module)
    Script::ModuleManager::Release(_script_module);
}

void Scriptable::LoadScript(string module_name, string filepath)
{
  asIScriptEngine* engine = Script::Engine::Get();

  if (_script_module)
    Script::ModuleManager::Release(_script_module);
  _script_context = (engine          ? engine->CreateContext() : 0);
  _script_module  = (_script_context ? Script::ModuleManager::Require(module_name, filepath) : 0);
  std::for_each(_script_func_ptrs.begin(), _script_func_ptrs.end(), [](ScriptFuncPtr& func_ptr)
  { *func_ptr.first = 0; });
}

void Scriptable::ReloadFunction(asIScriptFunction** pointer)
{
  *pointer = 0;
  if (_script_context && _script_module)
  {
    ScriptFuncPtrs::iterator cur, end;

    for (cur = _script_func_ptrs.begin(), end = _script_func_ptrs.end() ; cur != end ; ++cur)
    {
      if (cur->first == pointer)
      {
	*pointer = _script_module->GetFunctionByDecl(cur->second.c_str());
	break ;
      }
    }
  }
}

void StatModel::LoadFunctions(void)
{
  _selectRandomEncounter = _scriptUpdateAllValues = _scriptAvailableTraits = _scriptActivateTraits = _scriptAddExperience = _scriptAddSpecialPoint = _scriptIsReady = _scriptLevelUp = _scriptXpNextLevel = _scriptAddPerk = 0;
  _script_func_ptrs.clear();
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAvailableTraits, "StringList AvailableTraits(Data)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptActivateTraits,  "bool ActivateTraits(Data, string, bool)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAddExperience,   "void AddExperience(Data, int)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAddSpecialPoint, "bool AddSpecialPoint(Data, string, int)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptXpNextLevel,     "int  XpNextLevel(Data)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptLevelUp,         "void LevelUp(Data)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptIsReady,         "bool IsReady(Data)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptUpdateAllValues, "void UpdateAllValues(Data)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_scriptAddPerk,         "bool AddPerk(Data, string)"));
  _script_func_ptrs.push_back(ScriptFuncPtr(&_selectRandomEncounter, "string SelectRandomEncounter(Data)"));
  LoadScript("special", "scripts/ai/special.as");
}

void           StatModel::AddKill(const string& race)
{
  if (_statsheet["Kills"][race].Nil())
    _statsheet["Kills"][race] = 1;
  else
    _statsheet["Kills"][race] = (unsigned int)_statsheet["Kills"][race] + 1;
}

bool           StatModel::AddPerk(const string& perk)
{
  bool         success = true;
  
  ReloadFunction(&_scriptAddPerk);
  if (_scriptAddPerk)
  {
    string tmp = perk;

    _script_context->Prepare(_scriptAddPerk);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->SetArgObject(1, &tmp);
    _script_context->Execute();
    success = _script_context->GetReturnByte();
  }
  else
  {
    int points_left = _statsheet["Variables"]["Perks"];
    
    if (points_left > 0)
    {
      _statsheet["Perks"][perk]        = 1;
      _statsheet["Variables"]["Perks"] = points_left - 1;
    }
    else
      success = false;
  }
  return (success);
}

void           StatModel::SetCurrentHp(short hp)
{
  _statsheet["Variables"]["Hit Points"] = hp;
}

unsigned short StatModel::GetLevel(void) const
{
  return (_statsheet["Variables"]["Level"]);
}

void           StatModel::LevelUp(void)
{
  ReloadFunction(&_scriptLevelUp);
  if (_scriptLevelUp)
  {
    _script_context->Prepare(_scriptLevelUp);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
  }
  else
    _statsheet["Variables"]["Level"] = GetLevel() + 1;
  LevelUpped.Emit(GetLevel());
}

bool           StatModel::IsReady(void)
{
  bool         is_ready = true;

  ReloadFunction(&_scriptIsReady);
  if (_scriptIsReady)
  {
    _script_context->Prepare(_scriptIsReady);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
    is_ready = _script_context->GetReturnByte();
  }
  return (is_ready);
}

list<string>   StatModel::GetAvailableTraits(void)
{
  list<string> ret;

  ReloadFunction(&_scriptAvailableTraits);
  if (_scriptAvailableTraits)
  {
    _script_context->Prepare(_scriptAvailableTraits);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
    ret = *((list<string>*)_script_context->GetReturnObject());
  }
  return (ret);
}

void           StatModel::ToggleSkillAffinity(const string& skill)
{
  int          left      = _statsheet["Variables"]["Affinities"];
  Data         affinity  = _statsheet["Affinities"][skill];
  bool         is_active = (!affinity.Nil()) && affinity == 1;

  if (!is_active && left <= 0)
    return ;
  affinity = (is_active ? 0 : 1);
  _statsheet["Variables"]["Affinities"] = left + (is_active ? 1 : -1);
}

void           StatModel::ToggleTrait(const string& trait)
{
  Data         dtrait    = _statsheet["Traits"][trait];
  bool         is_active = (!dtrait.Nil()) && dtrait == 1;

  ReloadFunction(&_scriptActivateTraits);
  if (_scriptActivateTraits)
  {
    string tmp = trait;
    
    _script_context->Prepare(_scriptActivateTraits);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->SetArgObject(1, &tmp);
    _script_context->SetArgByte(2, !is_active);
    _script_context->Execute();
    if (_script_context->GetReturnByte())
      UpdateAllValues();
  }
}

void           StatModel::SetExperience(unsigned short e)
{
  unsigned int   currentXp = _statsheet["Variables"]["Experience"];
  unsigned short nextLevel = 0;

  _statsheet["Variables"]["Experience"] = e;
  ReloadFunction(&_scriptAddExperience);
  if (_scriptAddExperience)
  {
    _script_context->Prepare(_scriptAddExperience);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->SetArgDWord(1, e - currentXp);
    _script_context->Execute();
  }
  else
    _statsheet["Variables"]["Experience"] = currentXp + e;
  ReloadFunction(&_scriptXpNextLevel);
  if (_scriptXpNextLevel)
  {
    _script_context->Prepare(_scriptXpNextLevel);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
    nextLevel = _script_context->GetReturnDWord();
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

    _script_context->Prepare(_scriptAddSpecialPoint);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->SetArgObject(1, &stat_);
    _script_context->SetArgDWord(2, (value - currentValue));
    _script_context->Execute();
    sendSignal = _script_context->GetReturnByte();
  }
  else
    _statsheet["Special"][stat] = value;
  if (sendSignal)
  {
    SpecialChanged.Emit(stat, _statsheet["Special"][stat]);
    UpdateAllValues();
  }
}

list<string>  StatModel::GetTraits(void) const
{
  Data         data_traits = _statsheet["Traits"];
  list<string> traits;

  for_each(data_traits.begin(), data_traits.end(), [&traits](Data trait)
  {
    if (trait == 1)
      traits.push_back(trait.Key());
  });
  return (traits);
}

list<string>  StatModel::GetPerks(void) const
{
  Data         data_perks = _statsheet["Perks"];
  list<string> perks;

  for_each(data_perks.begin(), data_perks.end(), [&perks](Data perk)
  {
    perks.push_back(perk.Key());
  });
  return (perks);
}

bool          StatModel::UpdateAllValues(void)
{
  ReloadFunction(&_scriptUpdateAllValues);
  if (_scriptUpdateAllValues)
  {
    _script_context->Prepare(_scriptUpdateAllValues);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
    
    std::for_each(_statsheet["Special"].begin(), _statsheet["Special"].end(), [this](Data value)
    { SpecialChanged.Emit(value.Key(), value); });
    
    std::for_each(_statsheet["Statistics"].begin(), _statsheet["Statistics"].end(), [this](Data value)
    { StatisticChanged.Emit(value.Key(), value); });

    std::for_each(_statsheet["Skills"].begin(), _statsheet["Skills"].end(), [this](Data value)
    { SkillChanged.Emit(value.Key(), value); });
    
    MaxHpChanged.Emit(_statsheet["Statistics"]["Hit Points"]);
    PerksChanged.Emit();

    return (true);
  }
  return (false);
}

unsigned short StatModel::GetXpNextLevel(void)
{
  ReloadFunction(&_scriptXpNextLevel);
  if (_scriptXpNextLevel)
  {
    _script_context->Prepare(_scriptXpNextLevel);
    _script_context->SetArgObject(0, &_statsheet);
    _script_context->Execute();
    return (_script_context->GetReturnDWord());
  }
  return (0);
}

int           StatModel::Action(const std::string& action, const std::string& fmt, ...)
{
  string             func_name   = "action_" + action;
  asIScriptFunction* action_func = _script_module->GetFunctionByName(func_name.c_str());
  
  if (action_func)
  {
    va_list      ap;

    _script_context->Prepare(action_func);
    va_start(ap, fmt);
    for (unsigned short it = 0 ; it < fmt.size() ; ++it)
    {
      switch (fmt[it])
      {
        case 'o':
          _script_context->SetArgObject(it, va_arg(ap, void*));
          break ;
        case 'i':
          _script_context->SetArgDWord(it, va_arg(ap, int));
          break ;
        case 'b':
          _script_context->SetArgByte(it, va_arg(ap, int));
          break ;
        case 'f':
          _script_context->SetArgFloat(it, va_arg(ap, double));
          break ;
        case 'd':
          _script_context->SetArgDouble(it, va_arg(ap, double));
          break ;
        default:
          cout << "[StatModel][Action] Call will fail: unsuported argument '" << fmt[it] << "' provided" << endl;
          break ;
      }
    }
    va_end(ap);
    _script_context->Execute();
    return (_script_context->GetReturnWord());
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
  if (!(_statsheet_backup.Nil()))
  {
    if ((short)_statsheet_backup["Skills"][stat] > value)
      return ;
  }
  if (v_skill_points >= 0)
  {
    skill        = value;
    skill_points = v_skill_points;
    SkillChanged.Emit(stat, value);
  }
}

std::string    StatModel::GetStatistic(const std::string& stat) const
{
  return (_statsheet["Statistics"][stat].Value());
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

string StatModel::SelectRandomEncounter(void)
{
  ReloadFunction(&_selectRandomEncounter);
  if (_selectRandomEncounter)
  {
    _script_context->Prepare(_selectRandomEncounter);
    _script_context->SetArgObject(1, &_statsheet);
    _script_context->Execute();
  }
  else
    AlertUi::NewAlert.Emit("Missing special.as function `string SelectRandomEncounter(Data)`");
  return ("");
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
  _model.LevelUpped.Connect      (LevelUp, &Sync::Signal<void (unsigned short)>::Emit);
  _model.MaxHpChanged.Connect    (*this,   &StatController::SetMaxHp);
  _model.PerksChanged.Connect    (*this,   &StatController::PerksChanged);
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

void StatController::PerksChanged(void)
{
  if (_view)
  {
    _view->SetNumPerks(_model.GetPerksPoints());
    _view->SetPerks(_model.GetPerks());
    _view->SetAvailablePerks(_model.GetAvailablePerks());
  }
}

void StatController::SkillAffinityToggled(const string& skill)
{
  _model.ToggleSkillAffinity(skill);
  if (&_view)
    _view->SetSkillAffinity(skill, _model.HasSkillAffinity(skill));
}

void StatController::TraitToggled(const string& trait)
{
  _model.ToggleTrait(trait);
  if (_view)
  {
    list<string>           traits;
    list<string>::iterator it;

    traits = _model.GetTraits();
    it     = find(traits.begin(), traits.end(), trait);
    _view->SetTraitActive(trait, it != traits.end());
  }
}

void StatController::LevelChanged(unsigned short lvl)
{
  if (_view)
  {
    unsigned short skill_points;

    GameTask::CurrentGameTask->PlaySound("pipbuck/levelup");
    _view->SetIdValue("level", lvl);
    _view->SetIdValue("next-level", _model.GetXpNextLevel());
    if (((skill_points = _model.GetSkillPoints()) > 0) || (_model.GetPerksPoints() > 0))
    {
      _view->SetEditMode(StatView::Update);
      _view->SetIdValue("skill-points", skill_points);
      _view->SetNumPerks(_model.GetPerksPoints());
      _view->SetPerks(_model.GetAvailablePerks());
    }
  }
}

void StatController::RunMetabolism(void)
{
  stringstream stream, stream_hp;
  int          hp, max_hp;

  stream << _model.GetStatistic("Healing Rate");
  stream >> hp;
  cout << "Healing Rate => " << hp << endl;
  stream_hp << _model.GetStatistic("Hit Points");
  stream_hp >> max_hp;
  hp = (int)(_model.GetAll()["Variables"]["Hit Points"]) + hp;
  hp = hp > max_hp ? max_hp : hp;
  SetCurrentHp(hp);  
}

void StatController::AddKill(const string& race)
{
  _model.AddKill(race);
  if (_view)
    _view->SetFieldValue("Kills", race, _model.GetKills(race));
}

void StatController::AddExperience(unsigned int exp)
{
  exp += _model.GetExperience();
  _model.SetExperience(exp);
  if (_view)
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
  bool affinity = _model.HasSkillAffinity(stat);

  SetSkill(stat, _model.GetSkill(stat) + (affinity ? 2 : 1));
}

void StatController::DownSkill(const std::string& stat)
{
  bool affinity = _model.HasSkillAffinity(stat);

  SetSkill(stat, _model.GetSkill(stat) - (affinity ? 2 : 1));
}

void StatController::SetSkill(const std::string& stat, short value)
{
  _model.SetSkill(stat, value);
}

void StatController::AcceptChanges(void)
{
  if (!(_model.IsReady()))
  {
    cout << "[StatController] Thou art not ready" << endl;
    // TODO popup a dialog saying that you're not ready
    return ;
  }
  cout << "[StatController] Thou art ready" << endl;
  if (_model.GetSpecialPoints() > 0)
    _view->SetEditMode(StatView::Create);
  else if (_model.GetSkillPoints() > 0 || _model.GetPerksPoints() > 0)
    _view->SetEditMode(StatView::Update);
  else
    _view->SetEditMode(StatView::Display);
  _view->Hide();
  ChangesAccepted.Emit();
}

void StatController::CancelChanges(void)
{
  _view->Hide();
  if (_view->GetEditMode() == StatView::Update)
    _model.RestoreBackup();
  ChangesCanceled.Emit();
}

void StatController::MakeBackup(void)
{
  _model.Backup();
}

void StatController::SetView(StatView* view)
{
  list<string>   traits;
  vector<string> specials, skills, statistics;

  if (_view)
    _viewObservers.DisconnectAll();
  _view      = view;
  traits     = _model.GetTraits();
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
  
  for_each(traits.begin(), traits.end(), [this](const string& key)
  { _view->SetTraitActive(key, true); });

  for_each(_model.GetAll()["Kills"].begin(), _model.GetAll()["Kills"].end(), [this](Data data)
  { _view->SetFieldValue("Kills", data.Key(), data.Value()); });

  _view->SetInformation("Name",   _model.GetName());
  _view->SetInformation("Age",    _model.GetAge());
  _view->SetInformation("Race",   _model.GetRace());
  _view->SetInformation("Gender", _model.GetGender());
  
  _view->SetIdValue("special-points", _model.GetSpecialPoints());
  _view->SetIdValue("skill-points",   _model.GetSkillPoints());

  SetCurrentHp(_model.GetCurrentHp());
  SetMaxHp(_model.GetMaxHp());
  _view->SetExperience(_model.GetExperience(), _model.GetLevel(), _model.GetXpNextLevel());

  _viewObservers.Connect(_view->StatDowned,          *this, &StatController::ViewStatDowned);
  _viewObservers.Connect(_view->StatUpped,           *this, &StatController::ViewStatUpped);
  _viewObservers.Connect(_view->InformationChanged,  *this, &StatController::InformationChanged);
  _viewObservers.Connect(_view->AgeChanged,          *this, &StatController::AgeChanged);
  _viewObservers.Connect(_view->ToggleSkillAffinity, *this, &StatController::SkillAffinityToggled);
  _viewObservers.Connect(_view->TraitToggled,        *this, &StatController::TraitToggled);
  _viewObservers.Connect(_view->PerkToggled,         *this, &StatController::PerkAdded);
  _viewObservers.Connect(_view->Accepted,            *this, &StatController::AcceptChanges);
  _viewObservers.Connect(_view->Canceled,            *this, &StatController::CancelChanges);
  _viewObservers.Connect(_view->MakeBackup,          *this, &StatController::MakeBackup);

  _view->SetTraits(_model.GetAvailableTraits());
  
  Data affinities = _model.GetSkillAffinities();
  for_each(affinities.begin(), affinities.end(), [this](Data affinity)
  { if (affinity == 1) _view->SetSkillAffinity(affinity.Key(), true); });

  if (_model.GetSpecialPoints() > 0)
    _view->SetEditMode(StatView::Create);
  else if (_model.GetSkillPoints() > 0)
    _view->SetEditMode(StatView::Update);
  else
    _view->SetEditMode(StatView::Display);

  _view->SetNumPerks      (_model.GetPerksPoints());
  _view->SetPerks         (_model.GetPerks());
  _view->SetAvailablePerks(_model.GetAvailablePerks());
}

void StatController::SetCurrentHp(short hp)
{
  cout << "StatController::SetCurrentHp (" << hp << ")" << endl;
  _model.SetCurrentHp(hp);
  if (_view)
    _view->SetInformation("char-state-hp-id-value", hp);
  HpChanged.Emit(hp);
}

void StatController::SetMaxHp(short hp)
{
  if (_view)
  {
    _view->SetInformation("char-state-hp-id-total", hp);
    if (_view->GetEditMode() == StatView::Create)
      SetCurrentHp(hp);
  }
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

void StatController::PerkAdded(const string& str)
{
  if (_model.AddPerk(str) && _view)
  {
    _view->SetNumPerks(_model.GetPerksPoints());
    _view->SetPerks(_model.GetPerks());
    _view->SetAvailablePerks(_model.GetAvailablePerks());
  }
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
  if (!(array.empty()))
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
	Data::my_iterator it           = requirements.begin();
	Data::my_iterator end          = requirements.end();
	
	for (; it != end ; ++it)
	{
	  Data         requirement = *it;
	  string       comp        = requirement["Comp"].Value();
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
  _root     = context->LoadDocument("data/charsheet.rml");

  if (_root)
  {
    _i18n = i18n::GetStatistics();
    
    ToggleEventListener(true, "continue", "click", DoneButton);
    ToggleEventListener(true, "cancel",   "click", CancelButton);
    DoneButton.EventReceived.Connect  (*this, &StatViewRocket::Accept);
    CancelButton.EventReceived.Connect(*this, &StatViewRocket::Cancel);

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
    
    ToggleEventListener(true, "char-age-edit-ok",       "click", EventAgeChanged);
    ToggleEventListener(true, "char-name-edit-ok",      "click", EventNameChanged);
    ToggleEventListener(true, "char-gender-edit-ok",    "click", EventGenderChanged);
    ToggleEventListener(true, "edit-value-cursor-plus", "click", ButtonUp);
    ToggleEventListener(true, "edit-value-cursor-less", "click", ButtonDown);
    ButtonUp.EventReceived.Connect  (*this, &StatViewRocket::StatMore);
    ButtonDown.EventReceived.Connect(*this, &StatViewRocket::StatLess);

    SetEditMode(Display);
    Translate();
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
  Core::Element* gender_edit   = _root->GetElementById("char-gender-edit");
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
  if (gender_edit)
    gender_edit->SetProperty("display", "none");
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
  Core::Element* current = _context->GetHoverElement();

  while (current && current->GetClassNames() != "skill-datagrid")
	  current = current->GetParentNode();  
  if (_editMode == StatView::Create && current)
  {
    Core::Variant* var = current->GetAttribute("data-key");
    Core::String   str = (var ? var->Get<Core::String>() : "");
    
    ToggleSkillAffinity.Emit(str.CString());
  }
  else if (_editMode == StatView::Update)
  {
    Core::Element* cursor = _root->GetElementById("edit-value-cursor");
    
    if (cursor && current)
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
  const char*    createElems[]  = { "continue", "cancel", "special-points-title", 0 };
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
      if (skill)   skill->AddEventListener  ("click", &EventSkillClicked);
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
  {
    if (category == "Kills")
    {
      stringstream rml;
      Core::String old_rml;

      if ((element = _root->GetElementById("panel-kills")))
      {
        element->GetInnerRML(old_rml);
        rml << "<datagrid>";
        rml << "<col width='80%'><span class='kills-key' i18n='" << key << "'>" << i18n::T(key) << "</span></col>";
        rml << "<col width='20%'><span class='kills-value' id='" << strId << "'>" << value << "</span></col>";
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

void StatViewRocket::SetExperience(unsigned int xp, unsigned short lvl, unsigned int next_level)
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

void StatViewRocket::SetSkillAffinity(const string& skill, bool active)
{
  if (_root)
  {
    string         elem_id = "skill-datagrid-" + underscore(skill);
    Core::Element* elem    = _root->GetElementById(elem_id.c_str());

    if (elem)
      elem->SetProperty("color", (active ? "yellow" : "white"));
  }
}

void StatViewRocket::SetTraitActive(const string& trait, bool active)
{
  if (_root)
  {
    string         elem_id = "text-" + underscore(trait);
    Core::Element* elem    = _root->GetElementById(elem_id.c_str());
    
    if (elem)
      elem->SetProperty("color", (active ? "yellow" : "white"));
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
      
      for_each(traits.begin(), traits.end(), [this, &rml](const string trait)
      {
	rml << "<div class='traits-row'><button id='" << underscore(trait) << "' class='small_button'>O</button>";
	rml << "<span class='text-trait' id='text-" << underscore(trait) << "'>" << _i18n[trait].Value() << "</span><br /></div>";
      });
      element->SetInnerRML(rml.str().c_str());

      _traits.clear();
      for_each(traits.begin(), traits.end(), [this](const string trait)
      {
	string elem_id = underscore(trait);
	ToggleEventListener(true, elem_id.c_str(), "click", EventTraitClicked);
	_traits.push_back(_root->GetElementById(elem_id.c_str()));
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
	  rml << "  <p class='special-value console-value' id='special-value-" << underscored << "'>0</p>\n";
	  rml << "  <p class='special-commt console-value' id='special-commt-" << underscored << "'>Great</p>\n";
          rml << "</p>\n\n";
	  topX += 40;
	}
	else if (category == "Statistics")
	{
	  rml << "<datagrid  class='statistics-datagrid'>\n";
          rml << "  <col width='80%'><span class='statistics-key'>" << _i18n[keys[i]].Value() << "</span></col>\n";
          rml << "  <col width='15%'><span class='statistics-value' id='statistics-value-" << underscored << "'></span></col>\n";
          rml << "</datagrid>\n\n";
	}
	else if (category == "Skills")
	{
	  rml << "<datagrid id='skill-datagrid-" << underscored << "' class='skill-datagrid' data-type='Skills' data-key='" << keys[i] << "'>\n";
          rml << "  <col width='70%'><span class='skill-key'>" << _i18n[keys[i]].Value() << "</span></col>\n";
          rml << "  <col width='20%'><span class='skill-value' id='skills-value-" << underscored << "'>0</span></col>\n";
          rml << "  <col width='5%'><span>%</span></col>";
          rml << "</datagrid>\n\n";
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
  if (_root)
  {
    Core::Element* panel_perks = _root->GetElementById("panel-perks");
    stringstream   rml;

    for_each(perks.begin(), perks.end(), [&rml](const string& perk)
    {
      rml << "- " << perk << "<br />" << endl;
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

/*
 * PerksDialog
 */
StatViewRocket::PerksDialog::PerksDialog(WindowFramework* w, Core::Context* c) : UiBase(w, c)
{
  _root = c->LoadDocument("data/perks_menu.rml");
  if (_root)
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
    cerr << "[PerksDialog][Missing RML Template]" << endl;
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
  PerkSelected.Emit(data_perk);
}

void StatViewRocket::PerksDialog::CallbackChoosePerk(Core::Event&)
{
  PerkChoosen.Emit(_selected_perk);
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
  if (_root)
  {
    Core::Element* element = _root->GetElementById("perks-selector");
    
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

void StatViewRocket::PerksDialog::SetPerkDescription(string description)
{
  if (_root)
  {
    Core::Element* element = _root->GetElementById("perks-description");
    
    if (element)
      element->SetInnerRML(description.c_str());
  }
}
