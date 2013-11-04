#ifndef QANGELSCRIPTEDIT_H
# define QANGELSCRIPTEDIT_H

# include <QTextEdit>

# include <QSyntaxHighlighter>
# include <QVector>

class QAngelScriptHighlighter : public QSyntaxHighlighter
{
public:
    explicit QAngelScriptHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
    {
        QTextCharFormat format;
        QStringList     patterns;

        // KEYWORDDS
        format.setForeground(Qt::darkBlue);
        format.setFontWeight(QFont::Bold);
        patterns << "\\b[a-zA-Z0-9]+\b" << "\\bint8\\b" << "\\bint16\\b" << "\\bint\\b" << "\\bint64\\b"
                 << "\\buint\\8\\b" << "\\buint\\16\\b" << "\\buint\\b" << "\\buint64\\b" << "\\bint32\\b" << "\\buint32\\b"
                 << "\\bfloat\\b" << "\\bdouble\\b" << "\\bstring\\b" << "\\bbool\\b" << "\\bvoid\\b";
        // ENGINE TYPES
        patterns << "\\bData\\b" << "\\bWorld\\b" << "\\bCharacter\\b" << "\\bDynamicObject\\b" << "\\bSound\\b" << "\\bWorldMap\\b" << "\\bGame\\b" << "\\bRmlElement\\b" << "\\bCamera\\b" << "\\bInventory\\b" << "\\bItem\\b" << "\\bCharacterList\\b" << "\\bDoor\\b" << "\\bShelf\\b" << "\\bSpecial\\b";

        PushRules(patterns, format);
        patterns.clear();

        format.setForeground(Qt::gray);
        format.setFontWeight(QFont::Normal);
        patterns << "//[^\n]*";
        PushRules(patterns, format);
        patterns.clear();

        format.setForeground(Qt::darkGreen);
        format.setFontWeight(QFont::Cursive);
        patterns << "\"[^\"]*\"";
        PushRules(patterns, format);
        patterns.clear();

        format.setForeground(Qt::black);
        format.setFontWeight(QFont::Bold);
        patterns << "\\b[A-Za-z0-9_]+[ ]*(?=\\()";
        PushRules(patterns, format);
        patterns.clear();

        format.setForeground(Qt::darkYellow);
        format.setFontWeight(QFont::Bold);
        patterns << "\\bif\\b" << "\\belse\\b" << "\\bwhile\\b" << "\\bfor\\b";
        PushRules(patterns, format);
        patterns.clear();
    }

    void highlightBlock(const QString &text)
    {
        foreach (const HighlightRule& rule, _rules)
        {
            QRegExp regxp = rule.pattern;
            int     index = regxp.indexIn(text);

            while (index >= 0)
            {
                int length = regxp.matchedLength();
                setFormat(index, length, rule.format);
                index = regxp.indexIn(text, index + length);
            }
        }
    }

private:
    void PushRules(const QStringList& patterns, QTextCharFormat format)
    {
        HighlightRule   rule;

        foreach (const QString& pattern, patterns)
        {
            rule.pattern = QRegExp(pattern);
            rule.format  = format;
            _rules.push_back(rule);
        }
    }

    struct HighlightRule
    {
        QRegExp         pattern;
        QTextCharFormat format;
    };

    QVector<HighlightRule> _rules;
};

class QAngelScriptEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit QAngelScriptEdit(QWidget *parent = 0);

    bool                    HasChanged(void) const;
    int                     CloseDocument(void);
    const QString&          GetFilename(void) const { return (_filename); }
    void                    GoTo(QRegExp text);

signals:
    void                    RequestSave(void);

public slots:
    void                    TextLoad(QString, QString);
    void                    TextSaved(void);
    
private:
    QAngelScriptHighlighter _highlighter;
    QString                 _lastSavedState;
    QString                 _filename;
};

#endif // QANGELSCRIPTEDIT_H
