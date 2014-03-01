#ifndef  INTERACTIONS_INTERACTION_HPP
# define INTERACTIONS_INTERACTION_HPP

# include "globals.hpp"
# include "observatory.hpp"
# include <list>

class InstanceDynamicObject;

namespace Interactions
{
  typedef Sync::Signal<void (InstanceDynamicObject*)> Trigger;
  
  struct Interaction
  {
    Interaction(const std::string& name, InstanceDynamicObject* instance, Trigger* signal)
    : name(name), instance(instance), signal(signal) {}

    bool                   operator==(const std::string& name) const { return (this->name == name); }
    
    std::string            name;
    InstanceDynamicObject* instance;
    Trigger*               signal;
  };
  
  typedef std::list<Interaction> InteractionList;
}

#endif