#ifndef  ROCKET_EXTENSION_HPP
# define ROCKET_EXTENSION_HPP

# include <panda3d/pandaFramework.h>
# include <Rocket/Core.h>
# include "observatory.hpp"
# include "i18n.hpp"

struct MyRocket
{
  static void SetVisibility(Rocket::Core::Context* context, bool visibility);
private:
  MyRocket() {}
};

struct RocketListener : public Rocket::Core::EventListener
{
  void ProcessEvent(Rocket::Core::Event& event) { EventReceived.Emit(event); }

  Observatory::Signal<void (Rocket::Core::Event&)> EventReceived;
};

class GameUi;

class UiBase
{
  friend class GameUi;
  
  struct Listener
  {
    Listener(Rocket::Core::Element* elem, const std::string& event, RocketListener& instance) : elem(elem->GetId().CString()), event(event), instance(instance) {}

    bool operator==(const Listener& listener) const { return (listener.elem == elem && listener.event == event && &listener.instance == &instance); }

    const std::string      elem;
    const std::string      event;
    RocketListener&        instance;
  };
  
  typedef std::list<Listener> Listeners;
  
public:
  UiBase(WindowFramework* window, Rocket::Core::Context* context);
  virtual ~UiBase();
  
  void         FireShow(Rocket::Core::Event&) { Show(); }
  void         FireHide(Rocket::Core::Event&) { Hide(); }

  virtual void Show(void)            { if (_root) { _root->Show(); VisibilityToggled.Emit(true); VisibilityToggledOn.Emit(); } }
  virtual void Hide(void)            { if (_root) { _root->Hide(); _root->PushToBack(); VisibilityToggled.Emit(false); VisibilityToggledOff.Emit(); } }
  bool         IsVisible(void) const { return (_root && _root->IsVisible()); }

  virtual void Destroy(void) { if (_root) { _root->Hide(); } }

  Observatory::Signal<void (bool)> VisibilityToggled;
  Observatory::Signal<void>        VisibilityToggledOn, VisibilityToggledOff;
  
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
  
  void ToggleEventListener(bool toggle_on, const std::string& id, const std::string& event, RocketListener& listener);
  void Translate(void);

protected:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
  Rocket::Core::Context*         _context;
private:
  void RecursiveTranslate(Rocket::Core::Element*);
  
  Observatory::ObserverId        _languageObs;
  Listeners                      _listeners;
};

#endif