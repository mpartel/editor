#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_process(new QProcess(this))
{
    ui->setupUi(this);

    m_editorViewManager = new EditorManager(ui->editorTabs, this);

    connect(this->ui->actionNew, SIGNAL(triggered()), m_editorViewManager, SLOT(startNewFile()));
    connect(this->ui->actionSave, SIGNAL(triggered()), m_editorViewManager, SLOT(saveRequested()));
    connect(this->ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(this->ui->actionStop_program, SIGNAL(triggered()), m_process, SLOT(kill()));

    //FIXME: avoid full buffering (on Linux at least) by letting subprocess think it's on a terminal
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processStartError(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    ui->ioView->setIODevice(m_process);
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

void MainWindow::on_actionRun_program_triggered()
{
    m_editorViewManager->saveRequested(); //TODO: save all instead

    OpenFile* openFile = m_editorViewManager->activeOpenFile();
    if (m_process->state() == QProcess::NotRunning && openFile) {
        ui->ioView->clear();

        QFileInfo fileInfo(openFile->path());
        m_process->setWorkingDirectory(fileInfo.dir().path());

        QStringList args;
        args.append(fileInfo.fileName());

        m_process->start("python3", args);
    }
}

void MainWindow::processStarted()
{
    ui->actionRun_program->setEnabled(false);
    ui->actionStop_program->setEnabled(true);
}

void MainWindow::processStartError(QProcess::ProcessError err)
{
    Q_UNUSED(err);
    ui->actionRun_program->setEnabled(true);
    ui->actionStop_program->setEnabled(false);
}

void MainWindow::processFinished(int status)
{
    Q_UNUSED(status);
    ui->actionRun_program->setEnabled(true);
    ui->actionStop_program->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    m_process->kill();
    m_process->waitForFinished(5000);
    if (m_editorViewManager->closeAllRequested()) {
        e->accept();
    } else {
        e->ignore();
    }
}
