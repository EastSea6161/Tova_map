/**
 * @file ScenarioAddYearDlg.cpp
 * @brief KScenarioAddYearDlg 구현
 * @author 
 * @date 2010.08.18
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "ScenarioAddYearDlg.h"



/******************************************************************************
* KScenarioAddYearDlg                                                       *
******************************************************************************/
IMPLEMENT_DYNAMIC(kScenarioAddYearDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
kScenarioAddYearDlg::kScenarioAddYearDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(kScenarioAddYearDlg::IDD, pParent)
	, m_nTargetYear(2012)
{
    CTime timeCurrent = CTime::GetCurrentTime();
    m_nTargetYear     = timeCurrent.GetYear();
}


kScenarioAddYearDlg::~kScenarioAddYearDlg()
{
}



/**************************************
* Operations                          *
**************************************/
bool kScenarioAddYearDlg::isYearExist( int nYear )
{
	std::vector< int >::iterator itYearArray = m_YearArray.begin();
	while( m_YearArray.end() != itYearArray )
	{
		if( (*itYearArray) == nYear )
		{
			return true;
		}
		++itYearArray;
	}
	return false;
}



/**************************************
* Overrides                           *
**************************************/
BOOL kScenarioAddYearDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	std::vector< int > ::iterator itYear = m_YearArray.begin();
	while( m_YearArray.end() != itYear )
	{
		if( m_nTargetYear < (*itYear + 5) )
		{
			m_nTargetYear = (*itYear + 5);
		}
		++itYear;
	}

	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void kScenarioAddYearDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_nTargetYear);
}



BEGIN_MESSAGE_MAP(kScenarioAddYearDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &kScenarioAddYearDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &kScenarioAddYearDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void kScenarioAddYearDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	if( isYearExist( m_nTargetYear) == true )
	{
		CString strMsg;
		strMsg.Format( _T("동일한 분석 년도('%d')가 이미 존재합니다."), m_nTargetYear );
		AfxMessageBox( strMsg );
		GetDlgItem( IDC_EDIT2 )->SetFocus();
		return;
	}

	OnOK();
}




void kScenarioAddYearDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}
