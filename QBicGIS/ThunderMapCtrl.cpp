#include "stdafx.h"
#include "ThunderMapCtrl.h"
#include "IncludeMessageID.h"
#include "TxMapMessage.h"

IMPLEMENT_DYNAMIC(ThunderMapCtrl, CWnd)

BEGIN_MESSAGE_MAP(ThunderMapCtrl, CWnd)
    ON_WM_CREATE()
    
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()

    ON_WM_SETCURSOR()
    ON_WM_TIMER()
    //ON_WM_MOUSEHOVER()

    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()

    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()

    ON_WM_LBUTTONDBLCLK()

    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()

    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()

    // 지도 백그라운드 레이어 드로잉 종료 메시지
    ON_MESSAGE(WM_MAP_BACKGROUND_DRAW_END, &ThunderMapCtrl::OnMapLayerDrawEnd)
END_MESSAGE_MAP()

ThunderMapCtrl::ThunderMapCtrl()
{
    RegisterWindowClass();

    /*m_spThunderMap  = ThunderMapPtr(new ThunderMap());
    m_spThunderMap->SetWindow(m_hWnd);*/
    m_bTimer = false;
    m_bSendOverMsg = false;
    TimerID = 1394;
}

ThunderMapCtrl::~ThunderMapCtrl()
{
}

BOOL ThunderMapCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, THUNDER_MAPCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = THUNDER_MAPCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}


int ThunderMapCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if( CWnd::OnCreate( lpCreateStruct ) == -1 ) {
        return -1;
    }
        
    return 1;
}

BOOL ThunderMapCtrl::Create( CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle /*= WS_VISIBLE*/ )
{
    CWnd::Create(THUNDER_MAPCTRL_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);

    ModifyStyle(1, WS_BORDER);
    m_spThunderMap  = ThunderMapPtr(new ThunderMap());
    m_spThunderMap->SetWindow(m_hWnd);
        
    return TRUE;
}

void ThunderMapCtrl::PreSubclassWindow()
{
    CWnd::PreSubclassWindow();

    ModifyStyle(1, WS_BORDER);
    
    m_spThunderMap  = ThunderMapPtr(new ThunderMap());
    m_spThunderMap->SetWindow(m_hWnd);
}

void ThunderMapCtrl::OnPaint()
{
    /*m_hWndOwner*/
    if (m_spThunderMap)
        m_spThunderMap->Draw();

    CWnd::OnPaint();
}

BOOL ThunderMapCtrl::OnEraseBkgnd( CDC* pDC )
{
    // Don't Redraw Background * return CWnd::OnEraseBkgnd(pDC)
    return FALSE;  
}

void ThunderMapCtrl::OnSize( UINT nType, int cx, int cy )
{
    CWnd::OnSize(nType, cx, cy);

    if (m_hWnd == nullptr) {
        return;
    }

    if (m_spThunderMap)
        m_spThunderMap->Resize(cx, cy);
}

void ThunderMapCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
    if (m_spThunderMap)
        m_spThunderMap->OnLButtonDown(nFlags, point);
    else 
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;

    SendNotifyMessage(WM_LBUTTONDOWN, &oEvent);

    CWnd::OnLButtonDown(nFlags, point);
}

void ThunderMapCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
    if (m_spThunderMap)
        m_spThunderMap->OnLButtonUp(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;

    SendNotifyMessage(WM_LBUTTONUP, &oEvent);
    CWnd::OnLButtonUp(nFlags, point);
}

void ThunderMapCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    if (false) {
        CString strTemp(_T(""));
        strTemp.Format(_T("OnLButtonDblClk Flag : %d"), nFlags);
        TxLogDebug((LPCTSTR)strTemp);
    }    

    if (m_spThunderMap)
        m_spThunderMap->OnLButtonUp(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;
    SendNotifyMessage(WM_LBUTTONDBLCLK, &oEvent);    

    CWnd::OnLButtonDblClk(nFlags, point);
}

