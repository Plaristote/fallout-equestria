#include "level/level.hpp"

#define AP_COST_USE             2

#define ScriptErrorEmit(message) \
  { Script::Engine::ScriptError.Emit(std::string("<span class='console-error'>") + message + std::string("</span>")); return ; }

typedef std::function<void (InstanceDynamicObject*)> ActionCallback;

using namespace std;

void Level::CallbackActionUse(InstanceDynamicObject* object)
{
  CloseRunningUi<UiItInteractMenu>();
  ActionUse(GetPlayer(), object);
}

void Level::CallbackActionBarter(ObjectCharacter* character)
{
  cout << "CallbackActionBarter" << endl;
  UiBarter* ui_barter;
  
  CloseRunningUi<UiItBarter>();
  if (_currentUis[UiItBarter])
    delete _currentUis[UiItBarter];
  ui_barter = new UiBarter(_window, _levelUi.GetContext(), GetPlayer(), character);
  ui_barter->BarterEnded.Connect(*this, &Level::CloseRunningUi<UiItBarter>);
  ui_barter->Show();
  _currentUis[UiItBarter] = ui_barter;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  CloseRunningUi<UiItInteractMenu>();
  if (GetState() == Fight)
  {
    ConsoleWrite(i18n::T("Can't talk during fight."));
    return ;
  }
  if ((GetPlayer()->HasLineOfSight(object)) && GetPlayer()->GetPathDistance(object) <= 3)
  {
    if (_currentUis[UiItRunningDialog])
      delete _currentUis[UiItRunningDialog];
    
    // Player must look at its target
    GetPlayer()->LookAt(object);
    // If target is a character, then it must look back
    ObjectCharacter* talkTo = object->Get<ObjectCharacter>();
    if (talkTo)
      talkTo->LookAt(GetPlayer());
    
    _currentRunningDialog = new DialogController(_window, _levelUi.GetContext(), talkTo, i18n::GetDialogs());
    _currentRunningDialog->DialogEnded.Connect(*this, &Level::CloseRunningUi<UiItRunningDialog>);
    _currentRunningDialog->DialogEnded.Connect(_levelUi.GetMainBar(), &UiBase::Show);
    _currentRunningDialog->StartBarter.Connect(*this, &Level::CallbackActionBarter);
    _mouseActionBlocked = true;
    _camera.SetEnabledScroll(false);
    _levelUi.GetMainBar().Hide();
    _currentUis[UiItRunningDialog] = _currentRunningDialog;
    SetInterrupted(true);
  }
  else
  {
    GetPlayer()->GoTo(object, 3);
    if (GetPlayer()->GetPathSize() > 1)
      GetPlayer()->ReachedDestination.Connect([this, object](InstanceDynamicObject*) { CallbackActionTalkTo(object); });
    else
      ConsoleWrite(i18n::T("No line of sight"));
  }
}

void Level::CallbackActionUseObjectOn(InstanceDynamicObject* target)
{
  CloseRunningUi<UiItInteractMenu>();
  if (_currentUis[UiItUseObjectOn])
    delete _currentUis[UiItUseObjectOn];
  std::cout << "CallbackActionUseObjectOn" << std::endl;
  _currentUseObjectOn = new UiUseObjectOn(_window, _levelUi.GetContext(), GetPlayer()->GetInventory());
  _currentUseObjectOn->ActionCanceled.Connect(*this, &Level::CloseRunningUi<UiItUseObjectOn>);
  _currentUseObjectOn->ObjectSelected.Connect([this, target](InventoryObject* object)
  {
    ActionUseObjectOn(GetPlayer(), target, object, 0);
  });
  _mouseActionBlocked = true;
  _camera.SetEnabledScroll(false);
  SetInterrupted(true);
  _currentUis[UiItUseObjectOn] = _currentUseObjectOn;
}

void Level::CallbackActionUseSkillOn(InstanceDynamicObject* target)
{
  CloseRunningUi<UiItInteractMenu>();
  if (_currentUis[UiItUseSkillOn])
    delete _currentUis[UiItUseSkillOn];
  {
    UiUseSkillOn* ui_use_skill_on = new UiUseSkillOn(_window, _levelUi.GetContext(), GetPlayer(), target);

    ui_use_skill_on->Closed.Connect(*this, &Level::CloseRunningUi<UiItUseSkillOn>);
    ui_use_skill_on->SkillPicked.Connect([this, target](const std::string& skill)
    {
      ActionUseSkillOn(GetPlayer(), target, skill);
    });
    ui_use_skill_on->Show();
    _mouseActionBlocked = true;
    _camera.SetEnabledScroll(false);
    _currentUis[UiItUseSkillOn] = ui_use_skill_on;
    SetInterrupted(true);
  }
}

