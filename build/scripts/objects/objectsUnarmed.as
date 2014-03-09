#include "objectsMain.as"

bool ProcessAttack(Item@ item, string action, string hit_sound, Character@ user, Character@ target)
{
  Data   item_data     = item.AsData();
  Data   action_data   = item_data["actions"][action];
  string message;
  bool   success       = false;

  if (UnarmedSuccessChance(item, user, target) >= Random() % 100)
  {
    Cout("Script Processed attack0");
    int  critical_roll = Random() % 100;
    bool critical      = ComputeIfCritical(user, critical_roll);
    Cout("Script Processed attack1");
    int  damage        = ComputeDamage(item, action, user, target, critical_roll);
    Cout("Script Processed attack2");

    success            = true;
    message            = user.GetName() + " hit " + target.GetName();
    Cout("Script Processed attack3");
    if (critical)
      message         += " critically";
    message           += " for " + damage + " hit points";
    target.SetHitPoints(target.GetHitPoints() - damage);
    Cout("Script Processed attack4");
    level.PlaySound("hoof2hoof/" + hit_sound);
    Cout("Script Processed attack5");
  }
  else
  {
    Cout("Script Processed attack1");
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
