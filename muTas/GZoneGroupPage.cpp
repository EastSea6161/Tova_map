/**
 * @file GZoneGroupPage.cpp
 * @brief KGZoneGroupPage 구현 파일
 * @author 
 * @date 2011.05.04
 * @remark
 */
#pragma once

//#include "Target.h"
#include "stdafx.h"
#include "KmzApp.h"
#include "GZoneGroupPage.h"
#include "afxdialogex.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "GenerationInfo.h"
#include "GFunctionRegressionDlg.h"
#include "GFunctionUnitDlg.h"
#include "GGroupExpressionContainer.h"
#include "GRegressionExpression.h"
#include "GUnitExpression.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"
#include "KGCategoryExpression.h"
#include "KGCategoryExpressionDlg.h"

// User message define
#define UM_EXPRESSIONEDIT		WM_USER + 1001


// KGGroupExpressionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGZoneGroupPage, CXTPPropertyPage)

KGZoneGroupPage::KGZoneGroupPage()
	:	CXTPPropertyPage(KGZoneGroupPage::IDD),
		m_pKGInfo(NULL),
		m_pKZone(NULL),
		m_pKExpressionRoot(NULL),
		m_pUnitCopyExpression(nullptr),
		m_pRegressionCopyExpression(nullptr),
		m_pCategoryCopyExpression(nullptr)

{

}

KGZoneGroupPage::~KGZoneGroupPage()
{
	if(NULL != m_pKExpressionRoot)
	{
		QBicDelete(m_pKExpressionRoot);
	}
	
	ClearCopyData();
}

void KGZoneGroupPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_COMBO1, m_cboField);
}



BEGIN_MESSAGE_MAP(KGZoneGroupPage, CXTPPropertyPage)
	ON_BN_CLICKED(IDC_ADD, &KGZoneGroupPage::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &KGZoneGroupPage::OnBnClickedDelete)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT, OnReportSelChanged)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &KGZoneGroupPage::OnBnClickedButtonDefault)
	ON_NOTIFY(NM_RCLICK, IDC_REPORT, &KGZoneGroupPage::OnReportItemRClick)
END_MESSAGE_MAP()


void KGZoneGroupPage::SetGenerationInfo(KGenerationInfo* a_pGInfo)
{
	m_pKGInfo              = a_pGInfo;
	m_lstCurrentColumnList = m_lstColumnList;
	ASSERT(m_pKGInfo);

	KGBaseExpressionContainer* pBaseCont = m_pKGInfo->ExpressionContainer();
	if(NULL == pBaseCont)
	{
		return;
	}
	if(pBaseCont->ContainerType() != KEMGExpressionContainerZoneGroup)
	{
		m_pKGInfo->DeleteExpression();
		return;
	}
	ASSERT(pBaseCont->ContainerType() == KEMGExpressionContainerZoneGroup);
	KGGroupExpressionContainer* pContainer = (KGGroupExpressionContainer*)pBaseCont;
	KZoneGroupColumn* pGroupRoot           = pContainer->GetGroupRoot();
	if(NULL != pGroupRoot)
	{
		m_pKExpressionRoot = new KZoneGroupColumn(*pGroupRoot);
	}
}



void KGZoneGroupPage::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}
	m_pTarget = a_pTarget;
}



void KGZoneGroupPage::SetZoneTable(KIOTable* a_pTable)
{
	m_pKZone = a_pTable;
}


void KGZoneGroupPage::SetCodeManager(KCodeManager* a_pCodeManager)
{
	m_pKCodeManager = a_pCodeManager;
}


void KGZoneGroupPage::UpdateReportColumns(std::list<TGroupColumnInfo>& a_lstRemainColumns)
{
	std::list<TGroupColumnInfo>::iterator itColumns;
	CXTPReportRecord* pColRecord = NULL;
	CXTPReportRecord* pValRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;

	while(a_lstRemainColumns.empty() == false)
	{
		TGroupColumnInfo g = a_lstRemainColumns.front();
		KZoneGroupColumn* pColumn = g.pColumn;
		CXTPReportRecord* pRecord = g.pParent;
		
		// Report에 column 정보를 추가
		pColRecord = pRecord->GetChilds()->Add(new CXTPReportRecord);
		pItem = pColRecord->AddItem(new CXTPReportRecordItemText(pColumn->ColumnCaption()));
		pItem->SetItemData((DWORD_PTR)(pColumn));

		pItem = pColRecord->AddItem(new CXTPReportRecordItemText(_T("")));

		KZoneGroupValue* pValue = pColumn->GetFirstZoneGroupValue();
		while(NULL != pValue)
		{
			pValRecord = pColRecord->GetChilds()->Add(new CXTPReportRecord);
			pItem = pValRecord->AddItem(new CXTPReportRecordItemText(pValue->ValueName()));
			pItem->SetItemData((DWORD_PTR)pValue);

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				gci.pParent = pValRecord;
				a_lstRemainColumns.push_back(gci);

			}
			else	// 그렇지 않으면 expression을 설정
			{
				KGBaseExpression* pExpression = pValue->ExpressionObject();
				pItem = pValRecord->AddItem(new CXTPReportRecordItemText(pExpression->GetCaption()));
				CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(!pButton)
				{
					continue;
				}
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("..."));
				pButton->SetSize(CSize(22, 0));
			}
			pValue = pColumn->GetNextZoneGroupValue();
		}
		a_lstRemainColumns.pop_front();
	}
}


