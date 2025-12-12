// InterInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterInputDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "Purpose.h"
#include "PAPurpose.h"
#include "InterInputInfo.h"

#include "IncludeIOTable.h"
#include "DBaseConnection.h"

/*Define purpose report column index*/
#define INTER_PURPOSE_TYPE		0
#define INTER_PURPOSE_APPLY		1

#define UM_EXPRESSIONEDIT		WM_USER + 1001

// KInterInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterInputDlg, CDialogEx)

KInterInputDlg::KInterInputDlg(KTarget* a_pTarget, std::list<KInterInputInfo*>& a_lstInputInfo, 
	CWnd* pParent /*=NULL*/)
	:	CDialogEx(KInterInputDlg::IDD, pParent),
		m_pTarget(a_pTarget),
		m_lstInputInfo(a_lstInputInfo)
{

}

KInterInputDlg::~KInterInputDlg()
{
}

void KInterInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndPurposeReport);
	DDX_Control(pDX, IDC_REPORT4, m_wndInputReport);
}


BEGIN_MESSAGE_MAP(KInterInputDlg, CDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT1, OnReportCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT4, OnInputCheckItem)
	ON_BN_CLICKED(IDOK, &KInterInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KInterInputDlg 메시지 처리기입니다.


BOOL KInterInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitPurposeReport();
	UpdatePurposeReport();

	InitInputReport();
	UpdateInputReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterInputDlg::InitPurposeReport()
{
	/*Setting Report Header*/
	m_wndPurposeReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndPurposeReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndPurposeReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndPurposeReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	/*Setting Report tree*/
	m_wndPurposeReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndPurposeReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	/*Add columns*/
	CXTPReportColumn *pReportColumn = NULL;

	pReportColumn = m_wndPurposeReport.AddColumn(
		new CXTPReportColumn(INTER_PURPOSE_TYPE, _T("Purpose"), 150));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	
	pReportColumn = m_wndPurposeReport.AddColumn(
		new CXTPReportColumn(INTER_PURPOSE_APPLY, _T("Select"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(xtpColumnIconCenter);

	/*Setting Report Control*/
	m_wndPurposeReport.AllowEdit(TRUE);
	m_wndPurposeReport.EditOnClick(TRUE);
	m_wndPurposeReport.SetMultipleSelection(FALSE);
}


void KInterInputDlg::UpdatePurposeReport()
{
	/*update purpose record*/
	std::list<KInterInputInfo*>::iterator itInput, itEnd = m_lstInputInfo.end();
	
	for(itInput = m_lstInputInfo.begin();itInput != itEnd; itInput++)
	{
		KInterInputInfo* pInputInfo = *itInput;
		
		KPurpose* pPurpose = pInputInfo->GetPurpose();

		CXTPReportRecord* pReportRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord);

		/*Purpose*/
		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetCaption(pPurpose->PurposeName());
		pRecordItem->SetItemData((DWORD_PTR)pInputInfo);
		
		pRecordItem->SetAlignment(DT_CENTER);

		/*Apply*/
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->HasCheckbox(TRUE);
		
		if( pInputInfo->GetSelected() )
		{
			pRecordItem->SetChecked(TRUE);
		}
	}
		
	m_wndPurposeReport.Populate();
	m_wndPurposeReport.ExpandAll(TRUE);
}

void KInterInputDlg::OnBnClickedOk()
{	
	try
	{
		ApplyPurposeInfos();
		ApplyInputInfos();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebugWarning();
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("Not Known Error..."));
		return;
	}
		
	CDialogEx::OnOK();
}

void KInterInputDlg::ApplyPurposeInfos()
{
	bool bCheckNotAll = false;

	CXTPReportRecords* pRerocds = m_wndPurposeReport.GetRecords();
	int nRecordCount = pRerocds->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* 	  pRecord 	  = pRerocds->GetAt(i);
		CXTPReportRecordItem* pRecordItem = pRecord->GetItem(0);
		KInterInputInfo* 	  pInputInfo  = (KInterInputInfo*)pRecordItem->GetItemData();
		
		pRecordItem 		  			  = pRecord->GetItem(1);
		
		if (TRUE == pRecordItem->IsChecked())
		{
			pInputInfo->SetSelected(true);
			bCheckNotAll = true;
		}
		else
		{
			pInputInfo->SetSelected(false);
		}
	}

	if (!bCheckNotAll)
		ThrowException(_T("항목을 선택해 주세요."));
}

void KInterInputDlg::ApplyInputInfos( void )
{
	CXTPReportRecords* pRecords 	= m_wndInputReport.GetRecords();
	int 			   nRecordCount = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* 	  pRecord 	     = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   	     = pRecord->GetItem(0);
		KInterInputInfo* 	  pInputInfo     = (KInterInputInfo*)pItem->GetItemData();
		CXTPReportRecords*    pColumnRecords = pRecord->GetChilds();
		int                   nColumnCount   = pColumnRecords->GetCount();

		for (int j = 0; j < nColumnCount; ++j)
		{
			CXTPReportRecord* pColumnRecord = pColumnRecords->GetAt(j);
			CXTPReportRecordItem* pColumnItem = pColumnRecord->GetItem(0);
			if(pColumnItem->IsChecked() == TRUE)
			{
				KIOColumn* pColumn = (KIOColumn*)(pColumnItem->GetItemData());
				pInputInfo->InputColumn(pColumn);
				break;
			}
		}
	}
}

