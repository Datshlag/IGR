#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <QColor>
#include <QObject>

class DrawZone : public QWidget
{
    Q_OBJECT

public:
    DrawZone(QWidget*);
    ~DrawZone();

private:
    QWidget* parent;
    QPoint* pointA;
    QPoint* pointB;
    QColor* penColor;

    float penThickness;
    Qt::PenCapStyle penCapStyle;

    int lineNotDrawn=0;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:

public slots:
    void setPenColor(QColor*);
    void setThickness(float);
    void setCapStyle(Qt::PenCapStyle);

};

#endif // DRAWZONE_H
