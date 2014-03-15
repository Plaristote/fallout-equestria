#include "mousecursor.hpp"

using namespace std;

MouseCursor* MouseCursor::_static = 0;

MouseCursor::MouseCursor(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  cout << "[MouseCursor] Initializing" << endl;
  root   = context->CreateDocument();
  if (root)
  {
    root->SetInnerRML("<img id='mouse-cursor' style='width:31px;height:32px' src='textures/cursor-interaction.png' /><span id='mouse-hint'></span>");
    _cursor = root->GetElementById("mouse-cursor");
    _hint   = root->GetElementById("mouse-hint");
    if (_cursor)
      _cursor->SetProperty("position", "absolute");
    if (_hint)
      _hint->SetProperty("position", "absolute");
  }
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
      std::string rml = "<img src='textures/mouse-hints/" + key + ".png' />";
      
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
    std::string css;
    
    css          += "font-family: JH_Fallout;";
    css          += "color: white;";
    css          += "hint-font-effect: outline;";
    css          += "hint-width: 2px;";
    css          += "hint-colour: black;";
    _current_hint = str;
    str           = "<span class='mouse-hint-success-rate' style='" + css + "'>" + str + "</span>";
    _hint->SetInnerRML(str.c_str());
  }
}

void MouseCursor::Update(void)
{
  if (_cursor && IsVisible() && window->get_graphics_window() != 0)
  {
    MouseData    pointer = window->get_graphics_window()->get_pointer(0);
    
    {
      stringstream strTop, strLeft;
      
      strLeft << ((int)pointer.get_x() + 1);
      strTop  << ((int)pointer.get_y() + 1);
      _cursor->SetProperty("top",  strTop.str().c_str());
      _cursor->SetProperty("left", strLeft.str().c_str());
    }
    {
      stringstream strTop, strLeft;
      
      strLeft << ((int)pointer.get_x() + 30);
      strTop  << ((int)pointer.get_y() + 30);
      _hint->SetProperty("top",  strTop.str().c_str());
      _hint->SetProperty("left", strLeft.str().c_str());
    }
    root->PullToFront();
  }
}
