#include "stdafx.h"
#include "QBicDialogEx.h"
#include <Imm.h>

BEGIN_MESSAGE_MAP(KResizeDialogEx, CDialog)
	//{{AFX_MSG_MAP(KResizeDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

KResizeDialogEx::KResizeDialogEx()
	: CXTPResize(this, 0)
{
	m_nDialogID   = 0;
	m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn  = false;
}

KResizeDialogEx::KResizeDialogEx(const UINT nID, CWnd* pParent, const UINT nFlags) :
CDialog(nID, pParent), CXTPResize(this, nFlags)
{
	m_nDialogID   = nID;
	m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn  = false;
}

KResizeDialogEx::KResizeDialogEx(LPCTSTR lpszTemplateName, CWnd* pParent, const UINT nFlags) :
CDialog(lpszTemplateName, pParent), CXTPResize(this, nFlags)
{
	m_nDialogID    = 0;
	m_bUserClosed  = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

BOOL KResizeDialogEx::OnInitDialog()
{
	// Only show the resize grip if the frame is a resizing frame
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0)
	{
		SetFlag(xtpResizeNoSizeIcon);
	}

    m_tooltipCtrl.Create(this);
    m_tooltipCtrl.SetMaxTipWidth(150);
    m_tooltipCtrl.SetDelayTime(TTDT_AUTOPOP, 5000);

	CDialog::OnInitDialog();
	CXTPResize::Init();

    //CenterWindow(GetParent());    
	return TRUE;
}


void KResizeDialogEx::AddImChampTooltip( UINT nID, CString strTooltipMsg )
{
    try
    {
    	m_tooltipCtrl.AddTool(GetDlgItem(nID), strTooltipMsg);
    }
    catch (...)
    {
    }
}

void KResizeDialogEx::UpdateImChampTooltip(UINT nID, CString strTooltipMsg)
{
	try
	{
		m_tooltipCtrl.UpdateTipText(strTooltipMsg, GetDlgItem(nID));
	}
	catch (...)
	{
	}
}

void KResizeDialogEx::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void KResizeDialogEx::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CXTPResize::Size();
}

void KResizeDialogEx::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CDialog::OnDestroy();
}

BOOL KResizeDialogEx::PreTranslateMessage( MSG* pMsg )
{
    m_tooltipCtrl.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
        if (pMsg->message == WM_KEYDOWN)
        {
            if(pMsg->wParam == VK_ESCAPE)
            {
                if (m_bUseEscape == false)
                    return TRUE;
            }

            if(pMsg->wParam == VK_RETURN)
            {
                if (m_bUseReturn == false)
                    return TRUE;
            }
        }
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void KResizeDialogEx::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if (bShow)
	{
		m_bUserClosed = false;
	}
	
	CDialog::OnShowWindow(bShow, nStatus);
}

// 한글모드로 전환하는 함수
void KResizeDialogEx::SetNativeLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_NATIVE;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }    
}

// 영문모드로 전환하는 함수
void KResizeDialogEx::SetEnglishLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_ALPHANUMERIC;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }
}

void KResizeDialogEx::SetModelessReceiver( ImChampModelessFeedback* a_pReceiver )
{
    if (a_pReceiver != nullptr)
        m_pModelessReceiver = a_pReceiver;
}

bool KResizeDialogEx::IsUserClosed()
{
    return m_bUserClosed;
}

void KResizeDialogEx::OnCancel()
{
    m_bUserClosed = true;
	try
	{
		if (m_pModelessReceiver != nullptr)
		{
			m_pModelessReceiver->ModelessCancleFeedback(m_nDialogID);
		}
	}
	catch(...)
	{
	}
    CDialog::OnCancel();
}

void KResizeDialogEx::OnOK()
{
    m_bUserClosed = true;
    try
    {
        if (m_pModelessReceiver != nullptr)
        {
            m_pModelessReceiver->ModelessOKFeedback(m_nDialogID);
        }
    }
    catch (...)
    {
    }
    CDialog::OnOK();
}


//**************************************************************************************************


