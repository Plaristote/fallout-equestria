#include "pipbuck/pipbuck.hpp"
#include <gametask.hpp>
#include "pipbuck/clock_app.hpp"
#include "pipbuck/quest_app.hpp"
#include "pipbuck/scripted_app.hpp"

using namespace std;

extern asIScriptContext* as_current_context;
extern asIScriptModule*  as_current_module;

Pipbuck::Pipbuck(WindowFramework* w, Rocket::Core::Context* c, DataEngine& data): UiBase(w, c), _data_engine(data), _object("scripts/ai/general_pony.as")
{
  _root          = c->LoadDocument("data/pipbuck.rml");
  _asked_unfocus = false;
  EventStartApp.EventReceived.Connect   (*this, &Pipbuck::StartApp);
  EventQuitApp.EventReceived.Connect    (*this, &Pipbuck::QuitApp);
  EventHome.EventReceived.Connect       (*this, &Pipbuck::GoHome);
  EventHidePipbuck.EventReceived.Connect(*this, &UiBase::FireHide);

  _object.asDefineMethod("GetAvailableApps", "StringList GetAvailableApps(Data)");

  if (_root)
  {
    ToggleEventListener(true, "button-quit", "click", EventQuitApp);
    ToggleEventListener(true, "button-home", "click", EventHome);
    ToggleEventListener(true, "button-exit", "click", EventHidePipbuck);
  }
  Translate();
}

Pipbuck::~Pipbuck()
{
  if (_root)
  {
    Rocket::Core::Element* element = _root->GetElementById("app_list");

    if (element)
    {
      Rocket::Core::Element* child;
      
      for (unsigned int i = 0 ; ((child = element->GetChild(i)) != 0) ; ++i)
	child->RemoveEventListener("click", &EventStartApp);
    }
    ToggleEventListener(false, "button-quit", "click", EventQuitApp);
    ToggleEventListener(false, "button-home", "click", EventHome);
    ToggleEventListener(false, "button-exit", "click", EventHidePipbuck);    
  }
}

void Pipbuck::StartApp(Rocket::Core::Event& event)
{
  Rocket::Core::Element* button = event.GetCurrentElement();
  Rocket::Core::Variant* var    = button->GetAttribute("data-app");
  
  if (var)
  {
    const string app_name = var->Get<Rocket::Core::String>().CString();
    Data         app_data = _data_engine["Pipbuck"]["Apps"][app_name];
    App*         app      = 0;
    bool         to_start = true;

    // Check if the app is already running
    AppList::iterator it  = _running_apps.begin();
    AppList::iterator end = _running_apps.end();
    for ( ; it != end ; ++it)
    {
      if ((*it)->GetAppId() == app_name)
      {
	app      = *it;
	to_start = false;
	break ;
      }
    }

    // If the app isn't already running, create an instance of it
    if (app_data.NotNil() && to_start)
    {
      unsigned int app_type = app_data["type"];

      switch (app_type)
      {
	case 1:
	  app = new PipbuckClockApp(app_data);
	  break ;
        case 2:
          app = new PipbuckQuestApp(app_data);
          break ;
        default:
        case 0:
          app = new PipbuckAppScript(app_data);
          break ;
      }
    }
    
    // If an app exist now, start it
    if (app)
    {
      if (to_start)
      {
        cout << "Start Application" << endl;
	app->AskFocus.Connect(*this, &Pipbuck::AppAskedFocus);
	app->Exit.Connect    (*this, &Pipbuck::AppAskedExit);
	app->Unfocus.Connect (*this, &Pipbuck::AppAskedUnfocus);
        if (!(app->Started(_data_engine))) // If starting the app failed
	{
          cout << "Failed to start application" << endl;
	  delete app;
	  return ;
	}
        cout << "Application Started Successfully" << endl;
        _running_apps.push_back(app);
      }
      else
        cout << "Application is executing as background task right now" << endl;
      app->Focused(_root->GetElementById("app_container"), _data_engine);
      _running_app = app;
    }
  }
}

