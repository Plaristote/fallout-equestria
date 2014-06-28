#include "factiondialog.h"
#include "ui_factiondialog.h"
#include "selectableresource.h"
#include <QInputDialog>

using namespace std;

FactionDialog::FactionDialog(QWidget *parent) : QDialog(parent),
  ui(new Ui::FactionDialog)
{
  ui->setupUi(this);
  data_engine = 0;
  connect(ui->addAlly,       SIGNAL(clicked()),                   this, SLOT(AddToAllies()));
  connect(ui->addEnemy,      SIGNAL(clicked()),                   this, SLOT(AddToEnemies()));
  connect(ui->removeAlly,    SIGNAL(clicked()),                   this, SLOT(RemoveFromAllies()));
  connect(ui->removeEnemy,   SIGNAL(clicked()),                   this, SLOT(RemoveFromEnemies()));
  connect(ui->factionList,   SIGNAL(currentTextChanged(QString)), this, SLOT(UpdateCurrentFaction(QString)));
  connect(ui->addFaction,    SIGNAL(clicked()),                   this, SLOT(AddFaction()));
  connect(ui->removeFaction, SIGNAL(clicked()),                   this, SLOT(RemoveFaction()));
}

FactionDialog::~FactionDialog()
{
  if (data_engine)
    delete data_engine;
  delete ui;
}

const QString& FactionDialog::GetCurrentFaction() const
{
  return (current_faction);
}

void FactionDialog::Initialize()
{
  if (data_engine != 0)
    delete data_engine;
  data_engine = DataTree::Factory::JSON("data/newgame/dataengine.json");
  if (data_engine)
  {
    Data data(data_engine);
    Data factions = data["Factions"];

    ui->factionList->clear();
    for_each(factions.begin(), factions.end(), [this](Data faction)
    {
      SelectableResource::Factions().AddResource(faction.Key().c_str());
      ui->factionList->addItem(faction.Key().c_str());
    });
  }
}

void FactionDialog::Save()
{
  DataTree::Writers::JSON(data_engine, "data/newgame/dataengine.json");
}

Data FactionDialog::GetCurrentFactionData(void)
{
  return (Data(data_engine)["Factions"][current_faction.toStdString()]);
}

void FactionDialog::AddToAllies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

  if (faction["Allies"][faction_name.toStdString()].Value() != "1")
    ui->alliesList->addItem(faction_name);
  faction["Allies"][faction_name.toStdString()] = 1;
  faction["Enemies"][faction_name.toStdString()].Remove();
}

void FactionDialog::AddToEnemies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

  if (faction["Enemies"][faction_name.toStdString()].Value() != "1")
    ui->enemiesList->addItem(faction_name);
  faction["Enemies"][faction_name.toStdString()] = 1;
  faction["Allies"][faction_name.toStdString()].Remove();
}

void FactionDialog::RemoveFromAllies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

  faction["Allies"][faction_name.toStdString()].Remove();
}

void FactionDialog::RemoveFromEnemies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

  faction["Enemies"][faction_name.toStdString()].Remove();
}

void FactionDialog::AddToAllies()
{
  SelectableResource::Factions().SelectResource([this](QString faction) { AddToAllies(faction); });
}

void FactionDialog::AddToEnemies()
{
  SelectableResource::Factions().SelectResource([this](QString faction) { AddToEnemies(faction); });
}

void FactionDialog::RemoveFromAllies()
{
  QListWidgetItem* item = ui->alliesList->currentItem();

  if (item)
  {
    QString        name = item->text();
    Data           data = GetCurrentFactionData();

    ui->alliesList->removeItemWidget(item);
    delete item;
    data["Allies"][name.toStdString()] = 0;
    data["Allies"][name.toStdString()].Remove();
  }
}

void FactionDialog::RemoveFromEnemies()
{
    QListWidgetItem* item = ui->enemiesList->currentItem();

    if (item)
    {
      QString        name = item->text();
      Data           data = GetCurrentFactionData();

      ui->enemiesList->removeItemWidget(item);
      delete item;
      data["Enemies"][name.toStdString()] = 0;
      data["Enemies"][name.toStdString()].Remove();
    }
}

void FactionDialog::UpdateCurrentFaction(QString new_current_faction)
{
  current_faction = new_current_faction;
  InitializeAllies();
  InitializeEnemies();
  if (ui->factionList->currentItem() && ui->factionList->currentItem()->text() != new_current_faction)
  {
    for (int i = 0 ; i < ui->factionList->count() ; ++i)
    {
      if (ui->factionList->item(i)->text() == new_current_faction)
      {
        ui->factionList->setCurrentRow(i);
        break ;
      }
    }
  }
}

void FactionDialog::InitializeAllies()
{
  Data faction = GetCurrentFactionData();
  Data allies  = faction["Allies"];

  ui->alliesList->clear();
  for_each(allies.begin(), allies.end(), [this](Data ally)
  {
    if (ally == 1)
      ui->alliesList->addItem(ally.Key().c_str());
  });
}

void FactionDialog::InitializeEnemies()
{
  Data faction = GetCurrentFactionData();
  Data enemies = faction["Enemies"];

  ui->enemiesList->clear();
  for_each(enemies.begin(), enemies.end(), [this](Data enemy)
  {
    if (enemy == 1)
      ui->enemiesList->addItem(enemy.Key().c_str());
  });
}

void FactionDialog::AddFaction()
{
  QString name = QInputDialog::getText(this, "New faction", "Faction name");

  if (name != "")
  {
    Data factions = Data(data_engine)["Factions"];

    if (factions[name.toStdString()].Nil())
    {
      ui->factionList->addItem(name);
      factions[name.toStdString()] = 1;
    }
  }
}

void FactionDialog::RemoveFaction()
{
  QListWidgetItem* item = ui->factionList->currentItem();

  if (item)
  {
    Data(data_engine)["Factions"][item->text().toStdString()].Remove();
    ui->factionList->removeItemWidget(item);
    delete item;
  }
}