// KGGroupExpressionPage 메시지 처리기입니다.


//Step_1
BOOL KGZoneGroupPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	//WS_EX_STATICEDGE 
	// combo box를 초기화 한 후
	InitZoneGroup();
	// report control을  초기화

	//KReportCtrlSetting::Default(m_oReport, TRUE);
	//KReportCtrlSetting::SelectionEnableFalse(m_oReport);

	KReportCtrlSetting::DefaultTreeStructuerDotSize(m_wndReport, 15);
	InitReportControl();
	UpdateReportControl();
	UpdateComboList();
	UpdateButtonState();
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT );
	SetResize(IDC_COMBO1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT );
	SetResize(IDC_ADD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT );
	SetResize(IDC_DELETE, SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT );
	SetResize(IDC_BUTTON1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KGZoneGroupPage::InitZoneGroup(void)
{
	if(NULL == m_pKZone)
	{
		return;
	}

	const KIOColumns* pColumns = m_pKZone->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			m_lstColumnList.push_back(pColumn);
		}
	}
	m_lstCurrentColumnList = m_lstColumnList;
	UpdateComboList();
}


void KGZoneGroupPage::InitReportControl(void)
{
	// report header 설정
// 	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// report tree 설정
// 	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureDots;
// 	m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Zone Group"), 300));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 그룹"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Formula"), 700));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수식"));
    }

	// Report control 설정
// 	m_wndReport.AllowEdit(FALSE);
// 	m_wndReport.EditOnClick(FALSE);
// 	m_wndReport.SetMultipleSelection(FALSE);
// 	m_wndReport.SetTreeIndent(10);
}

void KGZoneGroupPage::UpdateReportControl(BOOL a_bExpandAll)
{
	if((m_wndReport.GetSafeHwnd() == NULL) || (NULL == m_pKGInfo))
	{
		return;
	}

	m_wndReport.GetRecords()->RemoveAll();

	KZoneGroupColumn* pGroupRoot = m_pKExpressionRoot;

	if(NULL != pGroupRoot)
	{
		std::list<TGroupColumnInfo> lstRemainColumns;
		CXTPReportRecord* pColRecord = NULL;
		CXTPReportRecord* pValRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		// root column 추가
		pColRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem = pColRecord->AddItem(new CXTPReportRecordItemText(pGroupRoot->ColumnCaption()));
		pColRecord->AddItem(new CXTPReportRecordItemText());
		pItem->SetItemData((DWORD_PTR)pGroupRoot);

		// value 추가
		KZoneGroupValue* pValue = pGroupRoot->GetFirstZoneGroupValue();
		while(NULL != pValue)
		{
			pValRecord = pColRecord->GetChilds()->Add(new CXTPReportRecord);
			pItem = pValRecord->AddItem(new CXTPReportRecordItemText(pValue->ValueName()));
			pItem->SetItemData((DWORD_PTR)pValue);

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo oTGci;
				oTGci.pColumn = pValue->GetSubColumn();
				oTGci.pParent = pValRecord;
				lstRemainColumns.push_back(oTGci);

				pItem = pValRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			}
			else	// 그렇지 않으면 expression을 설정
			{
				KGBaseExpression* pExpression = pValue->ExpressionObject();
				if (pExpression)
				{
					pItem = pValRecord->AddItem(new CXTPReportRecordItemText(pExpression->GetCaption()));
				}
				else
				{
					pItem = pValRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				}
				CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(!pButton)
				{
					continue;
				}

				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("..."));
				pButton->SetSize(CSize(22, 0));
			}
			pValue = pGroupRoot->GetNextZoneGroupValue();
		}
		UpdateReportColumns(lstRemainColumns);
	}

	m_wndReport.Populate();
	if(TRUE == a_bExpandAll)
	{
		m_wndReport.ExpandAll(TRUE);
	}
}

