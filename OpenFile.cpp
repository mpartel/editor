#include "OpenFile.h"
#include <QTextCursor>
#include <QFile>
#include <QFileInfo>

OpenFile::OpenFile(QString path, QObject* parent) :
    QObject(parent),
    m_path(path),
    m_doc(new QTextDocument(this)),
    m_refcount(0)
{
    connect(m_doc, SIGNAL(modificationChanged(bool)), this, SLOT(emitDocumentModifiedStatusChanged(bool)));
}

QString OpenFile::title() const
{
    if (m_path.isNull()) {
        return "Untitled";
    } else {
        QFileInfo fi(m_path);
        return fi.baseName();
    }
}

void OpenFile::registerReference(QObject* obj)
{
    connect(obj, SIGNAL(destroyed()), this, SLOT(decrementRefcount()));
}

bool OpenFile::save()
{
    if (m_path.isNull()) {
        m_lastError = tr("Internal error: destination file not set.");
        emit writeError(this, m_lastError);
        return false;
    }

    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = file.errorString();
        emit writeError(this, m_lastError);
        return false;
    }
    QByteArray data = m_doc->toPlainText().toLocal8Bit();
    if (file.write(data) != data.length()) {
        m_lastError = file.errorString();
        emit writeError(this, m_lastError);
        return false;
    }

    m_doc->setModified(false);
    emit documentSaved(this);
    return true;
}

bool OpenFile::revertToSavedState()
{
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = file.errorString();
        emit readError(this, m_lastError);
        return false;
    }
    QString text = file.readAll();
    if (file.error() != QFile::NoError) {
        m_lastError = file.errorString();
        emit readError(this, m_lastError);
        return false;
    }

    QTextCursor cursor(m_doc);
    cursor.insertText(text);
    m_doc->clearUndoRedoStacks();

    m_doc->setModified(false);
    emit documentLoaded(this);
    return true;
}

void OpenFile::emitDocumentModifiedStatusChanged(bool newStatus)
{
    emit documentModifiedStatusChanged(this, newStatus);
}

void OpenFile::decrementRefcount()
{
    m_refcount -= 1;
    Q_ASSERT(m_refcount >= 0);
    if (m_refcount == 0) {
        emit noMoreReferences(this);
    }
}
