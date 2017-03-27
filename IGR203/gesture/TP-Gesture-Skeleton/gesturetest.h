#ifndef GESTURETEST_H
#define GESTURETEST_H
#include "gesturedrawer.h"
#include <QMouseEvent>
#include <QGestureEvent>
#include "gesturetemplate.h"
#include <QLineEdit>

class GestureTest : public GestureDrawer
{
    Q_OBJECT
public:
    explicit GestureTest(QWidget *parent = 0);
    QPolygonF recognizedStroke(){ return m_recognizedStroke;}
    void paintEvent(QPaintEvent *);
    bool event(QEvent *);
    bool onGestureEvent(QGestureEvent *);

public slots:
    void setTemplates(const QVector<GestureTemplate> & );

protected:
    void drawRecognizedGesture(QPainter &p);

    QPolygonF m_recognizedStroke;
    Qt::GestureType ONE_DOLLAR;
    QLineEdit *edit;
};

#endif // GESTURETEST_H
