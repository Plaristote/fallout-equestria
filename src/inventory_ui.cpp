#include "inventory_ui.hpp"
#include <panda3d/pandaVersion.h>

#if PANDA_MAJOR_VERSION > 1 || PANDA_MINOR_VERSION > 8
# define INVENTORY_USE_DRAGDROP
#endif

/*
 * InventoryView
 */
InventoryView::InventoryView(Rocket::Core::Element* element, Inventory& inventory) : _element(*element), _inventory(inventory)
{
#ifdef INVENTORY_USE_DRAGDROP
  //element->AddEventListener("dragstart", this);
  element->AddEventListener("dragdrop",  this);
#endif
  UpdateView();
}

InventoryView::~InventoryView()
{
  _element.RemoveEventListener("dragdrop", this);
}

bool InventoryView::operator==(Rocket::Core::Element* element)
{
  return (element == &_element);
}

Observatory::Signal<void (InventoryView*, Rocket::Core::Element*)> ObjectDropped;
Observatory::Signal<void (InventoryObject*)>                       ObjectSelected;
Observatory::Signal<void (InventoryObject*)>                       ObjectMenuRequested;
Observatory::Signal<void (InventoryObject*)>                       ObjectFocused;

void InventoryView::UpdateView(void)
{
  Inventory::Content&          content = _inventory.GetContent();
  Inventory::Content::iterator it      = content.begin();
  Inventory::Content::iterator end     = content.end();
  std::string                  rml;
  unsigned short               count;

  Destroy();

  for (count = 0 ; it != end ; ++it, ++count)
  {
    InventoryObject&  item = *(*it);
    
    item["hidden"].Nil();
    item["hidden"].Value() != "1";
    item.IsEquiped();
    if ((item["hidden"].Nil() || item["hidden"].Value() != "1") && !(item.IsEquiped()))
    {
      std::stringstream stream;
      bool              notVisible = false;
      unsigned short    quantity   = 1;

      // Look for groupable objects
      {
	Inventory::Content::iterator groupIt     = content.begin();
	Inventory::Content::iterator groupEnd    = content.end();
	bool                         hideIfFound = true;

	for (; groupIt != groupEnd ; ++groupIt)
	{
	  if (groupIt == it)
	  {
	    hideIfFound = false;
	    continue ;
	  }
	  if ((*groupIt)->IsGroupableWith(*it))
	  {
	    if ((*groupIt)->IsEquiped())
	      continue ;
	    if (hideIfFound)
	    {
	      notVisible = true;
	      break ;
	    }
	    quantity++;
	  }
	}
      }

      if (!notVisible)
      {
	stream << "<span class='inventory-item-icon";
#ifdef INVENTORY_USE_DRAGDROP
	stream << " inventory-item-draggable";
#endif
	stream << "' id='" << count << "'>";
        stream << "<img src='../textures/itemIcons/";
	if (item["icon"].Value() != "")
	  stream << item["icon"].Value();
	else
	  stream << "default.png";
	stream << "' />";
	if (quantity > 1)
	  stream << "<span class='inventory-item-quantity'>x" << quantity << "</span>";
	stream << "</span>";
        rml += stream.str();
      }
    }
  }
  _element.SetInnerRML(rml.c_str());
  
  for (unsigned short i = 0 ; i < _element.GetNumChildren() ; ++i)
  {
    _element.GetChild(i)->AddEventListener("dblclick",  this);
    _element.GetChild(i)->AddEventListener("mouseover", this);
    _element.GetChild(i)->AddEventListener("click",     this);
  }
}

void             InventoryView::Destroy()
{
  Rocket::Core::Element* element;
  
  for (int i = 0 ; (element = _element.GetChild(i)) != 0 ; ++i)
  {
    element->RemoveEventListener("dblclick",  this);
    element->RemoveEventListener("mouseover", this);
    element->RemoveEventListener("click",     this);
  }
}

InventoryObject* InventoryView::GetObjectFromId(const std::string& id)
{
  Inventory::Content&          content = _inventory.GetContent();
  Inventory::Content::iterator it      = content.begin();
  Inventory::Content::iterator end     = content.end();
  std::stringstream            stream;
  unsigned short               count, intIt;

  stream << id;
  stream >> count;
  for (intIt = 0 ; it != end && intIt != count ; ++it, ++intIt);
  if (it != end)
    return (*it);
  return (0);
}

void InventoryView::ProcessEvent(Rocket::Core::Event& event)
{
  if (event == "dragdrop")
  {
    Rocket::Core::Element* drag_element   = static_cast<Rocket::Core::Element*>(event.GetParameter< void* >("drag_element", NULL));

    if (drag_element->GetParentNode() != &_element)
      ObjectDropped.Emit(this, drag_element);
  }
  else
  {
    Rocket::Core::Element* currentElement = event.GetCurrentElement();
    InventoryObject*       object         = GetObjectFromId(currentElement->GetId().CString());

    if (event == "dblclick")
      ObjectSelected.Emit(object);
    else if (event == "mouseover")
      ObjectFocused.Emit(object);
  }
}

