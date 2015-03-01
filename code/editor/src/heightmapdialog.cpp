#include "heightmapdialog.h"
#include "ui_heightmapdialog.h"
#include "datatree.hpp"
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

HeightmapDialog::HeightmapDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HeightmapDialog)
{
  ui->setupUi(this);
  connect(this, SIGNAL(accepted()), this, SLOT(compile()));
  connect(ui->pickHeightmapFile, SIGNAL(clicked()), this, SLOT(pickHeightmapFile()));
}

HeightmapDialog::~HeightmapDialog()
{
  delete ui;
}

void HeightmapDialog::pickHeightmapFile()
{
  QString   filter    = "Images (*.png *.jpg *.bmp)";
  QString   base_path = QDir::currentPath();
  QString   path      = QFileDialog::getOpenFileName(this, "Select a heightmap", base_path, filter);
  QFileInfo info(path);
  QDir      current_dir(base_path);

  if (info.exists())
    ui->heightmapFile->setText(current_dir.relativeFilePath(path));
}

QString HeightmapDialog::filename() const
{
  QFileInfo info(ui->heightmapFile->text());

  return info.fileName();
}

QString HeightmapDialog::outputFilename() const
{
  return "./models/" + filename() + ".bam";
}

QString HeightmapDialog::inputFilename() const
{
  return "./" + filename() + ".json";
}

void HeightmapDialog::generateInputFile() const
{
  DataTree datatree;
  {
    Data    data(&datatree);
    QString heightmapPath = ui->heightmapFile->text();

    data["heightmap"]  = heightmapPath.toStdString();
    data["block_size"] = ui->blockSize->value();
    data["factor"]     = ui->factor->value();
    data["min_level"]  = ui->minimumLevel->value();
    data["max_height"] = ui->maximumLevel->value();
    DataTree::Writers::JSON(data, inputFilename().toStdString());
  }
}

void HeightmapDialog::compile()
{
  QString     working_directory = QDir::currentPath();
  QProcess    process(this);
  QStringList args;

  generateInputFile();
  args << "--compile-heightmap" << inputFilename() << outputFilename();
  process.setWorkingDirectory(working_directory);
#ifdef _WIN32
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("Path", "..\\Panda3D-1.9.0\\bin;" + env.value("Path"));
    process.setProcessEnvironment(env);
    process.start("./game.exe", args);
#else
    process.start("./game", args);
#endif
    if (process.waitForFinished())
    {
      if (process.exitCode() == 0)
        emit heightmapCreated(outputFilename());
      else
        QMessageBox::warning(this, "Error", "The heightmap compiling failed.");
    }
    else
      QMessageBox::warning(this, "Error", "The heightmap compiling failed to start.");

}
