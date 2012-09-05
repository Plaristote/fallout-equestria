#ifndef  MAINMENU_HPP
# define MAINMENU_HPP

# include "gametask.hpp"

class MainMenu : public AsyncTask
{
  struct View : public UiBase
  {
    View(WindowFramework* window, Rocket::Core::Context* context);

    Observatory::Signal<void (Rocket::Core::Event&)> NewGame;
    Observatory::Signal<void (Rocket::Core::Event&)> Quit;
    Observatory::Signal<void (Rocket::Core::Event&)> LoadGame;
    Observatory::Signal<void (Rocket::Core::Event&)> Options;
    
  private:
    RocketListener NewGameClicked, QuitClicked, LoadGameClicked, OptionsClicked;
  };
  
public:
  MainMenu(WindowFramework* window);

  void NewGame(Rocket::Core::Event&);  
  void EndGame(void);
  DoneStatus do_task(void);
  
private:
  WindowFramework* _window;
  GeneralUi        _generalUi;
  LevelTask*       _levelTask;
  View             _view;
};


#endif