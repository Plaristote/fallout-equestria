#ifndef  GAME_OPTIONS_HPP
# define GAME_OPTIONS_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"
# include <list>

class GameOptions : public UiBase
{
public:
  GameOptions(WindowFramework* window, Rocket::Core::Context* context);
  ~GameOptions(void);
private:
  RocketListener ExitClicked;
  RocketListener LanguageSelected, QualitySelected, ScreenSelected, FullscreenToggled, CameraFocusChanged;
  
  void SetLanguage(Rocket::Core::Event&);
  void SetResolution(Rocket::Core::Event&);
  void SetQuality(Rocket::Core::Event&);
  void ToggleFullscreen(Rocket::Core::Event&);
  
  std::list<Rocket::Core::Element*> _language_options;
};

#endif
