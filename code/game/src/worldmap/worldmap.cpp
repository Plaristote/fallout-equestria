#include "worldmap/worldmap.hpp"
#include "musicmanager.hpp"
#include "executor.hpp"
#include <dices.hpp>
#include "ui/alert_ui.hpp"
#include "ui/loading_screen.hpp"

using namespace std;
using namespace Rocket;

bool RocketFactoryInstanceElementText(Core::Element* elem, Core::String innerRml);

WorldMap* WorldMap::CurrentWorldMap = 0;

WorldMap::~WorldMap()
{
  _timeManager.ClearTasks(TASK_LVL_WORLDMAP);
  
  ToggleEventListener(false, "button-inventory", "click", ButtonInventory);
  ToggleEventListener(false, "button-character", "click", ButtonCharacter);
  ToggleEventListener(false, "button-pipbuck",   "click", ButtonPipbuck);
  ToggleEventListener(false, "button-menu",      "click", ButtonMenu);
  for_each(_cases.begin(), _cases.end(), [this](Core::Element* tile)
  { tile->RemoveEventListener("click", &MapClickedEvent); });
  _cursor->RemoveEventListener("click", &PartyCursorClicked);
  
  for_each(_cities.begin(), _cities.end(), [this](const City& city)
  {
    ToggleEventListener(false, "city-"      + city.name, "click", CityButtonClicked);
    ToggleEventListener(false, "city-halo-" + city.name, "click", MapClickedEvent);
  });
  
  Destroy();
  if (_mapTree)
    delete _mapTree;
  if (_cityTree)
    delete _cityTree;
  if (_city_splash)
    delete _city_splash;
  CurrentWorldMap = 0;
  root->Close();
  root->RemoveReference();
  root = 0;
}

void WorldMap::Save(const string& savepath)
{
  DataTree::Writers::JSON(_mapTree, "saves/map.json");
}

