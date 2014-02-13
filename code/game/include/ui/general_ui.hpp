#ifndef  GENERAL_UI_HPP
# define GENERAL_UI_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/rocketRegion.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"
# include "game_console.hpp"
# include "game_options.hpp"

class GeneralUi
{
public:
  GeneralUi(WindowFramework*);
  ~GeneralUi();

  PT(RocketRegion)       GetRocketRegion(void) const { return (_rocket); }
  GameOptions&           GetOptions(void) { return (*_options); }

private:
  WindowFramework*       _window;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;

  GameConsole*           _console;
  GameOptions*           _options;
};

#endif
