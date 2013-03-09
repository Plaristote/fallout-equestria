
string MakeDeal()
{
  return ("SuccessfulBusiness");
}

string HookInit()
{
  Data data_engine = level.GetDataEngine();
  Data have_talked = data_engine["dialogs"]["Sterling"]["happened"];

  if (have_talked.Nil())
  {
    have_talked = 1;
    return ("Howdy");
  }
  return ("WelcomeBack");
}
