#ifndef  INTERACT_MENU_HPP
# define INTERACT_MENU_HPP

# include "ui/rocket_extension.hpp"
# include "level/objects/instance_dynamic_object.hpp"

class InteractMenu : public UiBase
{  
public:
  InteractMenu(WindowFramework* window, Rocket::Core::Context* context, InstanceDynamicObject& object);
  ~InteractMenu();

  Sync::Signal<void> Done;
private:
  void ButtonClicked(Rocket::Core::Event&);
  void ButtonHovered(Rocket::Core::Event&);
  void MouseButton(Rocket::Core::Event&);
  
  void ExecuteForButtonId(Rocket::Core::Event& event, std::function<bool (Rocket::Core::Event&, const std::string&, Interactions::Interaction*)> callback);

  typedef std::vector<Interactions::Interaction*> Listeners;
  typedef std::vector<Rocket::Core::Element*>     Buttons;

  Buttons                      _buttons;
  Listeners                    _listeners;
  RocketListener               _buttonListener;
  RocketListener               _buttonHover, _buttonClick;
  Sync::ObserverHandler _obs;
  bool                         _done;
};

#endif
