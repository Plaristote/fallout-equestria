#include "charsheeteditor.h"
#include "ui_charsheeteditor.h"
#include "selectableresource.h"
#include <QDir>
#include <QMessageBox>
#include <QProcess>

#include <iostream>
using namespace std;

CharsheetEditor::CharsheetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharsheetEditor)
{
    ui->setupUi(this);
    this->setEnabled(false);
    charsheet = 0;

    ui->frameStatistics->layout()->setAlignment(Qt::AlignTop);
    ui->frameSkills->layout()->setAlignment(Qt::AlignTop);

    special_widgets << SpecialWidget("STR", ui->strength);
    special_widgets << SpecialWidget("PER", ui->perception);
    special_widgets << SpecialWidget("END", ui->endurance);
    special_widgets << SpecialWidget("CHA", ui->charisma);
    special_widgets << SpecialWidget("INT", ui->intelligence);
    special_widgets << SpecialWidget("AGI", ui->agility);
    special_widgets << SpecialWidget("LUC", ui->luck);
}

CharsheetEditor::~CharsheetEditor()
{
    delete ui;
}

void CharsheetEditor::New(QString name)
{
  QString path = QDir::currentPath() + "/data/charsheets/" + name + ".json";

  if (!charsheet)
    charsheet = new DataTree;
  DataTree::Writers::JSON(charsheet, path.toStdString());
  SelectableResource::Charsheets().AddResource(name);
}

void CharsheetEditor::Delete()
{
  QString path = QDir::currentPath() + "/data/charsheets/" + name + ".json";

  QFile file(path);

  if (!(file.remove()))
    QMessageBox::warning(this, "Error", "Couldn't remove file '" + path + "'");
  SelectableResource::Charsheets().DelResource(name);
  this->setEnabled(false);
}

void CharsheetEditor::Load(QString name)
{
  QString path = QDir::currentPath() + "/data/charsheets/" + name + ".json";

  this->name = name;
  if (charsheet)
    delete charsheet;
  charsheet = DataTree::Factory::JSON(path.toStdString());
  if (charsheet)
  {
    Data special = Charsheet()["Special"];
    Data stats   = Charsheet()["Statistics"];
    Data skills  = Charsheet()["Skills"];

    foreach(SpecialWidget item, special_widgets)
    {
      Data value = special[item.first.toStdString()];

      item.second->setValue(value);
    }
    ui->name->setText(QString::fromStdString(Charsheet()["Name"].Value()));
    ui->age->setValue(Charsheet()["Age"]);
    ui->race->setText(QString::fromStdString(Charsheet()["Race"].Value()));
    ui->faction->setText(QString::fromStdString(Charsheet()["Faction"].Value()));
    ui->hitPoints->setValue(Charsheet()["Variables"]["Hit Points"]);
    ui->level->setValue(Charsheet()["Variables"]["Level"]);

    foreach(StatWidget widget, statistics_widgets)
      delete widget;
    foreach(StatWidget widget, skills_widgets)
      delete widget;
    statistics_widgets.clear();
    skills_widgets.clear();

    for (unsigned short i = 0 ; i < stats.Count() ; ++i)
    {
      FormKeyValue* widget = new FormKeyValue(ui->frameStatistics);

      ui->frameStatistics->layout()->addWidget(widget);
      widget->FromData(stats[i]);
      widget->show();
      statistics_widgets << widget;
    }
    for (unsigned short i = 0 ; i < skills.Count() ; ++i)
    {
      FormKeyValue* widget = new FormKeyValue(ui->frameSkills);

      ui->frameSkills->layout()->addWidget(widget);
      widget->FromData(skills[i]);
      widget->show();
      skills_widgets << widget;
    }
    this->setEnabled(true);
  }
}

void CharsheetEditor::UpdateDataTree()
{
  Data data    = Charsheet();
  Data special = Charsheet()["Special"];
  Data stats   = Charsheet()["Statistics"];
  Data skills  = Charsheet()["Skills"];

  foreach(SpecialWidget item, special_widgets)
  {
    Data value = special[item.first.toStdString()];

    value = item.second->value();
  }
  foreach(StatWidget widget, statistics_widgets)
  {
    std::string key = widget->GetKey().toStdString();

    stats[key] = widget->GetValue().toStdString();
  }
  foreach(StatWidget widget, skills_widgets)
  {
    std::string key = widget->GetKey().toStdString();

    skills[key] = widget->GetValue().toStdString();
  }
  data["Name"]    = ui->name->text().toStdString();
  data["Age"]     = ui->age->value();
  data["Race"]    = ui->race->text().toStdString();
  data["Faction"] = ui->faction->text().toStdString();
  data["Variables"]["Hit Points"] = ui->hitPoints->value();
  data["Variables"]["Level"]      = ui->level->value();
}

void CharsheetEditor::Save()
{
  if (charsheet)
  {
    QString path = QDir::currentPath() + "/data/charsheets/" + name + ".json";

    UpdateDataTree();
    if (DataTree::Writers::JSON(Charsheet(), path.toStdString()))
    {
      int ret = QMessageBox::warning(this, "Do you want to compile ?", "Do you also want to compile the statsheet using the special.as script ?", QMessageBox::Yes, QMessageBox::No);

      if (ret == QMessageBox::Yes)
        Compile();
    }
    else
      QMessageBox::warning(this, "Error", "Failed to save the statistic sheet");
  }
}

void CharsheetEditor::Compile(void)
{
  if (charsheet)
  {
    QString     working_directory = QDir::currentPath();
    QProcess    process(this);
    QStringList args;

    args << "--compile-statsheet" << name;
    process.setWorkingDirectory(working_directory);
    process.start("./game", args);
    if (process.waitForFinished())
    {
      if (process.exitCode() == 0)
        Load(name);
      else
        QMessageBox::warning(this, "Error", "The statsheet compiling failed, most likely the special.as script crashed.");
    }
    else
      QMessageBox::warning(this, "Error", "The statsheet compiling failed to start.");
    std::cout << "Process exit => " << process.exitCode() << std::endl;
  }
}
