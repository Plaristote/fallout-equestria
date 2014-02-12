#ifndef TABL18N_H
#define TABL18N_H

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

class TabL18n : public QObject
{
    friend class MainWindow;

    Q_OBJECT
public:
    explicit TabL18n(QObject *parent, Ui::MainWindow* ui);
    void LoadAllLanguages(void);
	DataTree* GetDefaultLanguage(void);

private slots:
    void NewLanguage(void);
    void LoadLanguage(QString filepath);
    void SwapLanguage(QListWidgetItem*);
    void RemoveLanguage(void);

private:
    typedef QMultiMap<QString, DataTree*>         DialogFiles;

    Ui::MainWindow*          ui;
    DialogFiles              languages;
	DataTree*				 lang_default;
};

#endif // TABL18N_H
