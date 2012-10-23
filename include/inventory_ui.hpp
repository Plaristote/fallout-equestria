#ifndef  INVENTORY_UI_HPP
# define INVENTORY_UI_HPP

#include "level/inventory.hpp"

# include <Rocket/Core/Event.h>
# include <Rocket/Core/EventListener.h>
# include <Rocket/Core/Element.h>
# include "rocket_extension.hpp"
# include "level/character.hpp"
# include <sstream>

class InventoryView : public Rocket::Core::EventListener
{
public:
  InventoryView(Rocket::Core::Element* element, Inventory& inventory);
  virtual ~InventoryView(void);
  
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

  virtual bool AllowDrop(InventoryView& from, InventoryView& to) { return (true); }
  
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

class UiBarter : public UiBase, public InventoryViewController
{
public:
  UiBarter(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, ObjectCharacter* other);
  ~UiBarter();
  
  bool AllowDrop(InventoryView& from, InventoryView& to);
  
  Observatory::Signal<void> BarterEnded;

private:
  void SwapObjects(InventoryObject* object);
  void MakeDeal(Rocket::Core::Event&);
  void BarterEnd(Rocket::Core::Event&);
  void UpdateInterface(void);
  void UpdateInterfaceSide(Rocket::Core::Element* e, Inventory::Content&, StatController*, StatController*);
  
  int  GetStackValue(Inventory::Content&, StatController*, StatController*);
  void DropInventory(Inventory& from, Inventory& to);
  bool SwapFunctor(InventoryObject* object, Inventory& from, Inventory& to);
  
  RocketListener   EventMakeDeal, EventBarterEnd;
  
  Inventory        _stack_player,  _stack_other;
  StatController  *_stats_player, *_stats_other;
  Inventory&       _inventory_player;
  Inventory&       _inventory_other;
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

class UiEquipMode : public UiBase
{
public:
  UiEquipMode(WindowFramework* window, Rocket::Core::Context* context, unsigned short it, InventoryObject* object);
  ~UiEquipMode();

  void Destroy(void);
  Observatory::Signal<void (unsigned short, InventoryObject*, EquipedMode)> EquipModeSelected;
  Observatory::Signal<void> Closed;
  
  void             DisableMode(EquipedMode);

private:
  RocketListener   MouthClicked, MagicClicked, BattleSaddleClicked, CancelClicked;
  
  template<EquipedMode mode>
  void CallbackButton(Rocket::Core::Event&) { EquipModeSelected.Emit(_it, &_object, mode); Closed.Emit(); }
  
  void CallbackCancel(Rocket::Core::Event&) { Closed.Emit(); }
  
  unsigned short   _it;
  InventoryObject& _object;
};

class UiNextZone : public UiBase
{
public:
  UiNextZone(WindowFramework* window, Rocket::Core::Context* context, const std::vector<std::string> zones);
  ~UiNextZone();
  
  Observatory::Signal<void (const std::string&)> NextZoneSelected;
  Observatory::Signal<void (void)>               Cancel;

private:
  RocketListener LevelSelected, CancelSelected;
  
  void CallbackLevelSelected(Rocket::Core::Event&);
  void CallbackCancel(Rocket::Core::Event&);
  
  std::vector<Rocket::Core::Element*> _elements;
};

#endif
