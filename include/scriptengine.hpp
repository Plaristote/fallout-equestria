#ifndef  SCRIPTENGINE_HPP
# define SCRIPTENGINE_HPP

# include <angelscript.h>
# include <scriptbuilder/scriptbuilder.h>
# include <scriptstdstring/scriptstdstring.h>

namespace Script
{
  class Engine
  {
  public:
    static asIScriptEngine* Get(void) { return (_engine);  }
    static void             Initialize(void);
    static void             Finalize(void);
    static asIScriptModule* LoadModule(const std::string& name, const std::string& filepath);

  private:
    static asIScriptEngine* _engine;
  };
}

#endif