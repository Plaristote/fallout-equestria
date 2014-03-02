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
#include "directory.hpp"

asIScriptContext*                   AngelScript::ContextLock::current_context = 0;
asIScriptModule*                    AngelScript::ContextLock::current_module  = 0;
AngelScript::ContextLock::ObjectPtr AngelScript::ContextLock::current_object;

AngelScript::Object::Object(const std::string& filepath) : filepath(filepath), module(0)
{
  asIScriptEngine* engine = Script::Engine::Get();

  required_module  = false;
  required_context = true;
  if (engine)
    context = engine->CreateContext();
  else
  {
    context = 0;
    std::cerr << "[AngelScript] Could not create context." << std::endl;
  }
  Initialize();
}

AngelScript::Object::Object(asIScriptContext* context, const std::string& filepath) : filepath(filepath), context(context), module(0)
{
  required_module = required_context = false;
  Initialize();
}

AngelScript::Object::Object(asIScriptContext* context, asIScriptModule* module) : filepath(), context(context), module(module)
{
  required_module = required_context = false;
}

AngelScript::Object::~Object()
{
  ObjectDestroyed.Emit();
  if (required_module == true)
    Script::ModuleManager::Release(module);
  if (required_context == true && context != 0)
    context->Release();
}

void AngelScript::Object::Initialize(void)
{
  if (!context)
    ; // throw something
  if (module)
    Script::ModuleManager::Release(module);
  if (filepath[filepath.size() - 1] != '/' && Filesystem::FileExists(filepath))
  {
    module          = Script::ModuleManager::Require(filepath, filepath);
    required_module = true;
  }
  std::for_each(functions.begin(), functions.end(), [this](Functions::value_type& item)
  {
    item.second.function = 0;
  });
}

void AngelScript::Object::asUndefineMethod(const std::string& name)
{
  auto existing = functions.find(name);

  if (existing != functions.end())
    functions.erase(existing);
}

void AngelScript::Object::asDefineMethod(const std::string& name, const std::string& declaration)
{
  auto     existing = functions.find(name);

  if (existing == functions.end() && declaration != "")
  {
    Function function;

    if (module)
      function.function = module->GetFunctionByDecl(declaration.c_str());
    else
      function.function = 0;
    function.signature  = declaration;
    if (function.function || !module)
      functions.insert(Functions::value_type(name, function));
  }
  else if (existing->second.function == 0 || (declaration != existing->second.signature))
  {
    if (declaration != "")
      existing->second.signature = declaration;
    if (module)
      existing->second.function  = module->GetFunctionByDecl(existing->second.signature.c_str());
  }
}

AngelScript::Object::ReturnType AngelScript::Object::Call(const std::string& name, unsigned int argc, ...)
{
  ContextLock context_lock(context, module, this);
  va_list     ap;
  auto        it = functions.find(name);

  if (!context || !module)
    throw AngelScript::Exception(AngelScript::Exception::UnloadableFunction, name);
  if (it == functions.end())
    throw AngelScript::Exception(AngelScript::Exception::UndeclaredFunction, name);
  if (!(it->second.function))
  {
    it->second.function = module->GetFunctionByDecl(it->second.signature.c_str());
    if (!(it->second.function))
      throw AngelScript::Exception(AngelScript::Exception::UnloadableFunction, name);
  }
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
        context->SetArgByte  (i, (bool)(*((Type<bool>*)(param))));
        break ;
      case 'i':
        context->SetArgDWord (i, (int)(*((Type<int>*)(param))));
        //context->SetArgWord  (i, (int)(*((Type<int>*)(param)))); // This isn't the proper call. Pretty sure it should be.
        break ;
      case 'l':
        context->SetArgDWord (i, (long)(*((Type<long>*)(param))));
        break ;
      case 'd':
        context->SetArgDouble(i, (double)(*((Type<double>*)(param))));
        break ;
      case 'f':
        context->SetArgFloat (i, (float)(*((Type<float>*)(param))));
        break ;
    }
  }
  va_end(ap);
  switch (context->Execute())
  {
    case asCONTEXT_NOT_PREPARED:
      throw AngelScript::Exception(AngelScript::Exception::InternalError, name); 
    case asEXECUTION_ABORTED:
    case asEXECUTION_SUSPENDED:
    case asEXECUTION_EXCEPTION:
      throw AngelScript::Exception(AngelScript::Exception::AngelScriptException, name);
    case asEXECUTION_FINISHED:
      break ;
  }
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
