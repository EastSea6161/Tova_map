// PreferenceMaxNetworkID.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PreferenceMaxNetworkID.h"
#include "afxdialogex.h"


// KPreferenceMaxNetworkID 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPreferenceMaxNetworkID, KDialogEx)

KPreferenceMaxNetworkID::KPreferenceMaxNetworkID(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPreferenceMaxNetworkID::IDD, pParent)
{
    m_dNodeFontSize   = ImChampMapDefineLabel::NodeLabelSize;
    m_dLinkFontSize   = ImChampMapDefineLabel::LinkLabelSize;
}

KPreferenceMaxNetworkID::~KPreferenceMaxNetworkID()
{
}

void KPreferenceMaxNetworkID::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KPreferenceMaxNetworkID, KDialogEx)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// KPreferenceMaxNetworkID 메시지 처리기입니다.
BOOL KPreferenceMaxNetworkID::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetBackgroundColor(RGB(255,255,255));
       
    CString strTemp(_T(""));
    strTemp.Format(_T("%.f"), m_dNodeFontSize);
    SetDlgItemText(IDC_EDIT_MAX_NODE_ID, strTemp);

    strTemp.Format(_T("%.f"), m_dLinkFontSize);
    SetDlgItemText(IDC_EDIT_MAX_LINK_ID, strTemp);

    return TRUE;
}

HBRUSH KPreferenceMaxNetworkID::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  여기서 DC의 특성을 변경합니다.
    pDC->SetBkMode(TRANSPARENT);
    hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

    return hbr;
}

void KPreferenceMaxNetworkID::UserCommand(int a_nCommand)
{
    CString strTemp(_T(""));
    GetDlgItemText(IDC_EDIT_MAX_NODE_ID, strTemp);
    ImChampMapDefineLabel::NodeLabelSize = _ttof(strTemp);

    GetDlgItemText(IDC_EDIT_MAX_LINK_ID, strTemp);
    ImChampMapDefineLabel::LinkLabelSize = _ttof(strTemp);    
}
