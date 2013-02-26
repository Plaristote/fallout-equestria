#ifndef WIZARDOBJECT_H
#define WIZARDOBJECT_H

# include <panda3d/cmath.h>
#include <QWizard>

namespace Ui {
class WizardObject;
}

class WizardObject : public QWizard
{
    Q_OBJECT
    
public:
    explicit WizardObject(QWidget *parent = 0);
    ~WizardObject();

    const QString GetName(void) const;
    const QString GetModel(void) const;
    const QString GetTexture(void) const;
    int           GetPosX(void) const;
    int           GetPosY(void) const;
    int           GetPosZ(void) const;
    const QString GetType(void) const;
    float         GetScale(void) const;

private slots:
    void SelectTexture(void);
    void SelectModel(void);

private:
    Ui::WizardObject *ui;
};

#endif // WIZARDOBJECT_H
