#ifndef SHAPE_H
#define SHAPE_H

#include <Qt>
#include <QPainterPath>
#include <QColor>
#include <QVariant>
#include <QDataStream>

class DrawZone;

class Shape
{

public:
    Shape();
    Shape( QColor, int, Qt::PenCapStyle,Qt::PenJoinStyle, bool, Qt::FillRule);
    ~Shape();
    static void initShapeSystem();

private:
    QPainterPath path;
    QPainterPath pathTemp;
    QColor color;
    int size;
    int capStyle;
    int joinStyle;
    int fillRule;
    bool fill;

    friend QDataStream & operator << (QDataStream & out, const Shape & valeur);
    friend QDataStream & operator >> (QDataStream & in, Shape & Valeur);


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

Q_DECLARE_METATYPE(Shape)
QDataStream & operator << (QDataStream & out, const Shape & valeur);
QDataStream & operator >> (QDataStream & in, Shape & Valeur);

#endif // SHAPE_H
