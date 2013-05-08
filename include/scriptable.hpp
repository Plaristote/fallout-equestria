#ifndef  SCRIPTABLE_HPP
# define SCRIPTABLE_HPP

# include <angelscript.h>
# include <list>
# include <utility>
# include <string>

class Scriptable
{
public:
  Scriptable(void);
  virtual ~Scriptable(void);

  void LoadScript(std::string module_name, std::string filepath, asIScriptContext* context = 0);
  void ReloadFunction(asIScriptFunction** pointer);
  
protected:
  typedef std::pair<asIScriptFunction**, std::string> ScriptFuncPtr;
  typedef std::list<ScriptFuncPtr>                    ScriptFuncPtrs;
  
  asIScriptContext* _script_context;
  asIScriptModule*  _script_module;
  ScriptFuncPtrs    _script_func_ptrs;
};

#endif