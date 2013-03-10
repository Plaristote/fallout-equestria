#ifndef  PIPBUCK_HPP
# define PIPBUCK_HPP

# include "rocket_extension.hpp"
# include "scriptengine.hpp"
# include "dataengine.hpp"

class Pipbuck : public UiBase
{
public:
  struct App
  {
    virtual ~App(void) {}
    virtual const std::string GetAppId(void) const                  = 0;
    
    virtual void RunAsMainTask(Rocket::Core::Element*, DataEngine&) = 0;
    virtual void RunAsBackgroundTask(DataEngine&)                   = 0;

    virtual bool Started(DataEngine&)                               = 0;
    virtual void Exited(DataEngine&)                                = 0;
    virtual void Unfocused(DataEngine&)                             = 0;
    virtual void Focused(Rocket::Core::Element*, DataEngine&)       = 0;

    Sync::Signal<void (App*)> Exit;
    Sync::Signal<void>        Unfocus;
    Sync::Signal<void (App*)> AskFocus;
  };
  
  Pipbuck(WindowFramework*, Rocket::Core::Context*, DataEngine&);
  ~Pipbuck(void);

  void Restart(void);
  void Run(void);

private:
  void ReloadApps(void);

  typedef std::list<App*> AppList;
  
  DataEngine&       _data_engine;
  asIScriptContext* _context;
  asIScriptModule*  _module;
  AppList           _running_apps;
  App*              _running_app;
  bool              _asked_unfocus;
  AppList           _asked_focus;
  AppList           _asked_exit;

  RocketListener    EventStartApp, EventQuitApp, EventHome, EventHidePipbuck;
  void              StartApp(Rocket::Core::Event&);
  void              QuitApp(Rocket::Core::Event&);
  void              GoHome(Rocket::Core::Event&);
  void              DoQuitApp(App*);
  
  void              AppAskedFocus(App*);
  void              AppAskedUnfocus(void);
  void              AppAskedExit(App*);
};

class PipbuckAppScript : public Pipbuck::App
{
public:
  PipbuckAppScript(Data script);
  ~PipbuckAppScript() {}
  
  const std::string GetAppId(void) const { return (_data.Key()); }

  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&);
  
  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);

private:
  asIScriptContext* _context;
  asIScriptModule*  _module;
  Data              _data;
};

# include "timer.hpp"

class PipbuckClockApp : public Pipbuck::App
{
public:
  PipbuckClockApp(Data script);
  ~PipbuckClockApp() {}
  
  const std::string GetAppId(void) const { return (_appid); }
  
  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&) {}

  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);
private:
  const std::string _appid;
  TimeManager&      _time_manager;
  std::string       _inner_rml;
  
  RocketListener        EventWaitMore, EventWaitLess, EventTimeUnitSelected, EventWait;
  
  void                  WaitChange(int chance);
  void                  Wait(Rocket::Core::Event&);
  void                  SelectTimeUnit(Rocket::Core::Event&);
  
  void                  SetValue(Rocket::Core::Element* element, unsigned short value);

  Rocket::Core::Element *_year, *_month, *_day, *_hour, *_minute, *_second;
  Rocket::Core::Element *_wait_days, *_wait_hours, *_wait_minutes;
  Rocket::Core::Element *_selected_wait;
  Rocket::Core::Element *_wait_more, *_wait_less, *_do_wait;
  Rocket::Core::Element *_span_error;
  
  unsigned int          _minutes_to_spend;
};

class PipbuckQuestApp : public Pipbuck::App
{
public:
  PipbuckQuestApp(Data script);
  ~PipbuckQuestApp() {}

  const std::string GetAppId(void) const { return (_appid); }
  
  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&) {}

  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);
private:
  const std::string _appid;
  std::string       _inner_rml;
  DataEngine*       _data_engine;
};

#endif
