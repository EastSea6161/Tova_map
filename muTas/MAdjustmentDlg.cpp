/**
 * @file MAdjustmentDlg.cpp
 * @brief KMAdjustmentDlg 구현 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "MAdjustmentDlg.h"
#include <queue>
#include "afxdialogex.h"
#include "CustomReportRecordItem.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "MAdjustmentDummy.h"
#include "ModeChoiceInfo.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "Purpose.h"
#include "Target.h"


/* Record column index define */
#define MADJUSTMENT_NAME		0
#define MADJUSTMENT_FIXED		1
#define MADJUSTMENT_DUMMY		2


///* Table name define */
//const TCHAR* TABLE_PARAMETER_OD	= _T("parameter_od");


// KMAdjustmentDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KMAdjustmentDlg, KDialogEx)

KMAdjustmentDlg::KMAdjustmentDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KMAdjustmentDlg::IDD, pParent)
	, m_bApplyDummy(false)
{

}


KMAdjustmentDlg::~KMAdjustmentDlg()
{
	std::map<KModeChoiceInfo*, KMAdjustmentDummy*>::iterator itDummy, itEnd = m_DummyMap.end();
	for(itDummy = m_DummyMap.begin(); itDummy != itEnd; ++itDummy)
	{
		KMAdjustmentDummy* pDummy = itDummy->second;
		delete pDummy;
	}
	m_DummyMap.clear();
}


void KMAdjustmentDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KMAdjustmentDlg, KDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &KMAdjustmentDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KMAdjustmentDlg::OnBnClickedRadio2)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
END_MESSAGE_MAP()



void KMAdjustmentDlg::AddModeChoiceInfo(KModeChoiceInfo* pInfo)
{
	std::map<KModeChoiceInfo*, KMAdjustmentDummy*>::iterator itDummy = m_DummyMap.find(pInfo);
	if(m_DummyMap.end() == itDummy)
	{
		KMAdjustmentDummy* pDummy = new KMAdjustmentDummy;

		KMAdjustmentDummy* pOriginDummy = pInfo->AdjustmentDummy();
		if(NULL != pOriginDummy)
		{
			*pDummy = *pOriginDummy;
		}
		
		m_DummyMap.insert(std::make_pair(pInfo, pDummy));
	}
}


bool KMAdjustmentDlg::ApplyDummy(void)
{
	return m_bApplyDummy;
}


void KMAdjustmentDlg::ApplyDummy(bool bApply)
{
	m_bApplyDummy = bApply;
}


void KMAdjustmentDlg::Target(const KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KMAdjustmentDlg::initReportControl(void)
{
	// report header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;

// 	m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MADJUSTMENT_NAME, _T("Purpose/Mode"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MADJUSTMENT_FIXED, _T("Fixed"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MADJUSTMENT_DUMMY, _T("Adjustment Dummy"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;


	// Report control 설정
	//m_wndReport.AllowEdit(FALSE);
	//m_wndReport.EditOnClick(FALSE);
	m_wndReport.AllowEdit(TRUE);
	m_wndReport.EditOnClick(TRUE);
	m_wndReport.SetMultipleSelection(FALSE);
	m_wndReport.SetTreeIndent(10);
}


void KMAdjustmentDlg::initDummyConstraint(void)
{
	CXTPReportColumn* pColumn = m_wndReport.GetColumns()->GetAt(MADJUSTMENT_DUMMY);
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
	pConstraints->RemoveAll();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	if(NULL == pTable)
	{
		return;
	}

	const KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if((pColumn->ColumnType() != KEMIOColumnTypeSystem) &&
			(pColumn->DataType() == KEMIODataTypeDouble))
		{
			pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
		}
	}
}


void KMAdjustmentDlg::updateReportRecord(void)
{
	std::map<KModeChoiceInfo*, KMAdjustmentDummy*>::iterator itDummy, itEnd = m_DummyMap.end();
	for(itDummy = m_DummyMap.begin(); itDummy != itEnd; ++itDummy)
	{
		KModeChoiceInfo* pMInfo = itDummy->first;
		KPurpose* pPurpose = pMInfo->Purpose();
		KMAdjustmentDummy* pDummy = itDummy->second;

		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pPurpose->PurposeName());
		pItem->SetItemData((DWORD_PTR)pMInfo);
		updateReportRecordMode(pRecord, pDummy);
	}

	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}