void KGZoneGroupPage::UpdateComboList(void)
{
	m_cboField.ResetContent();

	std::list<KIOColumn*>::iterator itColumn, itEnd = m_lstCurrentColumnList.end();
	for(itColumn = m_lstCurrentColumnList.begin(); itColumn != itEnd; ++itColumn)
	{
		KIOColumn* pColumn = *itColumn;
		int nIndex = m_cboField.AddString(pColumn->Caption());
		m_cboField.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cboField.SetCurSel(0);
}

void KGZoneGroupPage::UpdateButtonState(void)
{
	BOOL bAdd, bDelete;
	bAdd = bDelete = FALSE;

	if(m_wndReport.GetRecords()->GetCount() == 0)
	{
		bAdd = TRUE;
	}
	else
	{
		CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
		if(NULL != pRow)
		{
			CXTPReportRecord* pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			KZoneGroupBase* pGroupBase = (KZoneGroupBase*)(pItem->GetItemData());
			if(KEMZoneGroupColumn == pGroupBase->ZoneGroupObjectType())
			{
				bDelete = TRUE;
			}
			else
			{
				KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pGroupBase;
				if(pGroupValue->HasSubColumn() == false)
				{
					if(m_cboField.GetCount() > 0)
					{
						bAdd = TRUE;
					}
				}
			}
		}
	}

	m_btnAdd.EnableWindow(bAdd);
	m_btnDelete.EnableWindow(bDelete);
}

//Step_2
void KGZoneGroupPage::OnBnClickedAdd()
{
	// report 가 비어있으므로 새로 추가
	CXTPReportRecord* pParent = NULL;

	if(m_wndReport.GetRecords()->GetCount() == 0)
	{

	}
	else
	{
		// report가 비어있지 않으면 선택된 row가 자식을 가지고 있는지 확인하고
		// 자식이 없으면 추가
		CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
		if(NULL == pRow)
		{
			return;
		}
		pParent = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pParent->GetItem(0);
		KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
		ASSERT(NULL != pBase);

		if(pBase->ZoneGroupObjectType() != KEMZoneGroupValue)
		{
			return;
		}
	}

	AddField(pParent);
	UpdateButtonState();
}

void KGZoneGroupPage::AddField(CXTPReportRecord* a_pParent)
{
	// combo box에서 값을 가져온다.
	if(m_cboField.GetCount() == 0)
	{
		return;
	}

	// 해당 column 을 확인
	KIOColumn* pColumn = NULL;
	int nSel = m_cboField.GetCurSel();
	pColumn = (KIOColumn*)(m_cboField.GetItemData(nSel));
	ASSERT(NULL != pColumn);

	// 해당 column의 code data를 확인
	KCodeGroup* pCodeData = m_pKCodeManager->FindCodeGroup(pColumn->CodeGroup());
	ASSERT(NULL != pCodeData);

	KZoneGroupColumn* pGroupColumn = NULL;

	if(NULL == a_pParent)
	{
		m_pKExpressionRoot = new KZoneGroupColumn;
		m_pKExpressionRoot->ColumnName(pColumn->Name());
		m_pKExpressionRoot->ColumnCaption(pColumn->Caption());
		pGroupColumn = m_pKExpressionRoot;
	}
	else
	{
		CXTPReportRecordItem* pItem = a_pParent->GetItem(0);
		KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
		ASSERT(NULL != pBase);
		ASSERT(pBase->ZoneGroupObjectType() == KEMZoneGroupValue);

		KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
		pGroupColumn = pGroupValue->AddSubColumn(pColumn->Name(), pColumn->Caption());
	}

	std::map<CString, int> mapCodeGroup;
	GetParentsData(a_pParent, mapCodeGroup);

	std::set<int> setCode;
	KDBaseGeneration::GetSubCode(m_pTarget, pColumn->Name(), mapCodeGroup, setCode);

	if (mapCodeGroup.size() < 1)
	{
		std::vector<int> codeList;
		std::vector<int>::iterator itCode, itEnd;
		pCodeData->GetCodes(codeList);

		itEnd = codeList.end();

		for(itCode = codeList.begin(); itCode != itEnd; ++itCode)
		{
			KZoneGroupValue* pSubValue = pGroupColumn->AddZoneGroupValue();
			pSubValue->Value(*itCode);
			pSubValue->ValueName(pCodeData->SingleCodeValue(*itCode));
			pSubValue->CreateExpressionObject(m_pKGInfo->GenerationModel());
		}
	}
	else
	{
		if (setCode.size() < 1)
		{
			CXTPReportRecordItem* pItem = a_pParent->GetItem(0);
			KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());

			KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
			pGroupValue->RemoveSubColumn();
		}
		else
		{
			std::vector<int> codeList;
			std::vector<int>::iterator itCode, itEnd;
			pCodeData->GetCodes(codeList);

			itEnd = codeList.end();

			for(itCode = codeList.begin(); itCode != itEnd; ++itCode)
			{
				AutoType find = setCode.find(*itCode);
				AutoType end  = setCode.end();
				if (find != end)
				{
					KZoneGroupValue* pSubValue = pGroupColumn->AddZoneGroupValue();
					pSubValue->Value(*itCode);
					pSubValue->ValueName(pCodeData->SingleCodeValue(*itCode));
					pSubValue->CreateExpressionObject(m_pKGInfo->GenerationModel());
				}
			}
		}
	}

	UpdateReportControl();
}


