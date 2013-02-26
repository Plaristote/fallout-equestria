#ifndef  FORMKEYVALUE_H
# define FORMKEYVALUE_H

# include <QWidget>
# include "datatree.hpp"

namespace Ui
{
class FormKeyValue;
}

class FormKeyValue : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormKeyValue(QWidget *parent = 0);
    ~FormKeyValue();

    void FromData(Data data);
    void SetKey(QString);
    void SetValue(QString);

    QString GetKey(void)   const;
    QString GetValue(void) const;

signals:
    void ValueChanged(QString);
    
private:
    Ui::FormKeyValue *ui;
};

#endif // FORMKEYVALUE_H