void Pipbuck::QuitApp(Rocket::Core::Event&)
{
  if (_running_app)
  {
    DoQuitApp(_running_app);
    _running_app = 0;
    ReloadApps();
  }
}

void Pipbuck::DoQuitApp(App* to_quit)
{
  if (to_quit)
  {
    auto it = find(_running_apps.begin(), _running_apps.end(), _running_app);

    _running_apps.erase(it);
    to_quit->Exited(_data_engine);
  }
}

void Pipbuck::GoHome(Rocket::Core::Event&)
{
  if (_running_app)
  {
    _running_app->Unfocused(_data_engine);
    _running_app = 0;
  }
  ReloadApps();
}

void Pipbuck::AppAskedFocus(App* app)
{
  _asked_focus.push_back(app);
}

void Pipbuck::AppAskedUnfocus(void)
{
  _asked_unfocus = true;
}

void Pipbuck::AppAskedExit(App* app)
{
  _asked_exit.push_back(app);
}

void Pipbuck::Run(void)
{
  Rocket::Core::Element* app_container = (_root ? _root->GetElementById("app_container") : 0);
  AppList::iterator      it, end;

  for_each(_running_apps.begin(), _running_apps.end(), [this, app_container](App* app)
  {
    if (app == _running_app && app_container)
      app->RunAsMainTask(app_container, _data_engine);
    else
      app->RunAsBackgroundTask(_data_engine);
  });

  // Close the apps that asked to be closed
  while ((it = _asked_exit.begin()) != _asked_exit.end())
  {
    App* to_exit = *it;

    DoQuitApp(to_exit); 
    _asked_exit.erase(it);
    if (to_exit == _running_app)
    {
      _running_app = 0;
      ReloadApps();
    }
  }
  
  // Handling if the running app has asked to be unfocused
  if (_asked_unfocus)
  {
    _running_app->Unfocused(_data_engine);
    _running_app   = 0;
    _asked_unfocus = false;
    ReloadApps();
  }
  
  // Giving focus to the app who asked for it first
  if (_asked_focus.size() > 0)
  {
    if (_running_app)
      _running_app->Unfocused(_data_engine);
    _running_app = _asked_focus.front();
    _running_app->Focused(app_container, _data_engine);
    _asked_focus.clear();
  }
}

void Pipbuck::Restart(void)
{
  for_each(_running_apps.begin(), _running_apps.end(), [](App* app) { delete app; });
  _running_apps.clear();
  _running_app = 0;
  ReloadApps();
}

void Pipbuck::ReloadApps(void)
{
  if (!_root) return ;
  Rocket::Core::Element* elem      = _root->GetElementById("app_container");
  Data                   apps_data = _data_engine["Pipbuck"]["Apps"];
  list<string>           apps;
  stringstream           rml;

  // Get App List from scripts
  if (_object.IsDefined("GetAvailableApps"))
  {
    AngelScript::Type<Data*> param(&_data_engine);

    apps = *(list<string>*)(_object.Call("GetAvailableApps", 1, &param));
  }
  else
    cout << "pipbuck.as: missing function StringList GetAvailableApps(Data)" << endl;

  unsigned int iterator = 0;
  rml << "<div id='app_list'>";
  if (apps.empty())
    rml << i18n::T("You don't have any application right now");
  cout << "[Pipbuck] Adding applications" << endl;
  for_each(apps.begin(), apps.end(), [&rml, &iterator, apps_data](const string& app)
  {
    Data app_data = apps_data[app];

    cout << "-> Adding application " << app << endl;
    rml << "<button class='app_button' id='app_button_" << iterator << "' data-app='" << app << "'>";
    rml << "<img class='app_icon' src='../textures/pipbuck/" << app_data["icon"].Value() << "' />";
    rml << "</button>";
    ++iterator;
  });
  rml << "</div>";
  if (elem)
    elem->SetInnerRML(rml.str().c_str());
  for (unsigned int i = 0 ; i < iterator ; ++i)
  {
    stringstream id;

    id << "app_button_" << i;
    ToggleEventListener(true, id.str(), "click", EventStartApp);
  }
}

