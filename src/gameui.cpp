#include "gameui.hpp"
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket::Core;

extern PandaFramework framework;


/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window) : _window(window)
{
  Rocket::Controls::Initialise();  
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");

  _rocket = RocketRegion::make("rocketConsole", window->get_graphics_output());
  _rocket->set_active(true);

  _console   = new GameConsole  (window, _rocket->get_context());
  _menu      = new GameMenu     (window, _rocket->get_context());
  _mainBar   = new GameMainBar  (window, _rocket->get_context());
  _inventory = new GameInventory(window, _rocket->get_context());
  _pers      = new GamePers     (window, _rocket->get_context());
  
  _inventory->VisibilityToggled.Connect(InterfaceOpened, &Observatory::Signal<void (bool)>::Emit);
  _menu->VisibilityToggled.Connect     (InterfaceOpened, &Observatory::Signal<void (bool)>::Emit);

  _mainBar->MenuButtonClicked.EventReceived.Connect     (*this, &GameUi::OpenMenu);
  _mainBar->InventoryButtonClicked.EventReceived.Connect(*this, &GameUi::OpenInventory);
  _mainBar->PersButtonClicked.EventReceived.Connect     (*this, &GameUi::OpenPers);
  _menu->Hide();
  _inventory->Hide();
  _console->Hide();
  _pers->Hide();

  _ih = new RocketInputHandler();
  window->get_mouse().attach_new_node(_ih);
  _rocket->set_input_handler(_ih);

  _window->enable_keyboard();
  framework.define_key("tab", "ConsoleHandle", GameConsole::Toggle, (void*)_console);
}

GameUi::~GameUi()
{
  delete _console;
  delete _menu;
  delete _mainBar;
  delete _inventory;
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
 * GameConsole
 */
#include "scriptengine.hpp"
#include <scripthelper/scripthelper.h>
#include <sstream>
GameConsole* GameConsole::GConsole= 0;
GameConsole::GameConsole(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
{
	GConsole= this;

  _scriptContext = Script::Engine::Get()->CreateContext();
  _observerError = Script::Engine::ScriptError.Connect(*this, &GameConsole::Output);
  
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");
  
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/console.rml");

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

		if (keyId == Input::KI_RETURN) {
			Execute(event);
		};

		if (!_history.empty()) {
			if (keyId == Input::KI_DOWN) {
				if (_histIter!=_history.begin()) {
					_histIter--;
					control->SetValue( _histIter->c_str() );
				};
			};
			if (keyId == Input::KI_UP) {
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
GameInventory::GameInventory(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/inventory.rml");

  _root = doc;
  if (doc)
  {
    Element*         itemListContainer = doc->GetElementById("body-inventory-items");

    ElementDocument* parentItems = doc;
    for (unsigned short i = 0 ; i < 200 ; ++i)
    {
      Rocket::Core::Element* item = parentItems->CreateElement("invitem");

      item->SetInnerRML("<img src=\"item.png\" class='inventory-item' />");
      itemListContainer->AppendChild(item);
    }

    doc->Show();
  }
}

/*
 * GameMenu
 */
GameMenu::GameMenu(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/main_menu.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* elementWindow;
    Rocket::Core::Element* element;

    elementWindow = doc->GetElementById("content");

    Rocket::Core::Element* buttonContinue = elementWindow->GetElementById("continue");
    Rocket::Core::Element* buttonOptions  = elementWindow->GetElementById("options");
    Rocket::Core::Element* buttonExit     = elementWindow->GetElementById("exit");

    if (buttonContinue) buttonContinue->AddEventListener("click", &_continueClicked);
    if (buttonOptions)  buttonOptions->AddEventListener ("click", &_optionsClicked);
    if (buttonExit)     buttonExit->AddEventListener    ("click", &_exitClicked);

    _continueClicked.EventReceived.Connect(*this, &GameMenu::MenuEventContinue);
    _exitClicked.EventReceived.Connect(*this,     &GameMenu::MenuEventExit);
  }
}

void GameMenu::MenuEventExit(Rocket::Core::Event& event)
{
  framework.set_exit_flag();
}

/*
 * GamePers
 */
GamePers::GamePers(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
{
  Rocket::Core::ElementDocument* doc = context->LoadDocument("data/charsheet.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* button_continue = doc->GetElementById("continue");
    Rocket::Core::Element* button_cancel   = doc->GetElementById("cancel");

    if (button_continue) button_continue->AddEventListener("click", &DoneButton);
    if (button_cancel)   button_cancel->AddEventListener  ("click", &CancelButton);

    DoneButton.EventReceived.Connect  (*this, &GamePers::Close);
    CancelButton.EventReceived.Connect(*this, &GamePers::Close);
  }
}

/*
 * GameMainBar
 */
GameMainBar::GameMainBar(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
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

    Rocket::Core::Element* button1 = doc->GetElementById("menu");
    Rocket::Core::Element* button2 = doc->GetElementById("inv");
    Rocket::Core::Element* button3 = doc->GetElementById("charsheet");

    if (button1) button1->AddEventListener("click", &MenuButtonClicked);
    if (button2) button2->AddEventListener("click", &InventoryButtonClicked);
    if (button3) button3->AddEventListener("click", &PersButtonClicked);

    /*const Rocket::Core::Property* property = elementWindow->GetProperty("height");

    cout << "Property bottom " << property->ToString().CString() << endl;*/


    /*std::stringstream stream;
    stream << (_window->get_graphics_window()->get_y_size() - 140.f);
    elementWindow->SetProperty("bottom", stream.str().c_str());
    //elementWindow->SetAttribute("bottom", _window->get_graphics_window()->get_y_size() - 140.f);

    elementWindow = MyRocket::GetChildren(elementWindow, "content");

    cout << "Num children" << elementWindow->GetNumChildren() << endl;

    for (unsigned int i = 0 ; element = elementWindow->GetChild(i) ; ++i)
    {
      cout << "Element[" << i << "] id: " << element->GetId().CString() << endl;
    }*/
  }
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
    console->SetInnerRML(rml + toAdd);
  }
}