#ifndef  INVENTORY_UI_HPP
# define INVENTORY_UI_HPP

# include "globals.hpp"
# include "level/inventory.hpp"
# include <Rocket/Core/Event.h>
# include <Rocket/Core/EventListener.h>
# include <Rocket/Core/Element.h>
# include "rocket_extension.hpp"

class InventoryView : public Rocket::Core::EventListener
{
public:
  InventoryView(Rocket::Core::Element* element, Inventory& inventory);
  virtual ~InventoryView(void);
  
  bool operator==(Rocket::Core::Element* element);

  Sync::Signal<void (InventoryView*, Rocket::Core::Element*)> ObjectDropped;
  Sync::Signal<void (InventoryObject*)>                       ObjectSelected;
  Sync::Signal<void (InventoryObject*)>                       ObjectMenuRequested;
  Sync::Signal<void (InventoryObject*)>                       ObjectFocused;

  void             UpdateView(void);
  void             Destroy(void);
  InventoryObject* GetObjectFromId(const std::string& id);
  void             ProcessEvent(Rocket::Core::Event& event);
  
  Inventory&             GetInventory(void) { return (_inventory); }

private:
  Rocket::Core::Element& _element;
  Inventory&             _inventory;
};

class InventoryViewController
{
public:
  Sync::Signal<void (InventoryObject*)> ObjectSelected;
  Sync::Signal<void (InventoryObject*)> ObjectMenuRequested;
  Sync::Signal<void (InventoryObject*)> ObjectFocused;
  
  void AddView(Rocket::Core::Element* element, Inventory& inventory);
  void DragObserver(InventoryView* container, Rocket::Core::Element* element);
  void Destroy(void);
  void Update(void);

  virtual bool AllowDrop(InventoryView& from, InventoryView& to)
  {
    if (_allow_drop)
      _allow_drop(from, to);
    return (true);
  }
  
  bool CanSwap(InventoryObject* object) const
  {
    if (_can_swap)
      return (_can_swap(object));
    return (true);
  }

  void SetCanSwap(std::function<bool (InventoryObject*)> can_swap) { _can_swap = can_swap; }

private:
  std::vector<InventoryView*>                          _views;
  std::function<bool (InventoryView&, InventoryView&)> _allow_drop;
  std::function<bool (InventoryObject*)>               _can_swap;
};

#endif