//Step_3
void KGZoneGroupPage::OnBnClickedDelete()
{
	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL == pRow)
	{
		return;
	}

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
	if(pBase->ZoneGroupObjectType() != KEMZoneGroupColumn)
	{
		return;
	}

	CXTPReportRecord* pParent = pRecord->GetParentRecord();
	if(NULL == pParent)
	{
		delete m_pKExpressionRoot;
		m_pKExpressionRoot = NULL;
		m_lstCurrentColumnList = m_lstColumnList;
		UpdateComboList();
	}
	else
	{
		pItem = pParent->GetItem(0);
		KZoneGroupValue* pValue = (KZoneGroupValue*)(pItem->GetItemData());
		pValue->RemoveSubColumn();
	}

	UpdateReportControl();
	UpdateButtonState();
}


//Step_4
void KGZoneGroupPage::OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


//Step_5
void KGZoneGroupPage::OnReportSelChanged(NMHDR* a_pNMHDR, LRESULT* a_pResult)
{
	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL != pRow)
	{
		std::map<CString, KIOColumn*> columnMap;
		std::map<CString, KIOColumn*>::iterator itColumnMap;
		std::list<KIOColumn*>::iterator itColumn, itEnd = m_lstColumnList.end();
		for(itColumn = m_lstColumnList.begin(); itColumn != itEnd; ++itColumn)
		{
			KIOColumn* pColumn = *itColumn;
			columnMap.insert(std::make_pair(pColumn->Name(), pColumn));
		}

		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		KZoneGroupBase* pGroupBase = (KZoneGroupBase*)(pItem->GetItemData());
		if(KEMZoneGroupValue == pGroupBase->ZoneGroupObjectType())
		{
			// Parent의 column 목록을 삭제한다.
			CXTPReportRecord* pParent = pRecord->GetParentRecord();
			while(NULL != pParent)
			{
				pItem = pParent->GetItem(0);
				pGroupBase = (KZoneGroupBase*)(pItem->GetItemData());
				if(pGroupBase->ZoneGroupObjectType() == KEMZoneGroupColumn)
				{
					KZoneGroupColumn* pGroupColumn = (KZoneGroupColumn*)(pItem->GetItemData());
					itColumnMap = columnMap.find(pGroupColumn->ColumnName());
					if(itColumnMap != columnMap.end())
					{
						columnMap.erase(itColumnMap);
					}
				}
				pParent = pParent->GetParentRecord();
			}

			m_lstCurrentColumnList.clear();
			for(itColumnMap = columnMap.begin(); itColumnMap != columnMap.end(); ++itColumnMap)
			{
				m_lstCurrentColumnList.push_back(itColumnMap->second);
			}
			UpdateComboList();
		}
	}
	UpdateButtonState();
}


//Step_6
LRESULT KGZoneGroupPage::OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam)
{
	CXTPReportRecordItem* pItem       = (CXTPReportRecordItem*)a_wParam;
	KZoneGroupValue*      pGroupValue = (KZoneGroupValue*)(pItem->GetItemData());
	CXTPReportRow*        pRow        = m_wndReport.GetRows()->Find(pItem->GetRecord());
	int nSelectedIndex  = pRow->GetIndex();

	if(KEMGenerationUnit == m_pKGInfo->GenerationModel())
	{
		KGUnitExpression* pExpression = (KGUnitExpression*)(pGroupValue->ExpressionObject());
		KGFunctionUnitDlg dlg;
		dlg.SetTarget	(m_pTarget);
		dlg.Number      (pExpression->Number());
		dlg.FieldName   (pExpression->FieldName());
		dlg.FieldCaption(pExpression->FieldCaption());
		if(dlg.DoModal() == IDOK)
		{
			pExpression->Number(dlg.Number());
			pExpression->Field (dlg.FieldName(), dlg.FieldCaption());
			pGroupValue->DoneExpression(true);

			CXTPReportRecord*     pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(1);
			pItem->SetCaption(pExpression->GetCaption());
			/*this->UpdateReportControl(FALSE);*/
		}
	}
	else if(KEMGenerationRegression == m_pKGInfo->GenerationModel())
	{
		KGRegressionExpression* pExpression = (KGRegressionExpression*)(pGroupValue->ExpressionObject());
		CString strExpression               = pExpression->GetCaptionExpression();
		KGFunctionRegressionDlg dlg;
		if(strExpression != _T(""))
		{
			dlg.SetTarget(m_pTarget);
			dlg.SetExpressionView(strExpression);
		}

		dlg.SetTarget(m_pTarget);
		dlg.Expression(pExpression);
		if(dlg.DoModal() == IDOK)
		{
			pGroupValue->DoneExpression(true);

			CXTPReportRecord*     pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(1);
			pItem->SetCaption(pExpression->GetCaption());
			/*this->UpdateReportControl(FALSE);*/
		}
	}
    else if(KEMGenerationCrossClassification == m_pKGInfo->GenerationModel())
    {
        KGCategoryExpressionDlg dlg;
        KGCategoryExpression* pExpression = (KGCategoryExpression*)(pGroupValue->ExpressionObject());
        dlg.Expression(pExpression);
		dlg.SetTarget(m_pTarget);
        if(dlg.DoModal() == IDOK)
        {
			pGroupValue->DoneExpression(true);

			CXTPReportRecord*     pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(1);
			pItem->SetCaption(pExpression->GetCaption());
            /*this->UpdateReportControl(FALSE);*/
        }
    }

	CXTPReportRow* pUpdateRow = m_wndReport.GetRows()->GetAt(nSelectedIndex);
	if(NULL != pUpdateRow)
	{
		m_wndReport.SetFocusedRow(pUpdateRow);
		/*m_oReport.SetTopRow(pUpdateRow->GetIndex());*/
	}

	return 0;
}

