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
#include <QVector>
#include <Qt>
#include <QFileDialog>
#include <QMessageBox>
#include "shape.h"

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
    QPointF lastPointB;
    QColor fillingColor;
    int movingShape;
    bool isAAOn;
    int currentMode;

    int indexCurrent;
    QColor currentPenColor;
    int currentPenSize;
    Qt::PenCapStyle currentPenCapStyle;
    Qt::PenJoinStyle currentPenJoinStyle;
    bool filling;
    Qt::FillRule currentFillingRule;
    int currentShapeType;

    QVector<Shape > shapeDrawList;
    Shape * currentShape;
    Shape * shapeSelected;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void setCurrentPenJoinStyle(Qt::PenJoinStyle);
    void setCurrentPenCapStyle(Qt::PenCapStyle);
    void expandDrawList();
    int reduceDrawList();
    void findShapeSelected(QPointF);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:
    void drawZoneClicked();
    void penCapStyleChanged(int);
    void penJoinStyleChanged(int);
    void shapeStyleChanged(int);
    void currentModeChanged(int);
    void AAStateChanged(bool);
    void fillingStateChanged(bool);
    void fillingRuleChanged(int);

public slots:
    void setCurrentPenColor(QColor);
    void setCurrentPenWidth(int);
    void setCurrentPenCapStyle(int);
    void setCurrentPenCapStyle(QAction *);
    void setCurrentPenJoinStyle(int);
    void setCurrentPenJoinStyle(QAction *);
    void setCurrentObjectToDraw(QAction *);
    void setCurrentObjectToDraw(int);
    void setFillingColor(QColor);
    void setCurrentMode(int);
    void setCurrentMode(QAction *);
    void setDrawable(int);
    void setFillingRule(Qt::FillRule);
    void setFillingRule(int);

    void fillDrawZone(QColor);
    void toggleAA(bool);
    void toggleFilling(bool);
    void emptyDrawList();
    void clearSelectedElement();
    void cancel();
    void saveDrawList();
    void openDrawList();

};

#endif // DRAWZONE_H
