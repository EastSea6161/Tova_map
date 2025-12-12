// DevPlanDDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanDDlg.h"
#include "afxdialogex.h"

#include "KmzApp.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "CustomReportRecordItem.h"


// KDevPlanDDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanDDlg, KDialogEx)

KDevPlanDDlg::KDevPlanDDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanDDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_emDevplanDistributionMode(KEMDevplanDistributionModelFratar)
{
}

KDevPlanDDlg::~KDevPlanDDlg()
{
}

void KDevPlanDDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndTargetYearReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndTargetODReport);
	DDX_Control(pDX, IDC_REPORT4, m_wndBaseODReport);
	
	DDX_Radio  (pDX, IDC_RADIO1,  (int&)m_emDevplanDistributionMode);
}


BEGIN_MESSAGE_MAP(KDevPlanDDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
END_MESSAGE_MAP()

// KDevPlanGDlg 메시지 처리기입니다.

BOOL KDevPlanDDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitTargetYearReport();
	InitTargetODReport();

	UpdateTargetYearReport();
	UpdateTargetODReport();

	InitBaseODReport();
	UpdateBaseFieldConstraint();
	UpdateBaseODReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDevPlanDDlg::InitTargetYearReport( void )
{
	m_wndTargetYearReport.ShowHeader(FALSE);
	m_wndTargetYearReport
		.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;

	CXTPReportColumn* 
		pCXTPColumn = m_wndTargetYearReport.AddColumn(new CXTPReportColumn(0, _T("Target"), 1));

	pCXTPColumn->SetTreeColumn(TRUE);

	//Report Control 설정
	m_wndTargetYearReport.AllowEdit(FALSE);
	m_wndTargetYearReport.EditOnClick(FALSE);
	m_wndTargetYearReport.SetMultipleSelection(FALSE);
	m_wndTargetYearReport.SetTreeIndent(10);
}

void KDevPlanDDlg::UpdateTargetYearReport( void )
{
	CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
	KProject* pProject = pApp->GetProject();

	if(NULL == pProject)
	{
		return;
	}

	const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
	KTDScenarioMap::const_iterator itScenario, itEnd = scenarioMap.end();

	for(itScenario = scenarioMap.begin(); itScenario != itEnd; ++itScenario)
	{
		KScenario* pScenario = itScenario->second;
		if(pScenario->IsOpened() == false)
		{
			continue;
		}
		CXTPReportRecord*     pCXTPRecord;
		CXTPReportRecordItem* pCXTPItem;
		pCXTPRecord = m_wndTargetYearReport.AddRecord(new CXTPReportRecord);
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(pScenario->GetName());
		pCXTPItem->SetItemData((DWORD_PTR)pScenario);

		AddTreeRecord(pCXTPRecord, pScenario);
	}

	CXTPReportRecords* pCXTPRecords = m_wndTargetYearReport.GetRecords();
	CXTPReportRecord*  pCXTPRecord  = pCXTPRecords->GetAt(0);

	if(NULL != pCXTPRecord)
	{
		CXTPReportRecord* pCheckRecord = pCXTPRecord->GetChilds()->GetAt(0);
		if(NULL != pCheckRecord)
		{
			CXTPReportRecordItem* pCheckItem = pCheckRecord->GetItem(0);
			if(NULL != pCheckItem)
			{
				pCheckItem->SetChecked(TRUE);
				KTarget* pTarget = (KTarget*)(pCheckItem->GetItemData());
				UpdateFieldConstraint(pTarget);
			}
		}
	}

	m_wndTargetYearReport.Populate();
	m_wndTargetYearReport.ExpandAll(TRUE);
}

void KDevPlanDDlg::AddTreeRecord( CXTPReportRecord* a_pParnet, KScenario* a_pScenario )
{
	CXTPReportRecord*     pCXTPRecord = NULL;
	CXTPReportRecordItem* pCXTPItem = NULL;

	const KTDTargetMap& targetMap = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();
	for(itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
	{
		KTarget* pTarget = itTarget->second;
		if(pTarget->IsEmptyData() == true)
		{
			continue;
		}

		pCXTPRecord = a_pParnet->GetChilds()->Add(new CXTPReportRecord);
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(pTarget->GetName());
		pCXTPItem->SetItemData((DWORD_PTR)pTarget);
		pCXTPItem->HasCheckbox(TRUE);
	}
}

void KDevPlanDDlg::InitTargetODReport( void )
{
	// Report Header 설정
	m_wndTargetODReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndTargetODReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndTargetODReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndTargetODReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndTargetODReport.AddColumn(new CXTPReportColumn(0, _T("Purpose"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

	pColumn = m_wndTargetODReport.AddColumn(new CXTPReportColumn(1, _T("Field"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	// Report control 설정
	m_wndTargetODReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndTargetODReport.AllowEdit(TRUE);
	m_wndTargetODReport.FocusSubItems(TRUE);
	m_wndTargetODReport.Populate();
	//m_wndTargetODReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
}

void KDevPlanDDlg::UpdateTargetODReport( void )
{
	m_wndTargetODReport.ResetContent(TRUE);

	CXTPReportColumn* pCXTPColumn = m_wndTargetODReport.GetColumns()->GetAt(1);

	CXTPReportRecordItemConstraints* 
		pReportRecordItemConstraints = pCXTPColumn->GetEditOptions()->GetConstraints();

	if (pReportRecordItemConstraints->GetCount() < 1)
		return; 

	int nRecordCount = pReportRecordItemConstraints->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		if( 6 < i )
			continue;

		CXTPReportRecordItemConstraint* pConstraint = pReportRecordItemConstraints->GetAt(i);

		DWORD_PTR dwDefault = pConstraint->m_dwData;
		CString strDefault = pConstraint->m_strConstraint;

		CXTPReportRecord*  pCXTPRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		pCXTPRecord = m_wndTargetODReport.AddRecord(new CXTPReportRecord);
		pItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(strDefault);
		pItem->SetAlignment(DT_CENTER);

		pItem = pCXTPRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetCaption(strDefault);
		pItem->SetItemData(dwDefault);
	}
	
	m_wndTargetODReport.Populate();
}

void KDevPlanDDlg::UpdateFieldConstraint( KTarget* a_pTarget )
{
	ASSERT(NULL != a_pTarget);
	CXTPReportColumn*                pColumn      = m_wndTargetODReport.GetColumns()->GetAt(1);
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	pConstraints->RemoveAll();

	KIOTables* pIOTables = a_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pTable = pIOTables->FindTable(_T("purpose_od"));
	if(NULL == pTable)
	{
		return;
	}
	const KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		KEMIOColumnType emColumnType = pColumn->ColumnType();
		if((KEMIOColumnTypeFixed == emColumnType) || (KEMIOColumnTypeUserDefine == emColumnType))
		{
			pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
		}
	}
}

void KDevPlanDDlg::OnReportItemClick( NMHDR * pNotifyStruct, LRESULT * result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	if(pItemNotify->pColumn->GetItemIndex() == 0)
	{
		CXTPReportRecord* pCurrent = pItemNotify->pItem->GetRecord();
		if(pCurrent->HasChildren() == FALSE)
		{
			CXTPReportRecords* pParentRecords = m_wndTargetYearReport.GetRecords();
			int nParentCount = pParentRecords->GetCount();
			for(int i = 0; i < nParentCount; ++i)
			{
				CXTPReportRecord* pParentRecord = pParentRecords->GetAt(i);
				CXTPReportRecords* pRecords = pParentRecord->GetChilds();
				int nRecordCount = pRecords->GetCount();
				for(int j = 0; j < nRecordCount; ++j)
				{
					CXTPReportRecord* pRecord = pRecords->GetAt(j);
					CXTPReportRecordItem* pItem = pRecord->GetItem(0);
					pItem->SetChecked(FALSE);
				}
			}

			pItemNotify->pItem->SetChecked(TRUE);
			KTarget* pTarget = (KTarget*)(pItemNotify->pItem->GetItemData());
			UpdateFieldConstraint(pTarget);
			UpdateTargetODReport();
		}
	}
}

void KDevPlanDDlg::InitBaseODReport( void )
{
	// Report Header 설정
	m_wndBaseODReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndBaseODReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndBaseODReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndBaseODReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndBaseODReport.AddColumn(new CXTPReportColumn(0, _T("Purpose"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

	pColumn = m_wndBaseODReport.AddColumn(new CXTPReportColumn(1, _T("Field"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	// Report control 설정
	m_wndBaseODReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndBaseODReport.AllowEdit(TRUE);
	m_wndBaseODReport.FocusSubItems(TRUE);
	m_wndBaseODReport.Populate();
	//m_wndBaseODReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
}

void KDevPlanDDlg::UpdateBaseODReport( void )
{
	m_wndBaseODReport.ResetContent(TRUE);

	CXTPReportColumn* pCXTPColumn = m_wndBaseODReport.GetColumns()->GetAt(1);

	CXTPReportRecordItemConstraints* 
		pReportRecordItemConstraints = pCXTPColumn->GetEditOptions()->GetConstraints();

	if (pReportRecordItemConstraints->GetCount() < 1)
		return; 

	int nRecordCount = pReportRecordItemConstraints->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		if( 6 < i )
			continue;

		CXTPReportRecordItemConstraint* pConstraint = pReportRecordItemConstraints->GetAt(i);

		DWORD_PTR dwDefault = pConstraint->m_dwData;
		CString strDefault = pConstraint->m_strConstraint;

		CXTPReportRecord*  pCXTPRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		pCXTPRecord = m_wndBaseODReport.AddRecord(new CXTPReportRecord);
		pItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(strDefault);
		pItem->SetAlignment(DT_CENTER);

		pItem = pCXTPRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetCaption(strDefault);
		pItem->SetItemData(dwDefault);
	}

	m_wndBaseODReport.Populate();
}

void KDevPlanDDlg::UpdateBaseFieldConstraint()
{
	ASSERT(NULL != m_pTarget);
	CXTPReportColumn*                pColumn      = m_wndBaseODReport.GetColumns()->GetAt(1);
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	pConstraints->RemoveAll();

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pTable = pIOTables->FindTable(_T("purpose_od"));
	if(NULL == pTable)
	{
		return;
	}
	const KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		KEMIOColumnType emColumnType = pColumn->ColumnType();
		if((KEMIOColumnTypeFixed == emColumnType) || (KEMIOColumnTypeUserDefine == emColumnType))
		{
			pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
		}
	}
}