/*
 * InventoryViewController
 */
void InventoryViewController::AddView(Rocket::Core::Element* element, Inventory& inventory)
{
  InventoryView& view = *(new InventoryView(element, inventory));

  _views.push_back(&view);
  view.ObjectDropped.Connect      (*this,               &InventoryViewController::DragObserver);
  view.ObjectFocused.Connect      (ObjectFocused,       &Observatory::Signal<void (InventoryObject*)>::Emit);
  view.ObjectSelected.Connect     (ObjectSelected,      &Observatory::Signal<void (InventoryObject*)>::Emit);
  view.ObjectMenuRequested.Connect(ObjectMenuRequested, &Observatory::Signal<void (InventoryObject*)>::Emit);
}

void InventoryViewController::DragObserver(InventoryView* container, Rocket::Core::Element* element)
{
  std::vector<InventoryView*>::iterator itView = _views.begin();
  std::vector<InventoryView*>::iterator end    = _views.end();

  for (; itView != end && (!((*(*itView)) == element->GetParentNode())) ; ++itView);
  if (itView != _views.end())
  {
    InventoryView&   view = (*(*itView));
    InventoryObject* object;

    if (AllowDrop(view, *container))
    {
      object = view.GetObjectFromId(std::string(element->GetId().CString()));
      view.GetInventory().DelObject(object);
      container->GetInventory().AddObject(object);
      view.UpdateView();
      container->UpdateView();
    }
  }
}

void InventoryViewController::Destroy(void)
{
  std::for_each(_views.begin(), _views.end(), [](InventoryView* view)
  {
    view->Destroy();
    delete view;
  });
  _views.clear();
}

void InventoryViewController::Update(void)
{
  std::for_each(_views.begin(), _views.end(), [](InventoryView* view) { view->UpdateView(); });
}

/*
 * UiUseObjectOn
 */
UiUseObjectOn::UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory) : UiBase(window, context)
{
  _root = context->LoadDocument("data/useobjecton.rml");
  if (_root)
  {
    Rocket::Core::Element* eInventory = _root->GetElementById("inventory");

    ToggleEventListener(true, "button_cancel", "click", CancelClicked);
    CancelClicked.EventReceived.Connect(*this, &UiUseObjectOn::RocketCancelClicked);
    if (eInventory)
    {
      _viewController.AddView(eInventory, inventory);
      _viewController.ObjectSelected.Connect(ObjectSelected, &Observatory::Signal<void (InventoryObject*)>::Emit);
    }
    _root->Show();
  }
}

UiUseObjectOn::~UiUseObjectOn()
{
  ToggleEventListener(false, "button_cancel", "click", CancelClicked);
  _viewController.Destroy();
  if (_root)
  {
    _root->Close();
    _root->RemoveReference();
    _root = 0;
  }
}

void UiUseObjectOn::Destroy(void)
{
  if (_root)
    _root->Hide();
}

/*
 * UiLoot
 */
UiLoot::UiLoot(WindowFramework* window, Rocket::Core::Context* context, Inventory& looter, Inventory& looted) : UiBase(window, context), _looter(looter), _looted(looted)
{
  _root = context->LoadDocument("data/looting.rml");
  if (_root)
  {
    Rocket::Core::Element* eInvLooter = _root->GetElementById("self-inventory");
    Rocket::Core::Element* eInvLooted = _root->GetElementById("other-inventory");
    
    ToggleEventListener(true, "button_done",     "click", DoneClicked);
    ToggleEventListener(true, "button_take_all", "click", TakeAllClicked);
    DoneClicked.EventReceived.Connect(*this, &UiLoot::RocketDoneClicked);
    TakeAllClicked.EventReceived.Connect(*this, &UiLoot::RocketTakeAllClicked);
    if (eInvLooter)
      _viewController.AddView(eInvLooter, looter);
    if (eInvLooted)
      _viewController.AddView(eInvLooted, looted);
    _viewController.ObjectSelected.Connect(*this, &UiLoot::SwapObjects);
    _root->Show();
  }
}

UiLoot::~UiLoot()
{
  _viewController.Destroy();
  if (_root)
  {
    ToggleEventListener(false, "button_done",     "click", DoneClicked);
    ToggleEventListener(false, "button_take_all", "click", TakeAllClicked);
    _root->Close();
    _root->RemoveReference();
    _root = 0;
  }
}

void UiLoot::Destroy(void)
{
  if (_root)
    _root->Hide();
}

