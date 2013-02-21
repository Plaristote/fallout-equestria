#include "worldmapeditor.h"
#include "ui_worldmapeditor.h"
#include <QDir>
#include <QGraphicsEllipseItem>

WorldmapEditor::WorldmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorldmapEditor)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);

    scene.addItem(&pixmap_worldmap);

    connect(ui->cityList, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectedCity(QString)));

    connect(ui->cityPosX,    SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityPosY,    SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityRadius,  SIGNAL(valueChanged(double)), this, SLOT(UpdateCityData()));
    connect(ui->cityVisible, SIGNAL(toggled(bool)),        this, SLOT(UpdateCityData()));

    connect(ui->mapSizeX,  SIGNAL(valueChanged(int)), this, SLOT(UpdateMapData()));
    connect(ui->mapSizeY,  SIGNAL(valueChanged(int)), this, SLOT(UpdateMapData()));
    connect(ui->tileSizeX, SIGNAL(valueChanged(int)), this, SLOT(UpdateMapData()));

    lock_cities = false;
}

WorldmapEditor::~WorldmapEditor()
{
    delete ui;
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
  file_cities->Save();
  file_map->Save();
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
    Data data(file_cities);
    Data city = data[list_item->text().toStdString()];

    city["radius"]  = ui->cityRadius->value();
    city["pos_x"]   = ui->cityPosX->value();
    city["pos_y"]   = ui->cityPosY->value();
    city["visible"] = ui->cityVisible->isChecked() ? "1" : "0";
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
    QGraphicsEllipseItem* item   = new QGraphicsEllipseItem(pos_x, pos_y, radius, radius);

    item->setPen(QPen(QColor(50, 255, 10), 5));
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
