#include "stdafx.h"
#include "TxEnvelope.h"
#include "TxPoint.h"

TxEnvelope::TxEnvelope()
{
    m_dMinX = 0.0;
    m_dMinY = 0.0;
    m_dMaxX = 0.0;
    m_dMaxY = 0.0;
}

TxEnvelope::TxEnvelope( double dMinX, double dMinY )
{
    m_dMinX = dMinX;
    m_dMaxX = dMinX;

    m_dMinY = dMinY;
    m_dMaxY = dMinY;
}

TxEnvelope::TxEnvelope( double dCx, double dCy, double dSquareWidth )
{
    double dTemp = dSquareWidth * 0.5;

    m_dMinX = dCx - dTemp;
    m_dMinY = dCy - dTemp;

    m_dMaxX = dCx + dTemp;    
    m_dMaxY = dCy + dTemp;
}

TxEnvelope::TxEnvelope( double dMinX, double dMinY, double dMaxX, double dMaxY )
{
    SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

TxEnvelope::TxEnvelope( const TxEnvelope& right )
{
    m_dMinX = right.m_dMinX;
    m_dMaxX = right.m_dMaxX;

    m_dMinY = right.m_dMinY;
    m_dMaxY = right.m_dMaxY;
}

TxEnvelope::TxEnvelope( RECT& rc )
{
    m_dMinX = (double) rc.left;
    m_dMaxX = (double) rc.right;

    //화면 좌표계를 Geo 좌표로 변환
    m_dMinY = (double) rc.top;
    m_dMaxY = (double) rc.bottom;

    SetEnvelope(m_dMinX, m_dMinY, m_dMaxX, m_dMaxY);
}

TxEnvelope::TxEnvelope( BoostEnvelope& right )
{
    BoostPoint oMinPt = right.min_corner();
    BoostPoint oMaxPt = right.max_corner();

    double minPt[2] = {0.0};    
    minPt[0] = oMinPt.get<0>();
    minPt[1] = oMinPt.get<1>();

    double maxPt[2] = {0};    
    maxPt[0] = oMaxPt.get<0>();
    maxPt[1] = oMaxPt.get<1>();

    SetEnvelope(minPt[0], minPt[1], maxPt[0], maxPt[1]);
}


TxEnvelope::~TxEnvelope()
{

}

TxEnvelope& TxEnvelope::operator=( const TxEnvelope& right )
{
    if (&right == this)
        return *this;

    m_dMinX = right.m_dMinX;
    m_dMaxX = right.m_dMaxX;

    m_dMinY = right.m_dMinY;
    m_dMaxY = right.m_dMaxY;

    return *this;
}

TxEnvelope TxEnvelope::operator+( const TxEnvelope& right )
{
    double dMinX(0.0);
    double dMinY(0.0);
    double dMaxX(0.0);
    double dMaxY(0.0);

    if (m_dMinX < right.m_dMinX)
        dMinX = m_dMinX;
    else
        dMinX = right.m_dMinX;

    if (m_dMinY < right.m_dMinY)
        dMinY = m_dMinY;
    else
        dMinY = right.m_dMinY;

    if (m_dMaxX > right.m_dMaxX)
        dMaxX = m_dMaxX;
    else
        dMaxX = right.m_dMaxX;

    if (m_dMaxY > right.m_dMaxY)
        dMaxY = m_dMaxY;
    else
        dMaxY = right.m_dMaxY;


    return TxEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

TxEnvelope& TxEnvelope::operator+=( const TxEnvelope& right )
{
    double dMinX(0.0);
    double dMinY(0.0);
    double dMaxX(0.0);
    double dMaxY(0.0);

    if (m_dMinX < right.m_dMinX)
        dMinX = m_dMinX;
    else
        dMinX = right.m_dMinX;

    if (m_dMinY < right.m_dMinY)
        dMinY = m_dMinY;
    else
        dMinY = right.m_dMinY;

    if (m_dMaxX > right.m_dMaxX)
        dMaxX = m_dMaxX;
    else
        dMaxX = right.m_dMaxX;

    if (m_dMaxY > right.m_dMaxY)
        dMaxY = m_dMaxY;
    else
        dMaxY = right.m_dMaxY;

    SetEnvelope( dMinX, dMinY, dMaxX, dMaxY );

    return *this;
}

void TxEnvelope::SetEnvelope( double dMinX, double dMinY, double dMaxX, double dMaxY )
{
    if (dMinX < dMaxX)
    {
        m_dMinX = dMinX;
        m_dMaxX = dMaxX;
    }
    else
    {
        m_dMinX = dMaxX;
        m_dMaxX = dMinX;
    }

    if (dMinY < dMaxY)
    {
        m_dMinY = dMinY;
        m_dMaxY = dMaxY;
    }
    else
    {
        m_dMinY = dMaxY;
        m_dMaxY = dMinY;
    }
}

void TxEnvelope::GetEnvelope( double& dMinX, double& dMinY, double& dMaxX, double& dMaxY ) const
{// double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0) 
 // dMinX, dMinY, dMaxX, dMaxY
    dMinX = m_dMinX;
    dMinY = m_dMinY;

    dMaxX = m_dMaxX;
    dMaxY = m_dMaxY;
}

void TxEnvelope::GetCenter( double& dX, double& dY ) const
{
    dX = (m_dMaxX + m_dMinX) * 0.5;
    dY = (m_dMaxY + m_dMinY) * 0.5;
}

std::vector<TxPoint> TxEnvelope::GetPoints() const
{
    // 좌측하단에서 시계방향으로 돌린다. 마지막 포인트를 포함
    std::vector<TxPoint> vecPoint;

    vecPoint.push_back(TxPoint(m_dMinX, m_dMinY));
    vecPoint.push_back(TxPoint(m_dMinX, m_dMaxY));
    vecPoint.push_back(TxPoint(m_dMaxX, m_dMaxY));
    vecPoint.push_back(TxPoint(m_dMaxX, m_dMinY));
    vecPoint.push_back(TxPoint(m_dMinX, m_dMinY));

    return vecPoint;
}

void TxEnvelope::Move( double dNewX, double dNewY )
{
    double dHalfWidth  = (m_dMaxX - m_dMinX) * 0.5;
    double dHalfHeight = (m_dMaxY - m_dMinY) * 0.5;

    double dMinX = dNewX - dHalfWidth;
    double dMinY = dNewY - dHalfHeight;

    double dMaxX = dNewX + dHalfWidth;
    double dMaxY = dNewY + dHalfHeight;

    SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

double TxEnvelope::GetWidth() const
{
    return m_dMaxX - m_dMinX;
}

double TxEnvelope::GetHeight() const
{
    return m_dMaxY - m_dMinY;
}

void TxEnvelope::Offset( double dOffset )
{
    if (dOffset >= 0.0)
    {
        m_dMinX -= dOffset;
        m_dMaxX += dOffset;

        m_dMinY -= dOffset;
        m_dMaxY += dOffset;
    }
    else
    {
        double dWidth  = m_dMaxX - m_dMinX;
        double dHeight = m_dMaxY - m_dMinY;

        // Shrink (-)
        dOffset = -1.0 * dOffset;

        if (dWidth * 0.5 >= dOffset)
        {
            m_dMinX += dOffset;
            m_dMaxX -= dOffset;            
        }
        else
        {
            double dTemp = m_dMinX;
            m_dMinX = m_dMaxX - dOffset;
            m_dMaxX = dTemp   + dOffset;
        }

        if (dHeight * 0.5 >= dOffset)
        {
            m_dMinY += dOffset;
            m_dMaxY -= dOffset;
        }
        else
        {
            double dTemp = m_dMinY;
            m_dMinY = m_dMaxY - dOffset;
            m_dMaxY = dTemp   + dOffset;
        }
    }
}

void TxEnvelope::Expansion( double dRate )
{
    double dCx(0), dCy(0); {
        GetCenter(dCx, dCy);
    }

    if (dRate <= 0) {
        m_dMinX = dCx;
        m_dMaxX = dCx;
        m_dMinY = dCy;
        m_dMaxY = dCy;

        return;
    }

    double dHalfWidth  = (m_dMaxX - m_dMinX) * dRate * 0.5;
    double dHalfHeight = (m_dMaxY - m_dMinY) * dRate * 0.5;

    m_dMinX = dCx - dHalfWidth;
    m_dMaxX = dCx + dHalfWidth;
    m_dMinY = dCy - dHalfHeight;
    m_dMaxY = dCy + dHalfHeight;
}

void TxEnvelope::Shift( double dX, double dY )
{
    m_dMinX = m_dMinX + dX;
    m_dMaxX = m_dMaxX + dX;
    m_dMinY = m_dMinY + dY;
    m_dMaxY = m_dMaxY + dY;
}

bool TxEnvelope::Intersect( const TxEnvelope& rOther ) const
{
    if ( rOther.Contains(*this) == true )
        return true;

    // 왼쪽
    if (rOther.m_dMaxX < m_dMinX)
        return false;

    // 오른쪽
    if (rOther.m_dMinX > m_dMaxX)
        return false;

    // 아래쪽
    if (rOther.m_dMaxY < m_dMinY)
        return false;

    // 위쪽
    if (rOther.m_dMinY > m_dMaxY)
        return false;

    return true;
}

bool TxEnvelope::Contains( const TxEnvelope& rOther ) const
{
    try
    {
        bool bX = (m_dMinX <= rOther.m_dMinX) && (m_dMaxX >= rOther.m_dMaxX);
        bool bY = (m_dMinY <= rOther.m_dMinY) && (m_dMaxY >= rOther.m_dMaxY);

        return bX && bY;
    }
    catch (...)
    {
    	TxLogDebugException();
        throw -300;
    }
}

bool TxEnvelope::Equal( const TxEnvelope& right )
{
    if (m_dMinX == right.m_dMinX &&  m_dMinY == right.m_dMinY && m_dMaxX == right.m_dMaxX &&  m_dMaxY == right.m_dMaxY) {
        return true;
    }

    return false;
}

void TxEnvelope::Reset()
{
    m_dMinX = 0.0;
    m_dMinY = 0.0;
    m_dMaxX = 0.0;
    m_dMaxY = 0.0;
}

void TxEnvelope::Union( const TxEnvelope& oEnvelope )
{
    double dMinX(0.0);
    double dMinY(0.0);
    double dMaxX(0.0);
    double dMaxY(0.0);

    if (m_dMinX < oEnvelope.m_dMinX)
        dMinX = m_dMinX;
    else
        dMinX = oEnvelope.m_dMinX;

    if (m_dMinY < oEnvelope.m_dMinY)
        dMinY = m_dMinY;
    else
        dMinY = oEnvelope.m_dMinY;

    if (m_dMaxX > oEnvelope.m_dMaxX)
        dMaxX = m_dMaxX;
    else
        dMaxX = oEnvelope.m_dMaxX;

    if (m_dMaxY > oEnvelope.m_dMaxY)
        dMaxY = m_dMaxY;
    else
        dMaxY = oEnvelope.m_dMaxY;

    SetEnvelope( dMinX, dMinY, dMaxX, dMaxY );

}

bool TxEnvelope::IsInPoint( const TxPoint& _oPt ) const
{
    if (m_dMaxX < _oPt.X)
        return false;

    if (m_dMinX > _oPt.X)
        return false;

    if (m_dMaxY < _oPt.Y)
        return false;

    if (m_dMinY > _oPt.Y)
        return false;

    return true;
}

double TxEnvelope::Area() const
{
    return (m_dMaxX - m_dMinX) * (m_dMaxY - m_dMinY);
}

CString TxEnvelope::ToString() const
{
    CString strOut(_T(""));
    strOut.Format(_T("min[%.2f,%.2f], max[%.2f,%.2f]"), m_dMinX, m_dMinY, m_dMaxX, m_dMaxY);

    return strOut;
}
