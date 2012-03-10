#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextDocument;

class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    PythonHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    void highlightPattern(const QString &text, const QRegExp &pattern, const QTextCharFormat &format);
    void highlightText(const QString &text, const QString &separator, int state);

    QRegExp keywordPattern;
    QTextCharFormat keywordFormat;

    QRegExp stringPattern;
    QTextCharFormat stringFormat;

    QRegExp commentPattern;
    QTextCharFormat commentFormat;

    QRegExp functionPattern;
    QTextCharFormat functionFormat;

    QRegExp numberPattern;
    QTextCharFormat numberFormat;

    QRegExp textPattern;
    QTextCharFormat textFormat;

};

#endif // PYTHONHIGHLIGHTER_H
