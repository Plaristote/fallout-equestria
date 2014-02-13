#ifndef  UI_USE_OBJECT_ON_HPP
# define UI_USE_OBJECT_ON_HPP

# include "globals.hpp"
# include "inventory_ui.hpp"
# include <map>

class UiUseObjectOn : public UiBase
{
public:
  UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory);
  ~UiUseObjectOn();

  void Destroy(void);

  Sync::Signal<void ()>                 ActionCanceled;
  Sync::Signal<void (InventoryObject*)> ObjectSelected;

private:
  void RocketCancelClicked(Rocket::Core::Event&) { ActionCanceled.Emit(); }

  RocketListener                               CancelClicked;
  InventoryViewController                      _viewController;
};

#endif

