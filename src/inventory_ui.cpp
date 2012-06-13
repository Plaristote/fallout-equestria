#include "inventory_ui.hpp"

/*
 * InventoryView
 */
InventoryView::InventoryView(Rocket::Core::Element* element, Inventory& inventory) : _element(*element), _inventory(inventory)
{
  //element->AddEventListener("dragstart", this);
  element->AddEventListener("dragdrop",  this);
  UpdateView();
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
    
    if ((item["hidden"].Nil() || item["hidden"].Value() != "1") && !(item.IsEquiped()))
    {
      std::stringstream stream;

      stream << "<img id='" << count << "' src='../textures/itemIcons/" << item["icon"].Value() << "' />";
      rml += stream.str();
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
  std::cout << "PROCESS EVENT" << std::endl;
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

  for (; itView != end && (!((*(*itView)) == element->GetParentNode())) ; ++itView)

  if (itView != _views.end())
  {
    InventoryView&   view = (*(*itView));
    InventoryObject* object;

    object = view.GetObjectFromId(std::string(element->GetId().CString()));
    view.GetInventory().DelObject(object);
    container->GetInventory().AddObject(object);
    view.UpdateView();
    container->UpdateView();
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

/*
 * UiUseObjectOn
 */
UiUseObjectOn::UiUseObjectOn(WindowFramework* window, Rocket::Core::Context* context, Inventory& inventory) : UiBase(window)
{
  _root = context->LoadDocument("data/useobjecton.rml");
  if (_root)
  {
    Rocket::Core::Element* eCancel    = _root->GetElementById("button_cancel");
    Rocket::Core::Element* eInventory = _root->GetElementById("inventory");

    if (eCancel)
    {
      eCancel->AddEventListener("click", &CancelClicked);
      CancelClicked.EventReceived.Connect(*this, &UiUseObjectOn::RocketCancelClicked);
    }
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
  _viewController.Destroy();
  if (_root)
    _root->RemoveReference();
}

void UiUseObjectOn::Destroy(void)
{
  if (_root)
    _root->Hide();
}