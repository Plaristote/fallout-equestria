#ifndef  STATSHEET_HPP
# define STATSHEET_HPP

# include "rocket_extension.hpp"
# include "datatree.hpp"
# include "scriptengine.hpp"

class StatModel
{
public:
  StatModel(Data statsheet);
  ~StatModel(void);
  
  void           SetName(const std::string& name)  { _statsheet["Name"]   = name;           }
  std::string    GetName(void) const               { return (_statsheet["Name"].Value());   }
  void           SetRace(const std::string& race)  { _statsheet["Race"]   = race;           }
  std::string    GetRace(void) const               { return (_statsheet["Race"].Value());   }
  void           SetAge(unsigned short age)        { _statsheet["Age"]    = age;            }
  unsigned short GetAge(void) const                { return (_statsheet["Age"]);            }
  void           SetGender(const std::string& g)   { _statsheet["Gender"] = (g == "male" ? "Stallion" : "Mare"); }
  std::string    GetGender(void) const             { return (_statsheet["Gender"].Value()); }
  
  void           ToggleTrait(const std::string& trait);
  bool           HasTrait(const std::string& trait)    const { return (_statsheet["Traits"][trait] == 1); }

  void           SetStatistic(const std::string& stat, short value);
  void           SetSpecial(const std::string& stat, short value);
  void           SetSkill(const std::string& stat, short value);

  unsigned short GetLevel(void)                        const;
  std::string    GetStatistic(const std::string& stat) const;
  short          GetSpecial(const std::string& stat)   const;
  short          GetSkill(const std::string& stat)     const;
  
  unsigned short GetSpecialPoints(void)                const { return (_statsheet["Variables"]["Special Points"]); }
  unsigned short GetSkillPoints(void)                  const { return (_statsheet["Variables"]["Skill Points"]);   }
  unsigned short GetPerksPoints(void)                  const { return (_statsheet["Variables"]["Perks"]);          }

  std::list<std::string>   GetAvailablePerks(void);
  std::list<std::string>   GetAvailableTraits(void);
  std::vector<std::string> GetStatistics(void) const { return (GetStatKeys(_statsheet["Statistics"])); }
  std::vector<std::string> GetSpecials(void)   const { return (GetStatKeys(_statsheet["Special"])); }
  std::vector<std::string> GetSkills(void)     const { return (GetStatKeys(_statsheet["Skills"])); }

  unsigned short GetXpNextLevel(void);
  unsigned short GetExperience(void) const       { return (_statsheet["Variables"]["Experience"]); }
  void           SetExperience(unsigned short e);
  void           LevelUp(void);
  
  Observatory::Signal<void (unsigned short)>            LevelUpped;
  Observatory::Signal<void (const std::string&, short)> SpecialChanged, SkillChanged, StatisticChanged;

private:
  typedef std::pair<asIScriptFunction**, std::string> ScriptFuncPtr;
  typedef std::list<ScriptFuncPtr>                    ScriptFuncPtrs;
  void           LoadFunctions(void);
  void           ReloadFunction(asIScriptFunction**);
  ScriptFuncPtrs _script_func_ptrs;
  
  bool           UpdateAllValues(void);
  
  std::vector<std::string> GetStatKeys(Data stats) const;

  Data _statsheet;

  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction *_scriptAddSpecialPoint, *_scriptActivateTraits, *_scriptAddExperience;
  asIScriptFunction *_scriptXpNextLevel,     *_scriptLevelUp,        *_scriptUpdateAllValues;
  asIScriptFunction *_scriptIsReady,         *_scriptAvailableTraits;
};

class StatView
{
public:
  virtual ~StatView(void) {}
  
  enum EditMode { Create, Update, Display };

  virtual void SetEditMode(EditMode) = 0;
  
  virtual void SetInformation(const std::string& name, const std::string& value)                            = 0;
  virtual void SetInformation(const std::string& name, short value)                                         = 0;
  virtual void SetFieldValue(const std::string& category, const std::string& key, const std::string& value) = 0;
  virtual void SetFieldValue(const std::string& category, const std::string& key, short value)              = 0;
  virtual void SetCategoryFields(const std::string& category, const std::vector<std::string>& keys)         = 0;  
  virtual void SetIdValue(const std::string& id, const std::string& value)                                  = 0;
  virtual void SetIdValue(const std::string& id, short value)                                               = 0;
  virtual void SetExperience(unsigned short xp, unsigned short lvl, unsigned short next_level)              = 0;
  virtual void SetTraits(std::list<std::string>)                                                                      = 0;
  
  Observatory::Signal<void (const std::string&, const std::string&)> StatUpped, StatDowned; 
  Observatory::Signal<void (const std::string&, const std::string&)> InformationChanged;
  Observatory::Signal<void (unsigned char)>                          AgeChanged;
  Observatory::Signal<void (const std::string&)>                     TraitToggled;
};

class StatController
{
public:
  StatController(Data statsheet);
  
  void SetView(StatView*);

  void UpSpecial(const std::string& stat);
  void DownSpecial(const std::string& stat);
  void SetSpecial(const std::string& stat, short value);

  void SetStatistic(const std::string& stat, short value);

  void UpSkill(const std::string& stat);
  void DownSkill(const std::string& stat);
  void SetSkill(const std::string& stat, short value);

  void AddExperience(unsigned short experience);
  
  Observatory::Signal<void (unsigned short)> LevelUp;

private:
  void      SpecialChanged(const std::string&, short);
  void      SkillChanged(const std::string&, short);
  void      StatisticChanged(const std::string&, short);
  void      TraitToggled(const std::string&);
  void      LevelChanged(unsigned short);
  void      InformationChanged(const std::string&, const std::string&);
  void      AgeChanged(unsigned char);

  void      ViewStatUpped(const std::string&, const std::string&);
  void      ViewStatDowned(const std::string&, const std::string&);
  
  StatModel _model;
  StatView* _view;
};

class StatViewRocket : public UiBase, public StatView
{
public:
  StatViewRocket(WindowFramework* window, Rocket::Core::Context* context);
  
  void SetEditMode(EditMode);

  void SetInformation(const std::string& name, const std::string& value);
  void SetInformation(const std::string& name, short value);
  void SetFieldValue(const std::string& category, const std::string& key, const std::string& value);
  void SetFieldValue(const std::string& category, const std::string& key, short value);
  void SetIdValue(const std::string& id, const std::string& value);
  void SetIdValue(const std::string& id, short value);
  void SetCategoryFields(const std::string& category, const std::vector<std::string>& keys);
  void SetExperience(unsigned short, unsigned short, unsigned short);
  void SetTraits(std::list<std::string>);

private:  
  RocketListener CancelButton;
  RocketListener DoneButton;
  RocketListener EventSpecialClicked, EventSkillClicked, EventGeneralClicked, EventTraitClicked;
  RocketListener ButtonUp, ButtonDown;
  
  RocketListener EventNameChanged, EventAgeChanged, EventGenderChanged;

  void           Close(Rocket::Core::Event&) { Hide(); }
  
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
  
  EditMode               _editMode;
};

#endif