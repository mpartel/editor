#ifndef EDITORVIEWMANAGER_H
#define EDITORVIEWMANAGER_H

#include <QObject>
#include <QList>
#include <QTabWidget>
#include <QTextDocument>
#include "OpenFile.h"

class EditorViewManager : public QObject
{
    Q_OBJECT
public:
    explicit EditorViewManager(QTabWidget* activeTabWidget, QObject *parent = 0);

signals:
    
public slots:
    void openNewFile();
    void openFile(QString path);
    
private slots:
    void openFileUnreferenced(OpenFile* file);
    void tabCloseRequested(int index);

private:
    QList<OpenFile*> m_openFiles;
    QTabWidget* m_activeTabWidget;

    void connectErrorSignals(OpenFile* file);
    void addOpenFile(OpenFile* file);

    OpenFile* fileForDocument(QTextDocument* doc);
};

#endif // EDITORVIEWMANAGER_H
