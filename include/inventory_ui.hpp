#ifndef  INVENTORY_UI_HPP
# define INVENTORY_UI_HPP

#include "inventory.hpp"

# include <Rocket/Core/Event.h>
# include <Rocket/Core/EventListener.h>
# include <Rocket/Core/Element.h>
# include "rocket_extension.hpp"
# include <sstream>

class InventoryView : public Rocket::Core::EventListener
{
public:
  InventoryView(Rocket::Core::Element* element, Inventory& inventory);
  
  bool operator==(Rocket::Core::Element* element);

  Observatory::Signal<void (InventoryView*, Rocket::Core::Element*)> ObjectDropped;
  Observatory::Signal<void (InventoryObject*)>                       ObjectSelected;
  Observatory::Signal<void (InventoryObject*)>                       ObjectMenuRequested;
  Observatory::Signal<void (InventoryObject*)>                       ObjectFocused;

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
  Observatory::Signal<void (InventoryObject*)> ObjectSelected;
  Observatory::Signal<void (InventoryObject*)> ObjectMenuRequested;
  Observatory::Signal<void (InventoryObject*)> ObjectFocused;
  
  void AddView(Rocket::Core::Element* element, Inventory& inventory);
  void DragObserver(InventoryView* container, Rocket::Core::Element* element);
  void Destroy(void);
  void Update(void);
  
private:
  std::vector<InventoryView*> _views;
};

class UiUseObjectOn : public UiBase
{
public:
  UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory);
  ~UiUseObjectOn();  

  void Destroy(void);
  
  Observatory::Signal<void ()>                 ActionCanceled;
  Observatory::Signal<void (InventoryObject*)> ObjectSelected;

private:
  void RocketCancelClicked(Rocket::Core::Event&) { ActionCanceled.Emit(); }

  RocketListener                               CancelClicked;
  InventoryViewController                      _viewController;
};

class UiLoot : public UiBase
{
public:
  UiLoot(WindowFramework* window, Rocket::Core::Context* context, Inventory& looter, Inventory& looted);
  ~UiLoot();

  void Destroy(void);
  
  Observatory::Signal<void> Done;

private:
  void SwapObjects(InventoryObject* object);
  void RocketDoneClicked(Rocket::Core::Event&)    { Done.Emit(); }
  void RocketTakeAllClicked(Rocket::Core::Event&);

  RocketListener                               DoneClicked, TakeAllClicked;
  InventoryViewController                      _viewController;

  Inventory& _looter;
  Inventory& _looted;
};

#endif