// DefaultMDefineVariableGroupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "DefaultMDefineVariableGroupDlg.h"
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
#include "PurposeInfoCollection.h"

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"
#include "ImChampFrameWindow.h"
#include "DefineNamesDefaultPara.h"

/* Report column index define */
#define MVARIABLE_NAME			0
#define MVARIABLE_FUNCTION		1
#define MVARIABLE_PARAMETER		2

// KDefaultMDefineVariableGroupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultMDefineVariableGroupDlg, KDialogEx)

KDefaultMDefineVariableGroupDlg::KDefaultMDefineVariableGroupDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDefaultMDefineVariableGroupDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_pExpressionChecker = new KExpressionChecker;

	PrepareData();
}

KDefaultMDefineVariableGroupDlg::~KDefaultMDefineVariableGroupDlg()
{
	DeleteTempSaveData();

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator>::iterator itVariable, itEnd = m_VariableMap.end();
	for(itVariable = m_VariableMap.begin(); itVariable != itEnd; ++itVariable)
	{
		KMBaseVariable* pVariable = itVariable->second;
		delete pVariable;
	}
	m_VariableMap.clear();

	ClearMInfoList();
	ClearPurposeSet();

	if (nullptr != m_pExpressionChecker)
	{
		QBicDelete(m_pExpressionChecker);
	}
}

void KDefaultMDefineVariableGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cboFunction);
}


BEGIN_MESSAGE_MAP(KDefaultMDefineVariableGroupDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KDefaultMDefineVariableGroupDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KDefaultMDefineVariableGroupDlg::OnSelchangeTabObject)
	ON_BN_CLICKED(IDC_BTN_EDIT, &KDefaultMDefineVariableGroupDlg::OnBnClickedBtnEdit)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &KDefaultMDefineVariableGroupDlg::OnCbnSelchangeComboFunc)
	ON_BN_CLICKED(IDC_BTN_INIT, &KDefaultMDefineVariableGroupDlg::OnBnClickedBtnInit)
END_MESSAGE_MAP()


