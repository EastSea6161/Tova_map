// MDefineVariableGroupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "MDefineVariableGroupDlg.h"
#include "afxdialogex.h"

//#include "CodeGroup.h"
//#include "CodeManager.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "MGroupVariable.h"
#include "ModeFunction.h"
#include "ModeManager.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "ExpressionChecker.h"
#include "MSimpleVariable.h"
#include "Purpose.h"
#include "Mode.h"
#include "Target.h"
// #include "ImTasDBase.h"
// #include "DBaseConnector.h"
#include "MDefineVariableEdit.h"
#include "DBaseConnector.h"

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"
#include "ImChampFrameWindow.h"
#include "DefineNamesDefaultPara.h"

/* Report column index define */
#define MVARIABLE_NAME			0
#define MVARIABLE_FUNCTION		1
#define MVARIABLE_PARAMETER		2

// KMDefineVariableGroupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMDefineVariableGroupDlg, KDialogEx)

	KMDefineVariableGroupDlg::KMDefineVariableGroupDlg(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup, CWnd* pParent /*=NULL*/)
	: KDialogEx(KMDefineVariableGroupDlg::IDD, pParent)
	, m_mapDefaultODGroup(a_mapDefaultODGroup)
{
	m_pExpressionChecker = new KExpressionChecker;
}

KMDefineVariableGroupDlg::~KMDefineVariableGroupDlg()
{
	if (nullptr != m_pExpressionChecker)
	{
		QBicDelete(m_pExpressionChecker);
	}

	DeleteTempSaveData();

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KMBaseVariable* pVariable = itVariable->second;
		delete pVariable;
	}
	m_VariableMap.clear();
}

void KMDefineVariableGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cboFunction);
}


BEGIN_MESSAGE_MAP(KMDefineVariableGroupDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KMDefineVariableGroupDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KMDefineVariableGroupDlg::OnSelchangeTabObject)
	ON_BN_CLICKED(IDC_BTN_EDIT, &KMDefineVariableGroupDlg::OnBnClickedBtnEdit)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &KMDefineVariableGroupDlg::OnCbnSelchangeComboFunc)
END_MESSAGE_MAP()


void KMDefineVariableGroupDlg::Target(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KMDefineVariableGroupDlg::AddModeChoiceInfo(KModeChoiceInfo* pMCInfo)
{
	m_VariableMap[pMCInfo] = NULL;
}


void KMDefineVariableGroupDlg::DeleteTempSaveData()
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


void KMDefineVariableGroupDlg::updateVariableMap(void)
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

		KMSimpleVariable* pSimpleVariable = new KMSimpleVariable;
		if((NULL != pInfoVariable) && (pInfoVariable->ModeVariableType() == KEMModeVariableTypeSimple))
		{
			*pSimpleVariable = *(KMSimpleVariable*)pInfoVariable;
		}
		pSimpleVariable->ModeTree(pMCInfo->ModeTree());
		itVariable->second = pSimpleVariable;
	}
}


void KMDefineVariableGroupDlg::InitExpressionChecker(void)
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


void KMDefineVariableGroupDlg::initReportControl(void)
{
	KReportCtrlSetting::Default(m_ctrlReport, FALSE, FALSE);
	m_ctrlReport.SetTreeIndent(15);

	// Add Columns
	CXTPReportColumn* pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_NAME, _T("Group/Mode"), 25));
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("결정수"));
    }

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_FUNCTION, _T("Utility Function"), 48));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("효용함수"));
    }

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(MVARIABLE_PARAMETER, _T("Inclusive Value(1-α)"), 27));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("포괄가치 계수"));
    }
}