void KMAdjustmentDlg::updateReportRecordMode(CXTPReportRecord* pRoot, KMAdjustmentDummy* pDummy)
{
	CXTPReportColumn* pColumn = m_wndReport.GetColumns()->GetAt(MADJUSTMENT_DUMMY);
	CXTPReportRecordItemConstraint* pDefaultConstraint = pColumn->GetEditOptions()->GetConstraints()->GetAt(0);
	DWORD_PTR dwDefault = pDefaultConstraint->m_dwData;
	CString strDefault = pDefaultConstraint->m_strConstraint;

	CXTPReportRecordItem* pRootItem = pRoot->GetItem(MADJUSTMENT_NAME);

	KModeChoiceInfo* pInfo = (KModeChoiceInfo*)(pRootItem->GetItemData());
	ASSERT(NULL != pInfo);
	
	std::queue<KModeTreeNodeMode*> nodeQ;
	std::queue<CXTPReportRecord*> recordQ;
	KModeTree* pModeTree = pInfo->ModeTree();
	int nModeCount = pModeTree->ModeNodeCount();
	for(int i = 0; i < nModeCount; ++i)
	{
		KModeTreeNodeMode* pNode = (KModeTreeNodeMode*)(pModeTree->ModeNode(i));
		nodeQ.push(pNode);
		recordQ.push(pRoot);
	}

	while(nodeQ.empty() == false)
	{
		KModeTreeNodeMode* pNode = nodeQ.front();
		nodeQ.pop();
		CXTPReportRecord* pParentRecord = recordQ.front();
		recordQ.pop();

		CXTPReportRecord* pRecord = pParentRecord->GetChilds()->Add(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pNode->ModeName());
		pItem->SetItemData(pNode->ModeID());
		if(pNode->HasChild() == true)
		{
			int nSubNodeCount = pNode->NodeCount();
			for(int j = 0; j < nSubNodeCount; ++j)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(j));
				nodeQ.push(pSubNode);
				recordQ.push(pRecord);
			}
		}
		else
		{
			bool bChecked = false;
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->HasCheckbox(TRUE);
			if(pDummy->Fixed(pNode->ModeID()) == true)
			{
				pItem->SetChecked(TRUE);
				bChecked = true;
			}

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			if(true == bChecked)
			{
				pItem->SetEditable(FALSE);
				pItem->SetTextColor(RGB(150, 150, 150));
			}
			else
			{
				CString strColumnName = pDummy->DummyField(pNode->ModeID());
				CString strColumnCaption = getFieldCaption(strColumnName);
				CXTPReportRecordItemConstraint* pConst = 
					pColumn->GetEditOptions()->FindConstraint(strColumnCaption);
				if(NULL == pConst)
				{
					pItem->SetCaption(strDefault);
					pItem->SetItemData(dwDefault);
				}
				else
				{
					pItem->SetCaption(pConst->m_strConstraint);
					pItem->SetItemData(pConst->m_dwData);
				}
			}
		}
	}
}


void KMAdjustmentDlg::applyAdjustmentDummy(CXTPReportRecord* pRoot)
{
	CXTPReportRecordItem* pRootItem = pRoot->GetItem(MADJUSTMENT_NAME);
	KModeChoiceInfo* pInfo = (KModeChoiceInfo*)(pRootItem->GetItemData());
	std::map<KModeChoiceInfo*, KMAdjustmentDummy*>::iterator itDummy = m_DummyMap.find(pInfo);
	ASSERT(m_DummyMap.end() != itDummy);
	KMAdjustmentDummy* pDummy = itDummy->second;
	pDummy->RemoveAllDummy();

	std::queue<CXTPReportRecord*> recordQ;
	CXTPReportRecords* pRootRecords = pRoot->GetChilds();
	int nRecordCount = pRootRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRootRecords->GetAt(i);
		recordQ.push(pRecord);
	}

	while(recordQ.empty() == false)
	{
		CXTPReportRecord* pRecord = recordQ.front();
		recordQ.pop();

		CXTPReportRecords* pSubRecords = pRecord->GetChilds();
		int nSubRecordCount = pSubRecords->GetCount();
		if(0 < nSubRecordCount)
		{
			for(int j = 0; j < nSubRecordCount; ++j)
			{
				CXTPReportRecord* pSubRecord = pSubRecords->GetAt(j);
				recordQ.push(pSubRecord);
			}
		}
		else
		{
			CXTPReportRecordItem* pItem = pRecord->GetItem(MADJUSTMENT_NAME);
			int nModeID = pItem->GetItemData();
			
			pItem = pRecord->GetItem(MADJUSTMENT_FIXED);
			if(pItem->IsChecked() == TRUE)
			{
				pDummy->Fixed(nModeID, true);
			}
			else
			{
				pItem = pRecord->GetItem(MADJUSTMENT_DUMMY);
				KIOColumn* pColumn = (KIOColumn*)(pItem->GetItemData());
				pDummy->DummyField(nModeID, pColumn->Name());
			}
		}
	}
}


