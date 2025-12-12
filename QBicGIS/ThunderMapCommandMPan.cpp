#include "stdafx.h"
#include "ThunderMapCommandMPan.h"

ThunderMapCommandMPan::ThunderMapCommandMPan( IThunderMap* pThunderMap, Gdiplus::Color crBackground)
	: IThunderMapCommand(pThunderMap, MPanCommand, crBackground)
{
    m_bPanning     = false;
    m_ptFirstPoint = CPoint(0, 0);
    m_ptMovePoint  = CPoint(0, 0);
    m_pBitmap      = nullptr;
}

ThunderMapCommandMPan::~ThunderMapCommandMPan()
{
    if (m_pBitmap) {
        m_pBitmap->DeleteObject();
        DeleteNull(m_pBitmap);
    }
}

void ThunderMapCommandMPan::OnMButtonDown( UINT nFlags, CPoint point )
{
    if (m_pBitmap) {
        m_pBitmap->DeleteObject();
        DeleteNull(m_pBitmap);
    }

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

    mDC.SelectObject(pOldBitmap);
    mDC.DeleteDC();

    m_bPanning     = true;
    m_ptFirstPoint = point;
    m_ptMovePoint  = point;
}

void ThunderMapCommandMPan::OnMouseMove( UINT nFlags, CPoint point )
{
    if (m_pBitmap == nullptr) return;
    if (m_bPanning == false || m_ptFirstPoint == point)
        return;

    m_ptMovePoint  = point;
    
    HWND hWnd = m_pThunderMap->GetHwnd();
    if (hWnd == nullptr)
        return;

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

    int cx, cy;
    cx = m_ptFirstPoint.x - point.x;
    cy = m_ptFirstPoint.y - point.y;

    bufferDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferPreImageDC, cx, cy, SRCCOPY);         
    oDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0,  0, SRCCOPY);    

    bufferDC.SelectObject(pOldBrush1);
    bufferDC.SelectObject(pOldBitmap1);    
    brushBackGround.DeleteObject();

    bufferPreImageDC.SelectObject(pOldBitmap2);

    bufferDC.DeleteDC();
    bufferPreImageDC.DeleteDC();
}

void ThunderMapCommandMPan::OnMButtonUp( UINT nFlags, CPoint point )
{
    if (m_pBitmap) {
        m_pBitmap->DeleteObject();
        DeleteNull(m_pBitmap);
    } else {
        return;
    }
    
    m_ptMovePoint = point;
    m_bPanning    = false;

    if (point == m_ptFirstPoint)
        return;

    ThunderMapDispPtr spThunderMapDisp = m_pThunderMap->GetThunderMapDisp();
    TxEnvelope oMapEnvelope     = spThunderMapDisp->MapEnvelope();
    double dCx, dCy;
    oMapEnvelope.GetCenter(dCx, dCy);

    TxPoint oFirstPt = spThunderMapDisp->ScreenToMap(m_ptFirstPoint);
    TxPoint oEndPt   = spThunderMapDisp->ScreenToMap(m_ptMovePoint );

    double dX = oFirstPt.X - oEndPt.X;
    double dY = oFirstPt.Y - oEndPt.Y;    

    oMapEnvelope.Move(dCx + dX, dCy + dY);
    spThunderMapDisp->SetMapRect(oMapEnvelope);

    m_pThunderMap->Refresh();
}