void KMDefineVariableGroupDlg::UpdateReportRecord(void)
{
	m_ctrlReport.ResetContent();

	try
	{
		int nCur = m_cboFunction.GetCurSel();
		if (nCur < 0)
		{
			return;
		}

		KMSimpleVariable* pSimpleVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCur);
		KModeTree*        pModeTree       = pSimpleVariable->ModeTree();

		std::queue<KModeTreeNodeMode*> modeQ;
		std::queue<CXTPReportRecord*>  recordQ;

		CXTPReportRecord*     pRootRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pRootItem   = pRootRecord->AddItem(new CXTPReportRecordItemText);
		pRootItem->SetCaption(_T("Decision Tree"));
		pRootRecord->AddItem(new CXTPReportRecordItemText);
		pRootRecord->AddItem(new CXTPReportRecordItemText);

		int nModeCount = pModeTree->ModeNodeCount();
		for(int i = 0; i < nModeCount; ++i)
		{
			KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
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
				pItem->SetTooltip(strFunction);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	m_ctrlReport.Populate();
	m_ctrlReport.ExpandAll(TRUE);
}

// KMDefineVariableGroupDlg 메시지 처리기입니다.


void KMDefineVariableGroupDlg::LoadXmlDataModeChoiceVarDefault()
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


KMode* KMDefineVariableGroupDlg::FindMode(int a_nModeID)
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


void KMDefineVariableGroupDlg::GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function)
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


CString KMDefineVariableGroupDlg::GetFieldName(LPCTSTR strCaption)
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


void KMDefineVariableGroupDlg::LoadInitialFunction(int a_nPreVariableID)
{
	//LoadXmlData();

	m_mapFunctionInfo.clear();
	m_cboFunction.ResetContent();

	AutoType iter  = m_mapTempSaveData.begin();
	AutoType itEnd = m_mapTempSaveData.end();

	int nInitIndex(0);
	while (iter != itEnd)
	{
		KMSimpleVariable* pVariable = iter->second;

		int     nID = pVariable->VariableID();
		CString strName(_T(""));
		strName.Format(_T("%d:%s"), nID, pVariable->VariableName());

		int nIndex = m_cboFunction.AddString(strName);
		m_cboFunction.SetItemData(nIndex, (DWORD_PTR) pVariable);

		m_mapFunctionInfo.insert(std::make_pair(nID, strName));

		if (a_nPreVariableID == nID)
		{
			nInitIndex = nIndex;
		}

		++iter;
	}

	if (m_cboFunction.GetCount() > 0)
	{
		m_cboFunction.SetCurSel(nInitIndex);
	}
}


BOOL KMDefineVariableGroupDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitExpressionChecker();

	LoadXmlDataModeChoiceVarDefault();
	LoadInitialFunction();

	updateVariableMap();

	initReportControl();
	UpdateReportRecord();

	CreateChildPage();
	DisplayTabControl(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMDefineVariableGroupDlg::OnBnClickedOk()
{
	if (!InvalidateInputInfo())
	{
		return;
	}

	GetResultInfo(m_mapDefaultODGroup);

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KModeChoiceInfo* pInfo = itVariable->first;
		KMBaseVariable* pVariable = itVariable->second;
		pInfo->Variable(pVariable);
	}

	KDialogEx::OnOK();
}


