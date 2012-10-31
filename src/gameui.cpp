#include "gameui.hpp"
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket;

extern PandaFramework framework;

/*
 * GeneralUi
 */
#define PATH_FONTS "data"

GeneralUi::GeneralUi(WindowFramework* window) : _window(window)
{
  //HAIL MICROSOFT!!!
	string fonts[] = { "JH_FALLOUT.TTF",
                           "Delicious-Roman.otf",
                           "Delicious-Italic.otf",
                           "Delicious-Bold.otf",
                           "Delicious-BoldItalic.otf" };

  //ForEach(fonts, [](string path) { Core::FontDatabase::LoadFontFace(Core::String(PATH_FONTS) + "/" + path.c_str()); });
  for (int i=0; i<GET_ARRAY_SIZE(fonts); i++)
	  Core::FontDatabase::LoadFontFace(Core::String(PATH_FONTS) + "/" + fonts[i].c_str());

  _rocket = RocketRegion::make("interface", window->get_graphics_output());
  _rocket->set_active(true);
  
  Rocket::Controls::Initialise();

  _ih      = new RocketInputHandler();
  window->get_mouse().attach_new_node(_ih);
  _rocket->set_input_handler(_ih);
  
  _console = new GameConsole(window, _rocket->get_context());
  _console->Hide();
  
  _options = new GameOptions(window, _rocket->get_context());
  _options->Hide();

  _window->enable_keyboard();
  framework.define_key("tab", "ConsoleHandle", GameConsole::Toggle, (void*)_console);  
}

GeneralUi::~GeneralUi(void)
{
  //framework.define_key("tab", "ConsoleHandle", 0, 0);
  delete _console;
}

/*
 * LevelUi
 */
LevelUi::LevelUi(WindowFramework* window, GameUi& gameUi) : _gameUi(gameUi)
{
  _mainBar = new GameMainBar(window, _gameUi.GetRocketRegion()->get_context());
  
  _observers[0] = _mainBar->MenuButtonClicked.EventReceived.Connect     (_gameUi, &GameUi::OpenMenu);
  _observers[1] = _mainBar->InventoryButtonClicked.EventReceived.Connect(_gameUi, &GameUi::OpenInventory);
  _observers[2] = _mainBar->PersButtonClicked.EventReceived.Connect     (_gameUi, &GameUi::OpenPers);
  _observers[3] = _mainBar->PipbuckButtonClicked.EventReceived.Connect  (_gameUi.OpenPipbuck, &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);
  

  _observers[4] = _gameUi.GetInventory().VisibilityToggled.Connect(InterfaceOpened, &Observatory::Signal<void (bool)>::Emit);
  _observers[5] = _gameUi.GetMenu().VisibilityToggled.Connect     (InterfaceOpened, &Observatory::Signal<void (bool)>::Emit);  
  _observers[6] = _gameUi.GetPers().VisibilityToggled.Connect     (InterfaceOpened, &Observatory::Signal<void (bool)>::Emit);
}

LevelUi::~LevelUi(void)
{
  _gameUi.GetInventory().VisibilityToggled.Disconnect(_observers[0]);
  _gameUi.GetMenu().VisibilityToggled.Disconnect     (_observers[1]);
  _gameUi.GetPers().VisibilityToggled.Disconnect     (_observers[2]);
  
  _gameUi.GetInventory().VisibilityToggled.Disconnect(_observers[3]);
  _gameUi.GetMenu().VisibilityToggled.Disconnect     (_observers[4]);
  _gameUi.GetPers().VisibilityToggled.Disconnect     (_observers[5]);

  delete _mainBar;
}

/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window, PT(RocketRegion) rocket) : _rocket(rocket)
{
  _menu      = new GameMenu      (window, _rocket->get_context());
  _inventory = new GameInventory (window, _rocket->get_context());
  _pers      = new StatViewRocket(window, _rocket->get_context());
  _menu->Hide();
  _inventory->Hide();
  _pers->Hide();
}

GameUi::~GameUi(void)
{
  _menu->Destroy();
  _inventory->Destroy();
  _pers->Destroy();
  delete _menu;
  delete _inventory;
  delete _pers;
}

void GameUi::OpenMenu(Rocket::Core::Event&)
{
  _menu->Show();
}

