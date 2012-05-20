#include "gameui.hpp"

using namespace std;

extern PandaFramework framework;

/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window) : _window(window), _menu(window), _mainBar(window)
{
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
  Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");

  _mainBar.MenuButtonClicked.EventReceived.Connect(*this, &GameUi::OpenMenu);
  _menu.Hide();
}

void GameUi::OpenMenu(Rocket::Core::Event&)
{
  _menu.Show();
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
    
    element = MyRocket::GetChildren(element, "menu");

    if (!element)
      cout << "menu doesn't exisst" << endl;

    element->AddEventListener("click", &MenuButtonClicked);

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