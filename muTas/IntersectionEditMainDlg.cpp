// IntersectionEditMainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "IntersectionEditMainDlg.h"
#include "afxdialogex.h"
#include "MapView.h"
#include "Target.h"
//^^ #include "IOTable.h"

// KIntersectionEditMainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionEditMainDlg, KDialogEx)

KIntersectionEditMainDlg::KIntersectionEditMainDlg(KTarget* a_pTarget, KMapView* a_pMapView, TIntersectionBase a_oTSelectIntersection, std::map<Integer, TBoundInfo> a_mapBoundInfo, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KDialogEx(KIntersectionEditMainDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_oTSelectIntersection(a_oTSelectIntersection)
	, m_mapBoundInfo(a_mapBoundInfo)
	, m_pTable(a_pTable)
	, m_oTopologyDlg  (a_pTarget->GetDBaseConnection(), a_oTSelectIntersection, a_mapBoundInfo, this)
	, m_oBasicFourDlg (a_pTarget->GetDBaseConnection(), a_oTSelectIntersection, a_mapBoundInfo, this)
	, m_oBasicThreeDlg(a_pTarget->GetDBaseConnection(), a_oTSelectIntersection, a_mapBoundInfo, this)
	, m_oExtensionDlg (a_pTarget->GetDBaseConnection(), a_oTSelectIntersection, this)
{

}

KIntersectionEditMainDlg::~KIntersectionEditMainDlg()
{
}

void KIntersectionEditMainDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KIntersectionEditMainDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO3, &KIntersectionEditMainDlg::OnBnClickedRadioOption)
	ON_BN_CLICKED(IDC_BUTTON1, &KIntersectionEditMainDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL, &KIntersectionEditMainDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KIntersectionEditMainDlg 메시지 처리기입니다.


BOOL KIntersectionEditMainDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_oTopologyDlg.Create(KIntersectionTopologyPage::IDD, this);
	m_oTopologyDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oTopologyDlg.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	int &nIntersectionType = m_oTSelectIntersection.nType;
	if( KEMIntersectionTypeThree == nIntersectionType )
	{
		m_oBasicThreeDlg.Create(KIntersectionThreeBasicPage::IDD, this);
		m_oBasicThreeDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
		m_oBasicThreeDlg.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	}
	else if( KEMIntersectionTypeFour == nIntersectionType ) 
	{
		m_oBasicFourDlg.Create(KIntersectionBasicPage::IDD, this);
		m_oBasicFourDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
		m_oBasicFourDlg.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	}

	m_oExtensionDlg.Create(KIntersectionExtensionPage::IDD, this);
	m_oExtensionDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oExtensionDlg.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	RadioControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KIntersectionEditMainDlg::RadioControl()
{
	try
	{
		int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);

		KDialogEx* pBasicDlg = nullptr;


		int &nIntersectionType = m_oTSelectIntersection.nType;
		if( KEMIntersectionTypeThree == nIntersectionType )
		{
			pBasicDlg = &m_oBasicThreeDlg;
		}
		else if( KEMIntersectionTypeFour == nIntersectionType ) 
		{
			pBasicDlg = &m_oBasicFourDlg;
		}

		if(IDC_RADIO1 == nCheckedRadioBtn)
		{
			m_oTopologyDlg.ShowWindow(SW_SHOW);
			pBasicDlg->ShowWindow(SW_HIDE);
			m_oExtensionDlg.ShowWindow(SW_HIDE);
		}
		else if(IDC_RADIO2 == nCheckedRadioBtn)
		{
			m_oTopologyDlg.ShowWindow(SW_HIDE);
			pBasicDlg->ShowWindow(SW_SHOW);
			m_oExtensionDlg.ShowWindow(SW_HIDE);
		}
		else if (IDC_RADIO3 == nCheckedRadioBtn)
		{
			m_oTopologyDlg.ShowWindow(SW_HIDE);
			pBasicDlg->ShowWindow(SW_HIDE);
			m_oExtensionDlg.ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionEditMainDlg::OnBnClickedRadioOption(UINT nID)
{
	RadioControl();
}


void KIntersectionEditMainDlg::OnBnClickedButtonSave()
{
	try
	{
		CWaitCursor cw;
		if (m_oTopologyDlg.SaveTopology() == FALSE)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			RadioControl();
			return;
		}
		
		int &nIntersectionType = m_oTSelectIntersection.nType;
		if( KEMIntersectionTypeThree == nIntersectionType )
		{
			if (m_oBasicThreeDlg.SaveBasicThree() == FALSE)
			{
				CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
				RadioControl();
				return;
			}
		}
		else if( KEMIntersectionTypeFour == nIntersectionType ) 
		{
			if (m_oBasicFourDlg.SaveBasicFour() == FALSE)
			{
				CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
				RadioControl();
				return;
			}
		}

		if (m_oExtensionDlg.SaveExtension() == FALSE)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			RadioControl();
			return;
		}

		m_pTable->Notify();

		AfxMessageBox(_T("저장하였습니다."));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionEditMainDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}
