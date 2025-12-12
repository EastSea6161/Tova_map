// MDefineVariableEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MDefineVariableEdit.h"
#include "afxdialogex.h"
#include <queue>
#include "MFunctionDlg.h"
#include "MGroupVariable.h"
#include "ModeFunction.h"
#include "Mode.h"
#include "ModeManager.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "ExpressionChecker.h"
#include "MSimpleVariable.h"
#include "Target.h"

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"
#include "ImChampFrameWindow.h"
#include "DefineNamesDefaultPara.h"

#define UM_FUNCTIONEDIT			WM_USER + 1001

#define MVARIABLE_NAME			0
#define MVARIABLE_FUNCTION		1
#define MVARIABLE_PARAMETER		2

// KMDefineVariableEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMDefineVariableEdit, KDialogEx)

KMDefineVariableEdit::KMDefineVariableEdit(int a_nVariableID, KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KMDefineVariableEdit::IDD, pParent)
	, m_nPreSelectedVariableID(a_nVariableID)
	, m_pTarget(a_pTarget)
{
	m_pExpressionChecker = new KExpressionChecker;
	m_pModeTree          = new KModeTree;
	m_nFunctionID        = 0;
}

KMDefineVariableEdit::~KMDefineVariableEdit()
{
	if (nullptr != m_pModeTree)
	{
		QBicDelete(m_pModeTree);
	}

	if (nullptr != m_pExpressionChecker)
	{
		QBicDelete(m_pExpressionChecker);
	}


	DeleteTempSaveData();
}

void KMDefineVariableEdit::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cboFunction);
	DDX_Control(pDX, IDC_CBO_MODE, m_cboMode);
}


BEGIN_MESSAGE_MAP(KMDefineVariableEdit, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KMDefineVariableEdit::OnBnClickedRadioFunction)

	ON_BN_CLICKED(IDC_BTN_MODE_ADD, &KMDefineVariableEdit::OnBnClickedBtnModeAdd)
	ON_BN_CLICKED(IDC_BTN_MODE_DEL, &KMDefineVariableEdit::OnBnClickedBtnModeDel)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT, &KMDefineVariableEdit::OnReportSelChanged)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KMDefineVariableEdit::OnItemButtonClick)
	ON_MESSAGE(UM_FUNCTIONEDIT, &KMDefineVariableEdit::OnFunctionEdit)
	ON_BN_CLICKED(IDC_BTN_APPLY, &KMDefineVariableEdit::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BUTTON_FUNC_DELETE, &KMDefineVariableEdit::OnBnClickedButtonFuncDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &KMDefineVariableEdit::OnCbnSelchangeComboFunc)
	ON_BN_CLICKED(IDC_BTN_REMOVEALL, &KMDefineVariableEdit::OnBnClickedBtnRemoveall)
END_MESSAGE_MAP()


// KMDefineVariableEdit 메시지 처리기입니다.

