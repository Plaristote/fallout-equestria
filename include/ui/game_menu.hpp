#ifndef  GAME_MENU_HPP
# define GAME_MENU_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "rocket_extension.hpp"
# include "observatory.hpp"

class GameMenu : public UiBase
{
  friend class GameUi;
public:
  GameMenu(WindowFramework* window, Rocket::Core::Context* context);
  ~GameMenu();
  void MenuEventContinue(Rocket::Core::Event& event) { Hide(); }
  
  Sync::Signal<void (Rocket::Core::Event&)> SaveClicked;
  Sync::Signal<void (Rocket::Core::Event&)> LoadClicked;
  Sync::Signal<void (Rocket::Core::Event&)> ExitClicked;
  Sync::Signal<void (Rocket::Core::Event&)> OptionsClicked;

private:
  RocketListener         _continueClicked;
  RocketListener         _exitClicked;
  RocketListener         _optionsClicked;  
  RocketListener         _saveClicked;
  RocketListener         _loadClicked;
};


#endif
