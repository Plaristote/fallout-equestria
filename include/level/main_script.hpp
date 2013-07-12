#ifndef  MAIN_SCRIPT_HPP
# define MAIN_SCRIPT_HPP

# include "as_object.hpp"

class MainScript : public AngelScript::Object
{
public:
  MainScript(const std::string& level_name) : AngelScript::Object("scripts/level/" + level_name + ".as")
  {
    asDefineMethod("Run", "void run(float)");
  }
};

#endif