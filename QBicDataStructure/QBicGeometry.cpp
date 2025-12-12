#include "QBicGeometry.h"
#include "QBicMath.h"

QBicGeometry::QBicGeometry(void)
{
}


QBicGeometry::~QBicGeometry(void)
{
}

QBicPoint QBicGeometry::GetLineOffsetPoint( double dX1, double dY1, double dX2, double dY2, double dLength/*=0.0*/, double dOffset/*=1.0*/ )
{    
    QBicVector vA(dX1, dY1);
    QBicVector vB(dX2, dY2);

    QBicVector vAB  = vB- vA;
    vAB.Normalize();

    QBicVector vABrx = vAB;

    vABrx.RotateClockwise(90.0, true);
    vABrx.Multiply(dOffset);

    vAB.Multiply(dLength);
    double nX = dX1 + vAB.GetI();
    double nY = dY1 + vAB.GetJ();

    double nXro = nX + vABrx.GetI();
    double nYro = nY + vABrx.GetJ();

    QBicPoint oResult(nXro, nYro);
    return oResult;
}

QBicPoint QBicGeometry::GetLineOffsetPoint( const QBicPoint& fromPt, const QBicPoint& toPt, double dLength/*=0.0*/, double dOffset/*=1.0*/ )
{
    QBicVector vA(fromPt.X, fromPt.Y);
    QBicVector vB(toPt.X, toPt.Y);

    QBicVector vAB  = vB- vA;
    vAB.Normalize();

    QBicVector vABrx = vAB;

    vABrx.RotateClockwise(90.0, true);
    vABrx.Multiply(dOffset);

    vAB.Multiply(dLength);
    double nX = fromPt.X + vAB.GetI();
    double nY = fromPt.Y + vAB.GetJ();

    double nXro = nX + vABrx.GetI();
    double nYro = nY + vABrx.GetJ();

    QBicPoint oResult(nXro, nYro);
    return oResult;
}

std::vector<QBicPoint> QBicGeometry::GetLineOffsetPoints( const QBicPoint& fromPt, const QBicPoint& toPt, double dOffset/*=1.0*/ )
{
    double dLength = Length(fromPt, toPt);

    QBicVector vA(fromPt.X, fromPt.Y);
    QBicVector vB(toPt.X,   toPt.Y);

    QBicVector vAB  = vB- vA;
    vAB.Normalize();

    QBicVector vABrx = vAB;

    vABrx.RotateClockwise(90.0, true);
    vABrx.Multiply(dOffset);

    std::vector<QBicPoint> vecResult;
    
    QBicPoint oP1(fromPt.X + vABrx.GetI(), fromPt.Y + vABrx.GetJ());
    QBicPoint oP2(toPt.X   + vABrx.GetI(), toPt.Y   + vABrx.GetJ());
        
    vecResult.push_back(oP1);
    vecResult.push_back(oP2);
    return vecResult;
}

double QBicGeometry::Length( double dX1, double dY1, double dX2, double dY2 )
{
    double dLength = sqrt( pow((dX1-dX2), 2 ) + pow( (dY1-dY2), 2 ) );
    return dLength;
}

double QBicGeometry::Length( const QBicPoint& fromPt, const QBicPoint& toPt )
{
    return Length(fromPt.X, fromPt.Y, toPt.X, toPt.Y);
}

double QBicGeometry::AzimuthAngle( double dX1, double dY1, double dX2, double dY2 )
{
    double dX = dX2 - dX1;
    double dY = dY2 - dY1;

    double dRadian = atan2(dX, dY);
    return dRadian;
}

double QBicGeometry::AzimuthAngle( const QBicPoint& fromPt, const QBicPoint& toPt )
{
    return AzimuthAngle(fromPt.X, fromPt.Y, toPt.X, toPt.Y);
}