void KMDefineVariableEdit::InitExpressionChecker(void)
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
			if(pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
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


void KMDefineVariableEdit::DeleteTempSaveData()
{
	try
	{
		AutoType iter  = m_mapTempSaveData.begin();
		AutoType itEnd = m_mapTempSaveData.end();

		while (iter != itEnd)
		{
			KMSimpleVariable* pSimpleVariable = iter->second;

			if (nullptr != pSimpleVariable)
			{
				KModeTree*    pDelModeTree    = pSimpleVariable->ModeTree();
				if (nullptr != pDelModeTree)
				{
					QBicDelete(pDelModeTree);
				}

				QBicDelete(pSimpleVariable);
			}

			++iter;
		}

		m_mapTempSaveData.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::LoadXmlData()
{
	try
	{
		DeleteTempSaveData();

		KProject* pProject = ImChampFrameWindow::GetProject();
		CString   strDir   = pProject->GetLocation();
		strDir.AppendFormat(_T("\\%s\\"),  KDefaultParaFolder::FOLDER_PARAMETER);
		
		KXmlManager oXmlManager;
		oXmlManager.LoadTargetXml(strDir, KDefaultParaFile::FILE_MODECHOICE_VAR);

		TiXmlElement* pRootElement     = oXmlManager.AccessRootElement(_T("ModeChoice-Variable-Default"));

		TiXmlElement* pElementVariable = oXmlManager.AccessElment(_T("Variable"), pRootElement);
		while (nullptr != pElementVariable)
		{
			int     nVariableID     = oXmlManager.GetAttributeInt(_T("ID"),   pElementVariable);  
			CString strVariableName = oXmlManager.GetAttributeStr(_T("Name"), pElementVariable);

			KMSimpleVariable* pLoadSimpleVariable = new KMSimpleVariable;
			KModeTree*        pLoadModeTree       = new KModeTree;

			pLoadSimpleVariable->ModeTree    (pLoadModeTree);
			pLoadSimpleVariable->VariableID  (nVariableID);
			pLoadSimpleVariable->VariableName(strVariableName);

			m_mapTempSaveData.insert(std::make_pair(nVariableID, pLoadSimpleVariable));

			if (m_nFunctionID < nVariableID)
			{
				m_nFunctionID = nVariableID;
			}

			TiXmlElement* pElementMode = oXmlManager.AccessElment(_T("Mode"), pElementVariable);
			while (nullptr != pElementMode)
			{
				int     nModeID       = oXmlManager.GetAttributeInt   (_T("ID"),           pElementMode);
				CString strFunction   = oXmlManager.GetAttributeStr   (_T("FunctionVal"),  pElementMode);
				double  dInclusiveVal = oXmlManager.GetAttributeDouble(_T("InclusiveVal"), pElementMode);

				KMode* pLoadMode = FindMode(nModeID);
				if (pLoadMode != nullptr)
				{
					KModeTreeNodeMode* pParentModeNode = pLoadModeTree->AddModeNode();
					pParentModeNode->Mode(pLoadMode);

					if (pElementMode->NoChildren())
					{
						KModeFunction func;
						func.SetCaptionExpression(strFunction);
						GenerateModeFunction(strFunction, func);
						if( func.GetExpressionList().size() > 0 )
						{
							pLoadSimpleVariable->ModeFunction(nModeID, &func);
						}
					}
					else
					{
						pLoadSimpleVariable->ModeParameter(nModeID, dInclusiveVal);
					}

					TiXmlElement* pElementChdMode = oXmlManager.AccessElment(_T("Mode"), pElementMode);
					while (nullptr != pElementChdMode)
					{
						nModeID       = oXmlManager.GetAttributeInt   (_T("ID"),           pElementChdMode);
						strFunction   = oXmlManager.GetAttributeStr   (_T("FunctionVal"),  pElementChdMode);
						dInclusiveVal = oXmlManager.GetAttributeDouble(_T("InclusiveVal"), pElementChdMode);

						KMode* pMode = FindMode(nModeID);
						if (pMode != nullptr)
						{
							 KModeTreeNodeMode* pChdModeNode = pParentModeNode->AddModeNode();
							 pChdModeNode->Mode(pMode);

							 KModeFunction func;
							 func.SetCaptionExpression(strFunction);
							 GenerateModeFunction(strFunction, func);
							 if( func.GetExpressionList().size() > 0 )
							 {
								 pLoadSimpleVariable->ModeFunction(nModeID, &func);
							 }
						}

						pElementChdMode = oXmlManager.NextElement(_T("Mode"), pElementChdMode);
					}
				}

				pElementMode = oXmlManager.NextElement(_T("Mode"), pElementMode);
			}

			pElementVariable = oXmlManager.NextElement(_T("Variable"), pElementVariable);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


KMode* KMDefineVariableEdit::FindMode(int a_nModeID)
{
	try
	{
		KModeManager*    pModeManager = m_pTarget->ModeManager();
		int nModeCount = pModeManager->GetModeCount();
		for(int i = 0; i < nModeCount; i++)
		{
			KMode* pMode = pModeManager->GetMode(i);
			if (pMode->ModeID() == a_nModeID)
			{
				return pMode;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return nullptr;
}


BOOL KMDefineVariableEdit::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	LoadXmlData();

	InitExpressionChecker();

	InitReportHeader();

	LoadInitialFunction();
	if (m_cboFunction.GetCount() > 0)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	}
	else
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	}

	ControlRadioFunction();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMDefineVariableEdit::InitReportHeader(void)
{
	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE);
	m_ctrlReport.SetTreeIndent(15);

	// Add Columns
	CXTPReportColumn* pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_NAME, _T("Group/Mode"), 23));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("결정수"));
	}

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_FUNCTION, _T("Utility Function"), 54));
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("효용함수"));
	}

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_PARAMETER, _T("Inclusive Value(1-α)"), 23));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("포괄가치 계수"));
	}
}


