/**
 * @file MDefineVariableDlg.cpp
 * @brief KMDefineVariableDlg 구현 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "MDefineVariableDlg.h"
#include "afxdialogex.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "ExpressionChecker.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "MFunctionDlg.h"
#include "MGroupVariable.h"
#include "ModeChoiceInfo.h"
#include "ModeFunction.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "MSimpleVariable.h"
#include "Purpose.h"
#include "Mode.h"
#include "Target.h"
#include "ImTasDBase.h"
#include "DBaseConnector.h"

/* define user message */
#define UM_FUNCTIONEDIT			WM_USER + 1001


/* Report column index define */
#define MVARIABLE_NAME			0
#define MVARIABLE_FUNCTION		1
#define MVARIABLE_PARAMETER		2

///* Table name define */
//const TCHAR* TABLE_PARAMETER_OD	= _T("parameter_od");


// KMDefineVariableDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KMDefineVariableDlg, KDialogEx)

KMDefineVariableDlg::KMDefineVariableDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KMDefineVariableDlg::IDD, pParent)
{
	m_bGroup = false;
	m_pGroupColumn = NULL;
	m_pExpressionChecker = new KExpressionChecker;
}

KMDefineVariableDlg::~KMDefineVariableDlg()
{
	if(NULL != m_pExpressionChecker)
	{
		delete m_pExpressionChecker;
	}
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KMBaseVariable* pVariable = itVariable->second;
		delete pVariable;
	}
	m_VariableMap.clear();
}


void KMDefineVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_btnGroup);
	DDX_Control(pDX, IDC_STCGROUPLABEL, m_stcGroupLabel);
	DDX_Control(pDX, IDC_COMBO1, m_cmbGroupColumn);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KMDefineVariableDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KMDefineVariableDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &KMDefineVariableDlg::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KMDefineVariableDlg::OnCbnSelchangeCombo1)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_MESSAGE(UM_FUNCTIONEDIT, OnFunctionEdit)
	ON_BN_CLICKED(IDC_BUTTON1, &KMDefineVariableDlg::OnBnClickedButtonDefault)
END_MESSAGE_MAP()



void KMDefineVariableDlg::Target(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


bool KMDefineVariableDlg::Group(void)
{
	return m_bGroup;
}


void KMDefineVariableDlg::Group(bool bGroup)
{
	m_bGroup = bGroup;
}


//int KMDefineVariableDlg::CodeGroup(void)
//{
//	return m_nCodeGroup;
//}
//
//
//void KMDefineVariableDlg::CodeGroup(int nCodeGroup)
//{
//	m_nCodeGroup = nCodeGroup;
//}


KIOColumn* KMDefineVariableDlg::GroupColumn(void)
{
	return m_pGroupColumn;
}


void KMDefineVariableDlg::GroupColumn(KIOColumn* pColumn)
{
	m_pGroupColumn = pColumn;
}


void KMDefineVariableDlg::AddModeChoiceInfo(KModeChoiceInfo* pMCInfo)
{
	m_VariableMap[pMCInfo] = NULL;
}


void KMDefineVariableDlg::initExpressionChecker(void)
{
	ASSERT(NULL != m_pTarget);
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	if(NULL != pTable)
	{
		const KIOColumns* pColumns = pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				m_pExpressionChecker->AddField(pColumn->Caption());
			}
		}
	}

	m_pExpressionChecker->AddFunction(_T("SQRT"));
	m_pExpressionChecker->AddFunction(_T("EXP"));
	m_pExpressionChecker->AddFunction(_T("LN"));
	m_pExpressionChecker->AddFunction(_T("LOG"));
	m_pExpressionChecker->AddFunction(_T("ABS"));

	m_pExpressionChecker->AddOperator(_T("+"));
	m_pExpressionChecker->AddOperator(_T("-"));
	m_pExpressionChecker->AddOperator(_T("*"));
	m_pExpressionChecker->AddOperator(_T("/"));
	m_pExpressionChecker->AddOperator(_T("^"));
	m_pExpressionChecker->AddOperator(_T("("));
	m_pExpressionChecker->AddOperator(_T(")"));
}

void KMDefineVariableDlg::initGroupColumnList(void)
{
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
		if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			m_ColumnList.push_back(pColumn);
		}
	}
}


