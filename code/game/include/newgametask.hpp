#ifndef  NEWGAMETASK_HPP
# define NEWGAMETASK_HPP

# include "rocket_extension.hpp"
# include "options.hpp"
# include "cmap/statcontroller.hpp"
# include "ui/stat_view_rocket.hpp"

class UiNewGame : public UiBase
{
public:
  UiNewGame(WindowFramework*, Rocket::Core::Context*);
  ~UiNewGame(void);

  Sync::Signal<void>                      StartFromScratch;
  Sync::Signal<void (const std::string&)> SelectProfile;
  Sync::Signal<void>                      Cancel;

private:
  RocketListener PreviousProfile, NextProfile, EventSelectProfile, EventStartFromScratch, EventCancel;
  DataTree*      _data_profiles;
  
  void SelectedProfile(Rocket::Core::Event&);
  void ClickedStartFromScratch(Rocket::Core::Event&);
  void GoToPreviousProfile(Rocket::Core::Event&);
  void GoToNextProfile(Rocket::Core::Event&);
  void GoToProfile(const std::string&);
  void GoCancel(Rocket::Core::Event&) { Cancel.Emit(); }

  std::vector<std::string>                 _profiles;
  std::vector<std::string>::const_iterator _current_profile;
};

class NewGameTask
{
public:
  NewGameTask(WindowFramework*, Rocket::Core::Context*);
  ~NewGameTask();

  Sync::Signal<void> StartGame;
  Sync::Signal<void> Cancel;
  
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
