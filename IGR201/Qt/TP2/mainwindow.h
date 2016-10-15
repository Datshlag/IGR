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
#include <Qt>

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
    DrawZone* zone;
    DrawZone* colorPreview;
    QActionGroup* penCapStyleActionGroup;
    QActionGroup* penJoinStyleActionGroup;
    QActionGroup* shapeStyleActionGroup;
    QActionGroup* editionModeActionGroup;
    QButtonGroup* editionModeButtonGroup;

public slots:
    void openColorSelectionDialog();
    void openSizeSelectionBox();
    void actualizePenCapStyleUi(int);
    void actualizePenJoinStyleUi(int);
    void actualizeEditionModeUi(int);
    void actualizeAAStatusUi(bool);
    void actualizeShapeStyleUi(int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
