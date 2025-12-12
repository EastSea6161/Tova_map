// ResultViewDistributionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ResultViewDistributionDlg.h"
#include "afxdialogex.h"


#include "Target.h"

#include "ViewResultInfo.h"
#include "DBaseResultView.h"
#include "ODResultViewVRecord.h"

// KResultViewDistributionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KResultViewDistributionDlg, CXTResizeDialog)

KResultViewDistributionDlg::KResultViewDistributionDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KResultViewDistributionDlg::IDD, pParent)
	, m_pVirtualRecord(NULL)
{
	m_hICon = AfxGetApp()->LoadIcon(IDR_DLG_ICO);
}

KResultViewDistributionDlg::~KResultViewDistributionDlg()
{
}

void KResultViewDistributionDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeState);
	DDX_Control(pDX, IDC_CUSTOM_XTPREPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KResultViewDistributionDlg, CXTResizeDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &KResultViewDistributionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KResultViewDistributionDlg 메시지 처리기입니다.


BOOL KResultViewDistributionDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultE(m_wndReport, FALSE, FALSE);

	SetResize(IDC_TREE1, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CUSTOM_XTPREPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	
	if (KmzSystem::IsPassengerSystem())
	{
		SetWindowText(_T("Compare Purpose OD"));//목적 OD 데이터 비교
	}
	else
	{
		SetWindowText(_T("품목 P/C 데이터 비교"));
	}

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	SetIcon(m_hICon, TRUE); 

	m_treeState.SetModelChoice(DISTRIBUTION_MODEL);
	m_treeState.UpdateProjectTree();
	m_treeState.SetFont(&XTPAuxData().font);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KResultViewDistributionDlg::InitReportControl( void )
{
	int nColumnItemIndex	= 0;
	int nSelectedItemCount	= m_vecKViewResultInfo.size();

	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Orgin"), 70);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Dest"), 70);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	
	CString strCaption;

	for (int i = 0; i < nSelectedItemCount; ++i)
	{
		KViewResultInfo oViewResultInfo = m_vecKViewResultInfo[i];

		KIOColumn* pColumn = oViewResultInfo.GetColumnInfo();
		KTarget* pTarget = oViewResultInfo.GetTargetInfo();
		
		strCaption = pTarget->Caption() + pColumn->Caption();

		pReportColumn = new CXTPReportColumn(nColumnItemIndex++, strCaption, 110);
		pReportColumn = m_wndReport.AddColumn(pReportColumn);
		pReportColumn->SetHeaderAlignment(DT_CENTER);
		pReportColumn->SetAlignment(DT_CENTER);
	}

	m_wndReport.SetFreezeColumnsCount(2);
	m_wndReport.Populate();
}

void KResultViewDistributionDlg::UpdateReportControl( void )
{
	std::map<KODKey, KSDoubleRecord*> mapResultRecord;

	KDBaseResultView::ODResultView(_T("purpose_od"), m_vecKViewResultInfo, mapResultRecord);

	int nRecordColumnCount = m_vecKViewResultInfo.size() + 3;

	m_pVirtualRecord = new KODResultViewVRecord(mapResultRecord, nRecordColumnCount);

	m_wndReport.SetVirtualMode(m_pVirtualRecord, mapResultRecord.size());
}


void KResultViewDistributionDlg::OnBnClickedOk()
{
	CWaitCursor wait;

	m_wndReport.GetColumns()->Clear();
	m_wndReport.ResetContent();

	m_treeState.ClearViewResultInfo();
	m_treeState.GetTreeInfoList();

	m_vecKViewResultInfo = m_treeState.m_vecKResultInfo;

	m_wndReport.GetColumns()->Clear();
	m_wndReport.ResetContent(TRUE);

	InitReportControl();
	UpdateReportControl();

	m_wndReport.Populate();
}

BOOL KResultViewDistributionDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->wParam == VK_RETURN ||
		pMsg->wParam == VK_ESCAPE)
	{
		// don't allow dialog to process return or escape keys.
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

		return TRUE;
	}
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void KResultViewDistributionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hICon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}
