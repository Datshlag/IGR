#ifndef ONEDOLLARGESTURE_H
#define ONEDOLLARGESTURE_H

#include <QGesture>
#include <QPolygonF>
#include "gesturetemplate.h"
class OneDollarRecognizer;
class OneDollarGesture : public QGesture
{
    Q_OBJECT
    Q_PROPERTY(QPolygonF m_stroke READ stroke)

public:
    friend class OneDollarRecognizer;

    explicit OneDollarGesture(QObject *parent = 0):QGesture(parent){}
    QPolygonF stroke(){ return m_stroke;}
    void setStroke(QPolygonF p){ m_stroke = p;}
    void addPoint(QPoint p){ m_stroke.append(p);}
    void clear(){ m_stroke.clear(); }
    GestureTemplate recognizedTemplate()const{ return m_recognizedStroke; }
    float score()const{ return m_score; }
    float bestAngle()const{ return m_bestAngle; }

protected:
    void addPoint(QPointF _p){ m_stroke.append(_p); }
    void setScore(float _score){ m_score = _score; }
    void setBestAngle(float _a){ m_bestAngle = _a; }
    void setGestureTemplate(GestureTemplate _t){ m_recognizedStroke = _t; }

    QPolygonF m_stroke;
    GestureTemplate m_recognizedStroke;
    float m_score;
    float m_bestAngle;


};

#endif // ONEDOLLARGESTURE_H
