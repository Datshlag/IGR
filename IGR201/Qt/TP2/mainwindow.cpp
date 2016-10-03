#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    statusBar()->show();

    /*QMenuBar * menuBar = this->menuBar();

    QMenu * fileMenu = menuBar->addMenu(tr("&File"));
    QAction * openAction = new QAction( QIcon(":open.png"), tr("&Open..."), this);
    fileMenu->addAction(openAction);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, SIGNAL(triggered()),this,SLOT(openFile()));


    QAction * saveAction = new QAction( QIcon(":save.png"), tr("&Save..."), this);
    fileMenu->addAction(saveAction);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()),this,SLOT(saveFile()));

    QAction * quitAction = new QAction( QIcon(":quit.png"), tr("&Quit..."), this);
    fileMenu->addAction(quitAction);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()),this,SLOT(quitFile()));

    QToolBar * toolBar = this->addToolBar( tr("File") );
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(quitAction);

    text = new QTextEdit(this);
    setCentralWidget(text);*/

    zone = new DrawZone(this);
    zone->setMinimumSize(800,600);
    setCentralWidget(zone);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::closeEvent(QCloseEvent *event)
{
     quitFile();
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/cal/homes/pourchot/workspace/igr/TP1");
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString str;
    str=stream.readAll();
    text->setText(str);

}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"/cal/homes/pourchot/workspace/igr/TP1");
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&file);
    QString str;
    str=text->toPlainText();
    in << str;
}
void MainWindow::quitFile()
{
    QMessageBox msgBox;
    msgBox.setText("ARE YOU SURE YOU WANT TO QUIT ??N??§§!!!");
    msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Discard);
    int ret = msgBox.exec();
    if (ret==QMessageBox::Ok) this->close();
}
*/
