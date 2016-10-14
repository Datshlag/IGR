#include "drawzone.h"

int i=0;

DrawZone::DrawZone(QWidget *parent)
{

    this->parent=parent;
    drawable=1;
    fillingColor = Qt::white;

    indexCurrent=0;
    lineNotDrawn=0;
    drawingShape=0;
    currentRadius=0;

    drawingMode=1;
    selectingMode=0;
    movingShape=0;

    currentPenCapStyle=Qt::SquareCap;
    currentPenJoinStyle=Qt::BevelJoin;
    currentPenWidth=0;
    currentPenColor=Qt::black;
    currentPointA = QPointF(-1,-1);
    currentPointB = QPointF(-1,-1);

    objectType=0;

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

    QPen pen;
    for(int i=0; i<=indexCurrent; i++)
    {
        pen.setWidth(penWidthDrawList.at(i));
        pen.setColor(penColorDrawList.at(i));
        pen.setCapStyle(penCapStyleDrawList.at(i));
        pen.setJoinStyle(penJoinStyleDrawList.at(i));
        //painter.setBrush(QBrush(penColorDrawList.at(i)));
        painter.setBrush(QColor(0, 0, 0, 0));
        painter.setPen(pen);
        painter.drawPath(objectsDrawList.at(i));
    }
}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        if(drawingMode)
        {
            if(objectType==0)
            {

                currentPointA=e->pos();
                currentPointB=e->pos();
                this->drawLine(currentPointA,currentPointB);
                lineNotDrawn=1;
                update();
                drawingShape=1;
            }

            if(objectType==1)
            {

                currentPointA=e->pos();
                this->drawEllipse(currentPointA,currentPointA);
                update();
                drawingShape=1;
            }

            if(objectType==2)
            {

                currentPointA=e->pos();
                this->drawRectangle(currentPointA,currentPointA);
                update();
                drawingShape=1;
            }

            if(objectType==3)
            {

                drawingShape=1;
            }

            if(objectType==4)
            {

                currentPointA=e->pos();
                this->startFreePath(currentPointA);
                update();
                drawingShape=1;
            }
        }
        if(selectingMode)
        {

            currentPointA=e->pos();
            currentPointB=e->pos();
            itemSelected=findShapeSelected(currentPointA);
            if(itemSelected!=-1)
            {

                movingShape=1;

            }
        }
    }
    else if(e->button()==Qt::RightButton)
    {

        if(drawingPolygon)
        {

            this->endPolygon(currentPointA);
            this->drawLine(currentPointA, currentPointA);
            update();
            drawingPolygon=0;
            drawingShape=0;
        }
    }

    emit drawZoneClicked();
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(drawingMode)
    {
        if(drawingShape)
        {
            if(objectType==0)
            {

                currentPointB=e->pos();
                this->drawLine(currentPointA,currentPointB);
                update();
            }

            if(objectType==1)
            {

                currentPointB=e->pos();
                this->drawEllipse(currentPointA,currentPointB);
                update();
            }

            if(objectType==2)
            {

                currentPointB=e->pos();
                this->drawRectangle(currentPointA,currentPointB);
                update();
            }

            if(objectType==3)
            {
                qDebug()<<i++;
                if(drawingPolygon)
                {
                    this->drawLine(currentPointB,e->pos());
                    update();
                }
            }

            if(objectType==4)
            {

                currentPointA=e->pos();
                this->addFreeDraw(currentPointA);
                currentPointA=e->pos();
                update();
            }
        }
    }
    if(selectingMode)
    {

        if(movingShape)
        {

            this->translateShape(itemSelected, e->pos().x()-currentPointB.x(), e->pos().y()-currentPointB.y());
            currentPointB=e->pos();
            update();
        }
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(drawingMode)
    {
        if(drawingShape)
        {
            if(e->button()==Qt::LeftButton)
            {
                if(objectType==0)
                {
                    currentPointB=e->pos();
                    this->drawLine(currentPointA,currentPointB);
                    lineNotDrawn=0;
                    this->expandDrawList();
                    update();
                    drawingShape=0;
                }

                if(objectType==1)
                {

                    currentPointB=e->pos();
                    this->drawEllipse(currentPointA,currentPointB);
                    this->expandDrawList();
                    update();
                    drawingShape=0;
                }

                if(objectType==2)
                {
                    currentPointB=e->pos();
                    this->drawRectangle(currentPointA, currentPointB);
                    this->expandDrawList();
                    update();
                    drawingShape=0;
                }

                if(objectType==3)
                {
                    if(!drawingPolygon)
                    {
                        currentPointA=e->pos();
                        currentPointB=e->pos();
                        this->startPolygon(currentPointA);
                        this->expandDrawList();
                        drawingPolygon=1;
                    }
                    else
                    {

                        this->addSegment(e->pos());
                        this->clearLastElement();
                        currentPointB=e->pos();
                    }

                    update();
                }

                if(objectType==4)
                {

                    currentPointA=e->pos();
                    this->addFreeDraw(currentPointA);
                    this->expandDrawList();
                    update();
                    drawingShape=0;
                }
            }
        }
    }
    if(selectingMode)
    {

        movingShape=0;
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

int DrawZone::reduceDrawList()
{

    if(indexCurrent>0)
    {

        indexCurrent--;
        objectsDrawList.pop_back();
        penWidthDrawList.pop_back();
        penColorDrawList.pop_back();
        penCapStyleDrawList.pop_back();
        penJoinStyleDrawList.pop_back();
        return 0;
    }
    return -1;
}

void DrawZone::emptyDrawList()
{

    while(this->reduceDrawList()!=-1);
}

void DrawZone::setDrawable(int n)
{

    drawable=n;
}

void DrawZone::setFillingColor(QColor color)
{

    fillingColor=color;
}

void DrawZone::setObjectToDraw(QAction * action)
{

    QString s=action->text();
    if(s=="Line")
    {
        objectType=0;
    }
    else if(s=="Ellipse")
    {
        objectType=1;
    }
    else if(s=="Rectangle")
    {
        objectType=2;
    }
    else if(s=="Polygon")
    {
        objectType=3;
    }
    else if(s=="Free Draw")
    {
        objectType=4;
    }
    else if(s=="Poly Line")
    {
        objectType=5;
    }
}

void DrawZone::setObjectToDraw(int)
{


}

void DrawZone::fillDrawZone(QColor color)
{

    setFillingColor(color);
    update();
}

/*void DrawZone::drawCircle(QPoint center, int r)
{

    QPainterPath painter = QPainterPath();
    painter.moveTo(center);
    painter.addEllipse(QRect(QPoint(center.x()-r,center.y()-r),QPoint(center.x()+r,center.y()+r)));
    objectsDrawList[indexCurrent]=painter;
}*/

void DrawZone::drawEllipse(QPointF A, QPointF B)
{

    QPainterPath painter = QPainterPath();
    painter.moveTo(A);
    painter.addEllipse(QRectF(A,B));
    painter.setFillRule(Qt::WindingFill);
    objectsDrawList[indexCurrent]=painter;
}

void DrawZone::drawLine(QPointF A, QPointF B)
{
    QPainterPath painter=QPainterPath();
    painter.moveTo(A);
    painter.lineTo(B);
    objectsDrawList[indexCurrent]=painter;

}

void DrawZone::clearLastElement()
{

    QPainterPath path = QPainterPath();
    objectsDrawList[indexCurrent]=path;
}

void DrawZone::drawRectangle(QPointF A, QPointF B)
{

    QPainterPath path = QPainterPath();
    path.moveTo(A);
    path.addRect(QRectF(A,B));
    objectsDrawList[indexCurrent]=path;
}

void DrawZone::startFreePath(QPointF A)
{

    QPainterPath path = QPainterPath();
    path.moveTo(A);
    objectsDrawList[indexCurrent]=path;
}

void DrawZone::addFreeDraw(QPointF A)
{

    QPainterPath path = objectsDrawList[indexCurrent];
    path.lineTo(A);
    objectsDrawList[indexCurrent]=path;
}

void DrawZone::startPolygon(QPointF A)
{

    QPainterPath path = QPainterPath();
    path.moveTo(A);
    objectsDrawList[indexCurrent]=path;
}

void DrawZone::addSegment(QPointF A)
{

    QPainterPath path=objectsDrawList[indexCurrent-1];
    path.lineTo(A);
    objectsDrawList[indexCurrent-1]=path;
}

void DrawZone::endPolygon(QPointF start)
{

    this->addSegment(start);
}

int DrawZone::findShapeSelected(QPointF A)
{

    QRectF rect = QRectF(QPointF(A.x()-3,A.y()-3),QPointF(A.x()+3,A.y()+3));
    for(int i=indexCurrent; i>=0; i--)
    {

        QPainterPath path=objectsDrawList[i];
        QPainterPathStroker stroke;
        QPainterPath qpath = stroke.createStroke(path);
        if(qpath.intersects(rect)) return i;
    }
    return -1;
}

void DrawZone::translateShape(int index, float dx, float dy)
{

    objectsDrawList[index].translate(dx, dy);
}

void DrawZone::setEditionMode(int mode)
{

    if(mode==0) { drawingMode=1; selectingMode=0; }
    else if(mode==1) { drawingMode=0; selectingMode=1; }
    else { drawingMode=0; selectingMode=0 ;}
}

void DrawZone::setEditionMode(QAction * q)
{

    QString s=QString(q->text());
    if(s=="Dessin") { drawingMode=1; selectingMode=0; }
    else if(s=="Sélection") { drawingMode=0; selectingMode=1; }
    else { drawingMode=0; selectingMode=0 ;}
}
