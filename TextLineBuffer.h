#ifndef TEXTLINEBUFFER_H
#define TEXTLINEBUFFER_H

#include <QObject>
#include <QString>
#include <QIODevice>

/** Buffers incoming bytes until a complete line is formed.
  *
  * Then signals that a line is complete and begins buffering the next line.
  * When there are only enough bytes available for a partial line, emits
  * the partial line signal. The purpose of all this is to avoid a chunk of input
  * ending inside a multibyte character. */
class TextLineBuffer : public QObject
{
    Q_OBJECT
public:
    TextLineBuffer(QObject* parent = 0);

    bool readFrom(QIODevice* io);
    void discardBufferedLine();

signals:
    /** Emitted when a full line is read.
      *
      * Replaces any previous partial line calls.
      * A newline is present unless an EOF is encountered. */
    void lineComplete(QString);

    /** Emitted with the new partial current line when it changes. */
    void partialLine(QString);

private:
    QByteArray m_lineBuf;
    void flushLastLine();
};

#endif // TEXTLINEBUFFER_H
