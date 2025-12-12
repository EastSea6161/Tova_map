#include "stdafx.h"
#include "ThunderMapCommandZoomOut.h"
#include "TxConvertUtil.h"

ThunderMapCommandZoomOut::ThunderMapCommandZoomOut( IThunderMap* pThunderMap, Gdiplus::Color crBackground)
	: IThunderMapCommand(pThunderMap, ZoomOutCommand, crBackground)
{

}

ThunderMapCommandZoomOut::~ThunderMapCommandZoomOut()
{

}

void ThunderMapCommandZoomOut::OnLButtonDown( UINT nFlags, CPoint point )
{
    ThunderMapDispPtr spThunderMapDisp = m_pThunderMap->GetThunderMapDisp();

    TxEnvelope oMapEnvelope = spThunderMapDisp->MapEnvelope();
    TxEnvelope oPreEnvelope = oMapEnvelope;

    double dPixelDist = spThunderMapDisp->GetPixelToDist();
    double dPercent(1.5);

    //★ 현재 위치한 마우스의 위치 정보를 유지
    //★ 현재 마우스 커서의 맵 좌표
    TxPoint oPreMousePt = spThunderMapDisp->ScreenToMap(point);

    oMapEnvelope.Expansion(dPercent);   
    spThunderMapDisp->SetMapRect(oMapEnvelope);

    //★ 변경 위치에서의 마우스 커서의 맵 좌표
    TxPoint oNxtMousePt = spThunderMapDisp->ScreenToMap(point);

    double dGapWidth  = oPreMousePt.X - oNxtMousePt.X;
    double dGapHeight = oPreMousePt.Y - oNxtMousePt.Y;

    //★ 중심 이동
    oMapEnvelope.Shift(dGapWidth, dGapHeight);

    //★ 최종 영역 정의
    spThunderMapDisp->SetMapRect(oMapEnvelope);

    TxPoint ptLeftTop     = spThunderMapDisp->MapToScreen(TxPoint(oPreEnvelope.GetMinX(), oPreEnvelope.GetMaxY()));
    TxPoint ptRightBottom = spThunderMapDisp->MapToScreen(TxPoint(oPreEnvelope.GetMaxX(), oPreEnvelope.GetMinY()));

    Gdiplus::REAL  rWidth  = (Gdiplus::REAL)(ptRightBottom.X - ptLeftTop.X);
    Gdiplus::REAL  rHeight = (Gdiplus::REAL)(ptRightBottom.Y - ptLeftTop.Y);
    Gdiplus::RectF oDestRect((Gdiplus::REAL)ptLeftTop.X, (Gdiplus::REAL)ptLeftTop.Y, rWidth, rHeight);

    RubberBandDraw(oDestRect);
    m_pThunderMap->Refresh();

#pragma region OldVersion
    /*HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    CBitmap bitmap;
    {
        RECT  rc; ::GetClientRect(hWnd, &rc);
        CRect rect(rc);

        // 현재의 지도이미지를 복사한다.
        CClientDC oDC(pHandleWnd);
        CDC       mDC; 
        mDC.CreateCompatibleDC(&oDC);

        // 마우스 Up에서 Delete 한다.        
        bitmap.CreateCompatibleBitmap(&oDC, rect.Width(), rect.Height());
        CBitmap* pOldBitmap = mDC.SelectObject(&bitmap);

        // 현재 화면을 복사
        mDC.BitBlt(0, 0, rect.Width(), rect.Height(), &oDC, 0, 0, SRCCOPY);

        mDC.SelectObject(pOldBitmap);
        mDC.DeleteDC();
    }
        
    RECT  rc; ::GetClientRect(hWnd, &rc);
    CRect rect(rc);

    CClientDC oDC(pHandleWnd);
    CDC       bufferDC; 
    bufferDC.CreateCompatibleDC(&oDC);

    CBitmap oBufferBitmap; 
    oBufferBitmap.CreateCompatibleBitmap(&oDC, rect.Width(), rect.Height());

    // 메모리 비트맵 생성
    CBitmap* pOldBitmap1 = bufferDC.SelectObject(&oBufferBitmap);    

    // 배경을 칠한다.
    CBrush brushBackGround;
    brushBackGround.CreateSolidBrush(RGB(255, 255, 255)); 

    CBrush* pOldBrush1 = bufferDC.SelectObject(&brushBackGround);
    bufferDC.FillRect(rect, &brushBackGround);

    //디버깅 : oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY); 
    CDC bufferPreImageDC;
    bufferPreImageDC.CreateCompatibleDC(&oDC);    

    CBitmap* pOldBitmap2 = bufferPreImageDC.SelectObject(&bitmap);            
        
    ThunderMapDispPtr spThunderMapDisp = m_pThunderMap->GetThunderMapDisp();

    TxPoint tPt = spThunderMapDisp->ScreenToMap(0, 0);    
    TxEnvelope oMapEnvelope = spThunderMapDisp->MapEnvelope();

    //★ 현재 위치한 마우스의 위치 정보를 유지
    //★ 현재 마우스 커서의 맵 좌표
    TxPoint oPreMousePt = spThunderMapDisp->ScreenToMap(point);

    oMapEnvelope.Expansion(1.5);   
    spThunderMapDisp->SetMapRect(oMapEnvelope);

    //★ 변경 위치에서의 마우스 커서의 맵 좌표
    TxPoint oNxtMousePt = spThunderMapDisp->ScreenToMap(point);

    double dGapWidth  = oPreMousePt.X - oNxtMousePt.X;
    double dGapHeight = oPreMousePt.Y - oNxtMousePt.Y;

    //★ 중심 이동
    oMapEnvelope.Shift(dGapWidth, dGapHeight);

    //★ 최종 영역 정의
    spThunderMapDisp->SetMapRect(oMapEnvelope);
    
    TxPoint oScreenPt = spThunderMapDisp->MapToScreen(tPt);

    int nLeft = (int)oScreenPt.X;
    int nTop  = (int)oScreenPt.Y;

    int nWidth  = rect.Width()  - 2 * nLeft;
    int nHeight = rect.Height() - 2 * nTop;
    
    bufferDC.StretchBlt(nLeft, nTop, nWidth, nHeight, &bufferPreImageDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);    
    oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY);    

    bufferDC.SelectObject(pOldBrush1);
    bufferDC.SelectObject(pOldBitmap1);    
    brushBackGround.DeleteObject();

    bufferPreImageDC.SelectObject(pOldBitmap2);

    bufferDC.DeleteDC();
    bufferPreImageDC.DeleteDC();    
    m_pThunderMap->Refresh();*/
#pragma endregion OldVersion
}

