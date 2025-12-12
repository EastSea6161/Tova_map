// SymbologyThemeLinkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SymbologyThemeLinkDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "MapView.h"

// KSymbologyThemeLinkDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSymbologyThemeLinkDlg, KDialogEx)

KSymbologyThemeLinkDlg::KSymbologyThemeLinkDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KSymbologyThemeLinkDlg::IDD, pParent)
	, m_nRadio(0)
{
}

KSymbologyThemeLinkDlg::~KSymbologyThemeLinkDlg()
{
}

void KSymbologyThemeLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}

BEGIN_MESSAGE_MAP(KSymbologyThemeLinkDlg, KDialogEx)
	ON_BN_CLICKED(IDC_RADIO1,        &KSymbologyThemeLinkDlg::OnBnClickedRadioScale)
	ON_BN_CLICKED(IDC_RADIO2,        &KSymbologyThemeLinkDlg::OnBnClickedRadioClassification)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,  &KSymbologyThemeLinkDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_APPLY,  &KSymbologyThemeLinkDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL,          &KSymbologyThemeLinkDlg::OnBnClickedButtonCancel)
    ON_BN_CLICKED(IDC_CHECK1, &KSymbologyThemeLinkDlg::OnBnClickedCheckLabel)
END_MESSAGE_MAP()


// KSymbologyThemeLinkDlg 메시지 처리기입니다.

BOOL KSymbologyThemeLinkDlg::OnInitDialog()
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
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);

	m_dlgThemeLinkValueScale.SetLinkTable(pIOTable);
	m_dlgThemeLinkValueScale.Create(IDD_TOC_LINK_VALUE_SCALE, this);
	m_dlgThemeLinkValueScale.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_dlgThemeLinkValueScale.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_dlgThemeLinkValueScale.SetTargetMap(m_pTarget, m_pMapView);
	m_dlgThemeLinkValueScale.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	m_dlgThemeLinkValueScale.GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);

	m_dlgThemeLinkValueClassify.SetLinkTable(pIOTable);
	m_dlgThemeLinkValueClassify.Create(IDD_TOC_LINK_VALUE_CLASSIFY, this);
	m_dlgThemeLinkValueClassify.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_dlgThemeLinkValueClassify.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_dlgThemeLinkValueClassify.SetTargetMap(m_pTarget, m_pMapView);
	m_dlgThemeLinkValueClassify.GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	m_dlgThemeLinkValueClassify.GetDlgItem(IDC_BUTTON_APPLY)->ShowWindow(SW_HIDE);

	RadioControl();
	UpdateData(FALSE);

	return TRUE;  
}


void KSymbologyThemeLinkDlg::OnBnClickedRadioScale()
{
	RadioControl();
}


void KSymbologyThemeLinkDlg::OnBnClickedRadioClassification()
{
	RadioControl();
}


void KSymbologyThemeLinkDlg::RadioControl()
{
	UpdateData(TRUE);
	if(m_nRadio == 0)
	{
		m_dlgThemeLinkValueScale.ShowWindow(TRUE);
		m_dlgThemeLinkValueClassify.ShowWindow(FALSE);
	}
	else
	{
		m_dlgThemeLinkValueScale.ShowWindow(FALSE);
		m_dlgThemeLinkValueClassify.ShowWindow(TRUE);
	}
	Clear();
}


void KSymbologyThemeLinkDlg::OnBnClickedButtonClear()
{
    Clear();
}

void KSymbologyThemeLinkDlg::Clear()
{
    try
    {
        m_pMapView->ClearLinkThemes();
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

void KSymbologyThemeLinkDlg::OnBnClickedButtonApply()
{   
	UpdateData(TRUE);

    bool bLabel(false);
    if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
    {
        bLabel = true;
    }

    bool bResult(true);

	if(m_nRadio == 0)
	{	
        m_dlgThemeLinkValueScale.Draw(bLabel);
	}
	else
	{        
		m_dlgThemeLinkValueClassify.Draw(bLabel);		
	}
}

void KSymbologyThemeLinkDlg::NodeLayerSettingChanged()
{   
    if(m_nRadio == 0) {
        m_dlgThemeLinkValueScale.NodeLayerSettingChanged();
    }
    else {
        m_dlgThemeLinkValueClassify.NodeLayerSettingChanged();
    }
}

void KSymbologyThemeLinkDlg::OnBnClickedButtonCancel()
{
    Clear();
	KDialogEx::OnCancel();
}

void KSymbologyThemeLinkDlg::OnBnClickedCheckLabel()
{
    bool bLabel(false);
    if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
    {
        bLabel = true;
    }

    m_pMapView->SetLinkThemesLabel(bLabel);
}

void KSymbologyThemeLinkDlg::ModelessInitialize()
{
    
}