void KInterInputDlg::InitInputReport( void )
{
	// Report Header 설정
	m_wndInputReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndInputReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndInputReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndInputReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndInputReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndInputReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = m_wndInputReport.AddColumn(new CXTPReportColumn(0, _T("Purpose"), 1));
	pReportColumn->SetTreeColumn(TRUE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	pReportColumn = m_wndInputReport.AddColumn(new CXTPReportColumn(1, _T("Created Date"), 1));
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	pReportColumn = m_wndInputReport.AddColumn(new CXTPReportColumn(2, _T("Modified Date"), 1));
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	m_wndInputReport.AllowEdit(FALSE);
	m_wndInputReport.EditOnClick(FALSE);
	m_wndInputReport.SetMultipleSelection(FALSE);
	m_wndInputReport.SetTreeIndent(10);

}

void KInterInputDlg::UpdateInputReport( void )
{
	m_wndInputReport.GetRecords()->RemoveAll();

	std::list<KInterInputInfo*>::iterator iter, end = m_lstInputInfo.end();

	for (iter = m_lstInputInfo.begin(); iter != end; ++iter)
	{
		KInterInputInfo* pInputInfo = *iter;
		KPurpose* 		 pPurpose 	= pInputInfo->GetPurpose();

		if (!pInputInfo->GetSelected())
			continue;
		
		CXTPReportRecord* 	  pCXTPRecord = m_wndInputReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		
		pCXTPItem->SetCaption(pPurpose->PurposeName());
		pCXTPItem->SetItemData((DWORD_PTR)pInputInfo);
		
		UpdateInputRecord(pCXTPRecord, pPurpose);
	}

	m_wndInputReport.Populate();
	m_wndInputReport.ExpandAll();
}

void KInterInputDlg::UpdateInputRecord( CXTPReportRecord* a_pParent, KPurpose* a_pPurpose )
{
	bool bCheck = false;

	KIOTableManager*           pTableMgr  = m_pTarget->Tables();
	KIOTable*                  pTable     = pTableMgr->FindTable(TABLE_PURPOSE_OD);
	const KIOColumnCollection* pColumns   = pTable->Columns();

	CXTPReportRecordItem*      pItem      = a_pParent->GetItem(0);
	KInterInputInfo*           pInputInfo = (KInterInputInfo*)pItem->GetItemData();

	CString strQuery;
	strQuery.Format(
		_T(" SELECT purpose_od_column_name, start_date, end_date FROM distribution_model")
		_T(" WHERE object_group_code = %d AND detail_object_id = %d")
		_T(" ORDER BY end_date DESC"),
		a_pPurpose->PurposeGroup(), a_pPurpose->PurposeID()
		);

	KDBaseConnection oDBaseConnection(m_pTarget->GetDB());
	KResultSetPtr oResultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
	
	while (oResultSetPtr->Next())
	{
		CXTPReportRecord* 	  pCXTPRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem   = NULL;

		CString strTemp = oResultSetPtr->GetValueString(0);

		KIOColumn* pColumn = pColumns->GetColumn(oResultSetPtr->GetValueString(0));
		pCXTPItem 		   = pCXTPRecord->AddItem(new CXTPReportRecordItemText);

		ASSERT(NULL != pColumn);

		pCXTPItem->SetCaption(pColumn->Caption());
		pCXTPItem->SetItemData((DWORD_PTR)pColumn);
				
		pCXTPItem->HasCheckbox(TRUE);
		
		if (pInputInfo->InputColumn() == pColumn)
		{
			pCXTPItem->SetChecked(TRUE);
			bCheck = true;
		}

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(oResultSetPtr->GetValueString(1));

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(oResultSetPtr->GetValueString(2));
	}

	if ((false == bCheck) && (a_pParent->GetChilds()->GetCount() > 0))
	{
		CXTPReportRecord* pRecord = a_pParent->GetChilds()->GetAt(0);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(TRUE);
	}
}

void KInterInputDlg::OnReportCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify 	 = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
	CXTPReportRecord* 		 pRecord 	 	 = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* 	 pItem 		 	 = pRecord->GetItem(0);
	KInterInputInfo* 		 pInterInputInfo = (KInterInputInfo*)pItem->GetItemData();

	if (TRUE == pItemNotify->pItem->IsChecked())
	{
		pInterInputInfo->SetSelected(true);
	}
	else
	{
		pInterInputInfo->SetSelected(false);
		pInterInputInfo->InputColumn(NULL);
	}
	UpdateInputReport();
}

void KInterInputDlg::OnInputCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*)a_pNotifyStruct;
	pItemNotify->pItem->SetChecked(TRUE);

	CXTPReportRecord*     pCurrentRecord = pItemNotify->pItem->GetRecord();
	CXTPReportRecord*     pParentRecord  = pCurrentRecord->GetParentRecord();
// 	CXTPReportRecordItem* pPurposeItem   = pParentRecord->GetItem(0);
// 	KInterInputInfo*      pInputInfo     = (KInterInputInfo*)pPurposeItem->GetItemData();
// 
// 	CXTPReportRecordItem* pCurrentItem	 = pCurrentRecord->GetItem(0);
// 	KIOColumn*            pCurrentInput	 = (KIOColumn*)pCurrentItem->GetItemData();
// 	m_ColumnMap[pInputInfo] = pCurrentInput;

	CXTPReportRecords* pInputRecords = pParentRecord->GetChilds();
	int nInputCount = pInputRecords->GetCount();

	for(int i = 0; i < nInputCount; ++i)
	{
		CXTPReportRecord* pInputRecord = pInputRecords->GetAt(i);

		if(pInputRecord != pCurrentRecord)
		{
			CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(0);
			pInputItem->SetChecked(FALSE);
		}
	}
}
