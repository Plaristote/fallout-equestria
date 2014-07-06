#include "general_pony.as"
#include "lib-combat-base.as"

bool talk_to(DynamicObject@ self, Character@ character)
{
  self.AddTextBox("Bzzt", 255, 255, 255, 2);
  return (false);
}

bool is_security_enabled()
{
  Data data_engine      = level.GetDataEngine();
  Data security_enabled = data_engine["dungeon"]["security-enabled"];

  return (security_enabled.NotNil() && security_enabled.AsInt() == 1);
}

void set_behavior_towards_player(Character@ self)
{
  bool security_enabled = is_security_enabled();

  if (security_enabled)
  {
    if (self.HasEnemiesInSight())
      level.StartFight(self);
  }
  self.SetAsEnemy(level.GetPlayer(), security_enabled);
}

void main(Character@ self, float elapsedTime)
{
  Data object_data        = self.GetDataStore();
  Data return_to_waypoint = object_data["return-to"];

  set_behavior_towards_player(self);
  if (return_to_waypoint.Nil())
    return_to_waypoint = self.GetCurrentWaypoint();
  if (!(self.IsMoving()))
  {
    if (is_security_enabled())
      self.GoTo(level.GetPlayer().AsObject(), 1);
    else if (self.GetCurrentWaypoint() != return_to_waypoint.AsInt())
      self.GoTo(return_to_waypoint.AsInt());
  }
}

void combat(Character@ self)
{
  default_combat_behavior(self);
}
