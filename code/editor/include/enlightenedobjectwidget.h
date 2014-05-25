#ifndef  ENLIGHTENED_OBJECT_WIDGET_H
# define ENLIGHTENED_OBJECT_WIDGET_H

# include "globals.hpp"
# include <QTableWidget>
# include <QSpinBox>
# include <QCheckBox>

class EnlightenedObjectsWidget : public QTableWidget
{
  Q_OBJECT
public:
  EnlightenedObjectsWidget(QWidget* parent);
  virtual ~EnlightenedObjectsWidget() {}

  void           AddEnlightenedObject(const QString& name, unsigned short priority, bool propagates);
  unsigned short GetPriority(int row);
  bool           PropagatesToChildren(int row);
  QString        Name(int row);

  void           clear();
  void           removeRow(int);

signals:
  void updatedPriority();
  void updatedPropagation();

private slots:
  void ChangeCurrentRow(int,int,int,int);
  void SetRowEnabled(int, bool);
};

#endif
