#include "gesturetemplate.h"
#include <QLineF>
#include <qmath.h>

void GestureTemplate::init(){
    numPointsInGesture = 128;
    squareSize = 250;

    //--- 1/2 max distance across a square, which is the maximum distance
    //---  a point can be from the center of the gesture
    halfDiagonal = 0.5 * qSqrt((squareSize * squareSize) + (squareSize * squareSize));

    //--- Before matching, we rotate the symbol the user drew so that the
    //---  start point is at degree 0 (right side of symbol). That's how
    //---  the templates are rotated so it makes matching easier
    //--- Note: this assumes we want symbols to be rotation-invariant,
    //---  which we might not want. Using this, we can't tell the difference
    //---  between squares and diamonds (which is just a rotated square)
    setRotationInvariance(true);


}


//------------
GestureTemplate& GestureTemplate::operator=( const GestureTemplate& other ){
       this->clear();
       for(int i=0; i<other.size();i++){
           this->append(other.at(i));
       }
       setName(other.name());
       return *this;
}

//------------
QPointF GestureTemplate::centroid(const QPolygonF& poly){
    QPointF center = QPointF(0,0);
    for(int i=0;i<poly.size();i++){
        center += poly.at(i);
    }
    if(poly.size()>0){
        center = center/ poly.size();
    }
    return center;
}

//------------
bool GestureTemplate::is1D(const QPolygonF& stroke)const{
    QPolygonF newPoints = rotateToZero(stroke);
    QRectF box = newPoints.boundingRect();
    return (box.width() / box.height() <= 0.3 || box.height()/box.width() <= 0.3);
}

//------------
//scaleToSquare
QPolygonF GestureTemplate::scaleToSquare(const QPolygonF& stroke, int squareSize)const{

    QPolygonF newPoints;
    float xWidth = 0;
    float yWidth = 0;

    QRectF box = stroke.boundingRect();

    //--- If path is 1D then scale uniformly,
    //--- else stretch to square
    if(is1D(stroke)) {
        float maxDimension = box.width() > box.height() ? box.width() : box.height();
        xWidth = yWidth = maxDimension;
    } else {
        xWidth = box.width();
        yWidth = box.height();
    }

    for ( int i =0; i< stroke.size(); i++){
        QPointF p = stroke.at(i);
        //--- Scale the points to fit the main box
        //--- So if we wanted everything 100x100 and this was 50x50,
        //---  we'd multiply every point by 2
        float newPX = p.x() * ((float)squareSize / xWidth);
        float newPY = p.y() * ((float)squareSize / yWidth);
        //--- Why are we adding them to a new list rather than
        //---  just scaling them in-place?
        // TODO: try scaling in place (once you know this way works)
        newPoints.append(QPointF(newPX, newPY));
    }
    return newPoints;

}




//---------------------------
void GestureTemplate::normalize(){
    QPolygonF p = *this;

    //...

}

//-------------------------
void GestureTemplate::setRotationInvariance(bool b){
    ignoreRotation =b;

      if (ignoreRotation){
          angleRange = 45.0*3.14/180;
      }else{
          angleRange = 15.0*3.14/180;
      }
}

//------------------------
float GestureTemplate::length(QPolygonF stroke)const{
    float l =0;
    for(int i=0; i<stroke.size()-1;i++){
        l += QLineF(stroke.at(i), stroke.at(i+1)).length();
    }
    return l;
}

//------------------------
QPolygonF GestureTemplate::resample(const QPolygonF & _stroke) const{
    double interval = length(_stroke) / (numPointsInGesture - 1); // interval length
    double D = 0.0;
    QPolygonF stroke = _stroke;
    QPolygonF sampledStroke;

    //--- Store first point since we'll never resample it out of existence
    sampledStroke.append(stroke.at(0));
    for(int i = 1; i < (int)stroke.size(); i++)
    {
        QPointF curPoint = stroke.at(i);
        QPointF prevPoint = stroke.at(i-1);
        float d = QLineF(curPoint, prevPoint).length();
        if ( (D + d) >= interval)
        {
            float qx = prevPoint.x() + ((interval - D) / d) * (curPoint.x() - prevPoint.x());
            float qy = prevPoint.y() + ((interval - D) / d) * (curPoint.y() - prevPoint.y());
            QPointF point(qx, qy);
            sampledStroke.push_back(point);
            stroke.insert(i, point);
            D = 0.0;
        }
        else{ D += d; }
    }

    // somtimes we fall a rounding-error short of adding the last point, so add it if so
    if (sampledStroke.size() == (numPointsInGesture - 1))
    {
        sampledStroke.append(stroke.back());
    }

    return sampledStroke;
}

//-------------------
QPolygonF GestureTemplate::rotateBy(const QPolygonF& stroke, float rotation) const
{
    QPointF c = centroid(stroke);

    //--- can't name cos; creates compiler error since VC++ can't
    //---  tell the difference between the variable and function
    double cosine = qCos(rotation);
    double sine   = qSin(rotation);

    QPolygonF newPoints;

    for (int i=0; i< stroke.size(); i++)
    {
        QPointF p = stroke.at(i);
        double qx = (p.x() - c.x()) * cosine - (p.y() - c.y()) * sine   + c.x();
        double qy = (p.x() - c.x()) * sine   + (p.y() - c.y()) * cosine + c.y();
        newPoints.append( QPointF(qx, qy) );
    }

    return newPoints;
}

//----------------
QPolygonF GestureTemplate::rotateToZero(const QPolygonF &stroke)const
{
    QPointF c = GestureTemplate::centroid(stroke);
    double rotation = qAtan2(c.y() - stroke.at(0).y(), c.x() - stroke.at(0).x());
    return rotateBy(stroke, -rotation);
}

//----------------
QPolygonF GestureTemplate::translateToOrigin(const QPolygonF &stroke) const
  {
      QPointF c = GestureTemplate::centroid(stroke);
      return stroke.translated(-c);
  }

