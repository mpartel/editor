#ifndef IOVIEW_H
#define IOVIEW_H

#include "common.h"
#include <QWebView>
#include <QProcess>
#include <QWebElement>
#include "TextLineBuffer.h"

class IOView : public QWebView
{
    Q_OBJECT
public:
    explicit IOView(QWidget *parent = 0);

    void setProcess(QProcess *process);

    void writeStdin(QString input);

    void clear();

private slots:
    void outputAvailable();
    void processFinished();

    void lineComplete(QString line);
    void partialLine(QString line);

private:
    QProcess* m_process;
    TextLineBuffer* m_lineBuf;
    QWebElement m_textElement;

    void replaceLastLineWith(QString line);
    void newLastLine();
};

#endif // IOVIEW_H
