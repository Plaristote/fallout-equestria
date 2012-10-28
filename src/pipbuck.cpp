#include "pipbuck.hpp"

using namespace std;

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
	return ;
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
      }
    }
    
    // If an app exist now, start it
    if (app)
    {
      if (to_start)
      {
	app->AskFocus.Connect(*this, &Pipbuck::AppAskedFocus);
	app->Exit.Connect    (*this, &Pipbuck::AppAskedExit);
	app->Unfocus.Connect (*this, &Pipbuck::AppAskedUnfocus);
        if (!(app->Started(_data_engine))) // If starting the app failed
	{
	  delete app;
	  return ;
	}
        _running_apps.push_back(app);
      }
      app->Focused(_root->GetElementById("app_container"), _data_engine);
      _running_app = app;
    }
  }
}

void Pipbuck::QuitApp(Rocket::Core::Event&)
{
  if (_running_app)
  {
    _running_app->Exited(_data_engine);
    _running_app = 0;
    ReloadApps();
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
    
    to_exit->Exited(_data_engine);
    delete to_exit;
    if (to_exit == _running_app)
    {
      _running_app = 0;
      ReloadApps();
    }
    _asked_exit.erase(it);
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
    context->Release();
  }
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
    _context->Prepare(function);
    _context->SetArgObject(0, &de);
    _context->Execute();
  }
}

void PipbuckAppScript::Focused(Rocket::Core::Element* root, DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["focused"].Value() + "(RmlElement, Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  
  if (function)
  {
    _context->Prepare(function);
    _context->SetArgObject(0, root);
    _context->SetArgObject(1, &de);
    _context->Execute();
  }
}

void PipbuckAppScript::Unfocused(DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["unfocused"].Value() + "(Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  
  if (function)
  {
    _context->Prepare(function);
    _context->SetArgObject(0, &de);
    _context->Execute();
  }
}

void PipbuckAppScript::RunAsBackgroundTask(DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["run_background"].Value() + "(Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  int                ret_value     = 0;
  
  if (function)
  {
    _context->Prepare(function);
    _context->SetArgObject(1, &de);
    _context->Execute();
    ret_value = _context->GetReturnDWord();
  }
  switch (ret_value)
  {
    default:
    case 0:
      Exit.Emit(this);
      break ;
    case 1:
      AskFocus.Emit(this);
      break ;
  }
}

void PipbuckAppScript::RunAsMainTask(Rocket::Core::Element* root, DataEngine& de)
{
  const string       function_name = "void " + _data["hooks"]["run_focused"].Value() + "(RmlElement, Data)";
  asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
  int                ret_value     = 0;
  
  if (function)
  {
    _context->Prepare(function);
    _context->SetArgObject(0, root);
    _context->SetArgObject(1, &de);
    _context->Execute();
    ret_value = _context->GetReturnDWord();
  }
  switch (ret_value)
  {
    default:
    case 0:
      Exit.Emit(this);
      break ;
    case 1:
      Unfocus.Emit();
      break ;
  }
}

bool PipbuckAppScript::Started(DataEngine& de)
{
  bool               ret_value     = false;

  if (_context && _module)
  {
    const string       function_name = "void " + _data["hooks"]["start"].Value() + "(Data)";
    asIScriptFunction* function      = _module->GetFunctionByDecl(function_name.c_str());
    
    if (function)
    {
      _context->Prepare(function);
      _context->SetArgObject(0, &de);
      _context->Execute();
      ret_value = _context->GetReturnByte();
    }
  }
  return (false);
}