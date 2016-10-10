#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent)
{

    this->parent=parent;
    drawable=1;
    fillingColor = Qt::white;

    indexCurrent=0;
    lineNotDrawn=0;
    currentRadius=0;

    currentPenCapStyle=Qt::SquareCap;
    currentPenJoinStyle=Qt::BevelJoin;
    currentPenWidth=0;
    currentPenColor=Qt::black;
    currentPointA = QPoint(-1,-1);
    currentPointB = QPoint(-1,-1);

    penWidthDrawList.append(currentPenWidth);
    penColorDrawList.append(currentPenColor);
    penCapStyleDrawList.append(currentPenCapStyle);
    penJoinStyleDrawList.append(currentPenJoinStyle);
    objectsDrawList.append(QPainterPath());

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
            painter.drawPath(objectsDrawList.at(i));

        }
    }
}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        currentPointA=e->pos();
        currentPointB=e->pos();
        this->drawLine(currentPointA,currentPointB);
        lineNotDrawn=1;
        update();
    }

    emit drawZoneClicked();
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(lineNotDrawn){
        currentPointB=e->pos();
        this->drawLine(currentPointA,currentPointB);
        update();
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        currentPointB=e->pos();
        this->drawLine(currentPointA,currentPointB);
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
            this->setCurrentPenCapStyle(Qt::SquareCap);
            break;
        case(1):
            this->setCurrentPenCapStyle(Qt::FlatCap);
            break;
        case(2):
            this->setCurrentPenCapStyle(Qt::RoundCap);
    }
    emit penCapStyleChanged(n);
}

void DrawZone::setCurrentPenJoinStyle(int n)
{

    switch(n){
        case(0):
            this->setCurrentPenJoinStyle(Qt::BevelJoin);
            break;
        case(1):
            this->setCurrentPenJoinStyle(Qt::MiterJoin);
            break;
        case(2):
            this->setCurrentPenJoinStyle(Qt::RoundJoin);
    }
    emit penJoinStyleChanged(n);
}

void DrawZone::setCurrentPenCapStyle(QAction * action)
{

    QString s=action->text();
    int i;
    if(s=="Square Cap") {setCurrentPenCapStyle(Qt::SquareCap); i=0;}
    else if(s=="Flat Cap") {setCurrentPenCapStyle(Qt::FlatCap); i=1;}
    else {setCurrentPenCapStyle(Qt::RoundCap); i=2;}
    emit penCapStyleChanged(i);
}

void DrawZone::setCurrentPenJoinStyle(QAction * action)
{

    QString s=action->text();
    int i;
    if(s=="Bevel Join") {setCurrentPenJoinStyle(Qt::BevelJoin); i=0;}
    else if(s=="Miter Join") {setCurrentPenJoinStyle(Qt::MiterJoin); i=1;}
    else {setCurrentPenJoinStyle(Qt::RoundJoin); i=2;}
    emit penJoinStyleChanged(i);
}

void DrawZone::expandDrawList()
{

    indexCurrent++;
    objectsDrawList.append(QPainterPath());
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

void DrawZone::drawCircle(QPoint center, int r)
{

    QPainterPath painter = QPainterPath();
    painter.moveTo(center);
    painter.addEllipse(QRect(QPoint(center.x()-r,center.y()-r),QPoint(center.x()+r,center.y()+r)));
    objectsDrawList[indexCurrent]=painter;

}

void DrawZone::drawLine(QPoint A, QPoint B)
{
    QPainterPath painter=QPainterPath();
    painter.moveTo(QPointF(A));
    painter.lineTo(QPointF(B));
    objectsDrawList[indexCurrent]=painter;

}