void KMDefineVariableDlg::updateGroupColumnCombobox(void)
{
	std::list<KIOColumn*>::iterator itColumn, itEnd = m_ColumnList.end();
	for(itColumn = m_ColumnList.begin(); itColumn != itEnd; ++itColumn)
	{
		KIOColumn* pColumn = *itColumn;
		int nIndex = m_cmbGroupColumn.AddString(pColumn->Caption());
		m_cmbGroupColumn.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cmbGroupColumn.SetCurSel(0);
	int nItemCount = m_cmbGroupColumn.GetCount();
	for(int i = 0; i < nItemCount; ++i)
	{
		KIOColumn* pColumn = (KIOColumn*)(m_cmbGroupColumn.GetItemData(i));
		//if(pColumn->CodeGroup() == m_nCodeGroup)
		//{
		//	m_cmbGroupColumn.SetCurSel(i);
		//	break;
		//}
		if(pColumn == m_pGroupColumn)
		{
			m_cmbGroupColumn.SetCurSel(i);
			break;
		}
	}
}


void KMDefineVariableDlg::updateVariableMap(void)
{
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KModeChoiceInfo* pMCInfo = itVariable->first;
		KMBaseVariable* pInfoVariable = pMCInfo->Variable();

		KMBaseVariable* pVariable = itVariable->second;
		if(NULL != pVariable)
		{
			delete pVariable;
			pVariable = NULL;
		}

		if(true == m_bGroup)
		{
			KMGroupVariable* pGroupVariable = new KMGroupVariable;
			if((NULL != pInfoVariable) && (pInfoVariable->ModeVariableType() == KEMModeVariableTypeGroup))
			{
				*pGroupVariable = *(KMGroupVariable*)pInfoVariable;
			}
			pGroupVariable->ModeTree(pMCInfo->ModeTree());
			itVariable->second = pGroupVariable;
		}
		else
		{
			KMSimpleVariable* pSimpleVariable = new KMSimpleVariable;
			if((NULL != pInfoVariable) && (pInfoVariable->ModeVariableType() == KEMModeVariableTypeSimple))
			{
				*pSimpleVariable = *(KMSimpleVariable*)pInfoVariable;
			}
			pSimpleVariable->ModeTree(pMCInfo->ModeTree());
			itVariable->second = pSimpleVariable;
		}
	}
}

void KMDefineVariableDlg::initReportControl(void)
{
	KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE);
	m_wndReport.SetTreeIndent(10);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MVARIABLE_NAME, _T("Purpose/Group/Mode"), 110));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MVARIABLE_FUNCTION, _T("Utility Function"), 290));
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MVARIABLE_PARAMETER, _T("Inclusive Value(1-α)"), 110));
}


void KMDefineVariableDlg::updateReportRecord(void)
{
	m_wndReport.GetRecords()->RemoveAll();

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KModeChoiceInfo* pInfo = itVariable->first;
		KPurpose* pPurpose = pInfo->Purpose();
		KMBaseVariable* pVariable = itVariable->second;

		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		                              pRecord->AddItem(new CXTPReportRecordItemText);
									  pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pPurpose->PurposeName());
		pItem->SetItemData((DWORD_PTR)pInfo);

		if(false == m_bGroup)
		{
			updateReportRecordSimple(pRecord, pVariable);
		}
		else
		{
			KCodeManager* pCodeManager = m_pTarget->CodeManager();
			//KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(m_nCodeGroup);
			//ASSERT(NULL != pCodeData);
			ASSERT(NULL != m_pGroupColumn);
			int nCodeGroup = m_pGroupColumn->CodeGroup();
			KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(nCodeGroup);
			ASSERT(NULL != pCodeData);
			
			std::vector<int> codeArray;
			pCodeData->GetCodes(codeArray);
			std::vector<int>::iterator itCode, itCodeEnd = codeArray.end();
			for(itCode = codeArray.begin(); itCode != itCodeEnd; ++itCode)
			{
				CXTPReportRecord* pCodeRecord = pRecord->GetChilds()->Add(new CXTPReportRecord);
				CXTPReportRecordItem* pCodeItem = pCodeRecord->AddItem(new CXTPReportRecordItemText);

				pCodeItem->SetCaption(pCodeData->SingleCodeValue(*itCode));
				pCodeItem->SetItemData(*itCode);

				updateReportRecordGroup(pCodeRecord, *itCode, pVariable);
			}
		}
	}

	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}


