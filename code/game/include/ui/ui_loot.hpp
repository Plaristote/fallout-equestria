#ifndef  UI_LOOT_HPP
# define UI_LOOT_HPP

# include "globals.hpp"
# include "inventory_ui.hpp"
# include "ui_object_quantity_picker.hpp"

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

#endif

