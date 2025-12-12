#include "StdAfx.h"
#include "afxdialogex.h"
#include "PA2ODParamImport1Dlg.h"


IMPLEMENT_DYNAMIC(KPA2ODParamImport1Dlg, KDialogEx)

KPA2ODParamImport1Dlg::KPA2ODParamImport1Dlg(
	BOOL a_bContainHeader, int a_nSeparatorType, int a_nEncodingType,
	CString a_strFileName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KPA2ODParamImport1Dlg::IDD, pParent)
	, m_bContainHeader(a_bContainHeader)
	, m_nSeparatorType(a_nSeparatorType)
	, m_nEncodingType(a_nEncodingType)
	, m_strFileName(a_strFileName)
{
}


KPA2ODParamImport1Dlg::~KPA2ODParamImport1Dlg(void)
{
}

void KPA2ODParamImport1Dlg::DoDataExchange( CDataExchange* pDX )
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_edtFile);
	DDX_Check(pDX, IDC_CHECK1, m_bContainHeader);
	DDX_Radio(pDX, IDC_RADIO1, m_nSeparatorType);
	DDX_Radio(pDX, IDC_RADIO5, m_nEncodingType);
}

BEGIN_MESSAGE_MAP(KPA2ODParamImport1Dlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KPA2ODParamImport1Dlg::OnBnClickedOk)
END_MESSAGE_MAP()

void KPA2ODParamImport1Dlg::ContainHeader( BOOL a_bContainHeader )
{
	m_bContainHeader = a_bContainHeader;
}

BOOL KPA2ODParamImport1Dlg::ContainHeader( void )
{
	return m_bContainHeader;
}

void KPA2ODParamImport1Dlg::SeparatorType( int a_nSepartorType )
{
	m_nSeparatorType = a_nSepartorType;
}

int KPA2ODParamImport1Dlg::SeparatorType( void )
{
	return m_nSeparatorType;
}

void KPA2ODParamImport1Dlg::EncodingType( int a_nEncodingType )
{
	m_nEncodingType = a_nEncodingType;
}

int KPA2ODParamImport1Dlg::EncodingType( void )
{
	return m_nEncodingType;
}

void KPA2ODParamImport1Dlg::FileName( CString a_strFileName )
{
	m_strFileName = a_strFileName;
}

CString KPA2ODParamImport1Dlg::FileName( void )
{
	return m_strFileName;
}

BOOL KPA2ODParamImport1Dlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtFile.SetWindowText( m_strFileName );

	TCHAR szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	m_edtFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_edtFile.SetDlgOpenFile( TRUE );
	m_edtFile.SetReadOnly( TRUE );
	m_edtFile.SetDlgTitle( _T("Select file") );
	m_edtFile.SetDlgFilter( szFilter );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPA2ODParamImport1Dlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	m_edtFile.GetWindowText( m_strFileName );

	if (PathFileExists(m_strFileName) == FALSE)
	{
		CString strMsg;
		strMsg.Format(_T("File not exists : %s"), m_strFileName);
		AfxMessageBox(strMsg);
		return;
	}

	KDialogEx::OnOK();
}