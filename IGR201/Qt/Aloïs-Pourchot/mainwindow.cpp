#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->widget->setMouseTracking(true);
    zone=ui->widget;

    /*
     * Controller for pen size
     * a QSpinBox
     */
    QSpinBox* spinBox = ui->penSizeSpinBox;
    spinBox->setMinimum(0);
    spinBox->setValue(0);
    connect(spinBox, SIGNAL(valueChanged(int)), zone, SLOT(setCurrentPenWidth(int)));
    connect(ui->actionPen_size, SIGNAL(triggered(bool)), this, SLOT(openSizeSelectionBox()));



    /*
     * Controller for the pen color
     * a drawzone used to give a preview of the color
     * which can be clicked to open a QColorDialog
     */
    colorPreview = ui->widget_2;
    colorPreview->fillDrawZone(Qt::black);
    colorPreview->setCurrentMode(-1);
    connect(colorPreview, SIGNAL(drawZoneClicked()), this, SLOT(openColorSelectionDialog()));
    connect(ui->actionPen_color,SIGNAL(triggered(bool)),this,SLOT(openColorSelectionDialog()));

    /*
     * Controllers for the pen join style
     * a QComboBox
     * a QActionGroup
     */
    QComboBox* joinStyleComboBox = ui->joinStyleComboBox;
    penJoinStyleActionGroup = new QActionGroup(this);
    penJoinStyleActionGroup->addAction(ui->actionMiter_join);
    penJoinStyleActionGroup->addAction(ui->actionBevel_join);
    penJoinStyleActionGroup->addAction(ui->actionRound_join);
    penJoinStyleActionGroup->setExclusive(true);
    connect(joinStyleComboBox, SIGNAL(currentIndexChanged(int)), zone, SLOT(setCurrentPenJoinStyle(int)));
    connect(zone, SIGNAL(penJoinStyleChanged(int)), this, SLOT(actualizePenJoinStyleUi(int)));
    connect(penJoinStyleActionGroup, SIGNAL(triggered(QAction*)), zone, SLOT(setCurrentPenJoinStyle(QAction*)));

    /*
     * Controllers for the pen cap style
     * a QComboBox
     * a QActionGroup
     */
    QComboBox* capStyleComboBox = ui->capStyleComboBox;
    penCapStyleActionGroup = new QActionGroup(this);
    penCapStyleActionGroup->addAction(ui->actionSquare_cap);
    penCapStyleActionGroup->addAction(ui->actionFlat_cap);
    penCapStyleActionGroup->addAction(ui->actionRound_cap);
    penCapStyleActionGroup->setExclusive(true);
    connect(capStyleComboBox, SIGNAL(currentIndexChanged(int)),zone, SLOT(setCurrentPenCapStyle(int)));
    connect(zone, SIGNAL(penCapStyleChanged(int)), this, SLOT(actualizePenCapStyleUi(int)));
    connect(penCapStyleActionGroup, SIGNAL(triggered(QAction*)),zone,SLOT(setCurrentPenCapStyle(QAction*)));

    /*
     * Controllers for the shape's style
     * a QComboBox
     * a QActionGroup
     */
    QComboBox* shapeStyleComboBox = ui->shapeTypeComboBox;
    shapeStyleActionGroup = new QActionGroup(this);
    shapeStyleActionGroup->addAction(ui->actionLine);
    shapeStyleActionGroup->addAction(ui->actionEllipse);
    shapeStyleActionGroup->addAction(ui->actionRectangle);
    shapeStyleActionGroup->addAction(ui->actionPolygon);
    shapeStyleActionGroup->addAction(ui->actionFree_draw);
    shapeStyleActionGroup->addAction(ui->actionPolyline);
    shapeStyleActionGroup->setExclusive(true);
    connect(shapeStyleActionGroup, SIGNAL(triggered(QAction*)), zone, SLOT(setCurrentObjectToDraw(QAction*)));
    connect(shapeStyleComboBox, SIGNAL(currentIndexChanged(int)), zone, SLOT(setCurrentObjectToDraw(int)));
    connect(zone, SIGNAL(shapeStyleChanged(int)), this, SLOT(actualizeShapeStyleUi(int)));

    /*
     * Controllers for the shape filling status
     * a QComboBox
     * a QCheckBox
     */
    QComboBox* fillRuleComboBox = ui->fillRuleComboBox;
    QCheckBox* fillCheckBox = ui->fillCheckBox;
    connect(fillCheckBox, SIGNAL(toggled(bool)), zone, SLOT(toggleFilling(bool)));
    connect(fillRuleComboBox, SIGNAL(currentIndexChanged(int)), zone, SLOT(setFillingRule(int)));

    /*
     * Controllers for the edition mode
     * a QActionGroup
     * a ButtonGroup
     */
    editionModeActionGroup = new QActionGroup(this);
    editionModeActionGroup->addAction(ui->actionDraw);
    editionModeActionGroup->addAction(ui->actionSelect);
    editionModeActionGroup->setExclusive(true);
    editionModeButtonGroup = new QButtonGroup();
    editionModeButtonGroup->addButton(ui->drawRadioButton);
    editionModeButtonGroup->addButton(ui->selectRadioButton);
    editionModeButtonGroup->setId(ui->drawRadioButton,0);
    editionModeButtonGroup->setId(ui->selectRadioButton,1);
    connect(editionModeButtonGroup, SIGNAL(buttonClicked(int)), zone, SLOT(setCurrentMode(int)));
    connect(editionModeActionGroup, SIGNAL(triggered(QAction*)), zone, SLOT(setCurrentMode(QAction*)));
    connect(zone, SIGNAL(currentModeChanged(int)), this, SLOT(actualizeEditionModeUi(int)));

    /*
     * Controllers for AA setting
     * a QCheckBox
     * a QAction
     */
    connect(ui->actionAntialiasing, SIGNAL(toggled(bool)), zone, SLOT(toggleAA(bool)));
    connect(ui->AACheckBox, SIGNAL(toggled(bool)), zone, SLOT(toggleAA(bool)));
    connect(zone, SIGNAL(AAStateChanged(bool)), this, SLOT(actualizeAAStatusUi(bool)));

    connect(ui->actionSave, SIGNAL(triggered(bool)), zone, SLOT(saveDrawList()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), zone, SLOT(openDrawList()));
    connect(ui->actionClear, SIGNAL(triggered(bool)), zone, SLOT(emptyDrawList()));
    connect(ui->actionAnnuler, SIGNAL(triggered(bool)), zone, SLOT(cancel()));
    connect(ui->actionDelete, SIGNAL(triggered(bool)), zone, SLOT(clearSelectedElement()));

}