bool KMDefineVariableGroupDlg::InvalidateInputInfo()
{
	try
	{
		if (m_mapFunctionInfo.size() < 1)
		{
			AfxMessageBox(_T("수식이 정의되어 있지 않습니다.\r\n편집 버튼을 클릭하여 수식을 추가해 주세요."));
			return false;
		}

		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KMDefVarSubPagePtr spDefVarSubPage = iter->second;

			if (!spDefVarSubPage->InvalidateInputInfo())
			{
				int nTabIndex = spDefVarSubPage->GetPageID();
				m_ctrlTab.SetCurFocus(nTabIndex);
				return false;
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KMDefineVariableGroupDlg::CreateChildPage()
{
	try
	{
		int nTabIndex(0);

		std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
		for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
		{
			KModeChoiceInfo*  pMCInfo  = itVariable->first;
			KPurpose*         pPurpose = pMCInfo->Purpose();
			CString strName = pPurpose->PurposeName();
			int nPurposeID  = pPurpose->PurposeID();

			CRect oTabRect;
			m_ctrlTab.GetClientRect(&oTabRect);

			TDefVarDefaultODGroup oTDefVarDefaultODGroup;

			AutoType itFind = m_mapDefaultODGroup.find(nPurposeID);
			if (itFind != m_mapDefaultODGroup.end())
			{
				oTDefVarDefaultODGroup = itFind->second;
			}
			else
			{
				oTDefVarDefaultODGroup.bUseODGroup  = false;
				oTDefVarDefaultODGroup.nODGroupCode = -1;
				oTDefVarDefaultODGroup.nPurposeID = nPurposeID;
			}

			KMDefVarSubPagePtr spSubPage = KMDefVarSubPagePtr(new KMDefVarSubPage(nTabIndex, nPurposeID, strName, oTDefVarDefaultODGroup, m_pTarget, m_mapFunctionInfo));
			spSubPage->Create(KMDefVarSubPage::IDD, &m_ctrlTab);
			spSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

			m_ctrlTab.InsertItem(nTabIndex, strName);
			m_mapSubPage.insert(std::make_pair(nTabIndex, spSubPage));

			nTabIndex++;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableGroupDlg::DisplayTabControl( int a_nTabIndex )
{
	try
	{
		if (nullptr != m_spCurShowDefVarSubPage)
		{
			m_spCurShowDefVarSubPage->ShowWindow(SW_HIDE);
		}

		AutoType iter = m_mapSubPage.find(a_nTabIndex);
		if (iter != m_mapSubPage.end())
		{
			m_spCurShowDefVarSubPage = iter->second;
			m_spCurShowDefVarSubPage->ShowWindow(SW_SHOW);
			SubPageRedrawWindow();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableGroupDlg::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_spCurShowDefVarSubPage)
		{        
			CRect oTabRect;
			m_ctrlTab.GetClientRect(&oTabRect);	

			m_spCurShowDefVarSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableGroupDlg::OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	DisplayTabControl(m_ctrlTab.GetCurSel());
	*pResult = 0;
}


void KMDefineVariableGroupDlg::OnBnClickedBtnEdit()
{
	try
	{
		int nVariableID(-1);
		int nCur = m_cboFunction.GetCurSel();
		if (nCur < 0)
		{
			nVariableID = -1;
		}
		else
		{
			KMSimpleVariable* pVariable = (KMSimpleVariable*)m_cboFunction.GetItemData(nCur);
			nVariableID = pVariable->VariableID();
		}

		KMDefineVariableEdit oDlg(nVariableID, m_pTarget, this);
		oDlg.DoModal();

		LoadXmlDataModeChoiceVarDefault();
		LoadInitialFunction(nVariableID);
		UpdateReportRecord();

		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KMDefVarSubPagePtr spDefVarSubPage = iter->second;
			spDefVarSubPage->UpdateFunctionInfo();

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableGroupDlg::OnCbnSelchangeComboFunc()
{
	UpdateReportRecord();
}


void KMDefineVariableGroupDlg::UsedFunctionIDSet( std::set<int> &a_setUsedFunctionID )
{
	try
	{
		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KMDefVarSubPagePtr spDefVarSubPage = iter->second;
			spDefVarSubPage->UsedFunctionIDSet(a_setUsedFunctionID);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefineVariableGroupDlg::GetResultInfo(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup)
{
	try
	{
		a_mapDefaultODGroup.clear();

		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KMDefVarSubPagePtr spDefVarSubPage = iter->second;

			TDefVarDefaultODGroup oTDefVarGroupResultInfo;
			spDefVarSubPage->GetResultInfo(oTDefVarGroupResultInfo);

			a_mapDefaultODGroup.insert(std::make_pair(oTDefVarGroupResultInfo.nPurposeID, oTDefVarGroupResultInfo));

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
