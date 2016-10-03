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

    if(drawingLine==1){

    }

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){


        drawingLine=1;
        update();
    }
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        update();
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        drawingLine=0;
        update();
    }
}
