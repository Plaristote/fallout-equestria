#ifndef  WORLDMAP_HPP
# define WORLDMAP_HPP

# include "gameui.hpp"
# include "dataengine.hpp"
# include <fstream>

class WorldMap : public UiBase
{
  struct City
  {
    bool operator==(const std::string& str) const { return (name == str); }

    int         pos_x;
    int         pos_y;
    int         radius;
    std::string name;
  };

  typedef std::vector<City>                   Cities;
  typedef std::vector<Rocket::Core::Element*> Cases;

public:
  static WorldMap* CurrentWorldMap;

  WorldMap(WindowFramework*, GameUi*, DataEngine&, TimeManager&);
  ~WorldMap();
  
  void Show(void);
  
  void SetCityVisible(const std::string&);

  void MapClicked(Rocket::Core::Event& event);
  void PartyClicked(Rocket::Core::Event& event);
  void CityClicked(Rocket::Core::Event& event);

  void Run(void);
  void SetInterrupted(bool set);
  void Save(const std::string&);
  
  void MoveTowardsCoordinates(float x, float y);
  void MoveTowardsPlace(const std::string&);
  
  DataEngine& GetDataEngine(void) { return (_dataEngine); }
  Data        GetCaseData(int x, int y) const;

  Observatory::Signal<void (std::string)> GoToPlace;
  Observatory::Signal<void (int, int)>    RequestRandomEncounterCheck;

private:
  void                   MapTileGenerator(Data map);
  void                   UpdatePartyCursor(float elapsedTime);
  void                   UpdateClock(void);
  bool                   IsPartyInCity(std::string& ret) const;
  Rocket::Core::Element* GetCaseAt(int x, int y) const;
  void                   GetCurrentCase(int&, int&) const;
  void                   SetCaseVisibility(int x, int y, char visibility) const;

  void                   AddCityToList(Data);

  void                   SaveMapStatus(void) const;
  
  bool                   CheckRandomEncounter(void) const;

  RocketListener         MapClickedEvent, PartyCursorClicked, CityButtonClicked, ButtonInventory, ButtonCharacter, ButtonPipbuck, ButtonMenu;
  DataTree*              _mapTree;
  DataEngine&            _dataEngine;
  TimeManager&           _timeManager;
  GameUi&                _gameUi;
  Timer                  _timer;
  bool                   _interrupted;

  int                    _size_x,  _size_y;
  int                    _tsize_x, _tsize_y;
  
  float                  _current_pos_x, _current_pos_y;
  int                    _goal_x, _goal_y;
  Rocket::Core::Element* _cursor;

  Cities                 _cities;
  Cases                  _cases;
};

#endif