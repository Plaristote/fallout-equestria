#include "dialogsavemap.h"
#include "ui_dialogsavemap.h"

DialogSaveMap::DialogSaveMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveMap)
{
    ui->setupUi(this);
}

DialogSaveMap::~DialogSaveMap()
{
    delete ui;
}

bool DialogSaveMap::DoCompileDoors()
{
  return (ui->compileDoors->isChecked());
}

bool DialogSaveMap::DoCompileWaypoints()
{
  return (ui->compileWaypoints->isChecked());
}
