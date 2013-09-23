#include "objectsMain.as"

int GetDamage(Data action_stats, Character@ user, Character@ target)
{
  Data  user_stats   = user.GetStatistics();
  Data  target_stats = target.GetStatistics();
  float damage;
  float resistance   = target_stats["Statistics"]["Damage Resistance"].AsInt();

  damage = action_stats["damage"].AsInt() + user_stats["Statistics"]["Melee Damage"].AsInt();
  damage = damage + ((damage / 100.f) * user_stats["Statistics"]["Bonus Damage"].AsInt());
  damage = damage - (damage / 100 * resistance);
  return (damage);
}

bool Test_IsCritical(Character@ user)
{
  int critical_chance = user.GetStatistics()["Statistics"]["Critical Chance"].AsInt();
  int random          = Random() % 100;

  return (critical_chance >= random);
}

bool ProcessAttack(Item@ item, string action, string hit_sound, Character@ user, Character@ target)
{
  Data   item_data   = item.AsData();
  Data   action_data = item_data["actions"][action];
  string message;
  int    ap          = user.GetActionPoints();
  bool   success     = false;

  if (ap >= action_data["ap-cost"].AsInt())
  {
    user.SetActionPoints(ap - action_data["ap-cost"].AsInt());
    if (UnarmedSuccessChance(item, user, target) >= Random() % 100)
    {
      int  damage      = GetDamage(action_data, user, target);
      bool critical    = Test_IsCritical(user);

      success          = true;
      if (critical)
        damage         = damage * 5;
      item_data["tmp"] = damage;
      message          = user.GetName() + " hit " + target.GetName();
      if (critical)
        message       += " critically";
      message         += " for " + item_data["tmp"].AsString() + " hit points";
      target.SetHitPoints(target.GetHitPoints() - damage);
      level.PlaySound("hoof2hoof/" + hit_sound);
    }
    else
    {
      message = user.GetName() + " attacked " + target.GetName() + " but missed.";
      level.PlaySound("hoof2hoof/miss");
    }
  }
  else
    message = "Not enough action points";
  level.ConsoleWrite(message);
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
    Write("Putting buff on target");
    Data poison_buff = level.GetDataEngine()["poisoning"];
    game.PushBuff(target, poison_buff);
  }
  return (success);
}
