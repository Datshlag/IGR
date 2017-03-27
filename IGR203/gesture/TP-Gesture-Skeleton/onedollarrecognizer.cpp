#include "onedollarrecognizer.h"
#include <QDebug>
#include <QLine>
#include <QEvent>
#include <QMouseEvent>

OneDollarRecognizer::OneDollarRecognizer() :QGestureRecognizer(), OneDollarUtil()
{
}

QGesture * OneDollarRecognizer::create(QObject * target){
    //...
    return 0;
}

QGestureRecognizer::Result OneDollarRecognizer::recognize(QGesture * gesture, QObject * watched, QEvent * e){

    static bool pressed = false;
    QGestureRecognizer::Result result = QGestureRecognizer::Ignore;
    OneDollarGesture *pGesture = static_cast<OneDollarGesture*>(gesture);
    switch(e->type()) {
    case QEvent::MouseButtonPress: {
        pressed = true;
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        pGesture->addPoint(mouseEvent->pos());
        //result = QGestureRecognizer::?????;
    }
        break;
    case QEvent::MouseMove:{
        if(pressed){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
            pGesture->addPoint(mouseEvent->pos());
            //result = QGestureRecognizer::????;
        }
    }
        break;
    case QEvent::MouseButtonRelease: {
        pressed = false;
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        pGesture->addPoint(mouseEvent->pos());
        result = QGestureRecognizer::CancelGesture;

        QPolygonF stroke = pGesture->stroke();
        GestureTemplate winner;

        float score = 0;
        if(stroke.size()>10 && templates.size()>0){
            //recognize the gesture
            //winner = ????
            if (score > 0.1){
                pGesture->setGestureTemplate(winner);
                pGesture->setScore(score);
                //result = QGestureRecognizer::????FinishGesture;
                qDebug()<<"gesture recognized; score: "<< score<< " name: " <<winner.name();
            }
        }
    }
        break;
    default:
        break;
    }

    return result;
}
#include "gesturedrawer.h"


void OneDollarRecognizer::reset(QGesture *gesture){
    //...
}

