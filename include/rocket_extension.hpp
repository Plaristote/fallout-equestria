#ifndef  ROCKET_EXTENSION_HPP
# define ROCKET_EXTENSION_HPP

# include <panda3d/pandaFramework.h>
# include <Rocket/Core.h>
# include "observatory.hpp"

struct RocketListener : public Rocket::Core::EventListener
{
  void ProcessEvent(Rocket::Core::Event& event) { EventReceived.Emit(event); }

  Observatory::Signal<void (Rocket::Core::Event&)> EventReceived;
};

class GameUi;

class UiBase
{
  friend class GameUi;
public:
  UiBase(WindowFramework* window) : _window(window), _root(0) {}

  void Show(void)            { _root->Show();               }
  void Hide(void)            { _root->Hide();               }
  bool IsVisible(void) const { return (_root->IsVisible()); }

protected:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
};

#endif