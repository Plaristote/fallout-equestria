#include "ui/game_main_bar.hpp"
#include "level/inventory.hpp"
#include <cmap/statcontroller.hpp>

using namespace std;
using namespace Rocket;

GameMainBar::GameMainBar(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  Rocket::Core::ElementDocument* doc = context->LoadDocument("data/main_bar.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();
    ToggleEventListener(true, "menu",      "click",   MenuButtonClicked);
    ToggleEventListener(true, "inv",       "click",   InventoryButtonClicked);
    ToggleEventListener(true, "pipbuck",   "click",   PipbuckButtonClicked);
    ToggleEventListener(true, "charsheet", "click",   PersButtonClicked);
    ToggleEventListener(true, "equiped_1", "mouseup", EquipedItem1Clicked);
    ToggleEventListener(true, "equiped_2", "mouseup", EquipedItem2Clicked);
    ToggleEventListener(true, "skilldex",  "click",   SkilldexButtonClicked);
    ToggleEventListener(true, "spellbook", "click",   SpelldexButtonClicked);
    
    _apEnabled = false;
    _apMax     = 0;
    
    EquipedItem1Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem1Clicked);
    EquipedItem2Clicked.EventReceived.Connect(*this, &GameMainBar::CallbackEquipedItem2Clicked);
    CombatEndClicked.EventReceived.Connect(*this, &GameMainBar::CallbackCombatEndClicked);
    PassTurnClicked.EventReceived.Connect (*this, &GameMainBar::CallbackPassTurnClicked);
    SkilldexButtonClicked.EventReceived.Connect([this](Rocket::Core::Event&) { OpenSkilldex.Emit(); });
    SpelldexButtonClicked.EventReceived.Connect([this](Rocket::Core::Event&) { OpenSpelldex.Emit(); });
    
    Translate();
  }
}

GameMainBar::~GameMainBar()
{
  ToggleEventListener(false, "menu",       "click",   MenuButtonClicked);
  ToggleEventListener(false, "inv",        "click",   InventoryButtonClicked);
  ToggleEventListener(false, "pipbuck",    "click",   PipbuckButtonClicked);
  ToggleEventListener(false, "charsheet",  "click",   PersButtonClicked);
  ToggleEventListener(false, "equiped_1",  "mouseup", EquipedItem1Clicked);
  ToggleEventListener(false, "equiped_2",  "mouseup", EquipedItem2Clicked);
  ToggleEventListener(false, "pass_turn",  "click",   PassTurnClicked);
  ToggleEventListener(false, "stop_fight", "click",   CombatEndClicked);
  ToggleEventListener(false, "skilldex",   "click",   SkilldexButtonClicked);
  ToggleEventListener(false, "spellbook",  "click",   SpelldexButtonClicked);
}

void GameMainBar::SetStatistics(StatController* controller)
{
  StatModel& model   = controller->Model();
  Data       data_ap = controller->GetData()["Variables"]["Action Points"];
  short      max_ap  = controller->GetData()["Statistics"]["Action Points"];
  short      ap      = data_ap.Nil() ? max_ap : (short)data_ap;

  statistics = controller;
  SetCurrentAP(ap, max_ap);
  controller->HpChanged.Connect(*this, &GameMainBar::SetCurrentHp);
  controller->ArmorClassChanged.Connect(*this, &GameMainBar::SetCurrentAc);
  controller->ActionPointChanged.Connect([this](unsigned short ap)
  {
    short max_ap  = statistics->GetData()["Statistics"]["Action Points"];
    
    SetCurrentAP(ap, max_ap);
  });
}

void GameMainBar::AppendToConsole(const std::string& str)
{
  Rocket::Core::Element* console = _root->GetElementById("console");
  
  if (console)
  {
    Rocket::Core::String rml;
    Rocket::Core::String toAdd;

    console->GetInnerRML(rml);
    toAdd  = "<li>- ";
    toAdd += str.c_str();
    toAdd += "</li><br />";
    console->SetInnerRML(toAdd + rml);
  }
}

