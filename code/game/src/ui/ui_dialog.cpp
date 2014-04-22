#include "ui/ui_dialog.hpp"

using namespace std;

#include "executor.hpp"

UiDialog::UiDialog(WindowFramework* w, Rocket::Core::Context* c): UiBase(w, c)
{
  root = c->LoadDocument("data/ui_dialog.rml");
  if (root)
    _button_container = root->GetElementById("button-container");
  else
    _button_container = 0;
}

UiDialog::~UiDialog(void)
{
  if (root)
  {
    for_each(_buttons.begin(), _buttons.end(), [this](Button& button)
    {
      ToggleEventListener(false, button.id.c_str(), "click", button.listener);
    });
    _buttons.clear();
  }
}

void UiDialog::SetMessage(const string& message)
{
  if (root)
  {
    Rocket::Core::Element* container = root->GetElementById("message-container");

    container->SetAttribute("i18n", Rocket::Core::String(message.c_str()));
    container->SetInnerRML(i18n::T(message).c_str());
  }
}

void UiDialog::AddChoice(const string& name, std::function<void (Rocket::Core::Event&)> callback)
{
  if (_button_container)
  {
    Rocket::Core::String rml;
    stringstream         rml_stream;
    stringstream         id_stream;

    for_each(_buttons.begin(), _buttons.end(), [this](Button& button)
    {
      ToggleEventListener(false, button.id.c_str(), "click", button.listener);
    });
    _button_container->GetInnerRML(rml);
    id_stream  << "button-" << underscore(name);
    rml_stream << rml.CString();
    rml_stream << "<button id='" << id_stream.str() << "' class='universal_button' i18n='" << name << "'>";
    rml_stream << i18n::T(name) << "</button>";
    _button_container->SetInnerRML(rml_stream.str().c_str());
    _buttons.push_back(Button(id_stream.str()));
    _buttons.rbegin()->listener.EventReceived.Connect(callback);
    _buttons.rbegin()->listener.EventReceived.Connect(*this, &UiDialog::PickedChoice);
    for_each(_buttons.begin(), _buttons.end(), [this](Button& button)
    {
      ToggleEventListener(true, button.id.c_str(), "click", button.listener);
    });
  }
}

void UiDialog::PickedChoice(Rocket::Core::Event&)
{
  SetModal(false);
  Hide();
  Executor::ExecuteLater([this](void) { delete this; });
  Done.Emit();
}
