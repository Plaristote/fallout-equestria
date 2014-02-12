#ifndef  STATCONTROLLER_HPP
# define STATCONTROLLER_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"
# include "datatree.hpp"
# include "statmodel.hpp"

class StatView;

class StatController
{
public:
  StatController(Data statsheet);
  
  StatModel&       Model(void)         { return (_model);          }
  const StatModel& Model(void) const   { return (_model);          }
  Data             GetData(void)       { return (_model.GetAll()); }

  void SetView(StatView*);

  void UpSpecial(const std::string& stat);
  void DownSpecial(const std::string& stat);
  void SetSpecial(const std::string& stat, short value);

  void SetStatistic(const std::string& stat, short value);

  void UpSkill(const std::string& stat);
  void DownSkill(const std::string& stat);
  void SetSkill(const std::string& stat, short value);
  void TriggerSkillAffinity(const std::string& stat, bool);

  void AddExperience(unsigned int experience);
  void SetCurrentHp(short hp);
  void RunMetabolism(void);

  void AddKill(const std::string& race);
  void AddReputation(const std::string& faction, int);

  Sync::Signal<void (short)>          HpChanged;
  Sync::Signal<void (unsigned short)> LevelUp;
  Sync::Signal<void>                  ChangesAccepted;
  Sync::Signal<void>                  ChangesCanceled;

private:
  void      SetMaxHp(short hp);
  void      SpecialChanged(const std::string&, short);
  void      SkillChanged(const std::string&, short);
  void      StatisticChanged(const std::string&, short);
  void      PerksChanged(void);
  void      TraitToggled(const std::string&);
  void      SkillAffinityToggled(const std::string&);
  void      LevelChanged(unsigned short);
  void      InformationChanged(const std::string&, const std::string&);
  void      AgeChanged(unsigned char);
  void      PerkAdded(const std::string&);
  void      UpdateDetails(const std::string& type, const std::string& name);

  void      ViewStatUpped(const std::string&, const std::string&);
  void      ViewStatDowned(const std::string&, const std::string&);

  void      AcceptChanges(void);
  void      CancelChanges(void);
  void      MakeBackup(void);

  StatModel                    _model;
  StatView*                    _view;
  Sync::ObserverHandler _viewObservers;
};

#endif
