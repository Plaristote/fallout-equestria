#include "datatree.hpp"
#include "scriptengine.hpp"

int compile_statsheet(std::string name)
{
  DataTree* statsheet = DataTree::Factory::JSON("data/charsheets/" + name + ".json");

  if (statsheet)
  {
    Data               data(statsheet);
    asIScriptEngine*   engine  = Script::Engine::Get();
    asIScriptContext*  context = engine->CreateContext();
    asIScriptModule*   module  = Script::ModuleManager::Require("special", "scripts/ai/special.as");
    asIScriptFunction* update_all;

    update_all = module->GetFunctionByDecl("void UpdateAllValues(Data)");
    if (!update_all)
      return (-2);
    context->Prepare(update_all);
    context->SetArgObject(0, &data);
    context->Execute();
    DataTree::Writers::JSON(data, "data/charsheets/" + name + ".json");
    return (0);
  }
  return (-1);
}
