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

#include "directory.hpp"
class UiLoad : public UiBase
{
public:
  UiLoad(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context)
  {
    _selectedSlot = 0;
    _root         = context->LoadDocument("data/loadgame.rml");
    if (_root)
    {
      Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
      
      if (slotContainer)
      {
	Directory      dir;
	unsigned short nSlots = 0;

	dir.OpenDir(savePath);
	std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const struct dirent& entry)
	{
	  if (entry.d_type == DT_DIR)
	  {
	    std::string str("slot-");
	    bool        compare = true;

	    for (int i = 0 ; i < str.length() ; ++i)
	    {
	      if (str[i] != entry.d_name[i])
	      {
		compare = false;
		break ;
	      }
	    }
	    std::cout << "Directory " << entry.d_name << (compare ? "is" : "isn't") << " a directory" << std::endl;
	    if (compare)
	      nSlots++;
	  }
	});

	std::stringstream rml;
	for (int it = 0 ; it < nSlots ; ++it)
	{
	  rml << "<div class='load-slot' id='load-slot-" << it << "' data-slot='" << it << "'>";
	  rml << "<div class='load-slot-title'>Slot " << it << "</div>";
	  rml << "</div>";
	}
	slotContainer->SetInnerRML(rml.str().c_str());
	
	for (int it = 0 ; it < nSlots ; ++it)
	{
	  std::stringstream      idSlot;
	  Rocket::Core::Element* slotElement;

	  idSlot << "load-slot-" << it;
	  slotElement = _root->GetElementById(idSlot.str().c_str());
	  slotElement->AddEventListener("click",    &EventClickSlot);
	  slotElement->AddEventListener("dblclick", &EventClickSlot);
	  slotElement->AddEventListener("dblclick", &EventLoadGame);
	}
      }
	
      Rocket::Core::Element* buttonLoad = _root->GetElementById("button-load");
      
      if (buttonLoad)
	buttonLoad->AddEventListener("click", &EventLoadGame);
    }
    EventClickSlot.EventReceived.Connect(*this, &UiLoad::ClickSlot);
    EventLoadGame.EventReceived.Connect(*this, &UiLoad::LoadGame);
  }
  
  Observatory::Signal<void (unsigned char)> LoadSlot;
  
private:
  void LoadGame(Rocket::Core::Event&)
  {
    std::cout << "LoadGame called" << std::endl;
    if (_selectedSlot)
    {
      Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
      unsigned int           slot    = 0;

      slot = varSlot->Get<unsigned int>();
      LoadSlot.Emit((unsigned char)slot);
    }
  }
  
  void ClickSlot(Rocket::Core::Event& event)
  {
    _selectedSlot = event.GetCurrentElement();
  }
  
  RocketListener EventLoadGame, EventClickSlot;

  Rocket::Core::Element* _selectedSlot;  
};

class UiSave : public UiBase
{
public:
  UiSave(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context)
  {
    _root = context->LoadDocument("data/savegame.rml");
    if (_root)
    {
      Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
      
      if (slotContainer)
      {
	Directory      dir;
	unsigned short nSlots = 0;

	dir.OpenDir(savePath);
	std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const struct dirent& entry)
	{ if (entry.d_type == DT_DIR && entry.d_name != "." && entry.d_name != "..") nSlots++; });

	std::stringstream rml;
	
	for (unsigned short it = 0 ; it <= nSlots ; ++it)
	{
	  rml << "<div class='save-slot' id='save-slot-" << it << "' data-slot='" << it << "'>";
	  rml << "<div class='save-slot-title'>Slot " << it << "</div>";
	  rml << "</div>";
	}
	slotContainer->SetInnerRML(rml.str().c_str());
	for (unsigned short it = 0 ; it <= nSlots ; ++it)
	{
	  std::stringstream      idSlot;
	  Rocket::Core::Element* slotElement;
	  
	  idSlot << "save-slot-" << it;
	  slotElement = _root->GetElementById(idSlot.str().c_str());
	  slotElement->AddEventListener("click",    &EventClickSlot);
	  slotElement->AddEventListener("dblclick", &EventClickSlot);
	  slotElement->AddEventListener("dblclick", &EventSaveGame);
	}
      }
    }
    EventClickSlot.EventReceived.Connect(*this, &UiSave::ClickSlot);
    EventSaveGame.EventReceived.Connect(*this, &UiSave::SaveGame);
  }

  Observatory::Signal<bool (unsigned char)> SaveToSlot;

private:
  void SaveGame(Rocket::Core::Event&)
  {
    std::cout << "SaveGame called" << std::endl;
    if (_selectedSlot)
    {
      Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
      unsigned int           slot    = 0;

      slot = varSlot->Get<unsigned int>();
      SaveToSlot.Emit((unsigned char)slot);
    }
  }
  
  void ClickSlot(Rocket::Core::Event& event)
  {
    _selectedSlot = event.GetCurrentElement();
  }
  
  RocketListener EventSaveGame, EventClickSlot;

  Rocket::Core::Element* _selectedSlot;
};


