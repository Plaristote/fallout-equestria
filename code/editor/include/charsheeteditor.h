#ifndef CHARSHEETEDITOR_H
# define CHARSHEETEDITOR_H

# include <QWidget>
# include "datatree.hpp"
# include <QSpinBox>
# include "formkeyvalue.h"

namespace Ui {
class CharsheetEditor;
}

class CharsheetEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CharsheetEditor(QWidget *parent = 0);
    ~CharsheetEditor();

public slots:
    void New(QString name);
    void Delete(void);
    void Load(QString name);
    void Save(void);
    void Compile(void);

private slots:
    void UpdateDataTree(void);

private:
    Ui::CharsheetEditor *ui;

    Data Charsheet(void) { return (Data(charsheet)); }

    QString   name;
    DataTree* charsheet;

    typedef QPair<QString, QSpinBox*> SpecialWidget;
    QList<SpecialWidget> special_widgets;

    typedef FormKeyValue* StatWidget;
    QList<StatWidget> statistics_widgets;
    QList<StatWidget> skills_widgets;
};

#endif // CHARSHEETEDITOR_H
