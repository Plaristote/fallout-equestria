#ifndef  MOUSE_CURSOR_HPP
# define MOUSE_CURSOR_HPP

# include "rocket_extension.hpp"

class MouseCursor : public UiBase
{
public:
  MouseCursor(WindowFramework* window, Rocket::Core::Context* context);
  ~MouseCursor() { _static = 0; }
  void SetCursorTexture(const std::string& texture) { if (_cursor) _cursor->SetAttribute("src", texture.c_str()); }
  void Update(void);
  
  void PullToFront(void) { _root->PullToFront(); }
  
  static MouseCursor*    Get(void) { return (_static); }

private:
  Rocket::Core::Element* _cursor;
  static MouseCursor*    _static;
};

#endif