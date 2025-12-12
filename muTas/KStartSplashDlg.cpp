// DialogSplash.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KStartSplashDlg.h"
#include "ProgramVersionInfo.h"
// transparent를 적용하는 방법
#if WINVER >= 0x0500
   #include <uxtheme.h>
   #pragma comment (lib, "UxTheme.lib")
#endif


// KDialogExSplash 대화 상자입니다.

IMPLEMENT_DYNAMIC(KStartSplashDlg, KDialogEx)

KStartSplashDlg::KStartSplashDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KStartSplashDlg::IDD, pParent)
    , m_nSystemTypeInfo(0)
{
    //m_pbkBrush.CreateSolidBrush(RGB(255,255,255));
    m_pbkBrush.CreateStockObject(NULL_BRUSH);
}

KStartSplashDlg::~KStartSplashDlg()
{
    m_pbkBrush.DeleteObject();
}

void KStartSplashDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SPLASH, m_systemPicture);
    //DDX_Radio(pDX, IDC_RADIO_PASSENGER, m_nSystemType);
    //DDX_Control(pDX, IDC_RADIO_PASSENGER, m_btnOptionPassenger);
}

BEGIN_MESSAGE_MAP(KStartSplashDlg, KDialogEx)
    ON_BN_CLICKED(IDOK, &KStartSplashDlg::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    ON_STN_CLICKED(IDC_SPLASH, &KStartSplashDlg::OnStnClickedSplash)
END_MESSAGE_MAP()


BOOL KStartSplashDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();    
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    // TODO:  여기에 추가 초기화 작업을 추가합니다.  
#ifndef _UNICODE
    SetWindowTheme( GetDlgItem(IDC_RADIO_PASSENGER)->m_hWnd, L"", L"");
    SetWindowTheme( GetDlgItem(IDC_RADIO_FREIGHT)->m_hWnd, L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_STATIC_VER)->m_hWnd, L"", L"");
#else
    SetWindowTheme( GetDlgItem(IDC_RADIO_PASSENGER)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO_FREIGHT)->m_hWnd, _T(""), _T("") );
	SetWindowTheme( GetDlgItem(IDC_STATIC_VER)->m_hWnd, _T(""), _T(""));
#endif
    CRect rectPic, rectWin;
    GetWindowRect(&rectWin);
    m_systemPicture.GetWindowRect(&rectPic);
    MoveWindow(rectWin.left, rectWin.top,rectPic.Width(),rectPic.Height());

    SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)5);
    RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
	
	CButton* pButtonP = (CButton*)GetDlgItem(IDC_RADIO_PASSENGER);
	CButton* pButtonF = (CButton*)GetDlgItem(IDC_RADIO_FREIGHT);
	//pButtonF->EnableWindow(FALSE);
	if (m_nSystemTypeInfo == 0)
	{
		pButtonP->SetCheck(BST_CHECKED);
		pButtonF->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pButtonP->SetCheck(BST_UNCHECKED);
		pButtonF->SetCheck(BST_CHECKED);
	}

    CString  strVersionInfo(_T(""));
    CStringA strCompileInfo("");

    strCompileInfo.Format("%s, %s", __DATE__, __TIME__);
    strVersionInfo.AppendFormat(_T("Project : %d.0 | Local : %d.0"), GxnProjectVersion, GxnIoLocalVersion);
    strVersionInfo.AppendFormat(_T(" | Build : %s"), (CString)strCompileInfo);

	//GetDlgItem(IDC_STATIC_VER)->SetWindowText(_T("Version 13.0.0 (2014.01.24)"));
    GetDlgItem(IDC_STATIC_VER)->SetWindowText(strVersionInfo);
	
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
} 


void KStartSplashDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);
	
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_PASSENGER, IDC_RADIO_FREIGHT);
	if (nCheckedRadioBtn == IDC_RADIO_PASSENGER)
	{
		m_nSystemTypeInfo = 0;
	}
	else
	{
		m_nSystemTypeInfo = 1;
	}

    KDialogEx::OnOK();
}


HBRUSH KStartSplashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   /* HBRUSH hbr;    
    hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
    return hbr;*/

    HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_RADIO_FREIGHT || ID == IDC_RADIO_PASSENGER)
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }

	if (ID == IDC_STATIC_VER)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

    return hbr;

}


void KStartSplashDlg::OnStnClickedSplash()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int KStartSplashDlg::GetSystemType() const
{
	return m_nSystemTypeInfo;
}
