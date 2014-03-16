#include "ui/ui_barter.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"

using namespace std;

UiBarter::UiBarter(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, ObjectCharacter* other) : UiBase(window, context), _inventory_player(player->GetInventory()), _inventory_other(other->GetInventory())
{
  root            = context->LoadDocument("data/barter.rml");
  _quantity_picker = 0;
  _stats_player    = player->GetStatController();
  _stats_other     = other->GetStatController();
  if (root)
  {
    Rocket::Core::Element* eInvLooter   = root->GetElementById("self-inventory");
    Rocket::Core::Element* eInvLooted   = root->GetElementById("other-inventory");
    Rocket::Core::Element* eStackPlayer = root->GetElementById("stack-player");
    Rocket::Core::Element* eStackOther  = root->GetElementById("stack-other");

    ToggleEventListener(true, "button_done", "click", EventMakeDeal);
    ToggleEventListener(true, "button_quit", "click", EventBarterEnd);
    EventMakeDeal.EventReceived.Connect(*this, &UiBarter::MakeDeal);
    EventBarterEnd.EventReceived.Connect(*this, &UiBarter::BarterEnd);
    if (eInvLooter)
      AddView(eInvLooter,   _inventory_player);
    else
      cout << "[UiBarter][Rocket] Missing element self-inventory" << endl;
    if (eInvLooted)
      AddView(eInvLooted,   _inventory_other);
    else
      cout << "[UiBarter][Rocket] Missing element other-inventory" << endl;
    if (eStackPlayer)
      AddView(eStackPlayer, _stack_player);
    else
      cout << "[UiBarter][Rocket] Missing element stack-player" << endl;
    if (eStackOther)
      AddView(eStackOther,  _stack_other);
    else
      cout << "[UiBarter][Rocket] Missing element stack-other" << endl;
    ObjectSelected.Connect(*this, &UiBarter::SwapObjects);
    _stack_player.ContentChanged.Connect(*this, &UiBarter::UpdateInterface);
    _stack_other.ContentChanged.Connect (*this, &UiBarter::UpdateInterface);
    root->Show();    
  }
}

void UiBarter::UpdateInterfaceSide(Rocket::Core::Element* e, Inventory::Content& content, StatController* stats_self, StatController* stats_other)
{
  int                          total = 0;
  stringstream                 str;

  total = GetStackValue(content, stats_self, stats_other);
  str << total << ' ' << i18n::T("caps");
  e->SetInnerRML(str.str().c_str());
}

int  UiBarter::GetStackValue(Inventory::Content& content, StatController* stats_self, StatController* stats_other)
{
  Inventory::Content::iterator it    = content.begin();
  Inventory::Content::iterator end   = content.end();
  int                          total = 0;

  for (; it != end ; ++it)
  {
    if (stats_self && stats_other)
    {
      InventoryObject& object     = *(*it);
      Data             data_self  = stats_self->Model().GetAll();
      Data             data_other = stats_other->Model().GetAll();

      total += stats_self->Model().Action("barter_value", "ooo", &object, &data_self, &data_other);
    }
    else
      total += 5;
  }
  return (total);
}

void UiBarter::UpdateInterface(void)
{
  Rocket::Core::Element* value_player = root->GetElementById("value-player");
  Rocket::Core::Element* value_other  = root->GetElementById("value-other");

  if (value_player)
    UpdateInterfaceSide(value_player, _stack_player.GetContent(), _stats_player, _stats_other);
  if (value_other)
    UpdateInterfaceSide(value_other,  _stack_other.GetContent(),  _stats_other,  _stats_player);  
}

UiBarter::~UiBarter()
{
  if (_quantity_picker) delete _quantity_picker;
  ToggleEventListener(false, "button_done", "click", EventMakeDeal);
  ToggleEventListener(false, "button_quit", "click", EventBarterEnd);
}

void UiBarter::BarterEnd(Rocket::Core::Event&)
{
  Hide();
  DropInventory(_stack_player, _inventory_player);
  DropInventory(_stack_other,  _inventory_other);
  BarterEnded.Emit();
}

bool UiBarter::AllowDrop(InventoryView& from, InventoryView& to)
{
  Inventory* inv_from = &(from.GetInventory());
  Inventory* inv_to   = &(to.GetInventory());   

  if ((inv_from == &_inventory_player && inv_to == &_stack_player) ||
      (inv_from == &_stack_player     && inv_to == &_inventory_player))
    return (true);
  if ((inv_from == &_inventory_other && inv_to == &_stack_other) ||
      (inv_from == &_stack_other     && inv_to == &_inventory_other))
    return (true);
  return (false);
}

bool UiBarter::SwapFunctor(InventoryObject* object, Inventory& from, Inventory& to)
{
  if (from.IncludesObject(object))
  {
    string                          object_name = object->GetName();
    function<void (unsigned short)> swap_callback([this, &from, &to, object_name](unsigned short quantity)
    {
      while (quantity--)
      {
        InventoryObject* swapping = from.GetObject(object_name);

        to.AddObject(swapping);
        from.DelObject(swapping);
      }
      Update();
      if (_quantity_picker)
        _quantity_picker->SetModal(false);
    });
    
    if (from.ContainsHowMany(object->GetName()) > 1)
    {
      if (_quantity_picker) delete _quantity_picker;
      _quantity_picker = new UiObjectQuantityPicker(window, context, from, object);
      _quantity_picker->SetModal(true);
      _quantity_picker->QuantityPicked.Connect(swap_callback);
      _quantity_picker->Observer.Connect(VisibilityToggledOff, *_quantity_picker, &UiBase::Hide);
    }
    else
      swap_callback(1);
    return (true);
  }
  return (false);
}

void UiBarter::SwapObjects(InventoryObject* object)
{
  if (SwapFunctor(object, _stack_player, _inventory_player)) return ;
  if (SwapFunctor(object, _inventory_player, _stack_player)) return ;
  if (SwapFunctor(object, _stack_other, _inventory_other))   return ;
  if (SwapFunctor(object, _inventory_other, _stack_other))   return ;
}

void UiBarter::MakeDeal(Rocket::Core::Event& event)
{
  cout << "Make deal" << endl;
  int  total_player = GetStackValue(_stack_player.GetContent(), _stats_player, _stats_other);
  int  total_other  = GetStackValue(_stack_other.GetContent(),  _stats_other,  _stats_player);
  int  success;

  if (_stats_player && _stats_other)
  {
    cout << "Processing barter test" << endl;
    Data data_player  = _stats_player->Model().GetAll();
    Data data_other   = _stats_other->Model().GetAll();

    success = _stats_player->Model().Action("barter_deal", "oioi", &data_player, total_player, &data_other, total_other);
  }
  else
    success = total_player >= total_other;
  if (success)
  {
    cout << "Successfull barter is successfull" << endl;
    Level::CurrentLevel->PlaySound("pipbuck/barter+");
    DropInventory(_stack_player, _inventory_other);
    DropInventory(_stack_other,  _inventory_player);
    Update();
  }
}

void UiBarter::DropInventory(Inventory& from, Inventory& to)
{
  Inventory::Content::iterator it, end;

  it  = from.GetContent().begin();
  end = from.GetContent().end();
  while (it != end)
  {
    to.AddObject(*it);
    from.DelObject(*it);
    it = from.GetContent().begin();
  }
}
