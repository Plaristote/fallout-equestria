#include "level/mouse/mouse_interaction_hint.hpp"
#include "level/level.hpp"
#include "mousecursor.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include "level/objects/character.hpp"
#include "level/mouse/mouse_waypoint_hint.hpp"

using namespace std;

MouseInteractionHint::MouseInteractionHint(Level& level): MouseTargetHint(level), level(level), player(0)
{
}

void MouseInteractionHint::SetInteractionCursor(const std::string& type)
{
  const string texture_path = "cursor-interaction-" + type;

  MouseCursor::Get()->SetCursorType(texture_path);
}

void MouseInteractionHint::SetHoveredObject(NodePath object_path)
{
  cout << "Debug1" << endl;
  InstanceDynamicObject* object = level.FindObjectFromNode(object_path);

  if (player && object && object->GetInteractions().size() > 0)
  {
    ObjectCharacter*       character = object->Get<ObjectCharacter>();

    if (character)
      SetHoveredCharacter(character);
    else
      SetHoveredObject(object);
  }
  else
    {
      cout << "Debug1.5" << endl;
    SetInteractionCursor("none");
    }
  cout << "Debug2" << endl;
}

void MouseInteractionHint::SetHoveredCharacter(ObjectCharacter* character)
{
  if (player == character || player->GetFieldOfView().IsDetected(character))
    SetInteractionCursor("available");
  else
    SetInteractionCursor("none");
}

void MouseInteractionHint::SetHoveredObject(InstanceDynamicObject* _object)
{
  float distance      = player->GetDistance(_object);
  float field_of_view = GetFieldOfViewRadius();

  if (distance <= field_of_view && _object->GetInteractions().size() > 0)
    SetInteractionCursor("available");
  else
    SetInteractionCursor("none");
}

float MouseInteractionHint::GetFieldOfViewRadius(void) const
{
  float radius = player->GetFieldOfView().GetRadius();

  return (radius);
}


void MouseInteractionHint::Run(float elapsed_time)
{
  MouseWaypointHint::Run(elapsed_time);
}
