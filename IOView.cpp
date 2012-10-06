#include "IOView.h"
#include <QtWebKit>
#include <QFile>

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

void IOView::clear()
{
    QFile htmlFile(":/IOView_page.html");
    htmlFile.open(QIODevice::ReadOnly);
    QString html = htmlFile.readAll();
    page()->mainFrame()->setHtml(html);

    m_textElement = page()->mainFrame()->documentElement().findFirst("body > div#output");
    m_inputPrompt = page()->mainFrame()->documentElement().findFirst("#prompt");
    page()->mainFrame()->addToJavaScriptWindowObject("IOView", this);
    newLastLine(); // Always have at least one line
}

void IOView::writeStdin(QString input)
{
    qDebug() << "writeStdin()" << input;

    QWebElement lineDiv = m_textElement.lastChild();
    lineDiv.appendInside("<span></span>");

    QWebElement span = lineDiv.lastChild();
    span.addClass("input");
    span.setPlainText(input);

    newLastLine();
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
    QWebElement newDiv = m_textElement.lastChild();
    newDiv.setPlainText(line);
    newDiv.appendInside(m_inputPrompt);
qDebug() << page()->mainFrame()->toHtml();
}

void IOView::newLastLine()
{
    m_textElement.appendInside("<div></div>");
    replaceLastLineWith("");
}
