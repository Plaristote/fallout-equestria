#ifndef  LOADING_EXCEPTION_HPP
# define LOADING_EXCEPTION_HPP

# include "globals.hpp"
# include <string>
# include <exception>
# include <ui/alert_ui.hpp>

class LoadingException : public std::exception
{
public:
  LoadingException(const std::string& message) : message(message)
  {
  }

  void SetName(const std::string& name)
  {
    this->name = name;
  }

  void Display(void);

  const char* what(void) const throw() { return (message.c_str()); }

private:
  const std::string message;
  std::string       name;
};

#endif