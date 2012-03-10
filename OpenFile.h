#ifndef OPENFILE_H
#define OPENFILE_H

#include <QObject>
#include <QTextDocument>

class OpenFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTextDocument* document READ document)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString path READ path WRITE setPath)

public:
    explicit OpenFile(QString path = QString::null, QObject* parent = 0);
    
    QString path() const { return m_path; } // May be a null string
    QString title() const;
    QTextDocument* document() const { return m_doc; }

    void setPath(QString path) { m_path = path; }

    bool isNewUnsavedFile() const { return m_path.isNull(); }

    void registerReference(QObject* obj);
    bool hasReferences() const { return m_refcount > 0; }

signals:
    void noMoreReferences(OpenFile* file);

    void documentModifiedStatusChanged(OpenFile* file, bool newStatus);
    void documentSaved(OpenFile* file);
    void documentLoaded(OpenFile* file);

    void readError(OpenFile* file, QString msg);
    void writeError(OpenFile* file, QString msg);
    
public slots:
    bool save();
    bool revertToSavedState();
    
private slots:
    void emitDocumentModifiedStatusChanged(bool newStatus);
    void decrementRefcount();

private:
    QString m_path;
    QTextDocument* m_doc;
    int m_refcount;
    QString m_lastError;
};

#endif // OPENFILE_H
