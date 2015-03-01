#ifndef HEIGHTMAPDIALOG_H
#define HEIGHTMAPDIALOG_H

#include <QDialog>

namespace Ui {
  class HeightmapDialog;
}

class HeightmapDialog : public QDialog
{
  Q_OBJECT

public:
  explicit HeightmapDialog(QWidget *parent = 0);
  ~HeightmapDialog();

  QString outputFilename() const;
  QString inputFilename() const;
  QString filename() const;

signals:
  void heightmapCreated(QString);

private slots:
  void pickHeightmapFile();
  void compile();

private:
  void generateInputFile() const;

  Ui::HeightmapDialog *ui;
};

#endif // HEIGHTMAPDIALOG_H
