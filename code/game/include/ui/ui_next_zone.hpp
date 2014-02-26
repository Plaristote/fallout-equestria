#ifndef  UI_NEXT_ZONE_HPP
# define UI_NEXT_ZONE_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"
# include <vector>

class UiNextZone : public UiBase
{
public:
  UiNextZone(WindowFramework* window, Rocket::Core::Context* context, const std::vector<std::string> zones);
  ~UiNextZone();

  Sync::Signal<void (const std::string&)> NextZoneSelected;
  Sync::Signal<void (void)>               Done;
  Sync::Signal<void (void)>               Cancel;

private:
  RocketListener LevelSelected, CancelSelected;

  void CallbackLevelSelected(Rocket::Core::Event&);
  void CallbackCancel(Rocket::Core::Event&);

  std::vector<Rocket::Core::Element*> _elements;
};

#endif

