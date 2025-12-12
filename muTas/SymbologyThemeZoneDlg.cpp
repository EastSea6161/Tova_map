// SymbologyThemeZoneDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SymbologyThemeZoneDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
#include "MapView.h"

// KSymbologyThemeZoneDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSymbologyThemeZoneDlg, KDialogEx)

KSymbologyThemeZoneDlg::KSymbologyThemeZoneDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KSymbologyThemeZoneDlg::IDD, pParent)
{

}

KSymbologyThemeZoneDlg::~KSymbologyThemeZoneDlg()
{
}

void KSymbologyThemeZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KSymbologyThemeZoneDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,  &KSymbologyThemeZoneDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_APPLY,  &KSymbologyThemeZoneDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL,          &KSymbologyThemeZoneDlg::OnBnClickedButtonCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SED, IDC_RADIO_ZONE, &KSymbologyThemeZoneDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_CHECK1, &KSymbologyThemeZoneDlg::OnBnClickedCheckLabel)
END_MESSAGE_MAP()


// KSymbologyThemeZoneDlg 메시지 처리기입니다.

BOOL KSymbologyThemeZoneDlg::OnInitDialog()
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

	CheckRadioButton(IDC_RADIO_SED, IDC_RADIO_ZONE, IDC_RADIO_SED);
	ControlRadio();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);

	//m_dlgThemeZoneValueClassify.SetZoneTable(pTable);
	m_dlgThemeZoneValueClassify.Create(IDD_TOC_ZONE_VALUE_CLASSIFY, this);
	m_dlgThemeZoneValueClassify.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_dlgThemeZoneValueClassify.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_dlgThemeZoneValueClassify.SetTargetMap(m_pTarget, m_pMapView, m_bZoneTable);
	m_dlgThemeZoneValueClassify.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	m_dlgThemeZoneValueClassify.GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);

	m_dlgThemeZoneValueClassify.ShowWindow(SW_SHOW);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSymbologyThemeZoneDlg::OnBnClickedButtonClear()
{
	m_dlgThemeZoneValueClassify.Clear();
}


void KSymbologyThemeZoneDlg::OnBnClickedButtonApply()
{
	bool bLabel(false);
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		bLabel = true;
	}

	m_dlgThemeZoneValueClassify.Draw(bLabel);
}


void KSymbologyThemeZoneDlg::OnBnClickedButtonCancel()
{
	Clear();
	KDialogEx::OnCancel();
}


void KSymbologyThemeZoneDlg::Clear()
{
	try
	{
		m_pMapView->ClearZoneThemes();
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

	 
void KSymbologyThemeZoneDlg::OnBnClickedRadio( UINT nID )
{
	ControlRadio();
	m_dlgThemeZoneValueClassify.ChangeTable(m_bZoneTable);
	Clear();
}


void KSymbologyThemeZoneDlg::ControlRadio()
{
	if (GetCheckedRadioButton(IDC_RADIO_SED, IDC_RADIO_ZONE) == IDC_RADIO_SED)
	{
		m_bZoneTable = false;
	}
	else
	{
		m_bZoneTable = true;
	}
}


void KSymbologyThemeZoneDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bLabel = true;
		}

		m_pMapView->SetZoneThemesLabel(bLabel);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
