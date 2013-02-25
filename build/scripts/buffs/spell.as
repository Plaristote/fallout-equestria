
bool shieldBegin(Character@ target, Character@ caster)
{
  Write("Shield Started");
  return (true);
}

void shieldEnd(Character@ target)
{
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
