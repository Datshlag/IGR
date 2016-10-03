#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <iostream>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QCloseEvent>
#include <drawzone.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    DrawZone * zone;
    //void closeEvent(QCloseEvent *event);

public slots:
    /*void openFile();
    void saveFile();
    void quitFile();*/

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
