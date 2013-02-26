#include "worldmapeditor.h"
#include "ui_worldmapeditor.h"
#include <QDir>
#include <QGraphicsEllipseItem>
#include <QInputDialog>
#include <iostream>
#include "selectableresource.h"

using namespace std;

WorldmapEditor::WorldmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorldmapEditor)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);

    scene.editor = this;
    scene.addItem(&pixmap_worldmap);

    connect(ui->cityList, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectedCity(QString)));

    connect(ui->cityPosX,         SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityPosY,         SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityRadius,       SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityVisible,      SIGNAL(toggled(bool)),        this, SLOT(UpdateCityData()));
    connect(ui->buttonAddCity,    SIGNAL(clicked()),            this, SLOT(AddCity()));
    connect(ui->buttonDeleteCity, SIGNAL(clicked()),            this, SLOT(DelCity()));

    connect(ui->mapSizeX,         SIGNAL(valueChanged(int)),    this, SLOT(UpdateMapData()));
    connect(ui->mapSizeY,         SIGNAL(valueChanged(int)),    this, SLOT(UpdateMapData()));
    connect(ui->tileSizeX,        SIGNAL(valueChanged(int)),    this, SLOT(UpdateMapData()));

    connect(ui->terrainDiscardSelection, SIGNAL(clicked()), this, SLOT(UnselectAllCases()));
    connect(ui->terrainSelectAll, SIGNAL(clicked()), this, SLOT(SelectAllCases()));

    connect(ui->encounterAddMap,  SIGNAL(clicked()), this, SLOT(CaseAddMap()));
    connect(ui->encounterDelMap,  SIGNAL(clicked()), this, SLOT(CaseDelMap()));
    connect(ui->encounterAddType, SIGNAL(clicked()), this, SLOT(CaseAddEncounter()));
    connect(ui->encounterDelType, SIGNAL(clicked()), this, SLOT(CaseDelEncounter()));
    connect(ui->movementSpeed,    SIGNAL(valueChanged(double)), this, SLOT(UpdateCaseData()));

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(Save()));

    lock_cities = false;
    lock_mspeed = false;
}

WorldmapEditor::~WorldmapEditor()
{
    delete ui;
}

void WorldmapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QPointF               position = event->buttonDownScenePos(Qt::LeftButton);
  QGraphicsEllipseItem  cursor(position.x() - (3.f / 2), position.y() - (3.f / 2), 3.f, 3.f);
  QList<QGraphicsItem*> colliding;
  bool                  tile_selected = false;

  colliding = collidingItems(&cursor);
  foreach(QGraphicsItem* item, colliding)
  {
    if (item->type() == 4)
      reinterpret_cast<City*>(item)->mouseReleaseEvent(event);
    else if (item->type() == 3 && !tile_selected)
    {
      reinterpret_cast<MapTile*>(item)->mouseReleaseEvent(event);
        tile_selected = true;
    }
  }
  if (!tile_selected)
    editor->MapClicked(position.x(), position.y());
  QGraphicsScene::mouseReleaseEvent(event);
}

void City::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  my_signals.Click(name);
  QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void WorldmapEditor::Load(void)
{
  QString worldmap_path = QDir::currentPath() + "/data/worldmap.png";
  QString files_path    = QDir::currentPath() + "/data/newgame/";

  {
    QPixmap pixmap;

    if (pixmap.load(worldmap_path))
      pixmap_worldmap.setPixmap(pixmap);
    else
    {} // TODO Display error about missing file
  }

  {
    // TODO Add these files to the project creation thingy
    QString file_map_path    = files_path + "map.json";
    QString file_cities_path = files_path + "cities.json";

    file_map    = DataTree::Factory::JSON(file_map_path.toStdString());
    file_cities = DataTree::Factory::JSON(file_cities_path.toStdString());
  }

  if (file_cities)
  {
    Data data(file_cities);

    std::for_each(data.begin(), data.end(), [this](Data city)
    {
      ui->cityList->addItem(QString::fromStdString(city.Key()));
      AddCityHalo(QString::fromStdString(city.Key()));
    });
  }

  if (file_map)
  {
      Data data(file_map);

      lock_cities = true;
      ui->mapSizeX->setValue(data["size_x"]);
      ui->mapSizeY->setValue(data["size_y"]);
      ui->tileSizeX->setValue(data["tile_size_x"]);
      ui->tileSizeY->setValue(data["tile_size_y"]);
      lock_cities = false;
  }
}

