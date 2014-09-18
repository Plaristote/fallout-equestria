#ifndef  MAIN_SCRIPT_HPP
# define MAIN_SCRIPT_HPP

# include "as_object.hpp"

class MainScript : public AngelScript::Object
{
public:
  MainScript(const std::string& level_name);
  ~MainScript();
};

#endif