void GameMainBar::SetCurrentHp(short hp)
{
  Rocket::Core::Element* elem_hp = _root->GetElementById("current-hp");
  
  if (elem_hp)
  {
    stringstream rml;

    rml << hp;
    elem_hp->SetInnerRML(rml.str().c_str());
  }
}

void GameMainBar::SetCurrentAc(short ac)
{
  Rocket::Core::Element* elem_ac = _root->GetElementById("current-ac");
  
  if (elem_ac)
  {
    stringstream rml;

    rml << ac;
    elem_ac->SetInnerRML(rml.str().c_str());
  }
}

void GameMainBar::SetCurrentAP(unsigned short ap, unsigned short max)
{
  SetMaxAP(max);
  if (_apEnabled)
  {
    Rocket::Core::Element* apbar = _root->GetElementById("action_points");
    string                 rml;

    if (apbar)
    {
      for (unsigned short i = 0  ; i < ap ; ++i)
	rml += "<img class='img-ap'   height='20px' src='../textures/ap-active.png' /> ";
      for (unsigned short i = ap ; i < _apMax ; ++i)
	rml += "<img class='img-ap' height='20px' src='../textures/ap-inactive.png' /> ";
      apbar->SetInnerRML(rml.c_str());
    }
  }
}

void GameMainBar::SetMaxAP(unsigned short ap)
{
  Rocket::Core::Element* apbar = _root->GetElementById("action_points");
  string                 rml;

  _apMax = ap;
  if (apbar)
  {
    for (unsigned short i = 0 ; i < ap ; ++i)
      rml += "<img class='img-ap' src='../textures/ap-inactive.png' /> ";
    apbar->SetInnerRML(rml.c_str());
  }
}

void GameMainBar::SetEnabledAP(bool enabled)
{
  _apEnabled = enabled;
  SetMaxAP(_apMax);
  
  Rocket::Core::Element* passTurn  = _root->GetElementById("pass_turn");
  Rocket::Core::Element* combatEnd = _root->GetElementById("stop_fight");

  if (enabled)
  {
    passTurn->AddEventListener("click", &PassTurnClicked);
    combatEnd->AddEventListener("click", &CombatEndClicked);  
  }
  else
  {
    passTurn->RemoveEventListener("click", &PassTurnClicked);
    combatEnd->RemoveEventListener("click", &CombatEndClicked);  
  }
}

void GameMainBar::CallbackEquipedItem1Clicked(Rocket::Core::Event& event)
{
  int button = 0;

  button = event.GetParameter<int>("button", button);
  if (button == 0)
    UseEquipedItem.Emit(0);
  else if (button == 2)
    EquipedItemNextAction.Emit(0);
}

void GameMainBar::CallbackEquipedItem2Clicked(Rocket::Core::Event& event)
{
  int button = 0;

  button = event.GetParameter<int>("button", button);
  if (button == 0)
    UseEquipedItem.Emit(1);
  else if (button == 2)
    EquipedItemNextAction.Emit(1);
}


void GameMainBar::SetEquipedItem(unsigned short it, InventoryObject* item)
{
  SetEquipedItemAction(it, item, 0);
}

void GameMainBar::SetEquipedItemAction(unsigned short it, InventoryObject* item, unsigned char actionIt)
{
  cout << "SetEquipedItemAction: " << (int)actionIt << endl;
  if (item)
  {
    Rocket::Core::Element* elem;
    stringstream           stream;
    
    stream << "equiped_" << (it + 1);
    elem = _root->GetElementById(stream.str().c_str());
    if (elem)
    {
      stringstream rml;
      bool         actionExists = (*item)["actions"].Count() > actionIt;

      if (actionExists) rml << "<p class='equiped_action'>" << (*item)["actions"][actionIt].Key() << "</p>";
      rml << "<p class='equiped_image'><img src='../textures/itemIcons/" << (*item)["icon"].Value() << "' /></p>";
      if (actionExists) rml << "<p class='equiped_apcost'>" << (*item)["actions"][actionIt]["ap-cost"].Value() << "AP</p>";
      elem->SetInnerRML(rml.str().c_str());
    }
  }
}

