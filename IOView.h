#ifndef IOVIEW_H
#define IOVIEW_H

#include <QTextEdit>
#include <QTextCursor>
#include <QIODevice>
#include "TextLineBuffer.h"

class IOView : public QTextEdit
{
    Q_OBJECT
public:
    explicit IOView(QWidget* parent = 0);

    void setIODevice(QIODevice* device);

    void writeStdin(QString input);

private slots:
    void outputAvailable();
    void lineComplete(QString line);
    void partialLine(QString line);

private:
    QTextCursor m_beforeLastLine;
    QIODevice* m_device;
    TextLineBuffer* m_lineBuf;

    void replaceLastLineWith(QString line);
};

#endif // IOVIEW_H
