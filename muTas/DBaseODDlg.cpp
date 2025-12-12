/**
 * @file DBaseODDlg.cpp
 * @brief KDBaseODDlg 구현 파일
 * @author 
 * @date 2011.05.20
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DBaseODDlg.h"
#include "afxdialogex.h"
#include "CustomReportRecordItem.h"
#include "DistributionInfo.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"




IMPLEMENT_DYNAMIC(KDBaseODDlg, KDialogEx)

KDBaseODDlg::KDBaseODDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDBaseODDlg::IDD, pParent)
{

}

KDBaseODDlg::~KDBaseODDlg()
{
}

void KDBaseODDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReportTree);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportPurpose);
}


BEGIN_MESSAGE_MAP(KDBaseODDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
END_MESSAGE_MAP()



void KDBaseODDlg::initReportControl(void)
{
	KReportCtrlSetting::Default(m_wndReportPurpose);
	initTreeReportControl();
	initPurposeReportControl();
}


void KDBaseODDlg::initTreeReportControl(void)
{
	// Report Header 설정
	m_wndReportTree.ShowHeader(FALSE);

	// Report tree 설정
	m_wndReportTree.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReportTree.AddColumn(new CXTPReportColumn(0, _T("Target"), 150));
	pColumn->SetTreeColumn(TRUE);

	// Report control 설정
	m_wndReportTree.AllowEdit(FALSE);
	m_wndReportTree.EditOnClick(FALSE);
	m_wndReportTree.SetMultipleSelection(FALSE);
	m_wndReportTree.SetTreeIndent(10);
}


void KDBaseODDlg::initPurposeReportControl(void)
{
	// Report Header 설정
	/*m_wndReportPurpose.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportPurpose.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReportPurpose.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReportPurpose.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);*/

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	if (KmzSystem::IsPassengerSystem())
	{
		pColumn = m_wndReportPurpose.AddColumn(new CXTPReportColumn(0, _T("Purpose"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행목적"));
        }
	}
	else
	{
		pColumn = m_wndReportPurpose.AddColumn(new CXTPReportColumn(0, _T("Commodity"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	}
	
	pColumn = m_wndReportPurpose.AddColumn(new CXTPReportColumn(1, _T("Field"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	// Report control 설정
	m_wndReportPurpose.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportPurpose.AllowEdit(TRUE);
	m_wndReportPurpose.FocusSubItems(TRUE);
	m_wndReportPurpose.Populate();
	//m_wndReportPurpose.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
}


void KDBaseODDlg::updateTargetTree(void)
{
	CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
	KProject* pProject = pApp->GetProject();
	if(NULL == pProject)
	{
		return;
	}

	CXTPReportRecord* pBaseRecord = NULL;
	CXTPReportRecordItem* pBaseItem = NULL;

	KScenario* pBase = pProject->GetBaseScenario();
	if((NULL != pBase) && (pBase->IsOpened() == true))
	{
		pBaseRecord = m_wndReportTree.AddRecord(new CXTPReportRecord);
		pBaseItem = pBaseRecord->AddItem(new CXTPReportRecordItemText);
		pBaseItem->SetCaption(pBase->GetName());
		pBaseItem->SetItemData((DWORD_PTR)pBase);
		
		addTargetRecord(pBaseRecord, pBase);
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
		CXTPReportRecord* pRecord;
		CXTPReportRecordItem* pItem;
		pRecord = m_wndReportTree.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pScenario->GetName());
		pItem->SetItemData((DWORD_PTR)pScenario);

		addTargetRecord(pRecord, pScenario);
	}

	if(NULL != pBaseRecord)
	{
		CXTPReportRecord* pCheckRecord = pBaseRecord->GetChilds()->GetAt(0);
		if(NULL != pCheckRecord)
		{
			CXTPReportRecordItem* pCheckItem = pCheckRecord->GetItem(0);
			if(NULL != pCheckItem)
			{
				pCheckItem->SetChecked(TRUE);
				KTarget* pTarget = (KTarget*)(pCheckItem->GetItemData());
				updateFieldConstraint(pTarget);
			}
		}
	}

	m_wndReportTree.Populate();
	m_wndReportTree.ExpandAll(TRUE);
}


void KDBaseODDlg::addTargetRecord(CXTPReportRecord* pParnet, KScenario* pScenario)
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;

	const KTDTargetMap& targetMap = pScenario->GetTargetMap();
	KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();
	for(itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
	{
		KTarget* pTarget = itTarget->second;
		if(pTarget->IsEmptyData() == true)
		{
			continue;
		}

		pRecord = pParnet->GetChilds()->Add(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pTarget->GetName());
		pItem->SetItemData((DWORD_PTR)pTarget);
		pItem->HasCheckbox(TRUE);
	}
}


void KDBaseODDlg::updatePurposeRecord(void)
{
	m_wndReportPurpose.ResetContent(TRUE);
	CXTPReportColumn* pColumn = m_wndReportPurpose.GetColumns()->GetAt(1);

	CXTPReportRecordItemConstraints* pReportRecordItemConstraints = pColumn->GetEditOptions()->GetConstraints();
	if (pReportRecordItemConstraints->GetCount() < 1)
		return; 

	CXTPReportRecordItemConstraint* pConstraint = pReportRecordItemConstraints->GetAt(0);

	DWORD_PTR dwDefault = pConstraint->m_dwData;
	CString strDefault = pConstraint->m_strConstraint;

	std::list<KDistributionInfo*>::iterator itInfo, itEnd = m_DInfoList.end();
	for(itInfo = m_DInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = *itInfo;
		CXTPReportRecord* pRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		pRecord = m_wndReportPurpose.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pInfo->PurposeName());
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData((DWORD_PTR)pInfo);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetCaption(strDefault);
		pItem->SetItemData(dwDefault);
	}

	m_wndReportPurpose.Populate();
}


void KDBaseODDlg::updateFieldConstraint(KTarget* pTarget)
{
	ASSERT(NULL != pTarget);
	CXTPReportColumn* pColumn = m_wndReportPurpose.GetColumns()->GetAt(1);
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
    
	pConstraints->RemoveAll();

	KIOTables* pIOTables = pTarget->Tables();
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

// KDBaseODDlg 메시지 처리기입니다.
BOOL KDBaseODDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initReportControl();
	updateTargetTree();
	updatePurposeRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDBaseODDlg::OnOK()
{
	// base od의 scenario와 target을 갱신
	KTarget* pBaseTarget = NULL;
	KScenario* pBaseScenario = NULL;
	CXTPReportRecords* pRecords = m_wndReportTree.GetRecords();
	int nCount = pRecords->GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecords* pChildRecords = pRecord->GetChilds();
		int nChildCount = pChildRecords->GetCount();
		for(int j = 0; j < nChildCount; ++j)
		{
			CXTPReportRecord* pChildRecord = pChildRecords->GetAt(j);
			CXTPReportRecordItem* pItem = pChildRecord->GetItem(0);
			if(pItem->IsChecked() == TRUE)
			{
				pBaseTarget = (KTarget*)(pItem->GetItemData());
				pItem = pRecord->GetItem(0);
				pBaseScenario = (KScenario*)(pItem->GetItemData());
				break;
			}
		}
	}

	if(m_DInfoList.size() < 1)
	{
		AfxMessageBox(_T("목적 통행 데이터가 존재하지 않습니다."));
		return;
	}

	std::list<KDistributionInfo*>::iterator itDInfo, itEnd = m_DInfoList.end();
	for(itDInfo = m_DInfoList.begin(); itDInfo != itEnd; ++itDInfo)
	{
		KDistributionInfo* pDInfo = *itDInfo;
		pDInfo->BaseODScenario(pBaseScenario);
		pDInfo->BaseODTarget(pBaseTarget);
	}

	// base od의 column을 갱신
	CXTPReportRecords* pDInfoRecords = m_wndReportPurpose.GetRecords();
	int nDInfoCount = pDInfoRecords->GetCount();
	for(int i = 0; i < nDInfoCount; ++i)
	{
		CXTPReportRecord* pDInfoRecord = pDInfoRecords->GetAt(i);
		CXTPReportRecordItem *pItem;
		pItem = pDInfoRecord->GetItem(0);
		KDistributionInfo* pDInfo = (KDistributionInfo*)(pItem->GetItemData());
		pItem = pDInfoRecord->GetItem(1);
		KIOColumn* pColumn = (KIOColumn*)(pItem->GetItemData());
		pDInfo->BaseODColumn(pColumn);
	}

	KDialogEx::OnOK();
}


void KDBaseODDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
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
			CXTPReportRecords* pParentRecords = m_wndReportTree.GetRecords();
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
			updateFieldConstraint(pTarget);
			updatePurposeRecord();
		}
	}
}