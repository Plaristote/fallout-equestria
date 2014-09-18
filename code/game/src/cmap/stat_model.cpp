#include "cmap/statmodel.hpp"
#include "ui/gameui.hpp"
#include "ui/alert_ui.hpp"

using namespace std;

StatModel::StatModel(Data statsheet) : AngelScript::Object("scripts/ai/special.as"), _statsheet(statsheet)
{
  asDefineMethod("AvailableTraits",       "StringList AvailableTraits(Data)");
  asDefineMethod("ActivateTraits",        "bool ActivateTraits(Data, string, bool)");
  asDefineMethod("AddExperience",         "void AddExperience(Data, int)");
  asDefineMethod("AddSpecialPoint",       "bool AddSpecialPoint(Data, string, int)");
  asDefineMethod("XpNextLevel",           "int  XpNextLevel(Data)");
  asDefineMethod("LevelUp",               "void LevelUp(Data)");
  asDefineMethod("IsReady",               "bool IsReady(Data)");
  asDefineMethod("UpdateAllValues",       "void UpdateAllValues(Data)");
  asDefineMethod("AddPerk",               "bool AddPerk(Data, string)");
  asDefineMethod("AvailableSkills",       "StringList AvailableSkills(Data, bool)");
  asDefineMethod("AvailableSpells",       "StringList AvailableSpells(Data, bool)");
  asDefineMethod("SelectRandomEncounter", "string SelectRandomEncounter(Data)");
}

