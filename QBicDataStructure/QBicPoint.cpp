#include "QBicPoint.h"
#include "QBicEnvelope.h"

QBicPoint::QBicPoint() : X(0.0), Y(0.0)
{

}

QBicPoint::QBicPoint(double dX, double dY) : X(dX), Y(dY) 
{
}

QBicPoint::QBicPoint(int nX, int nY) : X(nX), Y(nY) 
{
}

QBicPoint::QBicPoint(const QBicPoint& right )
{
    X = right.X;
    Y = right.Y;
}

QBicPoint::~QBicPoint()
{
}

QBicPoint& QBicPoint::operator=(const QBicPoint& right )
{
    if (&right == this)
    {
        return *this;
    }

    X = right.X;
    Y = right.Y;

    return *this;
}

bool QBicPoint::operator==( const QBicPoint& right ) const
{
    if ( (right.X - X < 2.2204460492503131e-016) && (right.Y - Y < 2.2204460492503131e-016) )
    {
        return true;
    }

    return false;
}


QBicEnvelope QBicPoint::GetEnvelope()
{
    return QBicEnvelope(X, Y);
}


