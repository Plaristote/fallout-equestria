#include "qangelscriptedit.h"
#include <QMessageBox>
#include <QShortcut>

QAngelScriptEdit::QAngelScriptEdit(QWidget *parent) : QTextEdit(parent), _highlighter(this->document())
{
    QFont font;

    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
    _save_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SIGNAL(RequestSave()));
}

QAngelScriptEdit::~QAngelScriptEdit()
{
    delete _save_shortcut;
}

bool QAngelScriptEdit::HasChanged() const
{
    return (this->toPlainText() != _lastSavedState);
}

int QAngelScriptEdit::CloseDocument()
{
    if (HasChanged())
    {
      int ret;
      QString warningMsg = "Do you want to save the changes on '" + _filename + "'";

      ret = QMessageBox::warning(this, "Changes unsaved", warningMsg, QMessageBox::Yes, QMessageBox::No, QMessageBox::Abort);
      return (ret);
    }
    return (QMessageBox::No);
}

void QAngelScriptEdit::TextSaved()
{
    _lastSavedState = this->toPlainText();
}

void QAngelScriptEdit::TextLoad(QString filepath, QString text)
{
    _filename       = filepath;
    _lastSavedState = text;
    this->setPlainText(text);
}

void QAngelScriptEdit::GoTo(QRegExp text)
{
    QString     content  = this->toPlainText();
    int         position = content.indexOf(text);
    QTextCursor cursor   = this->textCursor();

    cursor.setPosition(position);
    this->setTextCursor(cursor);
    this->ensureCursorVisible();
}
