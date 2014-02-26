#include "level/mouse_hint.hpp"
#include "level/level.hpp"
#include "mousecursor.hpp"

using namespace std;

void MouseHint::Run(void)
{
  MouseEvents& mouse = level.GetMouse();
  
  if (!(mouse.IsHoveringUi()))
  {
    switch (mouse.GetState())
    {
      case MouseEvents::MouseTarget:
        TargetHint();
        break ;
      case MouseEvents::MouseAction:
        ActionHint();
        break ;
      default:
        ClearHint();
    }
  }
  else
    ClearHint();
}

void MouseHint::TargetHint()
{
  if (mouse.Hovering().hasDynObject)
    SuccessRateHint();
  else
    ClearHint();
}

void MouseHint::ActionHint()
{
  mouse.ClosestWaypoint(level.GetWorld(), level.GetCurrentFloor());
  if (mouse.Hovering().hasWaypoint)
    ExitZoneHint();
  else
    NoPathHint();
}

void MouseHint::ExitZoneHint()
{
  unsigned int waypoint_id = mouse.Hovering().waypoint_ptr->id;
  World*       world       = level.GetWorld();

  if (world->IsInExitZone(waypoint_id))
    MouseCursor::Get()->SetHint("exit");
  else
    ClearHint();
}

void MouseHint::NoPathHint()
{
  MouseCursor::Get()->SetHint("nowhere");
}

void MouseHint::ClearHint()
{
  MouseCursor::Get()->SetHint("");
}

void MouseHint::SuccessRateHint(void)
{
  InstanceDynamicObject* dynObject = level.FindObjectFromNode(mouse.Hovering().dynObject);

  if (dynObject)
  {
    ObjectCharacter*     player    = level.GetPlayer();
    InventoryObject*     item      = player->active_object;
    unsigned char        actionIt  = player->active_object_it;

    if ((*item)["actions"][actionIt]["combat"] == "1")
    {
      ObjectCharacter*   target = dynObject->Get<ObjectCharacter>();
      int                rate;

      if (!target)
        return ;
      rate = item->HitSuccessRate(player, target, actionIt);
      MouseCursor::Get()->SetHint(rate);
    }
    else
      ; // Not implemented yet
  }
  else
    ClearHint();
}
