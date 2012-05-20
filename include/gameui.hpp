#ifndef  GAME_UI_HPP
# define GAME_UI_HPP

# include <panda3d/pgVirtualFrame.h>
# include <panda3d/rocketRegion.h>
# include <Rocket/Core.h>
# include <panda3d/orthographicLens.h>
# include <Rocket/Core/XMLParser.h>
# include <panda3d/pandaFramework.h>

# include "observatory.hpp"

struct MyRocket
{
  static Rocket::Core::Element* GetChildren(Rocket::Core::Element* element, const std::string& name)
  {
    Rocket::Core::Element* elem;

    for (unsigned int i = 0 ; elem = element->GetChild(i) ; ++i)
    {
      if (elem->GetId().CString() == name)
        return (elem);
    }
    return (0);
  }
};

struct RocketListener : public Rocket::Core::EventListener
{
  void ProcessEvent(Rocket::Core::Event& event) { EventReceived.Emit(event); }

  Observatory::Signal<void (Rocket::Core::Event&)> EventReceived;
};

class GameMenu
{
public:
  GameMenu(WindowFramework* window);
  void MenuEventContinue(Rocket::Core::Event& event) { _rocket->set_active(false); }
  void MenuEventExit(Rocket::Core::Event& event);
  void Show(void) { _rocket->set_active(true);  }
  void Hide(void) { _rocket->set_active(false); }

private:
  WindowFramework*       _window;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;

  RocketListener         _continueClicked;
  RocketListener         _exitClicked;
  RocketListener         _optionsClicked;  
};

class GameMainBar
{
public:
  GameMainBar(WindowFramework* window);

  RocketListener         MenuButtonClicked;
  
private:
  WindowFramework*       _window;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;
};

class GameUi
{
public:
  GameUi(WindowFramework* window);

  GameMenu& GetMenu(void) { return (_menu); }

  void      OpenMenu(Rocket::Core::Event&);

private:
  WindowFramework* _window;
  GameMenu         _menu;
  GameMainBar      _mainBar;
};


#endif