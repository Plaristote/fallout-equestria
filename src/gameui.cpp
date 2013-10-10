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
  string fonts[] = { "JH_FALLOUT.TTF", // Fallout Console Font
                     "fallout.ttf",    // Fallout Regular Font
                     "Delicious-Roman.otf",
                     "Delicious-Italic.otf",
                     "Delicious-Bold.otf",
                     "Delicious-BoldItalic.otf" };

  string fonts_name[] = { "", "Fallout", "", "", "", "" };

  _rocket = RocketRegion::make("interface", window->get_graphics_output());
  _rocket->set_active(true);
  Core::Initialise();
  for (unsigned int i=0; i<GET_ARRAY_SIZE(fonts); i++)
  {
    string path = string(PATH_FONTS) + "/" + fonts[i];
	
	cout << "[UI] Loading font '" << fonts_name[i] << "' " << path << endl;
    if (fonts_name[i] == "")
      Core::FontDatabase::LoadFontFace(path.c_str());
    else
      Core::FontDatabase::LoadFontFace(path.c_str(), fonts_name[i].c_str(), Core::Font::STYLE_NORMAL, Rocket::Core::Font::WEIGHT_NORMAL);
  }
  cout << "[UI] Fonts loaded" << endl;

  Rocket::Controls::Initialise();

  _ih      = new RocketInputHandler();
  cout << "[UI] Binding mouse and Rocket" << endl;
  window->get_mouse().attach_new_node(_ih);
  cout << "[UI] Set rocket input handler" << endl;
  _rocket->set_input_handler(_ih);
  cout << "[UI] Input handler initialized" << endl;

  _console = new GameConsole(window, _rocket->get_context());
  _console->Hide();
  cout << "[UI] GameConsole ready" << endl;
  
  _options = new GameOptions(window, _rocket->get_context());
  _options->Hide();
  cout << "[UI] GameOptions ready" << endl;

  _window->enable_keyboard();
  framework.define_key("tab", "ConsoleHandle", GameConsole::Toggle, (void*)_console);  
  cout << "[UI] Keyboard shortcut initialized" << endl;
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
  cout << "- Initializing Level Ui" << endl;
  _mainBar = new GameMainBar(window, _gameUi.GetRocketRegion()->get_context());

  _obs.Connect(_mainBar->MenuButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenMenu);
  _obs.Connect(_mainBar->InventoryButtonClicked.EventReceived, _gameUi,             &GameUi::OpenInventory);
  _obs.Connect(_mainBar->PersButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenPers);
  _obs.Connect(_mainBar->PipbuckButtonClicked.EventReceived,   _gameUi.OpenPipbuck, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
  _obs.Connect(_gameUi.GetInventory().VisibilityToggled, InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetMenu().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetPers().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  cout << "-> Done." << endl;
}

LevelUi::~LevelUi(void)
{
  _obs.DisconnectAll();
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
  done = false;
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::AppendText(const std::string& str)
{
  Core::Element* input = _root->GetElementById("content");
  Core::String   content;

  Wait();
  input->GetInnerRML(content);
  content = Core::String(str.c_str()) + "<br />" + content;
  input->SetInnerRML(content);
  Post();
  Refresh();
}

void LoadingScreen::Refresh(void)
{
  Wait();
  framework.get_graphics_engine()->render_frame();
  Post();
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
    if (!_data)
      _data = new DataTree;
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
      bool             hide_cursor   = data["debug"]["cursor"] == 0;
      unsigned int     screen_width  = screen["x"];
      unsigned int     screen_height = screen["y"];
      bool             fullscreen    = screen["fullscreen"] == 1;
      WindowProperties props         = framework.get_window(0)->get_graphics_window()->get_properties();

      props.set_cursor_hidden(hide_cursor);
      props.set_fullscreen(fullscreen);
      props.set_size(screen_width, screen_height);
      props.set_icon_filename("data/icon.ico");
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
      
      for (int i = 0 ; i < select->GetNumOptions() ; ++i)
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
      
      for (int i = 0 ; i < select->GetNumOptions() ; ++i)
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
      typedef std::pair<std::string, Controls::ElementFormControlInput*> RadioButton;
      RadioButton radios_yes[] = {
        RadioButton("focus-self",    dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-focus"))),
        RadioButton("focus-self",    dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-fight-focus"))),
        RadioButton("focus-enemies", dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-enemy-focus")))
      };
      RadioButton radios_no[] = {
        RadioButton("focus-self",    dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-free"))),
        RadioButton("focus-self",    dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-fight-free"))),
        RadioButton("focus-enemies", dynamic_cast<Controls::ElementFormControlInput*>(_root->GetElementById("camera-enemy-free")))
      };
      Data        options = OptionsManager::Get();

      for (unsigned int i = 0 ; i < 3 ; ++i)
      {
        RadioButton radio_true  = radios_yes[i];
        RadioButton radio_false = radios_no[i];
        Data        data        = (i == 0 ? options["camera"] : options["camera"]["fight"]);

        if (data[radio_true.first] == 1)
          radio_true.second->Click();
        else
          radio_false.second->Click();
      }
    }

    ToggleEventListener(true, "fullscreen",       "click",  FullscreenToggled);
    ToggleEventListener(true, "screen-select",    "change", ScreenSelected);
    ToggleEventListener(true, "language-select",  "change", LanguageSelected);
    ToggleEventListener(true, "graphics-quality", "change", QualitySelected);
    ToggleEventListener(true, "exit",             "click",  ExitClicked);

    std::string camera_options[] = { "camera-focus", "camera-free", "camera-fight-focus", "camera-fight-free", "camera-enemy-focus", "camera-enemy-free" };
    for (unsigned int i = 0 ; i < 6 ; ++i)
      ToggleEventListener(true, camera_options[i], "click", CameraFocusChanged);

    CameraFocusChanged.EventReceived.Connect([this](Rocket::Core::Event& event)
    {
      Data        options = OptionsManager::Get();
      std::string option  = event.GetCurrentElement()->GetId().CString();

      if (option == "camera-focus")
        options["camera"]["focus-self"] = 1;
      else if (option == "camera-free")
        options["camera"]["focus-self"] = 0;
      else if (option == "camera-fight-focus")
        options["camera"]["fight"]["focus-self"] = 1;
      else if (option == "camera-fight-free")
        options["camera"]["fight"]["focus-self"] = 0;
      else if (option == "camera-enemy-focus")
        options["camera"]["fight"]["focus-enemies"] = 1;
      else if (option == "camera-enemy-free")
        options["camera"]["fight"]["focus-enemies"] = 0;
      OptionsManager::Refresh();
    });

    ExitClicked.EventReceived.Connect      (*this, &UiBase::FireHide);
    FullscreenToggled.EventReceived.Connect(*this, &GameOptions::ToggleFullscreen);
    QualitySelected.EventReceived.Connect  (*this, &GameOptions::SetQuality);
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
  ToggleEventListener(false, "graphics-quality", "change", QualitySelected);
  ToggleEventListener(false, "exit",            "click",  ExitClicked);
  std::string camera_options[] = { "camera-focus", "camera-free", "camera-fight-focus", "camera-fight-free", "camera-enemy-focus", "camera-enemy-free" };
  for (unsigned int i = 0 ; i < 6 ; ++i)
    ToggleEventListener(false, camera_options[i], "click", CameraFocusChanged);
  
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

void GameOptions::SetQuality(Rocket::Core::Event&)
{
  Controls::ElementFormControlSelect* select    = dynamic_cast<Controls::ElementFormControlSelect*>(_root->GetElementById("graphics-quality"));
  int                                 it_select = select->GetSelection();
  Controls::SelectOption*             option    = select->GetOption(it_select);
  
  if (option)
  {
    Data        opts = OptionsManager::Get();
    std::string str  = option->GetValue().CString();

    opts["graphics-quality"] = str;
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
template<typename T>
void ArrayToJson(std::vector<T> array, std::string output)
{
  DataTree tmp;

  {
    Data     root(&tmp);
    Data     data = root["array"];

    for_each(array.begin(), array.end(), [data](const T& value)
    {
      Data node = data["new_value"];

      node = value;
      node.SetKey("");
    });
    DataTree::Writers::JSON(root, output);
  }
}

#include "scriptengine.hpp"
#include <scripthelper/scripthelper.h>
#include <sstream>
GameConsole* GameConsole::GConsole= 0;
GameConsole::GameConsole(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  cout << "Loading GameConsole" << endl;
  //
  // Load Script context
  //
  GConsole= this;

  if (Script::Engine::Get())
    _script_context = Script::Engine::Get()->CreateContext();
  else
  {
    _script_context = 0;
    cerr << "/!\\ [GameConsole] No Script::Engine" << endl;
  }

  _observerError = Script::Engine::ScriptError.Connect(*this, &GameConsole::Output);

  //
  // Load History
  //
  {
    DataTree* history_file = DataTree::Factory::JSON("data/history_console.json");
    if (history_file)
    {
      {
        Data   data_history(history_file);

        ForEach(data_history["array"], [this](Data entry)
        {
          _history.push_back(entry.Value());
        });
      }
      delete history_file;
    }
    _history.push_back("LF();");
    _histIter = _history.end();
    _histIter--;
  }

  //
  // Load Interface
  //
  Core::ElementDocument* doc     = context->LoadDocument("data/console.rml");

  _root = doc;
  _input = doc->GetElementById("console_input");
  if (doc)
  {
    doc->Show();
    if (_input)
    {
      cout << "[UI] Console is ready" << endl;
      ToggleEventListener(true, "console_input", "keyup", ConsoleKeyUp);
      ConsoleKeyUp.EventReceived.Connect(*this, &GameConsole::KeyUp);
    }
    else
      cout << "[UI] No input for the console" << endl;
    Translate();
  }
}

GameConsole::~GameConsole()
{
  if (_script_context)
    _script_context->Release();
  Script::Engine::ScriptError.Disconnect(_observerError);
  ToggleEventListener(false, "console_input", "keyup", ConsoleKeyUp);
}

void GameConsole::Execute(Rocket::Core::Event&)
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_input);
  Rocket::Core::String string = control->GetValue();

  if (string != "")
  {
    _history.push_back(string.CString());
    ArrayToJson(_history, "data/history_console.json");
    _histIter= _history.end();

    if (_script_context)
    {
      stringstream stream;

      _currentLine = string.CString();
      int ret = ExecuteString(Script::Engine::Get(), _currentLine.c_str(), 0, _script_context);
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
  cout << "Some event" << endl;
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
}

//List all the functions registered inside AngelScript
void GameConsole::ListFunctions() {
	asIScriptEngine* engine = Script::Engine::Get();

	for (unsigned int i= 0; i<engine->GetGlobalFunctionCount(); i++)
		cout << "GLOBAL: " << engine->GetGlobalFunctionByIndex(i)->GetName() << endl;
	for (unsigned int i= 0; i<engine->GetFuncdefCount(); i++)
		cout << "FUNCDEF: " << engine->GetFuncdefByIndex(i)->GetName() << endl;
	for (unsigned int i= 0; i<engine->GetObjectTypeCount(); i++) {
		asIObjectType* obj = engine->GetObjectTypeByIndex(i);
		cout << "OBJ: " << obj->GetName() << endl;
		for (unsigned int j= 0; j<obj->GetMethodCount(); j++) 
			cout << "---->" << obj->GetMethodByIndex(j)->GetName() << endl;
	}
}

//Print out the entire scene graph into the console
void GameConsole::PrintScenegraph() {
	cout << "PRINTING SCENE GRAPH:" << endl;
	//render.ls();
	PrintChildren(GameConsole::Get()._window->get_render(),0);
	cout << endl;
}

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
	}
}
GameConsole& GameConsole::Get() {
	return *GConsole;
}

/*
 * GameInventory
 */
void GameInventory::ListenEquipModes(bool activate)
{
  std::string equip_types[] = { "hoof", "battlesaddle", "magic" };

  for (unsigned short slot = 1 ; slot < 3 ; ++slot)
  {
    for (unsigned short i = 0 ; i < 3 ; ++i)
    {
      std::stringstream id;

      id << "eq" << slot << '-' << equip_types[i];
      cout << ">> Listen to equip mode " << id.str() << endl;
      ToggleEventListener(activate, id.str(), "click", ButtonEquipMode);
    }
  }
}

void GameInventory::ListenDropables(bool activate)
{
  Core::ElementList elements;

  _root->GetElementsByTagName(elements, "itemslot");
  std::for_each(elements.begin(), elements.end(), [this, activate](Rocket::Core::Element* element)
  {
    Rocket::Core::Element* to_listen = element->GetChild(0);

    if (to_listen)
    {
      if (activate)
        to_listen->AddEventListener("dragdrop", &DropEvent);
      else
        to_listen->RemoveEventListener("dragdrop", &DropEvent);
    }
  });
}

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
    ListenEquipModes(true);
    ListenDropables(true);
    ToggleEventListener(true, "body-inventory-items", "dragdrop", DropEvent);

    ButtonUseClicked.EventReceived.Connect   (*this, &GameInventory::CallbackButtonUse);
    ButtonDropClicked.EventReceived.Connect  (*this, &GameInventory::CallbackButtonDrop);
    ButtonEquipMode.EventReceived.Connect    (*this, &GameInventory::CallbackSwapEquipMode);
    DropEvent.EventReceived.Connect          (*this, &GameInventory::CallbackDropEvent);

    Translate();
    doc->Show();
  }
  _inventoryView.ObjectSelected.Connect(*this, &GameInventory::SetSelectedObject);
  _selectedObject = 0;
  _inventory      = 0;
}

GameInventory::~GameInventory()
{
  ToggleEventListener(false, "button_use",       "click", ButtonUseClicked);
  ToggleEventListener(false, "button_drop",      "click", ButtonDropClicked);
  ListenEquipModes(false);
  ListenDropables(false);
}

void GameInventory::CallbackDropEvent(Rocket::Core::Event& event)
{
  Rocket::Core::Element* drag_element = static_cast<Rocket::Core::Element*>(event.GetParameter< void* >("drag_element", NULL));
  Rocket::Core::Element* target       = event.GetTargetElement();
  Rocket::Core::Element* from         = drag_element;

  while (target && std::string(target->GetClassNames().CString()).find("drop-target") == std::string::npos)
    target = target->GetParentNode();
  while (from && std::string(from->GetClassNames().CString()).find("drop-target") == std::string::npos)
    from   = from->GetParentNode();
  if (drag_element && target && from)
  {
    std::stringstream    str_count;
    unsigned int         count;
    auto                 obj_it  = _inventory->GetContent().begin();
    auto                 obj_end = _inventory->GetContent().end();

    Rocket::Core::String tmp;
    drag_element->GetInnerRML(tmp);

    str_count << drag_element->GetId().CString();
    str_count >> count;
    std::advance(obj_it, count);
    if (obj_it != obj_end)
    {
      InventoryObject* object          = *obj_it;
      Core::Variant*   target_slot_var = target->GetAttribute("data-target");
      Core::Variant*   target_num_var  = target->GetAttribute("data-slot");
      Core::Variant*   from_slot_var   = from->GetAttribute("data-target");
      Core::Variant*   from_num_var    = from->GetAttribute("data-slot");

      if (from_slot_var)
      {
        unsigned int slot_num  = (from_num_var ? from_num_var->Get<unsigned int>() : 0);
        std::string  slot_name = from_slot_var->Get<Core::String>().CString();

        UnequipItem.Emit(slot_name, slot_num);
      }
      if (target_slot_var)
      {
        unsigned int slot_num  = (target_num_var ? target_num_var->Get<unsigned int>() : 0);
        std::string  slot_name = target_slot_var->Get<Core::String>().CString();

        EquipItem.Emit(slot_name, slot_num, object);
      }
    }
  }
}

void GameInventory::CallbackSwapEquipMode(Rocket::Core::Event& event)
{
  Rocket::Core::Element* element    = event.GetCurrentElement();
  std::string            id         = element->GetId().CString();
  unsigned short         slot       = id[2] - '0' - 1;
  std::string            equip_type = id.substr(4);
  EquipedMode            equip_mode = EquipedMouth;
  
  if (equip_type == "hoof")              { equip_mode = EquipedMouth;        }
  else if (equip_type == "battlesaddle") { equip_mode = EquipedBattleSaddle; }
  else if (equip_type == "magic")        { equip_mode = EquipedMagic;        }
  SwapEquipMode.Emit(slot, equip_mode);
}

void GameInventory::SetEquipedItem(unsigned short slot, InventoryObject* item)
{
  std::stringstream elem_id;

  elem_id << "eq" << (slot + 1) << "-equiped";
  if (_inventory)
  {
    Rocket::Core::Element* element = _root->GetElementById(elem_id.str().c_str());

    if (element)
    {
      std::stringstream rml;

      if (item)
      {
        int item_count = _inventory->GetObjectIterator(item);

        rml << "<p id='" << item_count << "' class='equiped_image";
#ifdef INVENTORY_USE_DRAGDROP
        if (!(item->IsHidden()))
          rml << " inventory-item-draggable";
#endif
        if ((*item)["icon"].Value() != "")
          rml << "'><img src='../textures/itemIcons/" << (*item)["icon"].Value() << "' />";
        rml << "</p>";
      }
      element->SetInnerRML(rml.str().c_str());
    }
  }
  else
    cout << "GameInventory: no inventory set" << endl;
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
    
    stream << "Weight: " << (int)(_inventory->GetCurrentWeight()) << " / " << (int)(_inventory->GetCapacity()) << " kg";
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

    ToggleEventListener(true, "continue", "click", _continueClicked);
    ToggleEventListener(true, "options",  "click", _optionsClicked);
    ToggleEventListener(true, "exit",     "click", _exitClicked);
    ToggleEventListener(true, "save",     "click", _saveClicked);
    ToggleEventListener(true, "load",     "click", _loadClicked);

    _continueClicked.EventReceived.Connect(*this, &GameMenu::MenuEventContinue);
    _exitClicked.EventReceived.Connect(ExitClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _saveClicked.EventReceived.Connect(SaveClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _loadClicked.EventReceived.Connect(LoadClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _optionsClicked.EventReceived.Connect(OptionsClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);

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
    ToggleEventListener(true, "menu",      "click",   MenuButtonClicked);
    ToggleEventListener(true, "inv",       "click",   InventoryButtonClicked);
    ToggleEventListener(true, "pipbuck",   "click",   PipbuckButtonClicked);
    ToggleEventListener(true, "charsheet", "click",   PersButtonClicked);
    ToggleEventListener(true, "equiped_1", "mouseup", EquipedItem1Clicked);
    ToggleEventListener(true, "equiped_2", "mouseup", EquipedItem2Clicked);
    ToggleEventListener(true, "skilldex",  "click",   SkilldexButtonClicked);
    ToggleEventListener(true, "spellbook", "click",   SpelldexButtonClicked);
    
    _apEnabled = false;
    _apMax     = 0;
    
    EquipedItem1Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem1Clicked);
    EquipedItem2Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem2Clicked);
    CombatEndClicked.EventReceived.Connect(*this, &GameMainBar::CallbackCombatEndClicked);
    PassTurnClicked.EventReceived.Connect (*this, &GameMainBar::CallbackPassTurnClicked);
    SkilldexButtonClicked.EventReceived.Connect([this](Rocket::Core::Event&) { OpenSkilldex.Emit(); });
    SpelldexButtonClicked.EventReceived.Connect([this](Rocket::Core::Event&) { OpenSpelldex.Emit(); });
    
    Translate();
  }
}

GameMainBar::~GameMainBar()
{
  ToggleEventListener(false, "menu",       "click",   MenuButtonClicked);
  ToggleEventListener(false, "inv",        "click",   InventoryButtonClicked);
  ToggleEventListener(false, "pipbuck",    "click",   PipbuckButtonClicked);
  ToggleEventListener(false, "charsheet",  "click",   PersButtonClicked);
  ToggleEventListener(false, "equiped_1",  "mouseup", EquipedItem1Clicked);
  ToggleEventListener(false, "equiped_2",  "mouseup", EquipedItem2Clicked);
  ToggleEventListener(false, "pass_turn",  "click",   PassTurnClicked);
  ToggleEventListener(false, "stop_fight", "click",   CombatEndClicked);
  ToggleEventListener(false, "skilldex",   "click",   SkilldexButtonClicked);
  ToggleEventListener(false, "spellbook",  "click",   SpelldexButtonClicked);
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
	rml += "<img class='img-ap'   height='20px' src='../textures/ap-active.png' /> ";
      for (unsigned short i = ap ; i < _apMax ; ++i)
	rml += "<img class='img-ap' height='20px' src='../textures/ap-inactive.png' /> ";
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
      rml += "<img class='img-ap' src='../textures/ap-inactive.png' /> ";
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
  else if (button == 2)
    EquipedItemNextAction.Emit(0);
}

void GameMainBar::CallbackEquipedItem2Clicked(Rocket::Core::Event& event)
{
  int button = 0;

  button = event.GetParameter<int>("button", button);
  if (button == 0)
    UseEquipedItem.Emit(1);
  else if (button == 2)
    EquipedItemNextAction.Emit(1);
}


void GameMainBar::SetEquipedItem(unsigned short it, InventoryObject* item)
{
  SetEquipedItemAction(it, item, 0);
}

void GameMainBar::SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt)
{
  cout << "SetEquipedItemAction: " << (int)actionIt << endl;
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

      if (actionExists) rml << "<p class='equiped_action'>" << (*item)["actions"][actionIt].Key() << "</p>";
      rml << "<p class='equiped_image'><img src='../textures/itemIcons/" << (*item)["icon"].Value() << "' /></p>";
      if (actionExists) rml << "<p class='equiped_apcost'>" << (*item)["actions"][actionIt]["ap-cost"].Value() << "AP</p>";
      elem->SetInnerRML(rml.str().c_str());
    }
  }
}

/*
 * AlertUi
 */
Sync::Signal<void (const string)> AlertUi::NewAlert;

AlertUi::AlertUi(WindowFramework* window, Core::Context* context, const string& message) : UiBase(window, context)
{
  _continue = true;
  _root     = context->LoadDocument("data/alert.rml");
  if (_root)
  {
    Core::Element* elem_message = _root->GetElementById("message");

    elem_message->SetInnerRML(message.c_str());
    ToggleEventListener(true, "button-ok", "click", ButtonClicked);
    ButtonClicked.EventReceived.Connect([this](Core::Event&) { _continue = false; });
  }
}

AlertUi::~AlertUi()
{
}

bool AlertUi::Run(void)
{
  if (_root)
  {
    _root->PullToFront();
  }
  return (_continue);
}
