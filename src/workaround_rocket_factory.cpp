#include <Rocket/Core/Factory.h>

using namespace Rocket::Core;

bool RocketFactoryInstanceElementText(Element* elem, String innerRml)
{
  return (Factory::InstanceElementText(elem, innerRml));
}