#include "QBicPolygon.h"
#include "QBicGeometry.h"
#include "QBicPoint.h"
#include "QBicPolyline.h"
#include "QBicEnvelope.h"

QBicPolygon::QBicPolygon()
{

}

QBicPolygon::~QBicPolygon()
{

}

void QBicPolygon::Clear()
{
    m_vecPoints.clear();
}

void QBicPolygon::AddPoint( double dX, double dY )
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

void QBicPolygon::AddPoint( std::vector<QBicPoint>& vecPoint )
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

QBicEnvelope QBicPolygon::GetEnvelope()
{
    return m_oEnvelope;
}

bool QBicPolygon::IsInPoint( QBicPoint& oPt )
{
    size_t nxPnCount  = m_vecPoints.size();

    if (nxPnCount < 3 )
        return false;

    if ( m_oEnvelope.IsIntersect(QBicEnvelope(oPt.X, oPt.Y, oPt.X, oPt.Y)) == false )
        return false;
    
    double dWidth  = m_oEnvelope.GetWidth();
    double dHeight = m_oEnvelope.GetHeight();
    
    int nRightTruePointCount(0);

    QBicPoint& oLnFrom = oPt;
    QBicPoint  oLnToRight   = oLnFrom;
    oLnToRight.X = oLnFrom.X + dWidth;

    for (size_t i=0; i<nxPnCount-1; i++)
    {
        QBicPoint& oPnFrom = m_vecPoints[i];
        QBicPoint& oPnTo   = m_vecPoints[i+1];
        
        QBicPoint oIP;
        bool bIntersect = QBicGeometry::GetIntersectPoint(oPnFrom, oPnTo, oLnFrom, oLnToRight, oIP, false);
        if (bIntersect)
        {
            nRightTruePointCount++;
        }    
    }
        
    if ( nRightTruePointCount % 2 == 0)
        return false;
    
    return true;
}


bool QBicPolygon::IsIntersect( QBicPolyline& rOther )
{
    size_t nxPnCount  = m_vecPoints.size();
    size_t nxPlCount  = rOther.m_vecPoints.size();

    if (nxPnCount < 3 || nxPlCount < 2)
        return false;

    if ( m_oEnvelope.IsIntersect(rOther.GetEnvelope()) == false )
        return false;
    
    for (size_t i=0; i<nxPlCount-1; i++)
    {
        QBicPoint& oLnFrom = rOther.m_vecPoints[i];
        QBicPoint& oLnTo   = rOther.m_vecPoints[i+1];               

        if (IsInPoint(oLnFrom) == true)
            return true;

        for (size_t j=0; j<nxPnCount-1; j++)
        {
            bool bIntersect(false);
            bool bReverse(false);

            QBicPoint& oPnFrom = m_vecPoints[j];
            QBicPoint& oPnTo   = m_vecPoints[j+1];

            QBicPoint oIP;
            bIntersect = QBicGeometry::GetIntersectPoint(oPnFrom, oPnTo, oLnFrom, oLnTo, oIP, false);   
            if (bIntersect == true)
                return true;
        }
    }
    
    return false;
}

bool QBicPolygon::IsContain( QBicPolyline& rOther )
{
    // 오류 :  --- U --->  모양 또한 True로 리턴..
    size_t nxCount  = rOther.m_vecPoints.size();

    if (m_vecPoints.size() < 3 || nxCount < 2)
        return false;

    if ( m_oEnvelope.IsIntersect(rOther.GetEnvelope()) == false )
        return false;

    size_t nxInPtCount(0);

    for (size_t i=0; i<nxCount; i++)
    {
        QBicPoint& oPt = rOther.m_vecPoints[i];
        if (IsInPoint(oPt) == true)
        {
            nxInPtCount++;
        }
        else
        {
            return false;
        }
    }

    return true;
}

void QBicPolygon::GetNearPoint( const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist )
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

double QBicPolygon::GetIntersectLineLength( QBicPolyline& _oPolyline )
{
    std::vector<QBicPoint> vecPolylineVertex = _oPolyline.GetPoints();
    size_t nxCount = vecPolylineVertex.size();

    double dTotalLength(0.0);
    for (size_t i=0; i<nxCount-1; i++)
    {//폴리라인을 루핑
        QBicPoint oPt1 = vecPolylineVertex[i];
        QBicPoint oPt2 = vecPolylineVertex[i+1];

        bool bInPt1 = IsInPoint(oPt1);
        bool bInPt2 = IsInPoint(oPt2);
                
        if (bInPt1 == true && bInPt2 == true) 
        {// 두점이 폴리곤 안에 있다면  
            double dLength = QBicGeometry::Length(oPt1, oPt2);
            dTotalLength += dLength;
        }
        else 
        {// 두점이 폴리곤 밖에 있는 경우, 두점 중 한쪽이 폴리곤 안에 있는 경우
            std::vector<QBicPoint> vecInterSectPt;
            size_t nxPvCount = m_vecPoints.size();
            for (size_t pv=0; pv<nxPvCount-1; pv++)
            {
                QBicPoint oV1 = m_vecPoints[pv];
                QBicPoint oV2 = m_vecPoints[pv+1];

                QBicPoint oIP;
                bool bIntersect = QBicGeometry::GetIntersectPoint(oPt1, oPt2, oV1, oV2, oIP, false);
                if (bIntersect)
                    vecInterSectPt.push_back(oIP);
            }

            // 두점이 폴리곤 밖에 있으면서 교차가 없는 것은 정말 밖에 있는 선분
            if ((bInPt1 == false && bInPt2 == false) && vecInterSectPt.size() == 0)
                continue;

            std::vector<QBicPoint> vecPt;
            if (bInPt1 == true)
                vecPt.push_back(oPt1);

            for (size_t m=0; m<vecInterSectPt.size(); m++)
                vecPt.push_back(vecInterSectPt[m]);

            if (bInPt2 == true)
                vecPt.push_back(oPt2);

            // 라인의 시점 또는 종점이 폴리곤 선상에 있다.
            if (vecPt.size() == 1) {
                continue;
            }

            for (size_t k=0; 2*k+1 < vecPt.size(); k++)
            {
                double dLength = QBicGeometry::Length(vecPt[2*k], vecPt[2*k+1]);
                dTotalLength += dLength;
            }
        }        
    }

    return dTotalLength;
}

double QBicPolygon::GetNearPointAvgDist( QBicPolyline& _oPolyline )
{
    std::vector<QBicPoint> vecPolylineVertex = _oPolyline.GetPoints();
    size_t nxCount = vecPolylineVertex.size();

    if (nxCount == 0)
        return 0.0;

    double dSumDist(0.0);
    double dAverage(0.0);

    for (size_t i=0; i<nxCount; i++)
    {
        QBicPoint oPt  = vecPolylineVertex[i];
        QBicPoint oOutPt;

        double dLength(0.0);
        GetNearPoint(oPt, oOutPt, dLength);

        dSumDist += dLength;
    }

    dAverage = dSumDist / (double) nxCount;

    return dAverage;
}
