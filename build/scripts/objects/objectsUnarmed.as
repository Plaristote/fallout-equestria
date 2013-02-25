
int GetDamage(Data action_stats, Character@ user, Character@ target)
{
  Data  user_stats   = user.GetStatistics();
  Data  target_stats = target.GetStatistics();
  float damage;
  float resistance   = target_stats["Statistics"]["Damage Resistance"].AsInt();

  damage = action_stats["damage"].AsInt();
  damage = damage + user_stats["Statistics"]["Bonus Damage"].AsInt();
  damage = damage - (damage / 100 * resistance);
  return (damage);
}

bool Test_IsCritical(Character@ user)
{
  int critical_chance = user.GetStatistics()["Statistics"]["Critical Chance"].AsInt();
  int random          = Random() % 100;

  return (critical_chance >= random);
}

bool Test_IsHit(Data item_stats, string type, Character@ user, Character@ target)
{
  Data  user_stats   = user.GetStatistics();
  Data  target_stats = target.GetStatistics();
  int   perception   = user_stats["Special"]["PER"].AsInt();
  int   skill        = user_stats["Skills"][type].AsInt();
  int   armor_class  = target_stats["Statistics"]["Armor Class"].AsInt();
  float distance     = user.GetDistance(target.AsObject());
  int   hit_chances;
  int   random       = Random() % 100;

  hit_chances = skill - 30 + (perception - 2) * 16 - (distance / 2) - armor_class;
  if (hit_chances < 5)
    hit_chances = 5;
  else if (hit_chances > 95)
    hit_chances = 95;
  item_stats["tmp"]  = hit_chances;
  item_stats["tmp2"] = random;
  return (hit_chances >= random);
}

string ProcessAttack(Item@ item, string action, string type, Character@ user, Character@ target)
{
  Data   item_data   = item.AsData();
  Data   action_data = item_data["actions"][action];
  string message;
  int    ap          = user.GetActionPoints();

  if (ap >= action_data["ap-cost"].AsInt())
  {
    user.SetActionPoints(ap - action_data["ap-cost"].AsInt());
    if (Test_IsHit(item_data, type, user, target))
    {
      int  damage   = GetDamage(action_data, user, target);
      bool critical = Test_IsCritical(user);

      if (critical)
        damage = damage * 5;
      item_data["tmp"] = damage;
      message = user.GetName() + " hit " + target.GetName();
      if (critical)
        message += " critically";
      message += " for " + item_data["tmp"].AsString() + " hit points";
      target.SetHitPoints(target.GetHitPoints() - damage);
      level.PlaySound("hoof2hoof/punch");
    }
    else
      message = user.GetName() + " attacked " + target.GetName() + " but missed.";
  }
  else
    message = "Not enough action points";
  return (message);
}

string HoovesHit(Item@ item, Character@ user, Character@ target)
{
  return (ProcessAttack(item, "hooves", "Unarmed", user, target));
}

string BuckHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap >= 4)
  {
    int damage = 10;

    user.SetActionPoints(ap - 4);
    target.SetHitPoints(target.GetHitPoints() - damage);

    level.PlaySound("hoof2hoof/whack");    

    Write("Putting buff on target");
    Data poison_buff = level.GetDataEngine()["poisoning"];
    game.PushBuff(target, poison_buff);
    
    return (user.GetName() + " hit " + target.GetName() + " for 10 hit points");
  }
  return ("Not enough Action Points");
}