void KMDefineVariableDlg::updateReportRecordSimple(CXTPReportRecord* pParent, KMBaseVariable* pVariable)
{
	ASSERT(NULL != pParent);
	ASSERT(NULL != pVariable);
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);

	KModeTree* pModeTree = pVariable->ModeTree();
	KMSimpleVariable* pSimpleVariable = (KMSimpleVariable*)pVariable;

	std::queue<KModeTreeNode*> modeQ;
	std::queue<CXTPReportRecord*> recordQ;

	int nNodeCount = pModeTree->ModeNodeCount();
	for(int i = 0; i < nNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		modeQ.push(pModeNode);
		recordQ.push(pParent);
	}

	while(modeQ.empty() == false)
	{
		KModeTreeNodeMode* pNode = (KModeTreeNodeMode*)(modeQ.front());
		modeQ.pop();
		CXTPReportRecord* pRec = recordQ.front();
		recordQ.pop();

		CXTPReportRecord* pRecord = pRec->GetChilds()->Add(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pNode->ModeName());
		pItem->SetItemData(pNode->ModeID());

		if(pNode->HasChild() == true)
		{
			double dParam = pSimpleVariable->ModeParameter(pNode->ModeID());
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dParam, _T("%f")));

			int nChildCount = pNode->NodeCount();
			for(int i = 0; i < nChildCount; ++i)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(i));
				modeQ.push(pSubNode);
				recordQ.push(pRecord);
			}
		}
		else
		{
			CString strFunction = _T("");
			KModeFunction* pFunction = pSimpleVariable->ModeFunction(pNode->ModeID());
			if(NULL != pFunction)
			{
				strFunction = pFunction->GetCaptionExpression();
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			        pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strFunction);
			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(NULL != pButton)
			{
				pButton->SetAlignment(xtpItemControlUnknown);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					pButton->SetCaption(_T("편집"));
				}
				else {
					pButton->SetCaption(_T("Edit"));
				}
				pButton->SetSize(CSize(22, 0));
			}
		}
	}
}


void KMDefineVariableDlg::updateReportRecordGroup(CXTPReportRecord* pParent, int nGroupCode, KMBaseVariable* pVariable)
{
	ASSERT(NULL != pParent);
	ASSERT(NULL != pVariable);
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeGroup);

	KModeTree* pModeTree = pVariable->ModeTree();
	KMGroupVariable* pGroupVariable = (KMGroupVariable*)pVariable;

	std::queue<KModeTreeNode*> modeQ;
	std::queue<CXTPReportRecord*> recordQ;

	int nNodeCount = pModeTree->ModeNodeCount();
	for(int i = 0; i < nNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		modeQ.push(pModeNode);
		recordQ.push(pParent);
	}

	while(modeQ.empty() == false)
	{
		KModeTreeNodeMode* pNode = (KModeTreeNodeMode*)(modeQ.front());
		modeQ.pop();
		CXTPReportRecord* pRec = recordQ.front();
		recordQ.pop();

		CXTPReportRecord* pRecord = pRec->GetChilds()->Add(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pNode->ModeName());
		pItem->SetItemData(pNode->ModeID());

		if(pNode->HasChild() == true)
		{
			double dParam = pGroupVariable->ModeParameter(nGroupCode, pNode->ModeID());
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dParam, _T("%f")));

			int nChildCount = pNode->NodeCount();
			for(int i = 0; i < nChildCount; ++i)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(i));
				modeQ.push(pSubNode);
				recordQ.push(pRecord);
			}
		}
		else
		{
			CString strFunction = _T("");
			KModeFunction* pFunction = pGroupVariable->ModeFunction(nGroupCode, pNode->ModeID());
			if(NULL != pFunction)
			{
				strFunction = pFunction->GetCaptionExpression();
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			        pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strFunction);
			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(NULL != pButton)
			{
				pButton->SetAlignment(xtpItemControlUnknown);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					pButton->SetCaption(_T("편집"));
				}
				else {
					pButton->SetCaption(_T("Edit"));
				}
				pButton->SetSize(CSize(22, 0));
			}
		}
	}
}

