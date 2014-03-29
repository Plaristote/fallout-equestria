#ifndef  VISIBILITY_HALO_HPP
# define VISIBILITY_HALO_HPP

# include "globals.hpp"
# include "world/world.h"

class InstanceDynamicObject;

class VisibilityHalo
{
public:
  VisibilityHalo(void);
  ~VisibilityHalo(void);

  void                   Initialize(WindowFramework*, World*);
  void                   SetTarget(InstanceDynamicObject* t);
  static bool            IsObjectCuttable(const MapObject& object);
  
private:
  void                   MarkCuttableObjects(World*);

  InstanceDynamicObject* target;
  NodePath               halo;
};

#endif