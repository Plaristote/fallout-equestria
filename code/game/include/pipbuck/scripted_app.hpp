#ifndef  PIPBUCK_SCRIPTED_APP_HPP
# define PIPBUCK_SCRIPTED_APP_HPP

# include "pipbuck/pipbuck.hpp"

class PipbuckAppScript : public Pipbuck::App
{
public:
  PipbuckAppScript(Data script);
  ~PipbuckAppScript()
  {
    if (_object)
      delete _object;
  }
  
  std::string GetAppId(void) { return (_data.Key()); }

  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&);
  
  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);

private:
  Data                 _data;
  AngelScript::Object* _object;
};

#endif