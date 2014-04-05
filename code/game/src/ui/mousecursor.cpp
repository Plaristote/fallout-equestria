#include "mousecursor.hpp"

using namespace std;

MouseCursor* MouseCursor::_static = 0;

MouseCursor::MouseCursor(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  cout << "[MouseCursor] Initializing" << endl;

  root   = context->LoadMouseCursor("data/cursor.rml");
  if (root)
  {
    _mouse  = root->GetElementById("mouse");
    _cursor = root->GetElementById("mouse-cursor");
    _hint   = root->GetElementById("mouse-hint");
  }
  else
    _mouse = _cursor = _hint;
  Show();
  _static = this;
  cout << "[MouseCursor] Finished initialization" << endl;
}

void MouseCursor::SetHint(const std::string& key)
{
  if (_current_hint != key)
  {
    if (key != "")
    {
      std::string rml = "<img src='../textures/mouse-hints/" + key + ".png' />";
      
      _hint->SetInnerRML(rml.c_str());
    }
    else
      _hint->SetInnerRML("");
    _current_hint = key;
  }
}

void MouseCursor::SetHint(int value)
{
  std::stringstream stream;
  std::string       str;
  
  stream << value << '%';
  str = stream.str();
  if (_current_hint != str)
  {
    _current_hint = str;
    str           = "<span class='mouse-hint-success-rate'>" + str + "</span>";
    _hint->SetInnerRML(str.c_str());
  }
}

void MouseCursor::SetCursorType(const std::string& type)
{
  std::string source = "cursors/" + type + ".png";

  _cursor->SetClassNames(type.c_str());
  _cursor->SetAttribute("src", Rocket::Core::String(source.c_str()));
}
