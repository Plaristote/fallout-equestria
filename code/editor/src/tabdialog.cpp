#include "tabdialog.h"
#include "ui_mainwindow.h"
#include <iostream>

extern QString pathScriptCategories[];

TabDialog::TabDialog(QObject *parent, Ui::MainWindow* ui) : QObject(parent), ui(ui)
{
    locale= nullptr;
}

void TabDialog::LoadAllDialogs()
{
  QDir        dir("data/dialogs");
  QStringList fileList = dir.entryList();

  foreach (QString string, fileList)
  {
    QRegExp regexp("\\.json$");
    if (!(string.contains(regexp)))
      continue ;

    QString name     = string.replace(regexp, "");
    QString filepath = dir.path() + "/" + string + ".json";

    ui->dialogList->addItem(name);
    dialogs.insert(filepath, 0);
  }
  connect(ui->dialogEditor, SIGNAL(RequestScriptDisplay(QRegExp)), SLOT(MakeScriptRequest(QRegExp)));
}

void TabDialog::NewDialog()
{
    QString str = QInputDialog::getText((QWidget*)parent(), "New dialog", "Name");

    if (str != "")
    {
        QString   filepath = "data/dialogs/" + str + ".json";
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
				ui->dialogList->addItem(str);
				LoadDialog(filepath);
				ui->dialogList->setCurrentIndex( ui->dialogList->count()-1 );
            }
        }
    }
}

void TabDialog::LoadDialog(QString filepath)
{
    DialogFiles::Iterator it      = dialogs.begin();
    DialogFiles::Iterator end     = dialogs.end();

	RequestLocale();

    if (locale==nullptr) {
		QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Locale file not loaded");
        return;
	};

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
                break ;
              }
            }
            ui->dialogEditor->LoadDialog(*it,locale);
            break ;
        }
    }
}

void TabDialog::SwapDialog(QString name)
{
	if (name!="") {
        QString filepath = "data/dialogs/" + name + ".json";

		LoadDialog(filepath);
	};
}

void TabDialog::RemoveDialog()
{
	QString name= ui->dialogList->currentText();
	int		index= ui->dialogList->currentIndex();

	if (!(name.isEmpty()))
    {
        int     ret;
        QString message = "Are you sure you want to definitly remove '" + name + "' ?";

        ret = QMessageBox::warning((QWidget*)parent(), "Remove Dialog", message, QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            QString path = "data/dialogs/" + name + ".json";
            QFile  file(path);

            if (file.remove())
            {
                ui->dialogEditor->setEnabled(false);
                dialogs.remove(path);
				ui->dialogList->removeItem(index);
            }
            else
              QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't remove file");
        }
    }
}

void TabDialog::MakeScriptRequest(QRegExp prototype)
{
  RequestScript("scripts/dialogs/" + ui->dialogList->currentText() + ".as", prototype);
}
