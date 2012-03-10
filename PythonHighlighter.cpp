#include "PythonHighlighter.h"
#include <QDebug>

PythonHighlighter::PythonHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    keywordPattern = QRegExp("\\b(False|None|True|and|as|assert|break|class|continue|def|del|elif|else|except|finally|for|from|global|if|import|in|is|lambda|nonlocal|not|or|pass|raise|return|try|while|with|yield)\\b");
    keywordFormat.setForeground(QColor(228, 109, 62));

    stringPattern = QRegExp("\"[^\"]*(\\\\\"[^\"]*)*\"|'[^']*(\\\\'[^']*)*'");
    stringFormat.setForeground(QColor(69, 176, 60));

    commentPattern = QRegExp("#.*");
    commentFormat.setForeground(QColor(229, 0, 0));

    functionPattern = QRegExp("\\b[a-zA-z_][a-zA-z0-9_]*(?=\\()");
    functionFormat.setForeground(QColor(63, 64, 139));

    numberPattern = QRegExp("\\b([0-9]+|[0-9]*\\.[0-9]+)\\b");
    numberFormat.setForeground(QColor(63, 64, 139));

    textPattern = QRegExp("\"\"\"");
    textFormat.setForeground(QColor(69, 176, 60));
}

void PythonHighlighter::highlightPattern(const QString &text, const QRegExp &pattern, const QTextCharFormat &newFormat)
{
    QRegExp expression(pattern);
    int index = expression.indexIn(text);
    while (index >= 0) {
        int length = expression.matchedLength();
        if (format(index).foreground().color() == Qt::black) {
            setFormat(index, length, newFormat);
        }
        index = expression.indexIn(text, index + length);
    }
}

void PythonHighlighter::highlightText(const QString &text, const QString &separator, int state)
{
    int startIndex = 0;

    if (previousBlockState() != state) {
        startIndex = text.indexOf(separator);
    }

    while (startIndex >= 0) {
        int endIndex = text.indexOf(separator, startIndex);
        if (startIndex == endIndex && previousBlockState() != state) {
            endIndex = text.indexOf(separator, startIndex + separator.length());
        }
        int textLength;
        if (endIndex == -1) {
            setCurrentBlockState(state);
            textLength = text.length() - startIndex;
        } else {
            textLength = endIndex - startIndex + separator.length();
        }
        setFormat(startIndex, textLength, textFormat);
        startIndex = text.indexOf(separator, startIndex + textLength);
    }
}

void PythonHighlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(0);
    highlightText(text, "\"\"\"", 1);
    highlightText(text, "'''", 2);

    highlightPattern(text, numberPattern, numberFormat);
    highlightPattern(text, functionPattern, functionFormat);
    highlightPattern(text, keywordPattern, keywordFormat);
    highlightPattern(text, stringPattern, stringFormat);
    highlightPattern(text, commentPattern, commentFormat);
}
