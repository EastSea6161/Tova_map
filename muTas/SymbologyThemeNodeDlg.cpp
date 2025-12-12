// SymbologyThemeNodeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SymbologyThemeNodeDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^^ #include "TableOwner.h"
//^^ #include "IOTable.h"
#include "MapView.h"
// KSymbologyThemeNodeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSymbologyThemeNodeDlg, KDialogEx)

KSymbologyThemeNodeDlg::KSymbologyThemeNodeDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KSymbologyThemeNodeDlg::IDD, pParent)
	, m_nRadio(0)
{

}

KSymbologyThemeNodeDlg::~KSymbologyThemeNodeDlg()
{
}

void KSymbologyThemeNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}


BEGIN_MESSAGE_MAP(KSymbologyThemeNodeDlg, KDialogEx)
	ON_BN_CLICKED(IDC_RADIO1,        &KSymbologyThemeNodeDlg::OnBnClickedRadioScale)
	ON_BN_CLICKED(IDC_RADIO2,        &KSymbologyThemeNodeDlg::OnBnClickedRadioClassification)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,  &KSymbologyThemeNodeDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_APPLY,  &KSymbologyThemeNodeDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL,          &KSymbologyThemeNodeDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_CHECK1,        &KSymbologyThemeNodeDlg::OnBnClickedCheckLabel)
END_MESSAGE_MAP()


// KSymbologyThemeNodeDlg 메시지 처리기입니다.

BOOL KSymbologyThemeNodeDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	//####
	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

	m_dlgThemeNodeValueScale.SetNodeTable(pIOTable);
	m_dlgThemeNodeValueScale.Create(IDD_TOC_NODE_VALUE_SCALE, this);
	m_dlgThemeNodeValueScale.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_dlgThemeNodeValueScale.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_dlgThemeNodeValueScale.SetTargetMap(m_pTarget, m_pMapView);
	m_dlgThemeNodeValueScale.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	m_dlgThemeNodeValueScale.GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);

	m_dlgThemeNodeValueClassify.SetNodeTable(pIOTable);
	m_dlgThemeNodeValueClassify.Create(IDD_TOC_NODE_VALUE_CLASSIFY, this);
	m_dlgThemeNodeValueClassify.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_dlgThemeNodeValueClassify.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_dlgThemeNodeValueClassify.SetTargetMap(m_pTarget, m_pMapView);
	m_dlgThemeNodeValueClassify.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	m_dlgThemeNodeValueClassify.GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);

	RadioControl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSymbologyThemeNodeDlg::OnBnClickedRadioScale()
{
	RadioControl();
}


void KSymbologyThemeNodeDlg::OnBnClickedRadioClassification()
{
	RadioControl();
}


void KSymbologyThemeNodeDlg::RadioControl()
{
	UpdateData(TRUE);
	if(m_nRadio == 0)
	{
		m_dlgThemeNodeValueScale.ShowWindow(TRUE);
		m_dlgThemeNodeValueClassify.ShowWindow(FALSE);
	}
	else
	{
		m_dlgThemeNodeValueScale.ShowWindow(FALSE);
		m_dlgThemeNodeValueClassify.ShowWindow(TRUE);
	}
	Clear();
}


void KSymbologyThemeNodeDlg::OnBnClickedButtonClear()
{
	UpdateData(TRUE);
	if(m_nRadio == 0)
	{
		m_dlgThemeNodeValueScale.Clear();
	}
	else
	{
		m_dlgThemeNodeValueClassify.Clear();
	}
}


void KSymbologyThemeNodeDlg::OnBnClickedButtonApply()
{
	UpdateData(TRUE);

	bool bLabel(false);
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		bLabel = true;
	}

	if(m_nRadio == 0)
	{
		m_dlgThemeNodeValueScale.Draw(bLabel);
	}
	else
	{
		m_dlgThemeNodeValueClassify.Draw(bLabel);
	}
}


void KSymbologyThemeNodeDlg::OnBnClickedButtonCancel()
{
	Clear();
	KDialogEx::OnCancel();
}


void KSymbologyThemeNodeDlg::Clear()
{
	try
	{
		m_pMapView->ClearNodeThemes();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KSymbologyThemeNodeDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bLabel = true;
		}

		m_pMapView->SetNodeThemesLabel(bLabel);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
