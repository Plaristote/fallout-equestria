#include "pipbuck/quest_app.hpp"
#include "directory.hpp"
#include "quest_manager.hpp"
#include "gametask.hpp"

using namespace std; 

PipbuckQuestApp::PipbuckQuestApp(Data script) : _appid(script.Key()), _data_engine(0)
{
  EventQuestHovered.EventReceived.Connect(*this, &PipbuckQuestApp::ListQuestHovered);
  EventQuestClicked.EventReceived.Connect(*this, &PipbuckQuestApp::ListQuestClicked);
  EventBackClicked.EventReceived.Connect([this](Rocket::Core::Event&)
  {
    LoadQuestList(_root);
  });
}

bool PipbuckQuestApp::Started(DataEngine& de)
{
  _root         = 0;
  _data_engine  = &de;
  _current_view = QuestList;
  return (Filesystem::FileContent("data/pipbuck_quest_index.rml", _rml_index) &&
          Filesystem::FileContent("data/pipbuck_quest_view.rml",  _rml_view));
}

void PipbuckQuestApp::Exited(DataEngine& de)
{
}

void PipbuckQuestApp::Unfocused(DataEngine& de)
{
  _observer.DisconnectAll();
}

void PipbuckQuestApp::Focused(Rocket::Core::Element* root, DataEngine&)
{
  QuestManager& qm = GameTask::CurrentGameTask->GetQuestManager();
  
  _observer.Connect(qm.QuestsUpdated, [this](void)
  {
    if (_current_view == QuestList)
      LoadQuestList(_root);
  });

  _root         = 0;
  _last_hovered = 0;
  switch (_current_view)
  {
    case QuestList:
      LoadQuestList(root);
      break ;
    case QuestView:
      LoadQuestView(root);
      break ;
  }
}

void PipbuckQuestApp::RunAsMainTask(Rocket::Core::Element*, DataEngine&)
{
}

void PipbuckQuestApp::LoadQuestList(Rocket::Core::Element* root)
{
  //QuestManager&          quest_manager = GameTask::CurrentGameTask->GetQuestManager();
  Rocket::Core::Element* quest_list;
  
  _root = root;
  root->SetInnerRML(_rml_index.c_str());
  quest_list = root->GetElementById("quest_containers");
  if (quest_list)
  {
    std::stringstream rml;
    Data              quests = (*_data_engine)["Quests"];

    _current_view = QuestList;
    std::for_each(quests.begin(), quests.end(), [&rml](Data quest)
    {
      rml << "<div class='quest-list-item' id=\"" << underscore(quest.Key()) << "\" data-quest=\"" << quest.Key().c_str() << "\">";
      rml << "  <div class='quest-name'>" << i18n::T(quest.Key()) << "</div>";
      rml << "  <div class='quest-desc'>" << i18n::T(quest["description"].Value()) << "</div>";

      Quest* quest_object = GameTask::CurrentGameTask->GetQuestManager()[quest.Key()];
      bool   completed;
      
      completed = (quest_object && quest_object->IsOver());
      rml << "  <div class='quest-prog'>" << (completed ? "Done" : "In progress") << "</div>";
      rml << "</div>";
    });
    quest_list->SetInnerRML(rml.str().c_str());
    
    std::for_each(quests.begin(), quests.end(), [this, quest_list](Data quest)
    {
      Rocket::Core::Element* button = quest_list->GetElementById(underscore(quest.Key()).c_str());

      if (button)
      {
        button->AddEventListener("mouseover", &EventQuestHovered);
        button->AddEventListener("click",     &EventQuestClicked);
      }
    });
  }
}

void PipbuckQuestApp::ListQuestHovered(Rocket::Core::Event& event)
{
  Rocket::Core::Element* element = event.GetCurrentElement();
  
  if (element)
  {
    Rocket::Core::Element* child;
    Rocket::Core::String   color_on("yellow");
    Rocket::Core::String   color_off("white");

    if (_last_hovered != 0)
    {
      for (unsigned short i = 0 ; (child = _last_hovered->GetChild(i)) != 0 ; ++i)
        child->SetProperty("color", color_off);
    }
    for (unsigned short i = 0 ; (child = element->GetChild(i)) != 0 ; ++i)
    {
      child->SetProperty("color", color_on);
    }
    _last_hovered = element;
  }
}

void PipbuckQuestApp::ListQuestClicked(Rocket::Core::Event& event)
{
  Rocket::Core::Element* element = event.GetCurrentElement();
  
  if (element)
  {
    Rocket::Core::Variant* variant = element->GetAttribute("data-quest");
    
    if (variant)
    {
      _current_quest = variant->Get<Rocket::Core::String>().CString();
      LoadQuestView(_root);
    }
  }
}

void PipbuckQuestApp::LoadQuestView(Rocket::Core::Element* root)
{
  Data quest = (*_data_engine)["Quests"][_current_quest];
  
  if (quest.NotNil())
  {
    _last_hovered = 0;
    _current_view = QuestView;
    root->SetInnerRML(_rml_view.c_str());
    {
      std::stringstream      html;
      Data                   objectives = quest["objectives"];
      Rocket::Core::Element* elem_title = root->GetElementById("quest_title");
      Rocket::Core::Element* elem_desc  = root->GetElementById("quest_description"); 
      Rocket::Core::Element* elem_objs  = root->GetElementById("objectives_container");
      Rocket::Core::Element* back_button= root->GetElementById("back_button");

      if (elem_title)
        elem_title->SetInnerRML(i18n::T(quest.Key()).c_str());
      if (elem_desc)
        elem_desc->SetInnerRML(i18n::T(quest["description"].Value()).c_str());
      back_button->AddEventListener("click", &EventBackClicked);
      
      for_each(objectives.begin(), objectives.end(), [&html](Data objective)
      {
        Data conditions = objective["conditions"];
        bool completed  = true;
        auto it         = conditions.begin();
        auto end        = conditions.end();

        conditions.Output();
        
        for (; it != end ; ++it)
        {
          if ((int)((*it)["completed"]) != 1)
          {
            completed   = false;
            break ;
          }
        }
        html << "<div class='objective'>";
        html << "<div style='float:left; font-size: 75%; width: 25%;'>" << objective.Key() << "</div>";
        html << "<div style='float:left; font-size: 75%; width: 75%; color:" << (completed ? "green" : "#FFE84C") << ";'>";
        html << objective["description"].Value() << "</div>";
        html << "</div>";
      });
      elem_objs->SetInnerRML(html.str().c_str());
    }
  }
}