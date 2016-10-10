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
    int drawingLine;
    int drawingCircle;

    QPoint currentPointA;
    QPoint currentPointB;
    int currentRadius;

    QColor currentPenColor;
    QColor fillingColor;
    int currentPenWidth;
    Qt::PenCapStyle currentPenCapStyle;
    Qt::PenJoinStyle currentPenJoinStyle;
    int indexCurrent;

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
    void expandDrawList();
    void setFillingColor(QColor);
    void fillDrawZone(QColor);
    void setDrawable(int);

private slots:
    void setCurrentPenJoinStyle(Qt::PenJoinStyle);
    void setCurrentPenCapStyle(Qt::PenCapStyle);
    void drawLine(QPoint, QPoint);
    void drawCircle(QPoint, int);

};

#endif // DRAWZONE_H
