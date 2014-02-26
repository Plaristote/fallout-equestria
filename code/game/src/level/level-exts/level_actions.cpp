#include "level/level.hpp"
#include "ui/alert_ui.hpp"
#include "ui/ui_barter.hpp"
#include "ui/ui_use_object_on.hpp"
#include "ui/ui_use_skill_on.hpp"
#include "ui/ui_use_spell_on.hpp"

#define AP_COST_USE             2

#define ScriptErrorEmit(message) \
  { Script::Engine::ScriptError.Emit(std::string("<span class='console-error'>") + message + std::string("</span>")); return ; }

typedef std::function<void (InstanceDynamicObject*)> ActionCallback;

using namespace std;

void Level::CallbackActionUse(InstanceDynamicObject* object)
{
  level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
  ActionUse(GetPlayer(), object);
}

void Level::CallbackActionBarter(ObjectCharacter* character)
{
  cout << "CallbackActionBarter" << endl;
  UiBarter* ui_barter = level_ui.OpenUiBarter(GetPlayer(), character);

  camera.SetEnabledScroll(false);
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
  if (GetState() == Fight)
  {
    ConsoleWrite(i18n::T("Can't talk during fight."));
    return ;
  }
  if ((GetPlayer()->HasLineOfSight(object)) && GetPlayer()->GetPathDistance(object) <= 3)
  {
    // Player must look at its target
    GetPlayer()->LookAt(object);
    // If target is a character, then it must look back
    ObjectCharacter* talkTo = object->Get<ObjectCharacter>();
    if (talkTo)
      talkTo->LookAt(GetPlayer());

    {
      DialogController* controller = level_ui.OpenUiDialog(talkTo);

      controller->StartBarter.Connect(*this, &Level::CallbackActionBarter);
    }
  }
  else
  {
    GetPlayer()->GoTo(object, 3);
    if (GetPlayer()->GetPath().Size() > 1)
      GetPlayer()->ReachedDestination.Connect([this, object](InstanceDynamicObject*) { CallbackActionTalkTo(object); });
    else
      ConsoleWrite(i18n::T("No line of sight"));
  }
}

void Level::CallbackActionUseObjectOn(InstanceDynamicObject* target)
{
  UiUseObjectOn* ui = level_ui.OpenUiUseObjectOn(&(GetPlayer()->GetInventory()));

  ui->ObjectSelected.Connect([this, target](InventoryObject* object)
  {
    ActionUseObjectOn(GetPlayer(), target, object, 0);
  });
}

void Level::CallbackActionUseSkillOn(InstanceDynamicObject* target)
{
  UiUseSkillOn* ui_use_skill_on = level_ui.OpenUiUseSkillOn(GetPlayer(), target);

  ui_use_skill_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    ActionUseSkillOn(GetPlayer(), target, skill);
  });
  ui_use_skill_on->Show();
}

void Level::CallbackActionUseSpellOn(InstanceDynamicObject* target)
{
  UiUseSpellOn* ui_use_spell_on = level_ui.OpenUiUseSpellOn(GetPlayer(), target);

  ui_use_spell_on->Show();
  ui_use_spell_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    ActionUseSpellOn(GetPlayer(), target, skill);
  });
}

void Level::CallbackActionTargetUse(unsigned short it)
{
  ObjectCharacter* player    = GetPlayer();
  InventoryObject* item      = player->GetEquipedItem(it);
  unsigned char    action_it = player->GetequipedAction(it);
  
  std::cout << "ActionTargetUse" << std::endl;
  if (item)
  {
    Data action = (*item)["actions"][action_it];

    player->active_object    = item;
    player->active_object_it = action_it;
    if (action["targeted"].Value() == "1")
    {
      if (action["combat"].Value() == "1" && _state != Fight)
        StartFight(player);
      mouse.SetState(MouseEvents::MouseTarget);
      mouse.TargetPicked.Connect([this, item, action, action_it](InstanceDynamicObject* object)
      {
        cout << "TargetPicked Callback Called" << endl;
        ObjectCharacter* character = object->Get<ObjectCharacter>();
        
        if (character != 0 && action["combat"].Value() == "1")
          ActionUseWeaponOn(GetPlayer(), character, item, action_it);
        else
          ActionUseObjectOn(GetPlayer(), object,    item, action_it);
      });
    }
    else
      ActionUseObject(player, item, action_it);
  }
}

