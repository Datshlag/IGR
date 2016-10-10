#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zone=ui->widget;

    //QAction * changeColorAction = new QAction("Select Color", this);

    QSpinBox* spinBox = ui->penSizeSpinBox;
    spinBox->setMinimum(0);
    spinBox->setValue(0);
    spinBox->setSpecialValueText(tr(" Pen Width "));

    QComboBox* comboBox1 = ui->capStyleComboBox;
    comboBox1->addItem("Square Cap");
    comboBox1->addItem("Flat Cap");
    comboBox1->addItem("Round Cap");

    QComboBox* comboBox2 = ui->joinStyleComboBox;
    comboBox2->addItem("Bevel Join");
    comboBox2->addItem("Miter Join");
    comboBox2->addItem("Round Join");

    //QPushButton* pushButton = ui->pushButton;
    colorPreview = ui->widget_2;
    colorPreview->setDrawable(0);
    colorPreview->fillDrawZone(Qt::GlobalColor::black);

    connect(spinBox, SIGNAL(valueChanged(int)), zone, SLOT(setCurrentPenWidth(int)));
    connect(colorPreview, SIGNAL(drawZoneClicked()), this, SLOT(openColorSelectionDialog()));
    connect(comboBox1, SIGNAL(currentIndexChanged(int)),zone, SLOT(setCurrentPenCapStyleFromInt(int)));
    connect(comboBox2, SIGNAL(currentIndexChanged(int)), zone, SLOT(setCurrentPenJoinStyleFromInt(int)));

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::openColorSelectionDialog()
{

    QColorDialog *dialog = new QColorDialog(this);
    connect(dialog, SIGNAL(colorSelected(QColor)), zone, SLOT(setCurrentPenColor(QColor)));
    connect(dialog,SIGNAL(colorSelected(QColor)), colorPreview, SLOT(fillDrawZone(QColor)));
    dialog->exec();
    delete dialog;
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