//void KMDefineVariableDlg::updateReportRecordGroup(CXTPReportRecord* pParent, KModeTree* pModeTree, KMBaseVariable* pVariable)
//{
//	ASSERT(NULL != m_pCodeData);
//	std::vector<int> codeArray;
//	std::vector<int>::iterator itCode, itEnd;
//	m_pCodeData->GetCodes(codeArray);
//	for(itCode = codeArray.begin, itEnd = codeArray.end(); itCode != itEnd; ++itCode)
//	{
//		CString strCodeName = m_pCodeData->CodeValue(*itCode);
//		CXTPReportRecord* pRecord = pParent->GetChilds()->Add(new CXTPReportRecord);
//		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
//		pItem->SetCaption(strCodeName);
//		pItem->SetItemData(*itCode);
//
//		updateReportRecordMode(pRecord, pModeTree, pVariable);
//	}
//}
//
//
//void KMDefineVariableDlg::updateReportRecordGroupMode(CXTPReportRecord* pParent, KModeTree* pModeTree, KMBaseVariable* pVariable)
//{
//	std::queue<KModeTreeNode*> modeQ;
//	std::queue<CXTPReportRecord*> recordQ;
//
//	int nNodeCount = pModeTree->ModeNodeCount();
//	for(int i = 0; i < nNodeCount; ++i)
//	{
//		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
//		modeQ.push(pModeNode);
//		recordQ.push(pParent);
//	}
//
//	while(modeQ.empty() == false)
//	{
//		KModeTreeNodeMode* pNode = modeQ.front();
//		modeQ.pop();
//		CXTPReportRecord* pRec = recordQ.front();
//		recordQ.pop();
//
//		CXTPReportRecord* pRecord = pRec->GetChilds()->Add(new CXTPReportRecord);
//		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
//		pItem->SetCaption(pNode->ModeName());
//		pItem->SetItemData((DWORD_PTR)pNode)
//
//		if(pNode->HasChild() == true)
//		{
//			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
//			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber);
//		}
//		else
//		{
//			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
//			CXTPReportRecordItemButton* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
//			if(NULL != pButton)
//			{
//				pButton->SetAlignment(xtpItemControlRight);
//				pButton->SetCaption(_T("Edit"));
//				pButton->SetSize(CSize(22, 0));
//			}
//		}
//	}
//}


bool KMDefineVariableDlg::applySimpleVariable(CXTPReportRecord* pRoot)
{
	CXTPReportRecordItem* pRootItem = pRoot->GetItem(0);
	KModeChoiceInfo* pInfo = (KModeChoiceInfo*)(pRootItem->GetItemData());
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable = m_VariableMap.find(pInfo);
	if(m_VariableMap.end() == itVariable)
	{
		return false;
	}

	KMBaseVariable* pVariable = itVariable->second;
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);

	KMSimpleVariable* pSimpleVariable = (KMSimpleVariable*)pVariable;
	pSimpleVariable->RemoveAllModeFunction();
	pSimpleVariable->RemoveAllModeParameter();

	std::queue<CXTPReportRecord*> recordQ;
	CXTPReportRecords* pRecords = pRoot->GetChilds();
	int nRecordCount = pRecords->GetCount();

	if( nRecordCount< 1 )
	{
		CString strPurposeName = pInfo->Purpose()->PurposeName();

		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format(_T("\'%s\'에 대한 수단 관련 정보가 존재하지 않습니다."), strPurposeName);
		AfxMessageBox(strInputDataCheckMsg);

		return false;
	}

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		recordQ.push(pRecord);
	}

	while(recordQ.empty() == false)
	{
		CXTPReportRecord* pRecord = recordQ.front();
		recordQ.pop();

		CXTPReportRecordItem* pItem = pRecord->GetItem(MVARIABLE_NAME);
		int nModeID = pItem->GetItemData();
		CXTPReportRecords* pSubRecords = pRecord->GetChilds();
		
		int nSubRecordCount = pSubRecords->GetCount();
		if(0 < nSubRecordCount)
		{
			CXTPReportRecordItemNumber* pNumberItem = 
				(CXTPReportRecordItemNumber*)(pRecord->GetItem(MVARIABLE_PARAMETER));
			double dParam = pNumberItem->GetValue();
			pSimpleVariable->ModeParameter(nModeID, dParam);
			
			for(int j = 0; j < nSubRecordCount; ++j)
			{
				CXTPReportRecord* pSubRecord = pSubRecords->GetAt(j);
				recordQ.push(pSubRecord);
			}
		}
		else
		{
			CXTPReportRecordItem* pFunctionItem = pRecord->GetItem(MVARIABLE_FUNCTION);
            CString strFunction(pFunctionItem->GetCaption(NULL));

			KModeFunction func;
            func.SetCaptionExpression(strFunction);

			generateModeFunction(strFunction, func);

			if( func.GetExpressionList().size() < 1 )
			{
				CString strPurposeName       = pInfo->Purpose()->PurposeName();
				CString strInputDataCheckMsg = _T("");
				strInputDataCheckMsg.Format(_T("\'%s\'의 %s에 대한 수식 정보가 존재하지 않습니다."), strPurposeName, pItem->GetCaption(NULL));
				AfxMessageBox(strInputDataCheckMsg);

				CXTPReportRow* pRow = m_wndReport.GetRows()->Find(pFunctionItem->GetRecord());
				if(NULL != pRow)
				{
					pRow->SetSelected(TRUE);
					m_wndReport.SetFocusedRow(pRow);
					m_wndReport.SetTopRow(pRow->GetIndex());
				}

				return false;
			}

			pSimpleVariable->ModeFunction(nModeID, &func);
		}
	}

	return true;
}


