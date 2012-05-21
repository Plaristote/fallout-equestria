#include "gameui.hpp"

using namespace std;
using namespace Rocket::Core;

extern PandaFramework framework;

/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window) : _window(window), _menu(window), _mainBar(window), _inventory(window)
{
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");

  _mainBar.MenuButtonClicked.EventReceived.Connect(*this, &GameUi::OpenMenu);
  _mainBar.InventoryButtonClicked.EventReceived.Connect(*this, &GameUi::OpenInventory);
  _menu.Hide();
  _inventory.Hide();
}

void GameUi::OpenMenu(Rocket::Core::Event&)
{
  _menu.Show();
}

void GameUi::OpenInventory(Rocket::Core::Event&)
{
  _inventory.Show();
}

/*
 * GameInventory
 */
#include <Rocket/Controls.h>
GameInventory::GameInventory(WindowFramework* window) : _window(window)
{
  _rocket = RocketRegion::make("rocketInventory", window->get_graphics_output());

  Rocket::Controls::Initialise();
  Rocket::Core::Context*         context = _rocket->get_context();
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/inventory.rml");

  if (doc)
  {
    ElementDocument* docItemList       = context->LoadDocument("data/inventory-items.rml");
    Element*         itemListContainer = docItemList->GetElementById("inventory-items");

    doc->AppendChild(docItemList);

    ElementDocument* parentItems = docItemList;
    for (unsigned short i = 0 ; i < 200 ; ++i)
    {
      Rocket::Core::Element* item = parentItems->CreateElement("invitem");

      item->SetInnerRML("<img src=\"item.png\" />");
      itemListContainer->AppendChild(item);
    }

    PT(RocketInputHandler) ih = new RocketInputHandler();
    window->get_mouse().attach_new_node(ih);
    _rocket->set_input_handler(ih);

    doc->Show();
    docItemList->Show();
  }
}

/*
 * GameMenu
 */
GameMenu::GameMenu(WindowFramework* window) : _window(window)
{
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");

  _rocket = RocketRegion::make("pandaRocket", window->get_graphics_output());

  Rocket::Core::Context*         context = _rocket->get_context();
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/main_menu.rml");

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

    PT(RocketInputHandler) ih = new RocketInputHandler();
    window->get_mouse().attach_new_node(ih);
    _rocket->set_input_handler(ih);
  }
}

void GameMenu::MenuEventExit(Rocket::Core::Event& event)
{
  framework.close_all_windows();
}

/*
 * GameMainBar
 */
GameMainBar::GameMainBar(WindowFramework* window) : _window(window)
{
  _rocket = RocketRegion::make("rocketMainBar", window->get_graphics_output());
  _rocket->set_active(true);

  Rocket::Core::Context* context = _rocket->get_context();

  Rocket::Core::ElementDocument* doc = context->LoadDocument("data/main_bar.rml");

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

    PT(RocketInputHandler) ih = new RocketInputHandler();
    window->get_mouse().attach_new_node(ih);
    _rocket->set_input_handler(ih);
  }
}