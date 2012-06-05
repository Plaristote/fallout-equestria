#include "wizardobject.h"
#include "ui_wizardobject.h"

extern QString objectTypes[];

WizardObject::WizardObject(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardObject)
{
    ui->setupUi(this);
}

WizardObject::~WizardObject()
{
    delete ui;
}

const QString WizardObject::GetName() const
{
    return (ui->name->text());
}

const QString WizardObject::GetModel(void) const
{
    return (ui->model->text());
}

const QString WizardObject::GetTexture(void) const
{
    return (ui->texture->text());
}

int            WizardObject::GetPosX(void) const
{
    return (ui->posx->value());
}

int            WizardObject::GetPosY(void) const
{
    return (ui->posy->value());
}

float         WizardObject::GetScale(void) const
{
    return (ui->scale->value());
}

const QString WizardObject::GetType(void) const
{
    return ("ui->type->currentText()");
}
