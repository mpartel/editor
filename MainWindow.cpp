#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_lastOpenDir(QDir::currentPath())
{
    ui->setupUi(this);

    m_editorViewManager = new EditorViewManager(ui->editorTabs, this);

    connect(this->ui->action_New, SIGNAL(triggered()), m_editorViewManager, SLOT(openNewFile()));
    connect(this->ui->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QFileDialog* dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->exec();
    QList<QString> paths = dialog->selectedFiles();
    foreach (QString path, paths) {
        m_editorViewManager->openFile(path);
    }
}
