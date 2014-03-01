#include "level/interactions/action_drop_object.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"
#define AP_COST_USE 2

using namespace std;
using namespace Interactions;

Actions::DropObject::DropObject(ObjectCharacter* character, InventoryObject* item, ::Waypoint* drop_on): ActionRunner(character), item(item), drop_on(drop_on)
{
  SetActionPointCost(AP_COST_USE);
}

void Actions::DropObject::RunAction(void)
{
  ObjectCharacter* user          = GetUser();
  Level*           level         = GetLevel();
  World*           world         = level->GetWorld();
  DynamicObject*   item_graphics = item->CreateDynamicObject(world);
  
  if (item_graphics)
  {
    ObjectItem*    object_item;
    DynamicObject* item_dynamic_object;

    user->GetInventory().DelObject(item);
    object_item         = new ObjectItem(level, item_graphics, item);
    item_dynamic_object = object_item->GetDynamicObject();
    world->DynamicObjectChangeFloor(*item_dynamic_object, drop_on->floor);
    object_item->SetOccupiedWaypoint(drop_on);
    world->DynamicObjectSetWaypoint(*item_dynamic_object, *drop_on);
    item_dynamic_object->nodePath.set_pos(drop_on->nodePath.get_pos());
    level->InsertInstanceDynamicObject(object_item);
  }
}

ActionRunner* Actions::DropObject::Factory(ObjectCharacter* character, InventoryObject* object)
{
  ActionRunner* runner = 0;

  if (character && object)
  {
    ::Waypoint*   waypoint = character->GetOccupiedWaypoint();
    
    if (waypoint)
      runner = new DropObject(character, object, waypoint);
  }
  return (runner);
}