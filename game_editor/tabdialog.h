#ifndef  TABDIALOG_H
# define TABDIALOG_H

# include <panda3d/cmath.h>
# include <QObject>
# include "qangelscriptedit.h"
# include <QTreeWidgetItem>
# include <QDir>
# include <QMessageBox>
# include <QInputDialog>
# include <QListWidget>
# include "datatree.hpp"

namespace Ui
{
  class MainWindow;
}
class MainWindow;


class TabDialog : public QObject
{
    friend class MainWindow;

    Q_OBJECT
public:
    explicit TabDialog(QObject *parent, Ui::MainWindow* ui);

	void LoadLocale(DataTree* tree) { locale= tree;};
    void LoadAllDialogs(void);
    
signals:
	void RequestLocale(void);
    
private slots:
    void NewDialog(void);
    void LoadDialog(QString filepath);
    void SwapDialog(QString);
    void RemoveDialog(void);
    //void FilterDialog(QString);
    
private:
    typedef QMultiMap<QString, DataTree*>         DialogFiles;

    Ui::MainWindow*          ui;
    DialogFiles              dialogs;
	DataTree*				 locale;
};

#endif // TABDIALOG_H
