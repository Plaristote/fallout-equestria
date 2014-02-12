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

    void                     LoadAllScript(void);

    DialogNewScript          dialogNewScript;

public slots:
    void GoTo(QString file, QRegExp regexp);

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

    // TODO Rename this method. It nows update the whole contextual interface.
    void UpdateFilename(void);
    // Dis/enable the save button based on whether the current editor can be saved
    void                     RefreshSaveButton(void);
    // Dis/enable previous and next buttons
    void                     RefreshHistoryButtons(void);

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
