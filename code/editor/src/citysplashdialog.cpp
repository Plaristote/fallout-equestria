#include "citysplashdialog.h"
#include "ui_citysplashdialog.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <algorithm>
#include <iostream>
#include <functional>

CitySplashDialog::CitySplashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CitySplashDialog)
{
  ui->setupUi(this);

  connect(ui->addEntryZone,     SIGNAL(clicked()), this, SLOT(AddEntryZone()));
  connect(ui->addDiscoverZone,  SIGNAL(clicked()), this, SLOT(AddDiscoverZone()));
  connect(ui->delEntryZone,     SIGNAL(clicked()), this, SLOT(DelEntryZone()));
  connect(ui->delDiscoverZone,  SIGNAL(clicked()), this, SLOT(DelDiscoverZone()));
  connect(ui->pickSplashscreen, SIGNAL(clicked()), this, SLOT(PickSplashscreen()));
  setModal(true);
}

CitySplashDialog::~CitySplashDialog()
{
  delete ui;
}

void CitySplashDialog::EnableDelZones()
{
  ui->delEntryZone->setEnabled   (ui->entryZones->count()    > 0);
  ui->delDiscoverZone->setEnabled(ui->discoverZones->count() > 0);
}

void CitySplashDialog::Load(Data data)
{
  ui->splashscreen->setText("");
  ui->entryZones->clear();
  ui->discoverZones->clear();
  EnableDelZones();

  ui->splashscreen->setText(QString::fromStdString(data["splashcreen"].Value()));
  std::for_each(data["zones"].begin(), data["zones"].end(), [this](Data zone)
  {
    QString zone_name = QString::fromStdString(zone.Key());
    bool    available = zone == "1";

    if (available)
      ui->entryZones->addItem(zone_name);
    else
      ui->discoverZones->addItem(zone_name);
  });
}

void CitySplashDialog::Save(Data data)
{
  std::function<void (QListWidget*, int)> set_zones = [&data](QListWidget* list, int value)
  {
    for (int i = 0 ; i < list->count() ; ++i)
    {
      const std::string key = list->item(i)->text().toStdString();

      data["zones"][key] = value;
    }
  };

  data["splashcreen"] = ui->splashscreen->text().toStdString();
  set_zones(ui->entryZones,    1);
  set_zones(ui->discoverZones, 0);
}

void CitySplashDialog::AddEntryZone(void)
{
  AddZone(ui->entryZones);
}

void CitySplashDialog::AddDiscoverZone(void)
{
  AddZone(ui->discoverZones);
}

void CitySplashDialog::AddZone(QListWidget* widget)
{
  QString name = QInputDialog::getText(this, "New entry zone", "Name");

  if (name != "")
  {
    widget->addItem(name);
    EnableDelZones();
  }
}

void CitySplashDialog::DelEntryZone(void)
{
  DelZone(ui->entryZones);
}

void CitySplashDialog::DelDiscoverZone(void)
{
  DelZone(ui->discoverZones);
}

void CitySplashDialog::DelZone(QListWidget* widget)
{
  delete widget->currentItem();
  EnableDelZones();
}

void CitySplashDialog::PickSplashscreen()
{
    QString   filter    = "Images (*.png *.jpg *.bmp)";
    QString   base_path = QDir::currentPath() + "/textures/";
    QString   path      = QFileDialog::getOpenFileName(this, "Select a splash screen", base_path, filter);
    QFileInfo info(path);
    QString   relative_path;

    if (!(info.exists()))
      return ;
    if (!(path.startsWith(base_path)))
    {
      if (!(QFile::copy(path, base_path + info.fileName())))
      {
        QMessageBox::warning(this, "Error", "Couldn't copy file to the project directory.");
        return ;
      }
      path = base_path + info.fileName();
    }
    relative_path = path.remove(0, base_path.length());
    ui->splashscreen->setText(relative_path);
}
