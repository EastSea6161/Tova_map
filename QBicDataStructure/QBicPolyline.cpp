#include "QBicPolyline.h"
#include "QBicPolygon.h"
#include "QBicPoint.h"
#include "QBicEnvelope.h"
#include "QBicGeometry.h"

QBicPolyline::QBicPolyline() : m_oEnvelope(0.0, 0.0, 0.0, 0.0)
{
}

QBicPolyline::QBicPolyline( const QBicPolyline& right )
{
    if (this == &right)
        return;

    AddPoint(right.GetPoints());
}

QBicPolyline::~QBicPolyline()
{

}

QBicPolyline& QBicPolyline::operator=( const QBicPolyline& right )
{
    if (&right == this)
    {
        return *this;
    }

    Clear();
    AddPoint(right.GetPoints());

    return *this;
}

void QBicPolyline::Clear()
{
    m_vecPoints.clear();
}

void QBicPolyline::AddPoint( double dX, double dY )
{
    double dMinX(99999999999.0);
    double dMinY(99999999999.0);
    double dMaxX(-99999999999.0);
    double dMaxY(-99999999999.0);

    if (m_vecPoints.size() > 0)
        m_oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    if (dMinX > dX) 
        dMinX = dX;

    if (dMinY > dY) 
        dMinY = dY;

    if (dMaxX < dX)
        dMaxX = dX;

    if (dMaxY < dY)
        dMaxY = dY;
    
    m_vecPoints.push_back(QBicPoint(dX, dY));
    m_oEnvelope.SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

}

void QBicPolyline::AddPoint( std::vector<QBicPoint>& vecPoint )
{
    double dMinX(99999999999.0);
    double dMinY(99999999999.0);
    double dMaxX(-99999999999.0);
    double dMaxY(-99999999999.0);

    if (m_vecPoints.size() > 0)
        m_oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    size_t nxCount = vecPoint.size();

    for (size_t i=0; i<nxCount; i++)
    {
        QBicPoint& oPoint = vecPoint[i];
        double dX = oPoint.X;
        double dY = oPoint.Y;

        if (dMinX > dX) 
            dMinX = dX;

        if (dMinY > dY) 
            dMinY = dY;

        if (dMaxX < dX)
            dMaxX = dX;

        if (dMaxY < dY)
            dMaxY = dY;

        m_vecPoints.push_back(oPoint);
    }
    
    m_oEnvelope.SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

QBicEnvelope QBicPolyline::GetEnvelope()
{
    return m_oEnvelope;
}

bool QBicPolyline::Intersect( QBicPolyline& rOther )
{
    size_t nxCountA  = m_vecPoints.size();
    size_t nxCountB  = rOther.m_vecPoints.size();

    if (nxCountA < 2 || nxCountB < 2)
        return false;

    if ( m_oEnvelope.IsIntersect(rOther.GetEnvelope()) == false )
        return false;

    for (size_t i=0; i<nxCountA-1; i++)
    {
        QBicPoint& oAFrom = m_vecPoints[i];
        QBicPoint& oATo   = m_vecPoints[i+1];

        for (size_t j=0; j<nxCountB-1; j++)
        {
            QBicPoint& oBFrom = rOther.m_vecPoints[j];
            QBicPoint& oBTo   = rOther.m_vecPoints[j+1];

            QBicPoint oIP;
            bool bIntersect = QBicGeometry::GetIntersectPoint(oAFrom, oATo, oBFrom, oBTo, oIP, false);

            if (bIntersect)
                return true;
        }
    }
    
    return false;
}


bool QBicPolyline::IsPolyLineOnPoint( const QBicPoint& oCP, double dEpsilon )
{
    size_t nxCount = m_vecPoints.size();
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oAFrom = m_vecPoints[i];
        QBicPoint& oATo   = m_vecPoints[i+1];

        bool bIsOn = QBicGeometry::IsLineOnPoint(oAFrom, oATo, oCP, dEpsilon);
        if (bIsOn)
            return true;
    }

    return false;
}

