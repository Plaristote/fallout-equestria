#ifndef  GAME_INVENTORY_HPP
# define GAME_INVENTORY_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"
# include "ui/inventory_ui.hpp"

class GameInventory : public UiBase
{
  friend class GameUi;
public:
  GameInventory(WindowFramework* window, Rocket::Core::Context* context);
  ~GameInventory();

  void                     SetInventory(Inventory&);
  void                     UpdateInventory(void);

  Sync::Signal<void (const std::string& slot, unsigned short, InventoryObject*)> EquipItem;
  Sync::Signal<void (const std::string& slot, unsigned short)>                   UnequipItem;
  Sync::Signal<void (InventoryObject*)>                    UseObject;
  Sync::Signal<void (InventoryObject*)>                    DropObject;
  Sync::Signal<void (InventoryObject*, InventoryObject*)>  CombineObjects;
  
  void                     SetEquipedItem(unsigned short slot, InventoryObject* object);

private:
  void                     UpdateInventoryCapacity(void);
  void                     SetSelectedObject(InventoryObject*);
  void                     ListenDropables(bool);
  
  RocketListener           DropEvent;
  RocketListener           ButtonUseClicked, ButtonDropClicked;
  void                     CallbackButtonUse(Rocket::Core::Event&)      { if (_selectedObject) UseObject.Emit (_selectedObject);    }
  void                     CallbackButtonDrop(Rocket::Core::Event&)     { if (_selectedObject) DropObject.Emit(_selectedObject);    }
  void                     CallbackDropEvent(Rocket::Core::Event&);

  Inventory*               _inventory;
  InventoryViewController  _inventoryView;
  InventoryObject*         _selectedObject;
};

#endif
