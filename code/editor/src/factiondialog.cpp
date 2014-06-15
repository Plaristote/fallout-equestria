#include "factiondialog.h"
#include "ui_factiondialog.h"

FactionDialog::FactionDialog(QWidget *parent) : QDialog(parent),
  ui(new Ui::FactionDialog)
{
  ui->setupUi(this);
  data_engine = 0;
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
  if (data_engine == 0)
    data_engine = DataTree::Factory::JSON("data/newgame/dataengine.json");
}

Data FactionDialog::GetCurrentFactionData(void)
{
  return (Data(data_engine)["Factions"][current_faction.toStdString()]);
}

void FactionDialog::AddToAllies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

  faction["Allies"][faction_name.toStdString()] = 1;
  faction["Enemies"][faction_name.toStdString()].Remove();
}

void FactionDialog::AddToEnemies(const QString& faction_name)
{
  Data faction = GetCurrentFactionData();

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

