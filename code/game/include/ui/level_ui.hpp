#ifndef  LEVEL_UI_HPP
# define LEVEL_UI_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "rocket_extension.hpp"
# include "observatory.hpp"
# include "gameui.hpp"
# include "game_main_bar.hpp"

class LevelUi
{
public:
  LevelUi(WindowFramework* window, GameUi& gameUi);
  ~LevelUi(void);

  Rocket::Core::Context* GetContext() { return (_gameUi.GetRocketRegion()->get_context()); }  
  GameMainBar&     GetMainBar(void)   { return (*_mainBar);              }
  GameMenu&        GetMenu(void)      { return (_gameUi.GetMenu());      }
  GameInventory&   GetInventory(void) { return (_gameUi.GetInventory()); }
  StatViewRocket&  GetPers(void)      { return (_gameUi.GetPers());      }

  Sync::Signal<void (bool)> InterfaceOpened;  

private:
  GameUi&                      _gameUi;
  GameMainBar*                 _mainBar;  
  Sync::ObserverHandler _obs;
};

#endif
