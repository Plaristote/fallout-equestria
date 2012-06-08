
string UseKeyOnDoor(Item@ item, Character@ user, Door@ door)
{
  if (door.IsOpen() && door.IsLocked() == false)
    return ("Close the door first");
  if (door.GetKeyName() == item.GetName())
  {
    string postStr = (door.IsLocked() ? " unlocked a door" : " locked a door");

    door.Unlock();
    return (user.GetName() + postStr);
  }
  return ("The door won't open");
}