bool KMDefineVariableDlg::applyGroupVariable(CXTPReportRecord* pRoot)
{
	CXTPReportRecordItem* pRootItem = pRoot->GetItem(0);
	KModeChoiceInfo* pInfo = (KModeChoiceInfo*)(pRootItem->GetItemData());
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable = m_VariableMap.find(pInfo);
	if(m_VariableMap.end() == itVariable)
	{
		return false;
	}

	KMBaseVariable* pVariable = itVariable->second;
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeGroup);

	KMGroupVariable* pGroupVariable = (KMGroupVariable*)pVariable;
	pGroupVariable->RemoveAllModeFunction();
	pGroupVariable->RemoveAllModeParameter();

	CXTPReportRecords* pGroupRecords = pRoot->GetChilds();
	int nGroupCount = pGroupRecords->GetCount();

	for(int i = 0; i < nGroupCount; ++i)
	{
		CXTPReportRecord* pGroupRecord = pGroupRecords->GetAt(i);
		CXTPReportRecordItem* pGroupItem = pGroupRecord->GetItem(MVARIABLE_NAME);
		int nGroupCode = pGroupItem->GetItemData();

		std::queue<CXTPReportRecord*> recordQ;
		CXTPReportRecords* pRecords = pGroupRecord->GetChilds();
		int nRecordCount = pRecords->GetCount();

		if( nRecordCount< 1 )
		{
			CString strPurposeName = pInfo->Purpose()->PurposeName();

			CString strInputDataCheckMsg = _T("");
			strInputDataCheckMsg.Format(_T("\'%s\'에 대한 수단 관련 정보가 존재하지 않습니다."), strPurposeName);
			AfxMessageBox(strInputDataCheckMsg);

			return false;
		}

		for(int j = 0; j < nRecordCount; ++j)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(j);
			recordQ.push(pRecord);
		}

		while(recordQ.empty() == false)
		{
			CXTPReportRecord* pRecord = recordQ.front();
			recordQ.pop();

			CXTPReportRecordItem* pItem = pRecord->GetItem(MVARIABLE_NAME);
			int nModeID = pItem->GetItemData();
			CXTPReportRecords* pSubRecords = pRecord->GetChilds();

			int nSubRecordCount = pSubRecords->GetCount();
			if(0 < nSubRecordCount)
			{
				CXTPReportRecordItemNumber* pNumberItem = 
					(CXTPReportRecordItemNumber*)(pRecord->GetItem(MVARIABLE_PARAMETER));
				double dParam = pNumberItem->GetValue();
				pGroupVariable->ModeParameter(nGroupCode, nModeID, dParam);

				for(int k = 0; k < nSubRecordCount; ++k)
				{
					CXTPReportRecord* pSubRecord = pSubRecords->GetAt(k);
					recordQ.push(pSubRecord);
				}
			}
			else
			{
				CXTPReportRecordItem* pFunctionItem = pRecord->GetItem(MVARIABLE_FUNCTION);
                CString strFunction(pFunctionItem->GetCaption(NULL));

                KModeFunction func;
                func.SetCaptionExpression(strFunction);

				generateModeFunction(strFunction, func);

				if( func.GetExpressionList().size() < 1 )
				{
					CString strPurposeName       = pInfo->Purpose()->PurposeName();
					CString strInputDataCheckMsg = _T("");
					strInputDataCheckMsg.Format(_T("\'%s\'의 %s에 대한 수식 정보가 존재하지 않습니다."), strPurposeName, pGroupItem->GetCaption(NULL));
					AfxMessageBox(strInputDataCheckMsg);

					CXTPReportRow* pRow = m_wndReport.GetRows()->Find(pFunctionItem->GetRecord());
					if(NULL != pRow)
					{
						pRow->SetSelected(TRUE);
						m_wndReport.SetFocusedRow(pRow);
						m_wndReport.SetTopRow(pRow->GetIndex());
					}

					return false;
				}

				pGroupVariable->ModeFunction(nGroupCode, nModeID, &func);
			}
		}
	}

	return true;
}


