#include "objectsMain.as"

bool ProcessAttack(Item@ item, string action, string hit_sound, Character@ user, Character@ target)
{
  Data   item_data     = item.AsData();
  Data   action_data   = item_data["actions"][action];
  string message;
  bool   success       = false;
  string user_name     = user.GetName() == "self" ? "You" : user.GetName();

  if (UnarmedSuccessChance(item, user, target) >= Random() % 100)
  {
    int  critical_roll = Random() % 100;
    bool critical      = ComputeIfCritical(user, critical_roll);
    int  damage        = ComputeDamage(item, action, user, target, critical_roll);

    success            = true;
    message            = user_name + " hit " + target.GetName();
    if (critical)
      message         += " critically";
    message           += " for " + damage + " hit points";
    {
      int hit_points = target.GetHitPoints();

      if (hit_points > damage)
        target.SetHitPoints(hit_points - damage);
      else
        target.SetHitPoints(0);
    }
    level.PlaySound("hoof2hoof/" + hit_sound);
    Cout("ProcessAttack: damage: " + damage);
  }
  else
  {
    message = user_name + " tried to hit " + target.GetName() + " and missed.";
    level.PlaySound("hoof2hoof/miss");
  }
  level.AppendToConsole(message);
  return (success);
}

bool HoovesHit(Item@ item, Character@ user, Character@ target)
{
  Cout("Calling HoovesHit");
  if (@user == null || @target == null)
  {
    Cout("Error: called HoovesHit with user or target null");
    return (false);
  }
  return (ProcessAttack(item, "hooves", "punch", user, target));
}

bool BuckHit(Item@ item, Character@ user, Character@ target)
{
  bool success = false;//ProcessAttack(item, "buck", "whack", user, target);

  if (success)
  {
    // Implement K.Os
  }
  return (success);
}
