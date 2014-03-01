#ifndef  PIPBUCK_QUEST_APP_HPP
# define PIPBUCK_QUEST_APP_HPP

# include "pipbuck/pipbuck.hpp"

class PipbuckQuestApp : public Pipbuck::App
{
public:
  PipbuckQuestApp(Data script);
  ~PipbuckQuestApp() {}

  const std::string GetAppId(void) const { return (_appid); }
  
  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&) {}

  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);
private:
  enum Interfaces
  {
    QuestList,
    QuestView
  };

  void LoadQuestList(Rocket::Core::Element*);
  void LoadQuestView(Rocket::Core::Element*);
  
  RocketListener EventQuestHovered, EventQuestClicked, EventBackClicked;
  
  void               ListQuestHovered(Rocket::Core::Event&);
  void               ListQuestClicked(Rocket::Core::Event&);

  const std::string _appid;
  std::string       _rml_index, _rml_view;
  DataEngine*       _data_engine;
  Interfaces        _current_view;
  std::string       _current_quest;
  
  Rocket::Core::Element* _last_hovered;
  Rocket::Core::Element* _root;
  
  Sync::ObserverHandler  _observer;
};

#endif