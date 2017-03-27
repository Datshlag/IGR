#include "onedollarutil.h"
#include <QLineF>

OneDollarUtil::OneDollarUtil()
{
    anglePrecision = 2.0;
    //--- A magic number used in pre-processing the symbols
    goldenRatio    = 0.5 * (-1.0 + qSqrt(5.0));

}

GestureTemplate OneDollarUtil::recognizeStroke(QPolygonF poly, float* score){
    *score = 0;
    GestureTemplate winner;
    //...
    return winner;

}


float OneDollarUtil::distanceAtBestAngle(QPolygonF curGesture, QPolygonF aTemplate) {
    float angleRange = templates.at(0).angleRange;
    float startRange = -angleRange;
    float endRange   =  angleRange;
    float x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
    float f1 = distanceAtAngle(curGesture, aTemplate, x1);
    float x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
    float f2 = distanceAtAngle(curGesture, aTemplate, x2);

    while (abs((long)(endRange - startRange)) > anglePrecision)
    {
        if (f1 < f2)
        {
            //cout << "evaluation pour angle " << x1 << endl;
            endRange = x2;
            x2 = x1;
            f2 = f1;
            x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
            f1 = distanceAtAngle(curGesture, aTemplate, x1);
        }
        else
        {
            //cout << "evaluation pour angle " << x2 << endl;
            startRange = x1;
            x1 = x2;
            f1 = f2;
            x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
            f2 = distanceAtAngle(curGesture, aTemplate, x2);
        }
    }
    double bestAngle;
    if(f1<f2)
        bestAngle = x1;
    else
        bestAngle = x2;
    //cout << "best angle " << bestAngle*180/3.14 << endl;
    return qMin(f1, f2);
}

float OneDollarUtil::distanceAtAngle(QPolygonF curGesture, QPolygonF aTemplate, float rotation){
    GestureTemplate tmp;
    QPolygonF newPoints = tmp.rotateBy(curGesture, rotation);
    float d =  pathDistance(newPoints, aTemplate);
    return d;
}


float OneDollarUtil::pathDistance(QPolygonF p1, QPolygonF p2){
       // assumes pts1.size == pts2.size
       float distance = 0.0;
       for (int i = 0; i < p1.size(); i++)
           distance += QLineF(p1.at(i),p2.at(i)).length();
       return (distance / p1.size());
   }

void OneDollarUtil::setTemplates(const QVector<GestureTemplate> & _templates){
    templates = _templates;
}
