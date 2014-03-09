#include "level/mouse_events.hpp"
#include "level/level.hpp"
#include "options.hpp"

using namespace std;

MouseEvents::MouseEvents(Level& level, WindowFramework* window): Mouse(window), level(level)
{
  mouse_state = MouseAction;
  ButtonLeft.Connect  (*this,             &MouseEvents::MouseLeftClicked);
  ButtonRight.Connect (*this,             &MouseEvents::MouseRightClicked);
  WheelUp.Connect     (*this,             &MouseEvents::MouseWheelUp);
  WheelDown.Connect   (*this,             &MouseEvents::MouseWheelDown);
  ButtonMiddle.Connect(level.GetCamera(), &SceneCamera::SwapCameraView);
}

bool MouseEvents::IsHoveringUi(void) const
{
  LevelUi& level_ui = level.GetLevelUi();
  
  return (level_ui.GetContext()->GetHoverElement() != level_ui.GetContext()->GetRootElement());
}

void MouseEvents::SetState(State state)
{
  if (state != mouse_state)
  {
    // Cleanup if needed
    switch (mouse_state)
    {
      case MouseTarget:
        TargetPicked.DisconnectAll();
        break ;
      case MouseWaypointPicker:
        WaypointPicked.DisconnectAll();
        break ;
      default:
        break ;
    }
    level.GetHoveredPath().Hide();
    mouse_state = state;

    if (level.GetState() == Level::Fight && mouse_state == MouseTarget && level.GetCombat().GetCurrentCharacter() == level.GetPlayer())
      level.GetTargetOutliner().EnableOutline();
    else
      level.GetTargetOutliner().DisableOutline();
  }
  SetCursorFromState();
}

void MouseEvents::SetCursorFromState(void)
{
  switch (mouse_state)
  {
    case MouseWaypointPicker:
    case MouseAction:
      SetMouseState('a');
      break ;
    case MouseInteraction:
      SetMouseState('i');
      break ;
    case MouseTarget:
      SetMouseState('t');
      break ;
  }
}

void MouseEvents::MouseWheelDown(void)
{
  if (!(IsHoveringUi()))
  {
    Data  options  = OptionsManager::Get();
    float distance = options["camera"]["distance"];

    if (distance < 140.f)
      distance += 10.f;
    options["camera"]["distance"] = distance;
    level.GetCamera().RefreshCameraHeight();
  }
}

void MouseEvents::MouseWheelUp(void)
{
  if (!(IsHoveringUi()))
  {
    Data  options  = OptionsManager::Get();
    float distance = options["camera"]["distance"];

    if (distance > 50.f)
      distance -= 10.f;
    options["camera"]["distance"] = distance;
    level.GetCamera().RefreshCameraHeight();
  }
}

void MouseEvents::MouseLeftClicked(void)
{
  const MouseHovering& hovering = Hovering();
  LevelUi&             level_ui = level.GetLevelUi();
  World*               world    = level.GetWorld();

  if (level.GetState() == Level::Interrupted)
    return ;
  if (level_ui.GetContext()->GetHoverElement() != level_ui.GetContext()->GetRootElement())
    return ;
  switch (mouse_state)
  {
    case MouseAction:
      if (level_ui.HasOpenedUi<LevelUi::UiItInteractMenu>())
        return ;
      else
      {
        ClosestWaypoint(level.GetWorld(), level.GetCurrentFloor());
        if (hovering.hasWaypoint)
        {
          Waypoint* toGo = world->GetWaypointFromNodePath(hovering.waypoint);

          if (toGo && level.GetPlayer() != 0)
            level.GetPlayer()->GoTo(toGo);
        }
      }
      break ;
    case MouseInteraction:
      if (hovering.hasDynObject)
      {
        InstanceDynamicObject* object = level.FindObjectFromNode(hovering.dynObject);

        level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
        if (object && object->GetInteractions().size() != 0)
        {
          level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
          level_ui.OpenInteractMenu(object);
          level.GetCamera().SetEnabledScroll(false);
        }
      }
      break ;
    case MouseTarget:
      std::cout << "Mouse Target" << std::endl;
      if (hovering.hasDynObject)
      {
        InstanceDynamicObject* dynObject = level.FindObjectFromNode(hovering.dynObject);

        std::cout << "HasDynObject" << std::endl;
        if (dynObject)
          TargetPicked.Emit(dynObject);
      }
      break ;
    case MouseWaypointPicker:
      {
        std::cout << "Mouse Waypoint Picker" << std::endl;
        ClosestWaypoint(world, level.GetFloors().GetCurrentFloor());
        if (hovering.hasWaypoint)
        {
          Waypoint* toGo = world->GetWaypointFromNodePath(hovering.waypoint);

          if (toGo)
            WaypointPicked.Emit(toGo);
        }
      }
      break ;
  }
}

void MouseEvents::MouseRightClicked(void)
{
  level.GetLevelUi().CloseRunningUi<LevelUi::UiItInteractMenu>();
  SetState(mouse_state == MouseInteraction || mouse_state == MouseTarget ? MouseAction : MouseInteraction);
}