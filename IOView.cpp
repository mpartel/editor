#include "IOView.h"

IOView::IOView(QWidget* parent) :
    QTextEdit(parent),
    m_process(new QProcess(this)),
    m_lineBuf(new TextLineBuffer(this))
{
    m_beforeLastLine = QTextCursor(document());

    connect(m_lineBuf, SIGNAL(lineComplete(QString)), this, SLOT(lineComplete(QString)));
    connect(m_lineBuf, SIGNAL(partialLine(QString)), this, SLOT(lineComplete(QString)));
}

void IOView::setProcess(QProcess* process)
{
    if (m_process->parent() == this) {
        delete m_process;
    }
    m_process = process;
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(outputAvailable()));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished()));
}

void IOView::writeStdin(QString input)
{
    //TODO
    Q_UNUSED(input);
}

void IOView::outputAvailable()
{
    m_lineBuf->readFrom(m_process);
}

void IOView::processFinished()
{
    m_lineBuf->readFrom(m_process);
    m_lineBuf->discardBufferedLine();
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
