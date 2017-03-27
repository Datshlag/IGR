#include "gesturetraining.h"
#include <QMouseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

GestureTraining::GestureTraining(QWidget *parent) :
    GestureDrawer(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    QLabel * trainingLabel = new QLabel("Training",this);
    trainingLabel->move(0,0);
    QLabel *templateName = new QLabel ("Template Name", this);
    templateName->move(0,25);
    edit = new QLineEdit("Template 1",this);
    edit->move(templateName->width(),25);
    QPushButton *add = new QPushButton("Add", this);
    add->move(225,22);
    setMinimumWidth(add->x()+add->width());

    //connect the click event of the button to the slot createTemplate()
    //...

}

void GestureTraining::createTemplate(){
    //check that the stroke is valid
    //create a template
    //clear teh training area
    //...
}

void GestureTraining::mousePressEvent(QMouseEvent*e){
 //...
}

void GestureTraining::mouseMoveEvent(QMouseEvent*e){
//...
}

void GestureTraining::mouseReleaseEvent(QMouseEvent*e){
//..
}
