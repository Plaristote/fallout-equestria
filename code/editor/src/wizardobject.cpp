#include "wizardobject.h"
#include "ui_wizardobject.h"
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "heightmapdialog.h"

extern QString objectTypes[];

WizardObject::WizardObject(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardObject)
{
    ui->setupUi(this);

    connect(ui->pickModel,       SIGNAL(clicked()), this, SLOT(SelectModel()));
    connect(ui->pickTexture,     SIGNAL(clicked()), this, SLOT(SelectTexture()));
    connect(ui->importHeightmap, SIGNAL(clicked()), this, SLOT(ImportHeightmap()));
}

WizardObject::~WizardObject()
{
    delete ui;
}

const QString WizardObject::GetName() const
{
    return (ui->name->text());
}

const QString WizardObject::GetModel(void) const
{
    return (ui->model->text());
}

const QString WizardObject::GetTexture(void) const
{
    return (ui->texture->text());
}

int            WizardObject::GetPosX(void) const
{
    return (ui->posx->value());
}

int            WizardObject::GetPosY(void) const
{
    return (ui->posy->value());
}

int           WizardObject::GetPosZ(void) const
{
    return (ui->posz->value());
}

float         WizardObject::GetScale(void) const
{
    return (ui->scale->value());
}

const QString WizardObject::GetType(void) const
{
    return ("ui->type->currentText()");
}

void WizardObject::ImportHeightmap(void)
{
  HeightmapDialog* heightmap_dialog = new HeightmapDialog(this);

  connect(heightmap_dialog, SIGNAL(heightmapCreated(QString)), this, SLOT(ImportedHeightmap(QString)));
  heightmap_dialog->open();
}

void WizardObject::ImportedHeightmap(QString filename)
{
  ui->model->setText(filename);
}

void WizardObject::SelectModel(void)
{
    QString filter    = "Panda3D Models (*.egg *.bam *.egg.pz *.bam.pz *.obj)";
    QString base_path = QDir::currentPath() + "/models/";
    QString path      = QFileDialog::getOpenFileName(this, "Select a model", base_path, filter);
    QFileInfo info(path);
    QString   relative_path;

    if (!(info.exists()))
      return ;
    if (!(path.startsWith(base_path))) // Needs to be moved
    {
      if (!(QFile::copy(path, base_path + info.fileName())))
      {
        QMessageBox::warning(this, "Error", "Couldn't copy file to the project directory.");
        return ;
      }
      path = base_path + info.fileName();
    }
    relative_path = path.remove(0, base_path.length());
    ui->model->setText(relative_path);
}

void WizardObject::SelectTexture(void)
{
    QString   filter    = "Images (*.png *.jpg *.bmp)";
    QString   base_path = QDir::currentPath() + "/textures/";
    QString   path = QFileDialog::getOpenFileName(this, "Select a texture", base_path, filter);
    QFileInfo info(path);
    QString   relative_path;

    if (!(info.exists()))
      return ;
    if (!(path.startsWith(base_path))) // Needs to be moved
    {
      if (!(QFile::copy(path, base_path + info.fileName())))
      {
        QMessageBox::warning(this, "Error", "Couldn't copy file to the project directory.");
        return ;
      }
      path = base_path + info.fileName();
    }
    relative_path = path.remove(0, base_path.length());
    ui->texture->setText(relative_path);
}

