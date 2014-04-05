#ifndef  MOUSE_CURSOR_HPP
# define MOUSE_CURSOR_HPP

# include "ui/rocket_extension.hpp"

class MouseCursor : public UiBase
{
public:
  MouseCursor(WindowFramework* window, Rocket::Core::Context* context);
  ~MouseCursor() { _static = 0; }

  void SetCursorType(const std::string& type);
  void SetHint(const std::string& rml);
  void SetHint(int);

  static MouseCursor*           Get(void)        { return (_static); }

private:
  Rocket::Core::Element* _mouse;
  Rocket::Core::Element* _cursor;
  Rocket::Core::Element* _hint;
  std::string            _current_hint;
  static MouseCursor*    _static;
};

#endif
