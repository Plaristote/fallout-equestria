#include "pipbuck.hpp"
#include <gametask.hpp>

using namespace std;

extern asIScriptContext* as_current_context;
extern asIScriptModule*  as_current_module;

Pipbuck::Pipbuck(WindowFramework* w, Rocket::Core::Context* c, DataEngine& data): UiBase(w, c), _data_engine(data)
{
  _root          = c->LoadDocument("data/pipbuck.rml");
  _asked_unfocus = false;
  _module        = Script::ModuleManager::Require("pipbuck", "scripts/ai/general_pony.as");
  EventStartApp.EventReceived.Connect   (*this, &Pipbuck::StartApp);
  EventQuitApp.EventReceived.Connect    (*this, &Pipbuck::QuitApp);
  EventHome.EventReceived.Connect       (*this, &Pipbuck::GoHome);
  EventHidePipbuck.EventReceived.Connect(*this, &UiBase::FireHide);

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
      switch ((int)app_data["type"])
      {
	default:
	case 0:
	  app = new PipbuckAppScript(app_data);
	  break ;
	case 1:
	  app = new PipbuckClockApp(app_data);
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
  if (_module)
  {
    asIScriptContext*  context = Script::Engine::Get()->CreateContext();
    asIScriptFunction* func    = _module->GetFunctionByDecl("StringList GetAvailableApps(Data)");

    if (func)
    {
      context->Prepare(func);
      context->SetArgObject(0, &_data_engine);
      context->Execute();
      apps = *(reinterpret_cast<list<string>*>(context->GetReturnObject()));
      func->Release();
    }
    else
      cout << "pipbuck.as: missing function StringList GetAvailableApps(Data)" << endl;
    //context->Release();
  }
  else
    cout << "pipbuck.as: isn't loaded" << endl;
  unsigned int iterator = 0;
  rml << "<div id='app_list'>";
  if (apps.size() == 0)
    rml << i18n::T("You don't have any application right now");
  for_each(apps.begin(), apps.end(), [&rml, &iterator, apps_data](const string& app)
  {
    Data app_data = apps_data[app];

    rml << "<button class='app_button' id='app_button_" << iterator << "' data-app='" << app << "'>";
    rml << "<img class='app_icon' src='../textures/" << app_data["icon"].Value() << "' />";
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

/*
 * 2. PipbuckClockApp, for in-game time management
 */
PipbuckClockApp::PipbuckClockApp(Data script) : _appid(script.Key()), _time_manager(GameTask::CurrentGameTask->GetTimeManager())
{
  EventWait.EventReceived.Connect            (*this, &PipbuckClockApp::Wait);
  EventWaitMore.EventReceived.Connect        ([this](Rocket::Core::Event&) { WaitChange(1);  });
  EventWaitLess.EventReceived.Connect        ([this](Rocket::Core::Event&) { WaitChange(-1); });
  EventTimeUnitSelected.EventReceived.Connect(*this, &PipbuckClockApp::SelectTimeUnit);
}

bool PipbuckClockApp::Started(DataEngine& de)
{
  _year = _month = _day = _hour = _minute = _second = 0;
  return (Filesystem::FileContent("data/pipbuck_clock.rml", _inner_rml));
}

void PipbuckClockApp::Focused(Rocket::Core::Element* root, DataEngine& de)
{
  _minutes_to_spend = 0;
  
  root->SetInnerRML(_inner_rml.c_str());
  
  _span_error = root->GetElementById("span-error");

  _year    = root->GetElementById("value-year");
  _month   = root->GetElementById("value-month");
  _day     = root->GetElementById("value-day");
  _hour    = root->GetElementById("value-hour");
  _minute  = root->GetElementById("value-minute");
  _second  = root->GetElementById("value-second");

  _wait_days    = root->GetElementById("value-wait-days");
  _wait_hours   = root->GetElementById("value-wait-hours");
  _wait_minutes = root->GetElementById("value-wait-minutes");
  
  _wait_more    = root->GetElementById("wait-more");
  _wait_less    = root->GetElementById("wait-less");
  _do_wait      = root->GetElementById("button-wait");
  
  if (_wait_days && _wait_hours && _wait_minutes)
  {
    _wait_days->AddEventListener   ("click", &EventTimeUnitSelected);
    _wait_hours->AddEventListener  ("click", &EventTimeUnitSelected);
    _wait_minutes->AddEventListener("click", &EventTimeUnitSelected);
    _selected_wait = 0;
  }
  if (_wait_more && _wait_less && _do_wait)
  {
    _wait_more->AddEventListener("click", &EventWaitMore);
    _wait_less->AddEventListener("click", &EventWaitLess);
    _do_wait->AddEventListener  ("click", &EventWait);
  }
}

void PipbuckClockApp::Exited(DataEngine& de)
{
  Unfocused(de);
}

void PipbuckClockApp::Unfocused(DataEngine& de)
{
  if (_wait_days && _wait_hours && _wait_minutes)
  {
    _wait_days->RemoveEventListener   ("click", &EventTimeUnitSelected);
    _wait_hours->RemoveEventListener  ("click", &EventTimeUnitSelected);
    _wait_minutes->RemoveEventListener("click", &EventTimeUnitSelected);
    _selected_wait = 0;
  }
  if (_wait_more && _wait_less && _do_wait)
  {
    _wait_more->RemoveEventListener("click", &EventWaitMore);
    _wait_less->RemoveEventListener("click", &EventWaitLess);
    _do_wait->RemoveEventListener  ("click", &EventWait);
  }
  _year = _month = _day = _hour = _minute = _second = _span_error = 0;  
}

void PipbuckClockApp::SelectTimeUnit(Rocket::Core::Event& event)
{
  Rocket::Core::Element* new_unit = event.GetCurrentElement();

  if (_selected_wait)
    _selected_wait->SetProperty("color", "white");
  _selected_wait = new_unit;
  _selected_wait->SetProperty("color", "yellow");
}

void PipbuckClockApp::WaitChange(int change)
{
  if (_selected_wait)
  {
    Rocket::Core::Variant* var = _selected_wait->GetAttribute("data-value");
    
    if (var)
    {
      int          value = var->Get<int>();
      stringstream strvalue;

      value += change;
      if (value >= 0)
      {
	strvalue << value;
	_selected_wait->SetAttribute<int>("data-value", value);
	_selected_wait->SetInnerRML(strvalue.str().c_str());
      }
    }
  }
}

void PipbuckClockApp::Wait(Rocket::Core::Event&)
{
  Rocket::Core::Variant* var_days    = _wait_days->GetAttribute("data-value");
  Rocket::Core::Variant* var_hours   = _wait_hours->GetAttribute("data-value");
  Rocket::Core::Variant* var_minutes = _wait_minutes->GetAttribute("data-value");
  
  if (var_days && var_hours && var_minutes)
  {
    int ndays    = var_days->Get<int>();
    int nhours   = var_hours->Get<int>();
    int nminutes = var_minutes->Get<int>();
    
    _minutes_to_spend = ndays * 1440 + nhours * 60 + nminutes;
  }
}

void PipbuckClockApp::SetValue(Rocket::Core::Element* element, unsigned short value)
{
  stringstream strval;
  
  strval << value;
  element->SetInnerRML(strval.str().c_str());
}

void PipbuckClockApp::RunAsMainTask(Rocket::Core::Element* root, DataEngine& de)
{
  if (_year)   SetValue(_year,   _time_manager.GetYear());
  if (_month)  SetValue(_month,  _time_manager.GetMonth());
  if (_day)    SetValue(_day,    _time_manager.GetDay());
  if (_hour)   SetValue(_hour,   _time_manager.GetHour());
  if (_minute) SetValue(_minute, _time_manager.GetMinute());
  if (_second) SetValue(_second, _time_manager.GetSecond());
  if (_minutes_to_spend > 0)
  {
    Level::CurrentLevel->SetState(Level::Interrupted);
    if (Level::CurrentLevel != 0 && Level::CurrentLevel->GetState() == Level::Fight)
     {
       _span_error->SetInnerRML(i18n::T("You can't wait while fighting").c_str());
       _minutes_to_spend = 0;
       return ;
     }
     _span_error->SetInnerRML("");
     if (_minutes_to_spend > 60)
     {
       _time_manager.AddElapsedTime(0, 0, 1);
       _minutes_to_spend -= 60;
     }
     else
     {
       _time_manager.AddElapsedTime(0, 1);
       _minutes_to_spend--;
     }
     if (_minutes_to_spend == 0)
       Level::CurrentLevel->SetState(Level::Normal);
  }
}

/*
 * Pipbuck::App implementations
 * 1. PipbuckAppScript, for applications using only scripts
 */
PipbuckAppScript::PipbuckAppScript(Data script)
{
  _data.Duplicate(script);
  _context = Script::Engine::Get()->CreateContext();
  _module  = Script::ModuleManager::Require(_data["src"].Value(), "scripts/pipbuck/" + _data["src"].Value());
}

void PipbuckAppScript::Exited(DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["exit"].Value() + "(Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  
  if (function)
  {
    as_current_context = _context;
    as_current_module  = _module;
     _context->Prepare(function);
    _context->SetArgObject(0, &de);
    _context->Execute();
  }
}

void PipbuckAppScript::Focused(Rocket::Core::Element* root, DataEngine& de)
{
  cout << "Application Focused" << endl;
  const string       function_name = "void " + _data["hooks"]["focused"].Value() + "(RmlElement@, Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  
  if (function)
  {
    as_current_context = _context;
    as_current_module  = _module;
    _context->Prepare(function);
    _context->SetArgObject(0, root);
    _context->SetArgObject(1, &de);
    _context->Execute();
  }
  else
    cout << "App missing focused hook (prototype is '" << function_name << "')" << endl;
}

void PipbuckAppScript::Unfocused(DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["unfocused"].Value() + "(Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  
  if (function)
  {
    as_current_context = _context;
    as_current_module  = _module;
    _context->Prepare(function);
    _context->SetArgObject(0, &de);
    _context->Execute();
  }
}

void PipbuckAppScript::RunAsBackgroundTask(DataEngine& de)
{
  const string       function_name = "int " + _data["hooks"]["run_background"].Value() + "(Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  int                ret_value     = 0;
  
  if (function)
  {
    as_current_context = _context;
    as_current_module  = _module;
    _context->Prepare(function);
    _context->SetArgObject(0, &de);
    _context->Execute();
    ret_value = _context->GetReturnDWord();
  }
  switch (ret_value)
  {
    case 0:
      break ;
    default:
    case 1:
      Exit.Emit(this);
      break ;
    case 2:
      AskFocus.Emit(this);
      break ;
  }
}

void PipbuckAppScript::RunAsMainTask(Rocket::Core::Element* root, DataEngine& de)
{
  const string       function_name = "int " + _data["hooks"]["run_focused"].Value() + "(RmlElement@, Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  int                ret_value     = 0;
  
  if (function)
  {
    as_current_context = _context;
    as_current_module  = _module;
    _context->Prepare(function);
    _context->SetArgObject(0, root);
    _context->SetArgObject(1, &de);
    _context->Execute();
    ret_value = _context->GetReturnDWord();
  }
  switch (ret_value)
  {
    case 0:
      break ;
    default:
    case 1:
      Exit.Emit(this);
      break ;
    case 2:
      Unfocus.Emit();
      break ;
  }
}

bool PipbuckAppScript::Started(DataEngine& de)
{
  bool               ret_value     = false;

  if (_context && _module)
  {
    const string       function_name = "bool " + _data["hooks"]["start"].Value() + "(Data)";
    asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());

    if (function)
    {
      as_current_context = _context;
      as_current_module  = _module;
      _context->Prepare(function);
      _context->SetArgObject(0, &de);
      _context->Execute();
      ret_value = _context->GetReturnByte();
    }
    else
      cout << "App missing start hook (prototype is configured to be: '" << function_name << "'" << endl;
  }
  return (ret_value);
}
