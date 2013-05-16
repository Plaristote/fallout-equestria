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
  ~GameMenu();
  void MenuEventContinue(Rocket::Core::Event& event) { Hide(); }
  
  Sync::Signal<void (Rocket::Core::Event&)> SaveClicked;
  Sync::Signal<void (Rocket::Core::Event&)> LoadClicked;
  Sync::Signal<void (Rocket::Core::Event&)> ExitClicked;
  Sync::Signal<void (Rocket::Core::Event&)> OptionsClicked;

private:
  RocketListener         _continueClicked;
  RocketListener         _exitClicked;
  RocketListener         _optionsClicked;  
  RocketListener         _saveClicked;
  RocketListener         _loadClicked;
};

class InventoryObject;
class Inventory;

class GameMainBar : public UiBase
{
  friend class GameUi;
public:
  GameMainBar(WindowFramework* window, Rocket::Core::Context* context);
  ~GameMainBar();
  
  void                   AppendToConsole(const std::string& str);

  void                   SetCurrentAP(unsigned short ap, unsigned short maxap);
  void                   SetMaxAP(unsigned short ap);
  void                   SetEnabledAP(bool);
  
  void                   SetCurrentHp(short hp);
  void                   SetCurrentAc(short ac);
  
  void                   SetEquipedItem(unsigned short it, InventoryObject* item);
  void                   SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt);

  RocketListener         MenuButtonClicked;
  RocketListener         InventoryButtonClicked;
  RocketListener         PersButtonClicked;
  RocketListener         PipbuckButtonClicked;
  RocketListener         SkilldexButtonClicked, SpelldexButtonClicked;
  
  Sync::Signal<void (unsigned short)> UseEquipedItem;
  Sync::Signal<void (unsigned short)> EquipedItemNextAction;
  Sync::Signal<void>                  CombatPassTurn;
  Sync::Signal<void>                  CombatEnd;
  Sync::Signal<void>                  OpenSkilldex;
  Sync::Signal<void>                  OpenSpelldex;
  
private:
  RocketListener         EquipedItem1Clicked;
  RocketListener         EquipedItem2Clicked;
  RocketListener         PassTurnClicked;
  RocketListener         CombatEndClicked;
  
  void                   CallbackEquipedItem1Clicked(Rocket::Core::Event&);
  void                   CallbackEquipedItem2Clicked(Rocket::Core::Event&);
  void                   CallbackPassTurnClicked(Rocket::Core::Event&)  { CombatPassTurn.Emit(); }
  void                   CallbackCombatEndClicked(Rocket::Core::Event&) { CombatEnd.Emit();      }
  
  unsigned short         _apMax;
  bool                   _apEnabled;
};

#include "inventory_ui.hpp"
class GameInventory : public UiBase
{
  friend class GameUi;
public:
  GameInventory(WindowFramework* window, Rocket::Core::Context* context);
  ~GameInventory();

  void                     SetInventory(Inventory&);
  void                     UpdateInventory(void);

  Sync::Signal<void (unsigned short, InventoryObject*)>    EquipItem;
  Sync::Signal<void (unsigned short)>                      UnequipItem;
  Sync::Signal<void (unsigned short, EquipedMode)>         SwapEquipMode;
  Sync::Signal<void (InventoryObject*)>                    UseObject;
  Sync::Signal<void (InventoryObject*)>                    DropObject;
  Sync::Signal<void (InventoryObject*, InventoryObject*)>  CombineObjects;
  
  void                     SetEquipedItem(unsigned short slot, InventoryObject* object);

private:
  void                     UpdateInventoryCapacity(void);
  void                     SetSelectedObject(InventoryObject*);
  void                     ListenEquipModes(bool);
  void                     ListenDropables(bool);
  
  RocketListener           DropEvent;
  RocketListener           ButtonUseClicked, ButtonDropClicked, ButtonEquip1Clicked, ButtonEquip2Clicked, ButtonUnequip1, ButtonUnequip2, ButtonEquipMode;
  void                     CallbackButtonUse(Rocket::Core::Event&)      { if (_selectedObject) UseObject.Emit (_selectedObject);    }
  void                     CallbackButtonDrop(Rocket::Core::Event&)     { if (_selectedObject) DropObject.Emit(_selectedObject);    }
  void                     CallbackButtonEquip1(Rocket::Core::Event&)   { if (_selectedObject) EquipItem.Emit (0, _selectedObject); }
  void                     CallbackButtonEquip2(Rocket::Core::Event&)   { if (_selectedObject) EquipItem.Emit (1, _selectedObject); }
  void                     CallbackButtonUnequip1(Rocket::Core::Event&) { UnequipItem.Emit(0); }
  void                     CallbackButtonUnequip2(Rocket::Core::Event&) { UnequipItem.Emit(1); }
  void                     CallbackSwapEquipMode(Rocket::Core::Event&);
  void                     CallbackDropEvent(Rocket::Core::Event&);

