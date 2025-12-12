/**
 * @file GGroupExpressionPage.cpp
 * @brief KGGroupExpressionPage 구현 파일
 * @author teegee@zaolsoft.com
 * @date 2011.05.04
 * @remark
 */


#include "stdafx.h"
#include "kmz.h"
#include "GGroupExpressionPage.h"
#include "afxdialogex.h"
#include "CodeGroup.h"
#include "CodeManager.h"
#include "GenerationInfo.h"
#include "GFunctionRegressionDlg.h"
#include "GFunctionUnitDlg.h"
#include "GGroupExpressionContainer.h"
#include "GRegressionExpression.h"
#include "GUnitExpression.h"
#include "IOColumn.h"
#include "IOColumnCollection.h"
#include "IOTable.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"
#include "KGCategoryExpression.h"
#include "KGCategoryExpressionDlg.h"

// User message define
#define UM_EXPRESSIONEDIT		WM_USER + 1001


// KGGroupExpressionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGGroupExpressionPage, CXTPPropertyPage)

KGGroupExpressionPage::KGGroupExpressionPage()
	:	CXTPPropertyPage(KGGroupExpressionPage::IDD),
		m_pGInfo(NULL),
		m_pZone(NULL),
		m_pExpressionRoot(NULL)
{

}

KGGroupExpressionPage::~KGGroupExpressionPage()
{
	if(NULL != m_pExpressionRoot)
	{
		delete m_pExpressionRoot;
	}
}

void KGGroupExpressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_Report);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_COMBO1, m_cmbField);
}



BEGIN_MESSAGE_MAP(KGGroupExpressionPage, CXTPPropertyPage)
	ON_BN_CLICKED(IDC_ADD, &KGGroupExpressionPage::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &KGGroupExpressionPage::OnBnClickedDelete)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT, OnReportSelChanged)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
END_MESSAGE_MAP()



void KGGroupExpressionPage::SetGenerationInfo(KGenerationInfo* pGInfo)
{
	m_pGInfo = pGInfo;
	m_CurrentColumnList = m_ColumnList;
	ASSERT(m_pGInfo);

	KGBaseExpressionContainer* pBaseCont = m_pGInfo->ExpressionContainer();
	if(NULL == pBaseCont)
	{
		return;
	}
	ASSERT(pBaseCont->ContainerType() == KEMGExpressionContainerZoneGroup);
	KGGroupExpressionContainer* pContainer = (KGGroupExpressionContainer*)pBaseCont;
	KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
	if(NULL != pGroupRoot)
	{
		m_pExpressionRoot = new KZoneGroupColumn(*pGroupRoot);
	}
}


void KGGroupExpressionPage::SetZoneTable(KIOTable* pTable)
{
	m_pZone = pTable;
}


void KGGroupExpressionPage::SetCodeManager(KCodeManager* pCodeManager)
{
	m_pCodeManager = pCodeManager;
}


void KGGroupExpressionPage::initZoneGroup(void)
{
	if(NULL == m_pZone)
	{
		return;
	}

	const KIOColumnCollection* pColumns = m_pZone->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->Code() == true)
		{
			m_ColumnList.push_back(pColumn);
		}
	}
	m_CurrentColumnList = m_ColumnList;
	updateComboList();
}


void KGGroupExpressionPage::initReportControl(void)
{
	// report header 설정
	m_Report.GetReportHeader()->AllowColumnRemove(FALSE);
	m_Report.GetReportHeader()->AllowColumnSort(FALSE);
	m_Report.GetReportHeader()->AllowColumnReorder(FALSE);
	m_Report.GetReportHeader()->AllowColumnSort(FALSE);
	m_Report.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// report tree 설정
	m_Report.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureDots;
	
	m_Report.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
	m_Report.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = m_Report.AddColumn(new CXTPReportColumn(0, _T("Zone Group"), 200));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);
	pColumn = m_Report.AddColumn(new CXTPReportColumn(1, _T("Formular"), 800));

	// Report control 설정
	m_Report.AllowEdit(FALSE);
	m_Report.EditOnClick(FALSE);
	m_Report.SetMultipleSelection(FALSE);
	m_Report.SetTreeIndent(10);
}


