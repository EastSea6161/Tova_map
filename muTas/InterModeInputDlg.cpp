// InterModeInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterModeInputDlg.h"
#include "afxdialogex.h"
#include "Mode.h"
#include "Target.h"
#include "IncludeIOTable.h"

#include "CustomReportRecordItem.h"

// KInterModeInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterModeInputDlg, CDialogEx)

KInterModeInputDlg::KInterModeInputDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: CDialogEx(KInterModeInputDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_dAnalysisTime(10.0)
{
}

KInterModeInputDlg::~KInterModeInputDlg()
{
}

void KInterModeInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1,   m_wndModeReport);
	DDX_Control(pDX, IDC_REPORT2,   m_wndInputRecord);
	DDX_Control(pDX, IDC_REPORT7,   m_wndIntermodalReport);
	DDX_Control(pDX, IDC_EDIT_TIME, m_edtAnalysisTime);
}


BEGIN_MESSAGE_MAP(KInterModeInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KInterModeInputDlg::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT1, OnModeCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, OnReportValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT2, OnColumnCheckItem)
END_MESSAGE_MAP()

double KInterModeInputDlg::AnalysisTime( void )
{
	return m_dAnalysisTime;
}

void KInterModeInputDlg::AnalysisTime( double a_dAnalysisTime )
{
	m_dAnalysisTime = a_dAnalysisTime;
}

void KInterModeInputDlg::IntermodalModeInfo( 
	std::vector<TIntermodalMode> a_vecIntermodalModeInfo )
{
	m_vecIntermodalModeInfo = a_vecIntermodalModeInfo;
}

std::vector<TIntermodalMode> KInterModeInputDlg::IntermodalModeInfo( void )
{
	return m_vecIntermodalModeInfo;
}