void ThunderMapCommandZoomOut::RubberBandDraw( Gdiplus::RectF& oDestRect )
{
    ThunderMapdrawProcessPtr spThunderDraw = m_pThunderMap->GetThunderDxDraw();        
    Gdiplus::Bitmap* pBitMap = spThunderDraw->MapImage();
    if (pBitMap == nullptr) 
        return;

    HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

    RECT  rc; ::GetClientRect(hWnd, &rc);
    CRect rect(rc);

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    CClientDC oDC(pHandleWnd);    
    Gdiplus::REAL rImageWidth  = (Gdiplus::REAL)rect.Width();
    Gdiplus::REAL rImageHeight = (Gdiplus::REAL)rect.Height();

    Graphics graphics(oDC);
    Bitmap gdiBmpStack(rect.Width(), rect.Height()/*, PixelFormat32bppPARGB*/); {
        Graphics graphicsBack(&gdiBmpStack);
        graphicsBack.Clear(Color::White);
        graphicsBack.DrawImage(pBitMap, oDestRect, 0, 0, rImageWidth, rImageHeight, UnitPixel);
    }

    graphics.DrawImage(&gdiBmpStack, 0, 0);

    delete pBitMap;
}


void ThunderMapCommandZoomOut::OnMouseMove( UINT nFlags, CPoint point )
{
    
}

void ThunderMapCommandZoomOut::OnLButtonUp( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnLButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnRButtonDown( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnRButtonUp( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnRButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnMButtonDown( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomOut::OnMButtonUp( UINT nFlags, CPoint point )
{

}

void ThunderMapCommandZoomOut::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
}