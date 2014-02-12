#include "formkeyvalue.h"
#include "ui_formkeyvalue.h"

FormKeyValue::FormKeyValue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormKeyValue)
{
    ui->setupUi(this);

    connect(ui->value, SIGNAL(textChanged(QString)), this, SIGNAL(ValueChanged(QString)));
}

FormKeyValue::~FormKeyValue()
{
    delete ui;
}

void FormKeyValue::FromData(Data data)
{
    SetKey(QString::fromStdString(data.Key()));
    SetValue(QString::fromStdString(data.Value()));
}

void FormKeyValue::SetValue(QString value)
{
    ui->value->setText(value);
}

void FormKeyValue::SetKey(QString key)
{
    ui->key->setText(key);
}

QString FormKeyValue::GetKey(void) const
{
    return (ui->key->text());
}

QString FormKeyValue::GetValue(void) const
{
    return (ui->value->text());
}
