#ifndef  SCRIPTABLE_HPP
# define SCRIPTABLE_HPP

class Scriptable
{
public:
  Scriptable(void);
  virtual ~Scriptable(void);

  void LoadScript(std::string module_name, std::string filepath);
  void ReloadFunction(asIScriptFunction** pointer);
  
protected:
  typedef std::pair<asIScriptFunction**, std::string> ScriptFuncPtr;
  typedef std::list<ScriptFuncPtr>                    ScriptFuncPtrs;
  
  asIScriptContext* _script_context;
  asIScriptModule*  _script_module;
  ScriptFuncPtrs    _script_func_ptrs;
};

#endif