void GameUi::OpenInventory(Rocket::Core::Event&)
{
  if (!(_inventory->IsVisible()))
    _inventory->Show();
  else
    _inventory->Hide();
}

void GameUi::OpenPers(Rocket::Core::Event&)
{
  if (!(_pers->IsVisible()))
    _pers->Show();
  else
    _pers->Hide();
}

/*
 * LoadingScreen
 */
LoadingScreen::LoadingScreen(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  Core::ElementDocument* doc = context->LoadDocument("data/loading.rml");

  _root = doc;
  if (_root)
    Translate();
  Show();
}

void LoadingScreen::AppendText(const std::string& str)
{
  Core::Element* input = _root->GetElementById("content");
  Core::String   content;

  input->GetInnerRML(content);
  content = Core::String(str.c_str()) + "<br />" + content;
  input->SetInnerRML(content);
  framework.get_graphics_engine()->render_frame();
}

/*
 * GameOptions
 */
#include "options.hpp"
DataTree* OptionsManager::_data = 0;

void OptionsManager::Initialize(void)
{
  if (!_data)
  {
    _data = DataTree::Factory::JSON("conf.json");
    Refresh();
  }
}

void OptionsManager::Finalize(void)
{
  if (_data)
  {
    delete _data;
    _data = 0;
  }
}

Data OptionsManager::Get(void)
{
  if (!_data)
    Initialize();
  return (_data);
}

void OptionsManager::Refresh(void)
{
  Data data(_data);
  
  if (_data)
  {
    // Language
    Data language = data["language"];

    if (language.NotNil())
      i18n::Load(language.Value());
    
    // Screen
    Data             screen        = data["screen"];

    if (screen.NotNil())
    {
      unsigned int     screen_width  = screen["x"];
      unsigned int     screen_height = screen["y"];
      bool             fullscreen    = screen["fullscreen"] == 1;
      WindowProperties props         = framework.get_window(0)->get_graphics_window()->get_properties();

      props.set_cursor_hidden(false);
      props.set_fullscreen(fullscreen);
      props.set_size(screen_width, screen_height);
      framework.get_window(0)->get_graphics_window()->request_properties(props);
    }

    // Saving changes
    DataTree::Writers::JSON(_data, "conf.json");
  }
}

GameOptions::GameOptions(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/options.rml");
  if (_root)
  {
    Core::Element* fullscreen_box   = _root->GetElementById("fullscreen");
    Core::Element* screen_select    = _root->GetElementById("screen-select");
    Core::Element* language_select  = _root->GetElementById("language-select");
    vector<string> languages        = i18n::LanguagesAvailable();
    Data           current_language = OptionsManager::Get()["language"];

    {
      stringstream        rml;
      GraphicsPipe*       pipe   = framework.get_default_pipe();
      DisplayInformation* di     = pipe->get_display_information();
      int                 di_tot = di->get_total_display_modes();
      
      for (int i = 0 ; i < di_tot ; ++i)
      {
	rml << "<option value='" << i << "'>";
	rml << di->get_display_mode_width(i) << 'x' << di->get_display_mode_height(i) << ' ';
	if (di->get_display_mode_fullscreen_only(i))
	  rml << " (Fullscreen)";
	rml << "</option>";
      }
      screen_select->SetInnerRML(rml.str().c_str());
    }

    if (OptionsManager::Get()["screen"]["fullscreen"] == 1)
      fullscreen_box->SetAttribute("checked", "on");

    {
      stringstream rml;

      for_each(languages.begin(), languages.end(), [&rml](string language)
      {
	rml << "<option id='language-" << language << "' value='" << language << "'";
	rml << ">" << language << "</option>\n";
      });
      language_select->SetInnerRML(rml.str().c_str());
    }

    {
      Controls::ElementFormControlSelect* select    = dynamic_cast<Controls::ElementFormControlSelect*>(_root->GetElementById("language-select"));
      
      for (unsigned int i = 0 ; i < select->GetNumOptions() ; ++i)
      {
	Controls::SelectOption* option = select->GetOption(i);
	
	if (option)
	{
	  if (current_language.Value() == option->GetValue().CString())
	  {
	    select->SetSelection(i);
	    break ;
	  }
	}
      }
    }
    
    {
      Controls::ElementFormControlSelect* select = dynamic_cast<Controls::ElementFormControlSelect*>(screen_select);
      
      for (unsigned int i = 0 ; i < select->GetNumOptions() ; ++i)
      {
	Controls::SelectOption* option = select->GetOption(i);

	if (option)
	{
	  if (current_language.Value() == option->GetValue().CString())
	  {
	    select->SetSelection(i);
	    break ;
	  }
	}
      }
    }

    ToggleEventListener(true, "fullscreen",      "click",  FullscreenToggled);
    ToggleEventListener(true, "screen-select",   "change", ScreenSelected);
    ToggleEventListener(true, "language-select", "change", LanguageSelected);
    ToggleEventListener(true, "exit",            "click",  ExitClicked);
    ExitClicked.EventReceived.Connect      (*this, &UiBase::FireHide);
    FullscreenToggled.EventReceived.Connect(*this, &GameOptions::ToggleFullscreen);
    ScreenSelected.EventReceived.Connect   (*this, &GameOptions::SetResolution);
    LanguageSelected.EventReceived.Connect (*this, &GameOptions::SetLanguage);
    Translate();
  }
}