void KMDefineVariableEdit::UpdateReportData()
{
	m_ctrlReport.ResetContent();

	try
	{
		KMSimpleVariable* pSimpleVariable = nullptr;

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			int nCur = m_cboFunction.GetCurSel();
			if (nCur < 0)
				pSimpleVariable = nullptr;
			else
				pSimpleVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCur);
		}
		else
		{
			pSimpleVariable = nullptr;
		}

		std::queue<KModeTreeNodeMode*> modeQ;
		std::queue<CXTPReportRecord*>  recordQ;

		CXTPReportRecord*     pRootRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pRootItem   = pRootRecord->AddItem(new CXTPReportRecordItemText);
		pRootItem->SetCaption(_T("Decision Tree"));
		pRootRecord->AddItem(new CXTPReportRecordItemText);
		pRootRecord->AddItem(new CXTPReportRecordItemText);

		int nModeCount = m_pModeTree->ModeNodeCount();
		for(int i = 0; i < nModeCount; ++i)
		{
			KModeTreeNodeMode* pModeNode = m_pModeTree->ModeNode(i);
			modeQ.push(pModeNode);
			recordQ.push(pRootRecord);
		}

		while(modeQ.empty() == false)
		{
			ASSERT(modeQ.size() == recordQ.size());
			KModeTreeNodeMode* pModeNode     = modeQ.front();
			modeQ.pop();
			CXTPReportRecord*  pParentRecord = recordQ.front();
			recordQ.pop();

			CXTPReportRecord*     pRecord;
			CXTPReportRecordItem* pItem;
			pRecord = pParentRecord->GetChilds()->Add(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(pModeNode->ModeName());
			pItem->SetItemData((DWORD_PTR)pModeNode);

			// remain mode list에서 report에 추가된 mode를 삭제
			RemoveModeFromList(pModeNode->Mode());

			if(pModeNode->HasChild() == true)
			{
				double dParam(0.0);
				if (nullptr != pSimpleVariable)
				{
					dParam = pSimpleVariable->ModeParameter(pModeNode->ModeID());
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dParam, _T("%f")));

				int nChildCount = pModeNode->NodeCount();
				for(int i = 0; i < nChildCount; ++i)
				{
					KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pModeNode->Node(i));
					modeQ.push(pSubNode);
					recordQ.push(pRecord);
				}
			}
			else
			{
				CString strFunction(_T(""));
				if (nullptr != pSimpleVariable)
				{
					KModeFunction* pFunction = pSimpleVariable->ModeFunction(pModeNode->ModeID());
					if(NULL != pFunction)
					{
						strFunction = pFunction->GetCaptionExpression();
					}
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
	catch (...)
	{
		TxLogDebugException();	
	}

	m_ctrlReport.Populate();
	m_ctrlReport.ExpandAll(TRUE);
	m_ctrlReport.SetFocusedRow(m_ctrlReport.GetRows()->GetAt(0));
}


void KMDefineVariableEdit::RemoveModeFromList(KMode* pMode)
{
	std::list<KMode*>::iterator itMode, itEnd = m_RemainModeList.end();
	for(itMode = m_RemainModeList.begin(); itMode != itEnd; ++itMode)
	{
		KMode* pRemainMode = *itMode;
		if(pMode->ModeID() == pRemainMode->ModeID())
		{
			m_RemainModeList.erase(itMode);
			break;
		}
	}
}


void KMDefineVariableEdit::InitModeList()
{
	std::set<int> setCurShowModeID;
	try
	{
		int nModeCount = m_pModeTree->ModeNodeCount();
		for(int i = 0; i < nModeCount; ++i)
		{
			KModeTreeNodeMode* pModeNode = m_pModeTree->ModeNode(i);
			KMode*     pMode = pModeNode->Mode();
			int nID =  pMode->ModeID();

			setCurShowModeID.insert(nID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	try
	{
		m_RemainModeList.clear();

		KModeManager*  pModeManager = m_pTarget->ModeManager();
		ASSERT(NULL != pModeManager);
		int nModeCount = pModeManager->GetModeCount();
		for(int i = 0; i < nModeCount; ++i)
		{
			KMode* pMode    = pModeManager->GetMode(i);
			int    nModeID  = pMode->ModeID();
			
			AutoType itFind = setCurShowModeID.find(nModeID);
			if (itFind == setCurShowModeID.end())
			{
				m_RemainModeList.push_back(pMode);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::UpdateComboMode()
{
	try
	{
		if(m_cboMode.GetSafeHwnd() == NULL)
		{
			return;
		}

		m_cboMode.ResetContent();

		std::list<KMode*>::iterator itMode, itEnd = m_RemainModeList.end();
		for(itMode = m_RemainModeList.begin(); itMode != itEnd; ++itMode)
		{
			KMode* pMode = *itMode;
			int nIndex = m_cboMode.AddString(pMode->ModeName());
			m_cboMode.SetItemData(nIndex, (DWORD_PTR)pMode);
		}
		m_cboMode.SetCurSel(0);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::FunctionSelectChangeAction()
{
	int nCurSelectedIndex = -99;
	int nStyleID          = -99;

	if ( m_cboFunction.GetCount() != 0)
	{
		nCurSelectedIndex = m_cboFunction.GetCurSel();
		
		//nStyleID          = (int)m_cboFunction.GetItemData(nCurSelectedIndex);
		KMSimpleVariable* pEditSimpleVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCurSelectedIndex);
		nStyleID = pEditSimpleVariable->VariableID();
	}

// 	std::vector<TNodeTemplateValue> veTemplateValue;
// 
// 	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
// 	if (spDBaseConnection != NULL)
// 	{		
// 		KProjectDBase::NodeTemplateValue(spDBaseConnection, nStyleID, veTemplateValue);
// 	}
// 
// 	m_ctrlReport.ResetContent();
// 
// 	CXTPReportRecords*	  pRecords = NULL;
// 	CXTPReportRecord*     pRecord  = NULL;
// 	CXTPReportRecordItem* pItem    = NULL;
// 
// 	CXTPReportRecordItemNumber* pItemSize = NULL;
// 	CXTPReportRecordItemText*   pItemText = NULL;
// 
// 	int nTemplateCount = (int)veTemplateValue.size();
// 	for (int i=0; i<nTemplateCount; i++)
// 	{
// 		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
// 		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
// 		pItem->SetAlignment(DT_CENTER);
// 		pItem->SetEditable(FALSE);
// 		pItem->SetFont(m_pEsriDefaultMarkerFont);
// 		CString strSymbol;strSymbol.AppendChar(veTemplateValue[i].TSymbol);
// 		pItem->SetTextColor(RGB(veTemplateValue[i].TR, veTemplateValue[i].TG, veTemplateValue[i].TB));
// 		pItem->SetItemData(veTemplateValue[i].TSymbol);
// 		pItem->SetCaption(strSymbol);
// 
// 		pItemSize = (CXTPReportRecordItemNumber*)pRecord->AddItem(new CXTPReportRecordItemNumber(8.0, _T("%.1f")));         
// 		pItemSize->SetEditable(TRUE);
// 		pItemSize->SetAlignment(xtpColumnTextLeft);
// 
// 		pItemSize->SetValue(veTemplateValue[i].TSize);
// 		pItemSize->SetItemData((DWORD_PTR)veTemplateValue[i].TSize);
// 
// 		pItemText = (CXTPReportRecordItemText*)pRecord->AddItem(new CXTPReportRecordItemText());
// 
// 		CString strValue;
// 		strValue.Format(_T("%.1f"), veTemplateValue[i].TMax);
// 
// 		CString strCaption;
// 		strCaption.Format(_T("%.1f-%.1f"), veTemplateValue[i].TMin, veTemplateValue[i].TMax);
// 
// 		if (i==0)
// 		{
// 			pItemText->SetValue(strCaption);
// 		}
// 		else
// 		{
// 			pItemText->SetValue(strValue);
// 		}
// 
// 		pItemText->SetCaption(strCaption);
// 
// 		pItem = pRecord->AddItem(new CXTPReportRecordItem());
// 		pItem->SetEditable(TRUE);
// 		pItem->HasCheckbox(TRUE);
// 		pItem->SetChecked(FALSE);
// 		pItem->SetAlignment(xtpColumnIconCenter);
// 	}

	m_ctrlReport.Populate();
}


void KMDefineVariableEdit::OnBnClickedBtnModeAdd()
{
	try
	{
		int nSel = m_cboMode.GetCurSel();
		if (CB_ERR == nSel)
			return;

		AddNestedLogit();

		UpdateReportData();
		UpdateComboMode();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::AddNestedLogit(void)
{
	int nSel = m_cboMode.GetCurSel();
	if (CB_ERR == nSel)
		return;

	try
	{
		KMode* pMode = (KMode*)(m_cboMode.GetItemData(nSel));

		CXTPReportRow* pRow = m_ctrlReport.GetFocusedRow();
		if(nullptr == pRow)
		{
			return;
		}

		CXTPReportRecord*     pRecord     = pRow->GetRecord();
		CXTPReportRecordItem* pItem       = pRecord->GetItem(MVARIABLE_NAME);
		KModeTreeNodeMode*    pParentNode = (KModeTreeNodeMode*)(pItem->GetItemData());
		KModeTreeNodeMode*    pNode       = nullptr;
		if(NULL == pParentNode)
		{
			pNode = m_pModeTree->AddModeNode();
		}
		else
		{
			pNode = pParentNode->AddModeNode();
		}
		pNode->Mode(pMode);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnBnClickedBtnModeDel()
{
	try
	{
		RemoveNestedLogit();

		UpdateReportData();
		UpdateComboMode();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::RemoveNestedLogit(void)
{
	try
	{
		CXTPReportRow* pRow = m_ctrlReport.GetFocusedRow();
		if(nullptr == pRow)
			return;

		CXTPReportRecord*     pRecord     = pRow->GetRecord();
		CXTPReportRecordItem* pItem       = pRecord->GetItem(MVARIABLE_NAME);
		KModeTreeNodeMode*    pRemoveNode = (KModeTreeNodeMode*)(pItem->GetItemData());
		if(NULL == pRemoveNode)
		{
			return;
		}

		std::queue<KModeTreeNodeMode*> nodeQ;
		nodeQ.push(pRemoveNode);
		while(nodeQ.empty() == false)
		{
			KModeTreeNodeMode* pNode = nodeQ.front();
			nodeQ.pop();
			KMode* pMode     = pNode->Mode();
			m_RemainModeList.push_back(pMode);
			int nNodeCount = pNode->NodeCount();
			for(int i = 0; i < nNodeCount; ++i)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(i));
				nodeQ.push(pSubNode);
			}
		}

		CXTPReportRecord*     pParentRecord = pRecord->GetParentRecord();
		CXTPReportRecordItem* pParentItem   = pParentRecord->GetItem(MVARIABLE_NAME);
		KModeTreeNodeMode* pParentNode = (KModeTreeNodeMode*)(pParentItem->GetItemData());
		if(nullptr == pParentNode)
		{
			m_pModeTree->RemoveModeNode(pRemoveNode);
		}
		else
		{
			pParentNode->RemoveNode(pRemoveNode);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnBnClickedBtnRemoveall()
{
	try
	{
		if (nullptr != m_pModeTree)
		{
			QBicDelete(m_pModeTree);
		}
		m_pModeTree = new KModeTree;

		InitModeList();
		UpdateReportData();
		UpdateComboMode();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateModeButtonState();
}


void KMDefineVariableEdit::UpdateModeButtonState(void)
{
	try
	{
		BOOL bEnableAdd(FALSE);
		BOOL bEnableDel(FALSE);

		CXTPReportRow* pRow = m_ctrlReport.GetFocusedRow();
		if(nullptr == pRow)
		{
			bEnableAdd = FALSE;
			bEnableDel = FALSE;
		}
		else
		{
			CXTPReportRecord*     pRecord = pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(MVARIABLE_NAME);

			CXTPReportRecord* pParentRecord = pRecord->GetParentRecord();
			if(NULL == pParentRecord)
			{
				bEnableAdd = TRUE;
				bEnableDel = FALSE;
			}
			else
			{
				CXTPReportRecordItem* pParentItem = pParentRecord->GetItem(MVARIABLE_NAME);
				if(pParentItem->GetItemData() == NULL)
					bEnableAdd = TRUE;
				else
					bEnableAdd = FALSE;
				
				bEnableDel = TRUE;
			}
		}

		GetDlgItem(IDC_BTN_MODE_ADD)->EnableWindow(bEnableAdd);
		GetDlgItem(IDC_BTN_MODE_DEL)->EnableWindow(bEnableDel);
	}
	catch (...)
	{
		GetDlgItem(IDC_BTN_MODE_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MODE_DEL)->EnableWindow(FALSE);

		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	PostMessage(UM_FUNCTIONEDIT, (WPARAM)(pItemNotify->pItem));
}


LRESULT KMDefineVariableEdit::OnFunctionEdit(WPARAM wParam, LPARAM lParam)
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
		m_ctrlReport.RedrawControl();
	}

	return 0;
}


void KMDefineVariableEdit::GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function)
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

			CString strFieldName = GetFieldName(strToken.c_str());
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


CString KMDefineVariableEdit::GetFieldName(LPCTSTR strCaption)
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


bool KMDefineVariableEdit::InvalidateInput()
{
	try
	{
		if( m_pModeTree->ModeNodeCount()< 1)
		{
			CString strCaption(_T(""));
			GetDlgItemText(IDC_BTN_MODE_ADD, strCaption);

			CString strInputDataCheckMsg = _T("");
			strInputDataCheckMsg.Format(_T("Tree에 수단 관련 정보가 존재하지 않습니다.\r\n%s버튼을 클릭하여 설정해 주세요."), strCaption);
			AfxMessageBox(strInputDataCheckMsg);

			return false;
		}

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			int nCur = m_cboFunction.GetCurSel();
			if (nCur < 0)
			{
				CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
				ControlRadioFunction();

				GetDlgItem(IDC_EDIT_FUNC_NAME)->SetFocus();
				return false;
			}
		}
		else
		{
			CString strNewVariableName(_T(""));
			GetDlgItemText(IDC_EDIT_FUNC_NAME, strNewVariableName);
			strNewVariableName.Trim();
			SetDlgItemText(IDC_EDIT_FUNC_NAME, strNewVariableName);

			if (strNewVariableName.IsEmpty())
			{
				AfxMessageBox(_T("이름을 지정해 주세요."));
				GetDlgItem(IDC_EDIT_FUNC_NAME)->SetFocus();
				return false;
			}
		}


		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		if (nRecordCount != 1)
		{
			return false;
		}

		CXTPReportRecord*  pRootRecord = pRecords->GetAt(0);
		CXTPReportRecords* pModeRecords = pRootRecord->GetChilds();
		nRecordCount = pModeRecords->GetCount();

		std::queue<CXTPReportRecord*> recordQ;
		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pRecord = pModeRecords->GetAt(i);
			recordQ.push(pRecord);
		}

		while(recordQ.empty() == false)
		{
			CXTPReportRecord* pRecord = recordQ.front();
			recordQ.pop();

			CXTPReportRecordItem* pItem       = pRecord->GetItem(MVARIABLE_NAME);
			//KModeTreeNodeMode*    pModeNode   = (KModeTreeNodeMode*)pItem->GetItemData();
			CXTPReportRecords*    pSubRecords = pRecord->GetChilds();

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
				CXTPReportRecordItem* pFunctionItem = pRecord->GetItem(MVARIABLE_FUNCTION);
				CString strFunction(pFunctionItem->GetCaption(NULL));

				KModeFunction func;
				func.SetCaptionExpression(strFunction);

				GenerateModeFunction(strFunction, func);

				if( func.GetExpressionList().size() < 1 )
				{
					CString strInputDataCheckMsg = _T("");
					strInputDataCheckMsg.Format(_T("%s에 대한 수식 관련 정보가 존재하지 않습니다."), pItem->GetCaption(NULL));
					AfxMessageBox(strInputDataCheckMsg);

					CXTPReportRow* pRow = m_ctrlReport.GetRows()->Find(pFunctionItem->GetRecord());
					if(NULL != pRow)
					{
						pRow->SetSelected(TRUE);
						m_ctrlReport.SetFocusedRow(pRow);
						m_ctrlReport.SetTopRow(pRow->GetIndex());
					}

					return false;
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KMDefineVariableEdit::OnBnClickedBtnApply()
{
	try
	{
		if (!InvalidateInput())
		{
			return;
		}

		KMSimpleVariable* pSimpleVariable = nullptr;

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			int nCur = m_cboFunction.GetCurSel();

			KMSimpleVariable* pEditSimpleVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCur);
			KModeTree*        pEditModeTree       = pEditSimpleVariable->ModeTree();
			
			*pEditModeTree = *m_pModeTree;

			m_nPreSelectedVariableID = pEditSimpleVariable->VariableID();

			pSimpleVariable = pEditSimpleVariable;
		}
		else
		{
			CString strNewVariableName(_T(""));
			GetDlgItemText(IDC_EDIT_FUNC_NAME, strNewVariableName);

			KMSimpleVariable* pNewSimpleVariable = new KMSimpleVariable;
			KModeTree*        pNewModeTree    = new KModeTree;

			*pNewModeTree = *m_pModeTree;

			int nNewID = ++m_nFunctionID;
			pNewSimpleVariable->ModeTree(pNewModeTree);
			pNewSimpleVariable->VariableID(nNewID);
			pNewSimpleVariable->VariableName(strNewVariableName);

			m_mapTempSaveData.insert(std::make_pair(nNewID, pNewSimpleVariable));

			m_nPreSelectedVariableID = nNewID;

			pSimpleVariable = pNewSimpleVariable;
		}

		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			if( false == ApplySimpleVariable(pRecord, pSimpleVariable) )
			{
				return;
			}
		}

		SaveXmlData();
		LoadInitialFunction();

		AfxMessageBox(_T("적용되었습니다."));

		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		ControlRadioFunction();
		SetDlgItemText(IDC_EDIT_FUNC_NAME, _T(""));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KMDefineVariableEdit::ApplySimpleVariable(CXTPReportRecord* a_pRecordRoot, KMSimpleVariable* a_pSaveSimpleVariable)
{
	a_pSaveSimpleVariable->RemoveAllModeFunction();
	a_pSaveSimpleVariable->RemoveAllModeParameter();

	std::queue<CXTPReportRecord*> recordQ;
	CXTPReportRecords* pRecords = a_pRecordRoot->GetChilds();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		recordQ.push(pRecord);
	}

	while(recordQ.empty() == false)
	{
		CXTPReportRecord* pRecord = recordQ.front();
		recordQ.pop();

		CXTPReportRecordItem* pItem       = pRecord->GetItem(MVARIABLE_NAME);
		KModeTreeNodeMode*    pModeNode   = (KModeTreeNodeMode*)pItem->GetItemData();
		int nModeID         = pModeNode->ModeID();
		CXTPReportRecords*    pSubRecords = pRecord->GetChilds();

		int nSubRecordCount = pSubRecords->GetCount();
		if(0 < nSubRecordCount)
		{
			CXTPReportRecordItemNumber* pNumberItem = 
				(CXTPReportRecordItemNumber*)(pRecord->GetItem(MVARIABLE_PARAMETER));
			double dParam = pNumberItem->GetValue();
			a_pSaveSimpleVariable->ModeParameter(nModeID, dParam);

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

			GenerateModeFunction(strFunction, func);

			if( func.GetExpressionList().size() > 0 )
			{
				a_pSaveSimpleVariable->ModeFunction(nModeID, &func);
			}
		}
	}

	return true;
}


void KMDefineVariableEdit::LoadInitialFunction()
{
	//LoadXmlData();

	m_cboFunction.ResetContent();

	int nPreSelectedIndex = -1;

	AutoType iter  = m_mapTempSaveData.begin();
	AutoType itEnd = m_mapTempSaveData.end();

	while (iter != itEnd)
	{
		KMSimpleVariable* pVariable = iter->second;
	
		int     nID = pVariable->VariableID();
		CString strName(_T(""));
		strName.Format(_T("%d:%s"), nID, pVariable->VariableName());

		int nIndex = m_cboFunction.AddString(strName);
		m_cboFunction.SetItemData(nIndex, (DWORD_PTR) pVariable);

		if (nID == m_nPreSelectedVariableID)
		{
			nPreSelectedIndex = nIndex;
		}

		++iter;
	}

	if (m_cboFunction.GetCount() > 0)
	{
		if (nPreSelectedIndex >= 0)
		{
			m_cboFunction.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboFunction.SetCurSel(0);
		}			
	}
}


void KMDefineVariableEdit::SaveXmlData()
{
	try
	{
		KProject* pProject = ImChampFrameWindow::GetProject();
		CString   strDir   = pProject->GetLocation();
		strDir.AppendFormat(_T("\\%s\\"),  KDefaultParaFolder::FOLDER_PARAMETER);

		CFileFind odirFind;
		if (odirFind.FindFile(strDir) == 0)
		{
			CreateDirectory(strDir, NULL);
		}

		KXmlManager oXmlManager;
		if ( !oXmlManager.LoadTargetXml(strDir, KDefaultParaFile::FILE_MODECHOICE_VAR))
		{
			oXmlManager.CreateDeclaration(_T("1.0"), _T("euc-kr"), _T(""));
		}
		 
		TiXmlElement* pRootElement     = oXmlManager.CheckRootElement(_T("ModeChoice-Variable-Default"));
		pRootElement->Clear();

		AutoType iter  = m_mapTempSaveData.begin();
		AutoType itEnd = m_mapTempSaveData.end();

		while (iter != itEnd)
		{
			KMSimpleVariable* pSimpleVariable  = iter->second;
			KModeTree*        pModifyModeTree  = pSimpleVariable->ModeTree();
			
			TiXmlElement* pElementVariable = oXmlManager.CreateChildElement(_T("Variable"), pRootElement);

			oXmlManager.SetAttributeInt   (_T("ID"),    pElementVariable,  pSimpleVariable->VariableID());
			oXmlManager.SetAttributeString(_T("Name"),  pElementVariable,  pSimpleVariable->VariableName());

			std::queue<KModeTreeNodeMode*> modeQ;
			std::queue<TiXmlElement*>      elementQ;

			int nModeCount = pModifyModeTree->ModeNodeCount();
			for(int i = 0; i < nModeCount; ++i)
			{
				KModeTreeNodeMode* pModeNode = pModifyModeTree->ModeNode(i);
				modeQ.push(pModeNode);
				elementQ.push(pElementVariable);
			}

			while(modeQ.empty() == false)
			{
				ASSERT(modeQ.size() == elementQ.size());
				KModeTreeNodeMode* pModeNode     = modeQ.front();
				modeQ.pop();

				TiXmlElement* pParentElement = elementQ.front();
				elementQ.pop();

				TiXmlElement*     pElement = oXmlManager.CreateChildElement(_T("Mode"), pParentElement);

				pModeNode->ModeName();
				pModeNode->Mode();

				if(pModeNode->HasChild() == true)
				{
					double dParam(0.0);
					if (nullptr != pSimpleVariable)
					{
						dParam = pSimpleVariable->ModeParameter(pModeNode->ModeID());
					}
					
					oXmlManager.SetAttributeInt   (_T("ID"),           pElement,  pModeNode->ModeID());
					oXmlManager.SetAttributeString(_T("FunctionVal"),  pElement,  _T(""));
					oXmlManager.SetAttributeDouble(_T("InclusiveVal"), pElement,  dParam);

					int nChildCount = pModeNode->NodeCount();
					for(int i = 0; i < nChildCount; ++i)
					{
						KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pModeNode->Node(i));
						modeQ.push(pSubNode);
						elementQ.push(pElement);
					}
				}
				else
				{
					CString strFunction(_T(""));
					if (nullptr != pSimpleVariable)
					{
						KModeFunction* pFunction = pSimpleVariable->ModeFunction(pModeNode->ModeID());
						if(NULL != pFunction)
						{
							strFunction = pFunction->GetCaptionExpression();
						}
					}

					oXmlManager.SetAttributeInt   (_T("ID"),           pElement,  pModeNode->ModeID());
					oXmlManager.SetAttributeString(_T("FunctionVal"),  pElement,  strFunction);
					oXmlManager.SetAttributeDouble(_T("InclusiveVal"), pElement,  0.0);
				}
			}

			++iter;
		}

		bool bResult = oXmlManager.SaveTargetXml(strDir, KDefaultParaFile::FILE_MODECHOICE_VAR);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnBnClickedRadioFunction(UINT nID)
{
	ControlRadioFunction(); 
}


void KMDefineVariableEdit::ControlRadioFunction()
{
	try
	{
		if (nullptr != m_pModeTree)
		{
			QBicDelete(m_pModeTree);
		}
		m_pModeTree = new KModeTree;

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			GetDlgItem(IDC_BUTTON_FUNC_DELETE)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_FUNC)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_FUNC_NAME)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_FUNC_DELETE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_FUNC)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FUNC_NAME)->EnableWindow(TRUE);
		}

		int nCur = m_cboFunction.GetCurSel();
		if (nCur < 0)
		{
		}
		else
		{
			KMSimpleVariable* pEditSimpleVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCur);
			KModeTree*        pModifyModeTree     = pEditSimpleVariable->ModeTree();
			if (nullptr != pModifyModeTree)
			{
				*m_pModeTree = *pModifyModeTree;
			}
		}

		InitModeList();
		UpdateReportData();
		UpdateComboMode();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   
}

void KMDefineVariableEdit::OnBnClickedButtonFuncDelete()
{
	try
	{
		int nCur = m_cboFunction.GetCurSel();
		if (nCur < 0)
		{
			return;
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
				return;
		}
		else {
			if (AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO) == IDNO)
				return;
		}
		//if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
		//{
		//	return;
		//}

		KMSimpleVariable* pSimpleVariable = (KMSimpleVariable* )m_cboFunction.GetItemData(nCur);
		if (nullptr != pSimpleVariable)
		{
			int nVariableID = pSimpleVariable->VariableID();

			KModeTree*    pDelModeTree    = pSimpleVariable->ModeTree();
			if (nullptr != pDelModeTree)
			{
				QBicDelete(pDelModeTree);
			}

			QBicDelete(pSimpleVariable);

			AutoType itFind = m_mapTempSaveData.find(nVariableID);
			if (itFind != m_mapTempSaveData.end())
			{
				m_mapTempSaveData.erase(itFind);
				m_nPreSelectedVariableID = -1;
			}
		}
		
		SaveXmlData();
		LoadInitialFunction();
		ControlRadioFunction(); 
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableEdit::OnCbnSelchangeComboFunc()
{
	ControlRadioFunction();
}
