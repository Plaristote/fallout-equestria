#include "dialognewscript.h"
#include "ui_dialognewscript.h"

DialogNewScript::DialogNewScript(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewScript)
{
    ui->setupUi(this);
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
    return (ui->name->text());
}

QString DialogNewScript::GetTemplate(void) const
{
    return (ui->templateList->currentText());
}

void DialogNewScript::Clear(void)
{
    ui->name->clear();
}