GameOptions::~GameOptions()
{
  ToggleEventListener(false, "fullscreen",      "click",  FullscreenToggled);
  ToggleEventListener(false, "screen-select",   "change", ScreenSelected);  
  ToggleEventListener(false, "language-select", "change", LanguageSelected);
  ToggleEventListener(false, "exit",            "click",  ExitClicked);
}

void GameOptions::ToggleFullscreen(Rocket::Core::Event& event)
{
  Core::Element* checkbox = event.GetCurrentElement();
  
  if (checkbox)
  {
    Core::Variant* var  = checkbox->GetAttribute("checked");
    Data           opts = OptionsManager::Get();
    
    opts["screen"]["fullscreen"] = (var ? 1 : 0);
    OptionsManager::Refresh();
  }
}

void GameOptions::SetResolution(Rocket::Core::Event&)
{
  Controls::ElementFormControlSelect* select    = dynamic_cast<Controls::ElementFormControlSelect*>(_root->GetElementById("screen-select"));
  int                                 it_select = select->GetSelection();
  Controls::SelectOption*             option    = select->GetOption(it_select);

  if (option)
  {
    stringstream        str_it;
    int                 it;
    Core::String        str    = option->GetValue();
    GraphicsPipe*       pipe   = framework.get_default_pipe();
    DisplayInformation* di     = pipe->get_display_information();
    
    str_it << str.CString();
    str_it >> it;
    if (it < di->get_total_display_modes())
    {
      Data opts = OptionsManager::Get();
      
      opts["screen"]["x"] = di->get_display_mode_width(it);
      opts["screen"]["y"] = di->get_display_mode_height(it);
      OptionsManager::Refresh();
    }
  }
}

void GameOptions::SetLanguage(Core::Event& event)
{
  Controls::ElementFormControlSelect* select    = dynamic_cast<Controls::ElementFormControlSelect*>(_root->GetElementById("language-select"));
  int                                 it_select = select->GetSelection();
  Controls::SelectOption*             option    = select->GetOption(it_select);
  
  if (option)
  {
    Core::String str  = option->GetValue();
    Data         opts = OptionsManager::Get();

    opts["language"] = str.CString();
    OptionsManager::Refresh();    
  }
}

/*
 * GameConsole
 */
#include "scriptengine.hpp"
#include <scripthelper/scripthelper.h>
#include <sstream>
GameConsole* GameConsole::GConsole= 0;
GameConsole::GameConsole(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  GConsole= this;

  if (Script::Engine::Get())
    _scriptContext = Script::Engine::Get()->CreateContext();
  else
  {
    _scriptContext = 0;
    cerr << "/!\\ [GameConsole] No Script::Engine" << endl;
  }

  _observerError = Script::Engine::ScriptError.Connect(*this, &GameConsole::Output);

  Core::ElementDocument* doc     = context->LoadDocument("data/console.rml");

  _root = doc;
  _input = doc->GetElementById("console_input");
  _history.push_back("LF();");
  _histIter= _history.end();
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* form  = doc->GetElementById("console_form");

    if (_input)
    {
      cout << "[UI] Console is ready" << endl;
      _input->AddEventListener("keyup",     &ConsoleKeyUp);
      ConsoleKeyUp.EventReceived.Connect(*this, &GameConsole::KeyUp);
      ExecuteEvent.EventReceived.Connect(*this, &GameConsole::Execute);
    }
    else
      cout << "[UI] No input for the console" << endl;
    Translate();
  }
}

