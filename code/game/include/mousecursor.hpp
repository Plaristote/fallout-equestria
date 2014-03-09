#ifndef  MOUSE_CURSOR_HPP
# define MOUSE_CURSOR_HPP

# include "ui/rocket_extension.hpp"

class MouseCursor : public UiBase
{
public:
  MouseCursor(WindowFramework* window, Rocket::Core::Context* context);
  ~MouseCursor() { _static = 0; }

  void SetCursorTexture(const std::string& texture) { if (_cursor) _cursor->SetAttribute("src", texture.c_str()); }
  void SetHint(const std::string& rml);
  void SetHint(int);
  
  void Update(void);
  
  void PullToFront(void) { root->PullToFront(); }
  
  static MouseCursor*    Get(void) { return (_static); }

private:
  Rocket::Core::Element* _cursor;
  Rocket::Core::Element* _hint;
  std::string            _current_hint;
  static MouseCursor*    _static;
};

#endif
