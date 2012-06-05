#include "dialognewdynobject.h"
#include "ui_dialognewdynobject.h"

DialogNewDynObject::DialogNewDynObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDynObject)
{
    ui->setupUi(this);
}

DialogNewDynObject::~DialogNewDynObject()
{
    delete ui;
}
