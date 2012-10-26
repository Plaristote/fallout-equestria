
string HoovesHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap >= 3)
  {
    int damage = 5;

    user.SetActionPoints(ap - 3);
    target.SetHitPoints(target.GetHitPoints() - damage);
    
    Write("Putting buff on target");
    Data poison_buff = level.GetDataEngine()["poisoning"];
    Data to_send;
    to_send.Duplicate(poison_buff);
    to_send["it"]     = 0;
    to_send["max-it"] = 5;
    game.PushBuff(target, poison_buff);    
    
    return (user.GetName() + " hit " + target.GetName() + " for 5 hit points");
  }
  return ("Not enough Action Points");
}

string BuckHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap >= 4)
  {
    int damage = 10;

    user.SetActionPoints(ap - 4);
    target.SetHitPoints(target.GetHitPoints() - damage);
    
    Write("Putting buff on target");
    Data poison_buff = level.GetDataEngine()["poisoning"];
    game.PushBuff(target, poison_buff);
    
    return (user.GetName() + " hit " + target.GetName() + " for 10 hit points");
  }
  return ("Not enough Action Points");
}
