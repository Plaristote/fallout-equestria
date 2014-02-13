#ifndef  UI_BARTER_HPP
# define UI_BARTER_HPP

# include "globals.hpp"
# include "inventory_ui.hpp"
# include "ui_object_quantity_picker.hpp"
# include "cmap/statcontroller.hpp"

class UiBarter : public UiBase, public InventoryViewController
{
public:
  UiBarter(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, ObjectCharacter* other);
  ~UiBarter();

  bool AllowDrop(InventoryView& from, InventoryView& to);

  Sync::Signal<void> BarterEnded;

private:
  void SwapObjects(InventoryObject* object);
  void MakeDeal(Rocket::Core::Event&);
  void BarterEnd(Rocket::Core::Event&);
  void UpdateInterface(void);
  void UpdateInterfaceSide(Rocket::Core::Element* e, Inventory::Content&, StatController*, StatController*);

  int  GetStackValue(Inventory::Content&, StatController*, StatController*);
  void DropInventory(Inventory& from, Inventory& to);
  bool SwapFunctor(InventoryObject* object, Inventory& from, Inventory& to);

  RocketListener          EventMakeDeal, EventBarterEnd;

  Inventory               _stack_player,  _stack_other;
  StatController         *_stats_player, *_stats_other;
  Inventory&              _inventory_player;
  Inventory&              _inventory_other;
  UiObjectQuantityPicker* _quantity_picker;
};

#endif
