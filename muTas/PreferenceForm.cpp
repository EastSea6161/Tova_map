// PreferenceForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PreferenceForm.h"
#include "afxdialogex.h"


// KPreferenceForm 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPreferenceForm, KDialogEx)

KPreferenceForm::KPreferenceForm(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPreferenceForm::IDD, pParent)
{
    m_pCurrentShowClientDlg = NULL;
}

KPreferenceForm::~KPreferenceForm()
{
    m_pCurrentShowClientDlg = NULL;
}

void KPreferenceForm::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(KPreferenceForm, KDialogEx)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL, &KPreferenceForm::OnTcnSelchangeTab)
    ON_BN_CLICKED(IDC_BTN_APPLY, &KPreferenceForm::OnBnClickedBtnApply)
END_MESSAGE_MAP()


BOOL KPreferenceForm::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    CRect oTabRect, oWindowRect;
    GetWindowRect(oWindowRect);
    m_tabCtrl.GetClientRect(&oTabRect);

    m_tabCtrl.InsertItem(0, _T("Config map - Label"));
    m_tabCtrl.SetCurSel(0);
	m_tabCtrl.InsertItem(1, _T("Symbol"));
	m_tabCtrl.SetCurSel(1);

    int TAB_LEFT_MARGIN   = 5;
    int TAB_TOP_MARGIN    = 25;
    int TAB_WIDTH_MARGIN  = TAB_LEFT_MARGIN*2;
    int TAB_HEIGHT_MARGIN = 30;

    m_dlgPreferenceLabel.Create(IDD_0100_Preference_Label, &m_tabCtrl);
    m_dlgPreferenceLabel.SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

	m_dlgPreferenceSymbol.Create(IDD_0100_Preference_Symbol, &m_tabCtrl);
	m_dlgPreferenceSymbol.SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

    DisplayTabControl(0);	

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPreferenceForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    DisplayTabControl(m_tabCtrl.GetCurSel());
    *pResult = 0;
}

void KPreferenceForm::DisplayTabControl(int a_nTabIndex)
{
    if(m_pCurrentShowClientDlg != NULL)
    {        
        m_pCurrentShowClientDlg->ShowWindow(SW_HIDE);
        m_pCurrentShowClientDlg = NULL;
    }

    int nIndex    = a_nTabIndex; 
    int nTabCount = m_tabCtrl.GetItemCount();
    if (nIndex > nTabCount-1)
    {
        nIndex = 0;
    }

    m_tabCtrl.SetCurSel(nIndex);

    switch(nIndex)
    {
    case 0:
        m_dlgPreferenceLabel.ShowWindow(SW_SHOW);
        m_pCurrentShowClientDlg = &m_dlgPreferenceLabel;
        break;
	case 1:
		m_dlgPreferenceSymbol.ShowWindow(SW_SHOW);
		m_pCurrentShowClientDlg = &m_dlgPreferenceSymbol;
		break;
    default:
        break;
    }
}

void KPreferenceForm::OnBnClickedBtnApply()
{
    m_dlgPreferenceLabel.UserCommand();
	m_dlgPreferenceSymbol.UserCommand();
	KDialogEx::OnOK();
}
