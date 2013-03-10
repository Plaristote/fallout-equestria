#ifndef  STATSHEET_HPP
# define STATSHEET_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"
# include "datatree.hpp"
# include "scriptengine.hpp"
# include "scriptable.hpp"
# include <cstdarg>

class StatModel : public Scriptable
{
public:
  StatModel(Data statsheet);
  ~StatModel(void);
  
  void           Backup(void);
  void           RestoreBackup(void);

  int            Action(const std::string& action, const std::string& fmt, ...);
  Data           GetAll(void) { return (_statsheet); }

  void           SetName(const std::string& name)  { _statsheet["Name"]   = name;           }
  std::string    GetName(void) const               { return (_statsheet["Name"].Value());   }
  void           SetRace(const std::string& race)  { _statsheet["Race"]   = race;           }
  std::string    GetRace(void) const               { return (_statsheet["Race"].Value());   }
  void           SetAge(unsigned short age)        { _statsheet["Age"]    = age;            }
  unsigned short GetAge(void) const                { return (_statsheet["Age"]);            }
  void           SetGender(const std::string& g)   { _statsheet["Gender"] = (g == "male" ? "Stallion" : "Mare"); }
  std::string    GetGender(void) const             { return (_statsheet["Gender"].Value()); }

  void           AddKill(const std::string&);
  unsigned int   GetKills(const std::string& race) const { return (_statsheet["Kills"][race]); }

  bool           AddPerk(const std::string& perk);
  void           ToggleTrait(const std::string& trait);
  void           ToggleSkillAffinity(const std::string& skill);
  bool           HasTrait(const std::string& trait)         const { return (_statsheet["Traits"][trait]     == 1); }
  bool           HasSkillAffinity(const std::string& skill) const { return (_statsheet["Affinities"][skill] == 1); }
  Data           GetSkillAffinities(void)                         { return (_statsheet["Affinities"]);             }

  void           SetStatistic(const std::string& stat, short value);
  void           SetSpecial(const std::string& stat, short value);
  void           SetSkill(const std::string& stat, short value);

  unsigned short GetLevel(void)                        const;
  std::string    GetStatistic(const std::string& stat) const;
  short          GetSpecial(const std::string& stat)   const;
  short          GetSkill(const std::string& stat)     const;
  
  std::string    SelectRandomEncounter(void);
  
  unsigned short GetSpecialPoints(void)                const { return (_statsheet["Variables"]["Special Points"]); }
  unsigned short GetSkillPoints(void)                  const { return (_statsheet["Variables"]["Skill Points"]);   }
  unsigned short GetPerksPoints(void)                  const { return (_statsheet["Variables"]["Perks"]);          }

  std::list<std::string>   GetAvailablePerks(void);
  std::list<std::string>   GetAvailableTraits(void);
  std::list<std::string>   GetPerks(void)      const;
  std::list<std::string>   GetTraits(void)     const;
  std::vector<std::string> GetStatistics(void) const { return (GetStatKeys(_statsheet["Statistics"])); }
  std::vector<std::string> GetSpecials(void)   const { return (GetStatKeys(_statsheet["Special"])); }
  std::vector<std::string> GetSkills(void)     const { return (GetStatKeys(_statsheet["Skills"])); }

  unsigned short GetXpNextLevel(void);
  unsigned short GetExperience(void) const       { return (_statsheet["Variables"]["Experience"]); }
  void           SetExperience(unsigned short e);
  void           LevelUp(void);
  void           SetCurrentHp(short hp);
  short          GetCurrentHp(void) const { return (_statsheet["Variables"]["Hit Points"]);  }
  short          GetMaxHp(void)     const { return (_statsheet["Statistics"]["Hit Points"]); }
  
  bool           IsReady(void);
  
  Sync::Signal<void (unsigned short)>            LevelUpped;
  Sync::Signal<void (const std::string&, short)> SpecialChanged, SkillChanged, StatisticChanged;
  Sync::Signal<void (short)>                     MaxHpChanged;
  Sync::Signal<void>                             PerksChanged;

  bool           UpdateAllValues(void);  
  
private:
  void           LoadFunctions(void);
  
  std::vector<std::string> GetStatKeys(Data stats) const;

  Data               _statsheet;
  Data               _statsheet_backup;
  asIScriptFunction *_scriptAddSpecialPoint, *_scriptActivateTraits,  *_scriptAddExperience;
  asIScriptFunction *_scriptXpNextLevel,     *_scriptLevelUp,         *_scriptUpdateAllValues;
  asIScriptFunction *_scriptIsReady,         *_scriptAvailableTraits, *_scriptAddPerk;
  asIScriptFunction *_selectRandomEncounter;
};

class StatView
{
public:
  virtual ~StatView(void) {}
  
  enum EditMode { Create, Update, Display };

  virtual void SetEditMode(EditMode) = 0;
  EditMode     GetEditMode(void) { return (_editMode); }
  
  virtual void Hide(void) = 0;
  virtual void Show(void) = 0;

