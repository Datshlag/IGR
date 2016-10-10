#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent)
{

    this->parent=parent;
    drawable=1;
    fillingColor = Qt::GlobalColor::white;

    indexCurrent=0;

    currentPenCapStyle=Qt::PenCapStyle::SquareCap;
    currentPenJoinStyle=Qt::PenJoinStyle::BevelJoin;
    currentPenWidth=0;
    currentPenColor=Qt::GlobalColor::black;
    currentPointA = new QPoint();
    currentPointB = new QPoint();

    lineDrawList.append(QPair<QPoint,QPoint>(*currentPointA,*currentPointB));
    penWidthDrawList.append(currentPenWidth);
    penColorDrawList.append(currentPenColor);
    penCapStyleDrawList.append(currentPenCapStyle);
    penJoinStyleDrawList.append(currentPenJoinStyle);

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
    QBrush brush(fillingColor);
    painter.setBrush(brush);
    painter.drawRect(rectangle);

    if(drawable)
    {
        QPen pen;
        for(int i=0; i<=indexCurrent; i++)
        {
            pen.setWidth(penWidthDrawList.at(i));
            pen.setColor(penColorDrawList.at(i));
            pen.setCapStyle(penCapStyleDrawList.at(i));
            painter.setPen(pen);
            painter.drawLine(lineDrawList.at(i).first,lineDrawList.at(i).second);
        }
    }
}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        lineDrawList[indexCurrent].first=e->pos();
        lineDrawList[indexCurrent].second=e->pos();
        lineNotDrawn=1;
        update();
    }

    emit drawZoneClicked();
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(lineNotDrawn){
        lineDrawList[indexCurrent].second=e->pos();
    }
    update();
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        lineDrawList[indexCurrent].second=e->pos();
        lineNotDrawn=0;
        update();
        this->expandDrawList();
    }
}

void DrawZone::setCurrentPenColor(QColor color)
{

    currentPenColor=color;
    penColorDrawList[indexCurrent]=color;
}

void DrawZone::setCurrentPenWidth(int n)
{

    currentPenWidth=n;
    penWidthDrawList[indexCurrent]=n;
}

void DrawZone::setCurrentPenCapStyle(Qt::PenCapStyle pcs)
{

    penCapStyleDrawList[indexCurrent]=pcs;
    currentPenCapStyle=pcs;
}

void DrawZone::setCurrentPenJoinStyle(Qt::PenJoinStyle pjs)
{

    penJoinStyleDrawList[indexCurrent]=pjs;
    currentPenJoinStyle=pjs;
}

void DrawZone::setCurrentPenCapStyleFromInt(int n)
{

    qDebug()<<n;
    switch(n){
        case(0):
            this->setCurrentPenCapStyle(Qt::PenCapStyle::SquareCap);
            break;
        case(1):
            this->setCurrentPenCapStyle(Qt::PenCapStyle::FlatCap);
            break;
        case(2):
            this->setCurrentPenCapStyle(Qt::PenCapStyle::RoundCap);
    }
}

void DrawZone::setCurrentPenJoinStyleFromInt(int n)
{

    switch(n){
        case(0):
            this->setCurrentPenJoinStyle(Qt::PenJoinStyle::BevelJoin);
            break;
        case(1):
            this->setCurrentPenJoinStyle(Qt::PenJoinStyle::MiterJoin);
            break;
        case(2):
            this->setCurrentPenJoinStyle(Qt::PenJoinStyle::RoundJoin);
    }
}

void DrawZone::expandDrawList()
{

    indexCurrent++;
    lineDrawList.append(QPair<QPoint,QPoint>(*currentPointA,*currentPointB));
    penWidthDrawList.append(currentPenWidth);
    penColorDrawList.append(currentPenColor);
    penCapStyleDrawList.append(currentPenCapStyle);
    penJoinStyleDrawList.append(currentPenJoinStyle);
}

void DrawZone::setDrawable(int n)
{
    drawable=n;
}

void DrawZone::setFillingColor(QColor color)
{

    fillingColor=color;
}

void DrawZone::fillDrawZone(QColor color)
{

    setFillingColor(color);
    update();
}
