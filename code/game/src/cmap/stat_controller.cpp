#include "cmap/statcontroller.hpp"
#include "gametask.hpp"
#include "ui/gameui.hpp"
#include "ui/alert_ui.hpp"

using namespace std;

StatController::StatController(Data statsheet) : _model(statsheet), _view(0)
{
  _view = 0;
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
  if (_model.GetCurrentHp() > 0)
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
}

void StatController::AddKill(const string& race)
{
  if (race == "")
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

void StatController::AddReputation(const std::string& faction, int amount)
{
  _model.AddReputation(faction, amount);
  if (_view)
    _view->SetReputation(faction, _model.GetReputation(faction));
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
  if (!(_model.IsReady()))
  {
    cout << "[StatController] Thou art not ready" << endl;
    AlertUi::NewAlert.Emit("Your statistics are invalid.");
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

  for_each(_model.GetAll()["Reputation"].begin(), _model.GetAll()["Reputation"].end(), [this](Data data)
  { _view->SetReputation(data.Key(), data); });

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
  _viewObservers.Connect(_view->UpdatePerkDescription, [this](const std::string& perk_name)
  {
    Data perk = _model.GetPerk(perk_name);

    if (perk.NotNil())
      _view->SetPerkDescription(perk["Icon"].Value(), perk["Description"].Value());
  });
  _viewObservers.Connect(_view->UpdateDetails, [this](const string& type, const string& title)
  {
    _view->SetDetails("pipbuck/boy/" + underscore(title) + ".png", i18n::T(title), i18n::T(type + '-' + title + "-desc"));
  });

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
  if (hp <= 0)
    Died.Emit();
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

void StatController::SetActionPoints(unsigned short action_points)
{
  _model.SetActionPoints(action_points);
  ActionPointChanged.Emit(action_points);
}

void StatController::SetArmorClass(unsigned short armor_class)
{
  _model.SetArmorClass(armor_class);
  ArmorClassChanged.Emit(armor_class);
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
