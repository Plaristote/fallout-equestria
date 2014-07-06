#include "objectsMain.as"

bool CanWeild(Item@, Character@, string slot, int mode)
{
  if (slot == "equiped")
    return (mode == 0 || mode == 1);
  return (false);
}
