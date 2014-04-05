#ifndef  STATMODEL_HPP
# define STATMODEL_HPP

# include "globals.hpp"
# include "datatree.hpp"
# include "scriptengine.hpp"
# include "as_object.hpp"
# include <string>
# include <vector>
# include <list>
# include <cstdarg>

class StatModel : public AngelScript::Object
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
  unsigned int   GetKills(const std::string& race) const { return (_statsheet["Kills"][race].Or(0)); }

  bool           AddPerk(const std::string& perk);
  Data           GetPerk(const std::string& perk);
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

  std::list<std::string>   GetUsableSkills(bool on_self = false);
  std::list<std::string>   GetUsableSpells(bool on_self = false);

  unsigned short GetXpNextLevel(void);
  unsigned short GetExperience(void) const       { return (_statsheet["Variables"]["Experience"]); }
  void           SetExperience(unsigned short e);
  void           LevelUp(void);
  void           SetCurrentHp(short hp);
  void           SetArmorClass(unsigned short ac)   { _statsheet["Variables"]["Armor Class"]   = ac;    }
  void           SetActionPoints(unsigned short ap) { _statsheet["Variables"]["Action Points"] = ap;    }
  short          GetCurrentHp(void)      const      { return (_statsheet["Variables"]["Hit Points"].Or(GetMaxHp()));            }
  short          GetMaxHp(void)          const      { return (_statsheet["Statistics"]["Hit Points"].Or(1));                    }
  unsigned short GetArmorClass(void)     const      { return (_statsheet["Variables"]["Armor Class"].Or(GetBaseArmorClass()));  }
  unsigned short GetBaseArmorClass(void) const      { return (_statsheet["Statistics"]["Armor Class"].Or(1));                   }

  int            GetReputation(const std::string& faction) const;
  void           AddReputation(const std::string& faction, int amount);
  
  bool           IsReady(void);
  
  Sync::Signal<void (unsigned short)>            LevelUpped;
  Sync::Signal<void (const std::string&, short)> SpecialChanged, SkillChanged, StatisticChanged;
  Sync::Signal<void (short)>                     MaxHpChanged;
  Sync::Signal<void>                             PerksChanged;

  bool           UpdateAllValues(void);  
  
private:
  std::vector<std::string> GetStatKeys(Data stats) const;

  Data               _statsheet;
  Data               _statsheet_backup;
};

#endif
