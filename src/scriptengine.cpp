#include "scriptengine.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace Script;

asIScriptEngine*                        Engine::_engine;
Observatory::Signal<void (std::string)> Engine::ScriptError;
ModuleManager::Modules                  ModuleManager::_modules;

void Script::Call(asIScriptContext* context, asIScriptFunction* function, const std::string fmt, ...)
{
  va_list ap;

  context->Prepare(function);
  va_start(ap, fmt);
  for (unsigned short i = 0 ; fmt[i] ; ++i)
  {
    if (i == 'O')
      context->SetArgObject(i, va_arg(ap, void*));
    else if (i == 'b')
      context->SetArgByte(i, va_arg(ap, int));
    else if (i == 'i')
      context->SetArgDWord(i, va_arg(ap, int));
    else if (i == 'f')
      context->SetArgFloat(i, va_arg(ap, double));
  }
  va_end(ap);
  context->Execute();
}

void             Engine::Initialize(void)
{
  _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
  if (_engine)
  {
    _engine->SetMessageCallback(asFUNCTION(Engine::MessageCallback), 0, asCALL_CDECL);    
    RegisterStdString(_engine);
    //RegisterStdString_Generic(_engine);
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

  stream << "<span class='console-as-error'>[AngelScript][" << errorType << "]</span> " << msg->section << " (" << msg->row << ", " << msg->col << ")";
  stream << " " << msg->message;

  ScriptError.Emit(stream.str());
}

asIScriptModule* ModuleManager::Require(const std::string& name, const std::string& filepath)
{
  Modules::iterator existing = std::find(_modules.begin(), _modules.end(), filepath);
  
  if (existing == _modules.end())
  {
    LoadedModule module;
    
    module.users    = 1;
    module.filepath = filepath;
    module.ptr      = Engine::LoadModule(name, filepath);
    if (module.ptr == 0)
      return (0);
    _modules.push_back(module);
    return (module.ptr);
  }
  existing->users += 1;
  return (existing->ptr);
}

void             ModuleManager::Release(asIScriptModule* ptr)
{
  Modules::iterator existing = std::find(_modules.begin(), _modules.end(), ptr);
  
  if (existing != _modules.end())
  {
    existing->users -= 1;
    if (existing->users == 0)
    {
      Engine::Get()->DiscardModule(existing->ptr->GetName());
      _modules.erase(existing);
    }
  }
}
