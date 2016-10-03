#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent)
{
    this->parent=parent;
}

DrawZone::~DrawZone()
{
    delete this;
}

void DrawZone::paintEvent(QPaintEvent *e)
{

    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rectangle;
    rectangle.setCoords(-1,-1,this->width()-1,this->height()-1);

    QRadialGradient gradient(rectangle.center(),1000);
    QBrush gradientBrush(gradient);

    QBrush brush(Qt::GlobalColor::white);

    painter.setBrush(brush);
    //painter.setBrush(gradientBrush);
    painter.drawRect(rectangle);

    QPen pen;
    pen.setWidth(2);
    pen.setBrush(Qt::black);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);

    painter.drawLine(pointA,pointB);

}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        pointA=e->pos();
        pointB=pointA;
        lineNotDrawn=1;
        update();
    }
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(lineNotDrawn){
        pointB=e->pos();
    }
    update();
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        pointB=e->pos();
        lineNotDrawn=0;
        update();
    }
}
