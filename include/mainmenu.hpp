#ifndef  MAINMENU_HPP
# define MAINMENU_HPP

# include "gametask.hpp"
# include "mousecursor.hpp"
#include "newgametask.hpp"

class MainMenu : public AsyncTask
{
  struct View : public UiBase
  {
    View(WindowFramework* window, Rocket::Core::Context* context);

    Sync::Signal<void (Rocket::Core::Event&)> Continue;
    Sync::Signal<void (Rocket::Core::Event&)> NewGame;
    Sync::Signal<void (Rocket::Core::Event&)> Quit;
    Sync::Signal<void (Rocket::Core::Event&)> LoadGame;
    Sync::Signal<void (Rocket::Core::Event&)> Options;
    
  private:
    RocketListener NewGameClicked, QuitClicked, LoadGameClicked, OptionsClicked, ContinueClicked;
  };
  
  typedef std::list<AlertUi*> Alerts;
  
public:
  MainMenu(WindowFramework* window);
  ~MainMenu();

  void       Continue(Rocket::Core::Event&);  
  void       EndGame(void);
  void       NewGame(Rocket::Core::Event&);
  void       StartGame(void);
  void       CancelNewGame(void);
  DoneStatus do_task(void);

private:  
  WindowFramework* _window;
  GeneralUi        _generalUi;
  MouseCursor      _mouseCursor;
  NewGameTask*     _new_game_task;
  GameTask*        _levelTask;
  View             _view;
  Alerts           _alerts;

  UiLoad*          _uiLoad;
  
  void             AsyncCreateLevel(void);
  
  void             QuitGame(Rocket::Core::Event&);
  void             OpenUiLoad(Rocket::Core::Event&);
  void             LoadSlot(unsigned char slot);

  void             DisplayAlerts(void);  
  
  bool             _need_garbage_collect;
  bool             quitGamePlz;
  bool             createLevelPlz;
  short            slotToLoadPlz;
};


#endif
