#ifndef DIALOGEDITOR_H
#define DIALOGEDITOR_H

#include <QWidget>
#include "datatree.hpp"
#include <QTreeWidgetItem>
#include <QPlainTextEdit>
#include <QStyledItemDelegate>

namespace Ui {
class DialogEditor;
}

//A special override delegate, which allows to set the column of a QTreeWidgetItem to be non-editable, independent of the other columns
class NoEditDelegate: public QStyledItemDelegate {
    public:
      NoEditDelegate(QObject* parent=0): QStyledItemDelegate(parent) {}
      virtual QWidget* createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const {
        return 0;
      }
};


class DialogEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit DialogEditor(QWidget *parent = 0);
    ~DialogEditor();

    void LoadDialog(DataTree* tree, DataTree* locale);

    void AddNode(QString);
	void AddSuccessor(std::string node, std::string key);
	std::string  GetIntTag(DataTree* tree);
    void SaveData(bool save_I18n= false);

signals:
	void UpdateLocale(void);
    void RequestScriptDisplay(QRegExp text);

private slots:
    void NewNode(void);
    void DeleteNode(void);
    void SwapNode(QString);
    void NewSuccessor(void);
    void DeleteSuccessor(void);
    void MoveUp(void);
    void MoveDown(void);
    void ChangedCurrentSuccessor(void);
    void NpcLineChanged(void);
	void NpcLineChanging(void);
	void NpcLineLocaleChanged(void);
	void DoubleclickSuccessor(QTreeWidgetItem*,int);
    void SuccessorChanged(QTreeWidgetItem*, int);

private:
    void                    MakeScriptDisplayRequest(QString prototype);

    Ui::DialogEditor*		ui;

    DataTree*				tree;
	DataTree*				locale;
    inline QString			I18n(std::string key) { return QString::fromStdString( Data(locale)[key].Value()); }

	QTextDocument*			nodeDoc;
};

#endif // DIALOGEDITOR_H