  Inventory*               _inventory;
  InventoryViewController  _inventoryView;
  InventoryObject*         _selectedObject;
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
  asIScriptContext*              _script_context;
  Sync::ObserverId        _observerError;
};

# include "statsheet.hpp"
class GameUi
{
public:
  GameUi(WindowFramework* window, PT(RocketRegion) rocket);
  ~GameUi(void);
  
  Rocket::Core::Context* GetContext()       { return (_rocket->get_context()); }
  GameMenu&              GetMenu(void)      { return (*_menu); }
  GameInventory&         GetInventory(void) { return (*_inventory); }
  StatViewRocket&        GetPers(void)      { return (*_pers); }
  
  void                   OpenMenu(Rocket::Core::Event&);
  void                   OpenInventory(Rocket::Core::Event&);
  void                   OpenPers(Rocket::Core::Event&);
  Sync::Signal<void (Rocket::Core::Event&)> OpenPipbuck;
  
  PT(RocketRegion)       GetRocketRegion(void) { return (_rocket); }

private:
  PT(RocketRegion) _rocket;
  GameConsole*     _console;
  GameMenu*        _menu;
  GameInventory*   _inventory;
  StatViewRocket*  _pers;
};

#include <queue>
#include "Boots/functorthread.hpp"
class LoadingScreen : public UiBase, public Sync::Semaphore
{
  typedef std::queue<std::string> StringQueue;
public:
  LoadingScreen(WindowFramework*, Rocket::Core::Context* rocket);
  ~LoadingScreen();
  
  void             AppendText(const std::string& str);

private:
  void             Refresh(void);

  Sync::FunctorThread<void>* thread;
  bool                       done;
};

class LevelUi
{
public:
  LevelUi(WindowFramework* window, GameUi& gameUi);
  ~LevelUi(void);

  Rocket::Core::Context* GetContext() { return (_gameUi.GetRocketRegion()->get_context()); }  
  GameMainBar&     GetMainBar(void)   { return (*_mainBar);              }
  GameMenu&        GetMenu(void)      { return (_gameUi.GetMenu());      }
  GameInventory&   GetInventory(void) { return (_gameUi.GetInventory()); }
  StatViewRocket&  GetPers(void)      { return (_gameUi.GetPers());      }

  Sync::Signal<void (bool)> InterfaceOpened;  

private:
  GameUi&                      _gameUi;
  GameMainBar*                 _mainBar;  
  Sync::ObserverHandler _obs;
};

class GameOptions : public UiBase
{
public:
  GameOptions(WindowFramework* window, Rocket::Core::Context* context);
  ~GameOptions(void);
private:
  RocketListener ExitClicked;
  RocketListener LanguageSelected, QualitySelected, ScreenSelected, FullscreenToggled;
  
  void SetLanguage(Rocket::Core::Event&);
  void SetResolution(Rocket::Core::Event&);
  void SetQuality(Rocket::Core::Event&);
  void ToggleFullscreen(Rocket::Core::Event&);
  
  std::list<Rocket::Core::Element*> _language_options;
};

class AlertUi : public UiBase
{
public:
  static Sync::Signal<void (const std::string)> NewAlert;
  
  AlertUi(WindowFramework* window, Rocket::Core::Context* context, const std::string& message);
  ~AlertUi();

  bool Run(void);

private:
  RocketListener ButtonClicked;
  bool           _continue;
};

class GeneralUi
{
public:
  GeneralUi(WindowFramework*);
  ~GeneralUi();

  PT(RocketRegion)       GetRocketRegion(void) const { return (_rocket); }
  GameOptions&           GetOptions(void) { return (*_options); }

private:
  WindowFramework*       _window;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;

  GameConsole*           _console;
  GameOptions*           _options;
};

#endif
