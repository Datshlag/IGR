#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent)
{

    std::cout<<"test";
    this->parent=parent;
    penThickness=2;
    penColor = new QColor(0,0,0);
    penCapStyle=Qt::PenCapStyle::RoundCap;
    pointA=NULL;
    pointB=NULL;
}

DrawZone::~DrawZone()
{
    
}

void DrawZone::paintEvent(QPaintEvent *e)
{

    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rectangle;
    rectangle.setCoords(-1,-1,this->width(),this->height());

    QRadialGradient gradient(rectangle.center(),1000);
    QBrush gradientBrush(gradient);

    QBrush brush(Qt::GlobalColor::white);

    painter.setBrush(brush);
    //painter.setBrush(gradientBrush);
    painter.drawRect(rectangle);

    QPen pen;
    pen.setWidth(penThickness);
    pen.setColor(*penColor);
    pen.setCapStyle(penCapStyle);
    painter.setPen(pen);

    painter.drawLine(*pointA,*pointB);

}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        *pointA=e->pos();
        *pointB=*pointA;
        lineNotDrawn=1;
        update();
    }
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(lineNotDrawn){
        *pointB=e->pos();
    }
    update();
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        *pointB=e->pos();
        lineNotDrawn=0;
        update();
    }
}

void DrawZone::setPenColor(QColor* color)
{

    penColor=color;
}

void DrawZone::setThickness(float f)
{

    penThickness=f;
}

void DrawZone::setCapStyle(Qt::PenCapStyle pcs)
{

    penCapStyle=pcs;
}