  virtual void SetInformation(const std::string& name, const std::string& value)                            = 0;
  virtual void SetInformation(const std::string& name, short value)                                         = 0;
  virtual void SetFieldValue(const std::string& category, const std::string& key, const std::string& value) = 0;
  virtual void SetFieldValue(const std::string& category, const std::string& key, short value)              = 0;
  virtual void SetCategoryFields(const std::string& category, const std::vector<std::string>& keys)         = 0;  
  virtual void SetIdValue(const std::string& id, const std::string& value)                                  = 0;
  virtual void SetIdValue(const std::string& id, short value)                                               = 0;
  virtual void SetExperience(unsigned int xp, unsigned short lvl, unsigned int next_level)                  = 0;
  virtual void SetTraits(std::list<std::string>)                                                            = 0;
  virtual void SetTraitActive(const std::string&, bool)                                                     = 0;
  virtual void SetPerks(std::list<std::string>)                                                             = 0;
  virtual void SetAvailablePerks(std::list<std::string> perks)                                              = 0;
  virtual void SetSkillAffinity(const std::string& skill, bool)                                             = 0;

  Sync::Signal<void (const std::string&, const std::string&)> StatUpped, StatDowned; 
  Sync::Signal<void (const std::string&, const std::string&)> InformationChanged;
  Sync::Signal<void (unsigned char)>                          AgeChanged;
  Sync::Signal<void (const std::string&)>                     TraitToggled;
  Sync::Signal<void (const std::string&)>                     PerkToggled;
  Sync::Signal<void (const std::string&)>                     ToggleSkillAffinity;
  Sync::Signal<void>                                          Accepted, Canceled, MakeBackup;

  void         SetNumPerks(unsigned short n_perks)             { _n_perks = n_perks; }

protected:
  EditMode       _editMode;
  unsigned short _n_perks;
};

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

  void AddExperience(unsigned int experience);
  void SetCurrentHp(short hp);

  void TriggerSkillAffinity(const std::string& stat, bool);

  void AddKill(const std::string& race);
  void RunMetabolism(void);

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

  void      ViewStatUpped(const std::string&, const std::string&);
  void      ViewStatDowned(const std::string&, const std::string&);
  
  void      AcceptChanges(void);
  void      CancelChanges(void);
  void      MakeBackup(void);

  StatModel                    _model;
  StatView*                    _view;
  Sync::ObserverHandler _viewObservers;
};

class StatViewRocket : public UiBase, public StatView
{
  struct PerksDialog : public UiBase
  {
    PerksDialog(WindowFramework* window, Rocket::Core::Context* context);
    ~PerksDialog();
    
    void SetAvailablePerks(std::list<std::string> perks);
    void SetPerkDescription(std::string description);

    Sync::Signal<void (const std::string&)> PerkSelected;
    Sync::Signal<void (const std::string&)> PerkChoosen;
    
  private:
    void ClearPerksButtons(void);
    void SetSelectedPerk(Rocket::Core::Event& event);
    void CallbackChoosePerk(Rocket::Core::Event& event);
    void CallbackCancel(Rocket::Core::Event& event);
    void CallbackDblClickPerk(Rocket::Core::Event& event);

    RocketListener Cancel;
    RocketListener SelectPerk;
    RocketListener ChoosePerk;    
    RocketListener DblClickPerk;

    std::vector<Rocket::Core::Element*> _perks_buttons;
    string                              _selected_perk;
  };

public:
  StatViewRocket(WindowFramework* window, Rocket::Core::Context* context);
  ~StatViewRocket();
  
  void SetEditMode(EditMode);
  
  void Hide(void);
  void Show(void);

  void SetInformation(const std::string& name, const std::string& value);
  void SetInformation(const std::string& name, short value);
  void SetFieldValue(const std::string& category, const std::string& key, const std::string& value);
  void SetFieldValue(const std::string& category, const std::string& key, short value);
  void SetIdValue(const std::string& id, const std::string& value);
  void SetIdValue(const std::string& id, short value);
  void SetCategoryFields(const std::string& category, const std::vector<std::string>& keys);
  void SetExperience(unsigned int, unsigned short, unsigned int);
  void SetTraits(std::list<std::string>);
  void SetTraitActive(const std::string&, bool);
  void SetSkillAffinity(const std::string& skill, bool);
  void SetPerks(std::list<std::string>);
  void SetAvailablePerks(std::list<std::string>);

private:
  Data           _i18n;
  PerksDialog    _perks_dialog;
  RocketListener CancelButton;
  RocketListener DoneButton;
  RocketListener EventSpecialClicked, EventSkillClicked, EventGeneralClicked, EventTraitClicked;
  RocketListener ButtonUp, ButtonDown;
  
  RocketListener EventNameChanged, EventAgeChanged, EventGenderChanged;

  void           Cancel(Rocket::Core::Event&) { Canceled.Emit(); }
  void           Accept(Rocket::Core::Event&) { Accepted.Emit(); }
  
  void           UpdateName(Rocket::Core::Event&);
  void           UpdateGender(Rocket::Core::Event&);
  void           UpdateAge(Rocket::Core::Event&);
  
  void           TraitClicked(Rocket::Core::Event&);
  void           SpecialClicked(Rocket::Core::Event&);
  void           SkillClicked(Rocket::Core::Event&);
  void           GeneralClicked(Rocket::Core::Event&);
  void           StatMore(Rocket::Core::Event&);
  void           StatLess(Rocket::Core::Event&);
  void           StatUpdate(Rocket::Core::Event& event, std::string& type, std::string& stat);
  
  RocketListener         SpecialSelected, SkillSelected, TraitSelected;
  Rocket::Core::Element* _specialSelected;
  Rocket::Core::Element* _skillSelected;
  Rocket::Core::Element* _traitSelected;
  
  std::list<Rocket::Core::Element*> _traits;
};

#endif
