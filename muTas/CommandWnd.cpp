/**
 * @file StatusView.cpp
 * @brief KStatusView 구현
 * @author 
 * @date 2010.08.01
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "CommandWnd.h"
#include "MapFrameWnd.h"
#include "MapView.h"

IMPLEMENT_DYNAMIC(KCommandEdit, CEdit)

KCommandEdit::KCommandEdit()
{
    m_strCommand    = _T("");
    m_emCommandType =  KEMCommon;
}

KCommandEdit::~KCommandEdit()
{
}

// KCommandEdit 메시지 처리기입니다.
BEGIN_MESSAGE_MAP(KCommandEdit, CEdit)
    ON_WM_CHAR     ()
    ON_WM_KEYDOWN  ()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


void KCommandEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    int nCommandTextLength = m_strCommand.GetLength();
    KCommandWnd* pStatusViewWnd = (KCommandWnd*)GetParent();

    if (VK_RETURN == nChar)
    {        
        if (pStatusViewWnd)
        {
            CString       strCurrent, strUserCommand;
            GetWindowText(strCurrent);

            strUserCommand = strCurrent.Mid(nCommandTextLength, strCurrent.GetLength());
            if (strUserCommand.GetLength() > 0)
            {
                pStatusViewWnd->CommandMsg(strUserCommand.Trim());
            }            
        }
        return;
    }    

    if (VK_ESCAPE == nChar)
    {   
        if (pStatusViewWnd)
        {
            pStatusViewWnd->CommandMsg(_T("C"));
        }
        //SetSel(nCommandTextLength, nCommandTextLength);
        return;
    }

    int     nStart, nEnd;       
    GetSel (nStart, nEnd);

    if (nStart <= nCommandTextLength || nEnd <= nCommandTextLength)
    {
        SetSel(nCommandTextLength, nCommandTextLength);
    }

    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
    //SetFocus();
}


void KCommandEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CString       strCurrent;
    GetWindowText(strCurrent);
    int nCurrentTextLength = strCurrent.GetLength();
    int nCommandTextLength = m_strCommand.GetLength();

    if (VK_SPACE == nChar)
    {   
        KCommandWnd* pStatusViewWnd = (KCommandWnd*)GetParent();
        if (pStatusViewWnd)
        {
            CString  strUserCommand;
            
            strUserCommand = strCurrent.Mid(nCommandTextLength, strCurrent.GetLength());
            if (strUserCommand.GetLength() > 0)
            {
                pStatusViewWnd->CommandMsg(strUserCommand.Trim());
            }            
        }
        return;
    }
        
    int    nStart, nEnd;       
    GetSel(nStart, nEnd);

    if (nEnd <= nCommandTextLength)
    {
        SetSel(nCurrentTextLength, nCurrentTextLength);
    }

    if (VK_BACK == nChar)
    {   
        if (nCurrentTextLength <= nCommandTextLength)
        {
            return;
        }
    }

    CEdit::OnChar(nChar, nRepCnt, nFlags);
    //SetFocus();
}


void KCommandEdit::OnKillFocus(CWnd* pNewWnd)
{    
    CEdit::OnKillFocus(pNewWnd);
    //SetFocus();
}


void KCommandEdit::SetCommand( CString a_strCommand, KEMCommandType a_emCommandType )
{
    m_strCommand = a_strCommand + _T(">");
    SetWindowText(m_strCommand);
    
    SetSel(m_strCommand.GetLength(), m_strCommand.GetLength());
    if (a_emCommandType == KEMModal)
        SetFocus();

    m_emCommandType = a_emCommandType;
}


/******************************************************************************
* KStatusView                                                                 *
******************************************************************************/
IMPLEMENT_DYNAMIC(KCommandWnd, CWnd)


/**************************************
* Constructors and Destructor         *
**************************************/
KCommandWnd::KCommandWnd()
{   
}


KCommandWnd::~KCommandWnd()
{
}

