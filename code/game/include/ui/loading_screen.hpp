#ifndef  LOADING_SCREEN_HPP
# define LOADING_SCREEN_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"
# include <queue>

class LoadingScreen : public UiBase, public Sync::Semaphore
{
  typedef std::queue<std::string> StringQueue;
public:
  static LoadingScreen* Current;
  
  LoadingScreen(WindowFramework*, Rocket::Core::Context* rocket);
  ~LoadingScreen();

  static void      AppendText(const std::string& str);
  static void      SetBackground(const std::string& str);

private:
  void             DoAppendText(const std::string& str);
  void             Refresh(void);

  bool                       done;
};

#endif
