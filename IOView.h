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

    void clear();

public slots:
    void writeStdin(QString input);

private slots:
    void outputAvailable();
    void processFinished();

    void lineComplete(QString line);
    void partialLine(QString line);

private:
    QProcess* m_process;
    TextLineBuffer* m_lineBuf;
    QWebElement m_textElement;
    QWebElement m_inputPrompt;

    void replaceLastLineWith(QString line);
    void newLastLine();
};

#endif // IOVIEW_H
