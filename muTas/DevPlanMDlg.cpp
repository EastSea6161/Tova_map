// DevPlanMDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanMDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "Mode.h"
#include "ModeManager.h"
#include "PurposeInfoCollection.h"

// KDevPlanMDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanMDlg, KDialogEx)

KDevPlanMDlg::KDevPlanMDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanMDlg::IDD, pParent)
{

}

KDevPlanMDlg::~KDevPlanMDlg()
{
}

void KDevPlanMDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}


BEGIN_MESSAGE_MAP(KDevPlanMDlg, KDialogEx)
END_MESSAGE_MAP()

// KDevPlanMDlg 메시지 처리기입니다.

BOOL KDevPlanMDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	GetRate();
	InitReportCtrl();
	UpdateReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDevPlanMDlg::SetTarget( KTarget* pTarget )
{
	TxLogDebugStartMsg();
	m_pTarget = pTarget;
	TxLogDebugEndMsg();
}


void KDevPlanMDlg::InitReportCtrl( void )
{
	// Report Header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
// 	m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Mode / Purpose"), 150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Rate"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);

	// Report control 설정
	m_wndReport.AllowEdit(TRUE);
	m_wndReport.EditOnClick(TRUE);
	m_wndReport.SetMultipleSelection(FALSE);
	m_wndReport.SetTreeIndent(10);
	m_wndReport.SelectionEnable(FALSE);
	m_wndReport.Populate();
}


void KDevPlanMDlg::UpdateReportCtrl( void )
{
	m_wndReport.ResetContent(TRUE);
	CXTPReportColumns* pColumns = m_wndReport.GetColumns();

	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iterP = purposeInfos.begin();
	std::list<KPurpose*>::iterator endP  = purposeInfos.end();
	while(iterP != endP)
	{
		KPurpose* pPurpose = *iterP;
		CXTPReportRecord* pInfoRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pInfoItem = pInfoRecord->AddItem(new CXTPReportRecordItemText);
		pInfoItem->SetCaption(pPurpose->PurposeName());
		pInfoItem->SetItemData((DWORD_PTR)pPurpose);
		pInfoItem->SetBold(TRUE);
		pInfoItem->SetBackgroundColor(RGB(191,239,254));

		pInfoItem = pInfoRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
		pInfoItem->SetBackgroundColor(RGB(191,239,254));
		pInfoItem->SetEditable(FALSE);

		KModeManager* pModeManager = m_pTarget->ModeManager();
		int nModeCount = pModeManager->GetModeCount();
		for(int i = 0; i < nModeCount; ++i)
		{
			KMode* pMode = pModeManager->GetMode(i);
			CXTPReportRecord* pPARecord = pInfoRecord->GetChilds()->Add(new CXTPReportRecord);
			CXTPReportRecordItem* pPARecordItem = pPARecord->AddItem(new CXTPReportRecordItemText);
			pPARecordItem->SetCaption(pMode->ModeName());

			double dRate(0.0);
			if (i < (int)m_vecRate.size())
			{
				dRate = m_vecRate[i];
			}

			pPARecordItem = pPARecord->AddItem(new CXTPReportRecordItemNumber(dRate, _T("%.1f")));
		
		}

		++iterP;
	}
	
	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}

void KDevPlanMDlg::GetRate()
{
	m_vecRate.clear();
	m_vecRate.push_back(20);
	m_vecRate.push_back(30);
	m_vecRate.push_back(15);
	m_vecRate.push_back(15);
	m_vecRate.push_back(20);
}

void KDevPlanMDlg::GetValue()
{
	std::vector<double> vecValue;
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*   pRecord = pRecords->GetAt(i);	// 목적 이름
		CXTPReportRecords*  pChilds = pRecord->GetChilds();
		int nChildCount =   pChilds->GetCount();
		for(int j = 0; j < nChildCount; ++j)
		{
			CXTPReportRecord* pRecPurpose = pChilds->GetAt(j);
			CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecPurpose->GetItem(1));
			vecValue.push_back(pNumber->GetValue());
		}
	}

	CString strMassage = _T("");
	size_t nxCount = vecValue.size();
	for (size_t i=0; i<nxCount; i++)
	{
		double dvalue = vecValue[i];
		CString strValue = _T("");
		strValue.Format(_T("%f, "), dvalue);
		strMassage += strValue;
	}
	AfxMessageBox(strMassage);
}
