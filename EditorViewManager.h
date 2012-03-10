#ifndef EDITORVIEWMANAGER_H
#define EDITORVIEWMANAGER_H

#include <QObject>
#include <QList>
#include <QTabWidget>
#include <QTextDocument>
#include <QPair>
#include "OpenFile.h"

class EditorViewManager : public QObject
{
    Q_OBJECT
public:
    explicit EditorViewManager(QTabWidget* tabWidget, QObject *parent = 0);

signals:
    
public slots:
    void openNewFile();
    void openFile(QString path);
    void saveRequested();
    void tabCloseRequested(int index);
    
private slots:
    void fileUnreferenced(OpenFile* file);

    void updateFileTitle(OpenFile* file);

    void showReadError(OpenFile* file, QString msg);
    void showWriteError(OpenFile* file, QString msg);

private:
    QList<OpenFile*> m_openFiles;
    QTabWidget* m_tabWidget;

    QTextDocument* activeDocument() const; // or null
    OpenFile* activeOpenFile() const; // or null

    void connectErrorSignals(OpenFile* file);
    void addOpenFile(OpenFile* file);

    QString tabTitle(OpenFile* file);

    OpenFile* fileForDocument(QTextDocument* doc) const;
    QList<int> tabsForFile(OpenFile* file) const;
};

#endif // EDITORVIEWMANAGER_H
