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

    Observatory::Signal<void (App*)> Exit;
    Observatory::Signal<void>        Unfocus;
    Observatory::Signal<void (App*)> AskFocus;
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

#endif
