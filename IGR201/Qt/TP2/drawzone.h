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

class DrawZone : public QWidget
{
    Q_OBJECT

public:
    DrawZone(QWidget*);
    ~DrawZone();

private:
    QWidget* parent;
    int drawable;

    QPoint* currentPointA;
    QPoint* currentPointB;
    QColor currentPenColor;
    QColor fillingColor;
    int currentPenWidth;
    Qt::PenCapStyle currentPenCapStyle;
    Qt::PenJoinStyle currentPenJoinStyle;
    int indexCurrent;

    QVector<QPair<QPoint, QPoint>> lineDrawList;
    QVector<int> penWidthDrawList;
    QVector<QColor> penColorDrawList;
    QVector<Qt::PenCapStyle> penCapStyleDrawList;
    QVector<Qt::PenJoinStyle> penJoinStyleDrawList;

    int lineNotDrawn=0;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

protected:
    virtual void paintEvent(QPaintEvent*);

signals:
    void drawZoneClicked();

public slots:
    void setCurrentPenColor(QColor);
    void setCurrentPenWidth(int);
    void setCurrentPenCapStyle(Qt::PenCapStyle);
    void setCurrentPenJoinStyle(Qt::PenJoinStyle);
    void setCurrentPenCapStyleFromInt(int);
    void setCurrentPenJoinStyleFromInt(int);
    void expandDrawList();
    void setFillingColor(QColor);
    void fillDrawZone(QColor);
    void setDrawable(int);

};

#endif // DRAWZONE_H