void WorldmapEditor::Save(void)
{
  QString     path        = QDir::currentPath() + "/data/newgame/";
  std::string path_cities = path.toStdString() + "cities.json";
  std::string path_map    = path.toStdString() + "map.json";

  DataTree::Writers::JSON(Data(file_cities), path_cities);
  DataTree::Writers::JSON(Data(file_map),    path_map);
}

void WorldmapEditor::ClickedCity(QString name)
{
  for (int i = 0 ; i < ui->cityList->count() ; ++i)
  {
    if (ui->cityList->item(i)->text() == name)
    {
      ui->cityList->setCurrentRow(i);
      break ;
    }
  }
}

void WorldmapEditor::SelectedCity(QString name)
{
  Data data(file_cities);
  Data city = data[name.toStdString()];

  ui->cityFrame->setEnabled(!(city.Nil()));
  if (!(city.Nil()))
  {
    lock_cities = true;
    ui->cityRadius->setValue(city["radius"]);
    ui->cityPosX->setValue(city["pos_x"]);
    ui->cityPosY->setValue(city["pos_y"]);
    ui->cityVisible->setChecked(city["visible"].Value() == "1");
    lock_cities = false;
  }
}

void WorldmapEditor::UpdateCityData()
{
  QListWidgetItem* list_item = ui->cityList->currentItem();

  if (list_item && !lock_cities)
  {
    Data      data(file_cities);
    Data      city  = data[list_item->text().toStdString()];
    CityHalo& halo  = GetCityHalo(list_item->text());
    float     rect_x, rect_y;

    city["radius"]  = ui->cityRadius->value();
    city["pos_x"]   = ui->cityPosX->value();
    city["pos_y"]   = ui->cityPosY->value();
    city["visible"] = ui->cityVisible->isChecked() ? "1" : "0";
    rect_x          = (float)city["pos_x"] - (float)city["radius"] / 2;
    rect_y          = (float)city["pos_y"] - (float)city["radius"] / 2;
    halo.second->setRect(rect_x, rect_y, city["radius"], city["radius"]);
  }
}

void WorldmapEditor::UpdateMapData()
{
  if (!lock_cities)
  {
    Data data(file_map);

    data["size_x"]      = ui->mapSizeX->value();
    data["size_y"]      = ui->mapSizeY->value();
    data["tile_size_x"] = ui->tileSizeX->value();
    data["tile_size_y"] = ui->tileSizeY->value();

    unsigned short tile_count = ui->mapSizeX->value() * ui->mapSizeY->value();
    unsigned int i = data["tiles"].Count();

    for (i = data["tiles"].Count() ; i < tile_count ; ++i)
    {
      std::stringstream stream;

      stream << i;
      data["tiles"][stream.str()] = 0;
      data["tiles"][stream.str()].SetKey("");
    }
    for (i = data["tiles"].Count() ; i > tile_count ; --i)
    {
      // This does NOT work. Though it's not very important.
      data["tiles"][i].Remove();
    }
  }
}

void WorldmapEditor::UnselectAllCases()
{
  foreach(MapTile* tile, tile_selection)
    scene.removeItem(tile);
  tile_selection.clear();
  ui->frameEncounters->setEnabled(false);
}

void WorldmapEditor::SelectAllCases()
{
  UnselectAllCases();
  for (unsigned short x = 0 ; x < ui->mapSizeX->value() ; ++x)
  {
    for (unsigned short y = 0 ; y < ui->mapSizeY->value() ; ++y)
    {
      SelectCase(x, y);
    }
  }
}

WorldmapEditor::CityHalo& WorldmapEditor::GetCityHalo(const QString& name)
{
  CityHalos::Iterator it, end;

  for (it = city_halos.begin(), end = city_halos.end() ; it != end ; ++it)
  {
    CityHalo& halo = *it;

    if (halo.first == name)
      return (halo);
  }
  return (city_halos.front());
}

