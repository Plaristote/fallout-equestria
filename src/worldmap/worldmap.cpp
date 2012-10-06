#include "worldmap/worldmap.hpp"

using namespace std;
using namespace Rocket;

WorldMap* WorldMap::CurrentWorldMap = 0;

WorldMap::~WorldMap()
{
  delete _mapTree;
  CurrentWorldMap = 0;
}

void WorldMap::Save(const string& savepath)
{
  DataTree::Writers::JSON(_mapTree, "saves/map.json");
}

WorldMap::WorldMap(WindowFramework* window, GameUi* gameUi, DataEngine& de) : UiBase(window, gameUi->GetContext()), _gameUi(*gameUi), _dataEngine(de)
{
  cout << "Building worldmap" << endl;
  _current_pos_x = _goal_x = _dataEngine["worldmap"]["pos-x"];
  _current_pos_y = _goal_y = _dataEngine["worldmap"]["pos-y"];
  
  _mapTree = DataTree::Factory::JSON("saves/map.json");
  MapTileGenerator(_mapTree);

  if (_root)
  {
    //
    // Adds the known cities to the CityList.
    //
    DataTree* cityTree = DataTree::Factory::JSON("saves/cities.json");
    {
      Data      cities(cityTree);

      std::for_each(cities.begin(), cities.end(), [this](Data city) { AddCityToList(city); });
    }
    delete cityTree;
    
    //
    // Get some required elements
    //
    _cursor       = _root->GetElementById("party-cursor");

    //
    // Event management
    //
    _root->GetElementById("button-inventory")->AddEventListener("click", &ButtonInventory);
    _root->GetElementById("button-character")->AddEventListener("click", &ButtonCharacter);
    _root->GetElementById("button-menu")->AddEventListener("click", &ButtonMenu);
    ButtonInventory.EventReceived.Connect(_gameUi, &GameUi::OpenInventory);
    ButtonCharacter.EventReceived.Connect(_gameUi, &GameUi::OpenPers);
    ButtonMenu.EventReceived.Connect(_gameUi, &GameUi::OpenMenu);
    
    _cursor->AddEventListener("click", &PartyCursorClicked);
    PartyCursorClicked.EventReceived.Connect(*this, &WorldMap::PartyClicked);

    MapClickedEvent.EventReceived.Connect(*this, &WorldMap::MapClicked);
    CityButtonClicked.EventReceived.Connect(*this, &WorldMap::CityClicked);
    
    UpdatePartyCursor(0);
  }
  CurrentWorldMap = this;
}

void WorldMap::SetCityVisible(const string& name)
{
  Cities::iterator it = find(_cities.begin(), _cities.end(), name);
  
  if (it != _cities.end())
  {
    DataTree* cityTree = DataTree::Factory::JSON("saves/cities.json");
    
    if (cityTree)
    {
      Data citiesData(cityTree);
      Data cityData = citiesData[it->name];

      if (!(cityData.Nil()))
      {
        cityData["visible"] = "1";
        AddCityToList(cityData);
        DataTree::Writers::JSON(cityTree, cityTree->GetSourceFile());
      }
      delete cityTree;
    }
  }
}

void WorldMap::SaveMapStatus(void) const
{
  DataTree::Writers::JSON(_mapTree, _mapTree->GetSourceFile());
}

void WorldMap::AddCityToList(Data cityData)
{
  City city;

  city.pos_x  = cityData["pos_x"];
  city.pos_y  = cityData["pos_y"];
  city.radius = cityData["radius"];
  city.name   = cityData.Key();
  _cities.push_back(city);
  if (cityData["visible"].Value() == "1")
  {
    Rocket::Core::Element* elem = _root->GetElementById("city-list");
    Rocket::Core::String   innerRml;
    std::stringstream      rml;

    rml << "<div class='city-entry'>";
    rml << "<div class='city-button'><button id='city-" << cityData.Key() << "' data-city='" << cityData.Key() << "'>Go</button></div>";
    rml << "<div class='city-name'>" << cityData.Key() << "</div>";
    rml << "</div>";
    elem->GetInnerRML(innerRml);
    innerRml = innerRml + Rocket::Core::String(rml.str().c_str());
    elem->SetInnerRML(innerRml);
    
    elem = _root->GetElementById(string("city-" + cityData.Key()).c_str());
    elem->AddEventListener("click", &CityButtonClicked);
  }
}

void WorldMap::Show(void)
{
  UiBase::Show();
  _current_pos_x = _dataEngine["worldmap"]["pos-x"];
  _current_pos_y = _dataEngine["worldmap"]["pos-y"];
}

