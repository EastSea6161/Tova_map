#include "QBicMath.h"
#include "QBicConst.h"

QBicMath::QBicMath(void)
{
}

QBicMath::~QBicMath(void)
{
}

double QBicMath::Mod( double x, double y )
{
    return (x-y*(int)(x/y));
}

double QBicMath::Degree2Radian( double dDegree )
{
    double dRadian = dDegree * QBicConst::PI / 180.0;
    return dRadian;
}

double QBicMath::Radian2Degree( double dRadian, bool bPlus )
{
    double dDegree = dRadian * 180.0 / QBicConst::PI;
    dDegree = Mod(dDegree, 360.0);

    if (bPlus && dDegree < 0)
    {
        dDegree = 360 + dDegree;
    }
    return dDegree;
}

double QBicMath::Dist( double x1, double y1, double x2, double y2 )
{
    double dDist(0.0);
    dDist = sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));

    return dDist;
}

double QBicMath::Dist( QBicPoint oPt1, QBicPoint oPt2 )
{
    return Dist(oPt1.X, oPt1.Y, oPt2.X, oPt2.Y);
}