BEGIN_MESSAGE_MAP(KDialogEx, CDialogEx)
	//{{AFX_MSG_MAP(KDialogEx)
    ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

KDialogEx::KDialogEx(): CXTPResize(this, 0)
{
	m_nDialogID   = 0;
	m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

KDialogEx::KDialogEx(const UINT nID, CWnd* pParent, const UINT nFlags) : CDialogEx(nID, pParent), CXTPResize(this, nFlags)
{
	m_nDialogID = nID;
	m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

KDialogEx::KDialogEx(LPCTSTR lpszTemplateName, CWnd* pParent, const UINT nFlags) : CDialogEx(lpszTemplateName, pParent), CXTPResize(this, nFlags)
{
	m_nDialogID = 0;
	m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

BOOL KDialogEx::OnInitDialog()
{	
    DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
    if ((dwStyle & WS_THICKFRAME) == 0)
    {
        SetFlag(xtpResizeNoSizeIcon);
    }

    m_tooltipCtrl.Create(this);
    m_tooltipCtrl.SetMaxTipWidth(150);
    m_tooltipCtrl.SetDelayTime(TTDT_AUTOPOP, 5000);

	CDialogEx::OnInitDialog();
    CXTPResize::Init();
    //CenterWindow(GetParent());
    
	return TRUE;
}

void KDialogEx::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    CXTPResize::Size();
}

void KDialogEx::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void KDialogEx::AddImChampTooltip( UINT nID, CString strTooltipMsg )
{
    try
    {
        m_tooltipCtrl.AddTool(GetDlgItem(nID), strTooltipMsg);
    }
    catch (...)
    {
    }
}

void KDialogEx::UpdateImChampTooltip(UINT nID, CString strTooltipMsg)
{
	try
	{
		m_tooltipCtrl.UpdateTipText(strTooltipMsg, GetDlgItem(nID));
	}
	catch (...)
	{
	}
}

void KDialogEx::OnDestroy()
{
	CDialogEx::OnDestroy();
}

BOOL KDialogEx::PreTranslateMessage( MSG* pMsg )
{
    m_tooltipCtrl.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
        if(pMsg->wParam == VK_ESCAPE)
        {
            if (m_bUseEscape == false)
                return TRUE;
        }

        if(pMsg->wParam == VK_RETURN)
        {
            if (m_bUseReturn == false)
                return TRUE;
        }
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void KDialogEx::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if (bShow)
	{
		m_bUserClosed = false;
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}

// 한글모드로 전환하는 함수
void KDialogEx::SetNativeLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_NATIVE;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }    
}

// 영문모드로 전환하는 함수
void KDialogEx::SetEnglishLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_ALPHANUMERIC;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }
}

void KDialogEx::SetModelessReceiver(ImChampModelessFeedback* a_pReceiver)
{
    if (a_pReceiver != nullptr)
        m_pModelessReceiver = a_pReceiver;
}

bool KDialogEx::IsUserClosed()
{
    return m_bUserClosed;
}

void KDialogEx::OnCancel()
{
    m_bUserClosed = true;
    CDialogEx::OnCancel();
}

void KDialogEx::OnOK()
{
    m_bUserClosed = true;
    try
    {
        if (m_pModelessReceiver != nullptr)
        {
            m_pModelessReceiver->ModelessOKFeedback(m_nDialogID);
        }
    }
    catch (...)
    {
    }
    CDialogEx::OnOK();
}


//----------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(KWhiteDlgEx, CDialogEx)
    //{{AFX_MSG_MAP(KWhiteDlgEx)
    ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
    ON_WM_DESTROY()
    ON_WM_SHOWWINDOW()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