void KGGroupExpressionPage::addField(CXTPReportRecord* pParent)
{
	// combo box에서 값을 가져온다.
	if(m_cmbField.GetCount() == 0)
	{
		return;
	}

	// 해당 column 을 확인
	KIOColumn* pColumn = NULL;
	int nSel = m_cmbField.GetCurSel();
	pColumn = (KIOColumn*)(m_cmbField.GetItemData(nSel));
	ASSERT(NULL != pColumn);

	// 해당 column의 code data를 확인
	KCodeGroup* pCodeData = m_pCodeManager->FindCodeGroup(pColumn->CodeGroup());
	ASSERT(NULL != pCodeData);

	KZoneGroupColumn* pGroupColumn = NULL;

	//// root에 추가되는 경우
	//if(NULL == pParent)
	//{
	//	KGBaseExpressionContainer* pContainer = m_pGInfo->ExpressionContainer();
	//	if(pContainer->ContainerType() == KEMGExpressionContainerZoneGroup)
	//	{
	//		KGGroupExpressionContainer* pGroupContainer = (KGGroupExpressionContainer*)pContainer;
	//		pGroupColumn = pGroupContainer->CreateGroupRoot(pColumn->Name(), pColumn->Caption());
	//	}
	//	
	//}
	//else
	//{
	//	CXTPReportRecordItem* pItem = pParent->GetItem(0);
	//	KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
	//	ASSERT(NULL != pBase);
	//	ASSERT(pBase->ZoneGroupObjectType() == KEMZoneGroupValue);
	//	
	//	KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
	//	pGroupColumn = pGroupValue->AddSubColumn(pColumn->Name(), pColumn->Caption());
	//}

	//std::vector<int> codeList;
	//pCodeData->GetCodes(codeList);
	//KGBaseExpressionContainer* pContainer = m_pGInfo->ExpressionContainer();
	//for(std::vector<int>::iterator itCode = codeList.begin(); 
	//	itCode != codeList.end(); ++itCode)
	//{
	//	KZoneGroupValue* pSubValue = pGroupColumn->AddZoneGroupValue();
	//	pSubValue->Value(*itCode);
	//	pSubValue->ValueName(pCodeData->CodeValue(*itCode));
	//	
	//	pSubValue->CreateExpressionObject(pContainer->GenerationModel());
	//}

	// root에 추가
	if(NULL == pParent)
	{
		m_pExpressionRoot = new KZoneGroupColumn;
		m_pExpressionRoot->ColumnName(pColumn->Name());
		m_pExpressionRoot->ColumnCaption(pColumn->Caption());
		pGroupColumn = m_pExpressionRoot;
	}
	else
	{
		CXTPReportRecordItem* pItem = pParent->GetItem(0);
		KZoneGroupBase* pBase = (KZoneGroupBase*)(pItem->GetItemData());
		ASSERT(NULL != pBase);
		ASSERT(pBase->ZoneGroupObjectType() == KEMZoneGroupValue);

		KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
		pGroupColumn = pGroupValue->AddSubColumn(pColumn->Name(), pColumn->Caption());
	}

	std::vector<int> codeList;
	std::vector<int>::iterator itCode, itEnd;
	pCodeData->GetCodes(codeList);

	itEnd = codeList.end();

	for(itCode = codeList.begin(); itCode != itEnd; ++itCode)
	{
		KZoneGroupValue* pSubValue = pGroupColumn->AddZoneGroupValue();
		pSubValue->Value(*itCode);
		pSubValue->ValueName(pCodeData->CodeValue(*itCode));
		pSubValue->CreateExpressionObject(m_pGInfo->GenerationModel());
	}

	updateReportControl();
}


