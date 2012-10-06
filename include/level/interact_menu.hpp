#ifndef  INTERACT_MENU_HPP
# define INTERACT_MENU_HPP

# include "rocket_extension.hpp"
# include "objectnode.hpp"

class InteractMenu : public UiBase
{  
public:
  InteractMenu(WindowFramework* window, Rocket::Core::Context* context, InstanceDynamicObject& object);
  ~InteractMenu();
private:
  void ButtonClicked(Rocket::Core::Event&);

  typedef std::vector<InstanceDynamicObject::Interaction*> Listeners;
  typedef std::vector<Rocket::Core::Element*>              Buttons;
  
  Buttons                      _buttons;
  Listeners                    _listeners;
  RocketListener               _buttonListener;
  Observatory::ObserverHandler _obs;
  bool                         _done;
};

#endif
