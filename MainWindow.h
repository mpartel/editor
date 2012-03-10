#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QCloseEvent>
#include <QProcess>
#include "OpenFile.h"
#include "EditorManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_Tab_triggered();
    void on_actionDuplicate_Tab_triggered();

    void on_actionRun_program_triggered();

    void processStarted();
    void processStartError(QProcess::ProcessError err);
    void processFinished(int status);

protected:
    virtual void closeEvent(QCloseEvent* e);

private:
    Ui::MainWindow* ui;
    EditorManager* m_editorViewManager;
    QProcess* m_process;
};

#endif // MAINWINDOW_H
