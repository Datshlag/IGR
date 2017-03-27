#ifndef ONEDOLLARUTIL_H
#define ONEDOLLARUTIL_H
#include <QVector>
#include "gesturetemplate.h"

class OneDollarUtil
{
public:
    OneDollarUtil();

    void setTemplates(const QVector<GestureTemplate>& );

    GestureTemplate recognizeStroke(QPolygonF poly, float *score);
    float distanceAtBestAngle(QPolygonF curGesture, QPolygonF aTemplate);
    float distanceAtAngle(QPolygonF curGesture, QPolygonF aTemplate, float rotation);
    float pathDistance(QPolygonF p1, QPolygonF p2);

    QVector<GestureTemplate> templates;

    float anglePrecision;
    float goldenRatio;
};

#endif // ONEDOLLARUTIL_H