GameConsole::~GameConsole()
{
  if (_scriptContext)
    _scriptContext->Release();
  Script::Engine::ScriptError.Disconnect(_observerError);
}

void GameConsole::Execute(Rocket::Core::Event&)
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_input);
  Rocket::Core::String string = control->GetValue();

  if (string != "")
  {
    _history.push_back(string.CString());
    _histIter= _history.end();

    if (_scriptContext)
    {
      stringstream stream;

      _currentLine = string.CString();
      int ret = ExecuteString(Script::Engine::Get(), _currentLine.c_str(), 0, _scriptContext);
      if (ret == asEXECUTION_ABORTED || ret == asEXECUTION_EXCEPTION)
      {
	stream << "<span class='console-as-error'>[AngelScript]</span> ";
        if (ret == asEXECUTION_ABORTED)
	  stream << "Execution Aborted";
	else if (ret == asEXECUTION_EXCEPTION)
	  stream << "NullPointer error";
        Output(stream.str());
      }
    }
    _currentLine = "";
    control->SetValue("");
  }
}

void GameConsole::WriteOn(const std::string& str)
{
  GConsole->Output("<span class='console-write'>[Write]</span><span class='console-write-output'> " + str + "</span>");
}

void GameConsole::Output(const std::string str)
{
  Rocket::Core::Element* element = _root->GetElementById("console_output");
  Rocket::Core::String   string;

  element->GetInnerRML(string);
  string  = (str + "<br />" + string.CString()).c_str();
  element->SetInnerRML(string);
}

void GameConsole::KeyUp(Rocket::Core::Event& event)
{
	if (event == "keyup") {
		Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_input);
		int keyId= event.GetParameter<int>("key_identifier", 0);

		if (keyId == Core::Input::KI_RETURN) {
			Execute(event);
		};

		if (!_history.empty()) {
			if (keyId == Core::Input::KI_DOWN) {
				if (_histIter!=_history.begin()) {
					_histIter--;
					control->SetValue( _histIter->c_str() );
				};
			};
			if (keyId == Core::Input::KI_UP) {
				if (_histIter!=_history.end()) {
					control->SetValue( _histIter->c_str() );
					_histIter++;
				} else {
					control->SetValue("");
				};
			};
		};
	}

	/*else if (event == "textinput") {
		Rocket::Core::word defWord;
		Rocket::Core::word valueKey = event.GetParameter<Rocket::Core::word>("data", defWord);
	}*/
}

//List all the functions registered inside AngelScript
void GameConsole::ListFunctions() {
	asIScriptEngine* engine = Script::Engine::Get();

	for (int i= 0; i<engine->GetGlobalFunctionCount(); i++)
		cout << "GLOBAL: " << engine->GetGlobalFunctionByIndex(i)->GetName() << endl;
	for (int i= 0; i<engine->GetFuncdefCount(); i++)
		cout << "FUNCDEF: " << engine->GetFuncdefByIndex(i)->GetName() << endl;
	for (int i= 0; i<engine->GetObjectTypeCount(); i++) {
		asIObjectType* obj = engine->GetObjectTypeByIndex(i);
		cout << "OBJ: " << obj->GetName() << endl;
		for (int j= 0; j<obj->GetMethodCount(); j++) 
			cout << "---->" << obj->GetMethodByIndex(j)->GetName() << endl;
	};
};

//Print out the entire scene graph into the console
void GameConsole::PrintScenegraph() {
	cout << "PRINTING SCENE GRAPH:" << endl;
	//render.ls();
	PrintChildren(GameConsole::Get()._window->get_render(),0);
	cout << endl;
};

void GameConsole::PrintChildren(const NodePath& n, int lvl) {
	for (int i= 0; i<lvl; i++)
		cout << "---";
	if (n.get_name()!="")
		cout << n.get_name() << endl;
	else
		cout << n << endl;

	if (n.get_num_children()!=0) {
		if (n.get_num_children()<100)
			for (int i= 0; i<n.get_num_children(); i++)
				PrintChildren( n.get_child(i), lvl+1 );
		else {
			for (int i= 0; i<lvl; i++)
				cout << "   ";
			cout << "(skipping past " << n.get_num_children() << " instances)\n";
		}
	}
	else {
		//cout << "{";
		//n.ls();
		//cout << "}";
	};
};
GameConsole& GameConsole::Get() {
	return *GConsole;
};