KWhiteDlgEx::KWhiteDlgEx(): CXTPResize(this, 0)
{
    m_nDialogID   = 0;
    m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

KWhiteDlgEx::KWhiteDlgEx(const UINT nID, CWnd* pParent, const UINT nFlags) : CDialogEx(nID, pParent), CXTPResize(this, nFlags)
{
    m_nDialogID = nID;
    m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

KWhiteDlgEx::KWhiteDlgEx(LPCTSTR lpszTemplateName, CWnd* pParent, const UINT nFlags) : CDialogEx(lpszTemplateName, pParent), CXTPResize(this, nFlags)
{
    m_nDialogID = 0;
    m_bUserClosed = false;
    m_pModelessReceiver = nullptr;

    m_bUseEscape  = false;
    m_bUseReturn   = false;
}

BOOL KWhiteDlgEx::OnInitDialog()
{	
    DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
    if ((dwStyle & WS_THICKFRAME) == 0)
    {
        SetFlag(xtpResizeNoSizeIcon);
    }

    m_tooltipCtrl.Create(this);
    m_tooltipCtrl.SetMaxTipWidth(150);
    m_tooltipCtrl.SetDelayTime(TTDT_AUTOPOP, 5000);

    CDialogEx::OnInitDialog();
    CXTPResize::Init();
    //CenterWindow(GetParent());

    //SetBackgroundColor(RGB(255, 255, 255));
    return TRUE;
}

void KWhiteDlgEx::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    CXTPResize::Size();
}

void KWhiteDlgEx::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void KWhiteDlgEx::AddImChampTooltip( UINT nID, CString strTooltipMsg )
{
    try
    {
        m_tooltipCtrl.AddTool(GetDlgItem(nID), strTooltipMsg);
    }
    catch (...)
    {
    }
}

void KWhiteDlgEx::UpdateImChampTooltip(UINT nID, CString strTooltipMsg)
{
	try
	{
		m_tooltipCtrl.UpdateTipText(strTooltipMsg, GetDlgItem(nID));
	}
	catch (...)
	{
	}
}

void KWhiteDlgEx::OnDestroy()
{
    CDialogEx::OnDestroy();
}

HBRUSH KWhiteDlgEx::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
    HBRUSH hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
    return hbr;
}

BOOL KWhiteDlgEx::PreTranslateMessage( MSG* pMsg )
{
    m_tooltipCtrl.RelayEvent(pMsg);

    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_ESCAPE)
        {
            if (m_bUseEscape == false)
                return TRUE;
        }

        if(pMsg->wParam == VK_RETURN)
        {
            if (m_bUseReturn == false)
                return TRUE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void KWhiteDlgEx::OnShowWindow( BOOL bShow, UINT nStatus )
{
    if (bShow)
    {
        m_bUserClosed = false;
    }

    CDialogEx::OnShowWindow(bShow, nStatus);
}

// 한글모드로 전환하는 함수
void KWhiteDlgEx::SetNativeLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_NATIVE;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }    
}

// 영문모드로 전환하는 함수
void KWhiteDlgEx::SetEnglishLanguageMode(UINT nID)
{
    try
    {
        HWND hWnd = GetDlgItem(nID)->GetSafeHwnd();
        if (hWnd == nullptr)
            return;

        HIMC hIMC = ImmGetContext(hWnd);
        DWORD dwConv, dwSent;
        DWORD dwTemp;

        ImmGetConversionStatus(hIMC,&dwConv,&dwSent);
        dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
        dwTemp |= IME_CMODE_ALPHANUMERIC;
        dwConv = dwTemp;
        ImmSetConversionStatus(hIMC,dwConv,dwSent);
        ImmReleaseContext(hWnd,hIMC);
    }
    catch (...)
    {
    }
}

void KWhiteDlgEx::SetModelessReceiver(ImChampModelessFeedback* a_pReceiver)
{
    if (a_pReceiver != nullptr)
        m_pModelessReceiver = a_pReceiver;
}

bool KWhiteDlgEx::IsUserClosed()
{
    return m_bUserClosed;
}

void KWhiteDlgEx::OnCancel()
{
    m_bUserClosed = true;
    CDialogEx::OnCancel();
}

void KWhiteDlgEx::OnOK()
{
    m_bUserClosed = true;
    try
    {
        if (m_pModelessReceiver != nullptr)
        {
            m_pModelessReceiver->ModelessOKFeedback(m_nDialogID);
        }
    }
    catch (...)
    {
    }
    CDialogEx::OnOK();
}
