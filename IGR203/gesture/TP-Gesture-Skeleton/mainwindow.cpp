#include "mainwindow.h"
#include "gesturedrawer.h"
#include <QDebug>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    QWidget * container = new QWidget();
    this->setCentralWidget(container);


    QVBoxLayout* vLayout = new QVBoxLayout(container);
    QHBoxLayout* hLayout = new QHBoxLayout();

    //create a QScrollArea to display the templates
    QHBoxLayout* scrollAreaLayout = new QHBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    scrollAreaLayout->addWidget(new QPushButton("bouton 1"));
    scrollAreaLayout->addWidget(new QPushButton("bouton 2"));
    scrollArea->setLayout(scrollAreaLayout);
    scrollArea->setWidgetResizable(true);

    //create the training area
    GestureTest* gestureTest = new GestureTest();

    //create the test area
    GestureTraining* gestureTraining = new GestureTraining();

    vLayout->addWidget(scrollArea,1);
    hLayout->addWidget(gestureTraining, 1);
    hLayout->addWidget(gestureTest, 1);
    vLayout->addLayout(hLayout,5);
}

MainWindow::~MainWindow(){}

void MainWindow::addGestureTemplate(GestureTemplate gestureTemplate){
    qDebug() << "add templates";

    templateGallery.append(gestureTemplate);

    //add the template to the test area
    //test->setTemplates(templateGallery);

    //create a vignette of the template and add it to the template container layout.

}
