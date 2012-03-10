#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_editorViewManager = new EditorManager(ui->editorTabs, this);

    connect(this->ui->actionNew, SIGNAL(triggered()), m_editorViewManager, SLOT(startNewFile()));
    connect(this->ui->actionSave, SIGNAL(triggered()), m_editorViewManager, SLOT(saveRequested()));
    connect(this->ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QList<QString> paths = QFileDialog::getOpenFileNames(this);
    foreach (QString path, paths) {
        m_editorViewManager->openFile(path);
    }
}

void MainWindow::on_actionClose_Tab_triggered()
{
    if (ui->editorTabs->count() > 0) {
        m_editorViewManager->tabCloseRequested(ui->editorTabs->currentIndex());
    }
}

void MainWindow::on_actionDuplicate_Tab_triggered()
{
    if (ui->editorTabs->count() > 0) {
        m_editorViewManager->duplicateTab(ui->editorTabs->currentIndex());
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (m_editorViewManager->closeAllRequested()) {
        e->accept();
    } else {
        e->ignore();
    }
}