CString KMAdjustmentDlg::getFieldCaption(LPCTSTR strName)
{
	CString strRet = _T("");

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	if(NULL != pTable)
	{
		const KIOColumns* pColumns = pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			CString strColumnName = pColumn->Name();
			if(strColumnName.Compare(strName) == 0)
			{
				strRet = pColumn->Caption();
				break;
			}
		}
	}

	return strRet;
}


// KMAdjustmentDlg 메시지 처리기입니다.
BOOL KMAdjustmentDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if(true == m_bApplyDummy)
	{
		CButton* pButton = (CButton*)(GetDlgItem(IDC_RADIO2));
		pButton->SetCheck(TRUE);
		m_wndReport.EnableWindow(TRUE);
	}
	else
	{
		CButton* pButton = (CButton*)(GetDlgItem(IDC_RADIO1));
		pButton->SetCheck(TRUE);
		m_wndReport.EnableWindow(FALSE);
	}

	initReportControl();
	initDummyConstraint();
	updateReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMAdjustmentDlg::OnOK()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		applyAdjustmentDummy(pRecord);
	}


	std::map<KModeChoiceInfo*, KMAdjustmentDummy*>::iterator itDummy, itEnd = m_DummyMap.end();
	for(itDummy = m_DummyMap.begin(); itDummy != itEnd; ++itDummy)
	{
		KModeChoiceInfo* pInfo = itDummy->first;
		KMAdjustmentDummy* pDummy = itDummy->second;
		pInfo->AdjustmentDummy(pDummy);
	}

	KDialogEx::OnOK();
}


void KMAdjustmentDlg::OnBnClickedRadio1()
{
	m_bApplyDummy = false;
	m_wndReport.EnableWindow(FALSE);
}


void KMAdjustmentDlg::OnBnClickedRadio2()
{
	m_bApplyDummy = true;
	m_wndReport.EnableWindow(TRUE);
}


void KMAdjustmentDlg::OnReportCheckItem(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	ASSERT(pItemNotify != NULL);

	CXTPReportRecord* pRecord = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pDummyItem = pRecord->GetItem(MADJUSTMENT_DUMMY);
	if(pItemNotify->pItem->IsChecked() == TRUE)
	{
		pDummyItem->SetEditable(FALSE);
		pDummyItem->SetTextColor(RGB(150, 150, 150));
	}
	else
	{
		pDummyItem->SetEditable(TRUE);
		pDummyItem->SetTextColor(RGB(0, 0, 0));
	}

	CXTPReportRecord* pParent = pRecord;
	while(pParent->GetParentRecord() != NULL)
	{
		pParent = pParent->GetParentRecord();
	}

	std::queue<CXTPReportRecord*> recordQ;
	CXTPReportRecords* pChilds = pParent->GetChilds();
	int nChildCount = pChilds->GetCount();
	for(int i = 0; i < nChildCount; ++i)
	{
		CXTPReportRecord* pChildRecord = pChilds->GetAt(i);
		if(pChildRecord == pRecord)
		{
			continue;
		}

		recordQ.push(pChildRecord);
	}

	while(recordQ.empty() == false)
	{
		CXTPReportRecord* pCurrent = recordQ.front();
		recordQ.pop();
		CXTPReportRecords* pRecords = pCurrent->GetChilds();
		int nSubRecordCount = pRecords->GetCount();
		if(0 < nSubRecordCount)
		{
			for(int j = 0; j < nSubRecordCount; ++j)
			{
				CXTPReportRecord* pSubRecord = pRecords->GetAt(j);
				if(pSubRecord == pRecord)
				{
					continue;
				}
				recordQ.push(pSubRecord);
			}
		}
		else
		{
			CXTPReportRecordItem* pCurrentItem = pCurrent->GetItem(MADJUSTMENT_FIXED);
			pCurrentItem->SetChecked(FALSE);
			pCurrentItem = pCurrent->GetItem(MADJUSTMENT_DUMMY);
			pCurrentItem->SetEditable(TRUE);
			pCurrentItem->SetTextColor(RGB(0, 0, 0));
		}
	}
}