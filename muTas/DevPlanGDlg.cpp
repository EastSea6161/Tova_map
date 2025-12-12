// DevPlanGDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanGDlg.h"
#include "afxdialogex.h"

#include "KExRecordItem.h"
#include "CustomReportRecordItem.h"

#include "DBaseDevPlan.h"

#include "Target.h"
#include "PurposeManager.h"
#include "PAPurpose.h"
#include "DevPlanGInfo.h"


//^#include "DBaseConnector.h"

#define UM_EXPRESSIONEDIT1		WM_USER + 1001
#define UM_EXPRESSIONEDIT2		WM_USER + 1002

// KDevPlanGDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanGDlg, KDialogEx)

KDevPlanGDlg::KDevPlanGDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanGDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_emType(KEMDevPlanAutomaticCalc)
{
	InitPurposeInfos();
}

KDevPlanGDlg::~KDevPlanGDlg()
{
	TxLogDebugVisitor();
	ClearPurposeInfos();
}

void KDevPlanGDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT2, m_wndAutoReport);
	DDX_Radio  (pDX, IDC_RADIO1,  (int&)m_emType);
}


BEGIN_MESSAGE_MAP(KDevPlanGDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KDevPlanGDlg::OnBnClickedCalculate)
	ON_MESSAGE(UM_EXPRESSIONEDIT1, OnExpressionEdit1)
	ON_MESSAGE(UM_EXPRESSIONEDIT2, OnExpressionEdit2)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT2, OnItemButtonClick)
END_MESSAGE_MAP()

// KDevPlanGDlg 메시지 처리기입니다.

void KDevPlanGDlg::ClearPurposeInfos( void )
{
	TxLogDebugStartMsg();
	std::list<KDevPlanGInfo*>::iterator iter, end = m_lstPurpose.end();

	for(iter = m_lstPurpose.begin(); iter != end; ++iter)
	{
		KDevPlanGInfo* pDevPlanGInfo = *iter;
		delete pDevPlanGInfo;
	}
	m_lstPurpose.clear();
	TxLogDebugEndMsg();
}

void KDevPlanGDlg::InitPurposeInfos( void )
{
	TxLogDebugStartMsg();
	ClearPurposeInfos();

	KPurposeManager* pPurposeMgr = m_pTarget->PurposeManager();
	ASSERT(NULL != pPurposeMgr);
	int nPurposeCount = pPurposeMgr->GetPurposeCount();

	for (int i = 0; i < nPurposeCount; ++i)
	{
		KDevPlanGInfo* pDevPlanGInfo = new KDevPlanGInfo;
		KPAPurpose*    pPAPurpose 	 = pPurposeMgr->GetPurpose(i);

		pDevPlanGInfo->Purpose(pPAPurpose);

		m_lstPurpose.push_back(pDevPlanGInfo);
	}
	TxLogDebugEndMsg();
}

BOOL KDevPlanGDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, _T("Residence"));

	m_wndAutoReport.GetColumns()->Clear();
	m_wndAutoReport.ResetContent(TRUE);

 	InitAutoReport();
 	UpdateAutoReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDevPlanGDlg::InitAutoReport( void )
{
	m_wndAutoReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndAutoReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndAutoReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndAutoReport.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndAutoReport.SetGroupRowsBold(TRUE);

	CXTPReportColumn* pCXTPColumn  = NULL;
	int 			  nColumnIndex = 0;

	CXTPReportRecordItemEditOptions* pEditOptions = NULL;
	CXTPReportRecordItemConstraints* pConstraints = NULL;

	//#0
	pCXTPColumn = m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T(""), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetVisible(FALSE);

	m_wndAutoReport.GetColumns()->GetGroupsOrder()->Add(pCXTPColumn);

	//#1
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Purpose"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);

	//#2
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("P/A"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);

	//#3
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("SED"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);

	pEditOptions = pCXTPColumn->GetEditOptions();
	pConstraints = pEditOptions->GetConstraints();

	pEditOptions->AddComboButton(TRUE);
	pEditOptions->m_bAllowEdit = TRUE;
	pEditOptions->m_bExpandOnSelect = TRUE;

	KIOTables*           pTables        = m_pTarget->Tables();
	KIOTable* 		           pSEDTable      = pTables->FindTable(TABLE_SED_VECTOR);
	const KIOColumns* pSEDColumns    = pSEDTable->Columns();
	int 			           nSEDColumnCnt  = pSEDColumns->ColumnCount();

	for (int i = 0; i < nSEDColumnCnt; ++i)
	{
		KIOColumn* pColumn = pSEDColumns->GetColumn(i);

		if (KEMIODataTypeDouble == pColumn->DataType())
		{
			pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
		}
	}
	//#4
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Facility"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetEditable(FALSE);

	//#5
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Plan"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetEditable(FALSE);

	//#6
	pCXTPColumn = 
		m_wndAutoReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Basic Unit"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_RIGHT);
}