void WorldmapEditor::AddCityHalo(const QString &name)
{
  Data data(file_cities);
  Data city = data[name.toStdString()];

  if (!(city.Nil()))
  {
    float                 pos_x  = city["pos_x"];
    float                 pos_y  = city["pos_y"];
    float                 radius = city["radius"];
    City*                 item   = new City(pos_x - radius / 2, pos_y - radius / 2, radius, radius);

    connect(&item->my_signals, SIGNAL(Clicked(QString)), this, SLOT(ClickedCity(QString)));
    item->SetName(name);
    item->setPen(QPen(QColor(50, 255, 10), 2));
    item->setBrush(QBrush(QColor(50, 255, 10, 100)));
    scene.addItem(item);
    city_halos.push_back(CityHalo(name, item));
  }
}

void WorldmapEditor::DelCityHalo(const QString &name)
{
  CityHalos::Iterator it, end;

  for (it = city_halos.begin(), end = city_halos.end() ; it != end ; ++it)
  {
    CityHalo& halo = *it;

    if (halo.first == name)
    {
      delete halo.second;
      break ;
    }
  }
}

MapTile* WorldmapEditor::GetTile(unsigned int x, unsigned int y)
{
  foreach(MapTile* tile, tile_selection)
  {
    if (tile->pos_x == x && tile->pos_y == y)
      return (tile);
  }
  return (0);
}

Data WorldmapEditor::GetTileData(unsigned int x, unsigned int y)
{
  int size_x = ui->mapSizeX->value();
  int it     = (y * size_x) + x;

  return (Data(file_map)["tiles"][it]);
}

void WorldmapEditor::SelectCase(unsigned int x, unsigned int y)
{
    MapTile* tile = GetTile(x, y);

    if (ui->mapSizeX->value() <= (int)x || ui->mapSizeY->value() <= (int)y || ui->toolBox->currentWidget() != ui->pageTiles)
      return ;
    if (tile)
    {
      tile_selection.removeOne(tile);
      scene.removeItem(tile);
      //delete tile;
    }
    else
    {
      float pos_x, pos_y, width, height;

      width  = ui->tileSizeX->value();
      height = ui->tileSizeY->value();
      pos_x  = width  * x;
      pos_y  = height * y;
      tile   = new MapTile;
      tile->pos_x = x;
      tile->pos_y = y;
      tile->setRect(pos_x, pos_y, width, height);
      tile->setPen(QPen(QColor(100, 100, 255), 1));
      tile->setBrush(QBrush(QColor(50, 255, 255, 100)));
      scene.addItem(tile);
      tile_selection.push_back(tile);
      connect(&tile->my_signals, SIGNAL(Clicked(uint,uint)), this, SLOT(SelectCase(uint,uint)));
      UpdateCaseInterface();
    }
    if (tile_selection.size() > 0)
      UpdateCaseInterface();
    ui->frameEncounters->setEnabled(tile_selection.size() > 0);
}

void WorldmapEditor::UpdateCaseInterface(void)
{
  MapTile* tile  = tile_selection.last();
  Data     data  = GetTileData(tile->pos_x, tile->pos_y);
  Data     maps  = data["map-encounters"];
  Data     types = data["type-encounters"];

  data.Output();
  ui->encounterTypes->clear();
  ui->encounterMapList->clear();
  for (unsigned short i = 0 ; i < maps.Count() ; ++i)
    ui->encounterMapList->addItem(QString::fromStdString(maps[i].Value()));
  for (unsigned short i = 0 ; i < types.Count() ; ++i)
    ui->encounterTypes->addItem(QString::fromStdString(types[i].Value()));
  lock_mspeed = true;
  ui->movementSpeed->setValue(data["movement-speed"].Nil() ? 1.f : (float)data["movement-speed"]);
  lock_mspeed = false;
}

void WorldmapEditor::MapClicked(int x, int y)
{
  unsigned int tile_x = x / ui->tileSizeX->value();
  unsigned int tile_y = y / ui->tileSizeY->value();

  SelectCase(tile_x, tile_y);
}

