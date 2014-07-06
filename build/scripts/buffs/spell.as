
bool shieldBegin(Character@ target, Character@ caster)
{
  Special@ target_stats = target.GetStatController();
  Data     statistics   = target_stats.GetData();
  int      damage_resistance = statistics["Statistics"]["Damage Resistance"].AsInt();

  statistics["Statistics"] = damage_resistance + 25;
  Write("Shield Started");
  target.GetStatController();
  return (true);
}

void shieldEnd(Character@ target)
{
  Special@ target_stats = target.GetStatController();
  Data     statistics   = target_stats.GetData();
  int      damage_resistance = statistics["Statistics"]["Damage Resistance"].AsInt();

  statistics["Statistics"] = damage_resistance - 25;
  Write("Shield Ended");
}

bool execute_poison(Data buff, Special@ special)
{
  Data stats = special.GetData();
  Data it    = buff["it"];
  int  hp    = stats["Variables"]["Hit Points"].AsInt();
  int  i     = 0;

  if (it.NotNil())
    i = buff["it"].AsInt();
  hp -= buff["poison-damage"].AsInt();
  special.SetCurrentHp(hp);
  return (buff["it"].AsInt() != buff["max-it"].AsInt());
}

void CastShield(Character@ user, Character@ target)
{
  Data dataEngine = level.GetDataEngine();
  Data buffData   = dataEngine["shielding spell"];

  buffData["graphics"]["model"]          = "misc/sphere.egg.pz";
  buffData["graphics"]["scale"]          = 5;
  buffData["graphics"]["color"]["red"]   = 50;
  buffData["graphics"]["color"]["green"] = 50;
  buffData["graphics"]["color"]["blue"]  = 200;
  buffData["graphics"]["color"]["alpha"] = 0.5;
  buffData["duration"] = 30;

  buffData["script"]["source"]    = "spell.as";
  buffData["script"]["hookBegin"] = "shieldBegin";
  buffData["script"]["hookEnd"]   = "shieldEnd";
  user.PushBuff(buffData, target);
}
