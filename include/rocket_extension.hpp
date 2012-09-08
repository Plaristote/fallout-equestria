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
  virtual void Hide(void)    { if (_root) { _root->Hide(); _root->PushToBack(); VisibilityToggled.Emit(false); } }
  bool IsVisible(void) const { return (_root && _root->IsVisible()); }

  virtual void Destroy(void) { if (_root) { _root->Hide(); } }

  Observatory::Signal<void (bool)> VisibilityToggled;
  
  static void SetPropertyOnAll(Rocket::Core::Element* elem, const std::string& property, const std::string& value)
  {
    Rocket::Core::Element* child;
    
    for (int it = 0 ; child = elem->GetChild(it) ; ++it)
    {
      child->SetProperty(property.c_str(), value.c_str());
      SetPropertyOnAll(child, property, value);
    }
  }
  
  void FadeOut(void);
  void FadeIn(void);

protected:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
  Rocket::Core::Context*         _context;
};

#endif