#include "TextLineBuffer.h"

TextLineBuffer::TextLineBuffer(QObject* parent) :
    QObject(parent)
{
}

bool TextLineBuffer::readFrom(QIODevice* io)
{
    qint64 avail = io->bytesAvailable();
    if (avail == 0) {
        return true;
    }

    QByteArray buf(avail, '\0');
    bool ok = (io->read(buf.data(), avail) == avail);
    for (int i = 0; i < buf.size(); ++i) {
        char c = buf[i];
        if (c == '\n') {
            flushLastLine();
        } else {
            m_lineBuf.append(c);
        }
    }

    emit partialLine(QString::fromUtf8(m_lineBuf));
    return ok;
}

void TextLineBuffer::discardBufferedLine()
{
    m_lineBuf.clear();
}

void TextLineBuffer::flushLastLine()
{
    emit lineComplete(QString::fromUtf8(m_lineBuf));
    m_lineBuf.clear();
}
