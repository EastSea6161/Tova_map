#include "stdafx.h"
#include "ThunderMapDisp.h"
// grs80
// 장반경 : 6378137
// 단반경 : 6356752.3 

ThunderMapDisp::ThunderMapDisp(double dMonitorPixelDist) : m_oMapEnvelope(0.0, 0.0, 1000.0f, 1000.0f), 
	                                                       m_oScreenEnvelope(), m_dMapPixelDist(1.0), m_dMonitorPixelDist(dMonitorPixelDist)
{
    m_spThunderMapEnvelopeStatck = ThunderMapEnvelopeStatckPtr(new ThunderMapEnvelopeStatck());
}

ThunderMapDisp::~ThunderMapDisp()
{
}

std::shared_ptr<ThunderMapDisp> ThunderMapDisp::Copy()
{
    ThunderMapDisp* pTemp = new ThunderMapDisp(m_dMonitorPixelDist); {
        pTemp->m_dMapPixelDist    = m_dMapPixelDist;
        pTemp->m_dMapScale        = m_dMapScale;
        pTemp->m_oMapEnvelope     = m_oMapEnvelope;
        pTemp->m_oScreenEnvelope  = m_oScreenEnvelope;
        pTemp->m_dMapPixelDist    = m_dMapPixelDist;
        pTemp->m_oDrawMapEnvelope = m_oDrawMapEnvelope;
    }
    
    ThunderMapDispPtr spCopy = ThunderMapDispPtr(pTemp);
    return spCopy;
}

TxEnvelope ThunderMapDisp::MapEnvelope()
{
    return m_oDrawMapEnvelope;
}

void ThunderMapDisp::DrawEnvelopeAdjust()
{
    double dMapWidth     = m_oMapEnvelope.GetWidth();
    double dMapHeight    = m_oMapEnvelope.GetHeight();

    double dScreenWidth  = m_oScreenEnvelope.GetWidth();
    double dScreenHeight = m_oScreenEnvelope.GetHeight();

    if ( dMapWidth == 0.0 || dMapHeight == 0.0 || dScreenWidth == 0.0 || dScreenHeight == 0.0 ) {
        return;
    }

    double dScaleWidth  = dMapWidth  / dScreenWidth;
    double dScaleHeight = dMapHeight / dScreenHeight;

    //★ 2016-07-11 : 
    /*
    double dMinPixelMeter(1.0 / 1000.0);
    if (dScaleWidth <= dMinPixelMeter || dScaleHeight <= dMinPixelMeter)
        return; //더이상 확대 불가
    */
    //#########################################################################################################

    double dMapCenterX(0.0), dMapCenterY(0.0);
    m_oMapEnvelope.GetCenter(dMapCenterX, dMapCenterY);
        
    m_dMapPixelDist = (dScaleWidth + dScaleHeight ) * 0.5;
    
    double dNewMinX = dMapCenterX - m_dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMinY = dMapCenterY - m_dMapPixelDist * dScreenHeight * 0.5f;

    double dNewMaxX = dMapCenterX + m_dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMaxY = dMapCenterY + m_dMapPixelDist * dScreenHeight * 0.5f;

    m_oDrawMapEnvelope.SetEnvelope(dNewMinX, dNewMinY, dNewMaxX, dNewMaxY);     
}


void ThunderMapDisp::SetMapRect( const TxEnvelope& oMapEnvelope, bool bStack)
{   
    m_oMapEnvelope = oMapEnvelope;
    DrawEnvelopeAdjust();

    if (bStack)
        PushEnvelopeStack(oMapEnvelope);
}

void ThunderMapDisp::SetScreenRect( const TxEnvelope& oScreenEnvelope )
{
    m_oScreenEnvelope = oScreenEnvelope;
    DrawEnvelopeAdjust();   
}

bool ThunderMapDisp::Undo()
{
    TxEnvelope oEnvelope;
    if ( m_spThunderMapEnvelopeStatck->Undo(oEnvelope) == false)
        return false;

    m_oMapEnvelope = oEnvelope;
    DrawEnvelopeAdjust();
    return true;
}

bool ThunderMapDisp::Redo()
{
    TxEnvelope oEnvelope;
    if ( m_spThunderMapEnvelopeStatck->Redo(oEnvelope) == false)
        return false;

    m_oMapEnvelope = oEnvelope;
    DrawEnvelopeAdjust();
    return true;
}

TxPoint ThunderMapDisp::ScreenToMap( double x, double y )
{
    double dX = m_oDrawMapEnvelope.GetMinX() + (x - m_oScreenEnvelope.GetMinX()) * m_dMapPixelDist;
    double dY = m_oDrawMapEnvelope.GetMinY() + (m_oScreenEnvelope.GetMaxY() - y) * m_dMapPixelDist;

    return TxPoint(dX, dY);
}

TxPoint ThunderMapDisp::ScreenToMap( const TxPoint& oPt )
{
   return ScreenToMap(oPt.X, oPt.Y);
}

TxPoint ThunderMapDisp::ScreenToMap( const CPoint& oPt )
{
    return ScreenToMap((double)oPt.x, (double)oPt.y);
}

