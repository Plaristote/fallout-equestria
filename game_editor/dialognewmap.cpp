#include "dialognewmap.h"
#include "ui_dialognewmap.h"

DialogNewMap::DialogNewMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMap)
{
    ui->setupUi(this);
}

DialogNewMap::~DialogNewMap()
{
    delete ui;
}
