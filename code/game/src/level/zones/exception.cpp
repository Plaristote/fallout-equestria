#include "level/zones/exception.hpp"
#include "ui/alert_ui.hpp"

using namespace std;
using namespace Zones;

const char* ZoneException::what() const throw()
{
  return (message.c_str());
}

void ZoneException::Display()
{
  AlertUi::NewAlert.Emit(message);
}
