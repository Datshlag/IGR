#ifndef SHAPE_H
#define SHAPE_H

#include <Qt>
#include <QPainterPath>
#include <QColor>

class DrawZone;

class Shape
{

public:
    Shape();
    Shape(DrawZone*, QColor, int, Qt::PenCapStyle,Qt::PenJoinStyle, bool, Qt::FillRule);
    ~Shape();

private:
    QPainterPath path;
    QPainterPath pathTemp;
    DrawZone* parent;
    QColor color;
    int size;
    Qt::PenCapStyle capStyle;
    Qt::PenJoinStyle joinStyle;
    Qt::FillRule fillRule;
    bool fill;
    int shapeType;

public slots:
    void setColor(QColor);
    void setSize(int);
    void setCapStyle(Qt::PenCapStyle);
    void setJoinStyle(Qt::PenJoinStyle);
    void toggleFilling(bool);
    void setFillRule(Qt::FillRule);

    QPainterPath getPath();
    QColor getColor();
    int getSize();
    Qt::PenCapStyle getCapStyle();
    Qt::PenJoinStyle getJoinStyle();
    bool getFilling();
    Qt::FillRule getFillRule();

    void addLine(QPointF, QPointF);
    void addEllipse(QPointF, QPointF);
    void addRectangle(QPointF, QPointF);

    void startFreePath(QPointF);
    void addFreeDraw(QPointF);

    void startPolygon(QPointF);
    void addSegment(QPointF);
    void previewPolygon(QPointF,QPointF);
    void endPolygon(QPointF);

    int isShapeSelected(QPointF);
    void translate(float, float);

    void clear();
    void clearLine(QPointF,QPointF);

};

#endif // SHAPE_H
