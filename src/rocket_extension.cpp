#include "rocket_extension.hpp"

using namespace std;
using namespace Rocket;

UiBase::UiBase(WindowFramework* window, Rocket::Core::Context* context) : _window(window), _root(0), _context(context)
{
  _languageObs = i18n::LanguageChanged.Connect(*this, &UiBase::Translate);
}

UiBase::~UiBase()
{
  if (_root)
  {
    std::for_each(_listeners.begin(), _listeners.end(), [this](const Listener& listener)
    {
      Rocket::Core::Element* elem = _root->GetElementById(listener.elem.c_str());
      
      elem->RemoveEventListener(listener.event.c_str(), &(listener.instance));
    });
    _root->Close();
    _root->RemoveReference();
    _root = 0;
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

      if (toggle_on && it == _listeners.end())
      {
        element->AddEventListener(event.c_str(), &listener);
        _listeners.push_back(registered);
      }
      else if (!toggle_on)
      {
        element->RemoveEventListener(event.c_str(), &listener);
        if (it != _listeners.end())
          _listeners.erase(it);
      }
    }
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