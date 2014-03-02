#ifndef  GAME_UI_HPP
# define GAME_UI_HPP

# include "globals.hpp"
# include <panda3d/pgVirtualFrame.h>
# include <panda3d/rocketRegion.h>
# include <Rocket/Core.h>
# include <panda3d/orthographicLens.h>
# include <Rocket/Core/XMLParser.h>
# include <panda3d/pandaFramework.h>
# include "observatory.hpp"
# include "scriptengine.hpp"
# include "rocket_extension.hpp"

# include "ui/game_menu.hpp"
# include "ui/game_main_bar.hpp"
# include "ui/game_inventory.hpp"
# include "ui/game_console.hpp"
# include "ui/stat_view_rocket.hpp"
# include "saveuis.hpp"

class GameUi
{
public:
  GameUi(WindowFramework* window, PT(RocketRegion) rocket);
  ~GameUi(void);
  
  Rocket::Core::Context* GetContext()       { return (_rocket->get_context()); }
  GameMenu&              GetMenu(void)      { return (*_menu); }
  GameInventory&         GetInventory(void) { return (*_inventory); }
  StatViewRocket&        GetPers(void)      { return (*_pers); }
  
  void                   OpenMenu(Rocket::Core::Event&);
  void                   OpenInventory(Rocket::Core::Event&);
  void                   OpenPers(Rocket::Core::Event&);
  UiLoad*                OpenLoadingInterface(const std::string& savepath);
  UiSave*                OpenSavingInterface(const std::string& savepath);
  
  Sync::Signal<void (Rocket::Core::Event&)> OpenPipbuck;
  
  WindowFramework*       GetWindowFramework(void) const { return (window);  }
  PT(RocketRegion)       GetRocketRegion(void)    const { return (_rocket); }

private:
  WindowFramework* window;
  PT(RocketRegion) _rocket;
  GameConsole*     _console;
  GameMenu*        _menu;
  GameInventory*   _inventory;
  StatViewRocket*  _pers;
  UiSave*          ui_save_game;
  UiLoad*          ui_load_game;
};

#endif
