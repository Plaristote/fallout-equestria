#ifndef DIALOGSPLASHSCREEN_H
#define DIALOGSPLASHSCREEN_H

#include <QDialog>

namespace Ui {
class DialogSplashscreen;
}

class DialogSplashscreen : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSplashscreen(QWidget *parent = 0);
    ~DialogSplashscreen();

    const QString GetProjectPath(void) const;
    
private slots:
    void PickFolder(void);
    void PickHistory(QString);
    void FolderChanged(QString);
    void Accepted(void);
    void Abort(void);

private:
    Ui::DialogSplashscreen *ui;
};

#endif // DIALOGSPLASHSCREEN_H
