#include "shape.h"

Shape::Shape()
{

}
Shape::Shape(QColor currentColor,
             int currentSize,
             Qt::PenCapStyle currentPenCapStyle,
             Qt::PenJoinStyle currentPenJoinStyle,
             bool filling,
             Qt::FillRule currentFillRule)
{

    this->color=currentColor;
    this->size=currentSize;

    if(currentPenCapStyle==Qt::SquareCap) capStyle=0;
    if(currentPenCapStyle==Qt::FlatCap) capStyle=1;
    if(currentPenCapStyle==Qt::RoundCap) capStyle=2;

    if(currentPenJoinStyle==Qt::BevelJoin) joinStyle=0;
    if(currentPenJoinStyle==Qt::MiterJoin) joinStyle=1;
    if(currentPenJoinStyle==Qt::RoundJoin) joinStyle=2;

    this->capStyle=currentPenCapStyle;
    this->joinStyle=currentPenJoinStyle;
    this->fill=filling;

    if(currentFillRule==Qt::OddEvenFill) fillRule=0;
    if(currentFillRule==Qt::WindingFill) fillRule=1;
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
void Shape::setCapStyle(Qt::PenCapStyle currentPenCapStyle)
{

    if(currentPenCapStyle==Qt::SquareCap) capStyle=0;
    if(currentPenCapStyle==Qt::FlatCap) capStyle=1;
    if(currentPenCapStyle==Qt::RoundCap) capStyle=2;
}
void Shape::setJoinStyle(Qt::PenJoinStyle currentPenJoinStyle)
{

    if(currentPenJoinStyle==Qt::BevelJoin) joinStyle=0;
    if(currentPenJoinStyle==Qt::MiterJoin) joinStyle=1;
    if(currentPenJoinStyle==Qt::RoundJoin) joinStyle=2;
}
void Shape::toggleFilling(bool b)
{
    this->fill=b;
}
void Shape::setFillRule(Qt::FillRule currentFillRule)
{
    if(currentFillRule==Qt::OddEvenFill) fillRule=0;
    if(currentFillRule==Qt::WindingFill) fillRule=1;
    path.setFillRule(currentFillRule);
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
    if(capStyle==0) return Qt::SquareCap;
    if(capStyle==1) return Qt::FlatCap;
    if(capStyle==2) return Qt::RoundCap;
}
Qt::PenJoinStyle Shape::getJoinStyle()
{
    if(joinStyle==0) return Qt::BevelJoin;
    if(joinStyle==1) return Qt::MiterJoin;
    if(joinStyle==2) return Qt::RoundJoin;
}
bool Shape::getFilling()
{
    return fill;
}
Qt::FillRule Shape::getFillRule()
{
    if(fillRule==0) return Qt::OddEvenFill;
    else return Qt::WindingFill;
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

QDataStream & operator << (QDataStream & out, const Shape & Valeur)
{

    out << Valeur.path
        << Valeur.color
        << Valeur.size
        << Valeur.capStyle
        << Valeur.joinStyle
        << Valeur.fillRule
        << Valeur.fill;

    return out;
}

QDataStream & operator >> (QDataStream & in, Shape & Valeur)
{

    in >> Valeur.path
       >> Valeur.color
       >> Valeur.size
       >> Valeur.capStyle
       >> Valeur.joinStyle
       >> Valeur.fillRule
       >> Valeur.fill;

    return in;
}

void Shape::initShapeSystem()
{

    qRegisterMetaTypeStreamOperators<Shape>("Shape");
    qMetaTypeId<Shape>();
}
