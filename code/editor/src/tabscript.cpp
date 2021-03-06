#include "tabscript.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include "selectableresource.h"

extern QString strScriptCategories[N_SCRIPT_CAT];
extern QString pathScriptCategories[N_SCRIPT_CAT];

TabScript::TabScript(QObject *parent, Ui::MainWindow* ui) : QObject(parent), ui(ui)
{
    currentEditor = 0;
}

void TabScript::FilterScript(QString string)
{
    QRegExp regexp(string);

    for (short i = 0 ; i < N_SCRIPT_CAT ; ++i)
    {
        short count = scriptCategories[i].childCount();

        for (short ii = 0 ; ii < count ; ++ii)
        {
            QTreeWidgetItem* item = scriptCategories[i].child(ii);
            QString          text = item->text(0);

            item->setHidden(!(string == "" || text.contains(regexp)));
        }
    }
}

void TabScript::LoadAllScript(void)
{
  for (short i = 0 ; i < N_SCRIPT_CAT ; ++i)
  {
    QDir        dir("scripts/" + pathScriptCategories[i]);
    QStringList fileList = dir.entryList();

    foreach (QString string, fileList)
    {
      QRegExp regexp("\\.as$");
      if (!(string.contains(regexp)))
        continue ;

      QString name     = string.replace(regexp, "");
      QString filepath = dir.path() + "/" + string + ".as";
      QTreeWidgetItem* item = new QTreeWidgetItem(&scriptCategories[i]);

      item->setText(0, name);
      files.insert(filepath, 0);
      if (strScriptCategories[i] == "Artificial Intelligence")
        SelectableResource::AIs().AddResource(name);
      else if (strScriptCategories[i] == "Dialogues")
        SelectableResource::Dialogs().AddResource(name);
      else if (strScriptCategories[i] == "Objects")
        SelectableResource::ObjectScript().AddResource(name);
    }
  }
  UpdateFilename();
}

void TabScript::RemoveScript()
{
  if (currentEditor)
  {
    int     ret;
    QString message = "Are you sure you want to definitly remove '" + currentEditor->GetFilename() + "' ?";

    ret = QMessageBox::warning((QWidget*)parent(), "Remove file", message, QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
      QFile file(currentEditor->GetFilename());

      if (file.remove())
      {
        for (short i = 0 ; i < N_SCRIPT_CAT ; ++i)
        {
          short count = scriptCategories[i].childCount();

          for (short ii = 0 ; ii < count ; ++ii)
          {
            QTreeWidgetItem* item = scriptCategories[i].child(ii);
            QString          name = "scripts/" + pathScriptCategories[i] + "/"    + item->text(0) + ".as";

            if (name == currentEditor->GetFilename())
            {
               if (strScriptCategories[i] == "Artificial Intelligence")
                 SelectableResource::AIs().DelResource(item->text(0));
               else if (strScriptCategories[i] == "Dialogues")
                 SelectableResource::Dialogs().DelResource(item->text(0));
               else if (strScriptCategories[i] == "Objects")
                 SelectableResource::ObjectScript().DelResource(item->text(0));
               scriptCategories[i].removeChild(item);
               break ;
            }
          }
        }
        CloseScript(true);
      }
      else
        QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't remove file");
    }
  }
}

void TabScript::NewScript(void)
{
    int       type     = dialogNewScript.GetType();
    QString   name     = dialogNewScript.GetName();
    QString   filepath = "scripts/" + pathScriptCategories[type] + "/" + name + ".as";
    QFileInfo fileinfo(filepath);
    QFile     file(filepath);

    if (fileinfo.exists())
    {
        QMessageBox::warning((QWidget*)parent(), "Can't create file", "File already exists");
        dialogNewScript.open();
    }
    else
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(&scriptCategories[type]);

        item->setText(0, name);
        file.open(QFile::ReadOnly);
        file.close();
        files.insert(filepath, 0);
        LoadScript(filepath);
        dialogNewScript.Clear();
        if (strScriptCategories[type] == "Artificial Intelligence")
          SelectableResource::AIs().AddResource(name);
        else if (strScriptCategories[type] == "Dialogues")
          SelectableResource::Dialogs().AddResource(name);
        else if (strScriptCategories[type] == "Objects")
          SelectableResource::ObjectScript().AddResource(name);
    }
}

void TabScript::GoTo(QString filepath, QRegExp regexp)
{
  LoadScript(filepath);
  if (currentEditor && currentEditor->GetFilename() == filepath)
  {
    ui->tabWidget->setCurrentWidget(ui->tabScriptEditor);
    currentEditor->GoTo(regexp);
  }
  else
    QMessageBox::warning((QWidget*)parent(), "I'm afraid I cannot let you do that, Steve.", filepath + " does not exists.");
}

