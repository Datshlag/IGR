#ifndef GESTUREDRAWER_H
#define GESTUREDRAWER_H

#include <QWidget>
#include <QPolygonF>
#include <QPainter>
#include <QPushButton>
#include <qmath.h>

class GestureDrawer : public QWidget
{
    Q_OBJECT
public:
    explicit GestureDrawer(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    void clear(){ m_stroke.clear(); }

    QPolygonF getStroke(){ return m_stroke;}

    const QSize sizeHint() const;
    const QSize minimumSizeHint()const;


public slots:
    void setStroke(QPolygonF p){ m_stroke = p;}

protected:
    void drawWidget(QPainter&);
    QPolygonF m_stroke;

};

#endif // GESTUREDRAWER_H
