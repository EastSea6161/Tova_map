#include "stdafx.h"
#include "TxMapEnumType.h"
#include "TxPoint.h"
#include "TxConst.h"
#include "TxMath.h"

TxPoint::TxPoint() : ITxGeometry(GeoPointType), X(0.0), Y(0.0)
{
}

TxPoint::TxPoint(double dX, double dY) : ITxGeometry(GeoPointType), X(dX), Y(dY) 
{
}

TxPoint::TxPoint(int nX, int nY) : ITxGeometry(GeoPointType), X((double)nX), Y((double)nY) 
{
}

TxPoint::TxPoint( long nX, long nY ) : ITxGeometry(GeoPointType), X((double)nX), Y((double)nY) 
{

}

TxPoint::TxPoint( const TxPoint& right ) : ITxGeometry(GeoPointType)
{
    X = right.X;
    Y = right.Y;
}

TxPoint::TxPoint( QBicBinary& oBinary, size_t nIndex ) : ITxGeometry(GeoPointType)
{
    int nGeometryType(0);
    oBinary.Position(nIndex);
    oBinary.Read(nGeometryType);

    BinDouble dX(0), dY(0);
    oBinary.Read(dX);
    oBinary.Read(dY);

    X = (double)dX;
    Y = (double)dY;
}

TxPoint::TxPoint( std::shared_ptr<TxPoint> spPt ) : ITxGeometry(GeoPointType)
{
    X = spPt->X;
    Y = spPt->Y;
}

TxPoint::~TxPoint()
{
}

bool TxPoint::operator==( const TxPoint& right ) const
{
    if ( (abs(right.X - X) < TxConst::Epsilon()) && (abs(right.Y - Y) < TxConst::Epsilon()) )
    {
        return true;
    }

    return false;
}

bool TxPoint::operator != ( const TxPoint& right ) const
{
    if ( (abs(right.X - X) < TxConst::Epsilon()) && (abs(right.Y - Y) < TxConst::Epsilon()) )
    {
        return false;
    }

    return true;
}


void TxPoint::PutCoords( double dX, double dY )
{
    X = dX;
    Y = dY;
}

TxEnvelope TxPoint::Envelope() const
{
    return TxEnvelope(X, Y);
}

TxPoint& TxPoint::operator=( const TxPoint& right ) 
{
    if (&right == this)
        return *this;

    X = right.X;
    Y = right.Y;

    return *this;
}

void TxPoint::GetBinary( QBicBinary& oBinary )
{   
    oBinary.PushBack((int)m_enGeometryType);
    oBinary.PushBack((BinDouble)X);
    oBinary.PushBack((BinDouble)Y);
}

CString TxPoint::ToString()
{
    CString strOut(_T(""));
    strOut.Format(_T("Point : (%f, %f)"), X, Y);

    return strOut;
}

Gdiplus::RectF TxPoint::ToScreenGdiPlusRectF( double dWidth, double dHeight )
{
    if (dWidth < 0.0)
        dWidth *= -1.0f;

    if (dHeight < 0.0)
        dHeight *= -1.0f;

    double dXmin = X - 0.5f * dWidth;
    double dYmin = Y - 0.5f * dHeight;

    //★ 화면좌표계 
    //★ Gdiplus::RectF - XMin, YMax, Width, Height -> Y축이 반대이므로... YMax -> YMin
    return Gdiplus::RectF((float)dXmin, (float)dYmin, (float)dWidth, (float)dHeight);
}

double TxPoint::MinDist( const TxPoint& oPt )
{
    return TxMath::Dist(X, Y, oPt.X, oPt.Y);
}

bool TxPoint::Intersect( const TxEnvelope& _oEnvelope ) const
{
    if (_oEnvelope.GetMaxX() < X)
        return false;

    if (_oEnvelope.GetMinX() > X)
        return false;

    if (_oEnvelope.GetMaxY() < Y)
        return false;

    if (_oEnvelope.GetMinY() > Y)
        return false;

    return true;
}

TxMPoint::TxMPoint() : ITxGeometry(GeoMPointType)
{
}

TxMPoint::TxMPoint( const TxMPoint& right )
{
     m_vecPoint = right.m_vecPoint;
}

TxMPoint::TxMPoint( QBicBinary& oBinary, size_t nIndex/*=0*/ )
{
    int nGeometryType(0);
    int nSize(0);

    oBinary.Position(nIndex);
    oBinary.Read(nGeometryType);
    oBinary.Read(nSize);

    for(int i=0; i<nSize; i++) {
        BinDouble dX(0), dY(0);
        oBinary.Read(dX);
        oBinary.Read(dY);

        m_vecPoint.push_back(TxPoint(dX, dY));
    }
}

TxMPoint::~TxMPoint()
{

}

TxMPoint& TxMPoint::operator=( const TxMPoint& right )
{
    if (&right == this)
        return *this;

    m_vecPoint = right.m_vecPoint;
    return *this;
}


TxEnvelope TxMPoint::Envelope() const
{
    size_t nxCount = m_vecPoint.size();
    if (nxCount == 0)
        return TxEnvelope(0.0, 0.0);

    double dMinX(9999999999.0f);
    double dMinY(9999999999.0f);
    double dMaxX(-9999999999.0f);
    double dMaxY(-9999999999.0f);

    for (size_t i=0; i<m_vecPoint.size(); i++)
    {
        const TxPoint& oPt = m_vecPoint[i];

        if (dMinX > oPt.X)
            dMinX = oPt.X;
        if (dMinY > oPt.Y)
            dMinY = oPt.Y;

        if (dMaxX < oPt.X)
            dMaxX = oPt.X;
        if (dMaxY < oPt.Y)
            dMaxY = oPt.Y;
    }

    return TxEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

void TxMPoint::GetBinary( QBicBinary& oBinary )
{   
    oBinary.PushBack((int)m_enGeometryType);
    
    int nSize = (int)m_vecPoint.size();
    oBinary.PushBack(nSize);

    for (size_t i=0; i<m_vecPoint.size(); i++)
    {
        TxPoint& oPt = m_vecPoint[i];
        oBinary.PushBack(oPt.X);
        oBinary.PushBack(oPt.Y);
    }
}

double TxMPoint::MinDist( const TxPoint& oPt )
{
    double dMinDist(TxConst::MaxDbl());
    for (size_t i=0; i<m_vecPoint.size(); i++)
    {
        TxPoint& oPtLoop = m_vecPoint[i];
        double dDist = oPtLoop.MinDist(oPt);

        if (dMinDist > dDist)
            dMinDist = dDist;
    }

    return dMinDist;
}


bool TxMPoint::Intersect( const TxEnvelope& _oEnvelope ) const
{
    bool bResult(false);
    for (size_t i=0; i<m_vecPoint.size(); i++)
    {
        const TxPoint& oPtLoop = m_vecPoint[i];

        if ( oPtLoop.Intersect(_oEnvelope) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}