void TabScript::LoadScript(QString filepath)
{
    QFile                 file(filepath);
    ScriptFiles::iterator it  = files.begin();
    ScriptFiles::iterator end = files.end();
    QString               loadedText;

    for (; it != end ; ++it)
    {
        if ((it.key()) == filepath)
        {
            if (currentEditor)
            {
                currentEditor->hide();
                currentEditorHistory << currentEditor;
            }
            currentEditor = *it;
            if (currentEditor == 0)
            {
                ui->scriptBaseEdit->hide();
                currentEditor = new QAngelScriptEdit(ui->editContainer);
                connect(currentEditor, SIGNAL(RequestSave()), this, SLOT(SaveScript()));
                connect(currentEditor, SIGNAL(textChanged()), this, SLOT(RefreshSaveButton()));
                *it           = currentEditor;
                ui->editContainer->layout()->addWidget(currentEditor);

                if (file.open(QFile::ReadWrite))
                {
                    QTextStream           stream(&file);

                    while (!(stream.atEnd()))
                        loadedText += stream.readLine() + "\n";
                    currentEditor->TextLoad(filepath, loadedText);
                    file.close();
                }
                else
                    QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Can't open file '" + filepath + "'");
           }
           currentEditor->show();
           break ;
       }
    }
    currentEditorHistory.removeAll(currentEditor);
    currentEditorHistoryNext.removeAll(currentEditor);
    UpdateFilename();
}

void TabScript::SwapScript(QTreeWidgetItem* first)
{
    if (first)
    {
        for (short i = 0 ; i < N_SCRIPT_CAT ; ++i)
        {
            if (first->parent() == &scriptCategories[i])
            {
                QString filepath = "scripts/" + pathScriptCategories[i] + "/"    + first->text(0) + ".as";

                LoadScript(filepath);
                break ;
            }
        }
        UpdateFilename();
    }
}

void TabScript::RefreshSaveButton(void)
{
  ui->scriptSave->setEnabled(currentEditor && currentEditor->HasChanged());
}

void TabScript::RefreshHistoryButtons(void)
{
  ui->scriptPrevious->setEnabled(currentEditor && (currentEditorHistory.size()     != 0));
  ui->scriptNext->setEnabled    (currentEditor && (currentEditorHistoryNext.size() != 0));
}

void TabScript::CloseScript(bool forced)
{
    if (currentEditor)
    {
        ScriptFiles::iterator it  = files.begin();
        ScriptFiles::iterator end = files.end();

        if (!forced)
        {
            int doClose = currentEditor->CloseDocument();

            if (doClose == QMessageBox::Abort)
              return ;
            if (doClose == QMessageBox::Yes)
              SaveScript();
        }

        for (; it != end ; ++it)
        {
            if (*it == currentEditor)
            {
                *it = 0;
                delete currentEditor;
                if (currentEditorHistory.size())
                {
                  currentEditor = currentEditorHistory.last();
                  currentEditor->show();
                  currentEditorHistory.erase(--(currentEditorHistory.end()));
                }
                else
                {
                  currentEditor = 0;
                  ui->scriptBaseEdit->show();
                }
                break ;
            }
        }
    }
    UpdateFilename();
}

void TabScript::PreviousScript()
{
    if (!(currentEditorHistory.size()))
      return ;
    QAngelScriptEdit* last = currentEditorHistory.last();

    if (currentEditor && last)
    {
        currentEditor->hide();
        last->show();
        currentEditorHistoryNext.prepend(currentEditor);
        currentEditor = last;
        currentEditorHistory.erase(--(currentEditorHistory.end()));
    }
    UpdateFilename();
}

void TabScript::NextScript()
{
    if (!(currentEditorHistoryNext.size()))
      return ;
    QAngelScriptEdit* last = *(currentEditorHistoryNext.begin());

    if (currentEditor && last)
    {
        currentEditor->hide();
        last->show();
        currentEditorHistory << currentEditor;
        currentEditor = last;
        currentEditorHistoryNext.erase(currentEditorHistoryNext.begin());
    }
    UpdateFilename();
}

void TabScript::UpdateFilename()
{
    if (currentEditor)
    {
        ScriptFiles::iterator it  = files.begin();
        ScriptFiles::iterator end = files.end();

        for (; it != end ; ++it)
        {
            if (*it == currentEditor)
            {
                QString name = it.key();

                if (currentEditor->HasChanged())
                  name += " (has unsaved changes)";
                ui->labelFilename->setText(name);
                break ;
            }
        }
    }
    else
        ui->labelFilename->setText("");
    RefreshSaveButton();
    RefreshHistoryButtons();
}

void TabScript::SaveScript()
{
    if (currentEditor)
    {
        ScriptFiles::iterator it  = files.begin();
        ScriptFiles::iterator end = files.end();

        for (; it != end ; ++it)
        {
            if (*it == currentEditor)
            {
                QFile file(it.key());

                if (file.open(QIODevice::WriteOnly))
                {
                    currentEditor->TextSaved();
                    UpdateFilename();
                    file.write(currentEditor->toPlainText().toStdString().c_str());
                    file.close();
                }
                else
                    QMessageBox::warning((QWidget*)parent(), "Fatal Error", "Failed to save the current file");
                break ;
            }
        }
    }
}
