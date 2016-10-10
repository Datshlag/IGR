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
    currentPointA = new QPoint(-1,-1);
    currentPointB = new QPoint(-1,-1);

    lineDrawList.append(QPair<QPoint,QPoint>(*currentPointA,*currentPointB));
    penWidthDrawList.append(currentPenWidth);
    penColorDrawList.append(currentPenColor);
    penCapStyleDrawList.append(currentPenCapStyle);
    penJoinStyleDrawList.append(currentPenJoinStyle);

}

DrawZone::~DrawZone()
{
    delete currentPointA;
    delete currentPointB;
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
            if(lineDrawList.at(i).first.x()!=-1){
                pen.setWidth(penWidthDrawList.at(i));
                pen.setColor(penColorDrawList.at(i));
                pen.setCapStyle(penCapStyleDrawList.at(i));
                painter.setPen(pen);
                painter.drawLine(lineDrawList.at(i).first,lineDrawList.at(i).second);
            }
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
        update();
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        lineDrawList[indexCurrent].second=e->pos();
        lineNotDrawn=0;
        this->expandDrawList();
        update();
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

void DrawZone::setCurrentPenCapStyle(int n)
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
    emit penCapStyleChanged(n);
}

void DrawZone::setCurrentPenJoinStyle(int n)
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
    emit penJoinStyleChanged(n);
}

void DrawZone::setCurrentPenCapStyle(QAction * action)
{

    QString s=action->text();
    int i;
    if(s=="Square Cap") {setCurrentPenCapStyle(Qt::PenCapStyle::SquareCap); i=0;}
    else if(s=="Flat Cap") {setCurrentPenCapStyle(Qt::PenCapStyle::FlatCap); i=1;}
    else {setCurrentPenCapStyle(Qt::PenCapStyle::RoundCap); i=2;}
    emit penCapStyleChanged(i);
}

void DrawZone::setCurrentPenJoinStyle(QAction * action)
{

    QString s=action->text();
    int i;
    if(s=="Bevel Join") {setCurrentPenJoinStyle(Qt::PenJoinStyle::BevelJoin); i=0;}
    else if(s=="Miter Join") {setCurrentPenJoinStyle(Qt::PenJoinStyle::MiterJoin); i=1;}
    else {setCurrentPenJoinStyle(Qt::PenJoinStyle::RoundJoin); i=2;}
    emit penJoinStyleChanged(i);
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
