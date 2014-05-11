#include "globals.hpp"
#include "charsheeteditor.h"
#include "ui_charsheeteditor.h"
#include "selectableresource.h"
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include "qpandaapplication.h"
#include "world/world.h"

#include <iostream>
using namespace std;

CharsheetEditor::CharsheetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharsheetEditor)
{
  QIcon iconAppearance("icons/appearance.png");
  QIcon iconInventory("icons/item.png");
  QIcon iconStatistics("icons/statistics.png");
  QIcon iconBehaviour("icons/behaviour.png");

    ui->setupUi(this);
    this->setEnabled(false);
    charsheet = 0;

    ui->tabWidget->setTabIcon(0, iconStatistics);
    ui->tabWidget->setTabIcon(1, iconInventory);
    ui->tabWidget->setTabIcon(2, iconAppearance);
    ui->tabWidget->setTabIcon(3, iconBehaviour);

    ui->frameStatistics->layout()->setAlignment(Qt::AlignTop);
    ui->frameSkills->layout()->setAlignment(Qt::AlignTop);

    special_widgets << SpecialWidget("STR", ui->strength);
    special_widgets << SpecialWidget("PER", ui->perception);
    special_widgets << SpecialWidget("END", ui->endurance);
    special_widgets << SpecialWidget("CHA", ui->charisma);
    special_widgets << SpecialWidget("INT", ui->intelligence);
    special_widgets << SpecialWidget("AGI", ui->agility);
    special_widgets << SpecialWidget("LUC", ui->luck);

    connect(ui->model,   SIGNAL(textChanged(QString)), this, SLOT(UpdateAppearence()));
    connect(ui->texture, SIGNAL(textChanged(QString)), this, SLOT(UpdateAppearence()));
    connect(ui->script,  SIGNAL(textChanged(QString)), this, SLOT(UpdateBehaviour()));
    connect(ui->dialog,  SIGNAL(textChanged(QString)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionTalkTo,    SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionUse,       SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionUseObject, SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionUseSpell,  SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionUseSkill,  SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->interactionLookAt,    SIGNAL(toggled(bool)), this, SLOT(UpdateBehaviour()));
    connect(ui->pickScript,  SIGNAL(clicked()), this, SLOT(SelectScript()));
    connect(ui->pickDialog,  SIGNAL(clicked()), this, SLOT(SelectDialog()));
    connect(ui->pickModel,   SIGNAL(clicked()), this, SLOT(SelectModel()));
    connect(ui->pickTexture, SIGNAL(clicked()), this, SLOT(SelectTexture()));
}

CharsheetEditor::~CharsheetEditor()
{
    delete ui;
}

void CharsheetEditor::SelectScript()
{
  SelectableResource::AIs().SelectResource([this](QString name)
  {
    ui->script->setText(name);
  });
}

void CharsheetEditor::SelectDialog()
{
  SelectableResource::Dialogs().SelectResource([this](QString name)
  {
    ui->dialog->setText(name);
  });
}

QString PickFile(QWidget* parent, QString title, QString filter, QString base_path);

void CharsheetEditor::SelectModel(void)
{
  QString filter    = "Panda3D Models (*.egg *.bam *.egg.pz *.bam.pz *.obj)";
  QString base_path = QDir::currentPath() + "/models/";
  QString result    = PickFile(this, "Select a model", filter, base_path);

  if (result != "" && charsheet)
  {
    Data stats(charsheet);
    ui->model->setText(result);
    stats["Appearance"]["model"]   = result.toStdString();
  }
}

void CharsheetEditor::SelectTexture(void)
{
  QString   filter    = "Images (*.png *.jpg *.bmp)";
  QString   base_path = QDir::currentPath() + "/textures/";
  QString   result    = PickFile(this, "Select a texture", filter, base_path);

  if (result != "" && charsheet)
  {
    Data stats(charsheet);
    ui->texture->setText(result);
    stats["Appearance"]["texture"] = result.toStdString();
  }
}

void CharsheetEditor::UpdateBehaviour(void)
{
  if (charsheet && this->isEnabled())
  {
    Data          stats(charsheet);
    unsigned char interactionFlag = 0;

    interactionFlag |= (ui->interactionTalkTo->isChecked()    ? Interactions::TalkTo    : 0);
    interactionFlag |= (ui->interactionUse->isChecked()       ? Interactions::Use       : 0);
    interactionFlag |= (ui->interactionUseObject->isChecked() ? Interactions::UseObject : 0);
    interactionFlag |= (ui->interactionUseSkill->isChecked()  ? Interactions::UseSkill  : 0);
    interactionFlag |= (ui->interactionUseSpell->isChecked()  ? Interactions::UseSpell  : 0);
    interactionFlag |= (ui->interactionLookAt->isChecked()    ? Interactions::LookAt    : 0);
    cout << "updating behaviour of " << stats["Name"].Value() << " to " << (int)interactionFlag << endl;
    stats["Behaviour"]["interactions"] = (int)interactionFlag;
    stats["Behaviour"]["script"]       = ui->script->text().toStdString();
    stats["Behaviour"]["dialog"]       = ui->dialog->text().toStdString();
  }
}

void CharsheetEditor::UpdateAppearence(void)
{
  if (charsheet)
  {
    /*Data             stats(charsheet);
    QString          model_path      = ui->model->text();
    QString          texture_path    = ui->texture->text();
    WindowFramework* window          = ui->modelPreview->Window();
    PandaFramework&  panda_framework = QPandaApplication::Framework();

    window = panda_framework.get_window(0);
    if (window && model_path != "")
    {
      NodePath    render = window->get_render();
      NodePath    model  = render.get_child(0);
      PT(Texture) texture;

      window->setup_trackball();
      //if (!model.is_empty())
      //  model.detach_node();
      model   = window->load_model(panda_framework.get_models(), MODEL_ROOT + model_path.toStdString());
      if (texture_path != "")
      {
        texture = TexturePool::load_texture(TEXT_ROOT + texture_path.toStdString());
        if (texture)
          model.set_texture(texture);
      }
      model.reparent_to(render);
      model.set_pos(0, 0, 0);
      model.set_hpr(0, 0, 0);
      model.set_scale(2, 2, 2);
      model.show();
    }*/
  }
}

void CharsheetEditor::New(QString name)
{
  QString path = QDir::currentPath() + "/data/charsheets/" + name + ".json";

  if (!charsheet)
    charsheet = new DataTree;
  this->name  = name;
  {
    Data special   = Data(charsheet)["Special"];
    special["STR"] = 5;
    special["PER"] = 5;
    special["END"] = 5;
    special["CHA"] = 5;
    special["INT"] = 5;
    special["AGI"] = 5;
    special["LUC"] = 5;
  }
  SelectableResource::Charsheets().AddResource(name);
  DataTree::Writers::JSON(Charsheet(), path.toStdString());
  Compile();
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

    this->setEnabled(false);
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

    ui->model->setText  (QString::fromStdString(Charsheet()["Appearance"]["model"].Value()));
    ui->texture->setText(QString::fromStdString(Charsheet()["Appearance"]["texture"].Value()));

    ui->script->setText (QString::fromStdString(Charsheet()["Behaviour"]["script"].Value()));
    ui->dialog->setText (QString::fromStdString(Charsheet()["Behaviour"]["dialog"].Value()));

    ui->inventory->SetInventory(Charsheet()["Inventory"]);
    {
      int interactionFlag = Charsheet()["Behaviour"]["interactions"].Or(0);
      cout << "interactionFlag of " << Charsheet()["Name"].Value() << ": " << interactionFlag << endl;
      ui->interactionUse->setChecked      (interactionFlag & Interactions::Use);
      ui->interactionTalkTo->setChecked   (interactionFlag & Interactions::TalkTo);
      ui->interactionUseObject->setChecked(interactionFlag & Interactions::UseObject);
      ui->interactionUseSkill->setChecked (interactionFlag & Interactions::UseSkill);
      ui->interactionUseSpell->setChecked (interactionFlag & Interactions::UseSpell);
      ui->interactionLookAt->setChecked   (interactionFlag & Interactions::LookAt);
    }

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
  data["Gender"]  = ui->sex->currentIndex() == 0 ? "Male" : "Female";
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
#ifdef _WIN32
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("Path", "..\\Panda3D-1.9.0\\bin;" + env.value("Path"));
    process.setProcessEnvironment(env);
    process.start("./game.exe", args);
#else
    process.start("./game", args);
#endif
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