void KDefaultMDefineVariableGroupDlg::PrepareData()
{
	InitPurposeSet();

	try
	{
		ClearMInfoList();
		std::list<KPurpose*>::iterator itInfo, itEnd = m_lstPurpose.end();
		for(itInfo = m_lstPurpose.begin(); itInfo != itEnd; ++itInfo)
		{
			KPurpose* pPurpose = *itInfo;
			KModeChoiceInfo* pInfo = new KModeChoiceInfo;
			pInfo->Purpose(pPurpose);
			m_MInfoVector.push_back(pInfo);
		}

		std::vector<KModeChoiceInfo*>::iterator iterB, itEndB = m_MInfoVector.end();
		for(iterB = m_MInfoVector.begin(); iterB != itEndB; ++iterB)
		{
			KModeChoiceInfo* pInfo = *iterB;
			m_VariableMap[pInfo] = NULL;
		}

		LoadDefaultPurpoeGroupODInfo();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultMDefineVariableGroupDlg::InitPurposeSet(void)
{
    TxLogDebugStartMsg();
	ClearPurposeSet();

	try
	{
		KPurposeInfoCollection purposeInfos(m_pTarget);

		std::list<KPurpose*>::iterator iter = purposeInfos.begin();
		std::list<KPurpose*>::iterator end  = purposeInfos.end();

		while(iter != end)
		{
			KPurpose* pPurpose = *iter;
			KPurpose* pNewPurpose = new KPurpose( pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName() );
			m_lstPurpose.push_back(pNewPurpose);
			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
    TxLogDebugEndMsg();
}


void KDefaultMDefineVariableGroupDlg::LoadDefaultPurpoeGroupODInfo()
{
	m_mapDefaultODGroup.clear();

	std::map<int, TDefVarDefaultODGroup> mapDefVarDefaultInfo;
	try
	{
		CString strSQL(_T(" SELECT purpose_id, use_odgroup, KCode_Group_Key, none_function_id from Default_ModeChoice_DefineVal_Info "));

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			TDefVarDefaultODGroup oTDefVarDefaultODGroup;

			oTDefVarDefaultODGroup.nPurposeID = spResultset->GetValueInt(0);

			int nUseODGroup                   = spResultset->GetValueInt(1);
			if (1==nUseODGroup)
				oTDefVarDefaultODGroup.bUseODGroup = true;
			else
				oTDefVarDefaultODGroup.bUseODGroup = false;

			oTDefVarDefaultODGroup.nODGroupCode = spResultset->GetValueInt(2);
			oTDefVarDefaultODGroup.nNoneGroupFunctionID = spResultset->GetValueInt(3);

			mapDefVarDefaultInfo.insert(std::make_pair(oTDefVarDefaultODGroup.nPurposeID, oTDefVarDefaultODGroup));
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

	try
	{
		size_t nxSize = m_MInfoVector.size();
		for (size_t i= 0; i< nxSize; i++)
		{
			KModeChoiceInfo*  pMCInfo  = m_MInfoVector[i];
			KPurpose*         pPurpose = pMCInfo->Purpose();
			int nPurposeID  = pPurpose->PurposeID();

			TDefVarDefaultODGroup oTDefVarDefaultODGroup;

			AutoType iter  = mapDefVarDefaultInfo.find(nPurposeID);
			if (iter != mapDefVarDefaultInfo.end())
			{
				oTDefVarDefaultODGroup = iter->second;
				if (oTDefVarDefaultODGroup.bUseODGroup)
				{
					LoadODGroupDefineVariableID(oTDefVarDefaultODGroup.nODGroupCode, nPurposeID, oTDefVarDefaultODGroup.mapFunctionID);
				}
			}
			else
			{
				oTDefVarDefaultODGroup.bUseODGroup  = false;
				oTDefVarDefaultODGroup.nODGroupCode = -1;
				oTDefVarDefaultODGroup.nNoneGroupFunctionID = -1;
			}

			m_mapDefaultODGroup.insert(std::make_pair(nPurposeID, oTDefVarDefaultODGroup));

			// 주의 사항이 있음 m_mapDefaultODGroup 에  Default 내지 저장되어있는 DB에서 받아 올것인데  m_VariableMap에 있는 Purpose 정보가 전부 들어 있어야 함
			// m_mapDefaultODGroup에 해당 Purpose가 없으면 TDefVarDefaultODGroup 를 None Group으로 셋팅 하여 추가해줘야 한다.
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultMDefineVariableGroupDlg::LoadODGroupDefineVariableID(int a_nGroupKey, int a_nPurposeID, std::map<int, int> &a_mapODGroupSavedFunctionID)
{
	try
	{
		a_mapODGroupSavedFunctionID.clear();	

		CString strSQL(_T(""));
		strSQL.Append      (_T(" SELECT KCode_Key, function_id FROM Default_ModeChoice_Define_Value "));
		strSQL.AppendFormat(_T(" WHERE purpose_id = %d AND  KCode_Group_Key = %d "), a_nPurposeID, a_nGroupKey);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		try
		{
			KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
			while (spResultSet->Next())
			{
				int nCodeKey    = spResultSet->GetValueInt(0);
				int nFunctionID = spResultSet->GetValueInt(1);

				a_mapODGroupSavedFunctionID.insert(std::make_pair(nCodeKey, nFunctionID));
			}
		}
		catch (...)
		{
			TxLogDebugException();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapODGroupSavedFunctionID.clear();	
	}
}


void KDefaultMDefineVariableGroupDlg::ClearPurposeSet(void)
{
    TxLogDebugStartMsg();

	try
	{
		std::list<KPurpose*>::iterator itInfo, itEnd = m_lstPurpose.end();
		for(itInfo = m_lstPurpose.begin(); itInfo != itEnd; ++itInfo)
		{
			KPurpose* pPurpose = *itInfo;
			delete pPurpose;
		}
		m_lstPurpose.clear();	
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	TxLogDebugEndMsg();
}


void KDefaultMDefineVariableGroupDlg::ClearMInfoList(void)
{
	TxLogDebugStartMsg();
	std::vector<KModeChoiceInfo*>::iterator itMInfo, itEnd = m_MInfoVector.end();
	for(itMInfo = m_MInfoVector.begin(); itMInfo != itEnd; ++itMInfo)
	{
		KModeChoiceInfo* pInfo = *itMInfo;
		delete pInfo;
	}

	m_MInfoVector.clear();
	TxLogDebugEndMsg();
}


void KDefaultMDefineVariableGroupDlg::DeleteTempSaveData()
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


void KDefaultMDefineVariableGroupDlg::updateVariableMap(void)
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


void KDefaultMDefineVariableGroupDlg::InitExpressionChecker(void)
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


void KDefaultMDefineVariableGroupDlg::initReportControl(void)
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


void KDefaultMDefineVariableGroupDlg::UpdateReportRecord(void)
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


// KDefaultMDefineVariableGroupDlg 메시지 처리기입니다.


void KDefaultMDefineVariableGroupDlg::LoadXmlDataModeChoiceVarDefault()
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


KMode* KDefaultMDefineVariableGroupDlg::FindMode(int a_nModeID)
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


void KDefaultMDefineVariableGroupDlg::GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function)
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


CString KDefaultMDefineVariableGroupDlg::GetFieldName(LPCTSTR strCaption)
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


void KDefaultMDefineVariableGroupDlg::LoadInitialFunction(int a_nPreVariableID)
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


BOOL KDefaultMDefineVariableGroupDlg::OnInitDialog()
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


void KDefaultMDefineVariableGroupDlg::CreateChildPage()
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


void KDefaultMDefineVariableGroupDlg::DisplayTabControl( int a_nTabIndex )
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


void KDefaultMDefineVariableGroupDlg::SubPageRedrawWindow()
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


void KDefaultMDefineVariableGroupDlg::OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	DisplayTabControl(m_ctrlTab.GetCurSel());
	*pResult = 0;
}


void KDefaultMDefineVariableGroupDlg::OnBnClickedBtnEdit()
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


void KDefaultMDefineVariableGroupDlg::OnCbnSelchangeComboFunc()
{
	UpdateReportRecord();
}


void KDefaultMDefineVariableGroupDlg::OnBnClickedOk()
{
	if (!InvalidateInputInfo())
		return;

	GetResultInfo(m_mapDefaultODGroup);

	if (SaveDefaultPurpoeGroupODInfo(m_mapDefaultODGroup))
	{
		SaveODGroupDefineVariableID(m_mapDefaultODGroup);
	}

	KDialogEx::OnOK();
}


bool KDefaultMDefineVariableGroupDlg::InvalidateInputInfo()
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


void KDefaultMDefineVariableGroupDlg::GetResultInfo(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup)
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


bool KDefaultMDefineVariableGroupDlg::SaveDefaultPurpoeGroupODInfo(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup)
{
	KDBaseConPtr    spDBaseConnection   = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQLDel(_T(" Delete From Default_ModeChoice_DefineVal_Info "));

		CString strSQL(_T(""));
		strSQL.Format(_T(" Insert into Default_ModeChoice_DefineVal_Info Values( ?, ?, ?, ? ) "));
		
		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQLDel);
		KPreparedStatementPtr  spPrepareStatement  = spDBaseConnection->PrepareStatement(strSQL);
			
		int nPurposeID           (0);
		int nUseODGroup          (0);
		int nODGroupCode         (-1);
		int nNoneGroupFunctionID (-1);

		AutoType iter  = a_mapDefaultODGroup.begin();
		AutoType itEnd = a_mapDefaultODGroup.end();

		while(iter != itEnd)
		{
			TDefVarDefaultODGroup &oTDefVarDefaultODGroup = iter->second;
			
			nPurposeID               = iter->first;
			if (oTDefVarDefaultODGroup.bUseODGroup)
			{
				nUseODGroup          = 1;
				nODGroupCode         = oTDefVarDefaultODGroup.nODGroupCode;
				nNoneGroupFunctionID = -1;
			}
			else
			{
				nUseODGroup          = 0;
				nODGroupCode         = -1;
				nNoneGroupFunctionID = oTDefVarDefaultODGroup.nNoneGroupFunctionID;
			}

			spPrepareStatement->BindInt(1, nPurposeID);
			spPrepareStatement->BindInt(2, nUseODGroup);
			spPrepareStatement->BindInt(3, nODGroupCode);
			spPrepareStatement->BindInt(4, nNoneGroupFunctionID);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


bool KDefaultMDefineVariableGroupDlg::SaveODGroupDefineVariableID(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup)
{
	KDBaseConPtr    spDBaseConnection   = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQLDel(_T(" Delete From Default_ModeChoice_Define_Value "));

		CString strSQL(_T(""));
		strSQL.Format(_T(" Insert into Default_ModeChoice_Define_Value Values( ?, ?, ?, ? ) "));

		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQLDel);
		KPreparedStatementPtr  spPrepareStatement  = spDBaseConnection->PrepareStatement(strSQL);

		int nPurposeID  (0);
		int nODGroupCode(-1);
		int nCode       (-1);    
		int nFunctionID (-1);

		AutoType iter  = a_mapDefaultODGroup.begin();
		AutoType itEnd = a_mapDefaultODGroup.end();

		while(iter != itEnd)
		{
			TDefVarDefaultODGroup &oTDefVarDefaultODGroup = iter->second;

			if (!oTDefVarDefaultODGroup.bUseODGroup)
			{
				++iter;
				continue;
			}

			nPurposeID   = iter->first;
			nODGroupCode = oTDefVarDefaultODGroup.nODGroupCode;
			std::map<int, int> &mapFunctionID = oTDefVarDefaultODGroup.mapFunctionID;
			
			AutoType iterB  = mapFunctionID.begin();
			AutoType itEndB = mapFunctionID.end();
			while (iterB != itEndB)
			{
				nCode        = iterB->first;
				nFunctionID  = iterB->second;

				spPrepareStatement->BindInt(1, nPurposeID);
				spPrepareStatement->BindInt(2, nODGroupCode);
				spPrepareStatement->BindInt(3, nCode);
				spPrepareStatement->BindInt(4, nFunctionID);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++iterB;
			}

			++iter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


void KDefaultMDefineVariableGroupDlg::OnBnClickedBtnInit()
{
	LoadDefaultPurpoeGroupODInfo();

	try
	{
		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KMDefVarSubPagePtr spSubPage = iter->second;
			int nPurposeID   = spSubPage->GetPurposeID();

			AutoType itFind = m_mapDefaultODGroup.find(nPurposeID);
			if (itFind != m_mapDefaultODGroup.end())
			{
				TDefVarDefaultODGroup &oTDefVarDefaultODGroup = itFind->second;
				spSubPage->ExternalInitialize(oTDefVarDefaultODGroup);
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