BOOL KGZoneGroupPage::OnApply()
{
	if( FALSE == InvalidateInputInfo() )
	{
		return FALSE;
	}
	
	if(NULL != m_pKExpressionRoot)
	{
		KGGroupExpressionContainer* pContainer = 
			(KGGroupExpressionContainer*)m_pKGInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGGroupExpressionContainer*)(m_pKGInfo->CreateExpression());
		}

		KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
		if(NULL == pGroupRoot)
		{
			pGroupRoot = pContainer->CreateGroupRoot(m_pKExpressionRoot->ColumnName(), m_pKExpressionRoot->ColumnCaption());
		}

		*pGroupRoot = *m_pKExpressionRoot;
		
		int k = 0;
	}

	CXTPPropertyPage::OnApply();
	return TRUE;
}


BOOL KGZoneGroupPage::InvalidateInputInfo()
{
	UpdateData(TRUE);

	if((m_wndReport.GetSafeHwnd() == NULL))
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}
		AfxMessageBox(_T("m_oReport.GetSafeHwnd() null"));
		return FALSE;
	}

	CXTPReportRecords*	  pRecords   = NULL;

	KZoneGroupColumn*     pGroupRoot = NULL;
	std::list<TGroupColumnInfo> lstRemainColumns;
	CXTPReportRecord*     pColRecord = NULL;
	CXTPReportRecord*     pValRecord = NULL;
	CXTPReportRecordItem* pItem      = NULL;

	// root column 추가
	pRecords = m_wndReport.GetRecords();

	if(pRecords->GetCount() < 1)
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}
		AfxMessageBox(_T("카테고리 항목을 선택하고 추가 버튼을 클릭해 주세요."));
		m_cboField.SetFocus();
		return FALSE;
	}

	for(int i= 0; i< pRecords->GetCount(); i++)
	{
		pColRecord    = pRecords->GetAt(i);
			
		pItem = pColRecord->GetItem(0);
		pGroupRoot = (KZoneGroupColumn*)pItem->GetItemData();

		// value 추가
		KZoneGroupValue* pValue = pGroupRoot->GetFirstZoneGroupValue();
		int nChdIdx = 0;
		
		while( pValue != NULL )
		{
			pValRecord = pColRecord->GetChilds()->GetAt(nChdIdx);
			pItem = pValRecord->GetItem(0);
			pValue = (KZoneGroupValue*)pItem->GetItemData();

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo oTGci;
				oTGci.pColumn = pValue->GetSubColumn();
				oTGci.pParent = pValRecord;
				lstRemainColumns.push_back(oTGci);
			}
			else	// 그렇지 않으면 expression을 설정
			{
				KGBaseExpression* pExpression = pValue->ExpressionObject();
				if (pExpression)
				{
					pItem = pValRecord->GetItem(1);
				}
				else
				{
					pItem = pValRecord->GetItem(1);
				}

				if( false == pValue->DoneExpression() )
				{
					if(m_pParentSheet != NULL)
					{
						m_pParentSheet->SetActivePage(this);
					}
					AfxMessageBox(_T("수식을 설정해 주세요."));

					CXTPReportRow* pRow = m_wndReport.GetRows()->Find(pItem->GetRecord());
					if(NULL != pRow)
					{
						pRow->SetSelected(TRUE);
						m_wndReport.SetFocusedRow(pRow);
						m_wndReport.SetTopRow(pRow->GetIndex());
					}

					return FALSE;
				}
			}
			pValue = pGroupRoot->GetNextZoneGroupValue();
			nChdIdx++;
		}
		/*UpdateReportColumns(lstRemainColumns);*/
		if( FALSE == CheckReportColumns(lstRemainColumns))
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL KGZoneGroupPage::CheckReportColumns(std::list<TGroupColumnInfo>& a_lstRemainColumns)
{

	std::list<TGroupColumnInfo>::iterator itColumns;
	CXTPReportRecord* pColRecord = NULL;
	CXTPReportRecord* pValRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;

	int nIdx = 0;
	while(a_lstRemainColumns.empty() == false)
	{
		TGroupColumnInfo g = a_lstRemainColumns.front();
		KZoneGroupColumn* pColumn = g.pColumn;
		CXTPReportRecord* pRecord = g.pParent;

		// Report에 column 정보를 추가
		int ncount = pRecord->GetChilds()->GetCount();
		pColRecord = pRecord->GetChilds()->GetAt(0);
		pItem = pColRecord->GetItem(0);
		pColumn = (KZoneGroupColumn*)pItem->GetItemData();

		KZoneGroupValue* pValue = pColumn->GetFirstZoneGroupValue();
		int nChdIdx = 0;
		while(NULL != pValue)
		{
			pValRecord = pColRecord->GetChilds()->GetAt(nChdIdx);
			pItem = pValRecord->GetItem(0);
			pValue = (KZoneGroupValue* )pItem->GetItemData();

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				gci.pParent = pValRecord;
				a_lstRemainColumns.push_back(gci);
			}
			else	// 그렇지 않으면 expression을 설정
			{
				KGBaseExpression* pExpression = pValue->ExpressionObject();
				pItem = pValRecord->GetItem(1);
			
				if( false == pValue->DoneExpression() )
				{
					if(m_pParentSheet != NULL)
					{
						m_pParentSheet->SetActivePage(this);
					}
					AfxMessageBox(_T("수식을 설정해 주세요."));

					CXTPReportRow* pRow = m_wndReport.GetRows()->Find(pItem->GetRecord());
					if(NULL != pRow)
					{
						pRow->SetSelected(TRUE);
						m_wndReport.SetFocusedRow(pRow);
						m_wndReport.SetTopRow(pRow->GetIndex());
					}

					return FALSE;
				}
				
			}
			pValue = pColumn->GetNextZoneGroupValue();

			nChdIdx++;
		}
		a_lstRemainColumns.pop_front();

		nIdx++;
	}

	return TRUE;
}


void KGZoneGroupPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CXTPPropertyPage::OnPaint()을(를) 호출하지 마십시오.
}

bool KGZoneGroupPage::GetParentsData( CXTPReportRecord* a_pParent, std::map<CString, int>& a_mapCode )
{
	try
	{
		CXTPReportRecord* pParentRecord = nullptr;
						  pParentRecord = a_pParent->GetParentRecord();
		if (pParentRecord == nullptr)
		{
			return false;
		}

		CXTPReportRecordItem* pItem = a_pParent->GetItem(0);
		KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
		KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
		int nCodeValue = pGroupValue->Value();

		if(pGroupValue->ZoneGroupObjectType() != KEMZoneGroupValue)
		{
			if (GetParentsData(pParentRecord, a_mapCode) == false)
			{
				return false;
			}
		}

		CXTPReportRecordItem* pItem2 = pParentRecord->GetItem(0);
		KZoneGroupBase* pBase2 = (KZoneGroupBase*)(pItem2->GetItemData());
		KZoneGroupColumn* pGroupValue2 = (KZoneGroupColumn*)pBase2;

		a_mapCode.insert(std::make_pair(pGroupValue2->ColumnName(), nCodeValue));

		if (GetParentsData(pParentRecord, a_mapCode) == false)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return true;
}


void KGZoneGroupPage::OnBnClickedButtonDefault()
{
	KGBaseExpressionContainer* pBaseCont = m_pDefaultGInfo->ExpressionContainer();
	if(nullptr == pBaseCont)
	{
		return;
	}
	if(pBaseCont->ContainerType() != KEMGExpressionContainerZoneGroup)
	{
		m_pDefaultGInfo->DeleteExpression();
		return;
	}
	
	KGGroupExpressionContainer* pContainer = (KGGroupExpressionContainer*)pBaseCont;
	if (pContainer == nullptr)
	{
		return;
	}

	KZoneGroupColumn* pGroupRoot           = pContainer->GetGroupRoot();
	if(NULL != pGroupRoot)
	{
		m_pKExpressionRoot = new KZoneGroupColumn(*pGroupRoot);
	}

	UpdateReportControl();
}

void KGZoneGroupPage::OnReportItemRClick( NMHDR * pNotifyStruct, LRESULT * result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow)
		return;

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_ZONEGROUP));

	KZoneGroupBase* pGroupBase = nullptr;
	CXTPReportRow* pRow = pItemNotify->pRow;
	if(NULL != pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pGroupBase = (KZoneGroupBase*)(pItem->GetItemData());

		KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pGroupBase;

		if(KEMZoneGroupColumn == pGroupBase->ZoneGroupObjectType() || pGroupValue->HasSubColumn() == true)
		{
			menu.EnableMenuItem(ID_ZONEGROUP_COPY, MF_BYCOMMAND|MF_DISABLED);
			menu.EnableMenuItem(ID_ZONEGROUP_PASTE, MF_BYCOMMAND|MF_DISABLED);
			menu.EnableMenuItem(ID_ZONEGROUP_PASTEALL, MF_BYCOMMAND|MF_DISABLED);
		}
		else
		{
			if (m_pUnitCopyExpression == nullptr && m_pRegressionCopyExpression == nullptr && m_pCategoryCopyExpression == nullptr)
			{
				menu.EnableMenuItem(ID_ZONEGROUP_COPY, MF_BYCOMMAND|MF_CHECKED);
				menu.EnableMenuItem(ID_ZONEGROUP_PASTE, MF_BYCOMMAND|MF_DISABLED);
				menu.EnableMenuItem(ID_ZONEGROUP_PASTEALL, MF_BYCOMMAND|MF_DISABLED);
			}
			else
			{
				menu.EnableMenuItem(ID_ZONEGROUP_COPY, MF_BYCOMMAND|MF_CHECKED);
				menu.EnableMenuItem(ID_ZONEGROUP_PASTE, MF_BYCOMMAND|MF_CHECKED);
				menu.EnableMenuItem(ID_ZONEGROUP_PASTEALL, MF_BYCOMMAND|MF_CHECKED);
			}
		}
	}

	// track menu
	int nMenuResult = CXTPCommandBars::TrackPopupMenu(menu.GetSubMenu(0), TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, pItemNotify->pt.x, pItemNotify->pt.y, this, NULL);

	switch(nMenuResult)
	{
	case ID_ZONEGROUP_COPY:
		{
			KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pGroupBase;
			KGBaseExpression* pBaseExpression = pGroupValue->ExpressionObject();
			if (pBaseExpression == nullptr)
			{
				break;
			}

			ClearCopyData();

			if (m_pKGInfo->GenerationModel() == KEMGenerationUnit)
			{
				m_pUnitCopyExpression = new KGUnitExpression;
				*m_pUnitCopyExpression = *(KGUnitExpression*)pBaseExpression;
			}
			else if (m_pKGInfo->GenerationModel() == KEMGenerationRegression)
			{
				m_pRegressionCopyExpression = new KGRegressionExpression;
				*m_pRegressionCopyExpression = *(KGRegressionExpression*)pBaseExpression;
			}
			else if(m_pKGInfo->GenerationModel() == KEMGenerationCrossClassification)
			{
				m_pCategoryCopyExpression = new KGCategoryExpression;
				*m_pCategoryCopyExpression = *(KGCategoryExpression*)pBaseExpression;
			}
			else
			{
				return;
			}

			break;
		}
	case ID_ZONEGROUP_PASTE:
		{
			CXTPReportRecord* pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
			KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
			if (pGroupValue->ZoneGroupObjectType() == KEMZoneGroupValue)
			{
				if (KEMGenerationUnit == m_pKGInfo->GenerationModel())
				{
					if (m_pUnitCopyExpression != nullptr)
					{
						pItem = pRecord->GetItem(1);
						pItem->SetCaption(m_pUnitCopyExpression->GetCaption());
						KGUnitExpression* pUnitExpression = (KGUnitExpression*)pGroupValue->ExpressionObject();
						*pUnitExpression = *m_pUnitCopyExpression;
						pGroupValue->DoneExpression(true);
					}
				}
				else if (KEMGenerationRegression == m_pKGInfo->GenerationModel())
				{
					if (m_pRegressionCopyExpression != nullptr)
					{
						pItem = pRecord->GetItem(1);
						pItem->SetCaption(m_pRegressionCopyExpression->GetCaption());
						KGRegressionExpression* pRegressionExpression = (KGRegressionExpression*)pGroupValue->ExpressionObject();
						*pRegressionExpression = *m_pRegressionCopyExpression;
						pGroupValue->DoneExpression(true);
					}
				}
				else if (KEMGenerationCrossClassification == m_pKGInfo->GenerationModel())
				{
					if (m_pCategoryCopyExpression != nullptr)
					{
						pItem = pRecord->GetItem(1);
						pItem->SetCaption(m_pCategoryCopyExpression->GetCaption());
						KGCategoryExpression* pCategoryExpression = (KGCategoryExpression*)pGroupValue->ExpressionObject();
						*pCategoryExpression = *m_pCategoryCopyExpression;
						pGroupValue->DoneExpression(true);
					}
				}
			}
			m_wndReport.Populate();
			break;
		}
	case ID_ZONEGROUP_PASTEALL:
		{
			InsertCopyData();
			m_wndReport.Populate();
			break;
		}
	}
}

