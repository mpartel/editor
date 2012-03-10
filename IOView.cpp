#include "IOView.h"

IOView::IOView(QWidget* parent) :
    QTextEdit(parent),
    m_device(0),
    m_lineBuf(new TextLineBuffer(this))
{
    m_beforeLastLine = QTextCursor(document());

    connect(m_lineBuf, SIGNAL(lineComplete(QString)), this, SLOT(lineComplete(QString)));
    connect(m_lineBuf, SIGNAL(partialLine(QString)), this, SLOT(lineComplete(QString)));
}

void IOView::setIODevice(QIODevice* device)
{
    m_device = device;
    connect(m_device, SIGNAL(readyRead()), this, SLOT(outputAvailable()));
}

void IOView::writeStdin(QString input)
{
    //TODO
    Q_UNUSED(input);
}

void IOView::outputAvailable()
{
    m_lineBuf->readFrom(m_device);
}

void IOView::lineComplete(QString line)
{
    replaceLastLineWith(line);
    m_beforeLastLine.movePosition(QTextCursor::End);
    m_beforeLastLine.insertText("\n");
    ensureCursorVisible();
}

void IOView::partialLine(QString line)
{
    replaceLastLineWith(line);
}

void IOView::replaceLastLineWith(QString line)
{
    QTextCursor cursor(m_beforeLastLine);

    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(line);
}
