#include "gesturedrawer.h"
#include <QPainter>

GestureDrawer::GestureDrawer(QWidget *parent) :
    QWidget(parent)
{
}

void GestureDrawer::drawWidget(QPainter &p) {

    p.fillRect(0,0,width(),height(), Qt::white);
    p.drawPolyline(getStroke());
}

void GestureDrawer::paintEvent(QPaintEvent *e) {

    QPainter p(this);
    drawWidget(p);
}

const QSize GestureDrawer::sizeHint() const {


}

const QSize GestureDrawer::minimumSizeHint() const {

    return sizeHint();
}
