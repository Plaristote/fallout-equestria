#include "dialogsplashscreen.h"
#include "ui_dialogsplashscreen.h"
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include "qpandaapplication.h"

DialogSplashscreen::DialogSplashscreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSplashscreen)
{
    QSettings settings;
    int       nRecents = settings.beginReadArray("RecentProjects");

    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
    for (int i = 0 ; i < nRecents ; ++i)
    {
        QString toPrint;
        QDir    directory;

        settings.setArrayIndex(i);
        toPrint = settings.value("path").toString();
        directory.setPath(toPrint);
        if (directory.exists())
          ui->history->addItem(toPrint);
    }
    settings.endArray();
    connect(ui->folderPicker, SIGNAL(clicked()),            this, SLOT(PickFolder()));
    connect(ui->path,         SIGNAL(textChanged(QString)), this, SLOT(FolderChanged(QString)));
    connect(ui->history,      SIGNAL(currentTextChanged(QString)), this, SLOT(PickHistory(QString)));
    connect(ui->buttonBox,    SIGNAL(accepted()),           this, SLOT(Accepted()));
    connect(ui->buttonBox,    SIGNAL(rejected()),           this, SLOT(Abort()));
}

DialogSplashscreen::~DialogSplashscreen()
{
    delete ui;
}

void DialogSplashscreen::Abort()
{
    QPandaApplication::Close();
}

void DialogSplashscreen::FolderChanged(QString path)
{
    QDir dir(path);

    if (dir.exists())
      ui->buttonBox->setEnabled(true);
    else
      ui->buttonBox->setEnabled(false);
}

void DialogSplashscreen::PickFolder()
{
  const QString folderpath = QFileDialog::getExistingDirectory(this, "Dossier du projet");

  ui->path->setText(folderpath);
}

void DialogSplashscreen::PickHistory(QString item)
{
  ui->path->setText(item);
}

const QString DialogSplashscreen::GetProjectPath(void) const
{
    return (ui->path->text());
}

void DialogSplashscreen::Accepted()
{
    QSettings   settings;
    QStringList recents;
    int         nRecents = settings.beginReadArray("RecentProjects");
    QString     path     = ui->path->text();

    if (path == "")
      return ;
    recents << path;
    for (int i = 0 ; i < nRecents ; ++i)
    {
        QString recent;

        settings.setArrayIndex(i);
        recent = settings.value("path").toString();
        if (path != recent)
          recents << recent;
    }
    settings.endArray();
    settings.remove("RecentProjects");
    settings.beginWriteArray("RecentProjects");
    for (int i = 0 ; i < recents.size() && i < 10 ; ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recents.at(i));
    }
    settings.endArray();
}
