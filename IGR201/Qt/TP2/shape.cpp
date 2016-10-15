#include "shape.h"

Shape::Shape()
{

}
Shape::Shape(DrawZone *parent,
             QColor currentColor,
             int currentSize,
             Qt::PenCapStyle currentPenCapStyle,
             Qt::PenJoinStyle currentPenJoinStyle,
             bool filling,
             Qt::FillRule currentFillRule)
{

    this->parent=parent;
    this->color=currentColor;
    this->size=currentSize;
    this->capStyle=currentPenCapStyle;
    this->joinStyle=currentPenJoinStyle;
    this->fill=filling;
    this->fillRule=currentFillRule;
}

Shape::~Shape(){

}

void Shape::setColor(QColor color)
{
    this->color=color;
}
void Shape::setSize(int size)
{
    this->size=size;
}
void Shape::setCapStyle(Qt::PenCapStyle pcs)
{
    this->capStyle=pcs;
}
void Shape::setJoinStyle(Qt::PenJoinStyle pjs)
{

    this->joinStyle=pjs;
}
void Shape::toggleFilling(bool b)
{
    this->fill=b;
}
void Shape::setFillRule(Qt::FillRule fr)
{
    path.setFillRule(fr);
    this->fillRule=fr;
}

QPainterPath Shape::getPath()
{
    return path;
}
QColor Shape::getColor()
{
    return color;
}
int Shape::getSize()
{
    return size;
}
Qt::PenCapStyle Shape::getCapStyle()
{
    return capStyle;
}
Qt::PenJoinStyle Shape::getJoinStyle()
{
    return joinStyle;
}
bool Shape::getFilling()
{
    return fill;
}
Qt::FillRule Shape::getFillRule()
{
    return fillRule;
}

void Shape::addLine(QPointF A, QPointF B)
{
    path.moveTo(A);
    path.lineTo(B);
}
void Shape::addEllipse(QPointF A, QPointF B)
{

    path.moveTo(A);
    path.QPainterPath::addEllipse(QRectF(A,B));
}
void Shape::addRectangle(QPointF A, QPointF B)
{

    path.moveTo(A);
    path.addRect(QRectF(A,B));
}

void Shape::startFreePath(QPointF A)
{

    path.moveTo(A);
}
void Shape::addFreeDraw(QPointF A)
{

    path.lineTo(A);
}

void Shape::startPolygon(QPointF A)
{

    path.moveTo(A);
    pathTemp=path;
}
void Shape::addSegment(QPointF A)
{

    pathTemp.lineTo(A);
    path=pathTemp;
}
void Shape::previewPolygon(QPointF A, QPointF B)
{
    path=pathTemp;
    path.moveTo(A);
    path.lineTo(B);
}

void Shape::endPolygon(QPointF start)
{

    addSegment(start);
}

int Shape::isShapeSelected(QPointF A)
{

    QRectF rect = QRectF(QPointF(A.x()-2,A.y()-2),QPointF(A.x()+2,A.y()+2));
    if(!fill)
    {
        QPainterPathStroker stroke;
        QPainterPath qpath = stroke.createStroke(path);
        if(qpath.intersects(rect)) return 1;
    }
    else
    {
        if(path.intersects(rect)) return 1;
    }
    return 0;
}
void Shape::translate(float dx, float dy)
{

    path.translate(dx, dy);
}

void Shape::clear()
{

    path=QPainterPath();
}
void Shape::clearLine(QPointF A, QPointF B)
{
    QPainterPath pathToSubstract=QPainterPath();
    pathToSubstract.moveTo(A);
    pathToSubstract.lineTo(B);
    QPainterPath intersection=path.intersected(pathToSubstract);
    path=path.subtracted(pathToSubstract);
    path.addPath(intersection);
}
