#ifndef  TABSCRIPT_H
# define TABSCRIPT_H

# include <panda3d/cmath.h>
# include <QObject>
# include "qangelscriptedit.h"
# include <QTreeWidgetItem>
# include <QDir>
# include <QMessageBox>
# include <QInputDialog>
# include "dialognewscript.h"

# define N_SCRIPT_CAT 7

namespace Ui
{
  class MainWindow;
}
class MainWindow;

class TabScript : public QObject
{
    friend class MainWindow;

    Q_OBJECT
public:
    explicit TabScript(QObject *parent, Ui::MainWindow* ui);

    void LoadAllScript(void);

    DialogNewScript          dialogNewScript;
signals:
    
private slots:
    void NewScript(void);
    void LoadScript(QString filepath);
    void CloseScript(bool forced = false);
    void PreviousScript(void);
    void NextScript(void);
    void SaveScript(void);
    void RemoveScript(void);
    void FilterScript(QString);
    void SwapScript(QTreeWidgetItem*);
    void UpdateFilename(void);

private:
    typedef QMultiMap<QString, QAngelScriptEdit*> ScriptFiles;

    Ui::MainWindow*          ui;

    QTreeWidgetItem          scriptCategories[N_SCRIPT_CAT];
    ScriptFiles              files;
    QAngelScriptEdit*        currentEditor;
    QList<QAngelScriptEdit*> currentEditorHistory;
    QList<QAngelScriptEdit*> currentEditorHistoryNext;
};

#endif // TABSCRIPT_H