void WorldMap::Run(void)
{
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

void WorldMap::PartyClicked(Rocket::Core::Event&)
{
  string cityname;

  if (IsPartyInCity(cityname))
    GoToPlace.Emit(cityname);
  else
    cout << "You aren't in any city" << endl;
}

bool WorldMap::IsPartyInCity(string& cityname) const
{
  Cities::const_iterator it  = _cities.begin();
  Cities::const_iterator end = _cities.end();
  
  for (; it != end ; ++it)
  {
    const City& city   = *it;
    float       dist_x = city.pos_x - _current_pos_x;
    float       dist_y = city.pos_y - _current_pos_y;
    float       dist   = SQRT(dist_x * dist_x + dist_y * dist_y);

    if (dist <= city.radius)
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
  float movementSpeed = elapsedTime * 50;
  /*int   dist_x        = MAX(_goal_x, _current_pos_x) - MIN(_goal_x, _current_pos_x);
  int   dist_y        = MAX(_goal_y, _current_pos_y) - MIN(_goal_y, _current_pos_y);
  float tomove_x      = movementSpeed;
  float tomove_y      = movementSpeed;*/
  

  float a        = _current_pos_x - _goal_x;
  float b        = _goal_y        - _current_pos_y;
  float distance = SQRT(a * a + b * b);

  float k  = (movementSpeed > distance ? 1 : movementSpeed / distance);
  float dx = _current_pos_x + (k * _goal_x - k * _current_pos_x);
  float dy = _current_pos_y + (k * _goal_y - k * _current_pos_y);
  
  _current_pos_x = dx;
  _current_pos_y = dy;

  /*if (dist_x != 0 && dist_y != 0)
  {
    if      (dist_x > dist_y)   tomove_x = (dist_x / dist_y) * tomove_y;
    else if (dist_y > dist_x)   tomove_y = (dist_y / dist_x) * tomove_x;
  }

  if (_goal_x < _current_pos_x) tomove_x = -tomove_x;
  if (_goal_y < _current_pos_y) tomove_y = -tomove_y;
  
  if ((_current_pos_x + tomove_x > _goal_x && _goal_x > _current_pos_x) ||
      (_current_pos_x + tomove_x < _goal_x && _goal_x < _current_pos_x))
    _current_pos_x = _goal_x;
  else
    _current_pos_x += tomove_x;
  
  if ((_current_pos_y + tomove_y > _goal_y && _goal_y > _current_pos_y) ||
      (_current_pos_y + tomove_y < _goal_y && _goal_y < _current_pos_y))
    _current_pos_y = _goal_y;
  else
    _current_pos_y += tomove_y;*/

  if (_cursor)
  {
    stringstream str_x, str_y;

    str_x << _current_pos_x << "px";
    str_y << _current_pos_y << "px";
    _cursor->SetProperty("left", str_x.str().c_str());
    _cursor->SetProperty("top",  str_y.str().c_str());
  }
  
  _dataEngine["worldmap"]["pos-x"] = _current_pos_x;
  _dataEngine["worldmap"]["pos-y"] = _current_pos_y;
}

Core::Element* WorldMap::GetCaseAt(int x, int y) const
{
  int it = y * _size_x + x;

  if (_cases.size() > it)
    return (_cases[it]);
  return (0);
}

void WorldMap::GetCurrentCase(int& x, int& y) const
{
  x = _current_pos_x / _tsize_x;
  y = _current_pos_y / _tsize_y;
}

void WorldMap::SetCaseVisibility(int x, int y, char visibility) const
{
  int            it       = (y * _size_x) + x;
  Core::Element* element  = GetCaseAt(x, y);
  Data           dataCase = Data(_mapTree)["tiles"][it];
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
  Core::Element* frame = _root->GetElementById("map-frame");

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
  
  //
  // Generate RCSS and RML for the Tilemap
  //
  std::for_each(tiles.begin(), tiles.end(), [this, &rml, &rcss, &pos_x, &pos_y, size_x, size_y, tsize_x, tsize_y](Data tile)
  {
    rcss << "#tile" << pos_x << "-" << pos_y << "\n";
    rcss << "{\n" << "  top: "  << (pos_y * tsize_y) << "px;\n" << "  left: " << (pos_x * tsize_x) << "px;\n";
    rcss << "  height: " << tsize_y << "px; width: " << tsize_x << "px;\n";
    rcss << "  background-decorator: image;\n";
    rcss << "  background-image: worldmap.png " << (pos_x * tsize_x) << "px " << (pos_y * tsize_y) << "px ";
    rcss << (pos_x * tsize_x + tsize_x) << "px " << (pos_y * tsize_y + tsize_y) << "px;\n";
    rcss << "}\n\n";

    rml  << "<div id='tile" << pos_x << "-" << pos_y << "' class='tile' style='position: absolute;";
    rml  << "top: "  << (pos_y * tsize_y) << "px; ";
    rml  << "left: " << (pos_x * tsize_x) << "px; ";
    rml  << "height: " << tsize_y << "px; width: " << tsize_x << "px; ";
    rml  << "background-decorator: image; ";
    rml  << "background-image: worldmap.png " << (pos_x * tsize_x) << "px " << (pos_y * tsize_y) << "px ";
    rml  << (pos_x * tsize_x + tsize_x) << "px " << (pos_y * tsize_y + tsize_y) << "px;";
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
  std::ifstream file("data/worldmap.rml");

  if (file.is_open())
  {
    std::string fileRml;
    long        begin, end;
    long        size;
    char*       raw;

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
    
    std::ofstream ofile("data/tmp-worldmap.rml");
    
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
    _root = _context->LoadDocument("data/tmp-worldmap.rml");
    
    if (_root)
    {
      stringstream           streamSizeX, streamSizeY;
      Rocket::Core::Element* mapElem = _root->GetElementById("pworldmap");
      
      streamSizeX << (_size_x * _tsize_x);
      streamSizeY << (_size_y * _tsize_y);
      mapElem->SetProperty("width",  streamSizeX.str().c_str());
      mapElem->SetProperty("height", streamSizeY.str().c_str());
      mapElem->SetInnerRML(rml.str().c_str());
      for (pos_x = 0, pos_y = 0 ; pos_x < size_x && pos_y < size_y ;)
      {
	Rocket::Core::Element* tileElem;
	std::stringstream      idElem;
	
	idElem << "tile" << pos_x << "-" << pos_y;
	tileElem = _root->GetElementById(idElem.str().c_str());
	if (tileElem)
	  tileElem->AddEventListener("click", &MapClickedEvent);
	if (++pos_x >= size_x)
	{
	  ++pos_y;
	  pos_x = 0;
	}
	_cases.push_back(tileElem);
      }
    }
  }
}
