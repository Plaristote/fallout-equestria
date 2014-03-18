#ifndef  STAT_VIEW_ROCKET_HPP
# define STAT_VIEW_ROCKET_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "cmap/statview.hpp"
# include "ui/rocket_extension.hpp"

class StatViewRocket : public UiBase, public StatView
{
  struct PerksDialog : public UiBase
  {
    PerksDialog(WindowFramework* window, Rocket::Core::Context* context);
    ~PerksDialog();
    
    void SetAvailablePerks(std::list<std::string> perks);
    void SetPerkDescription(const std::string& icon, const std::string& description);

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
  
  void SetPartyMembers(const std::vector<std::string>& members);

  Sync::Signal<void (const std::string&)> SwapToPartyMember;

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
  void SetPerkDescription(const std::string& icon, const std::string& description);
  void SetAvailablePerks(std::list<std::string>);
  void SetReputation(const std::string&, int);
  void SetDetails(const std::string& icon, const std::string& title, const std::string& text);

private:
  PerksDialog    _perks_dialog;
  RocketListener CancelButton;
  RocketListener DoneButton;
  RocketListener EventSpecialClicked, EventSkillClicked, EventStatisticClicked, EventPerkClicked, EventGeneralClicked, EventTraitClicked, EventDetails;
  RocketListener ButtonUp, ButtonDown;
  RocketListener PartyMemberClicked;
  
  RocketListener EventNameChanged, EventAgeChanged, EventGenderChanged;

  void           Cancel(Rocket::Core::Event&) { Canceled.Emit(); }
  void           Accept(Rocket::Core::Event&) { Accepted.Emit(); }
  
  void           UpdateName(Rocket::Core::Event&);
  void           UpdateGender(Rocket::Core::Event&);
  void           UpdateAge(Rocket::Core::Event&);
  
  void           DisplayDetails(Rocket::Core::Event&);
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
