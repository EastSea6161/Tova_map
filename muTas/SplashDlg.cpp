#include "stdafx.h"
#include "SplashDlg.h"

IMPLEMENT_DYNAMIC(CSplashDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
    ON_WM_TIMER()
END_MESSAGE_MAP()

CSplashDlg::CSplashDlg( CWnd* pParent /*= NULL*/ ) : CDialogEx(CSplashDlg::IDD, pParent)
{
    c_pSplashDlg = NULL;
}

CSplashDlg::~CSplashDlg()
{

}

void CSplashDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CSplashDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();   
    CenterWindow();

    SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

    CStatic * pSplashImage = (CStatic*)GetDlgItem(IDC_STATIC);

    CRect rc;
    GetClientRect(&rc);

    pSplashImage->MoveWindow(rc);
    return TRUE;
}

BOOL CSplashDlg::PreTranslateMessage( MSG* pMsg )
{
    if (c_pSplashDlg == NULL)
        return FALSE;

    // If you receive a keyboard or mouse message, hide the splash screen.
    if (c_pSplashDlg->m_hWnd != NULL && pMsg->message == WM_KEYDOWN ||
        pMsg->message == WM_SYSKEYDOWN ||
        pMsg->message == WM_LBUTTONDOWN ||
        pMsg->message == WM_RBUTTONDOWN ||
        pMsg->message == WM_MBUTTONDOWN ||
        pMsg->message == WM_NCLBUTTONDOWN ||
        pMsg->message == WM_NCRBUTTONDOWN ||
        pMsg->message == WM_NCMBUTTONDOWN)
    {
        c_pSplashDlg->HideSplashScreen();
        return TRUE;	// message handled here
    }

    return FALSE;	// message not handled
}

void CSplashDlg::ShowSplashScreen( CWnd* pParentWnd /*= NULL*/ )
{
    // Allocate a new splash screen, and create the window.
    c_pSplashDlg = new CSplashDlg;
    if (!c_pSplashDlg->Create(CSplashDlg::IDD, pParentWnd))
        delete c_pSplashDlg;
    else
        c_pSplashDlg->ShowWindow(SW_SHOW);

    c_pSplashDlg->UpdateWindow();
    //c_pSplashDlg->SetTimer(1, 800, NULL);
}

void CSplashDlg::OnTimer( UINT_PTR nIDEvent )
{
	if (nIDEvent == 1)
	{
		c_pSplashDlg->KillTimer(1);
		HideSplashScreen();
	}
}

void CSplashDlg::HideSplashScreen()
{
    //c_pSplashDlg->KillTimer(1);
    DestroyWindow();
    //AfxGetMainWnd()->UpdateWindow();
    delete c_pSplashDlg;
    c_pSplashDlg = NULL;
}
