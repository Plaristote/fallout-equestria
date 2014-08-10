#ifndef  DEBUGINFO_HPP
# define DEBUGINFO_HPP

# include <iostream>

class DebugInfo
{
public:
  DebugInfo() : enabled(true), null_stream(0)
  {
  }

  std::ostream& out(void)
  {
    return (enabled ? cout : null_stream);
  }

  void set_enabled(bool enabled)
  {
    this->enabled = enabled;
  }

private:
  bool         enabled;
  std::ostream null_stream;
};

#endif
