#ifndef IOVIEW_H
#define IOVIEW_H

#include "common.h"
#include <QTextEdit>
#include <QTextCursor>
#include <QProcess>
#include "TextLineBuffer.h"

class IOView : public QTextEdit
{
    Q_OBJECT
public:
    explicit IOView(QWidget* parent = 0);

    void setProcess(QProcess* process);

    void writeStdin(QString input);

private slots:
    void outputAvailable();
    void processFinished();

    void lineComplete(QString line);
    void partialLine(QString line);

private:
    QTextCursor m_beforeLastLine;
    QProcess* m_process;
    TextLineBuffer* m_lineBuf;

    void replaceLastLineWith(QString line);
};

#endif // IOVIEW_H
