#ifndef  STATVIEW_HPP
# define STATVIEW_HPP

# include "globals.hpp"
# include "observatory.hpp"
# include <string>
# include <list>

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
  virtual void SetPerkDescription(const std::string& icon, const std::string& description)                  = 0;
  virtual void SetAvailablePerks(std::list<std::string> perks)                                              = 0;
  virtual void SetSkillAffinity(const std::string& skill, bool)                                             = 0;
  virtual void SetReputation(const std::string& faction, int)                                               = 0;
  virtual void SetDetails(const std::string& icon, const std::string& title, const std::string& text)       = 0;

  Sync::Signal<void (const std::string&, const std::string&)> StatUpped, StatDowned; 
  Sync::Signal<void (const std::string&, const std::string&)> InformationChanged;
  Sync::Signal<void (unsigned char)>                          AgeChanged;
  Sync::Signal<void (const std::string&)>                     TraitToggled;
  Sync::Signal<void (const std::string&)>                     PerkToggled;
  Sync::Signal<void (const std::string&)>                     ToggleSkillAffinity;
  Sync::Signal<void>                                          Accepted, Canceled, MakeBackup;
  Sync::Signal<void (const std::string&)>                     UpdatePerkDescription;
  Sync::Signal<void (const std::string&, const std::string&)> UpdateDetails;

  void         SetNumPerks(unsigned short n_perks)             { _n_perks = n_perks; }

protected:
  EditMode       _editMode;
  unsigned short _n_perks;
};

#endif
