#include "selectableresource.h"
#include "ui_selectableresource.h"

SelectableResource::SelectableResource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectableResource)
{
    ui->setupUi(this);

    connect(ui->search,    SIGNAL(textChanged(QString)), this, SLOT(SearchList()));
    connect(ui->buttonBox, SIGNAL(accepted()),           this, SLOT(SelectedItem()));
    connect(ui->buttonBox, SIGNAL(rejected()),           this, SLOT(hide()));
    setModal(true);
}

SelectableResource::~SelectableResource()
{
    delete ui;
}

void SelectableResource::SelectResource(std::function<void (QString)> callback)
{
  this->callback = callback;
  this->show();
}

void SelectableResource::AddResource(const QString& name)
{
  resources.push_back(name);
  ui->list->addItem(name);
}

void SelectableResource::DelResource(const QString& name)
{
  resources.removeAll(name);
  for (unsigned short i = 0 ; i < ui->list->count() ; ++i)
  {
    QListWidgetItem* item = ui->list->item(i);

    if (item->text() == name)
    {
      ui->list->removeItemWidget(item);
      delete item;
      --i;
    }
  }
}

void SelectableResource::SearchList()
{
  QString to_search = ui->search->text();

  for (unsigned short i = 0 ; i < ui->list->count() ; ++i)
  {
    QString item_text = ui->list->item(i)->text();
    QRegExp regexp(to_search);

    if (to_search == "")
      ui->list->item(i)->setHidden(false);
    else
      ui->list->item(i)->setHidden(!(item_text.contains(regexp)));
  }
}

void SelectableResource::SelectedItem(void)
{
    QListWidgetItem* selected = ui->list->currentItem();

    if (selected)
      callback(selected->text());
}

