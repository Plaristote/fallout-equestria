#include "saveuis.hpp"

using namespace std;

#include <timer.hpp>

extern PandaFramework framework;

void SetOpacityOnAll(Rocket::Core::Element* element, float alpha)
{
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

string UiLoad::LoadSlotRml(const string& prefix, unsigned short it)
{
  DataTree*    tree;
  stringstream dataengine_path;
  stringstream rml;

  dataengine_path << "saves/slots/slot-" << it << ".json";
  tree = DataTree::Factory::JSON(dataengine_path.str());
  rml << "<div class='" << prefix << "-slot' id='" << prefix << "-slot-" << it << "' data-slot='" << it << "'>\n";
  rml << "  <div class='load-slot-title'>Slot " << it << "</div>\n";
  rml << "  <div class='load-slot-data'>\n";
  if (tree)
  {
    Data data_engine(tree);
    Data time  = data_engine["time"];
    Data place = data_engine["system"]["current-level"];

    rml << "    <span class='load-slot-date'>" << time["year"].Value() << "/" << time["month"].Value() << "/" << time["days"].Value() << "</span>\n";
    rml << "    <span class='load-slot-time'> - " << time["hours"].Value() << ":" << time["minutes"].Value() << ":" << time["seconds"].Value() << "</span>\n";
    rml << "    <span class='load-slot-place'> - " << (place == 0 ? "Wasteland" : place.Value()) << "</span>\n";
  }
  else
    rml << "    Empty slot\n";
  rml << "  </div>\n";
  rml << "</div>\n";
  delete tree;
  cout << rml.str() << endl;
  return (rml.str());
}

/*
 * UiLoad
 */
UiLoad::UiLoad(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context), _savePath(savePath)
{
  cout << "UiLoad used" << endl;
  _selectedSlot = 0;
  _root         = context->LoadDocument("data/loadgame.rml");
  if (_root)
  {
    LoadSlots(savePath);
    ToggleEventListener(true, "button-load",   "click", EventLoadGame);
    ToggleEventListener(true, "button-cancel", "click", EventCancel);
    ToggleEventListener(true, "button-erase",  "click", EventErase);
  }
  EventClickSlot.EventReceived.Connect(*this, &UiLoad::ClickSlot);
  EventLoadGame.EventReceived.Connect(*this, &UiLoad::LoadGame);
  EventCancel.EventReceived.Connect(*this, &UiLoad::Cancel);
  EventErase.EventReceived.Connect(*this, &UiLoad::Erase);
  Translate();
}

void UiLoad::LoadSlots(const string& savePath)
{
  Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
  
  if (slotContainer)
  {
    Directory      dir;
    unsigned short nSlots = 0;

    dir.OpenDir(savePath);
    std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const Dirent& entry)
    {
      if (string(entry.d_name).find(".png") == string::npos)
        nSlots++;
    });

    std::stringstream rml;
    for (int it = 0 ; it < nSlots ; ++it)
      rml << UiLoad::LoadSlotRml("load", it);
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
  ToggleEventListener(false, "button-erase",  "click", EventErase);
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
  Rocket::Core::Element* preview = _root->GetElementById("slot-preview");

  _selectedSlot = event.GetCurrentElement();
  if (preview)
  {
    Rocket::Core::Variant* var_slot;
    stringstream           rml_preview;

    var_slot      = _selectedSlot->GetAttribute("data-slot");
    rml_preview << "<img id='preview-picture' style='color:rgba(255, 0, 255, 0);' src='../saves/slots/slot-" << var_slot->Get<unsigned int>() << ".png' />";
    preview->SetInnerRML(rml_preview.str().c_str());
  }
}

void UiLoad::Cancel(Rocket::Core::Event&)
{
  Hide();
}

void UiLoad::Erase(Rocket::Core::Event&)
{
  if (_selectedSlot)
  {
    Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
    unsigned int           slot    = 0;

    slot = varSlot->Get<unsigned int>();
    EraseSlot.Emit((unsigned char)slot);
    LoadSlots(_savePath);
    _selectedSlot = 0;
  }
}

/*
 * UiSave
 */
UiSave::UiSave(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath) : UiBase(window, context), _savePath(savePath)
{
  _selectedSlot = 0;
  _root         = context->LoadDocument("data/savegame.rml");
  if (_root)
  {
    LoadSlots(savePath);
    ToggleEventListener(true, "button-save",   "click", EventSaveGame);
    ToggleEventListener(true, "button-cancel", "click", EventCancel);
    ToggleEventListener(true, "button-erase",  "click", EventErase);
    Translate();
  }
  EventClickSlot.EventReceived.Connect(*this, &UiSave::ClickSlot);
  EventSaveGame.EventReceived.Connect(*this, &UiSave::SaveGame);
  EventCancel.EventReceived.Connect(*this, &UiSave::Cancel);
  EventErase.EventReceived.Connect(*this, &UiSave::Erase);
}

void UiSave::LoadSlots(const string& savePath)
{
  Rocket::Core::Element* slotContainer = _root->GetElementById("slot-container");
  
  if (slotContainer)
  {
    Directory      dir;
    unsigned short nSlots = 1;

    dir.OpenDir(savePath + "slots");
    std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [this, &nSlots](const Dirent& entry)
    {
      if (string(entry.d_name).find(".png") == string::npos)
        nSlots++;
    });

    std::stringstream rml;
    
    for (unsigned short it = 0 ; it < nSlots ; ++it)
    {
      rml << UiLoad::LoadSlotRml("save", it);
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
  ToggleEventListener(false, "button-erase",  "click", EventErase);
}

void UiSave::Erase(Rocket::Core::Event&)
{
  if (_selectedSlot)
  {
    Rocket::Core::Variant* varSlot = _selectedSlot->GetAttribute("data-slot");
    unsigned int           slot    = 0;

    slot = varSlot->Get<unsigned int>();
    EraseSlot.Emit((unsigned char)slot);
    LoadSlots(_savePath);
    _selectedSlot = 0;
  }  
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
  Rocket::Core::Element* preview = _root->GetElementById("slot-preview");

  _selectedSlot = event.GetCurrentElement();
  if (preview)
  {
    Rocket::Core::Variant* var_slot;
    stringstream           rml_preview;

    var_slot      = _selectedSlot->GetAttribute("data-slot");
    rml_preview << "<img id='preview-picture' src='../saves/slot-" << var_slot->Get<unsigned int>() << "/preview.png' />";
    preview->SetInnerRML(rml_preview.str().c_str());
  }
}

void UiSave::Cancel(Rocket::Core::Event&)
{
  Hide();
}
