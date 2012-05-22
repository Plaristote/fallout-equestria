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

  _mainBar->MenuButtonClicked.EventReceived.Connect(*this, &GameUi::OpenMenu);
  _mainBar->InventoryButtonClicked.EventReceived.Connect(*this, &GameUi::OpenInventory);
  _menu->Hide();
  _inventory->Hide();
  _console->Hide();

  _ih = new RocketInputHandler();
  window->get_mouse().attach_new_node(_ih);
  _rocket->set_input_handler(_ih);

  _window->enable_keyboard();
  framework.define_key("<", "ConsoleHandle", GameConsole::Toggle, (void*)_console);
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
  static bool inventoryShown = false;

  if (!inventoryShown)
    _inventory->Show();
  else
    _inventory->Hide();
  inventoryShown = !inventoryShown;
}

/*
 * GameConsole
 */
#include "scriptengine.hpp"
#include <scripthelper/scripthelper.h>
#include <sstream>
GameConsole::GameConsole(WindowFramework* window, Rocket::Core::Context* context) : _window(window)
{
  _scriptContext = Script::Engine::Get()->CreateContext();
  _observerError = Script::Engine::ScriptError.Connect(*this, &GameConsole::Output);
  
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");
  
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/console.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* input = doc->GetElementById("console_input");
    Rocket::Core::Element* form  = doc->GetElementById("console_form");

    if (input)
    {
      cout << "[UI] Console is ready" << endl;
      input->AddEventListener("textinput", &ConsoleKeyUp);
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
  Rocket::Core::Element*                element = _root->GetElementById("console_input");
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(element);
  Rocket::Core::String string = control->GetValue().CString();

  if (_scriptContext)
  {
    stringstream stream;

    _currentLine = string.CString();
    int ret = ExecuteString(Script::Engine::Get(), _currentLine.c_str(), 0, _scriptContext);
    stream << "<i>" << _currentLine << "</i>" << ": Returned value " << ret;
    Output(stream.str());
  }
  _currentLine = "";
  control->SetValue("");
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
  if (event == "keyup")
  {
    /*Rocket::Core::Input::KeyIdentifier keyId = Input::KeyIdentifier::KI_0;

    keyId = (event.GetParameter<Input::KeyIdentifier>("key_identifier", keyId));
    if (keyId == Input::KeyIdentifier::KI_RETURN)
    {
      cout << "Executing command: '" << _currentLine << "'" << endl;
      _currentLine = "";
    }

    Rocket::Controls::ElementFormControl* element = reinterpret_cast<Rocket::Controls::ElementFormControl*>(event.GetCurrentElement());
    
    Rocket::Core::String string = element->GetValue().CString();

    std::cout << "Value is : " << string.CString() << std::endl;*/
  }
  else if (event == "textinput")
  {
    Rocket::Core::word defWord;
    Rocket::Core::word valueKey = event.GetParameter<Rocket::Core::word>("data", defWord);

    if ((char)valueKey == ';')
      Execute(event);
  }
}

/*
 * GameInventory
 */
GameInventory::GameInventory(WindowFramework* window, Rocket::Core::Context* context) : _window(window)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/inventory.rml");

  _root = doc;
  if (doc)
  {
    //ElementDocument* docItemList       = context->LoadDocument("data/inventory-items.rml");
    Element*         itemListContainer = doc->GetElementById("body-inventory-items");

    //doc->AppendChild(docItemList);

    ElementDocument* parentItems = doc;
    for (unsigned short i = 0 ; i < 200 ; ++i)
    {
      Rocket::Core::Element* item = parentItems->CreateElement("invitem");

      item->SetInnerRML("<img src=\"item.png\" />");
      itemListContainer->AppendChild(item);
    }

    doc->Show();
    //docItemList->Show();
  }
}

/*
 * GameMenu
 */
GameMenu::GameMenu(WindowFramework* window, Rocket::Core::Context* context) : _window(window)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/main_menu.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* elementWindow;
    Rocket::Core::Element* element;

    elementWindow = MyRocket::GetChildren(doc,           "window");
    elementWindow = MyRocket::GetChildren(elementWindow, "content");

    Rocket::Core::Element* buttonContinue = MyRocket::GetChildren(elementWindow, "continue");
    Rocket::Core::Element* buttonOptions  = MyRocket::GetChildren(elementWindow, "options");
    Rocket::Core::Element* buttonExit     = MyRocket::GetChildren(elementWindow, "exit");

    if (buttonContinue) buttonContinue->AddEventListener("click", &_continueClicked);
    if (buttonOptions)  buttonOptions->AddEventListener("click",  &_optionsClicked);
    if (buttonExit)     buttonExit->AddEventListener("click",     &_exitClicked);

    _continueClicked.EventReceived.Connect(*this, &GameMenu::MenuEventContinue);
    _exitClicked.EventReceived.Connect(*this,     &GameMenu::MenuEventExit);
  }
}

void GameMenu::MenuEventExit(Rocket::Core::Event& event)
{
  framework.close_all_windows();
}

/*
 * GameMainBar
 */
GameMainBar::GameMainBar(WindowFramework* window, Rocket::Core::Context* context) : _window(window)
{
  Rocket::Core::ElementDocument* doc = context->LoadDocument("data/main_bar.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    Rocket::Core::Element* elementWindow;
    Rocket::Core::Element* element;

    elementWindow = MyRocket::GetChildren(doc, "window");

    for (unsigned int i = 0 ; element = elementWindow->GetChild(i) ; ++i)
    {
      cout << "Element[" << i << "] tag: '" << element->GetTagName().CString() << "' id: " << element->GetId().CString() << endl;
    }
    
    if (!elementWindow)
    {
      cout << "Window element doesn't exist" << endl;
      return ;
    }

    element = MyRocket::GetChildren(elementWindow, "content");
    element = MyRocket::GetChildren(element, "first_col");

    if (!element)
      cout << "first_col doesn't exist" << endl;

    for (unsigned int i = 0 ; element->GetChild(i) ; ++i)
    {
      cout << "Element[" << i << "] tag: '" << element->GetChild(i)->GetTagName().CString() << "' id: " << element->GetChild(i)->GetId().CString() << endl;
    }

    Rocket::Core::Element* button1 = MyRocket::GetChildren(element, "menu");
    Rocket::Core::Element* button2 = MyRocket::GetChildren(element, "inv");

    if (button1) button1->AddEventListener("click", &MenuButtonClicked);
    if (button2) button2->AddEventListener("click", &InventoryButtonClicked);

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