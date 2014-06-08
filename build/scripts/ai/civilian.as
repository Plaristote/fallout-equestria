#include "general_pony.as"

bool cares_about_accuracy(Character@ self)
{
  return (true);
}

void main(Character@ self, float elapsedTime)
{
  Data       data_engine = level.GetDataEngine();
  Character@ player      = level.GetPlayer();

  if (data_engine["variables"]["Sterling"]["allied"].AsInt() == 1)
    ai_template_follow_char(self, player, elapsedTime);
}

Character@ currentTarget = null;

bool       Fleeing(Character@ self)
{
  CharacterList enemies = self.GetNearbyEnemies();

  if (enemies.Size() > 0)
  {
    Character@ other = enemies[0];
    
    self.MoveAwayFrom(other.AsObject());
    return (self.IsMoving());
  }
  return (false);
}

Character@ SelectTarget(Character@ self)
{
  ia_debug("Civilian Select Target");
  CharacterList         enemies  = self.GetNearbyEnemies();
  int                   nEnemies = enemies.Size();
  int                   it       = 0;
  Character@            bestMatch;

  while (it < nEnemies)
  {
    Character@ current = enemies[it];

    if (@bestMatch == null || current.GetHitPoints() < bestMatch.GetHitPoints())
      @bestMatch = @current;
    it++;
  }
  if (@bestMatch != null)
  {
    ia_debug("-> Selected enemy: " + bestMatch.GetName());
    Write("-> Selected enemy: " + bestMatch.GetName());
  }
  else
  {
    ia_debug("-> No enemy detected");
  }
  return (bestMatch);
}

int get_available_ammo_for(Character@ self, Item@ item)
{
  Data ammo           = item.AsData()["ammo"];
  Data current_ammo   = ammo["current"];
  int  it             = 0;
  int  available_ammo = 0;

  if (current_ammo.Nil())
    current_ammo   = ammo["types"][0];
  if (current_ammo.NotNil())
    available_ammo = self.GetInventory().ContainsHowMany(current_ammo.AsString());
  while (available_ammo == 0 && it < ammo["types"].Count())
  {
    Data ammo      = ammo["types"][it];

    available_ammo = self.GetInventory().ContainsHowMany(current_ammo.AsString());
    it += 1;
  }
  return (available_ammo);
}

bool is_weapon_loaded(Item@ item)
{
  Data ammo = item.AsData()["ammo"];
  
  if (ammo["types"].NotNil())
  {
    Data amount = ammo["amount"];

    return (amount.NotNil() && amount.AsInt() > 0);
  }
  return (true);
}

int    max_perform_action(Character@ self, Item@ item, int action_point_cost)
{
  int    action_points         = self.GetActionPoints();
  bool   has_max_action_points = action_points == self.GetMaxActionPoints();

  if (!is_weapon_loaded(item))
  {
    if (get_available_ammo_for(self, item) > 0)
    {
      action_points        -= 2;
      has_max_action_points = false;
    }
    else
      return (0);
  }
  if (!has_max_action_points)
    return (action_points / action_point_cost);
  return (1);
}

int    max_damage_action(Character@ self, Item@ item, string action_name)
{
  int  action_cost = item.GetActionPointCost(self, action_name);
  int  max_uses    = max_perform_action(self, item, action_cost);

  Cout("Max uses: " + max_uses + ", ap cost: " + action_cost + " (available: " + self.GetActionPoints() + "), max damage: " + item.AsData()["actions"][action_name]["damage-max"].AsInt());
  return (item.AsData()["actions"][action_name]["damage-max"].AsInt() * max_uses);
}

bool   is_in_range(Character@ self, Character@ target, Data action)
{
  float distance = self.GetDistance(target.AsObject());

  Cout("distance: " + distance + ", range: " + action["range"].AsInt());
  return (action["range"].AsInt() >= distance);
}

string get_best_offensive_action(Character@ self, Character@ target, Item@ item)
{
  Data   item_data              = item.AsData();
  Data   actions                = item_data["actions"];
  int    action_iterator        = 0;
  int    action_points          = self.GetActionPoints();
  int    max_action_points      = self.GetMaxActionPoints();
  string best_action            = "";
  int    best_action_max_damage = 0;

  ia_debug(actions.Count() + " actions to check.");
  while (action_iterator < actions.Count())
  {
    Data action      = actions[action_iterator];

    if (action.Count() > 0 && is_in_range(self, target, action))
    {
      int  max_damage  = max_damage_action(self, item, action.Key());

      ia_debug("in range, max damage " + max_damage);
      if (max_damage > best_action_max_damage)
      {
        best_action            = action.Key();
        best_action_max_damage = max_damage;
      }
    }
    else
      ia_debug("out of range");
    action_iterator++;
  }
  return (best_action);
}

bool is_action_better_than_action(Character@ self, Character@ target, Item@ item_1, string action_1, Item@ item_2, string action_2)
{
  int max_damage_1 = max_damage_action(self, item_1, action_1);
  int max_damage_2 = max_damage_action(self, item_2, action_2);

  if (cares_about_accuracy(self))
  {
    max_damage_1 = (max_damage_1 * item_1.HitSuccessRate(self, target, action_1)) / 100;
    max_damage_2 = (max_damage_2 * item_2.HitSuccessRate(self, target, action_2)) / 100;
  }
  return (max_damage_1 > max_damage_2);
}

