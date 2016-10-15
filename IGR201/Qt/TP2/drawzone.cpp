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

    currentMode=0;
    movingShape=0;
    isAAOn=true;
    filling=false;

    currentPenCapStyle=Qt::SquareCap;
    currentPenJoinStyle=Qt::BevelJoin;
    currentFillingRule=Qt::OddEvenFill;
    currentPenSize=0;
    currentPenColor=Qt::black;
    currentPointA = QPointF(-1,-1);
    currentPointB = QPointF(-1,-1);

    currentShapeType=0;

    shapeDrawList.append(Shape(this,
                               currentPenColor,
                               currentPenSize,
                               currentPenCapStyle,
                               currentPenJoinStyle,
                               filling,
                               currentFillingRule));
    currentShape=&(shapeDrawList[0]);

}

DrawZone::~DrawZone()
{

}

void DrawZone::paintEvent(QPaintEvent *e)
{

    QWidget::paintEvent(e);
    QPainter painter(this);

    if(isAAOn) painter.setRenderHint(QPainter::Antialiasing);

    QRect rectangle;
    rectangle.setCoords(-1,-1,this->width(),this->height());
    QBrush brush(fillingColor);
    painter.setBrush(brush);
    painter.drawRect(rectangle);

    QPen pen;
    for(int i=0; i<=shapeDrawList.length()-1; i++)
    {

        pen.setWidth(shapeDrawList[i].getSize());
        pen.setColor(shapeDrawList[i].getColor());
        pen.setCapStyle(shapeDrawList[i].getCapStyle());
        pen.setJoinStyle(shapeDrawList[i].getJoinStyle());
        if(shapeDrawList[i].getFilling()) painter.setBrush(QBrush(shapeDrawList[i].getColor()));
        else painter.setBrush(QBrush(Qt::transparent));
        painter.setPen(pen);
        painter.drawPath(shapeDrawList[i].getPath());

    }
}

void DrawZone::mousePressEvent(QMouseEvent * e)
{
    if(e->button()==Qt::LeftButton){

        if(currentMode==0)
        {
            if(currentShapeType==0)
            {

                currentPointA=e->pos();
                currentPointB=e->pos();
                currentShape->clear();
                currentShape->addLine(e->pos(),e->pos());
                lineNotDrawn=1;
                drawingShape=1;
                update();
            }

            if(currentShapeType==1||currentShapeType==2)
            {

                drawingShape=1;
            }

            if(currentShapeType==3)
            {

                currentPointA=e->pos();
                currentShape->clear();
                currentShape->addEllipse(e->pos(),e->pos());
                drawingShape=1;
                update();
            }

            if(currentShapeType==4)
            {

                currentPointA=e->pos();
                currentShape->clear();
                currentShape->addRectangle(e->pos(),e->pos());
                drawingShape=1;
                update();
            }

            if(currentShapeType==5)
            {

                currentPointA=e->pos();
                //this->startFreePath(currentPointA);
                currentShape->startFreePath(e->pos());
                update();
                drawingShape=1;
            }
        }
        if(currentMode==1)
        {

            currentPointA=e->pos();
            currentPointB=e->pos();
            findShapeSelected(currentPointA);
            if(shapeSelected!=NULL)
            {

                movingShape=1;

            }
        }
    }
    else if(e->button()==Qt::RightButton)
    {

        if(drawingPolygon)
        {

            if(currentShapeType==2) currentShape->endPolygon(currentPointA);
            if(currentShapeType==1) currentShape->endPolygon(e->pos());
            drawingPolygon=0;
            drawingShape=0;
            update();
            expandDrawList();
        }
    }

    emit drawZoneClicked();
}

