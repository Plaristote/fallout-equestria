#include "tabl18n.h"
#include "ui_mainwindow.h"

TabL18n::TabL18n(QObject *parent, Ui::MainWindow* ui) : QObject(parent), ui(ui)
{
}

void TabL18n::LoadAllLanguages()
{
    QDir        dir("data/l18n");
    QStringList fileList = dir.entryList();
    QRegExp     regexp("\.json$");

    foreach (QString string, fileList)
    {
        if (!(string.contains(regexp)))
          continue ;
        QString name     = string.replace(regexp, "");
        QString filepath = dir.path() + "/" + string + ".json";

        new QListWidgetItem(name, ui->languageList);
        languages.insert(filepath, 0);
    }
}

void TabL18n::NewLanguage()
{
    QString str = QInputDialog::getText((QWidget*)parent(), "New language", "Name");

    if (str != "")
    {
        QString   filepath = "data/l18n/" + str + ".json";
        QFileInfo fileinfo(filepath);

        if (fileinfo.exists())
            QMessageBox::warning((QWidget*)parent(), "Error", "A language with that name already exists");
        else
        {
            QFile file(filepath);

            if (!(file.open(QFile::ReadWrite)))
                QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't create language file (" + filepath + ")");
            else
            {
                DataTree* newLanguage = new DataTree;

                file.close();
                languages.insert(filepath, newLanguage);
                LoadLanguage(filepath);
                new QListWidgetItem(str, ui->languageList);
            }
        }
    }
}

void TabL18n::LoadLanguage(QString filepath)
{
    DialogFiles::Iterator it  = languages.begin();
    DialogFiles::Iterator end = languages.end();

    for (; it != end ; ++it)
    {
        if (it.key() == filepath)
        {
            if (*it == 0)
            {
              *it = DataTree::Factory::JSON(filepath.toStdString());
              if (*it == 0)
              {
                QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Corrupted language file. You broke it, you fix it dude.");
                return ;
              }
            }
            ui->languageEditor->LoadLanguage(*it);
            break ;
        }
    }
}

void TabL18n::SwapLanguage(QListWidgetItem* item)
{
    QString filepath = "data/l18n/" + item->text() + ".json";

    LoadLanguage(filepath);
}

void TabL18n::RemoveLanguage()
{
    QListWidgetItem* item = ui->languageList->currentItem();

    if (item)
    {
        int     ret;
        QString message = "Are you sure you want to definitly remove '" + item->text() + "' ?";

        ret = QMessageBox::warning((QWidget*)parent(), "Remove Language", message, QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            QString path = "data/l18n/" + item->text() + ".json";
            QFile  file(path);

            if (file.remove())
            {
                ui->languageEditor->setEnabled(false);
                languages.remove(path);
                delete item;
            }
            else
              QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't remove file");
        }
    }
}