int item_get_ammo_amount(Item@ item)
{
  Data data = item.AsData();

  if (data["ammo"]["amount"].Nil())
    return (0);
  return (data["ammo"]["amount"].AsInt());
}

bool item_has_ammo(Inventory@ inventory, Item@ item)
{
  Data         item_data  = item.AsData();
  Data         ammo_types = item_data["ammo"]["types"];

  if (ammo_types.Count() == 0)
    return (true);
  else
  {
    if (item_get_ammo_amount(item) > 0)
      return (true);
    else
    {
      Data       ammo_types = item_data["ammo"]["types"];
      int        type_count = ammo_types.Count();
      int        iterator   = 0;

      while (iterator < type_count)
      {
        string ammo_name = ammo_types[iterator].AsString();
        Item@  ammo      = inventory.GetObject(ammo_name);

        if (@ammo != null)
	  return (true);
        iterator += 1;
      }
    }
  }
  return (false);
}

Item@ get_best_item_from_inventory(Character@ self, Character@ target)
{
  Inventory@       inventory   = self.GetInventory();
  int              item_count  = inventory.GetItemCount();
  int              iterator    = 0;
  Item@            best_item   = null;
  string           best_action = "";
  
  while (iterator < item_count)
  {
    Item@  current_item = inventory[iterator];
    
    if (current_item.IsWeapon())
    {
      string action     = get_best_offensive_action(self, target, current_item);

      if (action != "")
      {
        if (best_action == "" || is_action_better_than_action(self, target, current_item, action, best_item, best_action))
        {
          @best_item  = @current_item;
          best_action = action;
        }
      }
    }
    iterator++;
  }
  return (best_item);
}

string get_best_equip_mode(Character@ self, Item@ item)
{
  return ("Magic");
}

Item@ select_most_suitable_weapon(Character@ self, Character@ target)
{
  Item@ equiped_left    = self.GetInventory().GetEquipedItem("equiped", 0);
  Item@ equiped_right   = self.GetInventory().GetEquipedItem("equiped", 1);
  Data  data_item_left  = equiped_left.AsData();
  Data  data_item_right = equiped_right.AsData();
  Item@ best_choice;

  string best_action_left  = "";
  string best_action_right = "";
  
  ia_debug("equiped left  = " + equiped_left.GetName());
  ia_debug("equiped_right = " + equiped_right.GetName());

  if (item_has_ammo(self.GetInventory(), equiped_left))
    best_action_left  = get_best_offensive_action(self, target, equiped_left);
  else
    ia_debug("left out of ammo");
  if (item_has_ammo(self.GetInventory(), equiped_right))
    best_action_right = get_best_offensive_action(self, target, equiped_right);
  else
    ia_debug("right out of ammo");

  ia_debug("best action left  = " + best_action_left);
  ia_debug("best action right = " + best_action_right);
    
  if (best_action_left != "" && best_action_right != "")
  {
    if (is_action_better_than_action(self, target, equiped_left, best_action_left, equiped_right, best_action_right))
      @best_choice = @equiped_right;
    else
      @best_choice = @equiped_left;
  }
  else if (best_action_left != "")
    @best_choice = @equiped_left;
  else if (best_action_right != "")
    @best_choice = @equiped_right;
  else
  {
    ia_debug("getting weapon from inventory");
    @best_choice = @get_best_item_from_inventory(self, target);
    if (@best_choice != null)
    {
      ia_debug("getting best equip mode");
      string best_mode = get_best_equip_mode(self, best_choice);
    
      ia_debug("equiping item from inventory");
      self.GetInventory().SetEquipedItem("equiped", 1, best_choice, best_mode);
    }
  }
  ia_debug("returning best choice");
  return (best_choice);
}

void go_to_target(Character@ self)
{
  if (@currentTarget != null)
  {
    self.GoTo(currentTarget.AsObject(), 0);
    self.TruncatePath(self.GetActionPoints());
  }
  else
    level.NextTurn();
}

void combat(Character@ self)
{
  ia_debug("Civilian in Combat mode");
  if (self.IsMoving())
    return ;

  if (self.GetHitPoints() < 10 && Fleeing(self))
    return ;

  if (@currentTarget == null || !(currentTarget.IsAlive()))
    @currentTarget = @SelectTarget(self);
  if (@currentTarget == null)
  {
    ia_debug("-> Civilian passing turn");
    level.NextTurn();
  }
  else
  {
    ia_debug("-> Civilian acting on an enemy");
    if (self.HasLineOfSight(currentTarget.AsObject()))
    {
      Item@  weapon                = @select_most_suitable_weapon(self, currentTarget);
      string best_offensive_action = @weapon == null ? "" : get_best_offensive_action(self, currentTarget, weapon);

      if (@weapon == null)
        ia_debug("-> Civilian has no weapon to use");
      if (best_offensive_action != "")
      {
        if (!is_weapon_loaded(weapon))
          UseObject(self, weapon, weapon.GetActionFromName("Reload"));
        else
          UseWeapon(self, currentTarget, weapon, weapon.GetActionFromName(best_offensive_action));
      }
      else
        go_to_target(self);
    }
    else
      go_to_target(self);
  }
}
