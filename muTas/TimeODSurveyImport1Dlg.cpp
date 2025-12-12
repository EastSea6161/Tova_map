// TimeODSurveyImport.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODSurveyImport1Dlg.h"
#include "afxdialogex.h"


// KTimeODSurveyImport 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODSurveyImport1Dlg, KDialogEx)

KTimeODSurveyImport1Dlg::KTimeODSurveyImport1Dlg(
	BOOL a_bContainHeader, int a_nSeparatorType, int a_nEncodingType,
	CString a_strFileName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTimeODSurveyImport1Dlg::IDD, pParent)
	, m_bContainHeader(a_bContainHeader)
	, m_nSeparatorType(a_nSeparatorType)
	, m_nEncodingType(a_nEncodingType)
	, m_strFileName(a_strFileName)
{
}

KTimeODSurveyImport1Dlg::~KTimeODSurveyImport1Dlg()
{
}

void KTimeODSurveyImport1Dlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_edtFile);
	DDX_Check(pDX, IDC_CHECK1, m_bContainHeader);
	DDX_Radio(pDX, IDC_RADIO1, m_nSeparatorType);
	DDX_Radio(pDX, IDC_RADIO5, m_nEncodingType);
}


BEGIN_MESSAGE_MAP(KTimeODSurveyImport1Dlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTimeODSurveyImport1Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODSurveyImport 메시지 처리기입니다.

void KTimeODSurveyImport1Dlg::ContainHeader( BOOL a_bContainHeader )
{
	m_bContainHeader = a_bContainHeader;
}

BOOL KTimeODSurveyImport1Dlg::ContainHeader( void )
{
	return m_bContainHeader;
}

void KTimeODSurveyImport1Dlg::SeparatorType( int a_nSeparatorType )
{
	m_nSeparatorType = a_nSeparatorType;
}

int KTimeODSurveyImport1Dlg::SeparatorType( void )
{
	return m_nSeparatorType;
}

void KTimeODSurveyImport1Dlg::EncodingType( int a_nEncodingType )
{
	m_nEncodingType = a_nEncodingType;
}

int KTimeODSurveyImport1Dlg::EncodingType( void )
{
	return m_nEncodingType;
}

void KTimeODSurveyImport1Dlg::FileName( CString a_strFileName )
{
	m_strFileName = a_strFileName;
}

CString KTimeODSurveyImport1Dlg::FileName( void )
{
	return m_strFileName;
}

BOOL KTimeODSurveyImport1Dlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtFile.SetWindowText(m_strFileName);

	TCHAR szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	m_edtFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_edtFile.SetDlgOpenFile(TRUE);
	m_edtFile.SetReadOnly(TRUE);
	m_edtFile.SetDlgTitle(_T("Select file"));
	m_edtFile.SetDlgFilter(szFilter);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTimeODSurveyImport1Dlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_edtFile.GetWindowText(m_strFileName);

	if (FALSE == PathFileExists(m_strFileName))
	{
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("해당 파일이 존재하지 않습니다 : %s"), m_strFileName);
		AfxMessageBox(strErrorMsg);

		return;
	}
	KDialogEx::OnOK();
}
