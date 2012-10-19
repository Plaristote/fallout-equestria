#include "saveuis.hpp"

using namespace std;

#include <timer.hpp>

extern PandaFramework framework;

void SetOpacityOnAll(Rocket::Core::Element* element, float alpha)
{
  std::stringstream             stream;    
  const Rocket::Core::Property* property = element->GetProperty("color");
  std::string                   string   = property->ToString().CString();

  for (int it = 0 ; element->GetChild(it) ; ++it)
  {
    Rocket::Core::Element* cur = element->GetChild(it);

    SetOpacityOnAll(cur, alpha);
  }
}

void UiBase::FadeOut(void)
{
  Hide();
  /*float alpha = 255;
  Timer timer;
  float elapsedTime;

  for (alpha = 255 ; alpha >= 0 ; alpha -= (elapsedTime * 10.f))
  {
    std::stringstream stream;

    stream << "rgba(0, 0, 0, " << alpha << ")";
    SetOpacityOnAll(_root, alpha);
    //SetPropertyOnAll(_root, "color", stream.str());
    framework.get_graphics_engine()->render_frame();
    elapsedTime = timer.GetElapsedTime();
    timer.Restart();
  }
  Hide();*/
}

void UiBase::FadeIn(void)
{
  Show();
  /*float alpha = 0;
  Timer timer;
  float elapsedTime;    

  Show();
  for (alpha = 0 ; alpha <= 255 ; alpha -= (elapsedTime * 10.f))
  {
    std::stringstream stream;
    
    stream << "rgba(0, 0, 0, " << alpha << ")";
    SetOpacityOnAll(_root, alpha);
    //SetPropertyOnAll(_root, "color", stream.str());
    framework.get_graphics_engine()->render_frame();
    elapsedTime = timer.GetElapsedTime();
    timer.Restart();
  }*/
}


/*
 * UiLoad
 */
UiLoad::UiLoad(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context)
{
  _selectedSlot = 0;
  _root         = context->LoadDocument("data/loadgame.rml");
  if (_root)
  {
    Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
    
    if (slotContainer)
    {
      Directory      dir;
      unsigned short nSlots = 0;

      dir.OpenDir(savePath);
      std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const struct dirent& entry)
      {
	if (entry.d_type == DT_DIR)
	{
	  std::string str("slot-");
	  bool        compare = true;

	  for (int i = 0 ; i < str.length() ; ++i)
	  {
	    if (str[i] != entry.d_name[i])
	    {
	      compare = false;
	      break ;
	    }
	  }
	  if (compare)
	    nSlots++;
	}
      });

      std::stringstream rml;
      for (int it = 0 ; it < nSlots ; ++it)
      {
	rml << "<div class='load-slot' id='load-slot-" << it << "' data-slot='" << it << "'>";
	rml << "<div class='load-slot-title'>Slot " << it << "</div>";
	rml << "</div>";
      }
      slotContainer->SetInnerRML(rml.str().c_str());
      
      for (int it = 0 ; it < nSlots ; ++it)
      {
	std::stringstream      idSlot;
	Rocket::Core::Element* slotElement;

	idSlot << "load-slot-" << it;
	slotElement = _root->GetElementById(idSlot.str().c_str());
	slotElement->AddEventListener("click",    &EventClickSlot);
	slotElement->AddEventListener("dblclick", &EventClickSlot);
	slotElement->AddEventListener("dblclick", &EventLoadGame);
      }
    }
    ToggleEventListener(true, "button-load",   "click", EventLoadGame);
    ToggleEventListener(true, "button-cancel", "click", EventCancel);
  }
  EventClickSlot.EventReceived.Connect(*this, &UiLoad::ClickSlot);
  EventLoadGame.EventReceived.Connect(*this, &UiLoad::LoadGame);
  EventCancel.EventReceived.Connect(*this, &UiLoad::Cancel);
  Translate();
}

