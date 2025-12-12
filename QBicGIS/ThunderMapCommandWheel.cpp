#include "stdafx.h"
#include "ThunderMapCommandWheel.h"

ThunderMapCommandWheel::ThunderMapCommandWheel( IThunderMap* pThunderMap, Gdiplus::Color crBackground)
	: IThunderMapCommand(pThunderMap, MPanCommand, crBackground)
{
}

ThunderMapCommandWheel::~ThunderMapCommandWheel()
{
}

void ThunderMapCommandWheel::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
    ThunderMapDispPtr spThunderMapDisp = m_pThunderMap->GetThunderMapDisp();
    
    TxEnvelope oMapEnvelope = spThunderMapDisp->MapEnvelope();
    TxEnvelope oPreEnvelope = oMapEnvelope;
    
    double dPixelDist = spThunderMapDisp->GetPixelToDist();
    double dPercent(0.0);
    if (dPixelDist > 100) {
        if (0 <= zDelta) /*확대*/
            dPercent = 0.5;
        else
            dPercent = 4.0;
    }
    else {
        if (0 <= zDelta) /*확대*/
            dPercent = 0.5;
        else
            dPercent = 4.0;
    }    
    
    //★ 현재 위치한 마우스의 위치 정보를 유지
    //★ 현재 마우스 커서의 맵 좌표
    TxPoint oPreMousePt = spThunderMapDisp->ScreenToMap(point);

    double dScale = spThunderMapDisp->GetScale(); {
        dScale = dScale * dPercent;
    }

    int nCnt(0); {
        CString strTemp(_T("")); {
            strTemp.Format(_T("%.0f"), dScale);
        }

        nCnt = strTemp.GetLength();
    }
    
    if (nCnt >= 3) {
        dScale = dScale / (pow(10.0, nCnt-2));
        dScale = pow(10.0, nCnt-2) * (int)dScale;        
    }
    
    oMapEnvelope = spThunderMapDisp->GetScaleEnvelope(dScale);
    //oMapEnvelope.Expansion(dPercent);   
    spThunderMapDisp->SetMapRect(oMapEnvelope);

    //★ 변경 위치에서의 마우스 커서의 맵 좌표
    TxPoint oNxtMousePt = spThunderMapDisp->ScreenToMap(point);
    
    double dGapWidth  = oPreMousePt.X - oNxtMousePt.X;
    double dGapHeight = oPreMousePt.Y - oNxtMousePt.Y;

    //★ 조정된 Envelope
    oMapEnvelope = spThunderMapDisp->MapEnvelope();
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
}

void ThunderMapCommandWheel::RubberBandDraw(Gdiplus::RectF& oDestRect)
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
        //graphicsBack.Clear(Color::White);
		graphicsBack.Clear(m_crBackground);
        graphicsBack.DrawImage(pBitMap, oDestRect, 0, 0, rImageWidth, rImageHeight, UnitPixel);
    }

    graphics.DrawImage(&gdiBmpStack, 0, 0);

    delete pBitMap;
}

void ThunderMapCommandWheel::RubberBandDraw( TxEnvelope& oDestRect )
{
    HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

    RECT  rc; ::GetClientRect(hWnd, &rc);
    CRect rect(rc);

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    CClientDC oDC(pHandleWnd);
    CBitmap bitmap;
    {// 현재의 지도이미지를 복사한다.        
        CDC mDC; 
        mDC.CreateCompatibleDC(&oDC);

        // 마우스 Up에서 Delete 한다.        
        bitmap.CreateCompatibleBitmap(&oDC, rect.Width(), rect.Height());
        CBitmap* pOldBitmap = mDC.SelectObject(&bitmap);

        // 현재 화면을 복사
        mDC.BitBlt(0, 0, rect.Width(), rect.Height(), &oDC, 0, 0, SRCCOPY);

        mDC.SelectObject(pOldBitmap);
        mDC.DeleteDC();
    }

    CDC bufferDC; 
    bufferDC.CreateCompatibleDC(&oDC);

    CBitmap oBufferBitmap; 
    oBufferBitmap.CreateCompatibleBitmap(&oDC, rect.Width(), rect.Height());

    // 메모리 비트맵 생성
    CBitmap* pOldBitmap1 = bufferDC.SelectObject(&oBufferBitmap);  

    // 배경을 칠한다.
    CBrush brushBackGround;
    //brushBackGround.CreateSolidBrush(RGB(255, 255, 255)); 
	brushBackGround.CreateSolidBrush(m_crBackground.ToCOLORREF());


    CBrush* pOldBrush1 = bufferDC.SelectObject(&brushBackGround);
    bufferDC.FillRect(rect, &brushBackGround);

    // 이전 지도이미지 복사
    CDC bufferPreImageDC;
    bufferPreImageDC.CreateCompatibleDC(&oDC);    
    CBitmap* pOldBitmap2 = bufferPreImageDC.SelectObject(&bitmap);

    bufferDC.StretchBlt((int)oDestRect.GetMinX(), (int)oDestRect.GetMaxY(), (int)oDestRect.GetMaxX(), (int)oDestRect.GetMaxY(), &bufferPreImageDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);    
    oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY);    

    bufferDC.SelectObject(pOldBrush1);
    bufferDC.SelectObject(pOldBitmap1);    
    brushBackGround.DeleteObject();

    bufferPreImageDC.SelectObject(pOldBitmap2);

    bufferDC.DeleteDC();
    bufferPreImageDC.DeleteDC();
}
