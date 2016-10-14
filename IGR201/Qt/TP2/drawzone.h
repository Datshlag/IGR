#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <QColor>
#include <QObject>
#include <QDebug>
#include <QAction>
#include <Qt>

class DrawZone : public QWidget
{
    Q_OBJECT

public:
    DrawZone(QWidget*);
    ~DrawZone();

private:
    QWidget* parent;
    int drawable;
    int drawingShape;
    int drawingPolygon;

    QPointF currentPointA;
    QPointF currentPointB;
    int currentRadius;

    int itemSelected;
    int movingShape;

    QColor currentPenColor;
    QColor fillingColor;
    int currentPenWidth;
    Qt::PenCapStyle currentPenCapStyle;
    Qt::PenJoinStyle currentPenJoinStyle;
    int indexCurrent;
    int objectType;

    int drawingMode;
    int selectingMode;

    QVector<QPair<QPoint, QPoint> > lineDrawList;
    QVector<QPainterPath > objectsDrawList;
    QVector<int> penWidthDrawList;
    QVector<QColor> penColorDrawList;
    QVector<Qt::PenCapStyle> penCapStyleDrawList;
    QVector<Qt::PenJoinStyle> penJoinStyleDrawList;

    int lineNotDrawn;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:
    void drawZoneClicked();
    void penCapStyleChanged(int);
    void penJoinStyleChanged(int);
    void penCapStyleChanged(QString);
    void penJoinStyleChanged(QString);

public slots:
    void setCurrentPenColor(QColor);
    void setCurrentPenWidth(int);
    void setCurrentPenCapStyle(int);
    void setCurrentPenCapStyle(QAction *);
    void setCurrentPenJoinStyle(int);
    void setCurrentPenJoinStyle(QAction *);
    void setObjectToDraw(QAction *);
    void setFillingColor(QColor);
    void fillDrawZone(QColor);
    void setDrawable(int);
    void setEditionMode(int);
    void setEditionMode(QAction *);


private slots:
    void setCurrentPenJoinStyle(Qt::PenJoinStyle);
    void setCurrentPenCapStyle(Qt::PenCapStyle);
    void drawLine(QPointF, QPointF);
    void drawEllipse(QPointF, QPointF);
    void drawRectangle(QPointF, QPointF);
    void startFreePath(QPointF);
    void addFreeDraw(QPointF);
    void startPolygon(QPointF);
    void addSegment(QPointF);
    void endPolygon(QPointF);
    void expandDrawList();
    int reduceDrawList();
    void emptyDrawList();
    int findShapeSelected(QPointF);
    void clearLastElement();
    void translateShape(int, float, float);

};

#endif // DRAWZONE_H