void KGZoneGroupPage::ClearCopyData()
{
	if (nullptr != m_pUnitCopyExpression)
	{
		QBicDelete(m_pUnitCopyExpression);
	}
	if (nullptr != m_pRegressionCopyExpression)
	{
		QBicDelete(m_pRegressionCopyExpression)
	}
	if (nullptr != m_pCategoryCopyExpression)
	{
		QBicDelete(m_pCategoryCopyExpression)
	}
}

BOOL KGZoneGroupPage::InsertCopyData()
{
	UpdateData(TRUE);

	if((m_wndReport.GetSafeHwnd() == NULL))
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}
		AfxMessageBox(_T("m_oReport.GetSafeHwnd() null"));
		return FALSE;
	}

	CXTPReportRecords*	  pRecords   = NULL;

	KZoneGroupColumn*     pGroupRoot = NULL;
	std::list<TGroupColumnInfo> lstRemainColumns;
	CXTPReportRecord*     pColRecord = NULL;
	CXTPReportRecord*     pValRecord = NULL;
	CXTPReportRecordItem* pItem      = NULL;

	// root column 추가
	pRecords = m_wndReport.GetRecords();

	if(pRecords->GetCount() < 1)
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}
		AfxMessageBox(_T("카테고리 항목을 선택하고 추가 버튼을 클릭해 주세요."));
		m_cboField.SetFocus();
		return FALSE;
	}

	for(int i= 0; i< pRecords->GetCount(); i++)
	{
		pColRecord    = pRecords->GetAt(i);

		pItem = pColRecord->GetItem(0);
		pGroupRoot = (KZoneGroupColumn*)pItem->GetItemData();

		// value 추가
		KZoneGroupValue* pValue = pGroupRoot->GetFirstZoneGroupValue();
		int nChdIdx = 0;

		while( pValue != nullptr )
		{
			pValRecord = pColRecord->GetChilds()->GetAt(nChdIdx);
			pItem = pValRecord->GetItem(0);
			pValue = (KZoneGroupValue*)pItem->GetItemData();

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo oTGci;
				oTGci.pColumn = pValue->GetSubColumn();
				oTGci.pParent = pValRecord;
				lstRemainColumns.push_back(oTGci);
			}
			else	// 그렇지 않으면 expression을 설정
			{
				if (m_pKGInfo->GenerationModel() == KEMGenerationUnit)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pUnitCopyExpression->GetCaption());

					KGUnitExpression* pUnitExpression = (KGUnitExpression*)pValue->ExpressionObject();;
					*pUnitExpression = *m_pUnitCopyExpression;
					pValue->DoneExpression(true);
				}
				else if (m_pKGInfo->GenerationModel() == KEMGenerationRegression)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pRegressionCopyExpression->GetCaption());
					KGRegressionExpression* pRegressionExpression = (KGRegressionExpression*)pValue->ExpressionObject();
					*pRegressionExpression = *m_pRegressionCopyExpression;
					pValue->DoneExpression(true);
				}
				else if (m_pKGInfo->GenerationModel() == KEMGenerationCrossClassification)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pCategoryCopyExpression->GetCaption());
					KGCategoryExpression* pCategoryExpression = (KGCategoryExpression*)pValue->ExpressionObject();
					*pCategoryExpression = *m_pCategoryCopyExpression;
					pValue->DoneExpression(true);
				}
			}
			pValue = pGroupRoot->GetNextZoneGroupValue();
			nChdIdx++;
		}

		if( FALSE == InsertCopySubData(lstRemainColumns))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL KGZoneGroupPage::InsertCopySubData( std::list<TGroupColumnInfo>& a_lstRemainColumns )
{
	std::list<TGroupColumnInfo>::iterator itColumns;
	CXTPReportRecord* pColRecord = NULL;
	CXTPReportRecord* pValRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;

	int nIdx = 0;
	while(a_lstRemainColumns.empty() == false)
	{
		TGroupColumnInfo g = a_lstRemainColumns.front();
		KZoneGroupColumn* pColumn = g.pColumn;
		CXTPReportRecord* pRecord = g.pParent;

		// Report에 column 정보를 추가
		int ncount = pRecord->GetChilds()->GetCount();
		pColRecord = pRecord->GetChilds()->GetAt(0);
		pItem = pColRecord->GetItem(0);
		pColumn = (KZoneGroupColumn*)pItem->GetItemData();

		KZoneGroupValue* pValue = pColumn->GetFirstZoneGroupValue();
		int nChdIdx = 0;
		while(NULL != pValue)
		{
			pValRecord = pColRecord->GetChilds()->GetAt(nChdIdx);
			pItem = pValRecord->GetItem(0);
			pValue = (KZoneGroupValue* )pItem->GetItemData();

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				gci.pParent = pValRecord;	
				a_lstRemainColumns.push_back(gci);
			}
			else	// 그렇지 않으면 expression을 설정
			{
				if (m_pKGInfo->GenerationModel() == KEMGenerationUnit)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pUnitCopyExpression->GetCaption());

					KGUnitExpression* pUnitExpression = (KGUnitExpression*)pValue->ExpressionObject();;
					*pUnitExpression = *m_pUnitCopyExpression;
					pValue->DoneExpression(true);
				}
				else if (m_pKGInfo->GenerationModel() == KEMGenerationRegression)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pRegressionCopyExpression->GetCaption());
					KGRegressionExpression* pRegressionExpression = (KGRegressionExpression*)pValue->ExpressionObject();
					*pRegressionExpression = *m_pRegressionCopyExpression;
					pValue->DoneExpression(true);
				}
				else if (m_pKGInfo->GenerationModel() == KEMGenerationCrossClassification)
				{
					pItem = pValRecord->GetItem(1);
					pItem->SetCaption(m_pCategoryCopyExpression->GetCaption());
					KGCategoryExpression* pCategoryExpression = (KGCategoryExpression*)pValue->ExpressionObject();
					*pCategoryExpression = *m_pCategoryCopyExpression;
					pValue->DoneExpression(true);
				}

			}
			pValue = pColumn->GetNextZoneGroupValue();
			nChdIdx++;
		}
		a_lstRemainColumns.pop_front();

		nIdx++;
	}

	return TRUE;
}

