#include "ui/ui_use_object_on.hpp"

using namespace std;

UiUseObjectOn::UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory) : UiBase(window, context)
{
  root = context->LoadDocument("data/useobjecton.rml");
  if (root)
  {
    Rocket::Core::Element* eInventory = root->GetElementById("inventory");

    ToggleEventListener(true, "button_cancel", "click", CancelClicked);
    CancelClicked.EventReceived.Connect(*this, &UiUseObjectOn::RocketCancelClicked);
    if (eInventory)
    {
      _viewController.AddView(eInventory, inventory);
      _viewController.ObjectSelected.Connect(ObjectSelected, &Sync::Signal<void (InventoryObject*)>::Emit);
    }
    root->Show();
  }
  ObjectSelected.Connect([this](InventoryObject*) { Closed.Emit(); });
}

UiUseObjectOn::~UiUseObjectOn()
{
  ToggleEventListener(false, "button_cancel", "click", CancelClicked);
  _viewController.Destroy();
}

void UiUseObjectOn::Destroy(void)
{
  if (root)
    root->Hide();
}