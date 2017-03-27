#include "gesturetest.h"
#include <QLabel>
#include <QDebug>
#include <qmath.h>
#include <QRect>

GestureTest::GestureTest(QWidget *parent) :
    GestureDrawer(parent)
{
    setMinimumWidth(300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
    QLabel * testLabel = new QLabel("Test",this);
    testLabel->move(0,0);

    QLabel* editLabel = new QLabel("Result:", this);
    editLabel->move(0,25);
    edit = new QLineEdit("None", this);
    edit->move(50,25);
    edit->resize(230, 20);


    //create a oneDollarRecognizer
    //register this recognizer
    //...
}

void GestureTest::setTemplates(const QVector<GestureTemplate>& templates){
    //oneDollar->setTemplates( templates );
}

bool GestureTest::event(QEvent *e){
    if (e->type() == QEvent::Gesture) {
       return onGestureEvent(static_cast<QGestureEvent*>(e));
    }

    return QWidget::event(e);

}

bool GestureTest::onGestureEvent(QGestureEvent *e){
     qDebug("gesture...........");
    //...
    return true;
}


void GestureTest::drawRecognizedGesture(QPainter& p){
    p.setBrush(Qt::blue);
    p.setPen(Qt::blue);
    p.drawPolyline(m_recognizedStroke);
    if(m_recognizedStroke.size()>0){
        p.drawEllipse(m_recognizedStroke[0],2,2);
    }
}

void GestureTest::paintEvent(QPaintEvent *){
    QPainter p(this);
    drawWidget(p);
    drawRecognizedGesture(p);

}
