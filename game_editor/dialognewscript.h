#ifndef DIALOGNEWSCRIPT_H
#define DIALOGNEWSCRIPT_H

#include <QDialog>

namespace Ui {
class DialogNewScript;
}

class DialogNewScript : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogNewScript(QWidget *parent = 0);
    ~DialogNewScript();

    int     GetType(void)     const;
    QString GetName(void)     const;
    void    Clear(void);
    void    UpdateLevelList(QStringList levels);

private slots:
    void    UpdatedType(void);

private:
    Ui::DialogNewScript *ui;
};

#endif // DIALOGNEWSCRIPT_H