void UiLoot::SwapObjects(InventoryObject* object)
{
  bool fromLooted = false;

  std::for_each(_looted.GetContent().begin(), _looted.GetContent().end(), [&fromLooted, object](InventoryObject* ptr)
  {
    if (object == ptr)
      fromLooted = true;
  });
  
  Inventory& looted = (fromLooted ? _looted : _looter);
  Inventory& looter = (fromLooted ? _looter : _looted);
  
  if (looter.CanCarry(object))
  {
    looter.AddObject(object);
    looted.DelObject(object);
  }
  _viewController.Update();
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
      _looter.AddObject(object);
      _looted.DelObject(object);
      it = _looted.GetContent().begin();
    }
    else
      ++it;
  }
  _viewController.Update();
}

/*
 * Dialog Equip Mode
 */
UiEquipMode::UiEquipMode(WindowFramework* window, Rocket::Core::Context* context, unsigned short it, InventoryObject* object)
  : UiBase(window, context), _it(it), _object(*object)
{
  _root = context->LoadDocument("data/dialog_equiped_mode.rml");
  if (_root)
  {
    Rocket::Core::Element* eDialog       = _root->GetElementById("dialog-actions");
    Rocket::Core::Element* eMouth        = _root->GetElementById("mode_mouth");
    Rocket::Core::Element* eMagic        = _root->GetElementById("mode_magic");
    Rocket::Core::Element* eBattleSaddle = _root->GetElementById("mode_battlesaddle");

    if (eDialog)
    {
      if (eMouth)
      {
	if (_object["mode-mouth"].Value() != "1")
	  eDialog->RemoveChild(eMouth);
	else
	  eMouth->AddEventListener("click", &MouthClicked);
      }
      if (eMagic)
      {
	if (_object["mode-magic"].Value() != "1")
	  eDialog->RemoveChild(eMagic);
	else
	  eMagic->AddEventListener("click", &MagicClicked);
      }
      if (eBattleSaddle)
      {
	if (_object["mode-battlesaddle"].Value() != "1")
	  eDialog->RemoveChild(eBattleSaddle);
	else
	  eBattleSaddle->AddEventListener("click", &BattleSaddleClicked);
      }
    }
    ToggleEventListener(true, "cancel", "click", CancelClicked);
    
    MouthClicked.EventReceived.Connect(*this, &UiEquipMode::CallbackButton<EquipedMouth>);
    MagicClicked.EventReceived.Connect(*this, &UiEquipMode::CallbackButton<EquipedMagic>);
    BattleSaddleClicked.EventReceived.Connect(*this, &UiEquipMode::CallbackButton<EquipedBattleSaddle>);
    CancelClicked.EventReceived.Connect(*this, &UiEquipMode::CallbackCancel);
    
    _root->Show();
  }
}

UiEquipMode::~UiEquipMode()
{
  if (_root)
  {
    ToggleEventListener(false, "cancel",            "click", CancelClicked);
    ToggleEventListener(false, "mode_mouth",        "click", MouthClicked);
    ToggleEventListener(false, "mode_magic",        "click", MagicClicked);
    ToggleEventListener(false, "mode_battlesaddle", "click", BattleSaddleClicked);
  }
}

void UiEquipMode::DisableMode(EquipedMode mode)
{
  Rocket::Core::Element* eDialog = _root->GetElementById("dialog-actions");
  Rocket::Core::Element* element;
  
  if (!eDialog)
    return ;
  switch (mode)
  {
    case EquipedMouth:
      element = _root->GetElementById("mode-mouth");
      break ;
    case EquipedMagic:
      element = _root->GetElementById("mode-magic");
      break ;
    case EquipedBattleSaddle:
      element = _root->GetElementById("mode-battlesaddle");
      break ;
  }
  if (element)
    eDialog->RemoveChild(element);
}

void UiEquipMode::Destroy(void)
{
  if (_root)
    _root->Hide();
}

/*
 * UiNextZone
 */
using namespace std;

UiNextZone::UiNextZone(WindowFramework* window, Rocket::Core::Context* context, const std::vector<std::string> zones)
  : UiBase(window, context)
{
  _root = context->LoadDocument("data/dialog_zone_selector.rml");
  if (_root)
  {
    Rocket::Core::Element*         eContainer = _root->GetElementById("choices");
    vector<string>::const_iterator it         = zones.begin();
    vector<string>::const_iterator end        = zones.end();
    Rocket::Core::String           lastRml;
    stringstream                   rml;
    
    for(short n = 1 ; it != end ; ++it, ++n)
    {
      rml << "<button id='choice-" << n << "' class='button_menu' zone='" << *it << "'>";
      rml << *it;
      rml << "</button>";
      rml << "<br />";
    }
    rml << "<button id='cancel' class='button_menu'>Stay here</button>";
    eContainer->GetInnerRML(lastRml);
    eContainer->SetInnerRML(lastRml + rml.str().c_str());

    {
      ToggleEventListener(true, "cancel", "click", CancelSelected);
      for (short n = 1 ; n <= zones.size() ; ++n)
      {
	stringstream           name;
	Rocket::Core::Element* zoneButton;

	name << "choice-" << n;
	zoneButton = _root->GetElementById(name.str().c_str());
	if (zoneButton)
	{
	  _elements.push_back(zoneButton);
	  zoneButton->AddEventListener("click", &LevelSelected);
	}
      }
      LevelSelected.EventReceived.Connect (*this, &UiNextZone::CallbackLevelSelected);
      CancelSelected.EventReceived.Connect(*this, &UiNextZone::CallbackCancel);
    }
    _root->Show();
  }
}

