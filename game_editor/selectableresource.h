#ifndef SELECTABLERESOURCE_H
#define SELECTABLERESOURCE_H

#include <QDialog>
#include <functional>

namespace Ui { class SelectableResource; }

class SelectableResource : public QDialog
{
    Q_OBJECT

public:
    explicit SelectableResource(QWidget *parent = 0);
    ~SelectableResource();

    void AddResource(const QString& name);
    void DelResource(const QString& name);
    void SelectResource(std::function<void (QString)> callback);

    static SelectableResource& MapsResource(void)
    {
      static SelectableResource* ptr = new SelectableResource;

      return (*ptr);
    }

    static SelectableResource& Charsheets(void)
    {
      static SelectableResource* ptr = new SelectableResource;

      return (*ptr);
    }

    static SelectableResource& AIs(void)
    {
      static SelectableResource* ptr = new SelectableResource;

      return (*ptr);
    }

    static SelectableResource& Encounters(void)
    {
      static SelectableResource* ptr = new SelectableResource;

      return (*ptr);
    }

private slots:
    void SearchList(void);
    void SelectedItem(void);

private:
    Ui::SelectableResource *ui;

    QList<QString>                resources;
    std::function<void (QString)> callback;
};

#endif // SELECTABLERESOURCE_H
