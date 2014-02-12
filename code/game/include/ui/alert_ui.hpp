#ifndef  ALERT_UI_HPP
# define ALERT_UI_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "rocket_extension.hpp"
# include "observatory.hpp"

class AlertUi : public UiBase
{
public:
  static Sync::Signal<void (const std::string)> NewAlert;
  
  AlertUi(WindowFramework* window, Rocket::Core::Context* context, const std::string& message);
  ~AlertUi();

  bool Run(void);

private:
  RocketListener ButtonClicked;
  bool           _continue;
};

#endif