void Level::ActionUse(ObjectCharacter* user, InstanceDynamicObject* target)
{
  ActionCallback animation_step, logic_step;

  if (!user || !target) ScriptErrorEmit("[ActionUse] Aborted: NullPointer Error");
  logic_step     = [this, user, target](InstanceDynamicObject*)
  {
    user->LookAt(target);
    if (!(UseActionPoints(AP_COST_USE)))
      return ;
    target->CallbackActionUse(user);
    mouse.SetState(MouseEvents::MouseAction);
  };
  animation_step = [this, user, logic_step](InstanceDynamicObject*)
  {
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    user->PlayAnimation("use");
  };
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(animation_step);
}

void Level::ActionUseObject(ObjectCharacter* user, InventoryObject* object, unsigned char actionIt)
{
  if (!user || !object)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObject] Aborted: NullPointer Error</span>");
    return ;
  }
  if ((*object)["actions"].Count() <= actionIt)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[ActionUseObject] Invalid action iterator</span>");
    return ;
  }
  object->Use(user, actionIt);  
}

void Level::ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object, unsigned char actionIt)
{
  ActionCallback animation_step, logic_step;

  if (!object || !target || !user)              ScriptErrorEmit("[ActionUseObjectOn] Aborted: NullPointer Error");
  if ((*object)["actions"].Count() <= actionIt) ScriptErrorEmit("[ActionuseObjectOn] Invalid action iterator");
  if (!(object->CanUse(user, target, actionIt)))
    return ;
  logic_step     = [this, user, target, object, actionIt](InstanceDynamicObject*)
  {
    Data           data_use_cost = (*object)["use_cost"];
    unsigned short use_cost      = AP_COST_USE;

    if (!(data_use_cost.Nil()))
      use_cost = data_use_cost;
    user->LookAt(target);
    if (UseActionPoints(use_cost))
      object->UseOn(user, target, actionIt);
  };
  animation_step = [this, user, actionIt, logic_step, target](InstanceDynamicObject*)
  {
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    user->PlayEquipedItemAnimation(actionIt, "use", target);
  };
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(animation_step);
  if (user == GetPlayer())
    level_ui.CloseRunningUi<LevelUi::UiItUseObjectOn>();
}

struct XpFetcher
{
  XpFetcher(ObjectCharacter* killer, ObjectCharacter* target) : killer(killer), target(target)
  {
    observerId     = target->CharacterDied.Connect(*this, &XpFetcher::CharacterDied);
    character_died = false;
  }
  
  ~XpFetcher(void)
  {
    target->CharacterDied.Disconnect(observerId);
  }
  
  void CharacterDied(void)
  {
    character_died = true;
  }
  
  void Execute(void)
  {
    Data            stats      = target->GetStatistics();
    Data            xp_reward;
    StatController* controller = killer->GetStatController();
    
    if (stats.NotNil())
      xp_reward = stats["Variable"]["XpReward"];
    if (controller)
    {
      if (xp_reward.Nil())
        controller->AddExperience(1001);
      else
        controller->AddExperience(xp_reward);
      controller->AddKill(stats["Race"].Value());
    }
  }
  
  ObjectCharacter  *killer, *target;
  Sync::ObserverId observerId;
  bool             character_died;
};

struct ActionPicking
{
  bool operator==(const std::string& name) const { return (this->name == name); }
  std::string                                       name;
  std::function<void (std::function<void (void*)>)> Pick;
};

std::vector<ActionPicking> pick_handlers;

void Level::ActionUseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill)
{
  if (pick_handlers.size() == 0)
  {
    ActionPicking teleport_handler;
    
    teleport_handler.name = "waypoints";
    teleport_handler.Pick = [this](std::function<void (void*)> callback)
    {
      mouse.SetState(MouseEvents::MouseWaypointPicker);
      mouse.WaypointPicked.Connect([callback](Waypoint* waypoint)
      {
        callback(waypoint);
      });
    };
    pick_handlers.push_back(teleport_handler);
    
    teleport_handler.name = "characters";
    teleport_handler.Pick = [this](std::function<void (void*)> callback)
    {
      mouse.SetState(MouseEvents::MouseTarget);
      mouse.TargetPicked.Connect([callback](InstanceDynamicObject* object)
      {
        if (object->Get<ObjectCharacter>() != 0)
          callback(object);
      });
    };
    pick_handlers.push_back(teleport_handler);
    
    teleport_handler.name = "self";
    teleport_handler.Pick = [this](std::function<void (void*)> callback)
    {
      callback(GetPlayer());
    };
    pick_handlers.push_back(teleport_handler);
  }
  
  Data     spell       = GetDataEngine()["spells"][skill];
  string   target_type = spell["targets"].Value();

  cout << "ActionUseSpellOn " << skill << endl;
  if (!target)
  {
    auto   handler     = find(pick_handlers.begin(), pick_handlers.end(), target_type);

    if (handler != pick_handlers.end())
      handler->Pick([this, user, skill](void* item) { ActionUseSpellOn(user, (InstanceDynamicObject*)item, skill); });
  }
  else
  {
    try
    {
      AngelScript::Object                   object("scripts/buffs/" + spell["script"]["file"].Value());
      AngelScript::Type<ObjectCharacter*>   param1(user);
      AngelScript::Type<void*>              param2(target);

      object.asDefineMethod("use_spell", spell["script"]["method"].Value());
      if (target_type == "waypoints")
      {
        AngelScript::Type<int>              param_waypoint(((Waypoint*)(target))->id);

        object.Call("use_spell", 2, &param1, &param_waypoint);
      }
      else
        object.Call("use_spell", 2, &param1, &param2);
    }
    catch (AngelScript::Exception& e)
    {
      AlertUi::NewAlert.Emit(string("Spell script couldn't be executed: ") + e.what());
    }
  }
}

