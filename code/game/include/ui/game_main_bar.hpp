#ifndef  GAME_MAIN_BAR_HPP
# define GAME_MAIN_BAR_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"

class InventoryObject;
class StatController;

class GameMainBar : public UiBase
{
  friend class GameUi;
public:
  GameMainBar(WindowFramework* window, Rocket::Core::Context* context);
  ~GameMainBar();
  
  void                   AppendToConsole(const std::string& str);
  
  void                   SetStatistics(StatController*);

  void                   SetCurrentAP(unsigned short ap, unsigned short maxap);
  void                   SetMaxAP(unsigned short ap);
  void                   SetEnabledAP(bool);
  
  void                   SetCurrentHp(short hp);
  void                   SetCurrentAc(short ac);
  
  void                   SetEquipedItem(unsigned short it, InventoryObject* item);
  void                   SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt);

  RocketListener         MenuButtonClicked;
  RocketListener         InventoryButtonClicked;
  RocketListener         PersButtonClicked;
  RocketListener         PipbuckButtonClicked;
  RocketListener         SkilldexButtonClicked, SpelldexButtonClicked;
  
  Sync::Signal<void (unsigned short)> UseEquipedItem;
  Sync::Signal<void (unsigned short)> EquipedItemNextAction;
  Sync::Signal<void>                  CombatPassTurn;
  Sync::Signal<void>                  CombatEnd;
  Sync::Signal<void>                  OpenSkilldex;
  Sync::Signal<void>                  OpenSpelldex;
  
private:
  RocketListener         EquipedItem1Clicked;
  RocketListener         EquipedItem2Clicked;
  RocketListener         PassTurnClicked;
  RocketListener         CombatEndClicked;
  
  void                   CallbackEquipedItem1Clicked(Rocket::Core::Event&);
  void                   CallbackEquipedItem2Clicked(Rocket::Core::Event&);
  void                   CallbackPassTurnClicked(Rocket::Core::Event&)  { CombatPassTurn.Emit(); }
  void                   CallbackCombatEndClicked(Rocket::Core::Event&) { CombatEnd.Emit();      }
  
  unsigned short         _apMax;
  bool                   _apEnabled;
  StatController*        statistics;
  Sync::ObserverHandler  statistics_observer;
};

#endif
