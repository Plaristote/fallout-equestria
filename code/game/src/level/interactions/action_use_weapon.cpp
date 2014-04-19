#include "level/interactions/action_use_weapon.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"

using namespace std;
using namespace Interactions;

void Actions::UseWeaponOn::Run()
{
  if (CheckAndRemoveActionPoints())
  {
    cout << "Use weapon play animation" << endl;
    PlayAnimation();
  }
}

void Actions::UseWeaponOn::PlayAnimation()
{
  cout << "Use weapon, play animation" << endl;
  ObjectCharacter* user = GetUser();

  user->LookAt(GetObjectTarget());
  user->AnimationEndForObject.DisconnectAll();
  observers.Connect(user->AnimationEndForObject, [this](AnimatedObject*)
  {
    RunAction();
  });
  user->PlayEquipedItemAnimation(equiped_it, "attack", GetObjectTarget());
}

void Actions::UseWeaponOn::RunAction()
{
  cout << "Use weapon, play action" << endl;
  ObjectCharacter* target      = GetObjectTarget()->Get<ObjectCharacter>();
  StatController*  target_stat = target->GetStatController();

  observers.Connect(target_stat->Died, *this, &UseWeaponOn::TargetDied);
  hit_success = weapon->UseAsWeapon(GetUser(), target, action_it);
  observers.DisconnectAllFrom(target_stat->Died);
  if (HasProjectile())
    FireProjectile();
  else
    TargetAnimate();
  //mouse.MouseRightClicked();

  // TODO Once new diplomacy is ready, make this less shitty:
  if (!enemy_died)
  {
    GetUser()->SetAsEnemy(target, true);
    if (!(target->IsAlly(GetUser())))
      target->GetFieldOfView().SetEnemyDetected(*GetUser());
  }
}

void Actions::UseWeaponOn::TargetDied()
{
  cout << "Target died" << endl;
  StatController* controller = GetUser()->GetStatController();
  Data            stats      = GetObjectTarget()->Get<ObjectCharacter>()->GetStatistics();
  Data            xp_reward  = stats["Variable"]["XpReward"];

  if (controller)
  {
    string        race       = stats["Race"].Or(string("Unknown"));
    
    controller->AddExperience(xp_reward || DEFAULT_XP_REWARD);
    controller->AddKill(race);
  }
  enemy_died = true;
  cout << "/Target Died" << endl;
}

void Actions::UseWeaponOn::TargetAnimate()
{
  if (hit_success)
  {
    InstanceDynamicObject* target = GetObjectTarget();

    if (enemy_died)
      target->PlayAnimation("death");
    else
      target->PlayAnimation("use");
  }
}

void Actions::UseWeaponOn::FireProjectile()
{
  cout << "FIRING PROJECTILE!" << endl;
  Data                   projectile_data = action["animations"]["projectiles"]["attack"];
  NodePath               destination, weapon_node;
  InstanceDynamicObject* target = GetObjectTarget();
  Projectile*            projectile;
  World*                 world = GetLevel()->GetWorld();

  destination = hit_success ? target->GetNodePath() : target->GetOccupiedWaypoint()->GetRandomWaypoint()->nodePath;
  weapon_node = GetUser()->GetEquipedItemNode(equiped_it);
  projectile  = new Projectile(world, weapon_node, destination, projectile_data);
  projectile->SetTimeout(projectile_data["timeout"] || 10);
  projectile->SetColor(255, 255, 0, 1);
  if (hit_success)
    observers.Connect(projectile->HitsTarget, *this, &UseWeaponOn::TargetAnimate);
  if (projectile->HasReachedDestination())
    projectile->HitsTarget.Emit();
  GetLevel()->InsertProjectile(projectile);
}

bool Actions::UseWeaponOn::HasProjectile(void) const
{
  return (action["animations"]["projectiles"]["attack"].NotNil());
}

void Actions::UseWeaponOn::FindEquipedIterator()
{
  equiped_it = 0;
  for (unsigned int i = 0 ; i < MAX_EQUIPED_SLOTS ; ++i)
  {
    if (GetUser()->GetEquipedItem(i) == weapon)
    {
      equiped_it = i;
      break ;
    }
  }
}

Actions::UseWeaponOn::UseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char action_it) : ActionRunner(user), weapon(item), action_it(action_it)
{
  enemy_died = hit_success = false;
  action     = (*item)["actions"][action_it];
  FindEquipedIterator();
  SetTargetType(ActionRunner::Character);
  SetTarget(target);
  SetActionPointCost(item->GetActionPointCost(user, action_it));
}

ActionRunner* Actions::UseWeaponOn::Factory(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char action_it)
{
  cout << "Use weapon, factory" << endl;
  UseWeaponOn* runner = 0;
  
  if (user == target)
    ConsoleWrite(user, "You cannot target yourself");
  else if (!(item->CanUse(user, target, action_it)))
    ConsoleWrite(user, "You cannot use that object");
  else if (target->GetDistance(user) > (float)((*item)["actions"][action_it]["range"]))
    ConsoleWrite(user, "Out of range");
  else if (!(user->HasLineOfSight(target)))
    ConsoleWrite(user, "No line of sight");
  else
  {
    runner = new UseWeaponOn(user, target, item, action_it);
    runner->Run();
  }
  return (runner);
}
