
string default_weapon_1()
{
  return ("hooves");
}

string default_weapon_2()
{
  return ("buck");
}

StringList GetAvailableApps(Data data)
{
  StringList apps;

  apps.Add("helloworld");
  apps.Add("clock");
  apps.Add("quest manager");
  return (apps);
}
