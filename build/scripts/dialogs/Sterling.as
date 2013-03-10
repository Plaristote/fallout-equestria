
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

string TryTakeQuest()
{
  Data          data_engine   = level.GetDataEngine();
  QuestManager@ quest_manager = game.GetQuestManager();
  Data          quest         = data_engine["my_quests"]["Sterling's Quest"];

  quest_manager.AddQuest(quest);
  return ("kthxbye");
}

string StartFight()
{
  Character@ sterling = level.GetCharacter("Sterling");
  Character@ player   = level.GetPlayer();

  sterling.SetAsEnemy(player, true);
  //player.SetAsEnemy(sterling, true);
  return ("");
}