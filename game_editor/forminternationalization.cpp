#include "forminternationalization.h"
#include "ui_forminternationalization.h"
#include <QInputDialog>
#include <QMessageBox>

FormInternationalization::FormInternationalization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormInternationalization)
{
    QIcon iconSave("icons/save.png");

    ui->setupUi(this);
    ui->stringSave->setIcon(iconSave);
    currentLanguage = 0;

    connect(ui->stringNew, SIGNAL(clicked()), this, SLOT(NewString()));
    connect(ui->stringDelete, SIGNAL(clicked()), this, SLOT(DeleteString()));
    connect(ui->stringList, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectedStringChanged(QString)));
    connect(ui->stringEdit, SIGNAL(textChanged()), this, SLOT(CurrentStringChanged()));
    //connect(ui->stringEdit, SIGNAL())
    connect(ui->stringSave, SIGNAL(clicked()), this, SLOT(SaveCurrentString()));
}

FormInternationalization::~FormInternationalization()
{
    delete ui;
}

void FormInternationalization::LoadLanguage(DataTree* tree)
{
    Data data(tree);
    Data::iterator it  = data.begin();
    Data::iterator end = data.end();

    currentLanguage = tree;
    ui->stringList->clear();
    for (; it != end ; ++it)
      ui->stringList->addItem(QString::fromStdString((*it).Key()));
    ui->stringList->sortItems(Qt::AscendingOrder);
    this->setEnabled(true);
}

void FormInternationalization::CurrentStringChanged()
{
    //lastSavedString = ui->stringEdit->toPlainText();
}

void FormInternationalization::SelectedStringChanged(QString name)
{
    Data data(currentLanguage);

    if (lastKey != "" && lastSavedString != ui->stringEdit->toPlainText())
    {
        int i = QMessageBox::warning(this, "Save string", "Do you want to save the previous string", QMessageBox::Yes, QMessageBox::No);

        if (i == QMessageBox::Yes)
          SaveCurrentString();
    }

    ui->stringEdit->clear();
    ui->stringEdit->setPlainText(QString::fromStdString(data[name.toStdString()].Value()));
    lastSavedString = ui->stringEdit->toPlainText();
    lastKey         = name;
}

void FormInternationalization::NewString()
{
    QString key = QInputDialog::getText(this, "New String", "Key");

    if (key != "")
    {
      ui->stringList->addItem(key);
      ui->stringList->sortItems(Qt::AscendingOrder);
    }
}

void FormInternationalization::DeleteString()
{
    if (lastKey != "")
    {
        int i = QMessageBox::warning(this, "Remove string", "Do you really want to delete '" + lastKey + "'", QMessageBox::Yes, QMessageBox::No);

        if (i == QMessageBox::Yes)
        {
            Data data(currentLanguage);

            data[lastKey.toStdString()].Remove();
            DataTree::Writers::JSON(data, currentLanguage->GetSourceFile());
            delete ui->stringList->currentItem();
        }
    }
}

void FormInternationalization::SaveCurrentString()
{
    if (lastKey != "")
    {
        Data    data(currentLanguage);
        QString key = lastKey;

        data[key.toStdString()] = ui->stringEdit->toPlainText().toStdString();
        DataTree::Writers::JSON(data, currentLanguage->GetSourceFile());
    }
}
