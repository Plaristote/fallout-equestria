#include "pipbuck.hpp"

using namespace std;

Pipbuck::Pipbuck(WindowFramework* w, Rocket::Core::Context* c, DataEngine& data): UiBase(w, c), _data_engine(data)
{
  _module  = Script::ModuleManager::Require("pipbuck", "scripts/ai/pipbuck.as");
}

void Pipbuck::ReloadApps(void)
{
  Rocket::Core::Element* elem      = _root->GetElementById("app_list");
  Data                   apps_data = _data_engine["Pipbuck"]["Apps"];
  list<string>           apps;
  stringstream           rml;

  // Get App List from scripts
  if (_module)
  {
    asIScriptContext*  context = Script::Engine::Get()->CreateContext();
    asIScriptFunction* func    = _module->GetFunctionByDecl("StringList GetAvailableApps(Data);");

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
  for_each(apps.begin(), apps.end(), [&rml, apps_data](const string& app)
  {
    Data app_data = apps_data[app];

    rml << "<button class='app_button' id='" << app_data["id"].Value() << "'>";
    rml << "<img class='app_icon' src='build/textures/" << app_data["icon"].Value() << "' />";
    rml << "</button>";
  });
  if (elem)
    elem->SetInnerRML(rml.str().c_str());
}