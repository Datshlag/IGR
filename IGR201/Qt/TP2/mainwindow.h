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
#include <QColorDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QTabWidget>
#include <QtUiTools/QtUiTools>

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
    DrawZone * colorPreview;
    //DrawZone * penPreview;

public slots:
    void openColorSelectionDialog();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