void KMDefineVariableDlg::generateModeFunction(LPCTSTR strFunction, KModeFunction& function)
{
	std::tstring str = strFunction;

	typedef boost::tokenizer<
		boost::char_separator<TCHAR>, 
		std::tstring::const_iterator, 
		std::tstring> ttokenizer;

	boost::char_separator<TCHAR> sep(_T(" "));
	ttokenizer tokens(str, sep);
	for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		std::tstring strToken = *it;
		if(m_pExpressionChecker->IsField(strToken.c_str()) == true)
		{
			std::tstring::size_type n = strToken.find_first_not_of(_T("["));
			strToken = (n == std::tstring::npos) ? strToken : strToken.substr(n, strToken.length());
			n = strToken.find_last_not_of(_T("]"));
			strToken = (n == std::tstring::npos) ? strToken : strToken.substr(0, n + 1);

			CString strFieldName = getFieldName(strToken.c_str());
			CString strFieldCaption =strToken.c_str();
			function.AddField(strFieldName, strFieldCaption);
		}
		else if(m_pExpressionChecker->IsFunction(strToken.c_str()) == true)
		{
			function.AddFunction(strToken.c_str());
		}
		else if(m_pExpressionChecker->IsOperator(strToken.c_str()) == true)
		{
			function.AddOperator(strToken.at(0));
		}
		else
		{
			function.AddNumber(_tstof(strToken.c_str()));
		}
	}
}


CString KMDefineVariableDlg::getFieldName(LPCTSTR strCaption)
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
			CString strColumnCaption = pColumn->Caption();
			if(strColumnCaption.Compare(strCaption) == 0)
			{
				strRet = pColumn->Name();
				break;
			}
		}
	}

	return strRet;
}

// KMDefineVariableDlg 메시지 처리기입니다.
BOOL KMDefineVariableDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initExpressionChecker();

	updateVariableMap();

	if(true == m_bGroup)
	{
		m_btnGroup.SetCheck(BST_CHECKED);
		m_stcGroupLabel.EnableWindow(TRUE);
		m_cmbGroupColumn.EnableWindow(TRUE);
	}
	else
	{
		m_btnGroup.SetCheck(BST_UNCHECKED);
		m_stcGroupLabel.EnableWindow(FALSE);
		m_cmbGroupColumn.EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);

	initGroupColumnList();
	initReportControl();
	updateGroupColumnCombobox();
	updateReportRecord();

	LoadDefaultFunction(m_mapDefaultFunction);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMDefineVariableDlg::LoadDefaultFunction(std::map<int, std::map<int, CString>> &a_mapDefaultFunction)
{
	try
	{
		int nMasterCode(0); //0:koti
		KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();

		KImTasDBase::ModeChoiceFormula(spDBaseConnection, nMasterCode, KEMPurposeGroupODBase, KEMModeGroupODBase, a_mapDefaultFunction);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMDefineVariableDlg::OnBnClickedOk()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		if(false == m_bGroup)
		{
			if( false == applySimpleVariable(pRecord) )
			{
				return;
			}
		}
		else
		{
			applyGroupVariable(pRecord);
		}
	}

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KModeChoiceInfo* pInfo = itVariable->first;
		KMBaseVariable* pVariable = itVariable->second;
		pInfo->Variable(pVariable);
	}

	KDialogEx::OnOK();
}


