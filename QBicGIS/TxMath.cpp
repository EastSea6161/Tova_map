#include "stdafx.h"
#include "TxMath.h"
#include "TxVector.h"

TxMath::TxMath(void)
{
}

TxMath::~TxMath(void)
{
}

double TxMath::Mod( double x, double y )
{
    return (x - y * (int)(x/y));
}

double TxMath::Degree2Radian( double dDegree )
{
    double dRadian = dDegree * TxConst::PI() / 180.0;
    return dRadian;
}

double TxMath::Radian2Degree( double dRadian, bool bPlus )
{
    double dDegree = dRadian * 180.0 / TxConst::PI(); {
        dDegree = Mod(dDegree, 360.0);
    }
    
    if (bPlus && dDegree < 0) {
        dDegree = 360 + dDegree;
    }
    return dDegree;
}

double TxMath::Dist( double x1, double y1, double x2, double y2 )
{
    double dDist(0.0);
    dDist = sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));

    return dDist;
}

double TxMath::Dist( const TxPoint& oPt1, const TxPoint& oPt2 )
{
    return Dist(oPt1.X, oPt1.Y, oPt2.X, oPt2.Y);
}


std::vector<TxPoint> TxMath::GetLineOffsetPoints( const TxPoint& fromPt, const TxPoint& toPt, double dOffset/*=1.0*/ )
{
    std::vector<TxPoint> vecResult;

    double dLength = Dist(fromPt, toPt);
    if (dLength <= TxConst::Epsilon()) {
        vecResult.push_back(fromPt);
        vecResult.push_back(toPt);
        return vecResult;
    }

    TxVector vA(fromPt.X, fromPt.Y);
    TxVector vB(toPt.X,   toPt.Y);

    TxVector vAB  = vB- vA;
    vAB.Normalize();

    TxVector vABrx = vAB;

    vABrx.RotateClockwise(90.0, true);
    vABrx.Multiply(dOffset);
    
    TxPoint oP1(fromPt.X + vABrx.GetI(), fromPt.Y + vABrx.GetJ());
    TxPoint oP2(toPt.X   + vABrx.GetI(), toPt.Y   + vABrx.GetJ());

    vecResult.push_back(oP1);
    vecResult.push_back(oP2);

    return vecResult;
}

int TxMath::GetIntersectPoint( const TxPoint& oA1, const TxPoint& oA2, const TxPoint& oCP, TxPoint& oIP, double &dDistanceOnLine )
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
        
    // 라인 선상 거리
    dDistanceOnLine = sqrt((xx-ax)*(xx-ax) + (yy-ay)*(yy-ay));

    oIP.X = xx;
    oIP.Y = yy;

    //-1:역방향교차, 0:직선위의점, 1:직선진행방향
    if ( (0 <= r) && (r <= 1) )
        return 0;
    else if (r < 0)
        return -1;
    else 
        return 1;
}

bool TxMath::GetIntersectPoint( const TxPoint& oA1, const TxPoint& oA2, const TxPoint& oB1, const TxPoint& oB2, TxPoint& oIP, bool bExtendOption /*= true*/ )
{
    TxPoint oAP1(oA1);
    TxPoint oAP2(oA2);
    TxPoint oBP1(oB1);
    TxPoint oBP2(oB2);

    double t;
    double s; 

    double under = (oBP2.Y-oBP1.Y)*(oAP2.X-oAP1.X)-(oBP2.X-oBP1.X)*(oAP2.Y-oAP1.Y);
    if(under==0.0) 
        return false;

    double _t = (oBP2.X-oBP1.X)*(oAP1.Y-oBP1.Y) - (oBP2.Y-oBP1.Y)*(oAP1.X-oBP1.X);
    double _s = (oAP2.X-oAP1.X)*(oAP1.Y-oBP1.Y) - (oAP2.Y-oAP1.Y)*(oAP1.X-oBP1.X); 

    t = _t/under;
    s = _s/under; 

    bool bInternalLine = ( 0 <= t  && t <= 1) && ( 0 <= s  && s <= 1);
    if (bExtendOption == false && bInternalLine == false)
        return false;

    oIP.X = oAP1.X + t * (oAP2.X-oAP1.X);
    oIP.Y = oAP1.Y + t * (oAP2.Y-oAP1.Y);

    return true;
}

