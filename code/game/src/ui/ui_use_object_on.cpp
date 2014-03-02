#include "ui/ui_use_object_on.hpp"

using namespace std;

UiUseObjectOn::UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory) : UiBase(window, context)
{
  _root = context->LoadDocument("data/useobjecton.rml");
  if (_root)
  {
    Rocket::Core::Element* eInventory = _root->GetElementById("inventory");

    ToggleEventListener(true, "button_cancel", "click", CancelClicked);
    CancelClicked.EventReceived.Connect(*this, &UiUseObjectOn::RocketCancelClicked);
    if (eInventory)
    {
      _viewController.AddView(eInventory, inventory);
      _viewController.ObjectSelected.Connect(ObjectSelected, &Sync::Signal<void (InventoryObject*)>::Emit);
    }
    _root->Show();
  }
  ObjectSelected.Connect([this](InventoryObject*) { Closed.Emit(); });
}

UiUseObjectOn::~UiUseObjectOn()
{
  ToggleEventListener(false, "button_cancel", "click", CancelClicked);
  _viewController.Destroy();
  if (_root)
  {
    _root->Close();
    _root->RemoveReference();
    _root = 0;
  }
}

void UiUseObjectOn::Destroy(void)
{
  if (_root)
    _root->Hide();
}