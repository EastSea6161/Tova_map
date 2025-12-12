/**
 * @file ScenarioCopyDlg.cpp
 * @brief KScenarioCopyDlg 구현
 * @author 
 * @date 2010.08.17
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "ScenarioCopyDlg.h"
#include "Scenario.h"



/******************************************************************************
* KScenarioCopyDlg                                                            *
******************************************************************************/
IMPLEMENT_DYNAMIC(KScenarioCopyDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KScenarioCopyDlg::KScenarioCopyDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KScenarioCopyDlg::IDD, pParent)
	, m_bCopyResult(false)
{

}


KScenarioCopyDlg::~KScenarioCopyDlg()
{
}



/**************************************
* Overrides                           *
**************************************/
BOOL KScenarioCopyDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_lstDestScenario.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );

	CString strSource;
	GetDlgItem( IDC_STATIC )->GetWindowText( strSource );
	strSource += m_pSourceScenario->GetName();
	GetDlgItem( IDC_STATIC )->SetWindowText( strSource );

	int nRowIndex = 0;
	std::vector< KScenario* >::iterator itScenario = m_DestScenarioArray.begin();
	if (KmzSystem::GetLanguage() == KEMKorea) {
        m_lstDestScenario.InsertColumn( 0, _T("‘~로’ 시나리오 선택") );
    }
	else
	{
		m_lstDestScenario.InsertColumn( 0, _T("Scenario Name") );
	}

	while( m_DestScenarioArray.end() != itScenario )
	{
		m_lstDestScenario.InsertItem( nRowIndex, (*itScenario)->GetName() );
		m_lstDestScenario.SetItemData( nRowIndex, (DWORD_PTR)(*itScenario) );
		++nRowIndex;
		++itScenario;
	}
	m_lstDestScenario.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_DestScenarioArray.clear();

	//화물 프로젝트의 경우 시나리오 복사 시 Inclue Model Results 체크박스를 안나오게 수정
	if (KmzSystem::IsFreightSystem())
		GetDlgItem(IDC_CHECK1)->ShowWindow(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KScenarioCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstDestScenario);
}



BEGIN_MESSAGE_MAP(KScenarioCopyDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KScenarioCopyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KScenarioCopyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KScenarioCopyDlg::OnBnClickedOk()
{
	int nItemCount = m_lstDestScenario.GetItemCount();
	
	for(int i = 0; i < nItemCount; ++i )
	{
		if( m_lstDestScenario.GetCheck( i ) == TRUE )
		{
			m_DestScenarioArray.push_back( (KScenario*)m_lstDestScenario.GetItemData( i ) );
		}
	}

	if (IsDlgButtonChecked(IDC_CHECK1))
		m_bCopyResult = true;	

	OnOK();
}


void KScenarioCopyDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
