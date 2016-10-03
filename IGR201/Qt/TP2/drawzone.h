#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>

class DrawZone : public QWidget
{
public:
    DrawZone(QWidget*);
    ~DrawZone();

private:
    QWidget* parent;
    QPointF pointA;
    QPointF pointB;
    int lineNotDrawn=0;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:

public slots:

};

#endif // DRAWZONE_H
