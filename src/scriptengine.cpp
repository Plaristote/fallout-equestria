#include "scriptengine.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace Script;

asIScriptEngine*                        Engine::_engine;
Observatory::Signal<void (std::string)> Engine::ScriptError;

void             Engine::Initialize(void)
{
  _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
  if (_engine)
  {
    _engine->SetMessageCallback(asFUNCTION(Engine::MessageCallback), 0, asCALL_CDECL);    
    RegisterStdString_Generic(_engine);
  }
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

void Engine::MessageCallback(const asSMessageInfo* msg, void* param)
{
  stringstream stream;
  string       errorType("Error");

  if (msg->type == asMSGTYPE_WARNING)
    errorType = "Warning";
  else if (msg->type == asMSGTYPE_INFORMATION)
    errorType = "Information";

  stream << "[AngelScript][" << errorType << "] " << msg->section << " (" << msg->row << ", " << msg->col << ")";
  stream << " " << msg->message;

  ScriptError.Emit(stream.str());
}