// ViewResultAssignmentDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ResultViewAssignmentDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Target.h"
#include "Scenario.h"
#include "ViewResultInfo.h"

#include "DBaseResultView.h"

// KViewResultAssignmentDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KResultViewAssignmentDlg, CXTResizeDialog)

KResultViewAssignmentDlg::KResultViewAssignmentDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KResultViewAssignmentDlg::IDD,pParent)
{
	m_hICon = AfxGetApp()->LoadIcon(IDR_DLG_ICO);
}

KResultViewAssignmentDlg::~KResultViewAssignmentDlg()
{
}

void KResultViewAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeState);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndReport);
}


BEGIN_MESSAGE_MAP(KResultViewAssignmentDlg, CXTResizeDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &KResultViewAssignmentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KResultViewAssignmentDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL KResultViewAssignmentDlg::OnInitDialog()
{
	TxLogDebugStartMsg();
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultE(m_wndReport, FALSE, FALSE);

	SetResize(IDC_TREE1, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CUSTOM1, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	SetIcon(m_hICon, TRUE); 

	m_treeState.SetModelChoice(ASSIGNMENT_MODEL);
	m_treeState.UpdateProjectTree();
	m_treeState.SetFont(&XTPAuxData().font);

	TxLogDebugEndMsg();
	return TRUE;

}

void KResultViewAssignmentDlg::OnPaint()
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

BOOL KResultViewAssignmentDlg::PreTranslateMessage( MSG* pMsg )
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


void KResultViewAssignmentDlg::OnBnClickedOk()
{
	TxLogDebugStartMsg();

	CWaitCursor wait;

	m_treeState.ClearViewResultInfo();
	m_treeState.GetTreeInfoList();
	ClearResultInfo();
	m_vecKViewResultInfo = m_treeState.m_vecKResultInfo;

	m_wndReport.GetColumns()->Clear();
	m_wndReport.ResetContent(TRUE);

	InitReportControl();
	UpdateReportControl();
	TxLogDebugEndMsg();
}

void KResultViewAssignmentDlg::InitReportControl( void )
{
	int nItemIndex = 0;

	const int HEADER_WIDTH = 70;

	const CString HEADER_LINK_ID	= _T("LINK ID");
	const CString HEADER_LINK_FNODE = _T("F_NODE");
	const CString HEADER_LINK_TNODE = _T("T_NODE");

	CXTPReportColumn* pRerpotColumn = NULL;

	pRerpotColumn = new CXTPReportColumn(nItemIndex++, HEADER_LINK_ID, HEADER_WIDTH);
	pRerpotColumn = m_wndReport.AddColumn(pRerpotColumn);
	pRerpotColumn->SetHeaderAlignment(DT_CENTER);
	pRerpotColumn->SetAlignment(DT_CENTER);


	pRerpotColumn = new CXTPReportColumn(nItemIndex++, HEADER_LINK_FNODE, HEADER_WIDTH);
	pRerpotColumn = m_wndReport.AddColumn(pRerpotColumn);
	pRerpotColumn->SetHeaderAlignment(DT_CENTER);
	pRerpotColumn->SetAlignment(DT_CENTER);


	pRerpotColumn = new CXTPReportColumn(nItemIndex++, HEADER_LINK_TNODE, HEADER_WIDTH);
	pRerpotColumn = m_wndReport.AddColumn(pRerpotColumn);
	pRerpotColumn->SetHeaderAlignment(DT_CENTER);
	pRerpotColumn->SetAlignment(DT_CENTER);


	size_t nxCount = m_vecKViewResultInfo.size();
	for (size_t i = 0; i < nxCount; i++)
	{
		KViewResultInfo oKResultInfo = m_vecKViewResultInfo[i];
		KIOColumn* pKColumn = NULL;
		pKColumn = oKResultInfo.GetColumnInfo();

		KTarget* pKTarget = NULL;
		pKTarget = oKResultInfo.GetTargetInfo();

		CString strCaption = _T("");
		CString strTargetName = pKTarget->Caption();
		CString strColumnName = pKColumn->Caption();
		strCaption.Format(_T("%s_%s"),strTargetName,strColumnName);

		pRerpotColumn = new CXTPReportColumn(nItemIndex++, strCaption, 110);
		pRerpotColumn = m_wndReport.AddColumn(pRerpotColumn);
		pRerpotColumn->SetHeaderAlignment(DT_CENTER);
		pRerpotColumn->SetAlignment(DT_RIGHT);
	}

	m_wndReport.SetFreezeColumnsCount(3);
	m_wndReport.Populate();

}

void KResultViewAssignmentDlg::UpdateReportControl( void )
{
	std::map<KLinkKey, KSDoubleRecord*> mapAssingmentResultRecord;
	
	KDBaseResultView::AssignResultView(m_vecKViewResultInfo,mapAssingmentResultRecord);

	std::map<KLinkKey, KSDoubleRecord*>::iterator Iter	        = mapAssingmentResultRecord.begin();
	std::map<KLinkKey, KSDoubleRecord*>::iterator IterEnd		= mapAssingmentResultRecord.end();
	for (Iter; Iter != IterEnd; Iter++)
	{
		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		KLinkKey oLinkKey = Iter->first;

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)oLinkKey.LinkID));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)oLinkKey.FromNodeID));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)oLinkKey.ToNodeID));

		KSDoubleRecord* pRecord = Iter->second;
		int nRecordCount = pRecord->GetCount();

		for (int i = 0; i < nRecordCount; ++i )
		{
			double fff = pRecord->GetAt(i);
			pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pRecord->GetAt(i), _T("%.3f")));
		}
	}
	m_wndReport.Populate();
}


void KResultViewAssignmentDlg::OnBnClickedCancel()
{
	m_treeState.GetTreeInfoList();
	ClearResultInfo();

	CXTResizeDialog::OnCancel();
}

void KResultViewAssignmentDlg::ClearResultInfo()
{
	size_t nxCount = m_vecKViewResultInfo.size();
	for (size_t i = 0; i < nxCount; i++)
	{
		KViewResultInfo oKResultInfo = m_vecKViewResultInfo[i];
		KIOColumn* pKColumn   = NULL;
		KTarget*   pKTarget   = NULL;
		KScenario* pKScenario = NULL;
		oKResultInfo.SetColumnInfo(pKColumn);
		oKResultInfo.SetScenarioInfo(pKScenario);
		oKResultInfo.SetTargetInfo(pKTarget);
	}

	std::vector<KViewResultInfo> vecResultInfo;
	vecResultInfo.swap(m_vecKViewResultInfo);
}
