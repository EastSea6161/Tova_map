#include "StdAfx.h"
#include "IntersectionTopologyAnalysis.h"

KIntersectionTopologyAnalysis::KIntersectionTopologyAnalysis()
{

}

KIntersectionTopologyAnalysis::~KIntersectionTopologyAnalysis()
{

}


double KIntersectionTopologyAnalysis::XAxisCounterClockwiseAngle( ITxGeometryPtr spLink )
{
    //★ 진입하는 링크를 뒤집어서 계산한다.
    TxPolylinePtr spPline = std::dynamic_pointer_cast<TxPolyline>(spLink);
    std::vector<TxPoint> vecPt = spPline->GetPoints();
    
    if (vecPt.size() < 1)
        return 0.0;

    TxPoint oEnd   = vecPt[vecPt.size()-1];
    TxPoint oStart = vecPt[0];

    return XAxisCounterClockwiseAngle(oEnd.X, oEnd.Y, oStart.X, oStart.Y);
}

double KIntersectionTopologyAnalysis::XAxisCounterClockwiseAngle( double a_dX1, double a_dY1,double a_dX2, double a_dY2 )
{
    double PI = 3.141592653589793;

    double dAtan2  = atan2((a_dY2-a_dY1), (a_dX2-a_dX1));
    double dDegree = dAtan2 *180.0 / PI;

    if (dDegree < 0)
    {
        return 360.0 + dDegree;
    }

    return dDegree;
}

double KIntersectionTopologyAnalysis::Dist( double a_dX1, double a_dY1, double a_dX2, double a_dY2 )
{
    double dDist = 0.0;
    dDist = sqrt(pow((a_dX2-a_dX1), 2.0) + pow((a_dY2-a_dY1), 2.0));

    return dDist;
}

void KIntersectionTopologyAnalysis::Analysis3DirectionCrossRoad( std::vector<KLinkIDGeometry>& a_vecGeometry, std::vector<Integer>& ar_vecLink, int& ar_nCrossType )
{
    int nCount = (int)a_vecGeometry.size();
    if ( nCount != 3)
    {
        TxLogDebugException();
        return;
    }

    int    nMinSeq = 0;
    double dTemp   = 999.0;

    for (int i=0; i<nCount; i++)
    {        
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[i];
        oLinkIDGeometry.TAngle = XAxisCounterClockwiseAngle(oLinkIDGeometry.TGeometry);

        double dAngle = abs(oLinkIDGeometry.TAngle - 180.0);

        if (dTemp > dAngle)
        {
            nMinSeq = i;
            dTemp   = dAngle;
        }
    }

    // -X축에 가장 가까운 친구의 각도
    KLinkIDGeometry& oFirstLinkIDGeometry = a_vecGeometry[nMinSeq];
    double           dFirstAngle          = oFirstLinkIDGeometry.TAngle;

    std::map<double, int>  mapSortByAngle;
    std::map<double, int>::iterator  iter, end;

    for (int i=0; i<nCount; i++)
    {        
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[i];

        double dAngle = 0.0;

        if (dFirstAngle >= oLinkIDGeometry.TAngle)
        {
            dAngle = dFirstAngle - oLinkIDGeometry.TAngle;
        }
        else
        {
            dAngle = 360.0 - (oLinkIDGeometry.TAngle - dFirstAngle);
        }
        
        iter = mapSortByAngle.find(dAngle);
        end  = mapSortByAngle.end();

        if (iter != end)
        {
            dAngle += 0.0001 * i;
        }

        mapSortByAngle.insert(std::make_pair(dAngle, i));
    }

    nCount = (int)mapSortByAngle.size();
    if ( nCount != 3)
    {
        TxLogDebugException();
        return;
    }
       
    iter = mapSortByAngle.begin();
    end  = mapSortByAngle.end();
    
    // 시계 방향별 링크
    while(iter != end)
    {
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[iter->second];
        ar_vecLink.push_back(oLinkIDGeometry.TLinkID);

        ++iter;
    }

    // 주도로 판별
    std::vector<double> vecGapAngle;
    vecGapAngle.push_back(abs(abs(a_vecGeometry[0].TAngle - a_vecGeometry[1].TAngle) - 180.0));  
    vecGapAngle.push_back(abs(abs(a_vecGeometry[1].TAngle - a_vecGeometry[2].TAngle) - 180.0));
    vecGapAngle.push_back(abs(abs(a_vecGeometry[2].TAngle - a_vecGeometry[0].TAngle) - 180.0));
        
    nMinSeq = 0;
    dTemp   = 999.0;
    for (int i=0; i<3; i++)
    {
        if (vecGapAngle[i] < dTemp)
        {
            nMinSeq = i;
            dTemp   = vecGapAngle[i];
        }
    }

    if (nMinSeq == 0)
    {/*2번째가 부도로*/
        SubRoadType(a_vecGeometry[2].TAngle, ar_nCrossType);
        ar_vecLink.push_back(a_vecGeometry[2].TLinkID);
    }
    else if (nMinSeq == 1)
    {/*0번째가 부도로*/
        SubRoadType(a_vecGeometry[0].TAngle, ar_nCrossType);
        ar_vecLink.push_back(a_vecGeometry[0].TLinkID);
    }
    else
    {/*1번째가 부도로*/
        SubRoadType(a_vecGeometry[1].TAngle, ar_nCrossType);
        ar_vecLink.push_back(a_vecGeometry[1].TLinkID);
    }
}

