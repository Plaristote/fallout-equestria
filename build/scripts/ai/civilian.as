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
  Cout("Civilian Select Target");
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
    Cout("-> Selected enemy: " + bestMatch.GetName());
    Write("-> Selected enemy: " + bestMatch.GetName());
  }
  else
  {
    Cout("-> No enemy detected");
  }
  return (bestMatch);
}

int get_available_ammo_for(Character@ self, Item@ item)
{
  Data ammo         = item.AsData()["ammo"];
  Data current_ammo = ammo["current"];

  if (current_ammo.Nil())
    current_ammo = ammo["types"][0];
  if (current_ammo.Nil())
    return (self.GetInventory().ContainsHowMany(current_ammo.AsString()));
  return (0);
}

bool is_weapon_loaded(Item@ item)
{
  Data ammo = item.AsData()["actions"]["ammo"];
  
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
      action_points -= 2;
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
  
  return (item.AsData()["actions"][action_name]["damage-max"].AsInt() * max_uses);
}

bool   is_in_range(Character@ self, Character@ target, Data action)
{
  float distance = self.GetDistance(target.AsObject());

  return (action["range"].AsInt() >= distance);
}

string get_best_offensive_action(Character@ self, Character@ target, Item@ item)
{
  Data   item_data              = item.AsData();
  Data   actions                = item_data["actions"];
  int    action_iterator        = 0;
  int    action_points          = self.GetActionPoints();
  int    max_action_points      = self.GetMaxActionPoints();
  string best_action;
  int    best_action_max_damage = 0;

  while (action_iterator < actions.Count())
  {
    Data action      = actions[action_iterator];
    
    if (is_in_range(self, target, action))
    {
      int  max_damage  = max_damage_action(self, item, action.Key());

      if (max_damage > best_action_max_damage)
      {
        best_action            = action.Key();
        best_action_max_damage = max_damage;
      }
    }
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

Item@ select_most_suitable_weapon(Character@ self, Character@ target)
{
  Item@ equiped_left    = self.GetEquipedItem(0);
  Item@ equiped_right   = self.GetEquipedItem(1);
  Data  data_item_left  = equiped_left.AsData();
  Data  data_item_right = equiped_right.AsData();
  Item@ best_choice;

  string best_action_left  = get_best_offensive_action(self, target, equiped_left);
  string best_action_right = get_best_offensive_action(self, target, equiped_right);

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
    // TODO Pick item from inventory
  }
  return (best_choice);
}

void go_to_target(Character@ self)
{
  if (@currentTarget != null)
  {
    self.GoTo(currentTarget.AsObject(), 1);
    self.TruncatePath(self.GetActionPoints());
  }
  else
    level.NextTurn();
}

void combat(Character@ self)
{
  Cout("Civilian in Combat mode");
  if (self.IsMoving())
    return ;

  if (self.GetHitPoints() < 10 && Fleeing(self))
    return ;

  if (@currentTarget == null || !(currentTarget.IsAlive()))
    @currentTarget = @SelectTarget(self);
  if (@currentTarget == null)
  {
    Cout("-> Civilian passing turn");
    level.NextTurn();
  }
  else
  {
    Cout("-> Civilian acting on an enemy");
    if (self.HasLineOfSight(currentTarget.AsObject()))
    {
      Item@ weapon = @select_most_suitable_weapon(self, currentTarget);
      
      if (@weapon != null)
      {
        if (!is_weapon_loaded(weapon))
          weapon.Use(self, "reload");
        else
          weapon.UseWeaponOn(self, currentTarget, get_best_offensive_action(self, currentTarget, weapon));
      }
      else
        go_to_target(self);
    }
    else
      go_to_target(self);
  }
}