void KDevPlanGDlg::UpdateAutoReport( void )
{
	CXTPReportRecord*     pCXTPRecord = NULL;
	CXTPReportRecordItem* pCXTPItem = NULL;
	int nRecordIndex(0);

	std::list<KDevPlanGInfo*>::iterator iter, end = m_lstPurpose.end();

	for (iter = m_lstPurpose.begin(); iter != end; ++iter)
	{
		KDevPlanGInfo* pDevPlanGInfo = *iter;
		KPAPurpose*    pPAPurpose    = pDevPlanGInfo->Purpose();

		pCXTPRecord = m_wndAutoReport.AddRecord(new CXTPReportRecord);
		//#0
		pCXTPItem   = pCXTPRecord->AddItem(
			new KExRecordItem2GroupTextNo(pPAPurpose->PurposeName(), nRecordIndex++));
		pCXTPItem->SetItemData((DWORD_PTR)pDevPlanGInfo);
		//#1
		pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText);

		if (KEMPATypeProduction == pPAPurpose->PAType())
		{
			pCXTPItem->SetCaption(_T("Production"));
		}
		else
		{
			pCXTPItem->SetCaption(_T("Attraction"));
		}
		//#2
		pCXTPItem = pCXTPRecord->AddItem(new KComboReportRecordItem);

		CXTPReportRecordItemEditOptions* pEditOptions = pCXTPItem->GetEditOptions(NULL);
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

		pEditOptions->AddComboButton(TRUE);
		pEditOptions->m_bAllowEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;
		pConstraints->RemoveAll();

		UpdatePACombo(pEditOptions, pPAPurpose);

		//#3
		pCXTPItem = pCXTPRecord->AddItem(new CCustomReportRecordItem);

		//#4
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);

		CXTPReportRecordItemControl* pCXTPButton = 
			pCXTPItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (NULL != pCXTPButton)
		{
			pCXTPButton->SetAlignment(xtpItemControlRight);
			pCXTPButton->SetCaption(_T("..."));
			pCXTPButton->SetSize(CSize(22,0));
		}

		//#5
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPButton = pCXTPItem->
			GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (NULL != pCXTPButton)
		{
			pCXTPButton->SetAlignment(xtpItemControlRight);
			pCXTPButton->SetCaption(_T("..."));
			pCXTPButton->SetSize(CSize(22,0));
		}

		//#6
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.5f")));
	}

	m_wndAutoReport.AllowEdit(TRUE);
	m_wndAutoReport.EditOnClick(TRUE);
	m_wndAutoReport.Populate();
}


void KDevPlanGDlg::UpdatePACombo( 
	CXTPReportRecordItemEditOptions* a_pEditOptions, KPAPurpose* a_pPAPurpose )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pZoneTable = pIOTables->FindTable(TABLE_ZONE);
	ASSERT(NULL != pZoneTable);
	const KIOColumns* pColumns = pZoneTable->Columns();

	CString strQuery;
	strQuery.Format(
		_T(" SELECT zone_column_name, start_date, end_date, generation_run_seq")
		_T(" FROM generation_model")
		_T(" WHERE object_group_code = %d")
		_T(" AND detail_object_id = %d")
		_T(" AND pa_flag = '%c' ")
		_T(" ORDER BY end_date DESC"),
		a_pPAPurpose->PurposeGroup(),
		a_pPAPurpose->PurposeID(),
		a_pPAPurpose->PAType() == KEMPATypeProduction ? 'p' : 'a'
		);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);
	int nn = 0;
	while (spResultSet->Next())
	{
		CString    strColumn = spResultSet->GetValueString(0);
		KIOColumn* pColumn   = pColumns->GetColumn(strColumn);

		if(NULL != pColumn)
		{
			a_pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
		}
	}
}


void KDevPlanGDlg::OnBnClickedCalculate()
{
	CXTPReportRecords*    pCXTPRecords = m_wndAutoReport.GetRecords();
	int 			      nRecordCount = pCXTPRecords->GetCount();
	CXTPReportRecord*     pCXTPRecord  = NULL;
	CXTPReportRecordItem* pCXTPItem    = NULL;

	for (int i = 0; i < nRecordCount; ++i)
	{
		pCXTPRecord = pCXTPRecords->GetAt(i);

		pCXTPItem = pCXTPRecord->GetItem(2);
		KIOColumn* pPAColumn = (KIOColumn*)pCXTPItem->GetItemData();

		pCXTPItem = pCXTPRecord->GetItem(3);
		KIOColumn* pSEDColumn = (KIOColumn*)pCXTPItem->GetItemData();

		double dBasicUnit = KDBaseDevPlan::CalcBasicUnit(m_pTarget, pPAColumn, pSEDColumn);

		//Update BasicUnit
		CXTPReportRecordItemNumber* 
			pCXTPNumber = (CXTPReportRecordItemNumber*)pCXTPRecord->GetItem(6);
		pCXTPNumber->SetValue(dBasicUnit);
	}
}

void KDevPlanGDlg::OnItemButtonClick( NMHDR * a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem   = NULL;

	int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

	if (4 == nColumnIndex)
	{
		pItem = pRecord->GetItem(4);
		PostMessage(UM_EXPRESSIONEDIT1, (WPARAM)pItem);
	}
	else if (5 == nColumnIndex)
	{
		pItem = pRecord->GetItem(5);
		PostMessage(UM_EXPRESSIONEDIT2, (WPARAM)pItem);
	}
}

LRESULT KDevPlanGDlg::OnExpressionEdit1( WPARAM a_wParam, LPARAM a_lParam )
{
	CXTPReportColumn *pColum = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColum);

	return 0;
}

LRESULT KDevPlanGDlg::OnExpressionEdit2( WPARAM a_wParam, LPARAM a_lParam )
{
	CXTPReportColumn *pColum = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColum);

	return 0;
}
