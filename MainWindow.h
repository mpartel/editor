#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "OpenFile.h"
#include "EditorViewManager.h"

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
    void on_action_Open_triggered();

private:
    Ui::MainWindow* ui;
    EditorViewManager* m_editorViewManager;

    QString m_lastOpenDir;
};

#endif // MAINWINDOW_H