WorldMap::WorldMap(WindowFramework* window, GameUi* gameUi, DataEngine& de, TimeManager& tm) : UiBase(window, gameUi->GetContext()), _dataEngine(de), _timeManager(tm), _gameUi(*gameUi)
{
  _city_splash   = 0;
  _interrupted   = false;
  _current_pos_x = _goal_x = _dataEngine["worldmap"]["pos-x"];
  _current_pos_y = _goal_y = _dataEngine["worldmap"]["pos-y"];
  
  _mapTree = DataTree::Factory::JSON("saves/map.json");
  MapTileGenerator(_mapTree);

  if (root)
  {
    //
    // Adds the known cities to the CityList.
    //
    _cityTree = DataTree::Factory::JSON("saves/cities.json");
    if (_cityTree)
    {
      Data      cities(_cityTree);

      std::for_each(cities.begin(), cities.end(), [this](Data city) { AddCityToList(city); });
    }

    //
    // Get some required elements
    //
    _cursor       = root->GetElementById("party-cursor");

    //
    // Event management
    //
    ToggleEventListener(true, "button-inventory", "click", ButtonInventory);
    ToggleEventListener(true, "button-character", "click", ButtonCharacter);
    ToggleEventListener(true, "button-pipbuck",   "click", ButtonPipbuck);
    ToggleEventListener(true, "button-menu",      "click", ButtonMenu);
    ButtonInventory.EventReceived.Connect(_gameUi, &GameUi::OpenInventory);
    ButtonCharacter.EventReceived.Connect(_gameUi, &GameUi::OpenPers);
    ButtonPipbuck.EventReceived.Connect(_gameUi.OpenPipbuck, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    ButtonMenu.EventReceived.Connect(_gameUi, &GameUi::OpenMenu);
    
    _cursor->AddEventListener("click", &PartyCursorClicked);
    PartyCursorClicked.EventReceived.Connect(*this, &WorldMap::PartyClicked);

    MapClickedEvent.EventReceived.Connect(*this, &WorldMap::MapClicked);
    CityButtonClicked.EventReceived.Connect(*this, &WorldMap::CityClicked);
    
    UpdatePartyCursor(0);
    Translate();
  }
  CurrentWorldMap = this;
}

bool WorldMap::HasCity(const string& name) const
{
  Cities::const_iterator it = find(_cities.begin(), _cities.end(), name);
  
  return (it != _cities.end());
}

void WorldMap::SetCityVisible(const string& name)
{
  Cities::iterator it = find(_cities.begin(), _cities.end(), name);
  
  if (it != _cities.end())
  {
    DataTree* cityTree = DataTree::Factory::JSON("saves/cities.json");
    
    if (cityTree)
    {
      {
	Data citiesData(cityTree);
	Data cityData = citiesData[it->name];

	if (!(cityData.Nil()))
	{
	  cityData["visible"] = "1";
	  AddCityToList(cityData);
	  DataTree::Writers::JSON(cityTree, cityTree->GetSourceFile());
	}
      }
      delete cityTree;
    }
  }
}

void WorldMap::AddCity(const string& name, float x, float y, float radius)
{
  DataTree* cityTree = DataTree::Factory::JSON("saves/cities.json");

  if (cityTree)
  {
    Data      cities(cityTree);

    cities[name]["visible"] = 0;
    cities[name]["pos_x"]   = x;
    cities[name]["pos_y"]   = y;
    cities[name]["radius"]  = radius;
    AddCityToList(cities[name]);
    DataTree::Writers::JSON(cityTree, cityTree->GetSourceFile());
    delete cityTree;  
  }
}

void WorldMap::AddEntryZoneToCity(const string& city_name, const string& zone)
{
  Data cities(_cityTree);
  Data city = cities[city_name];

  if (city.NotNil())
    city["zones"][zone] = '1';
}

void WorldMap::SaveMapStatus(void) const
{
  DataTree::Writers::JSON(_mapTree, _mapTree->GetSourceFile());
}

void WorldMap::AddCityToList(Data cityData)
{
  City  new_city;
  auto  it     = find(_cities.begin(), _cities.end(), cityData.Key());
  City& city   = (it == _cities.end() ? new_city : *it);
  bool  show   = (it == _cities.end() ? true : cityData["visible"].Value() == "1" && city.visible == false);

  city.hidden  = cityData["hidden"] == "1";
  city.visible = cityData["visible"].Value() == "1";
  city.pos_x   = cityData["pos_x"];
  city.pos_y   = cityData["pos_y"];
  city.radius  = cityData["radius"];
  city.name    = cityData.Key();
  if (it == _cities.end())
    _cities.push_back(city);
  if (city.visible)
  {
    Core::Element* elem = root->GetElementById("city-list");
    Core::String   innerRml;
    stringstream   rml;

    rml << "<div class='city-entry'>";
    rml << "<button id='city-" << cityData.Key() << "' data-city='" << cityData.Key() << "' class='long_button city-button'>" << cityData.Key() << "</button>";
    rml << "</div>";
    innerRml = innerRml + Rocket::Core::String(rml.str().c_str());

    if ((RocketFactoryInstanceElementText(elem, innerRml)))
      ToggleEventListener(true, "city-" + cityData.Key(), "click", CityButtonClicked);
  }
  if (show && city.visible)
  {
    Core::Element* elem = root->GetElementById("pworldmap");
    stringstream   rml, css, elem_id;
    int            radius = city.radius * 2.5f;
    
    elem_id << "city-halo-" << cityData.Key();

    css << "position: absolute;";
    css << "top: "  << (city.pos_y - radius / 2.f) << "px;";
    css << "left: " << (city.pos_x - radius / 2.f) << "px;";
    css << "height: " << radius << "px;";
    css << "width: "  << radius << "px;";
    
    rml << "<div id='" << elem_id.str() << "' class='city-indicator' style='" << css.str() << "'>";
    rml << "<img src='worldmap-city.png' style='width:" << radius << "px;height:" << radius << "px;' />";
    rml << "</div>";

    if ((RocketFactoryInstanceElementText(elem, Core::String(rml.str().c_str()))))
      ToggleEventListener(true, elem_id.str(), "click", MapClickedEvent);
  }
}

void WorldMap::UpdateClock(void)
{
  Core::Element* elem_year    = root->GetElementById("clock-year");
  Core::Element* elem_month   = root->GetElementById("clock-month");
  Core::Element* elem_day     = root->GetElementById("clock-day");
  DateTime       current_time = _timeManager.GetDateTime();
  
  if (elem_year)
  {
    stringstream str;
    str << current_time.GetYear();
    elem_year->SetInnerRML(str.str().c_str());
  }
  if (elem_month)
  {
    stringstream str;
    str << current_time.GetMonth();
    elem_month->SetInnerRML(str.str().c_str());
  }
  if (elem_day)
  {
    stringstream str;
    str << current_time.GetDay();
    elem_day->SetInnerRML(str.str().c_str());
  }
}

void WorldMap::Show(void)
{
  UiBase::Show();
  _current_pos_x = _dataEngine["worldmap"]["pos-x"];
  _current_pos_y = _dataEngine["worldmap"]["pos-y"];
  UpdateClock();
  MusicManager::Get()->Play("worldmap");
}

void WorldMap::Run(void)
{
  if (_interrupted)
    return ;
  float elapsedTime = _timer.GetElapsedTime();

  if (_current_pos_x != _goal_x || _current_pos_y != _goal_y)
    UpdatePartyCursor(elapsedTime);

  // Reveal nearby cases
  {
    int x, y;

    GetCurrentCase(x, y);
    for (int i = -1 ; i <= 1 ; ++i)
    {
      for (int ii = -1 ; ii <= 1 ; ++ii)
      {
	int xit = x + i;
	int yit = y + ii;

	if (xit < 0 || yit < 0 || xit >= _size_x || yit >= _size_y)
	  continue ;
	SetCaseVisibility(xit, yit, 1);
      }
    }
    SetCaseVisibility(x, y, 2);
  }

  _timer.Restart();
}

void WorldMap::CityClicked(Rocket::Core::Event& event)
{
  Core::String     var    = event.GetCurrentElement()->GetAttribute("data-city")->Get<Core::String>();
  string           str    = var.CString();
  Cities::iterator cityIt = find(_cities.begin(), _cities.end(), str);
  
  if (cityIt != _cities.end())
  {
    _goal_x = cityIt->pos_x;
    _goal_y = cityIt->pos_y;
  }
}

void WorldMap::CloseCitySplash(void)
{
  _city_splash->Hide();
  Executor::ExecuteLater([this](void) { delete _city_splash; });
}

void WorldMap::OpenCitySplash(const std::string& cityname)
{
  Data      cities(_cityTree);
  Data      city = cities[cityname];

  if (city["zones"].Count() > 0)
  {
    try
    {
      _city_splash = new CitySplash(city, window, context);
      _city_splash->Canceled.Connect(*this, &WorldMap::CloseCitySplash);
      _city_splash->EntryZonePicked.Connect([this, cityname](std::string zone)
      {
        CloseCitySplash();
        GoToCityZone.Emit(cityname, zone);
      });
      _city_splash->Show();
    }
    catch (...)
    {
      if (_city_splash) { delete _city_splash; }
      AlertUi::NewAlert.Emit("Could not load splashscreen for the place named '" + cityname + '\'');
    }
  }
  else
    GoToPlace.Emit(cityname);
}

void WorldMap::PartyClicked(Rocket::Core::Event&)
{
  string cityname;

  if (IsPartyInCity(cityname))
    OpenCitySplash(cityname);
  else
  {
    int x, y;

    GetCurrentCase(x, y);
    RequestRandomEncounter.Emit(x, y, Dices::Throw(100) >= 85);
  }
}

bool WorldMap::IsPartyInCity(string& cityname, bool not_hidden) const
{
  Cities::const_iterator it  = _cities.begin();
  Cities::const_iterator end = _cities.end();
  
  for (; it != end ; ++it)
  {
    const City& city   = *it;
    float       dist_x = city.pos_x - _current_pos_x;
    float       dist_y = city.pos_y - _current_pos_y;
    float       dist   = SQRT(dist_x * dist_x + dist_y * dist_y);

    if ((!not_hidden || !city.hidden || city.visible) && dist <= city.radius)
    {
      cityname = city.name;
      return (true);
    }
  }
  return (false);
}

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

void WorldMap::UpdatePartyCursor(float elapsedTime)
{
  // Update the position
  int   current_case_x, current_case_y;
  GetCurrentCase(current_case_x, current_case_y);
  Data  case_data     = GetCaseData(current_case_x, current_case_y);
  float movementTime  = elapsedTime * 50;
  float caseSpeed     = case_data["movement-speed"].Nil() ? 1.f : case_data["movement-speed"];
  float movementSpeed = movementTime * (caseSpeed >= 0.1f ? caseSpeed : 1);
  float a             = _current_pos_x - _goal_x;
  float b             = _goal_y        - _current_pos_y;
  float distance      = SQRT(a * a + b * b);
  float k             = (movementSpeed > distance ? 1 : movementSpeed / distance);
  float dx            = _current_pos_x + (k * _goal_x - k * _current_pos_x);
  float dy            = _current_pos_y + (k * _goal_y - k * _current_pos_y);

  _current_pos_x                   = dx;
  _current_pos_y                   = dy;
  _dataEngine["worldmap"]["pos-x"] = _current_pos_x;
  _dataEngine["worldmap"]["pos-y"] = _current_pos_y;
  if (_cursor)
  {
    float height = _cursor->GetProperty("height")->Get<float>();
    float width  = _cursor->GetProperty("width")->Get<float>();
    stringstream str_x, str_y;

    str_x << (_current_pos_x - (width / 2)) << "px";
    str_y << (_current_pos_y - (height))    << "px";
    _cursor->SetProperty("left", str_x.str().c_str());
    _cursor->SetProperty("top",  str_y.str().c_str());
  }

  // Check for a city to display
  std::string city;

  if (IsPartyInCity(city, false))
  {
    auto it = find(_cities.begin(), _cities.end(), city);

    if (!it->hidden && !it->visible)
      SetCityVisible(city);
  }

  // Update the clock
  DateTime       current_time   = _timeManager.GetDateTime();
  unsigned short lastHour       = current_time.GetHour();
  unsigned short lastDay        = current_time.GetDay();
  unsigned short elapsedHours   = movementTime;
  unsigned short elapsedMinutes = (((movementTime * 100) - (elapsedHours * 100)) / 100) * 60;
  _timeManager.AddElapsedTime(DateTime::Hours(elapsedHours) + DateTime::Minutes(elapsedMinutes));
  if (lastDay != current_time.GetDay())
    UpdateClock();
  if (lastHour != current_time.GetHour())
  {
    string which_city;
    
    UpdateClock();
    if (!(IsPartyInCity(which_city)) && Dices::Throw(92) < 3)
    {
      int x, y;

      GetCurrentCase(x, y);
      RequestRandomEncounter.Emit(x, y, true);
    }
  }
}

Core::Element* WorldMap::GetCaseAt(int x, int y) const
{
  unsigned int it = y * _size_x + x;

  if (_cases.size() > it)
    return (_cases[it]);
  return (0);
}

void WorldMap::GetCurrentCase(int& x, int& y) const
{
  x = _current_pos_x / _tsize_x;
  y = _current_pos_y / _tsize_y;
}

Data WorldMap::GetCaseData(int x, int y) const
{
  int            it       = (y * _size_x) + x;

  return (Data(_mapTree)["tiles"][it]);
}

void WorldMap::SetCaseVisibility(int x, int y, char visibility) const
{
  //int            it       = (y * _size_x) + x;
  Core::Element* element  = GetCaseAt(x, y);
  Data           dataCase = GetCaseData(x, y);
  stringstream   stream;

  if (!element) return ;
  if ((int)dataCase["visibility"] >= visibility)
    return ;
  switch ((int)visibility)
  {
    case 0:
      stream << "<div style='background: rgba(0, 0, 0, 255); width: " << _tsize_x << "px; height: " << _tsize_y << ";'></div>";
      element->SetInnerRML(stream.str().c_str());
      dataCase["visibility"] = 0;
      break ;
    case 1:
      stream << "<div style='background: rgba(0, 0, 0, 125); width: " << _tsize_x << "px; height: " << _tsize_y << ";'></div>";
      element->SetInnerRML(stream.str().c_str());
      dataCase["visibility"] = 1;
      break ;
    case 2:
      element->SetInnerRML("");
      dataCase["visibility"] = 2;
      break ;
  }
}

void WorldMap::MoveTowardsCoordinates(float x, float y)
{
  _goal_x = x;
  _goal_y = y;
}

void WorldMap::MapClicked(Rocket::Core::Event& event)
{
  Core::Element* frame = root->GetElementById("map-frame");

  if (frame)
  {
    int   click_x    = 0;
    int   click_y    = 0;
    float scrollTop  = frame->GetScrollTop();
    float scrollLeft = frame->GetScrollLeft();
    
    click_x  = event.GetParameter("mouse_x", click_x);
    click_y  = event.GetParameter("mouse_y", click_y);
    MoveTowardsCoordinates(click_x + scrollLeft, click_y + scrollTop);
  }
}

void WorldMap::MapTileGenerator(Data map)
{
  std::stringstream rml, rcss;
  Data              tiles   = map["tiles"];
  int               size_x  = map["size_x"];
  int               size_y  = map["size_y"];
  int               tsize_x = map["tile_size_x"];
  int               tsize_y = map["tile_size_y"];
  int               pos_x   = 0;
  int               pos_y   = 0;

  _size_x  = size_x;
  _size_y  = size_y;
  _tsize_x = tsize_x;
  _tsize_y = tsize_y;

  {
    //
    // Generate RCSS and RML for the Tilemap
    //
    LoadingScreen::AppendText("Generating Tiles...");
    
    rcss << "#pworldmap\n";
    rcss << "{\n" << "  background-decorator: image;\n";
    rcss << "  background-image: worldmap.png 0px 0px " << (size_x * tsize_x) << "px " <<  (size_y * tsize_y) << "px" << ";\n";
    rcss << "  width:  " << (size_x * tsize_x) << "px;\n";
    rcss << "  height: " << (size_y * tsize_y) << "px;\n";
    rcss << "}\n\n";
    
    for_each(tiles.begin(), tiles.end(), [this, &rml, &rcss, &pos_x, &pos_y, size_x, size_y, tsize_x, tsize_y](Data tile)
    {
      rml  << "<div id='tile" << pos_x << "-" << pos_y << "' class='tile' style='position: absolute;";
      rml  << "top: "  << (pos_y * tsize_y) << "px; ";
      rml  << "left: " << (pos_x * tsize_x) << "px; ";
      rml  << "height: " << tsize_y << "px; width: " << tsize_x << "px;";
      rml  << "' data-pos-x='" << pos_x << "' data-pos-y='" << pos_y << "'>";
      
      if (tile["visibility"].Value() == "0")
	rml << "<div style='background: rgba(0, 0, 0, 255); width: " << tsize_x << "px; height: " << tsize_y << ";'></div>";
      if (tile["visibility"].Value() == "1")
	rml << "<div style='background: rgba(0, 0, 0, 125); width: " << tsize_x << "px; height: " << tsize_y << ";'></div>";

      rml  << "</div>\n";

      if (++pos_x >= size_x)
      {
	++pos_y;
	pos_x = 0;
      }
    });

    //
    // Load the worldmap rml template, replace the #{RML} and #{RCSS} bits with generated RML/RCSS,
    // create a temporary RML file with the result.
    //
    ifstream file("data/worldmap.rml.tpl", std::ios::binary);

    LoadingScreen::AppendText("Compiling Worldmap.");
    if (file.is_open())
    {
      string fileRml;
      long   begin, end;
      long   size;
      char*  raw;

      begin     = file.tellg();
      file.seekg (0, ios::end);
      end       = file.tellg();
      file.seekg(0, ios::beg);
      size      = end - begin;
      raw       = new char[size + 1];
      raw[size] = 0;
      file.read(raw, size);
      file.close();
      fileRml = raw;
      delete[] raw;

      size_t firstReplaceIt = fileRml.find("#{RCSS}");
      fileRml.replace(firstReplaceIt, strlen("#{RCSS}"), rcss.str());
      size_t secReplaceIt   = fileRml.find("#{RML}");
      fileRml.replace(secReplaceIt, strlen("#{RML}"), rml.str());

      size_t win_newline;
      while ((win_newline = fileRml.find("\r\n")) != fileRml.npos)
	fileRml.replace(win_newline, 2, "\n");

      ofstream ofile("data/worldmap.rml", std::ios::binary);

      if (ofile.is_open())
      {
	ofile.write(fileRml.c_str(), fileRml.size());
	ofile.close();
      }

      //
      // Load the temporary file and link every click event from every tile
      // to the RocketListener MapClickedEvent.
      // While adding every case Element to the Case array.
      //
      LoadingScreen::AppendText("Loading compiled worldmap");
      root = context->LoadDocument("data/worldmap.rml");
    }
  }

  if (root)
  {
    stringstream           streamSizeX, streamSizeY;
    Rocket::Core::Element* mapElem = root->GetElementById("pworldmap");

    streamSizeX << (_size_x * _tsize_x);
    streamSizeY << (_size_y * _tsize_y);
    mapElem->SetProperty("width",  streamSizeX.str().c_str());
    mapElem->SetProperty("height", streamSizeY.str().c_str());
    mapElem->SetInnerRML(rml.str().c_str());
    for (pos_x = 0, pos_y = 0 ; pos_x < size_x && pos_y < size_y ;)
    {
      Rocket::Core::Element* tileElem;
      stringstream           idElem;

      idElem << "tile" << pos_x << "-" << pos_y;
      tileElem = root->GetElementById(idElem.str().c_str());
      if (tileElem)
      {
	tileElem->AddEventListener("click", &MapClickedEvent);
        _cases.push_back(tileElem);
      }
      if (++pos_x >= size_x)
      {
	++pos_y;
	pos_x = 0;
      }
    }
    unlink("data/worldmap.rml");
  }
  LoadingScreen::AppendText("Worldmap loaded.");
}

void WorldMap::SetInterrupted(bool set)
{
  _interrupted = set;
  if (!set)
    _timer.Restart();
}
