#ifndef  FORMINTERNATIONALIZATION_H
# define FORMINTERNATIONALIZATION_H

# include <QWidget>
# include "datatree.hpp"

namespace Ui {
class FormInternationalization;
}

class FormInternationalization : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormInternationalization(QWidget *parent = 0);
    ~FormInternationalization();

    void LoadLanguage(DataTree*);

private slots:
    void CurrentStringChanged(void);
    void SelectedStringChanged(QString);
    void NewString(void);
    void DeleteString(void);
    void SaveCurrentString(void);

private:
    Ui::FormInternationalization *ui;

    DataTree* currentLanguage;
    QString   lastSavedString;
    QString   lastKey;
};

#endif // FORMINTERNATIONALIZATION_H
