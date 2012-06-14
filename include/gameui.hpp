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

# include "rocket_extension.hpp"

class GameMenu : public UiBase
{
  friend class GameUi;
public:
  GameMenu(WindowFramework* window, Rocket::Core::Context* context);
  void MenuEventContinue(Rocket::Core::Event& event) { Hide(); }
  void MenuEventExit(Rocket::Core::Event& event);

private:
  RocketListener         _continueClicked;
  RocketListener         _exitClicked;
  RocketListener         _optionsClicked;  
};

class InventoryObject;
class GameMainBar : public UiBase
{
  friend class GameUi;
public:
  GameMainBar(WindowFramework* window, Rocket::Core::Context* context);
  
  void                   AppendToConsole(const std::string& str);
  
  void                   SetCurrentAP(unsigned short ap);
  void                   SetMaxAP(unsigned short ap);
  void                   SetEnabledAP(bool);
  
  void                   SetEquipedItem(unsigned short it, InventoryObject* item);

  RocketListener         MenuButtonClicked;
  RocketListener         InventoryButtonClicked;
  RocketListener         PersButtonClicked;
  
  Observatory::Signal<void (unsigned short)> UseEquipedItem;
  Observatory::Signal<void>                  CombatPassTurn;
  Observatory::Signal<void>                  CombatEnd;
  
private:
  RocketListener         EquipedItem1Clicked;
  RocketListener         EquipedItem2Clicked;
  RocketListener         PassTurnClicked;
  RocketListener         CombatEndClicked;
  
  void                   CallbackEquipedItem1Clicked(Rocket::Core::Event&) { UseEquipedItem.Emit(0); }
  void                   CallbackEquipedItem2Clicked(Rocket::Core::Event&) { UseEquipedItem.Emit(1); }
  void                   CallbackPassTurnClicked(Rocket::Core::Event&)  { CombatPassTurn.Emit(); std::cout << "GENIUS" << std::endl; }
  void                   CallbackCombatEndClicked(Rocket::Core::Event&) { CombatEnd.Emit();      }
  
  unsigned short         _apMax;
  bool                   _apEnabled;
};

class GameInventory : public UiBase
{
  friend class GameUi;
public:
  GameInventory(WindowFramework* window, Rocket::Core::Context* context);
};

class GameConsole : public UiBase
{
  friend class GameUi;
public:
  GameConsole(WindowFramework* window, Rocket::Core::Context* context);
  ~GameConsole(void);

  static GameConsole* GConsole;

  void Show(void) { _root->Show(); }
  void Hide(void) { _root->Hide(); }

  static GameConsole& Get();

  static void WriteOn(const std::string& string);
  static void ListFunctions();
  static void PrintScenegraph();
  static void PrintChildren(const NodePath& n, int lvl);

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

  std::vector<std::string>      _history;
  vector<std::string>::iterator _histIter;

  WindowFramework*               _window;
  Rocket::Core::ElementDocument* _root;
  Rocket::Core::Element*         _input;
  std::string                    _currentLine;
  asIScriptContext*              _scriptContext;
  Observatory::ObserverId        _observerError;
};

class GamePers : public UiBase
{
public:
  GamePers(WindowFramework* window, Rocket::Core::Context* context);

  RocketListener CancelButton;
  RocketListener DoneButton;

private:
  void           Close(Rocket::Core::Event&) { Hide(); }
};

class GameUi
{
public:
  GameUi(WindowFramework* window);
  ~GameUi();

  GameMainBar&           GetMainBar(void) { return (*_mainBar); }
  GameMenu&              GetMenu(void) { return (*_menu); }
  Rocket::Core::Context* GetContext() { return (_rocket->get_context()); }

  Observatory::Signal<void (bool)> InterfaceOpened;
  
  void                   OpenMenu(Rocket::Core::Event&);
  void                   OpenInventory(Rocket::Core::Event&);
  void                   OpenPers(Rocket::Core::Event&);

private:
  WindowFramework*       _window;
  GameConsole*           _console;
  GameMenu*              _menu;
  GameMainBar*           _mainBar;
  GameInventory*         _inventory;
  GamePers*              _pers;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;
};

#endif