void KIntersectionTopologyAnalysis::SubRoadType( double a_dAngle, int& ar_nCrossType )
{
    // ㅏ(1), ㅓ(2), ㅗ(3), ㅜ(4)
    ar_nCrossType = 0;

    if (a_dAngle >= 45 && a_dAngle < 135)
    {
        ar_nCrossType = 3;
    }
    else if (a_dAngle >= 135 && a_dAngle < 225)
    {
        ar_nCrossType = 2;
    }
    else if (a_dAngle >= 225 && a_dAngle < 315)
    {
        ar_nCrossType = 4;
    }
    else
    {
        ar_nCrossType = 1;
    }
}

void KIntersectionTopologyAnalysis::Analysis4DirectionCrossRoad( std::vector<KLinkIDGeometry>& a_vecGeometry, std::vector<Integer>& ar_vecLink )
{
    ar_vecLink.clear();

    int nCount = (int)a_vecGeometry.size();
    if ( nCount != 4)
    {
        TxLogDebugException();
        return;
    }

    int    nMinSeq = 0;
    double dTemp = 999.0;

    for (int i=0; i<nCount; i++)
    {        
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[i];
        oLinkIDGeometry.TAngle = XAxisCounterClockwiseAngle(oLinkIDGeometry.TGeometry);
        
        double dAngle = abs(oLinkIDGeometry.TAngle - 180.0);
                        
        if (dTemp > dAngle)
        {
            nMinSeq = i;
            dTemp   = dAngle;
        }
    }

    // -X축에 가장 가까운 친구의 각도
    KLinkIDGeometry& oFirstLinkIDGeometry = a_vecGeometry[nMinSeq];
    double           dFirstAngle          = oFirstLinkIDGeometry.TAngle;

    std::map<double, int>  mapSortByAngle;
    std::map<double, int>::iterator      iter, end;

    for (int i=0; i<nCount; i++)
    {        
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[i];
        //oLinkIDGeometry.TAngle = XAxisCounterClockwiseAngle(oLinkIDGeometry.TGeometry);

        double dAngle = 0.0;

        if (dFirstAngle >= oLinkIDGeometry.TAngle)
        {
            dAngle = dFirstAngle - oLinkIDGeometry.TAngle;
        }
        else
        {
            dAngle = 360.0 - (oLinkIDGeometry.TAngle - dFirstAngle);
        }

        iter = mapSortByAngle.find(dAngle);
        end  = mapSortByAngle.end();

        if (iter != end)
        {
            dAngle += 0.0001 * i;
        }

        mapSortByAngle.insert(std::make_pair(dAngle, i));
    }
    
    nCount = (int)mapSortByAngle.size();
    if ( nCount != 4)
    {
        TxLogDebugException();
        return;
    }
    
    iter = mapSortByAngle.begin();
    end  = mapSortByAngle.end();

    while(iter != end)
    {
        KLinkIDGeometry& oLinkIDGeometry = a_vecGeometry[iter->second];
        ar_vecLink.push_back(oLinkIDGeometry.TLinkID);
        ++iter;
    }
}