UiLoad::~UiLoad()
{
  Rocket::Core::Element*   slot_container = _root->GetElementById("slot-container");
  
  if (slot_container)
  {
    Rocket::Core::Element* slot;

    for (unsigned short i = 0 ; (slot = slot_container->GetChild(i)) ; ++i)
    {
      slot->RemoveEventListener("click",    &EventClickSlot);
      slot->RemoveEventListener("dblclick", &EventClickSlot);
      slot->RemoveEventListener("dblclick", &EventLoadGame);
    }
  }
  ToggleEventListener(false, "button-load",   "click", EventLoadGame);
  ToggleEventListener(false, "button-cancel", "click", EventCancel);
}

void UiLoad::LoadGame(Rocket::Core::Event&)
{
  std::cout << "LoadGame called" << std::endl;
  if (_selectedSlot)
  {
    Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
    unsigned int           slot    = 0;

    slot = varSlot->Get<unsigned int>();
    LoadSlot.Emit((unsigned char)slot);
  }
}

void UiLoad::ClickSlot(Rocket::Core::Event& event)
{
  _selectedSlot = event.GetCurrentElement();
}

void UiLoad::Cancel(Rocket::Core::Event&)
{
  Hide();
}

/*
 * UiSave
 */
UiSave::UiSave(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context)
{
  _root = context->LoadDocument("data/savegame.rml");
  if (_root)
  {
    Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
    
    if (slotContainer)
    {
      Directory      dir;
      unsigned short nSlots = 0;

      dir.OpenDir(savePath);
      std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const struct dirent& entry)
      { if (entry.d_type == DT_DIR && entry.d_name != "." && entry.d_name != "..") nSlots++; });

      std::stringstream rml;
      
      for (unsigned short it = 0 ; it < nSlots ; ++it)
      {
	rml << "<div class='save-slot' id='save-slot-" << it << "' data-slot='" << it << "'>";
	rml << "<div class='save-slot-title'>Slot " << it << "</div>";
	rml << "</div>";
      }
      slotContainer->SetInnerRML(rml.str().c_str());
      for (unsigned short it = 0 ; it < nSlots ; ++it)
      {
	std::stringstream      idSlot;
	Rocket::Core::Element* slotElement;
	
	idSlot << "save-slot-" << it;
	slotElement = _root->GetElementById(idSlot.str().c_str());
	slotElement->AddEventListener("click",    &EventClickSlot);
	slotElement->AddEventListener("dblclick", &EventClickSlot);
	slotElement->AddEventListener("dblclick", &EventSaveGame);
      }
    }
    ToggleEventListener(true, "button-save",   "click", EventSaveGame);
    ToggleEventListener(true, "button-cancel", "click", EventCancel);
    Translate();
  }
  EventClickSlot.EventReceived.Connect(*this, &UiSave::ClickSlot);
  EventSaveGame.EventReceived.Connect(*this, &UiSave::SaveGame);
  EventCancel.EventReceived.Connect(*this, &UiSave::Cancel);
}

UiSave::~UiSave()
{
  Rocket::Core::Element*   slot_container = _root->GetElementById("slot-container");
  
  if (slot_container)
  {
    Rocket::Core::Element* slot;

    for (unsigned short i = 0 ; (slot = slot_container->GetChild(i)) ; ++i)
    {
      slot->RemoveEventListener("click",    &EventClickSlot);
      slot->RemoveEventListener("dblclick", &EventClickSlot);
      slot->RemoveEventListener("dblclick", &EventSaveGame);
    }
  }
  ToggleEventListener(false, "button-save",   "click", EventSaveGame);
  ToggleEventListener(false, "button-cancel", "click", EventCancel);
}

void UiSave::SaveGame(Rocket::Core::Event&)
{
  if (_selectedSlot)
  {
    Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
    unsigned int           slot    = 0;

    slot = varSlot->Get<unsigned int>();
    SaveToSlot.Emit((unsigned char)slot);
  }
}

void UiSave::ClickSlot(Rocket::Core::Event& event)
{
  _selectedSlot = event.GetCurrentElement();
}

void UiSave::Cancel(Rocket::Core::Event&)
{
  Hide();
}