#ifndef  UI_DIALOG_HPP
# define UI_DIALOG_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"

class UiDialog : public UiBase
{
  struct Button
  {
    Button(std::string id) : id(id) {}
    
    std::string    id;
    RocketListener listener;
  };
  
  typedef std::list<Button> Buttons;
  
public:
  UiDialog(WindowFramework* w, Rocket::Core::Context* c);
  ~UiDialog(void);

  void                   SetMessage(const std::string& message);
  void                   AddChoice(const std::string& name, std::function<void (Rocket::Core::Event&)> callback);

private:
  void                   PickedChoice(Rocket::Core::Event&);
  
  Rocket::Core::Element* _button_container;
  Buttons                _buttons;
};

#endif