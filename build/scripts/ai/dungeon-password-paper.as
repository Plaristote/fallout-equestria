bool look_at(DynamicObject@ self, Character@ character)
{
  if (character.GetDistance(self) < 8)
    level.AppendToConsole("Somewhere on the piece of paper, someone scribbled the word 'FUCKINGPASSWORD'");
  else
    level.AppendToConsole("You see a piece of paper.");
  return (true);
}
