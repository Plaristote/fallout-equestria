#include "ui/rocket_extension.hpp"
#include "mousecursor.hpp"

using namespace std;
using namespace Rocket;

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

UiBase::UiBase(WindowFramework* window, Rocket::Core::Context* context) : _window(window), _root(0), _context(context)
{
  root_outlives_this_object = false;
  _languageObs = i18n::LanguageChanged.Connect(*this, &UiBase::Translate);
}

UiBase::~UiBase()
{
  if (_root)
  {
    std::for_each(_listeners.begin(), _listeners.end(), [this](const Listener& listener)
    {
      Rocket::Core::Element* elem = _root->GetElementById(listener.elem.c_str());
      
      if (elem)
        elem->RemoveEventListener(listener.event.c_str(), &(listener.instance));
    });
    if (!root_outlives_this_object)
    {
      _root->Hide();
      _root->RemoveReference();
      _root = 0;
    }
  }
  i18n::LanguageChanged.Disconnect(_languageObs);
}

void UiBase::ToggleEventListener(bool toggle_on, const std::string& id, const std::string& event, RocketListener& listener)
{
  if (_root)
  {
    Rocket::Core::Element* element = _root->GetElementById(id.c_str());

    if (element)
    {
      Listener             registered(element, event, listener);
      auto                 it      = std::find(_listeners.begin(), _listeners.end(), registered);

      if (toggle_on)
      {
        if (it == _listeners.end())
          _listeners.push_back(registered);
        else
          element->RemoveEventListener(event.c_str(), &listener);
        element->AddEventListener(event.c_str(), &listener);
      }
      else
      {
        element->RemoveEventListener(event.c_str(), &listener);
        if (it != _listeners.end())
          _listeners.erase(it);
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
  if (_root)
    RecursiveTranslate(_root);
}

void UiBase::Show(void)
{
  if (_root)
  {
    _root->Show();
    VisibilityToggled.Emit(true);
    VisibilityToggledOn.Emit();
    if (MouseCursor::Get() != 0 && MouseCursor::Get() != this)
      MouseCursor::Get()->Show();
  }
}

void UiBase::Hide(void)
{
  if (_root)
  {
    _root->Hide();
    _root->PushToBack();
    VisibilityToggled.Emit(false);
    VisibilityToggledOff.Emit();
  }
}
bool UiBase::IsVisible(void) const
{
  return (_root && _root->IsVisible());
}

void UiBase::SetModal(bool modal)
{
  if (_root)
    _root->Show(modal ? Rocket::Core::ElementDocument::MODAL : Rocket::Core::ElementDocument::NONE);
}
