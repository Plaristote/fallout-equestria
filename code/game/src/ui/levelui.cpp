#include "ui/level_ui.hpp"
#include "ui/ui_loot.hpp"
#include "ui/ui_equip_mode.hpp"
#include "ui/ui_use_object_on.hpp"
#include "ui/ui_use_skill_on.hpp"
#include "ui/ui_use_spell_on.hpp"
#include "ui/ui_barter.hpp"
#include <ui/ui_next_zone.hpp>
#include "level/dialog.hpp"
#include "level/interact_menu.hpp"
#include <iostream>

using namespace std;

LevelUi::LevelUi(WindowFramework* window, GameUi& gameUi) : window(window), _gameUi(gameUi)
{
  cout << "- Initializing Level Ui" << endl;
  _mainBar = new GameMainBar(window, _gameUi.GetRocketRegion()->get_context());

  _obs.Connect(_mainBar->MenuButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenMenu);
  _obs.Connect(_mainBar->InventoryButtonClicked.EventReceived, _gameUi,             &GameUi::OpenInventory);
  _obs.Connect(_mainBar->PersButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenPers);
  _obs.Connect(_mainBar->PipbuckButtonClicked.EventReceived,   _gameUi.OpenPipbuck, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
  _obs.Connect(_gameUi.GetInventory().VisibilityToggled, InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetMenu().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetPers().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
    _currentUis[i] = 0;
  cout << "-> Done." << endl;
}

LevelUi::~LevelUi(void)
{
  _obs.DisconnectAll();
  delete _mainBar;
  for (unsigned short i = 0 ; i < UiTotalIt ; ++i)
  {
    if (_currentUis[i] != 0)
      delete _currentUis[i];
  }
}

bool LevelUi::HasOpenedWindows(void) const
{
  for (int i = 0 ; i < UiTotalIt ; ++i)
  {
    if (_currentUis[i] && _currentUis[i]->IsVisible())
      return (true);
  }
  return (false);
}

UiLoot*            LevelUi::OpenUiLoot(Inventory* looter, Inventory* looted)
{
  UiLoot* ui_loot = new UiLoot(window, GetContext(), *looter, *looted);

  SetUi<UiItLoot>(ui_loot);
  ui_loot->Done.Connect(*this, &LevelUi::CloseRunningUi<UiItLoot>);
  return (ui_loot);
}

UiEquipMode*       LevelUi::OpenUiEquipMode(void)
{
  UiEquipMode* ui_equip_mode = new UiEquipMode(window, GetContext());

  SetUi<UiItEquipMode>(ui_equip_mode);
  ui_equip_mode->Closed.Connect(*this, &LevelUi::CloseRunningUi<UiItEquipMode>);
  return (ui_equip_mode);
}

UiUseObjectOn*     LevelUi::OpenUiUseObjectOn(Inventory* user_inventory)
{
  UiUseObjectOn* ui_use_object_on = new UiUseObjectOn(window, GetContext(), *user_inventory);
  
  SetUi<UiItUseObjectOn>(ui_use_object_on);
  ui_use_object_on->Closed.Connect(*this, &LevelUi::CloseRunningUi<UiItUseObjectOn>);
  return (ui_use_object_on);
}

UiUseSkillOn*      LevelUi::OpenUiUseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target)
{
  UiUseSkillOn* ui_use_skill_on = new UiUseSkillOn(window, GetContext(), user, target);
  
  SetUi<UiItUseSkillOn>(ui_use_skill_on);
  ui_use_skill_on->Closed.Connect(*this, &LevelUi::CloseRunningUi<UiItUseSkillOn>);
  return (ui_use_skill_on);
}

UiUseSpellOn*      LevelUi::OpenUiUseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target)
{
  UiUseSpellOn* ui_use_spell_on = new UiUseSpellOn(window, GetContext(), user, target);
  
  SetUi<UiItUseSpellOn>(ui_use_spell_on);
  ui_use_spell_on->Closed.Connect(*this, &LevelUi::CloseRunningUi<UiItUseSpellOn>);
  return (ui_use_spell_on);
}

UiNextZone*        LevelUi::OpenZonePicker(const vector<string>& destinations)
{
  UiNextZone* ui_next_zone = new UiNextZone(window, GetContext(), destinations);
  
  SetUi<UiItNextZone>(ui_next_zone);
  ui_next_zone->Done.Connect(*this, &LevelUi::CloseRunningUi<UiItNextZone>);
  return (ui_next_zone);
}

UiBarter*          LevelUi::OpenUiBarter(ObjectCharacter* player, ObjectCharacter* trader)
{
  UiBarter* ui_barter = new UiBarter(window, GetContext(), player, trader);
  
  SetUi<UiItBarter>(ui_barter);
  ui_barter->BarterEnded.Connect(*this, &LevelUi::CloseRunningUi<UiItBarter>);
  return (ui_barter);
}

DialogController*  LevelUi::OpenUiDialog(InstanceDynamicObject* object)
{
  DialogController* ui_dialog = new DialogController(window, GetContext(), object);

  GetMainBar().Hide();
  SetUi<UiItRunningDialog>(ui_dialog);
  ui_dialog->DialogEnded.Connect(*this,        &LevelUi::CloseRunningUi<UiItRunningDialog>);
  ui_dialog->DialogEnded.Connect(GetMainBar(), &UiBase::Show);
  return (ui_dialog);
}

DialogController*  LevelUi::OpenUiDialog(ObjectCharacter* target)
{
  DialogController* ui_dialog = new DialogController(window, GetContext(), target);
  
  GetMainBar().Hide();
  SetUi<UiItRunningDialog>(ui_dialog);
  ui_dialog->DialogEnded.Connect(*this,        &LevelUi::CloseRunningUi<UiItRunningDialog>);
  ui_dialog->DialogEnded.Connect(GetMainBar(), &UiBase::Show);
  return (ui_dialog);
}

InteractMenu*      LevelUi::OpenInteractMenu(InstanceDynamicObject* object)
{
  InteractMenu* ui_interact_menu = new InteractMenu(window, GetContext(), *object);

  ui_interact_menu->Done.Connect(*this, &LevelUi::CloseRunningUi<UiItInteractMenu>);
  SetUi<UiItInteractMenu>(ui_interact_menu);
  return (ui_interact_menu);
}