void KGGroupExpressionPage::updateReportControl(void)
{
	if((m_Report.GetSafeHwnd() == NULL) || (NULL == m_pGInfo))
	{
		return;
	}

	m_Report.GetRecords()->RemoveAll();

	KZoneGroupColumn* pGroupRoot = m_pExpressionRoot;
	
	if(NULL != pGroupRoot)
	{
		std::list<GroupColumnInfo> remainColumns;
		CXTPReportRecord* pColRecord = NULL;
		CXTPReportRecord* pValRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		// root column 추가
		pColRecord = m_Report.AddRecord(new CXTPReportRecord);
		pItem = pColRecord->AddItem(new CXTPReportRecordItemText(pGroupRoot->ColumnCaption()));
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
				GroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				gci.pParent = pValRecord;
				remainColumns.push_back(gci);
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

				pButton->SetAlignment(xtpItemControlRight);
				pButton->SetCaption(_T("..."));
				pButton->SetSize(CSize(22, 0));
			}
			pValue = pGroupRoot->GetNextZoneGroupValue();
		}
		updateReportColumns(remainColumns);
	}
	
	m_Report.Populate();
	m_Report.ExpandAll(TRUE);
}


void KGGroupExpressionPage::updateReportColumns(std::list<GroupColumnInfo>& remainColumns)
{
	std::list<GroupColumnInfo>::iterator itColumns;
	CXTPReportRecord* pColRecord = NULL;
	CXTPReportRecord* pValRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;

	while(remainColumns.empty() == false)
	{
		GroupColumnInfo g = remainColumns.front();
		KZoneGroupColumn* pColumn = g.pColumn;
		CXTPReportRecord* pRecord = g.pParent;
		
		// Report에 column 정보를 추가
		pColRecord = pRecord->GetChilds()->Add(new CXTPReportRecord);
		pItem = pColRecord->AddItem(new CXTPReportRecordItemText(pColumn->ColumnCaption()));
		pItem->SetItemData((DWORD_PTR)(pColumn));

		KZoneGroupValue* pValue = pColumn->GetFirstZoneGroupValue();
		while(NULL != pValue)
		{
			pValRecord = pColRecord->GetChilds()->Add(new CXTPReportRecord);
			pItem = pValRecord->AddItem(new CXTPReportRecordItemText(pValue->ValueName()));
			pItem->SetItemData((DWORD_PTR)pValue);

			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				GroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				gci.pParent = pValRecord;
				remainColumns.push_back(gci);
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
				pButton->SetAlignment(xtpItemControlRight);
				pButton->SetCaption(_T("..."));
				pButton->SetSize(CSize(22, 0));
			}
			pValue = pColumn->GetNextZoneGroupValue();
		}
		remainColumns.pop_front();
	}
}