TxPoint ThunderMapDisp::ScreenToMap( long x, long y )
{
    return ScreenToMap((double)x, (double)y);
}

TxPoint ThunderMapDisp::ScreenToMap( int x, int y )
{
    return ScreenToMap((double)x, (double)y);
}

TxPoint ThunderMapDisp::MapToScreen( const TxPoint& oPt)
{   
    double dX = m_oScreenEnvelope.GetMinX() + (oPt.X - m_oDrawMapEnvelope.GetMinX()) / m_dMapPixelDist;
    double dY = m_oScreenEnvelope.GetMaxY() - (oPt.Y - m_oDrawMapEnvelope.GetMinY()) / m_dMapPixelDist;

    return TxPoint(dX, dY);
}

TxPoint ThunderMapDisp::MapToScreen( TxPoint* pPoint )
{
    double dX = m_oScreenEnvelope.GetMinX() + (pPoint->X - m_oDrawMapEnvelope.GetMinX())  / m_dMapPixelDist;
    double dY = m_oScreenEnvelope.GetMaxY() - (pPoint->Y - m_oDrawMapEnvelope.GetMinY() ) / m_dMapPixelDist;

    return TxPoint(dX, dY);
}

TxPoint ThunderMapDisp::MapToScreen( TxPointPtr spPoint )
{
    double dX = m_oScreenEnvelope.GetMinX() + (spPoint->X - m_oDrawMapEnvelope.GetMinX())  / m_dMapPixelDist;
    double dY = m_oScreenEnvelope.GetMaxY() - (spPoint->Y - m_oDrawMapEnvelope.GetMinY() ) / m_dMapPixelDist;

    return TxPoint(dX, dY);
}

TxPoint ThunderMapDisp::MapToScreen( double X, double Y )
{
    double dX = m_oScreenEnvelope.GetMinX() + (X - m_oDrawMapEnvelope.GetMinX())  / m_dMapPixelDist;
    double dY = m_oScreenEnvelope.GetMaxY() - (Y - m_oDrawMapEnvelope.GetMinY() ) / m_dMapPixelDist;

    return TxPoint(dX, dY);
}

double ThunderMapDisp::GetScale()
{	
	return m_dMapPixelDist / m_dMonitorPixelDist;
}

void ThunderMapDisp::SetScale( double dScale )
{
    double dScreenWidth  = m_oScreenEnvelope.GetWidth();
    double dScreenHeight = m_oScreenEnvelope.GetHeight();

    double dMapCenterX(0.0), dMapCenterY(0.0); {
        m_oMapEnvelope.GetCenter(dMapCenterX, dMapCenterY);
    }

    m_dMapPixelDist = dScale * m_dMonitorPixelDist;

    double dNewMinX = dMapCenterX - m_dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMinY = dMapCenterY - m_dMapPixelDist * dScreenHeight * 0.5f;

    double dNewMaxX = dMapCenterX + m_dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMaxY = dMapCenterY + m_dMapPixelDist * dScreenHeight * 0.5f;

    m_oDrawMapEnvelope.SetEnvelope(dNewMinX, dNewMinY, dNewMaxX, dNewMaxY);     
}

TxEnvelope ThunderMapDisp::GetScaleEnvelope( double dScale )
{
    double dScreenWidth  = m_oScreenEnvelope.GetWidth();
    double dScreenHeight = m_oScreenEnvelope.GetHeight();

    double dMapCenterX(0.0), dMapCenterY(0.0); {
        m_oMapEnvelope.GetCenter(dMapCenterX, dMapCenterY);
    }

    double dMapPixelDist = dScale * m_dMonitorPixelDist;

    double dNewMinX = dMapCenterX - dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMinY = dMapCenterY - dMapPixelDist * dScreenHeight * 0.5f;

    double dNewMaxX = dMapCenterX + dMapPixelDist * dScreenWidth  * 0.5f;
    double dNewMaxY = dMapCenterY + dMapPixelDist * dScreenHeight * 0.5f;

    TxEnvelope oEnvelope(dNewMinX, dNewMinY, dNewMaxX, dNewMaxY);
    return oEnvelope;
}

double ThunderMapDisp::GetPixelToDist()
{
	return m_dMapPixelDist;
}

double ThunderMapDisp::GetPointToDist()
{
#pragma region comment
// Point : Pixel = 72 * 96
// Point = 72/96 * Pixel
#pragma endregion comment
    
    double dPixelDist = GetPixelToDist();
    return (72.0 / 96.0 ) * dPixelDist;
}

double ThunderMapDisp::GetDistToPoint( double dDist )
{
    // 1Pixel의 거리
    double dPixelDist  = GetPixelToDist();
    double dPixelCount = dDist / dPixelDist;

    return (72.0 / 96.0 ) * dPixelCount;
}

double ThunderMapDisp::GetDistToPixel( double dDist )
{
    double dPixelDist  = GetPixelToDist();
    double dPixelCount = dDist / dPixelDist;

    return dPixelCount;
}