void KMDefineVariableDlg::OnBnClickedCheck1()
{
	int nCheck = m_btnGroup.GetCheck();
	if(BST_CHECKED == nCheck)
	{
		m_bGroup = true;
		m_stcGroupLabel.EnableWindow(TRUE);
		m_cmbGroupColumn.EnableWindow(TRUE);
		int nSel = m_cmbGroupColumn.GetCurSel();
		if(CB_ERR != nSel)
		{
			KIOColumn* pColumn = (KIOColumn*)(m_cmbGroupColumn.GetItemData(nSel));
			ASSERT(NULL != pColumn);

			//m_nCodeGroup = pColumn->CodeGroup();
			m_pGroupColumn = pColumn;
		}
	}
	else
	{
		m_bGroup = false;
		m_stcGroupLabel.EnableWindow(FALSE);
		m_cmbGroupColumn.EnableWindow(FALSE);
	}
	
	// group check가 변경되면 이전에 생성된 variable을 갱신한다.
	updateVariableMap();
	updateReportRecord();
}


void KMDefineVariableDlg::OnCbnSelchangeCombo1()
{
	int nSel = m_cmbGroupColumn.GetCurSel();
	if(CB_ERR == nSel)
	{
		return;
	}
	
	KIOColumn* pColumn = (KIOColumn*)(m_cmbGroupColumn.GetItemData(nSel));
	ASSERT(NULL != pColumn);

	//m_nCodeGroup = pColumn->CodeGroup();
	m_pGroupColumn = pColumn;

	updateReportRecord();
}


void KMDefineVariableDlg::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	//CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	//CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_FUNCTIONEDIT, (WPARAM)(pItemNotify->pItem));
}


LRESULT KMDefineVariableDlg::OnFunctionEdit(WPARAM wParam, LPARAM lParam)
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)wParam;
	
	KMFunctionDlg dlg;
	dlg.Expression(pItem->GetCaption(NULL));
	dlg.FieldList(m_pExpressionChecker->FieldList());
	dlg.FunctionList(m_pExpressionChecker->FunctionList());
	dlg.OperatorList(m_pExpressionChecker->OperatorList());

	if(dlg.DoModal() == IDOK)
	{
		pItem->SetCaption(dlg.Expression());
	}

	return 0;
}


void KMDefineVariableDlg::OnBnClickedButtonDefault()
{
	try
	{
		AutoType iter  = m_mapDefaultFunction.begin();
		AutoType itEnd = m_mapDefaultFunction.end();

		CString               strFunction(_T(""));
		CXTPReportRecords*    pRecords = m_wndReport.GetRecords();
		CXTPReportRecordItem* pItem    = nullptr;
		int                   nCount   = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*     pParRecord = pRecords->GetAt(i);
			pItem               = pParRecord->GetItem(MVARIABLE_NAME);
			KModeChoiceInfo*      pInfo    = (KModeChoiceInfo*)pItem->GetItemData();
			KPurpose*             pPurpose = pInfo->Purpose();
			int nPurposeID      = pPurpose->PurposeID();

			iter = m_mapDefaultFunction.find(nPurposeID);
			if (iter == itEnd)
				continue;

			std::map<int, CString> &mapFunction = iter->second;

			CXTPReportRecords*    pChdRecords = pParRecord->GetChilds();
			int nChdRecordCount = pChdRecords->GetCount();

			for (int k= 0; k< nChdRecordCount; k++)
			{
				CXTPReportRecord* pChdRecord = pChdRecords->GetAt(k);
				pItem           = pChdRecord->GetItem(MVARIABLE_NAME);
				int nModeID     = (int)pItem->GetItemData();

				AutoType iterB  = mapFunction.find(nModeID);
				AutoType itEndB = mapFunction.end();

				if (iterB == itEndB)
					continue;

				strFunction = iterB->second;

				pItem           = pChdRecord->GetItem(MVARIABLE_FUNCTION);
				pItem->SetCaption(strFunction);
			}
		}
		m_wndReport.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
}
