#include "ui/rocket_extension.hpp"
#include "mousecursor.hpp"

using namespace std;
using namespace Rocket;

Sync::Signal<void (bool)> UiBase::ToggleUserInterface;

namespace Rocket
{
  void ForeachElement(Rocket::Core::Element* root, const std::string& tag, std::function<void (Rocket::Core::Element*)> lambda)
  {
    if (root)
    {
      Rocket::Core::ElementList elements;

      root->GetElementsByTagName(elements, tag.c_str());
      std::for_each(elements.begin(), elements.end(), lambda);
    }
  }
}

UiBase::UiBase(WindowFramework* window, Rocket::Core::Context* context) : window(window), root(0), context(context)
{
  root_outlives_this_object = false;
  observers.Connect(i18n::LanguageChanged, *this, &UiBase::Translate);
  observers.Connect(ToggleUserInterface,   *this, &UiBase::UserInterfaceToggled);
  is_visible = false;
}

UiBase::~UiBase()
{
  if (root)
  {
    std::for_each(listeners.begin(), listeners.end(), [this](const Listener& listener)
    {
      Rocket::Core::Element* elem = root->GetElementById(listener.elem.c_str());
      
      if (elem)
        elem->RemoveEventListener(listener.event.c_str(), &(listener.instance));
    });
    if (!root_outlives_this_object)
    {
      root->Hide();
      root->RemoveReference();
      root = 0;
    }
  }
}

void UiBase::ToggleEventListener(bool toggle_on, const std::string& id, const std::string& event, RocketListener& listener)
{
  if (root)
  {
    Rocket::Core::Element* element = root->GetElementById(id.c_str());

    if (element)
    {
      Listener             registered(element, event, listener);
      auto                 it      = std::find(listeners.begin(), listeners.end(), registered);

      if (toggle_on)
      {
        if (it == listeners.end())
          listeners.push_back(registered);
        else
          element->RemoveEventListener(event.c_str(), &listener);
        element->AddEventListener(event.c_str(), &listener);
      }
      else
      {
        element->RemoveEventListener(event.c_str(), &listener);
        if (it != listeners.end())
          listeners.erase(it);
      }
    }
    else
      cout << "[WARNING] Element '" << id << "' doesn't exist." << endl;
  }
}

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
  if (root)
    RecursiveTranslate(root);
}

void UiBase::UserInterfaceToggled(bool toggle_on)
{
  if (root)
  {
    if (toggle_on && is_visible)
      root->Show();
    else
      root->Hide();
  }
}

void UiBase::Show(void)
{
  if (root)
  {
    is_visible = true;
    root->Show();
    VisibilityToggled.Emit(true);
    VisibilityToggledOn.Emit();
    if (MouseCursor::Get() != 0 && MouseCursor::Get() != this)
      MouseCursor::Get()->Show();
  }
}

void UiBase::Hide(void)
{
  if (root)
  {
    is_visible = false;
    root->Hide();
    root->PushToBack();
    VisibilityToggled.Emit(false);
    VisibilityToggledOff.Emit();
  }
}
bool UiBase::IsVisible(void) const
{
  return (root && root->IsVisible());
}

void UiBase::SetModal(bool modal)
{
  if (root)
    root->Show(modal ? Rocket::Core::ElementDocument::MODAL : Rocket::Core::ElementDocument::NONE);
}
