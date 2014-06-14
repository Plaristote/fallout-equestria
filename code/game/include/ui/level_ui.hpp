#ifndef  LEVEL_UI_HPP
# define LEVEL_UI_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"
# include "gameui.hpp"
# include "game_main_bar.hpp"
# include "executor.hpp"

class UiLoot;
class UiEquipMode;
class UiBarter;
class UiUseObjectOn;
class UiUseSkillOn;
class UiUseSpellOn;
class UiNextZone;
class DialogController;
class InteractMenu;

class InstanceDynamicObject;
class ObjectCharacter;

class LevelUi
{
public:
  enum UiIterator
  {
    UiItInteractMenu,
    UiItRunningDialog,
    UiItUseObjectOn,
    UiItUseSkillOn,
    UiItUseSpellOn,
    UiItLoot,
    UiItEquipMode,
    UiItNextZone,
    UiItBarter,
    UiTotalIt
  };
  
  LevelUi(WindowFramework* window, GameUi& gameUi);
  ~LevelUi(void);

  Sync::Signal<void (bool)>       InterfaceOpened;
  
  Rocket::Core::Context*          GetContext()       { return (_gameUi.GetRocketRegion()->get_context()); }
  GameMainBar&                    GetMainBar(void)   { return (*_mainBar);                                }
  GameMenu&                       GetMenu(void)      { return (_gameUi.GetMenu());                        }
  GameInventory&                  GetInventory(void) { return (_gameUi.GetInventory());                   }
  StatViewRocket&                 GetPers(void)      { return (_gameUi.GetPers());                        }
  UiLoot*                         OpenUiLoot(Inventory* looter, Inventory* looted);
  UiEquipMode*                    OpenUiEquipMode(void);
  UiUseObjectOn*                  OpenUiUseObjectOn(Inventory* user);
  UiUseSkillOn*                   OpenUiUseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target);
  UiUseSpellOn*                   OpenUiUseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target);
  UiNextZone*                     OpenZonePicker(const std::vector<std::string>& destinations);
  UiBarter*                       OpenUiBarter(ObjectCharacter* player, ObjectCharacter* trader);
  DialogController*               OpenUiDialog(ObjectCharacter* target);
  DialogController*               OpenUiDialog(InstanceDynamicObject* target);
  InteractMenu*                   OpenInteractMenu(InstanceDynamicObject*);

  bool                            HasOpenedWindows(void) const;
  template<UiIterator it> bool    HasOpenedUi(void)  const { return (_currentUis[it] && _currentUis[it]->IsVisible()); }  
  template<UiIterator it> UiBase* GetRunningUi(void) const { return (_currentUis[it]); }

  template<UiIterator it> void    CloseRunningUi(void)
  {
    if (_currentUis[it])
    {
      UiBase* ui      = _currentUis[it];

      _currentUis[it] = 0;
      ui->Destroy();
      Executor::ExecuteLater([ui]() { delete ui; });
      InterfaceOpened.Emit(false);
    }
  }

private:
  template<UiIterator it> void SetUi(UiBase* ui)
  {
    if (it != UiItInteractMenu)
      CloseRunningUi<UiItInteractMenu>();
    CloseRunningUi<it>();
    _currentUis[it] = ui;
    ui->Show();
    InterfaceOpened.Emit(true);
  }
  
  WindowFramework*             window;
  GameUi&                      _gameUi;
  GameMainBar*                 _mainBar;  
  Sync::ObserverHandler        _obs;
  UiBase*                      _currentUis[UiTotalIt];
};

#endif
