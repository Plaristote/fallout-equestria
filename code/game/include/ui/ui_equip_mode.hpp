#ifndef  UI_EQUIP_MODE_HPP
# define UI_EQUIP_MODE_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"

class UiEquipMode : public UiBase
{
public:
  UiEquipMode(WindowFramework* window, Rocket::Core::Context* context);
  ~UiEquipMode();

  void                               AddOption(unsigned char code, const std::string& name);
  void                               Initialize(void);

  Sync::Signal<void (unsigned char)> EquipModeSelected;
  Sync::Signal<void>                 Closed;

  void Destroy(void);

private:
  RocketListener   ModeClicked, CancelClicked;
  Rocket::Core::Element* root_choices;

  void CallbackCancel(Rocket::Core::Event&) { Closed.Emit(); }
  void ListenButtons(bool);
};

#endif
