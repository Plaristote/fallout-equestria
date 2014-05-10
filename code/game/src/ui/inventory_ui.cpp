#include "ui/inventory_ui.hpp"

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

Sync::Signal<void (InventoryView*, Rocket::Core::Element*)> ObjectDropped;
Sync::Signal<void (InventoryObject*)>                       ObjectSelected;
Sync::Signal<void (InventoryObject*)>                       ObjectMenuRequested;
Sync::Signal<void (InventoryObject*)>                       ObjectFocused;

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

    if (!(item.IsHidden() || item.IsEquiped()))
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
  std::stringstream            stream;
  unsigned short               count;

  stream << id;
  stream >> count;
  std::advance(it, count);
  if (it != content.end())
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
  view.ObjectFocused.Connect      (ObjectFocused,       &Sync::Signal<void (InventoryObject*)>::Emit);
  view.ObjectSelected.Connect     (ObjectSelected,      &Sync::Signal<void (InventoryObject*)>::Emit);
  view.ObjectMenuRequested.Connect(ObjectMenuRequested, &Sync::Signal<void (InventoryObject*)>::Emit);
}

void InventoryViewController::DragObserver(InventoryView* container, Rocket::Core::Element* element)
{
  std::vector<InventoryView*>::iterator itView = _views.begin();
  std::vector<InventoryView*>::iterator end    = _views.end();

  for (; itView != end && (!((*(*itView)) == element->GetParentNode())) ; ++itView);
  if (itView != _views.end())
  {
    InventoryView&   view = (*(*itView));

    if (AllowDrop(view, *container))
    {
      InventoryObject* object = view.GetObjectFromId(std::string(element->GetId().CString()));

      if (container->GetInventory().CanCarry(object) && CanSwap(object))
      {
        view.GetInventory().DelObject(object);
        container->GetInventory().AddObject(object);
        view.UpdateView();
        container->UpdateView();
      }
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