StatModel::~StatModel(void)
{
  cout << "Destroying StatModel" << endl;
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
  
  if (IsDefined("AddPerk"))
  {
    string tmp = perk;
    AngelScript::Type<string*> param_perk(&tmp);
    AngelScript::Type<Data*>   param_stats(&_statsheet);

    success = Call("AddPerk", 2, &param_stats, &param_perk);
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
  return (_statsheet["Variables"]["Level"].Or(1));
}

void           StatModel::LevelUp(void)
{
  if (IsDefined("LevelUp"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);
    
    Call("LevelUp", 1, &param_stats);
  }
  else
    _statsheet["Variables"]["Level"] = GetLevel() + 1;
  LevelUpped.Emit(GetLevel());
}

bool           StatModel::IsReady(void)
{
  if (IsDefined("IsReady"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);

    return (Call("IsReady", 1, &param_stats));
  }
  return (true);
}

list<string>   StatModel::GetUsableSkills(bool on_self_only)
{
  list<string> ret;

  if (IsDefined("AvailableSkills"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);
    AngelScript::Type<bool>  param_self_only(on_self_only);

    ret = *(list<string>*)(Call("AvailableSkills", 2, &param_stats, &param_self_only));
  }
  else // Fallback
  {
    if (!on_self_only)
    {
      ret.push_back("Lockpick");
      ret.push_back("Medicine");
      ret.push_back("Science");
      ret.push_back("Repair");
      ret.push_back("Steal");
    }
    else
    {
      ret.push_back("Medicine");
      ret.push_back("Sneak");
    }
  }
  return (ret);
}

list<string>   StatModel::GetUsableSpells(bool on_self_only)
{
  list<string> ret;

  if (IsDefined("AvailableSpells"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);
    AngelScript::Type<bool>  param_self_only(on_self_only);

    ret = *(list<string>*)(Call("AvailableSpells", 2, &param_stats, &param_self_only));
  }
  return (ret);
}

list<string>   StatModel::GetAvailableTraits(void)
{
  list<string> ret;

  if (IsDefined("AvailableTraits"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);

    ret = *(list<string>*)(Call("AvailableTraits", 1, &param_stats));
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
  if (IsDefined("ActivateTraits"))
  {
    Data   dtrait = _statsheet["Traits"][trait];
    string tmp    = trait;
    AngelScript::Type<Data*>   param_stats(&_statsheet);
    AngelScript::Type<string*> param_name(&tmp);
    AngelScript::Type<bool>    param_active(!((!dtrait.Nil()) && dtrait == 1));

    if ((bool)(Call("ActivateTraits", 3, &param_stats, &param_name, &param_active)) == true)
      UpdateAllValues();
  }
}

void           StatModel::SetExperience(unsigned short e)
{
  AngelScript::Type<Data*> param_stats(&_statsheet);
  unsigned int   currentXp = _statsheet["Variables"]["Experience"];
  unsigned short nextLevel = 0;

  _statsheet["Variables"]["Experience"] = e;
  if (IsDefined("AddExperience"))
  {
    AngelScript::Type<int> param_xp(e - currentXp);

    Call("AddExperience", 2, &param_stats, &param_xp);
  }
  else
    _statsheet["Variables"]["Experience"] = currentXp + e;
  if (IsDefined("XpNextLevel"))
    nextLevel = (int)Call("XpNextLevel", 1, &param_stats);
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

  if (IsDefined("AddSpecialPoint"))
  {
    string                     stat_ = stat;
    AngelScript::Type<Data*>   param_stats(&_statsheet);
    AngelScript::Type<string*> param_stat(&stat_);
    AngelScript::Type<int>     param_value(value - currentValue);

    sendSignal = Call("AddSpecialPoint", 3, &param_stats, &param_stat, &param_value);
  }
  else
  {
    cout << "No AddSpecialPoint function" << endl;
    sendSignal = (short)_statsheet["Special"][stat] != value;
    _statsheet["Special"][stat] = value;
  }
  if (sendSignal)
  {
    cout << "Sending signal " << _statsheet["Special"][stat].Value() << endl;
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
  if (IsDefined("UpdateAllValues"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);

    Call("UpdateAllValues", 1, &param_stats);

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
  if (IsDefined("XpNextLevel"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);
    
    return ((int)(Call("XpNextLevel", 1, &param_stats)));
  }
  return (1);
}

int           StatModel::Action(const std::string& action, const std::string& fmt, ...)
{
  asIScriptContext* _script_context = GetContext();
  asIScriptModule*  _script_module  = GetModule();
  string             func_name      = "action_" + action;
  asIScriptFunction* action_func    = _script_module->GetFunctionByName(func_name.c_str());
  
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
  bool           affinity       = HasSkillAffinity(stat);

  v_skill_points += (current_value - value);
  if (affinity)
    value += (value - current_value);
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
  return (_statsheet["Special"][stat].Or(1));
}

short          StatModel::GetSkill(const std::string& stat)     const
{
  return (_statsheet["Skills"][stat].Or(1));
}

vector<string> StatModel::GetStatKeys(Data stats) const
{
  vector<string> keys;
  
  for_each(stats.begin(), stats.end(), [&keys](Data field) { keys.push_back(field.Key()); });
  return (keys);
}

string StatModel::SelectRandomEncounter(void)
{
  if (IsDefined("SelectRandomEncounter"))
  {
    AngelScript::Type<Data*> param_stats(&_statsheet);
    
    Call("SelectRandomEncounter", 1, &param_stats);
  }
  else
    AlertUi::NewAlert.Emit("Missing special.as function `string SelectRandomEncounter(Data)`");
  return ("");
}

int StatModel::GetReputation(const std::string& faction) const
{
  return (_statsheet["Reputation"][faction].Or(0));
}

void StatModel::AddReputation(const std::string& faction, int amount)
{
  Data reputation         = _statsheet["Reputation"][faction];
  int  current_reputation = 0;

  if (reputation.NotNil())
    current_reputation    = reputation;
  current_reputation     += amount;
  reputation              = current_reputation;
}

list<string> split(const std::string& str, char c);

static Data DataGetFromPathRec(Data data, list<string> array)
{
  if (!(array.empty()) && data.NotNil())
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

Data         StatModel::GetPerk(const std::string& perk_name)
{
  DataTree*    file = DataTree::Factory::JSON("data/perks.json");
  Data         result;

  if (file)
  {
    Data       perks(file);

    result = perks[perk_name];
    delete file;
  }
  return (result);
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
          string       comp        = requirement["Comp"].Value();
          short        value       = requirement["Value"];
          Data         data_check  = DataGetFromPath(_statsheet, requirement.Key());
          short        to_check    = data_check.Nil() ? 0 : (short)data_check;

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
