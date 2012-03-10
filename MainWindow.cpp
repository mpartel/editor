#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_editorViewManager = new EditorManager(ui->editorTabs, this);

    connect(this->ui->action_New, SIGNAL(triggered()), m_editorViewManager, SLOT(startNewFile()));
    connect(this->ui->action_Save, SIGNAL(triggered()), m_editorViewManager, SLOT(saveRequested()));
    connect(this->ui->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QList<QString> paths = QFileDialog::getOpenFileNames(this);
    foreach (QString path, paths) {
        m_editorViewManager->openFile(path);
    }
}

void MainWindow::on_actionClose_Tab_triggered()
{
    m_editorViewManager->tabCloseRequested(ui->editorTabs->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (m_editorViewManager->closeAllRequested()) {
        e->accept();
    } else {
        e->ignore();
    }
}
