#include "gameui.hpp"

using namespace std;
using namespace Rocket;

void GameInventory::ListenDropables(bool activate)
{
  Core::ElementList elements;

  _root->GetElementsByTagName(elements, "itemslot");
  std::for_each(elements.begin(), elements.end(), [this, activate](Rocket::Core::Element* element)
  {
    Rocket::Core::Element* to_listen = element->GetChild(0);

    if (to_listen)
    {
      if (activate)
        to_listen->AddEventListener("dragdrop", &DropEvent);
      else
        to_listen->RemoveEventListener("dragdrop", &DropEvent);
    }
  });
}

GameInventory::GameInventory(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  if ((_root = context->LoadDocument("data/inventory.rml")) != 0)
  {
    Core::Element*         itemListContainer = _root->GetElementById("body-inventory-items");

    for (unsigned short i = 0 ; i < 200 ; ++i)
    {
      Rocket::Core::Element* item = _root->CreateElement("invitem");

      item->SetInnerRML("<img src=\"item.png\" class='inventory-item' />");
      itemListContainer->AppendChild(item);
    }
    
    ToggleEventListener(true, "button_use",       "click", ButtonUseClicked);
    ToggleEventListener(true, "button_drop",      "click", ButtonDropClicked);
    ListenDropables(true);
    ToggleEventListener(true, "body-inventory-items", "dragdrop", DropEvent);

    ButtonUseClicked.EventReceived.Connect   (*this, &GameInventory::CallbackButtonUse);
    ButtonDropClicked.EventReceived.Connect  (*this, &GameInventory::CallbackButtonDrop);
    DropEvent.EventReceived.Connect          (*this, &GameInventory::CallbackDropEvent);

    Translate();
    _root->Show();
  }
  _inventoryView.ObjectSelected.Connect(*this, &GameInventory::SetSelectedObject);
  _selectedObject = 0;
  _inventory      = 0;
}

GameInventory::~GameInventory()
{
  ToggleEventListener(false, "button_use",       "click", ButtonUseClicked);
  ToggleEventListener(false, "button_drop",      "click", ButtonDropClicked);
  ListenDropables(false);
}

void GameInventory::CallbackDropEvent(Rocket::Core::Event& event)
{
  Rocket::Core::Element* drag_element = *static_cast<Rocket::Core::Element**>(event.GetParameter< void* >("drag_element", NULL));
  Rocket::Core::Element* target       = event.GetTargetElement();
  Rocket::Core::Element* from         = drag_element;

  while (target && std::string(target->GetClassNames().CString()).find("drop-target") == std::string::npos)
    target = target->GetParentNode();
  while (from && std::string(from->GetClassNames().CString()).find("drop-target") == std::string::npos)
    from   = from->GetParentNode();
  if (drag_element && target && from)
  {
    std::stringstream    str_count;
    unsigned int         count;
    auto                 obj_it  = _inventory->GetContent().begin();
    auto                 obj_end = _inventory->GetContent().end();

    Rocket::Core::String tmp;
    drag_element->GetInnerRML(tmp);

    str_count << drag_element->GetId().CString();
    str_count >> count;
    std::advance(obj_it, count);
    if (obj_it != obj_end)
    {
      InventoryObject* object          = *obj_it;
      Core::Variant*   target_slot_var = target->GetAttribute("data-target");
      Core::Variant*   target_num_var  = target->GetAttribute("data-slot");
      Core::Variant*   from_slot_var   = from->GetAttribute("data-target");
      Core::Variant*   from_num_var    = from->GetAttribute("data-slot");

      if (from_slot_var)
      {
        unsigned int slot_num  = (from_num_var ? from_num_var->Get<unsigned int>() : 0);
        std::string  slot_name = from_slot_var->Get<Core::String>().CString();

        UnequipItem.Emit(slot_name, slot_num);
      }
      if (target_slot_var)
      {
        unsigned int slot_num  = (target_num_var ? target_num_var->Get<unsigned int>() : 0);
        std::string  slot_name = target_slot_var->Get<Core::String>().CString();

        EquipItem.Emit(slot_name, slot_num, object);
      }
    }
  }
}

void GameInventory::SetEquipedItem(unsigned short slot, InventoryObject* item)
{
  std::stringstream elem_id;

  elem_id << "eq" << (slot + 1) << "-equiped";
  if (_inventory)
  {
    Rocket::Core::Element* element = _root->GetElementById(elem_id.str().c_str());

    if (element)
    {
      std::stringstream rml;

      if (item)
      {
        int item_count = _inventory->GetObjectIterator(item);

        rml << "<p id='" << item_count << "' class='equiped_image";
#ifdef INVENTORY_USE_DRAGDROP
        if (!(item->IsHidden()))
          rml << " inventory-item-draggable";
#endif
        if ((*item)["icon"].Value() != "")
          rml << "'><img src='../textures/itemIcons/" << (*item)["icon"].Value() << "' />";
        rml << "</p>";
      }
      element->SetInnerRML(rml.str().c_str());
    }
  }
  else
    cout << "GameInventory: no inventory set" << endl;
}

void GameInventory::SetInventory(Inventory& inventory)
{
  Core::Element* itemListContainer = _root->GetElementById("body-inventory-items");

  if (itemListContainer)
  {
    _inventory = &inventory;
    _inventoryView.AddView(itemListContainer, *_inventory);
  }
  inventory.ContentChanged.Connect(*this, &GameInventory::UpdateInventory);
  UpdateInventoryCapacity();
}

void GameInventory::UpdateInventory(void)
{
  _inventoryView.Update();
  UpdateInventoryCapacity();
  SetSelectedObject(0);
}

void GameInventory::UpdateInventoryCapacity(void)
{
  Core::Element* capacity = _root->GetElementById("inventory-capacity");
  
  if (capacity)
  {
    stringstream stream;
    
    stream << "Weight: " << (int)(_inventory->GetCurrentWeight()) << " / " << (int)(_inventory->GetCapacity()) << " kg";
    capacity->SetInnerRML(stream.str().c_str());
  }
}

void GameInventory::SetSelectedObject(InventoryObject* inventory)
{
  Core::Element* itemDescription = _root->GetElementById("item-description");

  _selectedObject = inventory;
  if (itemDescription)
  {
    if (_selectedObject == 0)
      itemDescription->SetInnerRML("Seleccionar objeto");
    else
      itemDescription->SetInnerRML(_selectedObject->GetName().c_str());
  }
}

