#include "stdafx.h"
#include "TxSpatialOper.h"
#include "TxVector.h"
#include "TxConst.h"

TxPolygonPtr TxSpatialOper::Buffer( TxPointPtr spPt, double dBuffer )
{
    std::vector<TxPoint> vecPoint;
    if (dBuffer <= 0) {
        for (int i=0; i<4; i++) {
            vecPoint.push_back(TxPoint(spPt->X, spPt->Y));
        }

        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
        return spPolygon;
    }

    int nRoate(360);
    
    for (int i=0; i<=nRoate; i++) {        
        double dAngle = (360.0 / nRoate) * i;
        TxVector oVec(0, 1); {
            oVec.RotateClockwise(dAngle, true);
        }

        double dX = spPt->X + dBuffer * oVec.GetI();
        double dY = spPt->Y + dBuffer * oVec.GetJ();

        vecPoint.push_back(TxPoint(dX, dY));
    }

    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
    return spPolygon;
}

TxPolygonPtr TxSpatialOper::RectPoly( TxPointPtr spPt1, TxPointPtr spPt2 )
{
    std::vector<TxPoint> vecPt; {
        vecPt.push_back(TxPoint(spPt1->X, spPt1->Y));
        vecPt.push_back(TxPoint(spPt1->X, spPt2->Y));
        vecPt.push_back(TxPoint(spPt2->X, spPt2->Y));
        vecPt.push_back(TxPoint(spPt2->X, spPt1->Y));
        vecPt.push_back(TxPoint(spPt1->X, spPt1->Y));
    }
    TxPolygonPtr spPoly(new TxPolygon(vecPt));
    return spPoly;
}

std::vector<TxPoint> TxSpatialOper::LineRotate( TxPoint& oPt1, TxPoint& oPt2, double dDegree, bool bClockwise )
{
    TxVector vA(oPt1.X, oPt1.Y);
    TxVector vB(oPt2.X, oPt2.Y);

    double dRadius(0);
    TxVector vAB = vB - vA; {
        dRadius = vAB.Length();
        vAB.Normalize();
    }

    //★ 호의 길이
    double dArcLen = 2 * TxConst::PI() * dRadius * dDegree / 360.0;    

    int nLenCnt = (int)(dArcLen * 10); //★ 10Cm 기준
    int nAngCnt = (int)(dDegree * 72.0 / 360.0); //★ 0.5도 간격 1포인트

    int nCnt(nAngCnt);
    if (nAngCnt > nLenCnt) {
        nCnt = nLenCnt;
    }

    double dAngle = dDegree / nCnt;

    std::vector<TxPoint> vecPoint; {
        vecPoint.push_back(oPt2);
    }

    for (int i=0; i<nCnt; i++) {
        if (bClockwise)
            vAB.RotateClockwise(dAngle, true);
        else
            vAB.RotateUnClockwise(dAngle, true);

        double dX = oPt1.X + dRadius * vAB.GetI();
        double dY = oPt1.Y + dRadius * vAB.GetJ();

        vecPoint.push_back(TxPoint(dX, dY));
    }

    return vecPoint;
}

std::vector<TxPolygonPtr> TxSpatialOper::Union( TxPolygonPtr spRed, TxPolygonPtr spBlue )
{
    std::vector<TxPolygonPtr> vecUnion;
    try 
    {
        BoostPolygon oRed  = spRed->ToBoost();
        BoostPolygon oBlue = spBlue->ToBoost();

        std::vector<BoostPolygon> vecBxUnion; {
            boost::geometry::union_(oRed, oBlue, vecBxUnion);
        }

        for (size_t i=0; i<vecBxUnion.size(); i++) {
            vecUnion.push_back(TxPolygonPtr(new TxPolygon(vecBxUnion[i])));
        }
    }
    catch(...) {
        TxLogDebugException();
    }

    return vecUnion;
}