BEGIN_MESSAGE_MAP(KCommandWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/**************************************
* Message handlers                    *
**************************************/
int KCommandWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

    if (m_wndCommandView.GetSafeHwnd() == 0)
    {
        m_wndCommandView.Create(WS_VISIBLE | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, ID_VIEW_STATUS_DISPLAY);
    }

    if (m_wndCommandEdit.GetSafeHwnd() == 0)
    {
        m_wndCommandEdit.Create(WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEW_STATUS_COMMAND);
    }

    CClientDC dc(this) ; 

    int nHeight = -dc.GetDeviceCaps( LOGPIXELSY )*9/72 ;
    m_fontCommandEdit.CreateFont(nHeight, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        _T("Tahoma"));

    //nHeight = -dc.GetDeviceCaps( LOGPIXELSY )*9/72 ;
    m_fontCommandView.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        _T("Tahoma"));

    m_wndCommandEdit.SetFont(&m_fontCommandEdit);
    m_wndCommandView.SetFont(&m_fontCommandView);

    m_wndCommandEdit.SetCommand(_T("Command"));

	return 0;
}


void KCommandWnd::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
    
    if( !(m_wndCommandView.GetSafeHwnd() && m_wndCommandEdit.GetSafeHwnd()))
    {
        return;
    }

    CRect rectClient;
    GetClientRect( &rectClient );
    int   nClientHeight = rectClient.Height();
    
    int nCommandEditHeight = 20;
    int nSeperate          = 2;

    if (nCommandEditHeight < nClientHeight)
    {
        int nCommandViewHeight = nClientHeight - nSeperate - nCommandEditHeight;
        m_wndCommandView.SetWindowPos( NULL, rectClient.left,    rectClient.top, 
                                             rectClient.Width(), nCommandViewHeight, SWP_NOACTIVATE | SWP_NOZORDER );
        m_wndCommandEdit.SetWindowPos( NULL, rectClient.left,    rectClient.top+nCommandViewHeight + nSeperate, 
                                             rectClient.Width(), nCommandEditHeight, SWP_NOACTIVATE | SWP_NOZORDER );
    }
    else
    {
        m_wndCommandView.SetWindowPos( NULL, 0,               0,              0,                  0,                   SWP_NOACTIVATE | SWP_NOZORDER );
        m_wndCommandEdit.SetWindowPos( NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
    }
}


void KCommandWnd::OnSetFocus( CWnd* pOldWnd )
{
    m_wndCommandEdit.SetFocus();
}


void KCommandWnd::OnContextMenu( CWnd* pWnd, CPoint pos )
{
    //wndCommandEdit.GetTe
}

BOOL KCommandWnd::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    //return m_wndCommandEdit.PreTranslateMessage(pMsg);
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {
            m_wndCommandEdit.SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);    
            return TRUE;
        }
        break;
    case WM_CHAR:
        if( pMsg->hwnd != m_wndCommandEdit.GetSafeHwnd())
        {
            m_wndCommandEdit.SendMessage(WM_CHAR,    pMsg->wParam, pMsg->lParam);
            return TRUE;
        }        
        break;
    }            

    return CWnd::PreTranslateMessage(pMsg);
}

void KCommandWnd::CommandMsg(CString a_strCommand)
{   
    KMapFrameWnd* pMapFrame = (KMapFrameWnd*)GetParentFrame();
    if( NULL == pMapFrame )
    {
        return;
    }
    
    if (m_wndCommandEdit.GetCommandType() != KEMCommon)
    {
        CommandResultReply(a_strCommand);
        return;
    }

    enum KEMCommandType
    {
        KEMCommandUnknown  = 0,
        KEMCommandMap      = 1,
        KEMCommandMsg      = 2
    };

    KEMCommandType CommandType = KEMCommandUnknown;

    KMapView* pMapView = pMapFrame->GetMapView();
    if (a_strCommand.CompareNoCase(_T("Z")) == 0)
    {
        pMapView->SendMessage(WM_COMMAND, ID_MAPVIEW_ZOOMIN);
        CommandType = KEMCommandMap;
    }
    
    if (a_strCommand.CompareNoCase(_T("O")) == 0)
    {
        pMapView->SendMessage(WM_COMMAND, ID_MAPVIEW_ZOOMOUT);
        CommandType = KEMCommandMap;
    }

    if (a_strCommand.CompareNoCase(_T("P")) == 0)
    {
        pMapView->SendMessage(WM_COMMAND, ID_MAPVIEW_PAN);
        CommandType = KEMCommandMap;
    }
    
    if (a_strCommand.CompareNoCase(_T("H")) == 0)
    {
        CommandType = KEMCommandMsg;
        CommandLogMsg(_T("Help ?"));
    }

    if (a_strCommand.CompareNoCase(_T("C")) == 0)
    {
        CommandType = KEMCommandMsg;
        CommandLogMsg(_T("Canceled..."));
    }

    CString strOutMsg;

    switch (CommandType)
    {
    case KEMCommandUnknown:
        strOutMsg.Format(_T("Unknown Command : %s"), a_strCommand);
        CommandLogMsg(strOutMsg);
    	break;
    case KEMCommandMsg:
        break;
    default:
        m_wndCommandEdit.GetWindowText(strOutMsg);
        CommandLogMsg(strOutMsg);
        break;
    }      

    m_wndCommandEdit.SetCommand(_T("Command"), KEMCommon);
}

