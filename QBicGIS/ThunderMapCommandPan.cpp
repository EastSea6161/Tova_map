#include "stdafx.h"
#include "ThunderMapCommandPan.h"

ThunderMapCommandPan::ThunderMapCommandPan(IThunderMap* pThunderMap, Gdiplus::Color crBackground) 
	: IThunderMapCommand(pThunderMap, PanCommand, crBackground)
{
    m_bPanning     = false;
    m_ptFirstPoint = CPoint(0, 0);
    m_ptMovePoint  = CPoint(0, 0);
    m_pBitmap      = nullptr;
}

ThunderMapCommandPan::~ThunderMapCommandPan()
{
    if (m_pBitmap) {
        m_pBitmap->DeleteObject();
        DeleteNull(m_pBitmap);
    }
}

void ThunderMapCommandPan::OnLButtonDown( UINT nFlags, CPoint point )
{
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

    m_bPanning     = true;
    m_ptFirstPoint = point;
    m_ptMovePoint  = point;

    /*CImage oImage;
    oImage.Attach((HBITMAP)m_pBitmap->Detach());
    oImage.Save(_T("OnLButtonDown.bmp"));
    m_pBitmap->Attach(oImage.Detach());
    */
}

void ThunderMapCommandPan::OnMouseMove( UINT nFlags, CPoint point )
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
    // 추가 정보     
    Gdiplus::Graphics graphics(bufferDC);
#ifdef _DEBUG
    CString strMsg(_T(""));
    strMsg.AppendFormat(_T("*Pan : \n(%ld, %ld)->(%ld,%ld) : (%ld, %ld)"), m_ptFirstPoint.x, m_ptFirstPoint.y, point.x, point.y, cx, cy);
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
}

void ThunderMapCommandPan::OnLButtonUp( UINT nFlags, CPoint point )
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

void ThunderMapCommandPan::OnLButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandPan::OnRButtonDown( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandPan::OnRButtonUp( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandPan::OnRButtonDblClk( UINT nFlags, CPoint point )
{
}

void ThunderMapCommandPan::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
}

