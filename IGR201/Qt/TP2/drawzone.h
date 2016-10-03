#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>

class DrawZone : public QWidget
{
public:
    DrawZone(QWidget*);
    ~DrawZone();

private:
    QWidget* parent;
    int drawingLine=0;
    float currentXPos=0;
    float currentYPos=0;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:

public slots:

};

#endif // DRAWZONE_H