MainWindow::~MainWindow()
{
    delete penJoinStyleActionGroup;
    delete penCapStyleActionGroup;
    delete shapeStyleActionGroup;
    delete editionModeActionGroup;
    delete ui;
}

void MainWindow::openColorSelectionDialog()
{

    QColorDialog *dialog = new QColorDialog(this);
    connect(dialog, SIGNAL(colorSelected(QColor)), zone, SLOT(setCurrentPenColor(QColor)));
    connect(dialog, SIGNAL(colorSelected(QColor)), colorPreview, SLOT(fillDrawZone(QColor)));
    dialog->exec();
    delete dialog;
}

void MainWindow::openSizeSelectionBox()
{

    ui->penSizeSpinBox->clear();
    ui->penSizeSpinBox->setFocus();
}

void MainWindow::actualizePenCapStyleUi(int n)
{

    ui->capStyleComboBox->setCurrentIndex(n);
    switch(n){
    case(0):
        ui->actionSquare_cap->setChecked(true);
        break;
    case(1):
        ui->actionFlat_cap->setChecked(true);
        break;
    case(2):
        ui->actionRound_cap->setChecked(true);
        break;
    }
}

void MainWindow::actualizePenJoinStyleUi(int n)
{

    ui->joinStyleComboBox->setCurrentIndex(n);
    switch(n){
    case(0):
        ui->actionBevel_join->setChecked(true);
        break;
    case(1):
        ui->actionMiter_join->setChecked(true);
        break;
    case(2):
        ui->actionRound_join->setChecked(true);
        break;
    }
}

void MainWindow::actualizeEditionModeUi(int n)
{

    if(n==0)
    {
        ui->drawRadioButton->setChecked(true);
        ui->actionDraw->setChecked(true);
    }
    if(n==1)
    {
        ui->selectRadioButton->setChecked(true);
        ui->actionSelect->setChecked(true);
    }
}

void MainWindow::actualizeAAStatusUi(bool n)
{

    if(!n)
    {

        ui->AACheckBox->setChecked(false);
        ui->actionAntialiasing->setChecked(false);
    }
    if(n)
    {

        ui->AACheckBox->setChecked(true);
        ui->actionAntialiasing->setChecked(true);
    }
}

void MainWindow::actualizeShapeStyleUi(int n)
{

    ui->shapeTypeComboBox->setCurrentIndex(n);
    switch(n){
    case(0):
        ui->actionLine->setChecked(true);
        break;
    case(1):
        ui->actionPolyline->setChecked(true);
        break;
    case(2):
        ui->actionPolygon->setChecked(true);
        break;
    case(3):
        ui->actionEllipse->setChecked(true);
        break;
    case(4):
        ui->actionRectangle->setChecked(true);
        break;
    case(5):
        ui->actionFree_draw->setChecked(true);
        break;
    }
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
