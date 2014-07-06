
int iaStep = 0;

void main(Character@ self, float elapsedTime)
{
  if (self.IsMoving() == false)
  {
    MapPosition position = self.GetPosition();

    if (iaStep == 0)
      self.GoTo(position.get_x() + 5, position.get_y() + 10);
    else if (iaStep == 1)
      self.GoTo(position.get_x() + 5, position.get_y());
    else if (iaStep == 2)
      self.GoTo(position.get_x() - 6, position.get_y() - 6);
    else if (iaStep == 3)
      self.GoTo(position.get_x() - 4, position.get_y() - 4);
    iaStep++;
    if (iaStep == 4)
      iaStep = 0;
  }
}

