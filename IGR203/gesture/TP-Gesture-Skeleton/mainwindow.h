#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <gesturetemplate.h>
#include "gesturetest.h"
#include "gesturetraining.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addGestureTemplate(GestureTemplate);

private:
    GestureTraining * training;
    GestureTest * test;

    QVector<GestureTemplate> templateGallery;

};

#endif // MAINWINDOW_H
