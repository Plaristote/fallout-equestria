#ifndef FACTIONDIALOG_H
#define FACTIONDIALOG_H

#include <QDialog>

namespace Ui {
  class FactionDialog;
}

class FactionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit FactionDialog(QWidget *parent = 0);
  ~FactionDialog();

  const QString& GetCurrentFaction(void) const;

public slots:
  void UpdateCurrentFaction(QString);

private slots:
  void Initialize();
  void AddToAllies();
  void AddToEnemies();
  void RemoveFromAllies();
  void RemoveFromEnemies();

private:
  void AddToAllies(const QString&);
  void AddToEnemies(const QString&);
  void RemoveFromAllies(const QString&);
  void RemoveFromEnemies(const QString&);

  Data GetCurrentFactionData(void);

  Ui::FactionDialog *ui;
  QString            current_faction;
  DataTree*          data_engine;
};

#endif // FACTIONDIALOG_H
