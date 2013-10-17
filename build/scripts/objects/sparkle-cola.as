
bool DrinkCola(Item@ item, Character@ user)
{
  user.GetInventory().DelObject(item);
  level.PlaySound("use/sparkle-cola");
  return (true);
}