bool QBicPolyline::GetPolyLineOnPoint( const QBicPoint& oCP, double& dDist, double dEpsilon /*= QBicConst::Epsilon */ )
{
    double dTemp = 999999999.0;

    bool bFind(false);
    double dDistanceOnLine(0.0);
    double dDistanceShortestLine(0.0);

    QBicPoint oIP;

    size_t nxCount = m_vecPoints.size();
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oAFrom = m_vecPoints[i];
        QBicPoint& oATo   = m_vecPoints[i+1];

        if ( QBicGeometry::GetIntersectPoint(oAFrom, oATo, oCP, oIP, dDistanceOnLine, dDistanceShortestLine) == 0)
        {
            bFind = true;

            if (dTemp > dDistanceShortestLine)
                dTemp = dDistanceShortestLine;
        }
    }
    if (bFind)
        dDist = dTemp;

    return bFind;
}

QBicPolygon QBicPolyline::RightOffset( double dOffset )
{
    QBicPolygon oPolygon;

    std::vector<QBicPoint> vecPolyPoint;
    size_t nxCount = m_vecPoints.size();

    if (nxCount == 0)
        return oPolygon;

    QBicPolyline oPLine = *this;
    oPLine.Offset(dOffset);
    oPLine.Reverse();

    for (size_t i=0; i<nxCount; i++)
    {
        vecPolyPoint.push_back(m_vecPoints[i]);
    }
    
    nxCount = oPLine.m_vecPoints.size();
    for (size_t i=0; i<nxCount; i++)
    {
        vecPolyPoint.push_back(oPLine.m_vecPoints[i]);
    }
    
    vecPolyPoint.push_back(m_vecPoints[0]);
    oPolygon.AddPoint(vecPolyPoint);

    return oPolygon;
}

void QBicPolyline::Offset( double dOffset )
{   
    size_t nxCount = m_vecPoints.size();
    if (nxCount <= 1)
        return;
    
    // 폴리라인을 구성하는 버텍스를 Offset
    struct TLine
    {
        QBicPoint m_oFrom;
        QBicPoint m_oTo;
    };

    std::vector<TLine> vecLine;
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oFrom = m_vecPoints[i];
        QBicPoint& oTo   = m_vecPoints[i+1];

        std::vector<QBicPoint> vecPoint = QBicGeometry::GetLineOffsetPoints(oFrom, oTo, dOffset);
        
        TLine oLine;
        oLine.m_oFrom = vecPoint[0];
        oLine.m_oTo   = vecPoint[1];

        vecLine.push_back(oLine);
    }
    
    std::vector<QBicPoint> vecOffsetPoint;
    nxCount = vecLine.size();

    // 절점이 두개인 경우는 바로 리턴
    if (nxCount == 1)
    {
        TLine& oLine  = vecLine[0];
        vecOffsetPoint.push_back(oLine.m_oFrom);
        vecOffsetPoint.push_back(oLine.m_oTo);

        m_vecPoints.clear();
        AddPoint(vecOffsetPoint);
        return;
    }

    // 교차체크 - 교차하면 무조건 통과 
    // 교차되지 않는 경우 
    //  'ㅗ' 모양 : 과도하게 Offset된 경우
    //  'ㅜ' 모양 : 기본적으로 만날 수 없다.

    vecOffsetPoint.push_back(vecLine[0].m_oFrom);

    for (size_t i=0; i<nxCount-1; i++)
    {
        TLine& oFirstLine  = vecLine[i];
        TLine& oSecondLine = vecLine[i+1];

        QBicPoint oIP;
        int nIntersectType = QBicGeometry::IntersectPoint(oFirstLine.m_oFrom, oFirstLine.m_oTo, oSecondLine.m_oTo, oSecondLine.m_oFrom, oIP);        
        // 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
        if (nIntersectType == 0)
        {//교차
            vecOffsetPoint.push_back(oIP);
        }
        else if (nIntersectType == 1)
        {//연장선에서 교차한다
            vecOffsetPoint.push_back(oIP);
        }        
    }

    // 마지막점 추가
    vecOffsetPoint.push_back(vecLine[nxCount-1].m_oTo);
    
    m_vecPoints.clear();
    AddPoint(vecOffsetPoint);
}

