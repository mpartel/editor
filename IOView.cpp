#include "IOView.h"
#include <QtWebKit>

IOView::IOView(QWidget *parent) :
    QWebView(parent),
    m_process(new QProcess(this)),
    m_lineBuf(new TextLineBuffer(this))
{
    connect(m_lineBuf, SIGNAL(lineComplete(QString)), this, SLOT(lineComplete(QString)));
    connect(m_lineBuf, SIGNAL(partialLine(QString)), this, SLOT(lineComplete(QString)));

    this->clear();
}

void IOView::setProcess(QProcess *process)
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

void IOView::clear()
{
    page()->mainFrame()->setHtml(
                "<!doctype html>"
                "<html>"
                "    <head>"
                "       <meta charset=\"utf-8\" />"
                "       <style>body { font-family: monospace; }</style>"
                "       <title>I/O</title>"
                "    </head>"
                "    <body>"
                "      <div id=\"output\"></div>"
                "      <div id=\"end\"></div>"
                "    </body>"
                "</html>"
                );
    m_textElement = page()->mainFrame()->documentElement().findFirst("body > div#output");
    newLastLine(); // Always have at least one line
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
    newLastLine();
    page()->mainFrame()->scrollToAnchor("end");
}

void IOView::partialLine(QString line)
{
    replaceLastLineWith(line);
}

void IOView::replaceLastLineWith(QString line)
{
    m_textElement.lastChild().replace("<div></div>");
    m_textElement.lastChild().setPlainText(line);
}

void IOView::newLastLine()
{
    m_textElement.appendInside("<div></div>");
}
