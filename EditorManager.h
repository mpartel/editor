#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QObject>
#include <QList>
#include <QTabWidget>
#include <QTextDocument>
#include <QPair>
#include "OpenFile.h"

/** Manages OpenFile objects and the editor tabs that view them.
  */
class EditorManager : public QObject
{
    Q_OBJECT
public:
    explicit EditorManager(QTabWidget* tabWidget, QObject *parent = 0);

    QTextDocument* activeDocument() const; // may be null
    OpenFile* activeOpenFile() const; // may be null

signals:
    
public slots:
    void startNewFile();
    void openFile(QString path);
    bool saveRequested();
    bool closeAllRequested();
    void tabCloseRequested(int index);
    void duplicateTab(int index);
    
private slots:
    void fileUnreferenced(OpenFile* file);

    void updateFileTitle(OpenFile* file);

    void showReadError(OpenFile* file, QString msg);
    void showWriteError(OpenFile* file, QString msg);

private:
    QList<OpenFile*> m_openFiles;
    QTabWidget* m_tabWidget;

    void connectErrorSignals(OpenFile* file);
    void addOpenFile(OpenFile* file);
    void openTabFor(OpenFile* file);

    QString tabTitle(OpenFile* file);
    int confirmCloseUnsaved(OpenFile* file);

    OpenFile* fileForTab(int index) const;
    OpenFile* fileForDocument(QTextDocument* doc) const;
    QList<int> tabsForFile(OpenFile* file) const;
};

#endif // EDITORMANAGER_H
