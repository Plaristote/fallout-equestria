#ifndef DIALOGSAVEMAP_H
#define DIALOGSAVEMAP_H

#include <QDialog>

namespace Ui {
class DialogSaveMap;
}

class DialogSaveMap : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSaveMap(QWidget *parent = 0);
    ~DialogSaveMap();

    bool DoCompileWaypoints(void);
    bool DoCompileDoors(void);
    
private:
    Ui::DialogSaveMap *ui;
};

#endif // DIALOGSAVEMAP_H
