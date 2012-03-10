#include "OpenFile.h"
#include <QTextCursor>
#include <QFile>
#include <QFileInfo>

OpenFile::OpenFile(QString path, QObject* parent) :
    QObject(parent),
    m_path(path),
    m_doc(new QTextDocument(this)),
    m_changedAfterLastSave(false),
    m_refcount(0)
{
    connect(m_doc, SIGNAL(contentsChanged()), this, SLOT(documentChanged()));
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
    //TODO
    //m_changedAfterLastSave = false;
    //emit documentSaved(this);
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
    file.close();

    QTextCursor cursor(m_doc);
    cursor.insertText(text);
    m_changedAfterLastSave = false;
    return true;
}

void OpenFile::documentChanged()
{
    emit documentUnsaved(this);
    m_changedAfterLastSave = true;
}

void OpenFile::decrementRefcount()
{
    m_refcount -= 1;
    Q_ASSERT(m_refcount >= 0);
    if (m_refcount == 0) {
        emit noMoreReferences(this);
    }
}