void KCommandWnd::CommandLogMsg( CString a_strMsg )
{
    /*
    CString rText;
    wndCommandView.GetWindowText(rText);
    wndCommandView.SetWindowText(rText + _T("\r\n") + a_str);
    wndCommandView.LineScroll(wndCommandView.GetLineCount()-1);
    */
    
    CString strCommandView = _T("\r\n") + a_strMsg;
    int     nTextLength    = m_wndCommandView.GetWindowTextLength();
    
    m_wndCommandView.SetSel    (nTextLength, nTextLength);
    m_wndCommandView.ReplaceSel(strCommandView);
}

void KCommandWnd::Command( ImChampModalCommandPtr a_spModalCommand, KEMCommandType a_emCommandType)
{
    //m_spModalCommand.reset();
    m_spModalCommand = a_spModalCommand;

    // 링크 선택      1, 2, 3, 4          > ??
    // <strCommand> <setSelectableInfo> > ??
    CString strCommand = m_spModalCommand->GetCommand();
    std::vector<CString> vecSelectableInfo = m_spModalCommand->GetSelectableInfo();
    
    size_t nxCount = vecSelectableInfo.size();
    if (nxCount != 0)
    {
        strCommand += _T(" ");
    }

    for (size_t i=0; i<nxCount; i++)
    {
        if (i != nxCount-1)
        {
            strCommand = strCommand + vecSelectableInfo[i] + _T(", ");
        }
        else
        {
            strCommand = strCommand + vecSelectableInfo[i];
        }        
    }

    m_wndCommandEdit.SetCommand(strCommand, a_emCommandType);
}

void KCommandWnd::CommandResultReply(CString a_strCommand)
{
    CString strOutMsg;
    m_wndCommandEdit.GetWindowText(strOutMsg);

    if (m_spModalCommand == NULL)
    {
        CommandLogMsg(strOutMsg + _T("Canceled..."));
        SetDefaultCommand(); 
        return;
    }

    if ( a_strCommand.CompareNoCase(_T("C")) == 0)
    {
        try
        {
            CommandLogMsg(strOutMsg + _T("Canceled..."));
            SetDefaultCommand();            
            m_spModalCommand->GetReceiver()->Canceled();
        }
        catch(...)
        {
            TxLogDebugException();
        }        
        return;
    }

    bool bEnableCommand = m_spModalCommand->EnableCommand(a_strCommand);
        
    if (bEnableCommand)
    {
        try
        {
            // 응답하는 곳에서 다시 명령 요청을 할 수 있고 없을 수도 있음.  
            CommandLogMsg    (strOutMsg);
            SetDefaultCommand();
            m_spModalCommand->GetReceiver()->CommandFeedback(a_strCommand);
        }
        catch (...)
        {
            TxLogDebugException();
        }
    }
    else
    {        
        strOutMsg += _T("  ~~~ Selection Error <");
        std::vector<CString> vecSelectableInfo = m_spModalCommand->GetSelectableInfo();
        size_t nxCount = vecSelectableInfo.size();
        for (size_t i=0; i<nxCount; i++)
        {
            if (i != nxCount - 1)
            {
                strOutMsg = strOutMsg + vecSelectableInfo[i] + _T(", ");
            }
            else
            {
                strOutMsg = strOutMsg + vecSelectableInfo[i];
            }            
        }

        strOutMsg += _T("> 값만 선택 가능합니다.");

        CommandLogMsg(strOutMsg       );
        Command      (m_spModalCommand);
    }    
}

void KCommandWnd::SetDefaultCommand()
{
    m_wndCommandEdit.SetCommand(_T("Command"), KEMCommon);
}