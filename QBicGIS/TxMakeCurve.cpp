#include "stdafx.h"
#include "TxMakeCurve.h"
#include "TxVector.h"
#include "TxSpatialOper.h"

std::vector<TxPoint> TxMakeCurve::SimpleCurve( TxPoint oPt1, TxPoint oPt2, TxPoint oPt3, TxPoint oPt4, double dTailLen)
{
    //★ 직선(oPt1~oPt2), 직선(oPt3~oPt4) => 단곡선의 버텍스 정보 리턴
    //★ 단곡선
    
    std::vector<TxPoint> vecPt;
    
    double dRate(1);
    //★ 방위각 계산
    double dAzimuth01  = TxMath::Azimuth(oPt1, oPt2);    
    double dAzimuth02  = TxMath::Azimuth(oPt3, oPt4);

    //★ 방위각(Radian->Degree)
    double dAzimuth01D = TxMath::Radian2Degree(dAzimuth01);
    double dAzimuth02D = TxMath::Radian2Degree(dAzimuth02);
         
    double dInnerAngleD = fabs(dAzimuth01D - dAzimuth02D);
    if (dInnerAngleD >= 180.0) {
        dInnerAngleD = 360.0 - dInnerAngleD;
    }

    //★ 매우 뾰족한 케이스 평행한 케이스는 여기서 처리 완료한다.
    bool bParallel(false);
    if (dInnerAngleD > 179.0 || dInnerAngleD < 1.0) { 
        bParallel = true;  
    }

    TxPoint oIP;
    if (bParallel || TxMath::GetIntersectPoint(oPt1, oPt2, oPt3, oPt4, oIP) != true) {
        //★ 평행 또는 교차하지 않는 경우 처리
        double dDist1 = TxMath::Dist(oPt1, oPt2);
        double dDist2 = TxMath::Dist(oPt3, oPt4);
        
        double dShortDist = dTailLen * 0.2;
        if (dDist1 <= dShortDist || dDist1 < 10) {
            dShortDist = 0;
        }        

        if (dDist2 <= dShortDist || dDist2 < 10) {
            dShortDist = 0;
        } 

        TxPoint tPt;            
        TxMath::LineOnPoint(oPt2, oPt1, dTailLen, tPt);
        vecPt.push_back(tPt);
                
        TxMath::LineOnPoint(oPt2, oPt1, dShortDist, tPt);
        vecPt.push_back(tPt);

        TxMath::LineOnPoint(oPt3, oPt4, dShortDist, tPt);
        vecPt.push_back(tPt);

        TxMath::LineOnPoint(oPt3, oPt4, dTailLen, tPt);
        vecPt.push_back(tPt);
        
        return vecPt;
    }

    //★ 
    //★ 평행, 뾰족한 케이스는 없다는 조건에서 시작한다.
    //★ 

    double dDistAP = TxMath::Dist(oPt1, oIP);
    double dDistPB = TxMath::Dist(oIP , oPt4);

    double dLen12 = TxMath::Dist(oPt1, oPt2);
    double dLen34 = TxMath::Dist(oPt3, oPt4);
    
    double dTL(0);
    //★ 짧은 선을 기준으로 처리한다.
    if (dLen12 <= dLen34) {
        if (dLen12 <= dDistAP) {
            dTL = dDistAP - dLen12 * dRate;
        }
        else {
            dTL = dLen12 - dDistAP * dRate;
        }
    }
    else {
        if (dLen34 <= dDistPB) {
            dTL = dDistPB - dLen34 * dRate;
        }
        else {
            dTL = dLen34 - dDistPB * dRate;
        }
    }
    
    double dTheta = TxMath::Degree2Radian(dInnerAngleD*0.5);
    double dR = dTL / tan(dTheta); //★ tan(dTheta) : 0이 올 수 있는 케이스는 없다. 앞쪽에서 제거(수평체크)
    
    double dDistAC = dDistAP - dTL;    
    double dDistBD = dDistPB - dTL;
        
    double dRight(1);
    double dCheck = dAzimuth02D - dAzimuth01D;
    if (dCheck < 0)
        dCheck += 360;

    if (dCheck > 180)
        dRight = -1;

    double dAngle(0);
    TxPoint oPtC, oPt; {
        TxMath::LineOnPoint(oPt1, oIP, dDistAC, oPtC, oPt, dAngle, dRight * dR);
    }

    if (false) {
        TxPoint oPtD; {
            TxPoint otPt;
            TxMath::LineOnPoint(oPt4, oIP, dDistBD, oPtD, otPt, dAngle);
        }
    }
    
    std::vector<TxPoint> vecArcPt;
    if (dRight > 0) {
        //★ 시계방향으로 돌린다.
        vecArcPt = TxSpatialOper::LineRotate(oPt, oPtC, dInnerAngleD, true);
    }
    else {
        //★ 반시계방향으로 돌린다.
        vecArcPt = TxSpatialOper::LineRotate(oPt, oPtC, dInnerAngleD, false);
    }
    
    size_t nxCnt = vecArcPt.size();
    if (nxCnt >= 2) {
        TxPoint& oPcA = vecArcPt[0];
        TxPoint& oPcB = vecArcPt[nxCnt-1];

        TxPoint tPt;            
        TxMath::LineOnPoint(oPcA, oPt1, dTailLen, tPt);
        vecPt.push_back(tPt);

        for (size_t i=0; i<vecArcPt.size(); i++) {
            vecPt.push_back(vecArcPt[i]);
        }

        TxMath::LineOnPoint(oPcB, oPt4, dTailLen, tPt);
        vecPt.push_back(tPt);
    }
    else {
        vecPt.push_back(oPt1);
        for (size_t i=0; i<vecArcPt.size(); i++) {
            vecPt.push_back(vecArcPt[i]);
        }
        vecPt.push_back(oPt4);
    } 

    return vecPt;
}
