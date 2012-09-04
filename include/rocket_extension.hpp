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
  UiBase(WindowFramework* window, Rocket::Core::Context* context) : _window(window), _root(0), _context(context) {}
  virtual ~UiBase() {}

  virtual void Show(void)    { if (_root) { _root->Show(); VisibilityToggled.Emit(true);  } }
  virtual void Hide(void)    { if (_root) { _root->Hide(); VisibilityToggled.Emit(false); } }
  bool IsVisible(void) const { return (_root && _root->IsVisible()); }

  virtual void Destroy(void) { if (_root) { _root->Hide(); } }

  Observatory::Signal<void (bool)> VisibilityToggled;

protected:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
  Rocket::Core::Context*         _context;
};

#endif