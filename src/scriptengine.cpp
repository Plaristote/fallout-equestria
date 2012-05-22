#include "scriptengine.hpp"
#include <iostream>

using namespace std;
using namespace Script;

asIScriptEngine* Engine::_engine;

void             Engine::Initialize(void)
{
  _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
  if (_engine)
    RegisterStdString_Generic(_engine);
}

void             Engine::Finalize(void)
{
  if (_engine)
    _engine->Release();
}


asIScriptModule* Engine::LoadModule(const std::string& name, const std::string& filepath)
{
  CScriptBuilder builder;

  if ((builder.StartNewModule(_engine, name.c_str())) >= 0)
  {
    if ((builder.AddSectionFromFile(filepath.c_str())) >= 0)
    {
      if ((builder.BuildModule()) >= 0)
        return (_engine->GetModule(name.c_str()));
      else
        cerr << "[ScriptEngine] Can't compile module '" << name << "'" << endl;
    }
    else
      cerr << "[ScriptEngine] Can't load file '" << filepath << "'" << endl;
  }
  else
    cerr << "[ScriptEngine] Can't start Module Loader" << endl;
  return (0);
}