int QBicGeometry::IntersectPoint( const QBicPoint& oAP1, const QBicPoint& oAP2, const QBicPoint& oBP1, const QBicPoint& oBP2, QBicPoint& oIP )
{// 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
    double t;
    double s; 

    // 평행
    double under = (oBP2.Y-oBP1.Y)*(oAP2.X-oAP1.X)-(oBP2.X-oBP1.X)*(oAP2.Y-oAP1.Y);
    if(under==0) 
        return 9;

    double _t = (oBP2.X-oBP1.X)*(oAP1.Y-oBP1.Y) - (oBP2.Y-oBP1.Y)*(oAP1.X-oBP1.X);
    double _s = (oAP2.X-oAP1.X)*(oAP1.Y-oBP1.Y) - (oAP2.Y-oAP1.Y)*(oAP1.X-oBP1.X); 

    t = _t/under;
    s = _s/under; 

    oIP.X = oAP1.X + t * (double)(oAP2.X-oAP1.X);
    oIP.Y = oAP1.Y + t * (double)(oAP2.Y-oAP1.Y);

    bool bInternalLine = ( 0 <= t  && t <= 1) && ( 0 <= s  && s <= 1);
    if (bInternalLine == true)
        return 0;
    
    if (t >= 0.0 && s >= 0.0)
    {
        return 1;
    }
    else if (t < 0.0 && s < 0.0)
    {
        return -1;
    }

    return 2;
}


bool QBicGeometry::GetIntersectPoint(const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oB1, const QBicPoint& oB2, QBicPoint& oIP, bool bExtendOption) 
{    
    QBicPoint oAP1(oA1);
    QBicPoint oAP2(oA2);
    QBicPoint oBP1(oB1);
    QBicPoint oBP2(oB2);

    double t;
    double s; 

    // 평행
    double under = (oBP2.Y-oBP1.Y)*(oAP2.X-oAP1.X)-(oBP2.X-oBP1.X)*(oAP2.Y-oAP1.Y);
    if(under==0) 
        return false;

    double _t = (oBP2.X-oBP1.X)*(oAP1.Y-oBP1.Y) - (oBP2.Y-oBP1.Y)*(oAP1.X-oBP1.X);
    double _s = (oAP2.X-oAP1.X)*(oAP1.Y-oBP1.Y) - (oAP2.Y-oAP1.Y)*(oAP1.X-oBP1.X); 

    t = _t/under;
    s = _s/under; 

    bool bInternalLine = ( 0 <= t  && t <= 1) && ( 0 <= s  && s <= 1);
    if (bExtendOption == false && bInternalLine == false)
        return false;

    oIP.X = oAP1.X + t * (double)(oAP2.X-oAP1.X);
    oIP.Y = oAP1.Y + t * (double)(oAP2.Y-oAP1.Y);

    return true;
} 

bool QBicGeometry::IsLineOnPoint( const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, double dRadius )
{
    double cx = oCP.X;
    double cy = oCP.Y;
    double ax = oA1.X;
    double ay = oA1.Y;
    double bx = oA2.X;
    double by = oA2.Y;

    double r_numerator   = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
    double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
    double r = r_numerator / r_denomenator;

    double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
    double d = fabs(s)*sqrt(r_denomenator);

    if ( (0 <= r) && (r <= 1) && d <= dRadius)
    {
        return true;
    }

    return false;
}

int QBicGeometry::GetIntersectPoint( const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, QBicPoint& oIP, double &dDistanceOnLine, double &dDistanceShortestLine)
{
    //라인과 교차하는 점을 리턴한다.
    double cx = oCP.X;
    double cy = oCP.Y;
    double ax = oA1.X;
    double ay = oA1.Y;
    double bx = oA2.X;
    double by = oA2.Y;
        
	double r_numerator   = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
	double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
	double r = r_numerator / r_denomenator;

    double px = ax + r*(bx-ax);
    double py = ay + r*(by-ay);
     
    double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
	double d = fabs(s)*sqrt(r_denomenator);

	double xx = px;
	double yy = py;
    
    dDistanceShortestLine = d;
    dDistanceOnLine = sqrt((xx-ax)*(xx-ax) + (yy-ay)*(yy-ay));
    
    oIP.X = xx;
    oIP.Y = yy;

    if ( (0 <= r) && (r <= 1) )
        return 0;
    else if (r < 0)
        return -1;
    else 
        return 1;
}

