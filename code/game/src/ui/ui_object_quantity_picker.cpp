#include "ui/ui_object_quantity_picker.hpp"
#include <Rocket/Controls.h>

using namespace std;

UiObjectQuantityPicker::UiObjectQuantityPicker(WindowFramework* window, Rocket::Core::Context* context, const Inventory& inventory, const InventoryObject* object) : UiBase(window, context)
{
  _max_quantity = inventory.ContainsHowMany(object->GetName());
  root         = context->LoadDocument("data/object_quantity_picker.rml");
  if (root)
  {
    Rocket::Core::Element* icon = root->GetElementById("item_icon");

    _line_edit  = root->GetElementById("item_quantity");
    if (_line_edit)
    {
      ToggleEventListener(true, "button_confirm", "click", EventAccepted);
      EventAccepted.EventReceived.Connect(*this, &UiObjectQuantityPicker::Accepted);
    }
    if (icon)
    {
      Rocket::Core::String src("../textures/itemIcons/");

      src += object->GetIcon().c_str();
      icon->SetAttribute("src", src);
    }
    ToggleEventListener(true, "item_minus",    "click",  EventIncrement);
    ToggleEventListener(true, "item_plus",     "click",  EventIncrement);
    ToggleEventListener(true, "item_quantity", "change", EventValueChanged);
    ToggleEventListener(true, "button_cancel", "click",  EventCanceled);
    EventIncrement.EventReceived.Connect(*this, &UiObjectQuantityPicker::Increment);
    EventValueChanged.EventReceived.Connect([this](Rocket::Core::Event&) { SetQuantity(GetQuantity()); });
    EventCanceled.EventReceived.Connect(    [this](Rocket::Core::Event&) { Canceled.Emit();            });
    Canceled.Connect(*this, &UiBase::Hide);
    root->Show(Rocket::Core::ElementDocument::MODAL);
  }
}

UiObjectQuantityPicker::~UiObjectQuantityPicker()
{
  SetModal(false);
  ToggleEventListener(false, "button_confirm", "click",  EventAccepted);
  ToggleEventListener(false, "button_cancel",  "click",  EventCanceled);
  ToggleEventListener(false, "item_quantity",  "change", EventValueChanged);
  ToggleEventListener(false, "item_minus",     "click",  EventIncrement);
  ToggleEventListener(false, "item_plus",      "click",  EventIncrement);
}

void UiObjectQuantityPicker::Accepted(Rocket::Core::Event&)
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_line_edit);
  Rocket::Core::String                  string = control->GetValue();
  std::stringstream                     stream;
  unsigned short                        amount;

  stream << string.CString();
  stream >> amount;
  if (amount <= _max_quantity)
  {
    QuantityPicked.Emit(amount);
    Hide();
  }
}

unsigned short UiObjectQuantityPicker::GetQuantity(void) const
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_line_edit);
  Rocket::Core::String                  string = control->GetValue();
  std::stringstream                     stream;
  unsigned short                        amount;

  stream << string.CString();
  stream >> amount;
  return (amount);
}

void UiObjectQuantityPicker::SetQuantity(unsigned short value)
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_line_edit);
  std::stringstream                     stream;
  std::string                           str;

  if (value > _max_quantity)
    value = _max_quantity;
  stream << value;
  stream >> str;
  control->SetValue(str.c_str());
}

void UiObjectQuantityPicker::Increment(Rocket::Core::Event& event)
{
  const short                           value_change = (event.GetCurrentElement()->GetId() == "item_plus" ? 1 : -1);
  const unsigned short                  quantity     = GetQuantity();

  if (quantity > 0 || value_change != -1)
    SetQuantity(quantity + value_change);
}