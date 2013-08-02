#ifndef  CITYSPLASHDIALOG_H
# define CITYSPLASHDIALOG_H

# include <QDialog>
# include <QListWidget>
# include "datatree.hpp"

namespace Ui {
class CitySplashDialog;
}

class CitySplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CitySplashDialog(QWidget *parent = 0);
    ~CitySplashDialog();

public slots:
    void Load(Data);
    void Save(Data);

private slots:
    void EnableDelZones(void);
    void AddEntryZone(void);
    void AddDiscoverZone(void);
    void DelEntryZone(void);
    void DelDiscoverZone(void);
    void PickSplashscreen(void);

private:
    void AddZone(QListWidget* widget);
    void DelZone(QListWidget* widget);

private:
    Ui::CitySplashDialog *ui;
};

#endif // CITYSPLASHDIALOG_H
