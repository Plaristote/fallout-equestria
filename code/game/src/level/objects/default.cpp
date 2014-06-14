#include "level/objects/default.hpp"

using namespace std;

ObjectDefault::ObjectDefault(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  cout << "Init default with script " << object->script << endl;
  if (object->script != "")
  {
    string prefixPath = "scripts/ai/";
    string scriptPath = prefixPath + object->script + ".as";

    script = new AngelScript::Object(scriptPath);
    skill_target.Initialize(scriptPath, script->GetContext());
    SetupScript(script);
  }
  if (object->name == "control-room-terminal")
    exit(0);
  inventory.LoadInventory(object);
}
