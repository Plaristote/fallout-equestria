#ifndef  LOADING_SCREEN_HPP
# define LOADING_SCREEN_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "rocket_extension.hpp"
# include "observatory.hpp"
# include <queue>

class LoadingScreen : public UiBase, public Sync::Semaphore
{
  typedef std::queue<std::string> StringQueue;
public:
  LoadingScreen(WindowFramework*, Rocket::Core::Context* rocket);
  ~LoadingScreen();
  
  void             AppendText(const std::string& str);

private:
  void             Refresh(void);

  bool                       done;
};

#endif
