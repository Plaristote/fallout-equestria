#include "objectsMain.as"

bool ProcessAttack(Item@ item, string action, string hit_sound, Character@ user, Character@ target)
{
  Data   item_data     = item.AsData();
  Data   action_data   = item_data["actions"][action];
  string message;
  bool   success       = false;

  if (UnarmedSuccessChance(item, user, target) >= Random() % 100)
  {
    int  critical_roll = Random() % 100;
    bool critical      = ComputeIfCritical(user, critical_roll);
    int  damage        = ComputeDamage(item, action, user, target, critical_roll);

    success            = true;
    message            = user.GetName() + " hit " + target.GetName();
    if (critical)
      message         += " critically";
    message           += " for " + damage + " hit points";
    target.SetHitPoints(target.GetHitPoints() - damage);
    level.PlaySound("hoof2hoof/" + hit_sound);
  }
  else
  {
    message = user.GetName() + " tried to hit " + target.GetName() + " and missed.";
    level.PlaySound("hoof2hoof/miss");
  }
  level.AppendToConsole(message);
  return (success);
}

bool HoovesHit(Item@ item, Character@ user, Character@ target)
{
  return (ProcessAttack(item, "hooves", "punch", user, target));
}

bool BuckHit(Item@ item, Character@ user, Character@ target)
{
  bool success = ProcessAttack(item, "buck", "whack", user, target);

  if (success)
  {
    // Implement K.Os
  }
  return (success);
}
