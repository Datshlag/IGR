#ifndef GESTURETEMPLATE_H
#define GESTURETEMPLATE_H

#include <QPolygonF>
#include <QObject>
#include <qmath.h>
class OneDollarUtil;
class GestureTemplate : public QObject, public QPolygonF
{
    Q_OBJECT
    Q_PROPERTY(QString Name READ name WRITE setName)
public:
    friend class OneDollarUtil;
    explicit GestureTemplate():QObject(0),QPolygonF(){init();}
    GestureTemplate(const GestureTemplate& t):QObject(0),QPolygonF(t),m_name(t.name()){}
    explicit GestureTemplate(int _size):QObject(0),QPolygonF(_size){init();}
    explicit GestureTemplate(const QPolygonF& _p):QObject(0),QPolygonF(_p){init();}
    explicit GestureTemplate(const QPolygon & _p):QObject(0),QPolygonF(_p){init();}
    explicit GestureTemplate(const QVector<QPointF> & _points):QObject(0), QPolygonF(_points){init();}
    explicit GestureTemplate(const QRectF & _r):QObject(0), QPolygonF(_r){init();}
    GestureTemplate& operator=( const GestureTemplate& other );

    void normalize();

    const QString& name()const{ return m_name; }
    void setName(const QString& _name){ m_name = _name; }

    //centroid
    static QPointF centroid(const QPolygonF& poly);
    QPointF centroid(){ return GestureTemplate::centroid(*this); }

    //scaleToSquare
    QPolygonF scaleToSquare(const QPolygonF& stroke, int squareSize=250)const;//{ return GestureTemplate::scaleToSquare(*this, squareSize); }

 protected:
    bool is1D(const QPolygonF& stroke)const;

    void setRotationInvariance(bool b);
    bool isRotationInvariant()const{ return ignoreRotation; }
    GestureTemplate createGestureTemplate(const QPolygonF& stroke, const QString & name);
    QPolygonF resample(const QPolygonF& stroke)const;
    QPolygonF rotateBy(const QPolygonF &stroke, float rotation)const;
    QPolygonF rotateToZero(const QPolygonF &stroke) const;
    QPolygonF translateToOrigin(const QPolygonF& stroke)const;
    float length(QPolygonF poly) const;
    void init();

protected:
    QString m_name;
    double halfDiagonal;
    float angleRange;

    //--- How many points we use to define a shape
    //--- How many points do we use to represent a gesture
    //--- Best results between 32-256
    int numPointsInGesture;
    bool ignoreRotation;

    //---- Square we resize the shapes to
            //--- Before matching, we stretch the symbol across a square
            //--- That way we don't have to worry about the symbol the user drew
            //---  being smaller or larger than the one in the template
    int squareSize;







};

#endif // GESTURETEMPLATE_H
