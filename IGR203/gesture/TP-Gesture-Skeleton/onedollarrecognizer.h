#ifndef ONEDOLLARRECOGNIZER_H
#define ONEDOLLARRECOGNIZER_H

#include <QGestureRecognizer>
#include <QObject>
#include "onedollargesture.h"
#include "onedollarutil.h"

class OneDollarRecognizer : public QGestureRecognizer, public OneDollarUtil
{

public:
    explicit OneDollarRecognizer();
    QGesture *create(QObject * target);
    QGestureRecognizer::Result recognize(QGesture * gesture, QObject * watched, QEvent * event);
    void reset(QGesture*);

};

#endif // ONEDOLLARRECOGNIZER_H
