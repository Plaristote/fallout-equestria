#include "dialognewmap.h"
#include "ui_dialognewmap.h"

DialogNewMap::DialogNewMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMap)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(CreateMap()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

DialogNewMap::~DialogNewMap()
{
    delete ui;
}

const QString DialogNewMap::GetMapName(void) const
{
    return (ui->name->text());
}