void Level::CallbackActionTargetUse(unsigned short it)
{
  ObjectCharacter* player   = GetPlayer();
  InventoryObject* object   = player->GetEquipedItem(it);
  unsigned char    actionIt = player->GetequipedAction(it);
  
  std::cout << "ActionTargetUse" << std::endl;
  if (object)
  {
    player->active_object    = object;
    player->active_object_it = actionIt;
    if ((*object)["actions"][actionIt]["targeted"] == "1")
    {
      if ((*object)["actions"][actionIt]["combat"].Value() == "1" && _state != Fight)
        StartFight(player);
      SetMouseState(MouseTarget);
    }
    else
      ActionUseObject(player, object, actionIt);
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
    SetMouseState(MouseAction);
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
  
  std::cout << "ActionUseObject" << std::endl;
  // Use the object naow
  //   if (!(user->pendingAnimationDone))
  //   {
  //     user->pendingActionObject         = object;
  //     user->pendingActionObjectActionIt = actionIt;
  //     user->AnimationEnded.Connect(*this, &Level::PendingActionUseWeaponOn);
  //     user->PlayEquipedItemAnimation(user->GetEquipedItem(0) == object ? 0 : 1);
  //   }
}

void Level::ActionUseObjectOn(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* object, unsigned char actionIt)
{
  ActionCallback animation_step, logic_step;

  if (!object || !target || !user)              ScriptErrorEmit("[ActionUseObjectOn] Aborted: NullPointer Error");
  if ((*object)["actions"].Count() <= actionIt) ScriptErrorEmit("[ActionuseObjectOn] Invalid action iterator");
  logic_step     = [this, user, target, object, actionIt](InstanceDynamicObject*)
  {
    Data           data_use_cost = (*object)["use_cost"];
    unsigned short use_cost      = AP_COST_USE;

    if (!(data_use_cost.Nil()))
      use_cost = data_use_cost;
    user->LookAt(target);
    if (UseActionPoints(use_cost))
    {
      const string output = object->UseOn(user, target, actionIt);

      ConsoleWrite(output);
    }
  };
  animation_step = [this, user, actionIt, logic_step](InstanceDynamicObject*)
  {
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    user->PlayEquipedItemAnimation(actionIt, "use");
  };
  user->GoTo(target, 0);
  user->ReachedDestination.Connect(animation_step);
  if (user == GetPlayer())
    CloseRunningUi<UiItUseObjectOn>();
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
    CloseRunningUi<UiItUseSkillOn>();
}

void Level::ActionUseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char actionIt)
{
  ActionCallback logic_step;

  logic_step = [this, user, target, item, actionIt](InstanceDynamicObject*)
  {
    XpFetcher xpFetcher(user, target);
    string    output;
    
    user->SetAsEnemy(target, true);
    output = (item->UseAsWeapon(user, target, actionIt));
    MouseRightClicked();
    ConsoleWrite(output);
    if (!(target->IsAlly(user)))
      target->SetEnemyDetected(user);
    if (xpFetcher.character_died)
    {
      xpFetcher.Execute();
      if (user->GetStatController() && target->GetStatController())
      {
        string race  = target->GetStatistics()["Race"].Value();
        Data   kills = user->GetStatistics()["Kills"];
        
        if (kills[race].Nil())
          kills[race] = 0;
        else
          kills[race] = (unsigned int)kills[race] + 1;
      }
    }
  };
  if (user == target && target == GetPlayer())
  {
    ConsoleWrite("Stop hitting yourself !");
    return ;
  }
    
  user->LookAt(target);
    
  if (target->GetDistance(user) > (float)((*item)["actions"][actionIt]["range"]))
    ConsoleWrite("Out of range");
  else if (!(user->HasLineOfSight(target)))
    ConsoleWrite("No line of sight");
  else
  {
    unsigned int equipedIt            = 0;
      
    user->AnimationEnded.DisconnectAll();
    user->AnimationEnded.Connect(logic_step);
    if (user->GetEquipedItem(0))
      equipedIt = 0;
    else if (user->GetEquipedItem(1))
      equipedIt = 1;
    user->PlayEquipedItemAnimation(equipedIt, "attack");
  }
}