class GameMenu : public UiBase
{
  friend class GameUi;
public:
  GameMenu(WindowFramework* window, Rocket::Core::Context* context);
  void MenuEventContinue(Rocket::Core::Event& event) { Hide(); }
  void MenuEventExit(Rocket::Core::Event& event);
  
  Observatory::Signal<void (Rocket::Core::Event&)> SaveClicked;
  Observatory::Signal<void (Rocket::Core::Event&)> LoadClicked;

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
  
  void                   AppendToConsole(const std::string& str);
  
  void                   SetCurrentAP(unsigned short ap);
  void                   SetMaxAP(unsigned short ap);
  void                   SetEnabledAP(bool);
  
  void                   SetEquipedItem(unsigned short it, InventoryObject* item);
  void                   SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt);

  RocketListener         MenuButtonClicked;
  RocketListener         InventoryButtonClicked;
  RocketListener         PersButtonClicked;
  
  Observatory::Signal<void (unsigned short)> UseEquipedItem;
  Observatory::Signal<void (unsigned short)> EquipedItemNextAction;
  Observatory::Signal<void>                  CombatPassTurn;
  Observatory::Signal<void>                  CombatEnd;
  
private:
  RocketListener         EquipedItem1Clicked;
  RocketListener         EquipedItem2Clicked;
  RocketListener         PassTurnClicked;
  RocketListener         CombatEndClicked;
  
  void                   CallbackEquipedItem1Clicked(Rocket::Core::Event&);
  void                   CallbackEquipedItem2Clicked(Rocket::Core::Event&);
  void                   CallbackPassTurnClicked(Rocket::Core::Event&)  { CombatPassTurn.Emit(); std::cout << "GENIUS" << std::endl; }
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

  void                     SetInventory(Inventory&);
  void                     UpdateInventory(void);

  Observatory::Signal<void (unsigned short, InventoryObject*)> EquipItem;
  Observatory::Signal<void (unsigned short)>                   UnequipItem;
  Observatory::Signal<void (InventoryObject*)>                 UseObject;
  Observatory::Signal<void (InventoryObject*)>                 DropObject;

private:
  void                     UpdateInventoryCapacity(void);
  void                     SetSelectedObject(InventoryObject*);
  
  RocketListener           ButtonUseClicked, ButtonDropClicked, ButtonEquip1Clicked, ButtonEquip2Clicked, ButtonUnequip1, ButtonUnequip2;
  void                     CallbackButtonUse(Rocket::Core::Event&)      { if (_selectedObject) UseObject.Emit (_selectedObject);    }
  void                     CallbackButtonDrop(Rocket::Core::Event&)     { if (_selectedObject) DropObject.Emit(_selectedObject);    }
  void                     CallbackButtonEquip1(Rocket::Core::Event&)   { if (_selectedObject) EquipItem.Emit (0, _selectedObject); }
  void                     CallbackButtonEquip2(Rocket::Core::Event&)   { if (_selectedObject) EquipItem.Emit (1, _selectedObject); }
  void                     CallbackButtonUnequip1(Rocket::Core::Event&) { UnequipItem.Emit(0); }
  void                     CallbackButtonUnequip2(Rocket::Core::Event&) { UnequipItem.Emit(1); }

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
  GameUi(WindowFramework* window, PT(RocketRegion) rocket);
  ~GameUi(void);
  
  Rocket::Core::Context* GetContext()       { return (_rocket->get_context()); }
  GameMenu&              GetMenu(void)      { return (*_menu); }
  GameInventory&         GetInventory(void) { return (*_inventory); }
  GamePers&              GetPers(void)      { return (*_pers); }
  
  void                   OpenMenu(Rocket::Core::Event&);
  void                   OpenInventory(Rocket::Core::Event&);
  void                   OpenPers(Rocket::Core::Event&);
  
  PT(RocketRegion)       GetRocketRegion(void) { return (_rocket); }

private:
  PT(RocketRegion) _rocket;
  GameConsole*     _console;
  GameMenu*        _menu;
  GameInventory*   _inventory;
  GamePers*        _pers;
};

#include <queue>
class LoadingScreen : public UiBase
{
  typedef std::queue<std::string> StringQueue;
public:
  LoadingScreen(WindowFramework*, Rocket::Core::Context* rocket);
  
  void             AppendText(const std::string& str);
  void             FadeOut(void);
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
  GamePers&        GetPers(void)      { return (_gameUi.GetPers());      }

  Observatory::Signal<void (bool)> InterfaceOpened;  

private:
  GameUi&          _gameUi;
  GameMainBar*     _mainBar;
  
  Observatory::ObserverId _observers[6];
};

class GeneralUi
{
public:
  GeneralUi(WindowFramework*);
  ~GeneralUi();

  PT(RocketRegion)       GetRocketRegion(void) const { return (_rocket); }

private:
  WindowFramework*       _window;
  PT(RocketRegion)       _rocket;
  PT(RocketInputHandler) _ih;
  
  GameConsole*           _console;
};

#endif