void KGGroupExpressionPage::updateButtonState(void)
{
	BOOL bAdd, bDelete;
	bAdd = bDelete = FALSE;

	if(m_Report.GetRecords()->GetCount() == 0)
	{
		bAdd = TRUE;
	}
	else
	{
		CXTPReportRow* pRow = m_Report.GetFocusedRow();
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
					if(m_cmbField.GetCount() > 0)
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


void KGGroupExpressionPage::updateComboList(void)
{
	m_cmbField.ResetContent();

	std::list<KIOColumn*>::iterator itColumn, itEnd = m_CurrentColumnList.end();
	for(itColumn = m_CurrentColumnList.begin(); itColumn != itEnd; ++itColumn)
	{
		KIOColumn* pColumn = *itColumn;
		int nIndex = m_cmbField.AddString(pColumn->Caption());
		m_cmbField.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cmbField.SetCurSel(0);
}


// KGGroupExpressionPage 메시지 처리기입니다.
BOOL KGGroupExpressionPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();
	//WS_EX_STATICEDGE 
	// combo box를 초기화 한 후
	initZoneGroup();

	// report control을  초기화
	initReportControl();

	updateReportControl();
	updateComboList();
	updateButtonState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KGGroupExpressionPage::OnBnClickedAdd()
{
	// report 가 비어있으므로 새로 추가
	CXTPReportRecord* pParent = NULL;

	if(m_Report.GetRecords()->GetCount() == 0)
	{
		
	}
	else
	{
		// report가 비어있지 않으면 선택된 row가 자식을 가지고 있는지 확인하고
		// 자식이 없으면 추가
		CXTPReportRow* pRow = m_Report.GetFocusedRow();
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

	addField(pParent);
	updateButtonState();
}


void KGGroupExpressionPage::OnBnClickedDelete()
{
	CXTPReportRow* pRow = m_Report.GetFocusedRow();
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
		delete m_pExpressionRoot;
		m_pExpressionRoot = NULL;
		m_CurrentColumnList = m_ColumnList;
		updateComboList();
	}
	else
	{
		pItem = pParent->GetItem(0);
		KZoneGroupValue* pValue = (KZoneGroupValue*)(pItem->GetItemData());
		pValue->RemoveSubColumn();
	}

	updateReportControl();
	updateButtonState();
}


void KGGroupExpressionPage::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


void KGGroupExpressionPage::OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	CXTPReportRow* pRow = m_Report.GetFocusedRow();
	if(NULL != pRow)
	{
		std::map<CString, KIOColumn*> columnMap;
		std::map<CString, KIOColumn*>::iterator itColumnMap;
		std::list<KIOColumn*>::iterator itColumn, itEnd = m_ColumnList.end();
		for(itColumn = m_ColumnList.begin(); itColumn != itEnd; ++itColumn)
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

			m_CurrentColumnList.clear();
			for(itColumnMap = columnMap.begin(); itColumnMap != columnMap.end(); ++itColumnMap)
			{
				m_CurrentColumnList.push_back(itColumnMap->second);
			}
			updateComboList();
		}
	}
	updateButtonState();
}


LRESULT KGGroupExpressionPage::OnExpressionEdit(WPARAM wParam, LPARAM lParam)
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)wParam;
	KZoneGroupValue* pGroupValue = (KZoneGroupValue*)(pItem->GetItemData());
	
	if(KEMGenerationUnit == m_pGInfo->GenerationModel())
	{
		KGUnitExpression* pExpression = (KGUnitExpression*)(pGroupValue->ExpressionObject());
		KGFunctionUnitDlg dlg;
		dlg.Number(pExpression->Number());
		dlg.FieldName(pExpression->FieldName());
		dlg.FieldCaption(pExpression->FieldCaption());
		if(dlg.DoModal() == IDOK)
		{
			pExpression->Number(dlg.Number());
			pExpression->Field(dlg.FieldName(), dlg.FieldCaption());
			this->updateReportControl();
		}
	}
	else if(KEMGenerationRegression == m_pGInfo->GenerationModel())
	{
		KGRegressionExpression* pExpression = (KGRegressionExpression*)(pGroupValue->ExpressionObject());
		KGFunctionRegressionDlg dlg;
		dlg.Expression(pExpression);
		if(dlg.DoModal() == IDOK)
		{
			this->updateReportControl();
		}
	}
    else if(KEMGenerationCrossClassification == m_pGInfo->GenerationModel())
    {
        KGCategoryExpressionDlg dlg;
        KGCategoryExpression* pExpression = (KGCategoryExpression*)(pGroupValue->ExpressionObject());
        dlg.Expression(pExpression);

        if(dlg.DoModal() == IDOK)
        {
            this->updateReportControl();
        }
    }
	
	CXTPReportRow* pRow = m_Report.GetRows()->Find(pItem->GetRecord());
	if(NULL != pRow)
	{
		m_Report.SetFocusedRow(pRow);
	}
	return 0;
}

void KGGroupExpressionPage::OnOK()
{
	if(NULL != m_pExpressionRoot)
	{
		KGGroupExpressionContainer* pContainer = 
			(KGGroupExpressionContainer*)m_pGInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGGroupExpressionContainer*)(m_pGInfo->CreateExpression());
		}

		KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
		if(NULL == pGroupRoot)
		{
			pGroupRoot = pContainer->CreateGroupRoot(m_pExpressionRoot->ColumnName(), m_pExpressionRoot->ColumnCaption());
		}

		*pGroupRoot = *m_pExpressionRoot;
		int k = 0;
	}
	CXTPPropertyPage::OnOK();
}