/*
 * GameInventory
 */
GameInventory::GameInventory(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/inventory.rml");

  _root = doc;
  if (doc)
  {
    Core::Element*         itemListContainer = doc->GetElementById("body-inventory-items");

    Core::ElementDocument* parentItems = doc;
    for (unsigned short i = 0 ; i < 200 ; ++i)
    {
      Rocket::Core::Element* item = parentItems->CreateElement("invitem");

      item->SetInnerRML("<img src=\"item.png\" class='inventory-item' />");
      itemListContainer->AppendChild(item);
    }
    
    ToggleEventListener(true, "button_use",       "click", ButtonUseClicked);
    ToggleEventListener(true, "button_drop",      "click", ButtonDropClicked);
    ToggleEventListener(true, "button_equip_1",   "click", ButtonEquip1Clicked);
    ToggleEventListener(true, "button_equip_2",   "click", ButtonEquip2Clicked);
    ToggleEventListener(true, "button_unequip_1", "click", ButtonUnequip1);
    ToggleEventListener(true, "button_unequip_2", "click", ButtonUnequip2);

    ButtonUseClicked.EventReceived.Connect   (*this, &GameInventory::CallbackButtonUse);
    ButtonDropClicked.EventReceived.Connect  (*this, &GameInventory::CallbackButtonDrop);
    ButtonEquip1Clicked.EventReceived.Connect(*this, &GameInventory::CallbackButtonEquip1);
    ButtonEquip2Clicked.EventReceived.Connect(*this, &GameInventory::CallbackButtonEquip2);
    
    ButtonUnequip1.EventReceived.Connect(*this, &GameInventory::CallbackButtonUnequip1);
    ButtonUnequip2.EventReceived.Connect(*this, &GameInventory::CallbackButtonUnequip2);

    Translate();
    doc->Show();
  }
  _inventoryView.ObjectSelected.Connect(*this, &GameInventory::SetSelectedObject);
  _selectedObject = 0;  
}

GameInventory::~GameInventory()
{
  ToggleEventListener(false, "button_use",       "click", ButtonUseClicked);
  ToggleEventListener(false, "button_drop",      "click", ButtonDropClicked);
  ToggleEventListener(false, "button_equip_1",   "click", ButtonEquip1Clicked);
  ToggleEventListener(false, "button_equip_2",   "click", ButtonEquip2Clicked);
  ToggleEventListener(false, "button_unequip_1", "click", ButtonUnequip1);
  ToggleEventListener(false, "button_unequip_2", "click", ButtonUnequip2);
}

void GameInventory::SetInventory(Inventory& inventory)
{
  Core::Element* itemListContainer = _root->GetElementById("body-inventory-items");

  if (itemListContainer)
  {
    _inventory = &inventory;
    _inventoryView.AddView(itemListContainer, *_inventory);
  }
  inventory.ContentChanged.Connect(*this, &GameInventory::UpdateInventory);
  UpdateInventoryCapacity();
}

void GameInventory::UpdateInventory(void)
{
  _inventoryView.Update();
  UpdateInventoryCapacity();
  SetSelectedObject(0);
}

void GameInventory::UpdateInventoryCapacity(void)
{
  Core::Element* capacity = _root->GetElementById("inventory-capacity");
  
  if (capacity)
  {
    stringstream stream;
    
    stream << "Weight: " << (int)(_inventory->GetCurrentWeight()) << " / " << (int)(_inventory->GetCapacity()) << "kg";
    capacity->SetInnerRML(stream.str().c_str());
  }
}

void GameInventory::SetSelectedObject(InventoryObject* inventory)
{
  Core::Element* itemDescription = _root->GetElementById("item-description");

  _selectedObject = inventory;
  if (itemDescription)
  {
    if (_selectedObject == 0)
      itemDescription->SetInnerRML("Seleccionar objeto");
    else
      itemDescription->SetInnerRML(_selectedObject->GetName().c_str());
  }
}

/*
 * GameMenu
 */
