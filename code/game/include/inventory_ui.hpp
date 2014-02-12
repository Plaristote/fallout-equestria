#ifndef  INVENTORY_UI_HPP
# define INVENTORY_UI_HPP

# include "globals.hpp"
# include "level/inventory.hpp"
# include <Rocket/Core/Event.h>
# include <Rocket/Core/EventListener.h>
# include <Rocket/Core/Element.h>
# include "rocket_extension.hpp"
# include "level/objects/character.hpp"
#include "as_object.hpp"
# include <sstream>

class UiObjectQuantityPicker : public UiBase
{
public:
  UiObjectQuantityPicker(WindowFramework* window, Rocket::Core::Context* context, const Inventory& inventory, const InventoryObject* object);
  ~UiObjectQuantityPicker();

  Sync::Signal<void (unsigned short)> QuantityPicked;
  Sync::Signal<void>                  Canceled;  
  Sync::ObserverHandler               Observer;

private:
  RocketListener          EventAccepted, EventCanceled, EventIncrement, EventDecrement;
  RocketListener          EventValueChanged;
  
  void                    Accepted(Rocket::Core::Event& event);
  void                    Increment(Rocket::Core::Event& event);

  unsigned short          GetQuantity(void) const;
  void                    SetQuantity(unsigned short);

  unsigned short         _max_quantity;
  Rocket::Core::Element* _line_edit;
};

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

#include <map>
class UiUseSkillOn : public UiBase
{
public:
  UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target);
  virtual ~UiUseSkillOn();
  
  void Destroy(void);
  
  Sync::Signal<void ()>                   Closed;
  Sync::Signal<void (const std::string&)> SkillPicked;
protected:
  UiUseSkillOn(WindowFramework* window, Rocket::Core::Context* context);

  void                   SetSkillList(const std::map<std::string, short>& list);
  void                   Initialize(void);
private:
  RocketListener         EventCloseClicked, EventSkillPicked;
  std::string            skill_picked;
  std::list<std::string> skill_list;
};

class UiUseSpellOn : public UiUseSkillOn
{
public:
  UiUseSpellOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target);
private:
};

class UiBarter : public UiBase, public InventoryViewController
{
public:
  UiBarter(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, ObjectCharacter* other);
  ~UiBarter();
  
  bool AllowDrop(InventoryView& from, InventoryView& to);
  
  Sync::Signal<void> BarterEnded;

private:
  void SwapObjects(InventoryObject* object);
  void MakeDeal(Rocket::Core::Event&);
  void BarterEnd(Rocket::Core::Event&);
  void UpdateInterface(void);
  void UpdateInterfaceSide(Rocket::Core::Element* e, Inventory::Content&, StatController*, StatController*);
  
  int  GetStackValue(Inventory::Content&, StatController*, StatController*);
  void DropInventory(Inventory& from, Inventory& to);
  bool SwapFunctor(InventoryObject* object, Inventory& from, Inventory& to);
  
  RocketListener          EventMakeDeal, EventBarterEnd;

  Inventory               _stack_player,  _stack_other;
  StatController         *_stats_player, *_stats_other;
  Inventory&              _inventory_player;
  Inventory&              _inventory_other;
  UiObjectQuantityPicker* _quantity_picker;
};

class UiLoot : public UiBase
{
public:
  UiLoot(WindowFramework* window, Rocket::Core::Context* context, Inventory& looter, Inventory& looted);
  ~UiLoot();

  void Destroy(void);

  void SetScriptObject(ObjectCharacter* user, InstanceDynamicObject* target, asIScriptContext* context, const std::string& filepath);

  Sync::Signal<void>                      Done;
  Sync::Signal<void (const std::string&)> CannotCarry;

private:
  bool CanSwap(InventoryObject*);
  void SwapObjects(InventoryObject* object);
  void RocketDoneClicked(Rocket::Core::Event&)    { Done.Emit(); }
  void RocketTakeAllClicked(Rocket::Core::Event&);

  RocketListener                               DoneClicked, TakeAllClicked;
  InventoryViewController                      _viewController;
  UiObjectQuantityPicker*                      _quantity_picker;

  Inventory& _looter;
  Inventory& _looted;

  // Scripting Stuff
  AngelScript::Object*   as_object;
  InstanceDynamicObject* target_object;
  ObjectCharacter*       user_character;
};

class UiEquipMode : public UiBase
{
public:
  UiEquipMode(WindowFramework* window, Rocket::Core::Context* context);
  ~UiEquipMode();

  void                               AddOption(unsigned char code, const std::string& name);
  void                               Initialize(void);

  Sync::Signal<void (unsigned char)> EquipModeSelected;
  Sync::Signal<void>                 Closed;

  void Destroy(void);

private:
  RocketListener   ModeClicked, CancelClicked;
  Rocket::Core::Element* root_choices;

  void CallbackCancel(Rocket::Core::Event&) { Closed.Emit(); }
  void ListenButtons(bool);
};

class UiNextZone : public UiBase
{
public:
  UiNextZone(WindowFramework* window, Rocket::Core::Context* context, const std::vector<std::string> zones);
  ~UiNextZone();
  
  Sync::Signal<void (const std::string&)> NextZoneSelected;
  Sync::Signal<void (void)>               Cancel;

private:
  RocketListener LevelSelected, CancelSelected;
  
  void CallbackLevelSelected(Rocket::Core::Event&);
  void CallbackCancel(Rocket::Core::Event&);
  
  std::vector<Rocket::Core::Element*> _elements;
};

#endif
