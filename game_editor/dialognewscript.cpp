#include "dialognewscript.h"
#include "ui_dialognewscript.h"
#include <QFile>
#include <QDir>

DialogNewScript::DialogNewScript(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewScript)
{
    ui->setupUi(this);
    UpdatedType();
    connect(ui->typeList, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdatedType()));
}

DialogNewScript::~DialogNewScript()
{
    delete ui;
}

int DialogNewScript::GetType(void) const
{
    return (ui->typeList->currentIndex());
}

QString DialogNewScript::GetName(void) const
{
    if (ui->typeList->currentText() == "Level")
      return (ui->levelList->currentText());
    return (ui->name->text());
}

void DialogNewScript::Clear(void)
{
    ui->name->clear();
}

void DialogNewScript::UpdatedType(void)
{
  if (ui->typeList->currentText() == "Level")
  {
    ui->labelLevel->show();
    ui->levelList->show();
    ui->labelName->hide();
    ui->name->hide();
  }
  else
  {
    ui->labelLevel->hide();
    ui->levelList->hide();
    ui->labelName->show();
    ui->name->show();
  }
}

void DialogNewScript::UpdateLevelList(QStringList levels)
{
  ui->levelList->clear();
  foreach (QString level, levels)
  {
    QFile file(QDir::currentPath() + "/scripts/level/" + level + ".as");

    if (!(file.exists()))
      ui->levelList->addItem(level);
  }
}
