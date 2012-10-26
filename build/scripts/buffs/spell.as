
bool shieldBegin(Character@ target, Character@ caster)
{
  Write("Shield Started");
  return (true);
}

void shieldEnd(Character@ target)
{
  Write("Shield Ended");
}

bool execute_poison(Data buff, Data stats)
{
  Write("Executing Poison");
  Data it = buff["it"];
  int  hp = stats["Variables"]["Hit Points"].AsInt();
  int  i  = 0;

  if (it.NotNil())
    i = buff["it"].AsInt();
  hp -= buff["poison-damage"].AsInt();
  stats["Variables"]["Hit Points"] = hp;
  Write(buff["it"].AsString());
  Write(buff["max-it"].AsString());
  if (buff["it"].AsInt() != buff["max-it"].AsInt())
    Write("This aint the last time youll hear about me");
  else
    Write("NOOOOOOOOOOOOOOOOOOOOOOOOO");
  return (buff["it"].AsInt() != buff["max-it"].AsInt());
}