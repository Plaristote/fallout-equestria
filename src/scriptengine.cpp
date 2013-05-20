#include "scriptengine.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace Script;

asIScriptEngine*                        Engine::_engine;
Sync::Signal<void (std::string)> Engine::ScriptError;
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

/*
 * Begin AS OBJECT
 */
#include "as_object.hpp"

AngelScript::Object::Object(const std::string& filepath) : filepath(filepath), module(0)
{
  asIScriptEngine* engine = Script::Engine::Get();
  
  if (engine)
    context = engine->CreateContext();
  else
    ; // throw something
    Initialize();
}

AngelScript::Object::Object(asIScriptContext* context, const std::string& filepath) : filepath(filepath), context(context), module(0)
{
  Initialize();
}

AngelScript::Object::~Object()
{
  if (module)
    Script::ModuleManager::Release(module);
}

void AngelScript::Object::Initialize(void)
{
  if (!context)
    ; // throw something
    if (module)
      Script::ModuleManager::Release(module);
    module = Script::ModuleManager::Require(filepath, filepath);
  std::for_each(functions.begin(), functions.end(), [this](Functions::value_type& item)
  {
    item.second.function = 0;
  });
}

void AngelScript::Object::asDefineMethod(const std::string& name, const std::string& declaration)
{
  Function function;

  function.function  = 0;
  function.signature = declaration;
  //functions.emplace(name, function);
  functions.insert(Functions::value_type(name, function));
}

AngelScript::Object::ReturnType AngelScript::Object::Call(const std::string& name, unsigned int argc, ...)
{
  va_list ap;
  auto    it = functions.find(name);
  
  if (it == functions.end())
    ; // throw something
    if (!(it->second.function))
      it->second.function = module->GetFunctionByDecl(it->second.signature.c_str());
    if (!(it->second.function))
      ; // throw something
      context->Prepare(it->second.function);
    va_start(ap, argc);
  for (unsigned short i = 0 ; argc > i ; ++i)
  {
    IType* param = reinterpret_cast<IType*>(va_arg(ap, void*));
    
    switch (param->Flag())
    {
      case '0':
        context->SetArgObject(i, param->Ptr());
        break ;
      case 'b':
        context->SetArgByte  (i, *((Type<bool>*)(param)));
        break ;
      case 'i':
        context->SetArgWord  (i, *((Type<int>*)(param)));
        break ;
      case 'l':
        context->SetArgDWord (i, *((Type<long>*)(param)));
        break ;
      case 'd':
        context->SetArgDouble(i, *((Type<double>*)(param)));
        break ;
      case 'f':
        context->SetArgFloat (i, *((Type<float>*)(param)));
        break ;
    }
  }
  va_end(ap);
  context->Execute();
  return (ReturnType(context));
}

/*
 * End AS OBJECT
 */
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

void ModuleManager::OutputFunctionList(asIScriptModule* ptr)
{
  unsigned int count = ptr->GetFunctionCount();

  cout << "Function for module " << ptr->GetName() << endl;
  for (unsigned int it = 0 ; it < count ; ++it)
  {
    const string declaration(ptr->GetFunctionByIndex(it)->GetDeclaration());

    cout << "  " << declaration << endl;
  }
  cout << endl;
}
