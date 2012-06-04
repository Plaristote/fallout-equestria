#include "npc.hpp"

using namespace std;

/*ObjectNode* Npc::Factory(WindowFramework* window, Tilemap& tilemap, Characters& chars, Data data)
{
  Npc* npc = new Npc(window, tilemap, data, chars);

  chars.push_back(npc);
  return (npc);
}

Npc::Npc(WindowFramework* w, Tilemap& t, Data data, Characters& c) : Character(w, t, data, c)
{
  _scriptContext = 0;
  _scriptModule  = 0;
  _scriptMain    = 0;
  if (!(data["script"].Nil()))
  {
    string prefixName = "IA_";
    string prefixPath = "scripts/";

    _scriptContext = Script::Engine::Get()->CreateContext();
    _scriptModule  = Script::Engine::LoadModule(prefixName + GetName(), prefixPath + data["script"].Value());
    if (_scriptModule)
      _scriptMain    = _scriptModule->GetFunctionByDecl("void main(Character@, float)");
  }
}

Npc::~Npc()
{
  if (_scriptContext)
    _scriptContext->Release();
}

void Npc::Run(float elapsedTime)
{
  if (_scriptMain)
  {
    _scriptContext->Prepare(_scriptMain);
    _scriptContext->SetArgObject(0, this);
    _scriptContext->SetArgFloat(1,  elapsedTime);
    if ((_scriptContext->Execute()) != asEXECUTION_FINISHED)
    {
      cout << "IA Execution failed" << endl;
    }
  }
  Character::Run(elapsedTime);
}*/
