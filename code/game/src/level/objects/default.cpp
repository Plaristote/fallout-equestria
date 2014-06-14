#include "level/objects/default.hpp"

using namespace std;

ObjectDefault::ObjectDefault(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  _type = ObjectType2Code<ObjectDefault>::Type;
  if (object->script != "")
  {
    string prefixPath = "scripts/ai/";
    string scriptPath = prefixPath + object->script + ".as";

    script = new AngelScript::Object(scriptPath);
    skill_target.Initialize(scriptPath, script->GetContext());
    SetupScript(script);
    cout << "This: " << this << endl;
    TryToStartConversation(0);
  }
  inventory.LoadInventory(object);
}
