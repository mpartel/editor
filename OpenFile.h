#ifndef OPENFILE_H
#define OPENFILE_H

#include <QObject>
#include <QTextDocument>

class OpenFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTextDocument* document READ document)

public:
    explicit OpenFile(QString path = QString::null, QObject* parent = 0);
    
    QString path() const { return m_path; } // May be a null string
    QString title() const;
    QTextDocument* document() const { return m_doc; }

    bool isNewUnsavedFile() const { return m_path.isNull(); }
    bool isChangedAfterLastSave() const { return m_changedAfterLastSave; }

    void registerReference(QObject* obj);
    bool hasReferences() const { return m_refcount > 0; }

signals:
    void noMoreReferences(OpenFile* file);

    void documentUnsaved(OpenFile* file);
    void documentSaved(OpenFile* file);

    void readError(OpenFile* file, QString explanation);
    void writeError(OpenFile* file, QString explanation);
    
public slots:
    bool save();
    bool revertToSavedState();
    
private slots:
    void documentChanged();
    void decrementRefcount();

private:
    QString m_path;
    QTextDocument* m_doc;
    bool m_changedAfterLastSave;
    int m_refcount;
    QString m_lastError;
};

#endif // OPENFILE_H
