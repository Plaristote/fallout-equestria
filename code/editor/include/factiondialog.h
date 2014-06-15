#ifndef FACTIONDIALOG_H
#define FACTIONDIALOG_H

#include <QDialog>
#include "datatree.hpp"

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

  void Initialize();
  void Save();
public slots:
  void UpdateCurrentFaction(QString);

private slots:
  void AddFaction();
  void RemoveFaction();
  void AddToAllies();
  void AddToEnemies();
  void RemoveFromAllies();
  void RemoveFromEnemies();

private:
  void AddToAllies(const QString&);
  void AddToEnemies(const QString&);
  void RemoveFromAllies(const QString&);
  void RemoveFromEnemies(const QString&);

  void InitializeAllies();
  void InitializeEnemies();

  Data GetCurrentFactionData(void);

  Ui::FactionDialog *ui;
  QString            current_faction;
  DataTree*          data_engine;
};

#endif // FACTIONDIALOG_H