void UiNextZone::CallbackLevelSelected(Rocket::Core::Event& event)
{
  Rocket::Core::String name = event.GetCurrentElement()->GetId();
  Rocket::Core::String str  = event.GetCurrentElement()->GetAttribute("zone")->Get<Rocket::Core::String>();
  string               tmp = str.CString();

  NextZoneSelected.Emit(tmp);
}

void UiNextZone::CallbackCancel(Rocket::Core::Event&)
{
  Cancel.Emit();
}

UiNextZone::~UiNextZone()
{
  ToggleEventListener(false, "cancel", "click", CancelSelected);
  for_each(_elements.begin(), _elements.end(), [this](Rocket::Core::Element* zoneButton)
  { zoneButton->RemoveEventListener("click", &LevelSelected); });
}

/*
 * UiBarter
 */
UiBarter::UiBarter(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, ObjectCharacter* other) : UiBase(window, context), _inventory_player(player->GetInventory()), _inventory_other(other->GetInventory())
{
  _root = context->LoadDocument("data/barter.rml");
  _stats_player = player->GetStatController();
  _stats_other  = other->GetStatController();
  if (_root)
  {
    Rocket::Core::Element* eInvLooter   = _root->GetElementById("self-inventory");
    Rocket::Core::Element* eInvLooted   = _root->GetElementById("other-inventory");
    Rocket::Core::Element* eStackPlayer = _root->GetElementById("stack-player");
    Rocket::Core::Element* eStackOther  = _root->GetElementById("stack-other");

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
    _root->Show();    
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

      total += stats_self->Model().Action("barter_value", 3, &object, &data_self, &data_other);
    }
    else
      total += 5;
  }
  return (total);
}

void UiBarter::UpdateInterface(void)
{
  Rocket::Core::Element* value_player = _root->GetElementById("value-player");
  Rocket::Core::Element* value_other  = _root->GetElementById("value-other");

  if (value_player && _stats_player && _stats_other)
    UpdateInterfaceSide(value_player, _stack_player.GetContent(), _stats_player, _stats_other);
  if (value_other  && _stats_player && _stats_other)
    UpdateInterfaceSide(value_other,  _stack_other.GetContent(),  _stats_other,  _stats_player);  
}

UiBarter::~UiBarter()
{
  ToggleEventListener(false, "button_done", "click", EventMakeDeal);
  ToggleEventListener(false, "button_quit", "click", EventBarterEnd);
}

void UiBarter::BarterEnd(Rocket::Core::Event&)
{
  Hide();
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
    to.AddObject(object);
    from.DelObject(object);
    Update();
    return (true);
  }
  return (false);
}

void UiBarter::SwapObjects(InventoryObject* object)
{
  cout << "Swap Objects Executed" << endl;
  cout << "Swap player stack/inventory" << endl;
  if (SwapFunctor(object, _stack_player, _inventory_player)) return ;
  cout << "Swap player inventory/stack" << endl;
  if (SwapFunctor(object, _inventory_player, _stack_player)) return ;
  if (SwapFunctor(object, _stack_other, _inventory_other))   return ;
  if (SwapFunctor(object, _inventory_other, _stack_other))   return ;
  cout << "No swap were executed" << endl;
}

void UiBarter::MakeDeal(Rocket::Core::Event& event)
{
  cout << "Make deal" << endl;
  int  total_player = GetStackValue(_stack_player.GetContent(), _stats_player, _stats_other);
  int  total_other  = GetStackValue(_stack_other.GetContent(),  _stats_other,  _stats_player);
  int  success      = 1;

  if (_stats_player && _stats_other)
  {
    cout << "Processing barter test" << endl;
    Data data_player  = _stats_player->Model().GetAll();
    Data data_other   = _stats_other->Model().GetAll();

    success = _stats_player->Model().Action("barter_deal", 4, &data_player, total_player, &data_other, total_other);
  }
  if (success)
  {
    cout << "Successfull barter is successfull" << endl;
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