void WorldmapEditor::CaseAddMap()
{
  SelectableResource::MapsResource().SelectResource([this](QString map_name)
  {
  std::string value    = map_name.toStdString();

  if (map_name != "")
  {
    foreach(MapTile* tile, tile_selection)
    {
      Data           case_data = GetTileData(tile->pos_x, tile->pos_y);
      unsigned short i;

      for (i = 0 ; i < case_data.Count() ; ++i)
      {
        if (case_data["map-encounters"][i].Value() == value)
          break ;
      }
      if (i == case_data.Count())
      {
          case_data["map-encounters"][value] = value;
          case_data["map-encounters"][value].SetKey("");
      }
    }
    ui->encounterMapList->addItem(map_name);
  }
  });
}

void WorldmapEditor::CaseDelMap()
{
  QListWidgetItem* item = ui->encounterMapList->currentItem();

  if (item)
  {
    QString map_name = item->text();

    foreach(MapTile* tile, tile_selection)
    {
      Data           case_data = GetTileData(tile->pos_x, tile->pos_y);
      unsigned short i;

      for (i = 0 ; i < case_data.Count() ; ++i)
      {
        if (case_data["map-encounters"][i].Value() == map_name.toStdString())
        {
          case_data["map-encounters"][i] = "";
          case_data["map-encounters"][i].Remove();
          break ;
        }
      }
    }
    ui->encounterMapList->removeItemWidget(item);
    delete item;
  }
}

void WorldmapEditor::CaseAddEncounter()
{
  QString     map_name = QInputDialog::getText(this, "Add encounter type", "Encounter name");
  std::string value    = map_name.toStdString();

  if (map_name != "")
  {
    foreach(MapTile* tile, tile_selection)
    {
      Data           case_data = GetTileData(tile->pos_x, tile->pos_y);
      unsigned short i;

      for (i = 0 ; i < case_data.Count() ; ++i)
      {
        if (case_data["type-encounters"][i].Value() == value)
          break ;
      }
      if (i == case_data.Count())
      {
          case_data["type-encounters"][value] = value;
          case_data["type-encounters"][value].SetKey("");
      }
    }
    ui->encounterTypes->addItem(map_name);
  }
}

void WorldmapEditor::CaseDelEncounter()
{
  QListWidgetItem* item = ui->encounterTypes->currentItem();

  if (item)
  {
    QString map_name = item->text();

    foreach(MapTile* tile, tile_selection)
    {
      Data           case_data = GetTileData(tile->pos_x, tile->pos_y);
      unsigned short i;

      for (i = 0 ; i < case_data.Count() ; ++i)
      {
        if (case_data["type-encounters"][i].Value() == map_name.toStdString())
        {
          case_data["type-encounters"][i] = "";
          case_data["type-encounters"][i].Remove();
          break ;
        }
      }
    }
    ui->encounterTypes->removeItemWidget(item);
    delete item;
  }
}

void WorldmapEditor::UpdateCaseData()
{
  if (!lock_mspeed)
  {
    foreach(MapTile* tile, tile_selection)
    {
      Data case_data = GetTileData(tile->pos_x, tile->pos_y);

      case_data["movement-speed"] = ui->movementSpeed->value();
    }
  }
}

void WorldmapEditor::AddCity(void)
{
  SelectableResource::MapsResource().SelectResource([this](QString map_name)
  {
    std::string name = map_name.toStdString();
    Data        cities(file_cities);

    if (cities[name].Nil())
    {
      cities[name]["radius"] = 20;
      cities[name]["pos_x"]  = 0;
      cities[name]["pos_y"]  = 0;
      ui->cityList->addItem(map_name);
      ui->cityList->setCurrentRow(ui->cityList->count() - 1);
      AddCityHalo(map_name);
    }
  });
}

void WorldmapEditor::DelCity(void)
{
  QListWidgetItem* item = ui->cityList->currentItem();

  if (item)
  {
    QString map_name = item->text();
    Data    cities(file_cities);

    DelCityHalo(map_name);
    cities[map_name.toStdString()].Remove();
    ui->encounterTypes->removeItemWidget(item);
    delete item;
   }
}