void DrawZone::mouseMoveEvent(QMouseEvent * e)
{
    if(currentMode==0)
    {
        if(drawingShape)
        {
            if(currentShapeType==0)
            {

                currentPointB=e->pos();
                currentShape->clear();
                currentShape->addLine(currentPointA,currentPointB);
                update();
            }

            if(currentShapeType==1||currentShapeType==2)
            {

                if(drawingPolygon)
                {

                    currentShape->previewPolygon(currentPointB, e->pos());
                    update();
                }
            }

            if(currentShapeType==3)
            {

                currentPointB=e->pos();
                currentShape->clear();
                currentShape->addEllipse(currentPointA,currentPointB);
                update();
            }

            if(currentShapeType==4)
            {

                currentPointB=e->pos();
                currentShape->clear();
                currentShape->addRectangle(currentPointA,currentPointB);
                update();
            }

            if(currentShapeType==5)
            {

                currentPointA=e->pos();
                currentShape->addFreeDraw(currentPointA);
                currentPointA=e->pos();
                update();
            }
        }
    }
    if(currentMode==1)
    {

        if(movingShape)
        {

            shapeSelected->translate(e->pos().x()-currentPointB.x(), e->pos().y()-currentPointB.y());
            currentPointB=e->pos();
            update();
        }
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent * e)
{
    if(currentMode==0)
    {
        if(drawingShape)
        {
            if(e->button()==Qt::LeftButton)
            {
                if(currentShapeType==0)
                {
                    currentPointB=e->pos();
                    currentShape->clear();
                    currentShape->addLine(currentPointA,currentPointB);
                    lineNotDrawn=0;
                    drawingShape=0;
                    update();
                    this->expandDrawList();
                }

                if(currentShapeType==1||currentShapeType==2)
                {
                    if(!drawingPolygon)
                    {
                        currentPointA=e->pos();
                        currentPointB=e->pos();
                        currentShape->startPolygon(currentPointA);
                        drawingPolygon=1;
                    }
                    else
                    {

                        currentShape->addSegment(e->pos());
                        currentPointB=e->pos();
                    }

                    update();
                }

                if(currentShapeType==3)
                {

                    currentPointB=e->pos();
                    currentShape->clear();
                    currentShape->addEllipse(currentPointA,currentPointB);
                    drawingShape=0;
                    update();
                    this->expandDrawList();
                }

                if(currentShapeType==4)
                {
                    currentPointB=e->pos();
                    currentShape->clear();
                    currentShape->addRectangle(currentPointA,currentPointB);
                    drawingShape=0;
                    update();
                    this->expandDrawList();
                }

                if(currentShapeType==5)
                {

                    currentPointA=e->pos();
                    currentShape->addFreeDraw(currentPointA);
                    drawingShape=0;
                    update();
                    this->expandDrawList();
                }
            }
        }
    }
    if(currentMode==1)
    {

        movingShape=0;
    }
}


void DrawZone::setCurrentPenColor(QColor color)
{

    currentPenColor=color;
    currentShape->setColor(color);
}

void DrawZone::setCurrentPenWidth(int n)
{

    currentPenSize=n;
    currentShape->setSize(n);
}

void DrawZone::setCurrentPenCapStyle(Qt::PenCapStyle pcs)
{

    //penCapStyleDrawList[indexCurrent]=pcs;
    currentPenCapStyle=pcs;
    currentShape->setCapStyle(pcs);
}

void DrawZone::setCurrentPenJoinStyle(Qt::PenJoinStyle pjs)
{

    currentPenJoinStyle=pjs;
    currentShape->setJoinStyle(pjs);
}

void DrawZone::setCurrentPenCapStyle(int n)
{

    switch(n){
        case(0):
            setCurrentPenCapStyle(Qt::SquareCap);
            break;
        case(1):
            setCurrentPenCapStyle(Qt::FlatCap);
            break;
        case(2):
            setCurrentPenCapStyle(Qt::RoundCap);
    }
    emit penCapStyleChanged(n);
}

void DrawZone::setCurrentPenJoinStyle(int n)
{

    switch(n){
        case(0):
            setCurrentPenJoinStyle(Qt::BevelJoin);
            break;
        case(1):
            setCurrentPenJoinStyle(Qt::MiterJoin);
            break;
        case(2):
            setCurrentPenJoinStyle(Qt::RoundJoin);
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

void DrawZone::toggleFilling(bool b)
{

    filling=b;
    currentShape->toggleFilling(b);
    emit fillingStateChanged(b);
}

void DrawZone::setFillingRule(int n)
{

    if(n==0) setFillingRule(Qt::OddEvenFill);
    else if(n==1) setFillingRule(Qt::WindingFill);
    qDebug()<<n;
}

void DrawZone::setFillingRule(Qt::FillRule q)
{

    currentFillingRule=q;
    currentShape->setFillRule(q);
}

void DrawZone::setDrawable(int n)
{

    drawable=n;
}

void DrawZone::setFillingColor(QColor color)
{

    fillingColor=color;
}

void DrawZone::setCurrentObjectToDraw(QAction * action)
{

    QString s=action->text();
    if(s=="Line")
    {
        currentShapeType=0;
    }
    else if(s=="Poly Line")
    {
        currentShapeType=1;
    }
    else if(s=="Polygon")
    {
        currentShapeType=2;
    }
    else if(s=="Ellipse")
    {
        currentShapeType=3;
    }
    else if(s=="Rectangle")
    {
        currentShapeType=4;
    }
    else if(s=="Free Draw")
    {
        currentShapeType=5;
    }

    emit shapeStyleChanged(currentShapeType);
}

void DrawZone::setCurrentObjectToDraw(int n)
{

    currentShapeType=n;
    emit shapeStyleChanged(n);
}

void DrawZone::setCurrentMode(QAction * q)
{

    QString s(q->text());
    if(s=="Draw")
    {

        currentMode=0;
    }
    else
    {

        currentMode=1;
    }
    emit currentModeChanged(currentMode);
}

void DrawZone::setCurrentMode(int n)
{

    currentMode=n;
    emit currentModeChanged(currentMode);
}

void DrawZone::toggleAA(bool n)
{

    isAAOn=n;
    update();
    emit AAStateChanged(isAAOn);
}


void DrawZone::expandDrawList()
{

    shapeDrawList.append(Shape(this,
                               currentPenColor,
                               currentPenSize,
                               currentPenCapStyle,
                               currentPenJoinStyle,
                               filling,
                               currentFillingRule
                               ));
    currentShape=&shapeDrawList.last();
}

int DrawZone::reduceDrawList()
{

    if(indexCurrent>0)
    {
        currentShape=&shapeDrawList[shapeDrawList.length()-1];
        shapeDrawList.pop_back();
        return 0;
    }
    return -1;
}

void DrawZone::emptyDrawList()
{

    while(this->reduceDrawList()!=-1);
    expandDrawList();
}

void DrawZone::cancel()
{

    currentShape->clear();
}

void DrawZone::fillDrawZone(QColor color)
{

    setFillingColor(color);
    update();
}

void DrawZone::findShapeSelected(QPointF A)
{
    shapeSelected=NULL;
    for(int i=0; i<shapeDrawList.length()-1; i++)
    {
        if(shapeDrawList[i].isShapeSelected(A)) shapeSelected=&shapeDrawList[i];
    }
}

