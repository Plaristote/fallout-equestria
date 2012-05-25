#ifndef  GAME_UI_HPP
# define GAME_UI_HPP

# include <panda3d/pgVirtualFrame.h>
# include <panda3d/rocketRegion.h>
# include <Rocket/Core.h>
# include <panda3d/orthographicLens.h>
# include <Rocket/Core/XMLParser.h>
# include <panda3d/pandaFramework.h>

# include "observatory.hpp"
# include "scriptengine.hpp"

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
  friend class GameUi;
public:
  GameMenu(WindowFramework* window, Rocket::Core::Context* context);
  void MenuEventContinue(Rocket::Core::Event& event) { Hide(); }
  void MenuEventExit(Rocket::Core::Event& event);
  void Show(void) { _root->Show(); }
  void Hide(void) { _root->Hide(); }

private:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;

  RocketListener         _continueClicked;
  RocketListener         _exitClicked;
  RocketListener         _optionsClicked;  
};

class GameMainBar
{
  friend class GameUi;
public:
  GameMainBar(WindowFramework* window, Rocket::Core::Context* context);

  RocketListener         MenuButtonClicked;
  RocketListener         InventoryButtonClicked;
  
private:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
};

class GameInventory
{
  friend class GameUi;
public:
  GameInventory(WindowFramework* window, Rocket::Core::Context* context);

  void Show(void) { _root->Show(); }
  void Hide(void) { _root->Hide(); }

private:
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
};

class GameConsole
{
  friend class GameUi;
public:
  GameConsole(WindowFramework* window, Rocket::Core::Context* context);
  ~GameConsole(void);

  static GameConsole* GConsole;

  void Show(void) { _root->Show(); }
  void Hide(void) { _root->Hide(); }

  static GameConsole& Get();

  static void ListFunctions();
  static void PrintScenegraph();
  static void PrintChildren(NodePath& n, int lvl);

  static void Toggle(const Event*, void* data)
  {
    GameConsole* console = reinterpret_cast<GameConsole*>(data);
    (console->_root->IsVisible() ? console->Hide() : console->Show());
  }

  RocketListener         ConsoleKeyUp;
  RocketListener         ExecuteEvent;

private:
  void KeyUp(Rocket::Core::Event&);
  void Execute(Rocket::Core::Event&);
  void Output(const std::string str);

  std::vector<std::string>		_history;
  vector<std::string>::iterator _histIter;
  
  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
  Rocket::Core::Element*		 _input;
  std::string                    _currentLine;
  asIScriptContext*              _scriptContext;
  Observatory::ObserverId        _observerError;
};

class GameUi
{
public:
  GameUi(WindowFramework* window);
  ~GameUi();

  GameMenu& GetMenu(void) { return (*_menu); }

  void      OpenMenu(Rocket::Core::Event&);
  void      OpenInventory(Rocket::Core::Event&);

private:
  WindowFramework*       _window;
  GameConsole*           _console;
  GameMenu*              _menu;
  GameMainBar*           _mainBar;
  GameInventory*         _inventory;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;
};

#endif