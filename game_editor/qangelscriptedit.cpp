#include "qangelscriptedit.h"
#include <QMessageBox>

QAngelScriptEdit::QAngelScriptEdit(QWidget *parent) : QTextEdit(parent), _highlighter(this->document())
{
    QFont font;

    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
}

int QAngelScriptEdit::CloseDocument()
{
    if (this->toPlainText() != _lastSavedState)
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