void QBicGeometry::GetShortestPoint( const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, QBicPoint& oIP, double &dDistanceOnLine, double &dDistanceShortestLine )
{
    //라인과 가장 가까운 점을 리턴한다.
    double cx = oCP.X;
    double cy = oCP.Y;
    double ax = oA1.X;
    double ay = oA1.Y;
    double bx = oA2.X;
    double by = oA2.Y;
    
    //
	// find the distance from the point (cx,cy) to the line
	// determined by the points (ax,ay) and (bx,by)
	//
	// dDistanceSegment = distance from the point to the line segment
	// dDistanceLine    = distance from the point to the line (assuming
	//					  infinite extent in both directions
	//
/*

Subject 1.02: How do I find the distance from a point to a line?
    Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
    Let P be the point of perpendicular projection of C on AB.  The parameter
    r, which indicates P's position along AB, is computed by the dot product 
    of AC and AB divided by the square of the length of AB:
    
    (1)     AC dot AB
        r = ---------  
            ||AB||^2
    
    r has the following meaning:
    
        r=0      P = A
        r=1      P = B
        r<0      P is on the backward extension of AB
        r>1      P is on the forward  extension of AB
        0<r<1    P is interior to AB
    
    The length of a line segment in d dimensions, AB is computed by:
    
        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 + ... + (Bd-Ad)^2)

    so in 2D:   
    
        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )
    
    and the dot product of two vectors in d dimensions, U dot V is computed:
    
        D = (Ux * Vx) + (Uy * Vy) + ... + (Ud * Vd)
    
    so in 2D:   
    
        D = (Ux * Vx) + (Uy * Vy) 
    
    So (1) expands to:
    
            (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
        r = -------------------------------
                          L^2

    The point P can then be found:

        Px = Ax + r(Bx-Ax)
        Py = Ay + r(By-Ay)

    And the distance from A to P = r*L.

    Use another parameter s to indicate the location along PC, with the 
    following meaning:
           s<0      C is left of AB
           s>0      C is right of AB
           s=0      C is on AB

    Compute s as follows:

            (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
        s = -----------------------------
                        L^2


    Then the distance from C to P = |s|*L.
*/
    
	double r_numerator   = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
	double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
	double r = r_numerator / r_denomenator;
//
    double px = ax + r*(bx-ax);
    double py = ay + r*(by-ay);
//     
    double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
	double d = fabs(s)*sqrt(r_denomenator);

//
// (xx,yy) is the point on the lineSegment closest to (cx,cy)
//
	double xx = px;
	double yy = py;

	if ( (0 <= r) && (r <= 1) )
	{
		dDistanceShortestLine = d;
	}
	else
	{
        //라인의 연장선과 교차하기 때문에 시점, 종점과 비교
		double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
		double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
		if (dist1 < dist2)
		{
			xx = ax;
			yy = ay;
			dDistanceShortestLine = sqrt(dist1);
		}
		else
		{
			xx = bx;
			yy = by;
			dDistanceShortestLine = sqrt(dist2);
		}
	}

    dDistanceOnLine = sqrt((xx-ax)*(xx-ax) + (yy-ay)*(yy-ay));
    
    oIP.X = xx;
    oIP.Y = yy;
}

std::vector<QBicPoint> QBicGeometry::GetCWCircleLinePoint( const QBicPoint& centerPt, const QBicPoint& firstPt, double dCWRAngle, int nPointCount )
{
    std::vector<QBicPoint> vecResult;
    vecResult.push_back(firstPt);

    double dDist = QBicMath::Dist(centerPt, firstPt);
    QBicVector vA(centerPt.X, centerPt.Y);
    QBicVector vB(firstPt.X,  firstPt.Y);    
    QBicVector vAB  = vB- vA;
    vAB.Normalize();
    /*int nPointCount(360);*/
    double dAngle = dCWRAngle / nPointCount;

    for (int i=0; i<nPointCount; i++)
    {  
        vAB.RotateClockwise(dAngle, false);
        QBicPoint oResult(centerPt.X + dDist* vAB.GetI(), centerPt.Y + dDist* vAB.GetJ());
        vecResult.push_back(oResult);
    }

    return vecResult;
}

std::vector<QBicPoint> QBicGeometry::GetUCWCircleLinePoint( const QBicPoint& centerPt, const QBicPoint& firstPt, double dCWRAngle, int nPointCount )
{
    std::vector<QBicPoint> vecResult;
    vecResult.push_back(firstPt);

    double dDist = QBicMath::Dist(centerPt, firstPt);
    QBicVector vA(centerPt.X, centerPt.Y);
    QBicVector vB(firstPt.X,  firstPt.Y);    
    QBicVector vAB  = vB- vA;
    vAB.Normalize();

    /*int nPointCount(360);*/
    double dAngle = dCWRAngle / nPointCount;

    for (int i=0; i<nPointCount; i++)
    { 
        vAB.Rotate(dAngle, false);
        QBicPoint oResult(centerPt.X + dDist* vAB.GetI(), centerPt.Y + dDist* vAB.GetJ());
        vecResult.push_back(oResult);
    }

    return vecResult;
}





