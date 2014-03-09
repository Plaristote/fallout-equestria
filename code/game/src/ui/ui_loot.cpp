#include "ui/ui_loot.hpp"

using namespace std;

UiLoot::UiLoot(WindowFramework* window, Rocket::Core::Context* context, Inventory& looter, Inventory& looted) : UiBase(window, context), _looter(looter), _looted(looted)
{
  _quantity_picker = 0;
  as_object        = 0;
  root            = context->LoadDocument("data/looting.rml");
  if (root)
  {
    Rocket::Core::Element* eInvLooter = root->GetElementById("self-inventory");
    Rocket::Core::Element* eInvLooted = root->GetElementById("other-inventory");
    
    ToggleEventListener(true, "button_done",     "click", DoneClicked);
    ToggleEventListener(true, "button_take_all", "click", TakeAllClicked);
    DoneClicked.EventReceived.Connect(*this, &UiLoot::RocketDoneClicked);
    TakeAllClicked.EventReceived.Connect(*this, &UiLoot::RocketTakeAllClicked);
    if (eInvLooter)
      _viewController.AddView(eInvLooter, looter);
    if (eInvLooted)
      _viewController.AddView(eInvLooted, looted);
    _viewController.ObjectSelected.Connect(*this, &UiLoot::SwapObjects);
    _viewController.SetCanSwap([this](InventoryObject* object) -> bool { return (CanSwap(object)); });
    root->Show();
  }
}

UiLoot::~UiLoot()
{
  if (_quantity_picker) delete _quantity_picker;
  if (as_object)        delete as_object;
  _viewController.Destroy();
  if (root)
  {
    ToggleEventListener(false, "button_done",     "click", DoneClicked);
    ToggleEventListener(false, "button_take_all", "click", TakeAllClicked);
  }
}

void UiLoot::Destroy(void)
{
  if (root)
    root->Hide();
}

void UiLoot::SetScriptObject(ObjectCharacter* user, InstanceDynamicObject* target, asIScriptContext* context, const std::string& filepath)
{
  if (as_object) delete as_object;
  as_object = new AngelScript::Object(context, filepath);
  as_object->asDefineMethod("LootingTest",        "bool LootingTest(DynamicObject@,Character@,Item@)");
  as_object->asDefineMethod("LootingFailureTest", "bool LootingFailureTest()");
  user_character = user;
  target_object  = target;
}

bool UiLoot::CanSwap(InventoryObject* object)
{
  bool can_swap = true;

  if (as_object)
  {
    AngelScript::Type<InventoryObject*>       param1(object);
    AngelScript::Type<ObjectCharacter*>       param2(user_character);
    AngelScript::Type<InstanceDynamicObject*> param3(target_object);

    can_swap = as_object->Call("LootingTest", 3, &param3, &param2, &param1);
    if (!can_swap)
    {
      if ((bool)(as_object->Call("LootingFailureTest")))
        Done.Emit();
    }
  }
  return (can_swap);
}

void UiLoot::SwapObjects(InventoryObject* object)
{
  bool fromLooted = false;

  std::for_each(_looted.GetContent().begin(), _looted.GetContent().end(), [&fromLooted, object](InventoryObject* ptr)
  {
    if (object == ptr)
      fromLooted = true;
  });

  string     object_name = object->GetName();
  Inventory& looted      = (fromLooted ? _looted : _looter);
  Inventory& looter      = (fromLooted ? _looter : _looted);
  function<void (unsigned short)> loot_callback([this, &looted, &looter, object_name](unsigned short quantity)
  {
    while (quantity--)
    {
      InventoryObject* booty = looted.GetObject(object_name);

      if (!(CanSwap(booty)))
        return ;
      if (_looter.CanCarry(booty))
      {
        looter.AddObject(booty);
        looted.DelObject(booty);
      }
      else
        CannotCarry.Emit(object_name);
    }
    _viewController.Update();
    if (_quantity_picker)
      _quantity_picker->SetModal(false);
  });

  if (looted.ContainsHowMany(object->GetName()) > 1)
  {
    if (_quantity_picker) delete _quantity_picker;
    _quantity_picker = new UiObjectQuantityPicker(window, context, looted, object);
    _quantity_picker->Show();
    _quantity_picker->SetModal(true);
    _quantity_picker->QuantityPicked.Connect(loot_callback);
    _quantity_picker->Observer.Connect(VisibilityToggledOff, *_quantity_picker, &UiBase::Hide);
  }
  else
    loot_callback(1);
}

void UiLoot::RocketTakeAllClicked(Rocket::Core::Event&)
{
  Inventory::Content::iterator it  = _looted.GetContent().begin();

  while (it != _looted.GetContent().end())
  {
    InventoryObject* object = *it;
    bool             hidden = (*object)["hidden"] == "1";

    if (!hidden && _looter.CanCarry(object))
    {
      if (!(CanSwap(object)))
        return ;
      _looter.AddObject(object);
      _looted.DelObject(object);
      it = _looted.GetContent().begin();
    }
    else
      ++it;
  }
  _viewController.Update();
}