void Level::ActionUseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill)
{
  ActionCallback logic_step, animation_step;

  logic_step = [this, user, skill, target](InstanceDynamicObject*)
  {
    if (target != 0)
      target->CallbackActionUseSkill(user, skill);
  };
  animation_step = [this, user, logic_step](InstanceDynamicObject*)
  {
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    user->PlayAnimation("use");
  };
  if (target != user)
  {
    user->GoTo(target, 0);
    user->ReachedDestination.Connect(animation_step);
  }
  else
    animation_step(target);
  if (user == GetPlayer())
    level_ui.CloseRunningUi<LevelUi::UiItUseSkillOn>();
}

void Level::ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char actionIt)
{
  ActionCallback logic_step;
  unsigned int   equipedIt = 0;

  if (user->GetEquipedItem(0) == item)
    equipedIt = 0;
  else if (user->GetEquipedItem(1) == item)
    equipedIt = 1;

  function<void (bool)>           target_animate  = [this, target, item, actionIt](bool is_kill)
  {
    // TODO Play the proper animation
    target->PlayAnimation("use");
  };

  function<void (Data,bool,bool)> fire_projectile = [this, user, target, target_animate, equipedIt](Data projectile_data, bool bullseye, bool is_kill)
  {
    NodePath projectile_dest;
	auto     _target_animate = target_animate; // MSVC Workaround
    
    if (bullseye)
      projectile_dest = target->GetNodePath();
    else
      projectile_dest = target->GetOccupiedWaypoint()->GetRandomWaypoint()->nodePath;
    {
      Projectile* projectile = new Projectile(_world,
                                              user->GetEquipedItemNode(equipedIt),
                                              projectile_dest,
                                              projectile_data);

      projectile->SetTimeout(projectile_data["timeout"] || 10);
      projectile->SetColor(255, 255, 0, 1);
      if (bullseye)
        projectile->HitsTarget.Connect([_target_animate, is_kill](void) { _target_animate(is_kill); });
      if (projectile->HasReachedDestination())
        projectile->HitsTarget.Emit();
      InsertProjectile(projectile);
    }
  };

  logic_step = [this, user, target, item, actionIt, equipedIt, fire_projectile, target_animate](InstanceDynamicObject*)
  {
    XpFetcher xpFetcher(user, target);
    bool      bullseye; // false -> miss

    user->SetAsEnemy(target, true);
    bullseye = (item->UseAsWeapon(user, target, actionIt));
    mouse.MouseRightClicked();
    if (!(target->IsAlly(user)))
      target->GetFieldOfView().SetEnemyDetected(*user);
    if (xpFetcher.character_died)
      xpFetcher.Execute();

    {
      Data projectile_data = (*item)["actions"][actionIt]["animations"]["projectiles"]["attack"];

      if (projectile_data.NotNil())
        fire_projectile(projectile_data, bullseye, xpFetcher.character_died);
      else if (bullseye)
        target_animate(xpFetcher.character_died);
    }
  };
  if (user == target && target == GetPlayer())
    return ;
  if (!(item->CanUse(user, target, actionIt)))
    return ;

  user->LookAt(target);

  if (target->GetDistance(user) > (float)((*item)["actions"][actionIt]["range"]))
    ConsoleWrite("Out of range");
  else if (!(user->HasLineOfSight(target)))
    ConsoleWrite("No line of sight");
  else
  {
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    user->PlayEquipedItemAnimation(equipedIt, "attack", target);
  }
}