void QBicPolyline::Offset_Back( double dOffset )
{   
    size_t nxCount = m_vecPoints.size();
    if (nxCount <= 1)
        return;
    
    /*QBicPoint& oFrom = m_vecPoints[0];
    QBicPoint& oTo   = m_vecPoints[1];

    QBicPoint  oFromOffset = QBicGeometry::GetLineOffsetPoint(oFrom, oTo, 0.0, dOffset);
    double     dX = oFromOffset.X - oFrom.X;
    double     dY = oFromOffset.Y - oFrom.Y;

    std::vector<QBicPoint> vecOffsetPoint;
    for(size_t i=0; i<m_vecPoints.size(); i++)
    {
        QBicPoint oPt = m_vecPoints[i];
        oPt.X += dX;
        oPt.Y += dY;

        vecOffsetPoint.push_back(oPt);
    }
    m_vecPoints.clear();
    AddPoint(vecOffsetPoint);
    */

    // 폴리라인을 구성하는 버텍스를 Offset
    struct TLine
    {
        QBicPoint m_oFrom;
        QBicPoint m_oTo;
    };

    std::vector<TLine> vecLine;
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oFrom = m_vecPoints[i];
        QBicPoint& oTo   = m_vecPoints[i+1];

        std::vector<QBicPoint> vecPoint = QBicGeometry::GetLineOffsetPoints(oFrom, oTo, dOffset);
        
        TLine oLine;
        oLine.m_oFrom = vecPoint[0];
        oLine.m_oTo   = vecPoint[1];

        vecLine.push_back(oLine);
    }
    
    std::vector<QBicPoint> vecOffsetPoint;
    nxCount = vecLine.size();

    if (nxCount == 1)
    {
        TLine& oLine  = vecLine[0];
        vecOffsetPoint.push_back(oLine.m_oFrom);
        vecOffsetPoint.push_back(oLine.m_oTo);

        m_vecPoints.clear();
        AddPoint(vecOffsetPoint);
        return;
    }

    vecOffsetPoint.push_back(vecLine[0].m_oFrom);

    for (size_t i=0; i<nxCount-1; i++)
    {
        TLine& oFirstLine  = vecLine[i];
        TLine& oSecondLine = vecLine[i+1];

        QBicPoint oIP;
        bool bR = QBicGeometry::GetIntersectPoint(oFirstLine.m_oFrom, oFirstLine.m_oTo, oSecondLine.m_oFrom, oSecondLine.m_oTo, oIP);
                
        if (bR == true)
        {
            vecOffsetPoint.push_back(oIP);
        }
        else
        {
            vecOffsetPoint.push_back(oFirstLine.m_oTo);
        }
    }

    vecOffsetPoint.push_back(vecLine[nxCount-1].m_oTo);
    
    m_vecPoints.clear();
    AddPoint(vecOffsetPoint);
}

void QBicPolyline::Reverse()
{
    std::vector<QBicPoint> vecPoint;
    int nxCount = (int)m_vecPoints.size();
    for (int i=nxCount-1; i >= 0; i-- )
    {
        vecPoint.push_back(m_vecPoints[i]);
    }

    m_vecPoints.clear();
    AddPoint(vecPoint);
}

void QBicPolyline::GetNearPoint( const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist )
{
    double dTemp = 999999999.0;

    bool bFind(false);
    double dDistanceOnLine(0.0);
    double dDistanceShortestLine(0.0);

    QBicPoint oIP;

    size_t nxCount = m_vecPoints.size();
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oAFrom = m_vecPoints[i];
        QBicPoint& oATo   = m_vecPoints[i+1];

        QBicGeometry::GetShortestPoint(oAFrom, oATo, oCP, oIP, dDistanceOnLine, dDistanceShortestLine);
        if (dTemp > dDistanceShortestLine)
        {
            dTemp  = dDistanceShortestLine;
            oOutPt = oIP;
        }
    }

    dDist = dTemp;    
}

bool QBicPolyline::GetVerticalPoint( const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist )
{
    bool   bIsFind(false);
    double dTemp(999999999.0);

    QBicPoint oShortPt;
        
    double dDistanceOnLine(0.0);
    double dDistanceShortestLine(0.0);

    QBicPoint oIP;

    size_t nxCount = m_vecPoints.size();
    for (size_t i=0; i<nxCount-1; i++)
    {
        QBicPoint& oAFrom = m_vecPoints[i];
        QBicPoint& oATo   = m_vecPoints[i+1];

        int nResult = QBicGeometry::GetIntersectPoint(oAFrom, oATo, oCP, oIP, dDistanceOnLine, dDistanceShortestLine);

        if (dTemp > dDistanceShortestLine && nResult == 0)
        {
            dTemp    = dDistanceShortestLine;
            oShortPt = oIP;
            bIsFind  = true;
        }
    }

    if (bIsFind == true)
    {
        dDist   = dTemp;    
        oOutPt = oShortPt;
    }    

    return bIsFind;
}

