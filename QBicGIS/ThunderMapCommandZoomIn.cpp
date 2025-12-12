#include "stdafx.h"
#include "ThunderMapCommandZoomIn.h"
#include "TxConvertUtil.h"

ThunderMapCommandZoomIn::ThunderMapCommandZoomIn(IThunderMap* pThunderMap, Gdiplus::Color crBackground)
	: IThunderMapCommand(pThunderMap, ZoomInCommand, crBackground)
{
    m_bMoving      = false;
    m_ptFirstPoint = CPoint(0, 0);
    m_ptMovePoint  = CPoint(0, 0);
    m_pBitmap      = nullptr;
}

ThunderMapCommandZoomIn::~ThunderMapCommandZoomIn()
{
}

void ThunderMapCommandZoomIn::OnLButtonDown( UINT nFlags, CPoint point )
{
    m_bMoving      = true;
    m_ptFirstPoint = point;
    m_ptMovePoint  = point;

    HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

    RECT  rc; ::GetClientRect(hWnd, &rc);
    CRect rect(rc);

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    // 현재의 지도이미지를 복사한다.
    CClientDC oDC(pHandleWnd);
    CDC       mDC; 
    mDC.CreateCompatibleDC(&oDC);

    // 마우스 Up에서 Delete 한다.
    m_pBitmap = new CBitmap(); 
    m_pBitmap->CreateCompatibleBitmap(&oDC, rect.Width(), rect.Height());

    CBitmap* pOldBitmap = mDC.SelectObject(m_pBitmap);

    // 현재 화면을 복사
    mDC.BitBlt(0, 0, rect.Width(), rect.Height(), &oDC, 0, 0, SRCCOPY);

    // 명령어 출력 - 디버깅
    //mDC.SetTextColor(RGB(255, 255, 255));
    //mDC.SetBkColor(TRANSPARENT);
    //mDC.DrawText(_T("Pan"), CRect(0, 0, 50, 50), DT_SINGLELINE | DT_LEFT | DT_TOP);    
    // 여기까지 비트맵에 저장
    // 메모리에 있는 내용을 다시 출력해 준다.
    // oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &mDC, 0, 0, SRCCOPY);

    mDC.SelectObject(pOldBitmap);
    mDC.DeleteDC();
}

void ThunderMapCommandZoomIn::OnMouseMove( UINT nFlags, CPoint point )
{
    HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

    if (m_bMoving == false)
        return;

    m_ptMovePoint  = point;
        
    RECT  rc; ::GetClientRect(hWnd, &rc);
    CRect rect(rc);

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    CClientDC oDC(pHandleWnd);
    CDC       bufferDC; 
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

    //디버깅 : oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY); 
    CDC bufferPreImageDC;
    bufferPreImageDC.CreateCompatibleDC(&oDC);    

    CBitmap* pOldBitmap2 = bufferPreImageDC.SelectObject(m_pBitmap);            
        
    bufferDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferPreImageDC, 0, 0, SRCCOPY);   
    
    // 추가 정보 
    Gdiplus::Graphics graphics(bufferDC);
    Pen oPen(Color(255, 0, 0, 0));
    SolidBrush oFillBrush(Color(20, 0, 0, 255));

    Gdiplus::RectF rectZoomIn = TxConvertUtil::ToRectF(m_ptFirstPoint, m_ptMovePoint);
    graphics.FillRectangle(&oFillBrush, rectZoomIn);   
    graphics.DrawRectangle(&oPen, rectZoomIn);
#ifdef _DEBUG
    CString strMsg(_T(""));
    strMsg.AppendFormat(_T("*ZoomIn : \n(%ld, %ld)->(%ld,%ld)"), m_ptFirstPoint.x, m_ptFirstPoint.y, point.x, point.y);
    PointF pointF((Gdiplus::REAL)point.x, (Gdiplus::REAL)point.y);
    DebugLabel(graphics, pointF, strMsg);
#endif
    oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY);    

    bufferDC.SelectObject(pOldBrush1);
    bufferDC.SelectObject(pOldBitmap1);    
    brushBackGround.DeleteObject();

    bufferPreImageDC.SelectObject(pOldBitmap2);

    bufferDC.DeleteDC();
    bufferPreImageDC.DeleteDC();
    /*
    HWND hwnd = m_pThunderMap->GetHwnd();    
    CClientDC oDC(CWnd::FromHandle(hwnd));
    oDC.SetROP2(R2_XORPEN);    
    oDC.SelectStockObject(NULL_BRUSH);
    
    CPen   pen, *pOldPen;
    pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    pOldPen = oDC.SelectObject(&pen);
    
    CRect oPreRect = TxConvertUtil::ToCRect(m_ptFirstPoint, m_ptPrePoint);
    CRect oCurRect = TxConvertUtil::ToCRect(m_ptFirstPoint, m_ptMovePoint);

    oDC.Rectangle(oPreRect);
    oDC.Rectangle(oCurRect);
    oDC.SelectObject(pOldPen);
    m_ptPrePoint = point;
    */
}

void ThunderMapCommandZoomIn::OnLButtonUp( UINT nFlags, CPoint point )
{
    m_ptMovePoint = point;
    m_bMoving     = false;

    if (m_pBitmap)
    {
        m_pBitmap->DeleteObject();
        DeleteNull(m_pBitmap);
    }

    if (point == m_ptFirstPoint)
    {
        m_pThunderMap->Refresh();
        return;
    }
    
    ThunderMapDispPtr spThunderMapDisp = m_pThunderMap->GetThunderMapDisp();
    TxEnvelope        oMapEnvelope     = spThunderMapDisp->MapEnvelope();
    
    double dCx, dCy;
    oMapEnvelope.GetCenter(dCx, dCy);

    TxPoint oFirstPt = spThunderMapDisp->ScreenToMap(m_ptFirstPoint);
    TxPoint oEndPt   = spThunderMapDisp->ScreenToMap(m_ptMovePoint );

    double dX = oFirstPt.X - oEndPt.X;
    double dY = oFirstPt.Y - oEndPt.Y;    

    oMapEnvelope.SetEnvelope(oFirstPt.X, oFirstPt.Y, oEndPt.X, oEndPt.Y);
    spThunderMapDisp->SetMapRect(oMapEnvelope);
    m_pThunderMap->Refresh();
}

void ThunderMapCommandZoomIn::OnLButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomIn::OnRButtonDown( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomIn::OnRButtonUp( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomIn::OnRButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandZoomIn::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
}