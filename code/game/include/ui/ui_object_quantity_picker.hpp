#ifndef  UI_OBJECT_QUANTITY_PICKER_HPP
# define UI_OBJECT_QUANTITY_PICKER_HPP

# include "globals.hpp"
# include "rocket_extension.hpp"
# include "level/inventory.hpp"

class UiObjectQuantityPicker : public UiBase
{
public:
  UiObjectQuantityPicker(WindowFramework* window, Rocket::Core::Context* context, const Inventory& inventory, const InventoryObject* object);
  ~UiObjectQuantityPicker();

  Sync::Signal<void (unsigned short)> QuantityPicked;
  Sync::Signal<void>                  Canceled;
  Sync::ObserverHandler               Observer;

private:
  RocketListener          EventAccepted, EventCanceled, EventIncrement, EventDecrement;
  RocketListener          EventValueChanged;

  void                    Accepted(Rocket::Core::Event& event);
  void                    Increment(Rocket::Core::Event& event);

  unsigned short          GetQuantity(void) const;
  void                    SetQuantity(unsigned short);

  unsigned short         _max_quantity;
  Rocket::Core::Element* _line_edit;
};

#endif

