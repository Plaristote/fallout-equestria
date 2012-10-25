#ifndef  NEWGAMETASK_HPP
# define NEWGAMETASK_HPP

# include "rocket_extension.hpp"
# include "options.hpp"
# include "statsheet.hpp"

class UiNewGame : public UiBase
{
public:
  UiNewGame(WindowFramework*, Rocket::Core::Context*);
  ~UiNewGame(void);

  Observatory::Signal<void>                      StartFromScratch;
  Observatory::Signal<void (const std::string&)> SelectProfile;
  
private:
  RocketListener PreviousProfile, NextProfile, EventSelectProfile, EventStartFromScratch;
  DataTree*      _data_profiles;
  
  void SelectedProfile(Rocket::Core::Event&);
  void ClickedStartFromScratch(Rocket::Core::Event&);
  void GoToPreviousProfile(Rocket::Core::Event&);
  void GoToNextProfile(Rocket::Core::Event&);
  void GoToProfile(const std::string&);

  std::vector<std::string>                 _profiles;
  std::vector<std::string>::const_iterator _current_profile;
};

class NewGameTask
{
public:
  NewGameTask(WindowFramework*, Rocket::Core::Context*);
  ~NewGameTask();

  Observatory::Signal<void> StartGame;
  
private:
  void                      SelectProfile(const std::string&);
  void                      StartFromScratch(void);  
  void                      Done(void);

  WindowFramework*          _window;
  Rocket::Core::Context*    _rocket;
  UiNewGame                 _ui_new_game;
  StatViewRocket*           _stat_view;
  StatController*           _stat_controller;
  DataTree*                 _stat_sheet;
};

#endif