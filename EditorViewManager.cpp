#include "EditorViewManager.h"
#include <QTextEdit>

EditorViewManager::EditorViewManager(QTabWidget* activeTabWidget, QObject *parent) :
    QObject(parent),
    m_activeTabWidget(activeTabWidget)
{
    connect(m_activeTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
}

void EditorViewManager::openNewFile()
{
    OpenFile* file = new OpenFile(QString::null, this);
    connectErrorSignals(file);
    addOpenFile(file);
}

void EditorViewManager::openFile(QString path)
{
    OpenFile* file = new OpenFile(path, this);
    connectErrorSignals(file);

    if (!file->revertToSavedState()) {
        delete file;
        return;
    }

    addOpenFile(file);
}

void EditorViewManager::openFileUnreferenced(OpenFile* file)
{
    m_openFiles.removeOne(file);
    file->deleteLater();
}

void EditorViewManager::tabCloseRequested(int index)
{
    QWidget* widget = m_activeTabWidget->widget(index);
    QTextEdit* editor = dynamic_cast<QTextEdit*>(widget);
    if (editor) {
        OpenFile* file = fileForDocument(editor->document());
        if (file && file->isChangedAfterLastSave()) {
            //TODO: confirmation dialog
        }
    }
    m_activeTabWidget->removeTab(index);
}

void EditorViewManager::connectErrorSignals(OpenFile* file)
{
    //TODO:
    //connect(file, SIGNAL(readError(OpenFile*,QString)), ...)
    //connect(file, SIGNAL(writeError(OpenFile*,QString)), ...)
}

void EditorViewManager::addOpenFile(OpenFile* file)
{
    connect(file, SIGNAL(noMoreReferences(OpenFile*)), this, SLOT(openFileUnreferenced(OpenFile*)));
    m_openFiles.append(file);

    QTextEdit* editor = new QTextEdit(m_activeTabWidget);
    editor->setDocument(file->document());
    m_activeTabWidget->addTab(editor, file->title());
    m_activeTabWidget->setCurrentWidget(editor);
}

OpenFile* EditorViewManager::fileForDocument(QTextDocument* doc)
{
    foreach (OpenFile* file, m_openFiles) {
        if (file->document() == doc) {
            return file;
        }
    }
    return 0;
}
