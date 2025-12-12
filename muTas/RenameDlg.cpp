/**
 * @file RenameDlg.cpp
 * @brief KRenameDlg 구현
 * @author 
 * @date 2010.08.18
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "RenameDlg.h"


/******************************************************************************
* KRenameDlg                                                                  *
******************************************************************************/
IMPLEMENT_DYNAMIC(KRenameDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KRenameDlg::KRenameDlg(CWnd* pParent /*=NULL*/, KEMRenameType a_emRenameType)
	: KDialogEx(KRenameDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strOriginScenarioName(_T(""))
	, m_emRenameType(a_emRenameType)
{

}


KRenameDlg::~KRenameDlg()
{
}



/**************************************
* Operations                          *
**************************************/
bool KRenameDlg::isNameExist( CString strName )
{
	std::vector< CString >::iterator itNameArray = m_NameArray.begin();
	while( m_NameArray.end() != itNameArray )
	{
		if( (*itNameArray).CompareNoCase(strName) == 0 && strName.CompareNoCase(m_strOriginScenarioName) != 0)
		{
			return true;
		}
		++itNameArray;
	}

	return false;
}

/**************************************
* Overrides                           *
**************************************/
BOOL KRenameDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetWindowText( m_strCaption );

	if (m_emRenameType == KEMRenameTarget)
		GetDlgItem(IDC_EDIT1)->ModifyStyle(0, ES_NUMBER);
		
	AddImChampTooltip(IDC_EDIT1, _T("이름에 특수 문자를 사용할 수 없습니다.\r\n\\/:*?\"<>|"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
}



BEGIN_MESSAGE_MAP(KRenameDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KRenameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KRenameDlg::OnBnClickedOk()
{
	CString strName(_T(""));
	GetDlgItemText(IDC_EDIT1, strName);
	strName.Trim();
	SetDlgItemText(IDC_EDIT1, strName);

	if (m_emRenameType == KEMRenameScenario)
	{
		if (strName.CompareNoCase(_T("BASE")) == 0)
		{
			CString strMsg;
			strMsg.Format(_T("사용할 수 없는 시나리오 명칭입니다."));
			AfxMessageBox(strMsg);
			GetDlgItem(IDC_EDIT1)->SetFocus();

			return;
		}
	}

	//중복체크
	if( isNameExist( strName ) == true)
	{
		CString strMsg;
		strMsg.Format( _T("'%s' 시나리오는 이미 존재합니다.\n다시 입력해 주세요."), strName );
		AfxMessageBox( strMsg );
		GetDlgItem( IDC_EDIT1 )->SetFocus();
		return;
	}
	
	//사용가능
    CString strErroMsg(_T(""));
	if ( !QBicStringChecker::IsValidFileName( strName, strErroMsg ))
	{
		AfxMessageBox(_T("이름에는 다음과 같은 문자를 허용하지 않습니다.\r\n \\/:*?\"<>|")); // RNameDlg는 시나리오에서만 사용 되는 클래스가 아니기 때문에 '이름에는~' 문구로 메시지 시작
		GetDlgItem( IDC_EDIT1 )->SetFocus();
		return;
	}

	KDialogEx::OnOK();
}

