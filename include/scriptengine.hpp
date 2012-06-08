#ifndef  SCRIPTENGINE_HPP
# define SCRIPTENGINE_HPP

# include <angelscript.h>
# include <scriptbuilder/scriptbuilder.h>
# include <scriptstdstring/scriptstdstring.h>
# include "observatory.hpp"

namespace Script
{
  class Engine
  {
  public:
    static asIScriptEngine* Get(void) { return (_engine);  }
    static void             Initialize(void);
    static void             Finalize(void);
    static asIScriptModule* LoadModule(const std::string& name, const std::string& filepath);

    static Observatory::Signal<void (const std::string)> ScriptError;

  private:
    static void             MessageCallback(const asSMessageInfo* msg, void* param);
    
    static asIScriptEngine* _engine;
  };
  
  class ModuleManager
  {
  public:
    static asIScriptModule* Require(const std::string& name, const std::string& filepath);
    static void             Release(asIScriptModule* module);
    
  private:
    struct LoadedModule
    {
      bool operator==(asIScriptModule* module) const { return (ptr == module);   }
      bool operator==(const std::string& str)  const { return (filepath == str); }
      
      std::string       filepath;
      asIScriptModule*  ptr;
      unsigned short    users;
    };
    typedef std::list<LoadedModule> Modules;
    
    static Modules _modules;
  };
}

#endif