int TxMath::IntersectPoint( const TxPoint& oAP1, const TxPoint& oAP2, const TxPoint& oBP1, const TxPoint& oBP2, TxPoint& oIP )
{// 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
    double t;
    double s; 

    // 평행
    double under = (oBP2.Y-oBP1.Y)*(oAP2.X-oAP1.X)-(oBP2.X-oBP1.X)*(oAP2.Y-oAP1.Y);
    if(under==0) 
        return 9; //★ 평행(라인 위의 두점일 수도 있다)

    double _t = (oBP2.X-oBP1.X)*(oAP1.Y-oBP1.Y) - (oBP2.Y-oBP1.Y)*(oAP1.X-oBP1.X);
    double _s = (oAP2.X-oAP1.X)*(oAP1.Y-oBP1.Y) - (oAP2.Y-oAP1.Y)*(oAP1.X-oBP1.X); 

    t = _t/under;
    s = _s/under; 

    oIP.X = oAP1.X + t * (double)(oAP2.X-oAP1.X);
    oIP.Y = oAP1.Y + t * (double)(oAP2.Y-oAP1.Y);

    bool bInternalLine = ( 0 <= t  && t <= 1) && ( 0 <= s  && s <= 1);
    if (bInternalLine == true)
        return 0;

    if (t >= 0.0 && s >= 0.0) {
        return 1;
    }
    else if (t < 0.0 && s < 0.0) {
        return -1;
    }

    return 2;
}

//라인의 시점에서 요청거리에 해당하는 점을 Offset한 위치와 라인의 각도 정보 리턴
void TxMath::LineOnPoint( const TxPoint& oPt1, const TxPoint& oPt2, double dDist, TxPoint& oOnPt, TxPoint& oOffsetPt, double& dAngle, double dOffset )
{
    double dLength = Dist(oPt1, oPt2);
    
    TxVector vA(oPt1.X, oPt1.Y);
    TxVector vB(oPt2.X, oPt2.Y);

    TxVector vAB  = vB- vA;
    // 각도
    dAngle = vAB.Azimuth();

    TxVector vABrx = vAB; {
        vABrx.Normalize();
        vABrx.RotateClockwise(90.0, true);    
        vABrx.Multiply(dOffset);
    }    

    double dScale(0.0);
    if (dDist <= 0.0) 
        dScale = 0.0;
    else if (dDist > dLength)
        dScale = dLength;
    else
        dScale = dDist;

    vAB.SetLength(dScale);    

    // 최종 위치
    oOnPt.X = oPt1.X + vAB.GetI();
    oOnPt.Y = oPt1.Y + vAB.GetJ();

    oOffsetPt.X = oPt1.X + vAB.GetI() + vABrx.GetI();
    oOffsetPt.Y = oPt1.Y + vAB.GetJ() + vABrx.GetJ();
}

void TxMath::LineOnPoint( const TxPoint& oPt1, const TxPoint& oPt2, double dDist, TxPoint& oOnPt )
{
    TxPoint oOffsetPt;
    double  dAngle(0);
    double  dOffset(0);

    LineOnPoint(oPt1, oPt2, dDist, oOnPt, oOffsetPt, dAngle, dOffset);
}

double TxMath::Azimuth( double dX1, double dY1, double dX2, double dY2 )
{
    double dX = dX2 - dX1;
    double dY = dY2 - dY1;

    //★ 북쪽을 기준으로 시계방향
    double dRadian = atan2(dX, dY);
    return dRadian;
}

double TxMath::Azimuth( const TxPoint& fromPt, const TxPoint& toPt )
{
    return Azimuth(fromPt.X, fromPt.Y, toPt.X, toPt.Y);
}

double TxMath::AzimuthDegree( double dX1, double dY1, double dX2, double dY2 )
{
    double dRadian = Azimuth(dX1, dY1, dX2, dY2);
    double dDegree = Radian2Degree(dRadian);

    return dDegree;
}

double TxMath::AzimuthDegree( const TxPoint& fromPt, const TxPoint& toPt )
{
    return AzimuthDegree(fromPt.X, fromPt.Y, toPt.X, toPt.Y);
}
