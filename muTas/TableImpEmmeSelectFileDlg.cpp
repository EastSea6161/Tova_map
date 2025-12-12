// TableImpEmmeSelectFileDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableImpEmmeSelectFileDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "Project.h"
#include "Scenario.h"


// KTableImpEmmeSelectFileDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableImpEmmeSelectFileDlg, KDialogEx)

KTableImpEmmeSelectFileDlg::KTableImpEmmeSelectFileDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableImpEmmeSelectFileDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_strFile(_T(""))
{

}

KTableImpEmmeSelectFileDlg::~KTableImpEmmeSelectFileDlg()
{
	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject  = (KProject*)pScenario->GetParentObject();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());
}

void KTableImpEmmeSelectFileDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_editFile);
}


BEGIN_MESSAGE_MAP(KTableImpEmmeSelectFileDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTableImpEmmeSelectFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTableImpEmmeSelectFileDlg 메시지 처리기입니다.


BOOL KTableImpEmmeSelectFileDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ASSERT(m_pTarget != NULL);

	m_editFile.SetWindowText( m_strFile );

	TCHAR szFilter[] = _T("Emme Files (*.in)|*.in|Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editFile.SetDlgOpenFile( TRUE );
	m_editFile.SetReadOnly( TRUE );
	m_editFile.SetDlgTitle( _T("Select file") );
	m_editFile.SetDlgFilter( szFilter );

	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject  = (KProject*)pScenario->GetParentObject();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());
	//m_editFile.SetDlgInitialDir( m_pTarget->GetLocation() );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableImpEmmeSelectFileDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	m_editFile.GetWindowText( m_strFile );

	if( PathFileExists( m_strFile ) == FALSE )
	{
		CString strMsg;
		strMsg.Format( _T("해당 파일이 존재하지 않습니다 : %s"), m_strFile );
		AfxMessageBox( strMsg );
		return;
	}

	KDialogEx::OnOK();
}
