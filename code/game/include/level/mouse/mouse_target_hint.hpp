#ifndef  MOUSE_TARGET_HINT_HPP
# define MOUSE_TARGET_HINT_HPP

# include "mouse_zone_hint.hpp"

class Level;
class InstanceDynamicObject;
class ObjectCharacter;

class MouseTargetHint : public MouseZoneHint
{
public:
  MouseTargetHint(Level& level);
  
  void SetHoveredObject(NodePath);

private:
  void SuccessRateHint(InstanceDynamicObject*);
  void WeaponSuccessRate(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* weapon, unsigned char action_it);
  void ItemSuccessRate  (ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* weapon, unsigned char action_it);
  
  Level& level;
};

#endif