#include "level/mouse/mouse_target_hint.hpp"
#include "level/level.hpp"
#include <mousecursor.hpp>

using namespace std;

MouseTargetHint::MouseTargetHint(Level& level) : MouseZoneHint(level), level(level)
{
  
}

void MouseTargetHint::SetHoveredObject(NodePath object_path)
{
  InstanceDynamicObject* object = level.FindObjectFromNode(object_path);
  
  if (object)
    SuccessRateHint(object);
  else
    MouseCursor::Get()->SetHint("");
}

void MouseTargetHint::SuccessRateHint(InstanceDynamicObject* object)
{
  ObjectCharacter*     player    = level.GetPlayer();
  InventoryObject*     item      = player->active_object;
  unsigned char        action_it = player->active_object_it;

  if ((*item)["actions"][action_it]["combat"] == "1")
    WeaponSuccessRate(player, object, item, action_it);
  else
    ItemSuccessRate  (player, object, item, action_it);
}

void MouseTargetHint::WeaponSuccessRate(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* weapon, unsigned char action_it)
{
  ObjectCharacter* target_character = target->Get<ObjectCharacter>();

  if (target_character)
  {
    int rate = weapon->HitSuccessRate(user, target_character, action_it);
    
    MouseCursor::Get()->SetHint(rate);
  }
  else
    MouseCursor::Get()->SetHint("");
}

void MouseTargetHint::ItemSuccessRate(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* weapon, unsigned char action_it)
{
  // Not implemented yet
  MouseCursor::Get()->SetHint("");
}
