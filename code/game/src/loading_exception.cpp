#include "loading_exception.hpp"

using namespace std;

void LoadingException::Display(void)
{
  std::string message;

  if (name != "")
    message = "Failed to load '" + name + '\'';
  else
    message = "Failed to load.";
  message += ' ' + this->message;
  AlertUi::NewAlert.Emit(message);
}