BOOL KInterModeInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strValue;
	strValue.Format(_T("%f"), m_dAnalysisTime);
	m_edtAnalysisTime.SetWindowText(strValue);
	m_edtAnalysisTime.EnableRealNumber(true);
	m_edtAnalysisTime.EnableMinMax(0.0, 24.0);
	
	InitModeReportControl();
	UpdateModeReportRecord();

	InitInputReportControl();
	UpdateInputRecordRecord();

	InitIntermodalReport();
	UpdateIntermodalReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterModeInputDlg::InitModeReportControl( void )
{
	// Report Header 설정
	m_wndModeReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndModeReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndModeReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndModeReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndModeReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndModeReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(0, _T("Mode"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(1, _T("Occupancy/Ton"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(2, _T("PCU"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(3, _T("Network(Type)"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    pColumn->GetEditOptions()->AddComboButton(TRUE);
    pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

    CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
    CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
    pEditOptions->AddConstraint(_T("Highway"), 1);
    pEditOptions->AddConstraint(_T("Transit Line"), 2);
    pEditOptions->AddConstraint(_T("Rail"), 3);
    
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(4, _T("PreLoad"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	m_wndModeReport.AllowEdit(TRUE);
	m_wndModeReport.EditOnClick(TRUE);
	m_wndModeReport.SetMultipleSelection(FALSE);
}

void KInterModeInputDlg::UpdateModeReportRecord( void )
{
	std::list<KInterModeInputInfo*>::iterator itInput, itEnd = m_modeInputList.end();
	for(itInput = m_modeInputList.begin(); itInput != itEnd; ++itInput)
	{
		KInterModeInputInfo* pInput = *itInput;
		KMode* 				 pMode 	= pInput->Mode();

		CXTPReportRecord* pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);
		// Mode 
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pMode->ModeName());
		pItem->HasCheckbox(TRUE);
		if(pInput->Selected() == true)
		{
			pItem->SetChecked(TRUE);
		}
		pItem->SetItemData((DWORD_PTR)pInput);

		// Occupancy
		pItem = pRecord->AddItem(
			new CXTPReportRecordItemNumber(pInput->Occupancy(), _T("%.3f")));

		// PCU
		pItem = pRecord->AddItem(
			new CXTPReportRecordItemNumber(pInput->PCU(), _T("%.3f")));

		// NetworkType
		pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
		pItem->SetItemData(pInput->NetworkType());

		// PreLoad
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if(pInput->PreLoad() == true)
		{
			pItem->SetChecked(TRUE);
		}
	}
	m_wndModeReport.Populate();
	m_wndModeReport.ExpandAll(TRUE);
}

void KInterModeInputDlg::InitIntermodalReport()
{
	/*Setting Report Header*/
	m_wndIntermodalReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndIntermodalReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndIntermodalReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndIntermodalReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = 
		m_wndIntermodalReport.AddColumn(new CXTPReportColumn(0, _T("Mode"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	pCXTPColumn = 
		m_wndIntermodalReport.AddColumn(new CXTPReportColumn(1, _T("Network(Type)"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = FALSE;

	CXTPReportRecordItemEditOptions* pEditOptions = pCXTPColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
	pEditOptions->AddConstraint(_T("Highway"), 1);
	pEditOptions->AddConstraint(_T("Transit Line"), 2);
	pEditOptions->AddConstraint(_T("Rail"), 3);

	pCXTPColumn = 
		m_wndIntermodalReport.AddColumn(new CXTPReportColumn(2, _T("PCU"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_RIGHT);

	pCXTPColumn = 
		m_wndIntermodalReport.AddColumn(new CXTPReportColumn(3, _T("Occupancy/Ton"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_RIGHT);

	m_wndIntermodalReport.AllowEdit(TRUE);
	m_wndIntermodalReport.EditOnClick(TRUE);
	m_wndIntermodalReport.SetMultipleSelection(FALSE);
}

void KInterModeInputDlg::UpdateIntermodalReport()
{
	std::vector<TIntermodalMode>::iterator iter, end = m_vecIntermodalModeInfo.end();

	CXTPReportRecord* pCXTPRecord = NULL;

	for (iter = m_vecIntermodalModeInfo.begin(); iter != end; ++iter)
	{
		TIntermodalMode oTIntermodalMode = *iter;
		pCXTPRecord	= m_wndIntermodalReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);

		//Mode Name
		pCXTPItem->SetCaption(oTIntermodalMode.strModeName);

		//NetworkType
		pCXTPItem = pCXTPRecord->AddItem(new CCustomReportRecordItem);
		pCXTPItem->SetItemData(oTIntermodalMode.nNetworkType);

		//PCU
		pCXTPItem = pCXTPRecord->AddItem(
			new CXTPReportRecordItemNumber(oTIntermodalMode.dPCU, _T("%.3f")));

		//PCU
		pCXTPItem = pCXTPRecord->AddItem(
			new CXTPReportRecordItemNumber(oTIntermodalMode.dOccupancy, _T("%.3f")));
	}

	m_wndIntermodalReport.Populate();
}

void KInterModeInputDlg::OnBnClickedOk()
{
	ApplyIntermodalInfo();

	ApplyInputInfo();
	ApplyInputColumn();

	CString strNumber;
	m_edtAnalysisTime.GetWindowText(strNumber);
	m_dAnalysisTime = _ttof(strNumber);

	CDialogEx::OnOK();
}

void KInterModeInputDlg::ApplyIntermodalInfo()
{
	m_vecIntermodalModeInfo.clear();

	CXTPReportRecords* pCXTPRecords = m_wndIntermodalReport.GetRecords();
	int 			   nRecordCount = pCXTPRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		TIntermodalMode oTIntermodalMode;
		CXTPReportRecord* pCXTPRecord = pCXTPRecords->GetAt(i);

		CXTPReportRecordItem* pCXTPItem = pCXTPRecord->GetItem(0);
		oTIntermodalMode.strModeName = pCXTPItem->GetCaption(NULL);

		pCXTPItem = pCXTPRecord->GetItem(1);
		oTIntermodalMode.nNetworkType = pCXTPItem->GetItemData();

		CXTPReportRecordItemNumber* pCXTPNumber =
			(CXTPReportRecordItemNumber*)pCXTPRecord->GetItem(2);
		oTIntermodalMode.dPCU = pCXTPNumber->GetValue();

		pCXTPNumber = (CXTPReportRecordItemNumber*)pCXTPRecord->GetItem(3);
		oTIntermodalMode.dOccupancy = pCXTPNumber->GetValue();

		m_vecIntermodalModeInfo.push_back(oTIntermodalMode);
	}
}

void KInterModeInputDlg::ModeInputList( std::list<KInterModeInputInfo*>& a_inputList )
{
	m_modeInputList = a_inputList;

	m_ColumnMap.clear();
	std::list<KInterModeInputInfo*>::iterator itInput, itEnd = m_modeInputList.end();
	for(itInput = m_modeInputList.begin(); itInput != itEnd; ++itInput)
	{
		KInterModeInputInfo* pInput  = *itInput;
		KIOColumn*   		 pColumn = pInput->InputColumn();
		
		if (NULL != pColumn)
		{
			m_ColumnMap.insert(std::make_pair(pInput, pColumn));
		}
	}
}

void KInterModeInputDlg::InitInputReportControl( void )
{
	// Report Header 설정
	m_wndInputRecord.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndInputRecord.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndInputRecord.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndInputRecord.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndInputRecord.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndInputRecord.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndInputRecord.AddColumn(
		new CXTPReportColumn(0, _T("Mode"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_wndInputRecord.AddColumn(
		new CXTPReportColumn(1, _T("Created Date"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndInputRecord.AddColumn(
		new CXTPReportColumn(2, _T("Modified Date"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);

	// Report control 설정
	m_wndInputRecord.AllowEdit(FALSE);
	m_wndInputRecord.EditOnClick(FALSE);
	m_wndInputRecord.SetMultipleSelection(FALSE);
}

void KInterModeInputDlg::UpdateInputRecordRecord( void )
{
	m_wndInputRecord.GetRecords()->RemoveAll();

	CXTPReportRecords* pModeRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pModeRecords->GetCount();
	
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pModeRecord = pModeRecords->GetAt(i);
		CXTPReportRecordItem* pModeItem = pModeRecord->GetItem(0);
		
		if(pModeItem->IsChecked() == FALSE)
		{
			continue;
		}

		KInterModeInputInfo*  pInput 	= (KInterModeInputInfo*)(pModeItem->GetItemData());
		KMode* 				  pMode 	= pInput->Mode();
		CXTPReportRecord* 	  pRecord 	= m_wndInputRecord.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem 	= pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pMode->ModeName());
		pItem->SetItemData((DWORD_PTR)pInput);

		UpdateInputColumnRecord(pRecord, pInput);
	}

	m_wndInputRecord.Populate();
	m_wndInputRecord.ExpandAll(TRUE);
}

void KInterModeInputDlg::UpdateInputColumnRecord( 
	CXTPReportRecord* pParent, KInterModeInputInfo* pInput )
{
	sqlite3* pDB = m_pTarget->GetDB();
	sqlite3_stmt* pStmt;
	SqliteTail tail;
	bool bCheck = false;
	int nDBState = SQLITE_ERROR;
	CString strSQL = _T("");

	KMode* pMode = pInput->Mode();

	KIOTableManager* pTableManager = m_pTarget->Tables();
	ASSERT(NULL != pTableManager);

	KIOTable* pTable = pTableManager->FindTable(TABLE_MODE_OD);
	ASSERT(NULL != pTable);

	const KIOColumnCollection* pColumns = pTable->Columns();
	KIOColumn* pInputColumn = NULL;
	std::map<KInterModeInputInfo*, KIOColumn*>::iterator itInput = m_ColumnMap.find(pInput);
	if(m_ColumnMap.end() != itInput)
	{
		pInputColumn = itInput->second;
	}

	strSQL.Format(
		_T(" SELECT mode_od_column_name, start_date, end_date ")
		_T(" FROM modechoice_model ")
		_T(" WHERE mode_group_code = %d ")
		_T(" AND detail_mode_id = %d ")
		_T(" ORDER BY end_date DESC "),
		pMode->ModeGroup(),
		pMode->ModeID()
		);

	try
	{
		if(SqlitePrepare(pDB, strSQL, -1, &pStmt, &tail) == SQLITE_ERROR)
		{
			throw sqlite3_errcode(pDB);
		}

		CXTPReportRecords* pRecords = pParent->GetChilds();
		nDBState = SqliteStep(pStmt);
		while(SQLITE_ROW == nDBState)
		{
			CXTPReportRecord* pRecord = pRecords->Add(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = NULL;

			CString strColumn = (TCHAR*)(SqliteColumnText(pStmt, 0));
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			KIOColumn* pColumn = pColumns->GetColumn(strColumn);
			if(NULL != pColumn)
			{
				pItem->SetCaption(pColumn->Caption());
				pItem->SetItemData((DWORD_PTR)pColumn);
			}
			else
			{
				pItem->SetCaption(strColumn);
			}

			pItem->HasCheckbox(TRUE);

			if((NULL != pInputColumn) && (pInputColumn == pColumn))
			{
				pItem->SetChecked(TRUE);
				bCheck = true;
			}

			CString strStartDate = (TCHAR*)(SqliteColumnText(pStmt, 1));
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strStartDate);

			CString strEndDate = (TCHAR*)(SqliteColumnText(pStmt, 2));
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strEndDate);

			nDBState = SqliteStep(pStmt);
		}
	}
	catch(...)
	{

	}
	SqliteFinalize(pStmt);

	if((false == bCheck) && (pParent->GetChilds()->GetCount() > 0))
	{
		CXTPReportRecord* pRecord = pParent->GetChilds()->GetAt(0);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(TRUE);
		KIOColumn* pColumn = (KIOColumn*)(pItem->GetItemData());
		m_ColumnMap[pInput] = pColumn;
	}
}

void KInterModeInputDlg::OnModeCheckItem( NMHDR* pNotifyStruct, LRESULT* result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CXTPReportRecord*     pRecord = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
	KInterModeInputInfo*  pInfo   = (KInterModeInputInfo*)(pItem->GetItemData());

	if(pItemNotify->pItem->IsChecked() == FALSE)
	{
		std::map<KInterModeInputInfo*, KIOColumn*>::iterator itColumn = m_ColumnMap.find(pInfo);
		if(m_ColumnMap.end() != itColumn)
		{
			m_ColumnMap.erase(itColumn);
		}
	}
	UpdateInputRecordRecord();
}

void KInterModeInputDlg::OnReportValueChanged( NMHDR* pNotifyStruct, LRESULT* result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CXTPReportRecord*     pRecord      = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pItem        = pRecord->GetItem(3);
	CXTPReportRecordItem* pItemPreLoad = pRecord->GetItem(4);

	int nCase = (int)pItem->GetItemData();
	switch (nCase)
	{
	case 1 :
		pItemPreLoad->SetChecked(FALSE);
		pItemPreLoad->SetEditable(TRUE);
		break;
	case 2 :
		pItemPreLoad->SetChecked(TRUE);
		pItemPreLoad->SetEditable(FALSE);
		break;
	case 3:
		pItemPreLoad->SetChecked(FALSE);
		pItemPreLoad->SetEditable(FALSE);
		break;
	default:
		break;
	}
}

void KInterModeInputDlg::OnColumnCheckItem( NMHDR* pNotifyStruct, LRESULT* result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	pItemNotify->pItem->SetChecked(TRUE);

	CXTPReportRecord*     pCurrentRecord = pItemNotify->pItem->GetRecord();
	CXTPReportRecord* 	  pModeRecord    = pCurrentRecord->GetParentRecord();
	CXTPReportRecordItem* pModeItem	     = pModeRecord->GetItem(0);
	KInterModeInputInfo*  pInput 	     = (KInterModeInputInfo*)(pModeItem->GetItemData());

	CXTPReportRecordItem* pCurrentItem   = pCurrentRecord->GetItem(0);
	KIOColumn*            pCurrentInput  = (KIOColumn*)(pCurrentItem->GetItemData());

	m_ColumnMap[pInput] = pCurrentInput;

	CXTPReportRecords* pInputRecords = pModeRecord->GetChilds();
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

void KInterModeInputDlg::ApplyInputInfo( void )
{
	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
		KInterModeInputInfo*  pInfo   = (KInterModeInputInfo*)(pItem->GetItemData());

		if(pItem->IsChecked() == FALSE)
		{
			pInfo->Selected(false);
			pInfo->InputColumn(NULL);
			continue;
		} 
        
		pInfo->Selected(true);

		CXTPReportRecordItemNumber* 
			pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(1));
		pInfo->Occupancy(pNumber->GetValue());

		pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(2));
		pInfo->PCU(pNumber->GetValue());

		pItem = pRecord->GetItem(3);
        pInfo->NetworkType(pItem->GetItemData());

		pItem = pRecord->GetItem(4);
		pInfo->PreLoad((pItem->IsChecked() == TRUE ? true : false));
	}
}

void KInterModeInputDlg::ApplyInputColumn( void )
{
	CXTPReportRecords* pModeRecords 	= m_wndInputRecord.GetRecords();
	int 			   nModeRecordCount = pModeRecords->GetCount();
	for(int i = 0; i < nModeRecordCount; ++i)
	{
		CXTPReportRecord*     pModeRecord = pModeRecords->GetAt(i);
		CXTPReportRecordItem* pModeItem   = pModeRecord->GetItem(0);
		KInterModeInputInfo*  pInput      = (KInterModeInputInfo*)(pModeItem->GetItemData());

		CXTPReportRecords* pColumnRecords = pModeRecord->GetChilds();

		int nColumnCount = pColumnRecords->GetCount();
		if (nColumnCount == 0)
		{
			pInput->Selected(false);
		}

		for(int j = 0; j < nColumnCount; ++j)
		{
			CXTPReportRecord* pColumnRecord = pColumnRecords->GetAt(j);
			CXTPReportRecordItem* pColumnItem = pColumnRecord->GetItem(0);
			if(pColumnItem->IsChecked() == TRUE)
			{
				KIOColumn* pColumn = (KIOColumn*)(pColumnItem->GetItemData());
				pInput->InputColumn(pColumn);
				break;
			}
		}
	}
}