void ThunderMapCtrl::OnRButtonDown( UINT nFlags, CPoint point )
{
    if (m_spThunderMap)
        m_spThunderMap->OnRButtonDown(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;
    SendNotifyMessage(WM_RBUTTONDOWN, &oEvent);    

    CWnd::OnRButtonDown(nFlags, point);
}

void ThunderMapCtrl::OnRButtonUp( UINT nFlags, CPoint point )
{
    if (m_spThunderMap)
        m_spThunderMap->OnRButtonUp(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;
    SendNotifyMessage(WM_RBUTTONUP, &oEvent);    

    CWnd::OnRButtonUp(nFlags, point);
}

void ThunderMapCtrl::OnMButtonDown( UINT nFlags, CPoint point )
{
    if (false) {
        CString strTemp(_T(""));
        strTemp.Format(_T("OnMButtonDown Flag : %d"), nFlags);
        TxLogDebug((LPCTSTR)strTemp);
    }    

    if (m_spThunderMap)
        m_spThunderMap->OnMButtonDown(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;

    SendNotifyMessage(WM_MBUTTONDOWN, &oEvent);
    CWnd::OnMButtonDown(nFlags, point);
}

void ThunderMapCtrl::OnMButtonUp( UINT nFlags, CPoint point )
{
    if (false) {
        CString strTemp(_T(""));
        strTemp.Format(_T("OnMButtonUp Flag : %d"), nFlags);
        TxLogDebug((LPCTSTR)strTemp);
    }    

    if (m_spThunderMap)
        m_spThunderMap->OnMButtonUp(nFlags, point);
    else
        return;

    TxMapEventMouse oEvent;
    oEvent.MousePoint = point;
    oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
    oEvent.Flags      = nFlags;

    SendNotifyMessage(WM_MBUTTONUP, &oEvent);
    CWnd::OnMButtonUp(nFlags, point);
}

void ThunderMapCtrl::OnMouseMove( UINT nFlags, CPoint point )
{
    if (m_spThunderMap) {
        m_spThunderMap->OnMouseMove(nFlags, point);
    }
    else {
        CWnd::OnMouseMove(nFlags, point);
        return;    
    }
    
    int nStatus = m_spThunderMap->GetMapDrawStatus();
    if (nStatus != 2) {
        CWnd::OnMouseMove(nFlags, point);
        return;
    }

    //★ 작은 변화는 전송하지 않는다. <2정도 이상 주어야 의미가 큰 의미가 있지만>
    //★ if (TxMath::Dist(m_preMousePoint.x, m_preMousePoint.y, point.x, point.y) > 1) 
    if (true) {
        m_preMousePoint = point; //★ 마우스 위치 갱신

        TxMapEventMouse oEvent;
        oEvent.MousePoint = point;
        oEvent.MapPoint   = m_spThunderMap->ScreenToMap(point);
        oEvent.Flags      = nFlags;

        //★ 
        SendNotifyMessage(WM_MOUSEMOVE, &oEvent);
    }
    
    if (m_spThunderMap->EnableMapTooltip() == true) {
        if (m_bTimer == false) {
            CRect rect;
            GetClientRect(&rect);
            if ( rect.PtInRect(point) == TRUE) {
                if (TxMath::Dist(m_preMouseOverPoint.x, m_preMouseOverPoint.y, point.x, point.y) >= 1) {
                    m_bTimer  = true;
                    m_preMouseOverPoint = point;
                    SetTimer(TimerID, 500, nullptr);
                }  
            }              
        } else {
            if (m_bSendOverMsg == true && TxMath::Dist(m_preMouseOverPoint.x, m_preMouseOverPoint.y, point.x, point.y) >= 1) {            
                m_spThunderMap->OnMouseLeave();
                m_bSendOverMsg = false;
            }    
        }
    }    
    
    CWnd::OnMouseMove(nFlags, point);
}

BOOL ThunderMapCtrl::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
    try
    {
        if (m_spThunderMap && nFlags == 0) {
            ScreenToClient(&point);
            m_spThunderMap->OnMouseWheel(nFlags, zDelta, point);
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, point);
}

void ThunderMapCtrl::OnTimer( UINT_PTR nIDEvent )
{
    if (nIDEvent == TimerID) {
        CPoint point(GetMessagePos());
        ScreenToClient(&point);

        if (TxMath::Dist(m_preMouseOverPoint.x, m_preMouseOverPoint.y, point.x, point.y) <= 0) {
            m_bTimer = false;
            KillTimer(TimerID);

            if (m_spThunderMap) {
                m_bSendOverMsg = true;
                m_spThunderMap->OnMouseHover(0, point);
            }
        }

        CRect rect;
        GetClientRect(&rect);
        if ( rect.PtInRect(point) == TRUE) {
            m_preMouseOverPoint = point;
        }

        if ( m_spThunderMap->EnableMapTooltip() == false && m_bTimer == true) {
            m_bTimer = false;
            KillTimer(TimerID);
        }
    }

    return CWnd::OnTimer(nIDEvent);
}

LRESULT ThunderMapCtrl::SendNotifyMessage( UINT nMessage, NMHDR* pNMHDR /*= NULL*/ ) const
{
    if (!IsWindow(m_hWnd)) {
        TxLogDebugException();
        return 0;
    }

    NMHDR nmhdr;
    if (pNMHDR == NULL)
        pNMHDR = &nmhdr;

    pNMHDR->hwndFrom = GetSafeHwnd();
    pNMHDR->idFrom   = GetDlgCtrlID();
    pNMHDR->code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, pNMHDR->idFrom, (LPARAM)pNMHDR);
    else
        return 0;
}


LRESULT ThunderMapCtrl::OnMapLayerDrawEnd( WPARAM wParam, LPARAM lParam )
{
    return SendNotifyMessage(WM_MAP_BACKGROUND_DRAW_END);
}

BOOL ThunderMapCtrl::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
    if (nHitTest == HTCLIENT) {
        if (m_spThunderMap) {
            SendNotifyMessage(WM_SETCURSOR);
            return TRUE;
        }        
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