GameMenu::GameMenu(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/main_menu.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* elementWindow;
    Rocket::Core::Element* element;

    elementWindow = doc->GetElementById("content");

    ToggleEventListener(true, "continue", "click", _continueClicked);
    ToggleEventListener(true, "options",  "click", _optionsClicked);
    ToggleEventListener(true, "exit",     "click", _exitClicked);
    ToggleEventListener(true, "save",     "click", _saveClicked);
    ToggleEventListener(true, "load",     "click", _loadClicked);

    _continueClicked.EventReceived.Connect(*this, &GameMenu::MenuEventContinue);
    _exitClicked.EventReceived.Connect(ExitClicked, &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);
    _saveClicked.EventReceived.Connect(SaveClicked, &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);
    _loadClicked.EventReceived.Connect(LoadClicked, &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);
    _optionsClicked.EventReceived.Connect(OptionsClicked, &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);

    Translate();
  }
}

GameMenu::~GameMenu()
{
  ToggleEventListener(false, "continue", "click", _continueClicked);
  ToggleEventListener(false, "options",  "click", _optionsClicked);
  ToggleEventListener(false, "exit",     "click", _exitClicked);
  ToggleEventListener(false, "save",     "click", _saveClicked);
  ToggleEventListener(false, "load",     "click", _loadClicked);
}

/*
 * GameMainBar
 */
GameMainBar::GameMainBar(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  Rocket::Core::ElementDocument* doc = context->LoadDocument("data/main_bar.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* elementWindow;
    Rocket::Core::Element* element;

    elementWindow = doc->GetElementById("window");

    for (unsigned int i = 0 ; element = elementWindow->GetChild(i) ; ++i)
    {
      cout << "Element[" << i << "] tag: '" << element->GetTagName().CString() << "' id: " << element->GetId().CString() << endl;
    }
    
    if (!elementWindow)
    {
      cout << "Window element doesn't exist" << endl;
      return ;
    }

    element = doc->GetElementById("first_col");

    if (!element)
      cout << "first_col doesn't exist" << endl;

    ToggleEventListener(true, "menu",      "click", MenuButtonClicked);
    ToggleEventListener(true, "inv",       "click", InventoryButtonClicked);
    ToggleEventListener(true, "pipbuck",   "click", PipbuckButtonClicked);
    ToggleEventListener(true, "charsheet", "click", PersButtonClicked);
    ToggleEventListener(true, "equiped_1", "click", EquipedItem1Clicked);
    ToggleEventListener(true, "equiped_2", "click", EquipedItem2Clicked);
    
    _apEnabled = false;
    _apMax     = 0;
    
    EquipedItem1Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem1Clicked);
    EquipedItem2Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem2Clicked);
    CombatEndClicked.EventReceived.Connect(*this, &GameMainBar::CallbackCombatEndClicked);
    PassTurnClicked.EventReceived.Connect (*this, &GameMainBar::CallbackPassTurnClicked);
    
    Translate();
  }
}

GameMainBar::~GameMainBar()
{
  ToggleEventListener(false, "menu",       "click", MenuButtonClicked);
  ToggleEventListener(false, "inv",        "click", InventoryButtonClicked);
  ToggleEventListener(false, "pipbuck",    "click", PipbuckButtonClicked);
  ToggleEventListener(false, "charsheet",  "click", PersButtonClicked);
  ToggleEventListener(false, "equiped_1",  "click", EquipedItem1Clicked);
  ToggleEventListener(false, "equiped_2",  "click", EquipedItem2Clicked);
  ToggleEventListener(false, "pass_turn",  "click", PassTurnClicked);
  ToggleEventListener(false, "stop_fight", "click", CombatEndClicked);  
}

void GameMainBar::AppendToConsole(const std::string& str)
{
  Rocket::Core::Element* console = _root->GetElementById("console");
  
  if (console)
  {
    Rocket::Core::String rml;
    Rocket::Core::String toAdd;

    console->GetInnerRML(rml);
    toAdd  = "<li>- ";
    toAdd += str.c_str();
    toAdd += "</li><br />";
    console->SetInnerRML(toAdd + rml);
  }
}

void GameMainBar::SetCurrentHp(short hp)
{
  Rocket::Core::Element* elem_hp = _root->GetElementById("current-hp");
  
  if (elem_hp)
  {
    stringstream rml;

    rml << hp;
    elem_hp->SetInnerRML(rml.str().c_str());
  }
}

