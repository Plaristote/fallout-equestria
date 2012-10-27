#ifndef  PIPBUCK_HPP
# define PIPBUCK_HPP

# include "rocket_extension.hpp"
# include "scriptengine.hpp"
# include "dataengine.hpp"

class Pipbuck : public UiBase
{
public:
  Pipbuck(WindowFramework*, Rocket::Core::Context*, DataEngine&);

  void ReloadApps(void);

private:  
  DataEngine&       _data_engine;
  asIScriptContext* _context;
  asIScriptModule*  _module;
};

#endif
