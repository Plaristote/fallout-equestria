#ifndef  MAINMENU_HPP
# define MAINMENU_HPP

# include "gametask.hpp"
# include "mousecursor.hpp"

class MainMenu : public AsyncTask
{
  struct View : public UiBase
  {
    View(WindowFramework* window, Rocket::Core::Context* context);

    Observatory::Signal<void (Rocket::Core::Event&)> Continue;
    Observatory::Signal<void (Rocket::Core::Event&)> NewGame;
    Observatory::Signal<void (Rocket::Core::Event&)> Quit;
    Observatory::Signal<void (Rocket::Core::Event&)> LoadGame;
    Observatory::Signal<void (Rocket::Core::Event&)> Options;
    
  private:
    RocketListener NewGameClicked, QuitClicked, LoadGameClicked, OptionsClicked, ContinueClicked;
  };
  
public:
  MainMenu(WindowFramework* window);

  void       Continue(Rocket::Core::Event&);  
  void       EndGame(void);
  DoneStatus do_task(void);
  
private:
  WindowFramework* _window;
  GeneralUi        _generalUi;
  MouseCursor      _mouseCursor;
  LevelTask*       _levelTask;
  View             _view;

  UiLoad*          _uiLoad;
  
  void             AsyncCreateLevel(void);
  
  void             QuitGame(Rocket::Core::Event&);
  void             OpenUiLoad(Rocket::Core::Event&);
  void             LoadSlot(unsigned char slot);
  
  bool             quitGamePlz;
  bool             createLevelPlz;
  short            slotToLoadPlz;
};


#endif