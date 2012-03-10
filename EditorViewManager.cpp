#include "EditorViewManager.h"
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>

EditorViewManager::EditorViewManager(QTabWidget* tabWidget, QObject *parent) :
    QObject(parent),
    m_tabWidget(tabWidget)
{
    connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
}

void EditorViewManager::startNewFile()
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

bool EditorViewManager::saveRequested()
{
    OpenFile* f = activeOpenFile();
    if (f) {
        if (f->isNewUnsavedFile()) {
            QString dest = QFileDialog::getSaveFileName(m_tabWidget, QString("Save file"));
            if (dest.isNull()) {
                return false;
            }
            f->setPath(dest);
        }
        return f->save();
    }
    return false;
}

bool EditorViewManager::closeAllRequested()
{
    foreach (OpenFile* file, m_openFiles) {
        if (file->document()->isModified()) {
            int answer = confirmCloseUnsaved(file);
            switch (answer) {
            case QMessageBox::Discard:
                break;
            case QMessageBox::Save:
                if (!saveRequested()) {
                    return false;
                }
                break;
            case QMessageBox::Cancel:
            default:
                return false;
            }
        }
    }
    return true;
}

void EditorViewManager::tabCloseRequested(int index)
{
    QWidget* widget = m_tabWidget->widget(index);
    QTextEdit* editor = dynamic_cast<QTextEdit*>(widget);
    if (editor) {
        OpenFile* file = fileForDocument(editor->document());
        if (file && file->document()->isModified()) {
            int answer = confirmCloseUnsaved(file);
            switch (answer) {
            case QMessageBox::Discard:
                break;
            case QMessageBox::Save:
                if (!saveRequested()) {
                    return;
                }
                break;
            case QMessageBox::Cancel:
            default:
                return;
            }
        }
    }
    m_tabWidget->removeTab(index);
    delete widget;
}

void EditorViewManager::fileUnreferenced(OpenFile* file)
{
    m_openFiles.removeAll(file);
    file->deleteLater();
}

void EditorViewManager::updateFileTitle(OpenFile* file)
{
    foreach (int tabIndex, tabsForFile(file)) {
        m_tabWidget->setTabText(tabIndex, tabTitle(file));
    }
}

void EditorViewManager::showReadError(OpenFile* file, QString detail)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Failed to read `%1`").arg(file->path()));
    msgBox.setInformativeText(detail);
    msgBox.exec();
}

void EditorViewManager::showWriteError(OpenFile* file, QString detail)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Failed to save `%1`").arg(file->path()));
    msgBox.setInformativeText(detail);
    msgBox.exec();
}

QTextDocument* EditorViewManager::activeDocument() const
{
    QTextEdit* editor = dynamic_cast<QTextEdit*>(m_tabWidget->currentWidget());
    if (editor) {
        return editor->document();
    } else {
        return 0;
    }
}

OpenFile* EditorViewManager::activeOpenFile() const
{
    QTextDocument* doc = activeDocument();
    if (doc) {
        return fileForDocument(doc);
    } else {
        return 0;
    }
}

void EditorViewManager::connectErrorSignals(OpenFile* file)
{
    connect(file, SIGNAL(readError(OpenFile*,QString)), this, SLOT(showReadError(OpenFile*,QString)));
    connect(file, SIGNAL(writeError(OpenFile*,QString)), this, SLOT(showWriteError(OpenFile*,QString)));
}

void EditorViewManager::addOpenFile(OpenFile* file)
{
    connect(file, SIGNAL(noMoreReferences(OpenFile*)), this, SLOT(fileUnreferenced(OpenFile*)));
    m_openFiles.append(file);

    connect(file, SIGNAL(documentModifiedStatusChanged(OpenFile*,bool)), this, SLOT(updateFileTitle(OpenFile*)));

    QTextEdit* editor = new QTextEdit(m_tabWidget);
    editor->setDocument(file->document());
    m_tabWidget->addTab(editor, tabTitle(file));
    m_tabWidget->setCurrentWidget(editor);
}

QString EditorViewManager::tabTitle(OpenFile* file)
{
    QString result = file->title();
    if (file->document()->isModified()) {
        result += "*";
    }
    return result;
}

int EditorViewManager::confirmCloseUnsaved(OpenFile* file)
{
    Q_UNUSED(file);

    QMessageBox msgBox;
    msgBox.setText(tr("%1 has been modified.").arg(file->title()));
    msgBox.setInformativeText("Do you want to save it?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

OpenFile* EditorViewManager::fileForDocument(QTextDocument* doc) const
{
    foreach (OpenFile* file, m_openFiles) {
        if (file->document() == doc) {
            return file;
        }
    }
    return 0;
}

QList<int> EditorViewManager::tabsForFile(OpenFile* file) const
{
    QList<int> result;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QWidget* widget = m_tabWidget->widget(i);
        QTextEdit* editor = dynamic_cast<QTextEdit*>(widget);
        if (editor && editor->document() == file->document()) {
            result.append(i);
        }
    }
    return result;
}