void GameMainBar::SetCurrentAc(short ac)
{
  Rocket::Core::Element* elem_ac = _root->GetElementById("current-ac");
  
  if (elem_ac)
  {
    stringstream rml;

    rml << ac;
    elem_ac->SetInnerRML(rml.str().c_str());
  }
}

void GameMainBar::SetCurrentAP(unsigned short ap, unsigned short max)
{
  SetMaxAP(max);
  if (_apEnabled)
  {
    Rocket::Core::Element* apbar = _root->GetElementById("action_points");
    string                 rml;

    if (apbar)
    {
      for (unsigned short i = 0  ; i < ap ; ++i)
	rml += "<img src='../textures/ap-active.png' /> ";
      for (unsigned short i = ap ; i < _apMax ; ++i)
	rml += "<img src='../textures/ap-inactive.png' /> ";
      apbar->SetInnerRML(rml.c_str());
    }
  }
}

void GameMainBar::SetMaxAP(unsigned short ap)
{
  Rocket::Core::Element* apbar = _root->GetElementById("action_points");
  string                 rml;

  _apMax = ap;
  if (apbar)
  {
    for (unsigned short i = 0 ; i < ap ; ++i)
      rml += "<img src='../textures/ap-inactive.png' /> ";
    apbar->SetInnerRML(rml.c_str());
  }
}

void GameMainBar::SetEnabledAP(bool enabled)
{
  _apEnabled = enabled;
  SetMaxAP(_apMax);
  
  Rocket::Core::Element* passTurn  = _root->GetElementById("pass_turn");
  Rocket::Core::Element* combatEnd = _root->GetElementById("stop_fight");

  if (enabled)
  {
    passTurn->AddEventListener("click", &PassTurnClicked);
    combatEnd->AddEventListener("click", &CombatEndClicked);  
  }
  else
  {
    passTurn->RemoveEventListener("click", &PassTurnClicked);
    combatEnd->RemoveEventListener("click", &CombatEndClicked);  
  }
}

#include "level/inventory.hpp"
void GameMainBar::CallbackEquipedItem1Clicked(Rocket::Core::Event& event)
{
  int button = 0;

  button = event.GetParameter<int>("button", button);
  if (button == 0)
    UseEquipedItem.Emit(0);
  else
    EquipedItemNextAction.Emit(0);
}

void GameMainBar::CallbackEquipedItem2Clicked(Rocket::Core::Event& event)
{
  int button = 0;

  button = event.GetParameter<int>("button", button);
  if (button == 0)
    UseEquipedItem.Emit(1);
  else
    EquipedItemNextAction.Emit(1);
}


void GameMainBar::SetEquipedItem(unsigned short it, InventoryObject* item)
{
  SetEquipedItemAction(it, item, 0);
}

void GameMainBar::SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt)
{
  if (item)
  {
    Rocket::Core::Element* elem;
    stringstream           stream;
    
    stream << "equiped_" << (it + 1);
    elem = _root->GetElementById(stream.str().c_str());
    if (elem)
    {
      stringstream rml;
      bool         actionExists = (*item)["actions"].Count() > actionIt;

      std::cout << "Action It is " << (int)actionIt << std::endl;
      if (actionExists) rml << "<p class='equiped_action'>" << (*item)["actions"][actionIt].Key() << "</p>";
      rml << "<p class='equiped_image'><img src='../textures/itemIcons/" << (*item)["icon"].Value() << "' /></p>";
      if (actionExists) rml << "<p class='equiped_apcost'>" << (*item)["actions"][actionIt]["ap-cost"].Value() << "AP</p>";
      elem->SetInnerRML(rml.str().c_str());
    }
  }
}

/*
 * UiBase
 */
void UiBase::RecursiveTranslate(Core::Element* root)
{
  unsigned short it;
  Core::Element* child;

  if (!root)
    return ;
  for (it = 0 ; (child = root->GetChild(it)) ; ++it)
  {
    Core::Variant* attr = child->GetAttribute("i18n");

    if (attr)
    {
      string key = attr->Get<Core::String>().CString();

      child->SetInnerRML(i18n::T(key).c_str());
    }
    else
      RecursiveTranslate(child);
  }
}

void UiBase::Translate(void)
{
  if (_root)
    RecursiveTranslate(_root);
}