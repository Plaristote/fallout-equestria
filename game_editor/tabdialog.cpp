#include "tabdialog.h"
#include "ui_mainwindow.h"

extern QString pathScriptCategories[];

TabDialog::TabDialog(QObject *parent, Ui::MainWindow* ui) : QObject(parent), ui(ui)
{
}

void TabDialog::LoadAllDialogs()
{
    QDir        dir("data/" + pathScriptCategories[0]);
    QStringList fileList = dir.entryList();

    foreach (QString string, fileList)
    {
        QRegExp regexp("\.json$");
        if (!(string.contains(regexp)))
          continue ;

        QString name     = string.replace(regexp, "");
        QString filepath = dir.path() + "/" + string + ".json";

        new QListWidgetItem(name, ui->dialogList);
        dialogs.insert(filepath, 0);
    }
}

void TabDialog::NewDialog()
{
    QString str = QInputDialog::getText((QWidget*)parent(), "New dialog", "Name");

    if (str != "")
    {
        QString   filepath = "data/" + pathScriptCategories[0] + "/" + str + ".json";
        QFileInfo fileinfo(filepath);

        if (fileinfo.exists())
            QMessageBox::warning((QWidget*)parent(), "Error", "A dialog with that name already exists");
        else
        {
            QFile file(filepath);

            if (!(file.open(QFile::ReadWrite)))
                QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't create dialog file (" + filepath + ")");
            else
            {
                DataTree* newDialog = new DataTree;

                file.close();
                dialogs.insert(filepath, newDialog);
                LoadDialog(filepath);
                new QListWidgetItem(str, ui->dialogList);
            }
        }
    }
}

void TabDialog::LoadDialog(QString filepath)
{
    DialogFiles::Iterator it  = dialogs.begin();
    DialogFiles::Iterator end = dialogs.end();

    for (; it != end ; ++it)
    {
        if (it.key() == filepath)
        {
            if (*it == 0)
            {
              *it = DataTree::Factory::JSON(filepath.toStdString());
              if (*it == 0)
              {
                QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Corrupted dialog file. That's what you get for sticking your dirty hands on it");
                return ;
              }
            }
            ui->dialogEditor->LoadDialog(*it);
            break ;
        }
    }
}

void TabDialog::SwapDialog(QListWidgetItem* item)
{
    QString filepath = "data/" + pathScriptCategories[0] + "/" + item->text() + ".json";

    LoadDialog(filepath);
}

void TabDialog::RemoveDialog()
{
    QListWidgetItem* item = ui->dialogList->currentItem();

    if (item)
    {
        int     ret;
        QString message = "Are you sure you want to definitly remove '" + item->text() + "' ?";

        ret = QMessageBox::warning((QWidget*)parent(), "Remove Dialog", message, QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            QString path = "data/dialogs/" + item->text() + ".json";
            QFile  file(path);

            if (file.remove())
            {
                ui->dialogEditor->setEnabled(false);
                dialogs.remove(path);
                delete item;
            }
            else
              QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't remove file");
        }
    }
}

void TabDialog::FilterDialog(QString string)
{
    QRegExp regexp(string);

    short count = ui->dialogList->count();

    for (short i = 0 ; i < count ; ++i)
    {
        QListWidgetItem* item = ui->dialogList->item(i);

        if (string == "")
          item->setHidden(false);
        else
        {
            if (item->text().contains(regexp))
                item->setHidden(false);
            else
                item->setHidden(true);
        }
    }
}
