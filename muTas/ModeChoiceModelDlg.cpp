/**
 * @file ModeChoiceModelDlg.cpp
 * @brief KModeChoiceModelDlg 구현 파일
 * @author 
 * @date 2011.06.01
 * @remark
 */



#include "stdafx.h"
#include "ImChampDir.h"
#include <fstream>
#include <iostream>
#include "KBulkDBase2File.h"
#include "KmzApp.h"
#include "ModeChoiceModelDlg.h"
#include "afxdialogex.h"
#include <queue>
#include "../kmzModel/KModeChoice.h"
#include "ExpressionElementField.h"
#include "IOTableController.h"
#include "MAdjustmentDlg.h"
#include "MDemandStratumDlg.h"
#include "MDecisionTreePage.h"
#include "MDefineVariableDlg.h"
#include "DefineNames.h"
#include "SelectRegionDlg.h"
#include "SelectVariableDlg.h"
#include "../kmzModel/KParser.h"
#include "ModelChoiceArgInfo.h"
#include "Mode.h"
#include "ModeChoiceInfo.h"
#include "ModeFunction.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "MOutputDesignDlg.h"
#include "MOutputInfo.h"
#include "MGroupVariable.h"
#include "MSimpleVariable.h"
#include "Purpose.h"
#include "Target.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "IOTableView.h"
#include "IOView.h"
#include "PurposeInfoCollection.h"
#include "KUIDefModeChoice.h"
#include "ParseExpression.h"
#include "ImChampDir.h"

#include "Launcher.h"

using namespace std;
using namespace UIModeChoice;
// KModeChoiceModelDlg 대화 상자입니다.

#if WINVER >= 0x0500
#include <uxtheme.h>
#pragma comment (lib, "UxTheme.lib")
#endif

IMPLEMENT_DYNAMIC(KModeChoiceModelDlg, KDialogEx)

KModeChoiceModelDlg::KModeChoiceModelDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KModeChoiceModelDlg::IDD, pParent),
		m_emMModel(KEMModeChoiceNotNestedLogit),
		m_pTarget(NULL)
		, m_bSelectElasticity(FALSE)
{
    TxLogDebugStartMsg();
	m_bVariableGroup = false;
	//m_nVariableCodeGroup = 0;
	m_pVariableGroupColumn = NULL;
	m_bApplyDummy = false;
    TxLogDebugEndMsg();
}


KModeChoiceModelDlg::~KModeChoiceModelDlg()
{
    TxLogDebugStartMsg();
	clearMInfoList();
	clearPurposeSet();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_btnNotNested);
	DDX_Control(pDX, IDC_RADIO2, m_btnNested);
	DDX_Control(pDX, IDC_STATIC_MODEL, m_lblModel);
	DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow4);    
	DDX_Control(pDX, IDC_STATIC_ARROW5, m_uiArrow5);
	DDX_Control(pDX, IDC_STATIC_ARROW6, m_uiArrow6);
	DDX_Control(pDX, IDC_STATIC_ARROW7, m_uiArrow7);
	DDX_Control(pDX, IDC_STATIC_ARROW9, m_uiArrow9);
	DDX_Control(pDX, IDC_STATIC_ARROW10, m_uiArrow10);
	DDX_Control(pDX, IDC_OUTPUTDESIGN, m_btnOutputDesign);
	DDX_Control(pDX, IDC_OUTPUTTABLE, m_btnViewTable);
	DDX_Control(pDX, IDC_DEMANDSTRATUM, m_btnDemand);
	DDX_Control(pDX, IDC_DECISIONTREE, m_btnDecisionTree);
	DDX_Control(pDX, IDC_DEFINEVARIABLE, m_btnVariable);
	DDX_Control(pDX, IDC_ADJUSTMENT, m_btnAdjustment);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSelectVariable);
	DDX_Control(pDX, IDC_REGION, m_btnRegion);
	DDX_Control(pDX, IDC_RUNELASTICITY, m_btnElasticityRun);
	DDX_Control(pDX, IDC_LOGVIEW, m_btnViewLog);
	DDX_Check(pDX, IDC_SELECT_ELASTICITY, m_bSelectElasticity);
}


BEGIN_MESSAGE_MAP(KModeChoiceModelDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, OnRadioClicked)
	ON_BN_CLICKED(IDC_DEMANDSTRATUM, &KModeChoiceModelDlg::OnBnClickedDemandstratum)
	ON_BN_CLICKED(IDC_DECISIONTREE, &KModeChoiceModelDlg::OnBnClickedDecisiontree)
	ON_BN_CLICKED(IDC_DEFINEVARIABLE, &KModeChoiceModelDlg::OnBnClickedDefinevariable)
	ON_BN_CLICKED(IDC_ADJUSTMENT, &KModeChoiceModelDlg::OnBnClickedAdjustment)
	ON_BN_CLICKED(IDC_OUTPUTDESIGN, &KModeChoiceModelDlg::OnBnClickedOutputdesign)
	ON_BN_CLICKED(IDC_RUN, &KModeChoiceModelDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_CLOSE, &KModeChoiceModelDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_OUTPUTTABLE, &KModeChoiceModelDlg::OnBnClickedOutputtable)
	ON_BN_CLICKED(IDC_OUTPUTGRAPH, &KModeChoiceModelDlg::OnBnClickedOutputgraph)
    ON_WM_CTLCOLOR()
    //ON_WM_ACTIVATE() - markup button focus 있을때 외부 클릭 시 버튼 사라짐 문제 때문에 기능 삭제
	ON_BN_CLICKED(IDC_SELECT_ELASTICITY, &KModeChoiceModelDlg::OnBnClickedSelectElasticity)
	ON_BN_CLICKED(IDC_REGION, &KModeChoiceModelDlg::OnBnClickedRegion)
	ON_BN_CLICKED(IDC_BUTTON2, &KModeChoiceModelDlg::OnBnClickedSelectvariable)
	ON_BN_CLICKED(IDC_RUNELASTICITY, &KModeChoiceModelDlg::OnBnClickedRunelasticity)
	ON_BN_CLICKED(IDC_LOGVIEW, &KModeChoiceModelDlg::OnBnClickedLogview)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

void KModeChoiceModelDlg::SetTarget(KTarget* pTarget)
{
    TxLogDebugStartMsg();
	m_pTarget = pTarget;
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::ModeChoiceModelThreadProc(void* pParam)
{
	if(NULL == pParam)
	{
		g_pProgressDlg->Stop();
		return;
	}

	KModeChoiceModelDlg* pDlg = (KModeChoiceModelDlg*)pParam;

	//pDlg->runModel();
	pDlg->CalculateModel();

	if(NULL != g_pProgressDlg)
	{
		g_pProgressDlg->Stop();	
	}
}


void KModeChoiceModelDlg::initPurposeSet(void)
{
    TxLogDebugStartMsg();
	clearPurposeSet();

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

	//이전 set 버젼 list로 변경 

	/*while(iter != end){
		KPurpose* pPurpose = *iter;
		KPurpose* pNewPurpose = new KPurpose( pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName() );
		m_PurposeSet.insert(pNewPurpose);
		++iter;
	}*/
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::clearPurposeSet(void)
{
    TxLogDebugStartMsg();

	std::list<KPurpose*>::iterator itInfo, itEnd = m_lstPurpose.end();
	for(itInfo = m_lstPurpose.begin(); itInfo != itEnd; ++itInfo)
	{
		KPurpose* pPurpose = *itInfo;
		delete pPurpose;
	}
	m_lstPurpose.clear();

	//이전 set 버젼 list로 변경 
	/*m_SelectedPurposeSet.clear();
	std::set<KPurpose*>::iterator itPurpose, itEnd = m_PurposeSet.end();
	for(itPurpose = m_PurposeSet.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;
		delete pPurpose;
	}

	m_PurposeSet.clear();*/
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::clearMInfoList(void)
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


void KModeChoiceModelDlg::initOutputInfo(void)
{
    TxLogDebugStartMsg();
	clearOutputInfo();
	std::vector<KModeChoiceInfo*>::iterator itInfo, itEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo = *itInfo;
		KModeTree* pModeTree = pInfo->ModeTree();
		std::queue<KModeTreeNodeMode*> nodeQ;
		int nNodeCount = pModeTree->ModeNodeCount();
		for(int i = 0; i < nNodeCount; ++i)
		{
			KModeTreeNodeMode* pNode = pModeTree->ModeNode(i);
			nodeQ.push(pNode);
		}

		while(nodeQ.empty() == false)
		{
			KModeTreeNodeMode* pNode = nodeQ.front();
			nodeQ.pop();
			
			std::map<KMode*, KMOutputInfo*>::iterator itOutput = m_OutputMap.find(pNode->Mode());
			if(m_OutputMap.end() == itOutput)
			{
				KMOutputInfo* pOutput = new KMOutputInfo;
				m_OutputMap.insert(std::make_pair(pNode->Mode(), pOutput));
			}

			if(pNode->HasChild() == true)
			{
				int nSubNodeCount = pNode->NodeCount();
				for(int j = 0; j < nSubNodeCount; ++j)
				{
					KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(j));
					nodeQ.push(pSubNode);
				}
			}
		}
	}
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::clearOutputInfo(void)
{
    TxLogDebugStartMsg();
	std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
	for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
	{
		KMOutputInfo* pOutput = itOutput->second;
		delete pOutput;
	}

	m_OutputMap.clear();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::runModel(void)
{
	TxLogDebugStartMsg();
	bool  bResult = true;
	CTime timeStart, timeEnd;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pODTable = pIOTables->FindTable(TABLE_MODE_OD);    
	ASSERT(NULL != pODTable);
	
	std::map<CString, int> purposeODIndexMap;
	std::map<CString, int> paramODIndexMap;
	std::map<KMode*,  int> modeODIndexMap;

	timeStart = CTime::GetCurrentTime();

	// prepare purpose od data - 목적명 / 인덱스
	preparePurposeODIndex(purposeODIndexMap);
    size_t nODColumnCount = purposeODIndexMap.size();
    KODKeyDoubleRecords     purposeODRecords(nODColumnCount);
    preparePurposeODData(purposeODRecords);

    prepareParameterODIndex(paramODIndexMap);

    KIOColumns columnCollection;
    prepareModeIndexMap(modeODIndexMap);
    KODKeyDoubleRecords    modeODRecords(modeODIndexMap.size());

    if(false == m_bVariableGroup)
    {
        bResult = runNonGroupValue(purposeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
    }
    else 
    {
        bResult = runGroupValue(modeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
    }

    if (bResult)
    {
        bResult = addModeODColumns(modeODIndexMap, columnCollection);

        if (bResult)
        {
            bResult = KBulkDBase::BulkODUpsert(pODTable, columnCollection, modeODRecords);
            if (bResult)
            {
                timeEnd = CTime::GetCurrentTime();
                recordModeChoiceResult(timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
                    timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
            }
        }
    }

    if (bResult)
    {

    }
    else
    {
        TxLogDebugException();
    }
	pODTable->Notify();
	KRedrawWindow();
	TxLogDebugEndMsg();
}

void KModeChoiceModelDlg::preparePurposeODIndex(std::map<CString, int>& purposeIndexMap)
{
    TxLogDebugStartMsg();
	//int nIndex = 0; ptj
	int nIndex = 1;
	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo = *itInfo;
		KIOColumn* pInputColumn = pInfo->InputColumn();
		purposeIndexMap.insert(std::make_pair(pInputColumn->Name(), nIndex++));
	}
    TxLogDebugEndMsg();
}

void KModeChoiceModelDlg::preparePurposeODData(KODKeyDoubleRecords& records)
{
    TxLogDebugStartMsg();

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable* pTable = pIOTables->FindTable(TABLE_PURPOSE_OD);

    // request column 설정
    KIOColumns reqColumns;
    KIOColumn* pColumn = NULL;

    std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
    for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
    {
        KModeChoiceInfo* pInfo = *itInfo;        
        KIOColumn*       pInputColumn = pInfo->InputColumn();
        reqColumns.AddNewColumn(pInputColumn);
    }
    int nColumnCount = reqColumns.ColumnCount();
    KBulkDBase::BulkODSelect(pTable, reqColumns, records);

    TxLogDebugEndMsg();
}

KMultiIDDoubleRecord* KModeChoiceModelDlg::preparePurposeODData(void)
{
    TxLogDebugStartMsg();
	KMultiIDDoubleRecord* pPurposeODData = NULL;
	
    KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
	ASSERT(NULL != pTable);

	// request column 설정
	KIOColumns reqColumns;
	KIOColumn* pColumn = NULL;
	CString strEmpty = _T("");

	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(_T("ozone_id"));
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(_T("dzone_id"));

	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo = *itInfo;
		KIOColumn* pInputColumn = pInfo->InputColumn();
		reqColumns.AddNewColumn(pInputColumn);
	}
	int nColumnCount = reqColumns.ColumnCount();

	// get recordset
	KIORecordset* pRecordset = pTable->GetRecordset(KEMIORecordsetTypeDynamic, &reqColumns, strEmpty, strEmpty);
	
	if(NULL != pRecordset)
	{
		pPurposeODData = new KMultiIDDoubleRecord(nColumnCount - 2);
		int nRecordCount = pRecordset->GetRowCount();
		for(int i = 0; i < nRecordCount; ++i)
		{
			const KIORow* pRow = pRecordset->GetRow(i);
			KIOItem* pItem;
			pItem = pRow->GetItem(0);
			__int64 nOID = pItem->GetValueAsInteger64();
			pItem = pRow->GetItem(1);
			__int64 nDID = pItem->GetValueAsInteger64();
			KDoubleRecord* pPurposeRecord = pPurposeODData->AddRecord(KMultiID(nOID, nDID));

			for(int j = 2; j < nColumnCount; ++j)
			{
				pItem = pRow->GetItem(j);
				double dValue = pItem->GetValueAsDouble();
				pPurposeRecord->SetAt(j - 2, dValue);
			}
		}

		delete pRecordset;
	}

    TxLogDebugEndMsg();
	return pPurposeODData;
}


void KModeChoiceModelDlg::prepareParameterODIndex(std::map<CString, int>& paramIndexMap)
{
    // 파라메터 OD에서 Double 인 것만(순서는 파라메터 OD에 저장된 컬럼 순서) 
    TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);
	ASSERT(NULL != pTable);

	const
    KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	int nIndex = 0;

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() != KEMIODataTypeDouble)
		{
			continue;
		}

		CString strColumnName = pColumn->Name();
		paramIndexMap.insert(std::make_pair(strColumnName, nIndex++));
	}
    TxLogDebugEndMsg();
}


KODKeyDoubleRecords* KModeChoiceModelDlg::prepareParameterODData(CString strFilter)
{
    TxLogDebugStartMsg();

    
    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable* pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);

    KIOColumns reqColumns;
    KIOColumn*          pColumn = NULL;

    const 
    KIOColumns* pColumns = pTable->Columns();
    int nColumnCount = pColumns->ColumnCount();
    for(int i = 0; i < nColumnCount; ++i)
    {
        KIOColumn* pOriginColumn = pColumns->GetColumn(i);
        if(pOriginColumn->DataType() != KEMIODataTypeDouble)
        {
            continue;
        }

        reqColumns.AddNewColumn(pOriginColumn);
    }

    int nReqColumnCount = reqColumns.ColumnCount();

    KODKeyDoubleRecords* pRecords = new KODKeyDoubleRecords(nReqColumnCount);
    KBulkDBase::BulkODSelect(pTable, strFilter, reqColumns, *pRecords);

    TxLogDebugEndMsg();
    return pRecords;

}



KMultiIDDoubleRecord* KModeChoiceModelDlg::prepareModeODData(std::map<KMode*, int>& modeIndexMap)
{
    TxLogDebugStartMsg();
	KMultiIDDoubleRecord* pModeODData = NULL;
	//int nIndex = 0; ptj
	int nIndex = 1;
	std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
	for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
	{
		modeIndexMap.insert(std::make_pair(itOutput->first, nIndex++));
	}

	pModeODData = new KMultiIDDoubleRecord(nIndex);
    TxLogDebugEndMsg();
	return pModeODData;
}

void KModeChoiceModelDlg::prepareModeIndexMap(std::map<KMode*, int>& modeIndexMap)
{
    TxLogDebugStartMsg();
    
    //int nIndex = 0; ptj
	int nIndex = 1;
    std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
    for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
    {
        modeIndexMap.insert(std::make_pair(itOutput->first, nIndex++));
    }
   
    TxLogDebugEndMsg();
    
}

KDTreeNodeCollection* KModeChoiceModelDlg::generateModeTree(KModeChoiceInfo* pInfo, 
												std::map<CString, int>& paramIndexMap,
												std::map<KMode*, int>& modeIndexMap)
{
    TxLogDebugStartMsg();
    KIOTables*   pIOTables = m_pTarget->Tables();
    KIOTable*          pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);

	KDTreeNodeCollection* pNodeCollection = NULL;
	
	KMBaseVariable* pVariable = pInfo->Variable();
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);
	KMSimpleVariable* pSVariable = (KMSimpleVariable*)pVariable;

	std::queue<KModeTreeNodeMode*> modeQ;
	std::queue<KDTreeNestedNode*>  nestedQ;

	KModeTree* pModeTree = pInfo->ModeTree();
	int nModeNodeCount = pModeTree->ModeNodeCount();
	for(int i = 0; i < nModeNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		modeQ.push(pModeNode);
		nestedQ.push(NULL);
	}

	pNodeCollection = new KDTreeNodeCollection;

	while(modeQ.empty() == false)
	{
		KModeTreeNodeMode* pModeNode   = modeQ.front();
		modeQ.pop();
		KDTreeNestedNode*  pNestedNode = nestedQ.front();
		nestedQ.pop();

		KMode* pMode = pModeNode->Mode();
		std::map<KMode*, int>::iterator itModeIndex = modeIndexMap.find(pMode);
		ASSERT(modeIndexMap.end() != itModeIndex);
		int nModeIndex = itModeIndex->second;

		if(pModeNode->HasChild() == false)
		{
			KModeFunction* pFunction = pSVariable->ModeFunction(pMode->ModeID());

			// collection 또는 nestnode에 추가
			KDTreeModeNode* pTreeModeNode= NULL;
			if(NULL == pNestedNode)
			{
				pTreeModeNode = (KDTreeModeNode*)(pNodeCollection->AddNode(KEMModeChoice::MODE_NODE_TYPE));
			}
			else 
			{
				pTreeModeNode = pNestedNode->AddNewModeNode();
			}
			
            KParseExpression oParseExpression;
            oParseExpression.SetTable(pTable);
            oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

            std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
            CString              strRealFunction   = oParseExpression.GetRealExpression();

            size_t nxCount = vecUsedColumnName.size();
            for (size_t i=0; i<nxCount; i++)
            {
                std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(vecUsedColumnName[i]);
                if(paramIndexMap.end() != itParamIndex)
                {
                    int nParamIndex = itParamIndex->second;
                    pTreeModeNode->AddNewVariableInfo(vecUsedColumnName[i], nParamIndex);
                }
            }

            pTreeModeNode->SetUtilityFunction(strRealFunction);
            /*
            pTreeModeNode->SetUtilityFunction(pFunction->GetExpression());
			// parameter od index 설정 
			const std::list<KExpressionElement*>& elementList = pFunction->GetExpressionList();
			std::list<KExpressionElement*>::const_iterator itElement, itElementEnd = elementList.end();
			for(itElement = elementList.begin(); itElement != itElementEnd; ++itElement)
			{
				KExpressionElement* pElement = *itElement;
				if(pElement->ElementType() == KEMExpressionElementField)
				{
					KExpressionElementField* pField = (KExpressionElementField*)pElement;
					std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(pField->FieldName());
					if(paramIndexMap.end() != itParamIndex)
					{
						int nParamIndex = itParamIndex->second;
						pTreeModeNode->AddNewVariableInfo(pField->FieldName(), nParamIndex);
					}
				}
			}
			}*/

			pTreeModeNode->SetResultIndexKey(nModeIndex);
		}
		else
		{
			double dValue = pSVariable->ModeParameter(pMode->ModeID());

			KDTreeNestedNode* pNestMode = (KDTreeNestedNode*)(pNodeCollection->AddNode(KEMModeChoice::NESTED_NODE_TYPE));
			pNestMode->SetInclusiveValue(dValue);

			int nSubNodeCount = pModeNode->NodeCount();
			for(int j = 0; j < nSubNodeCount; ++j)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pModeNode->Node(j));
				modeQ.push  (pSubNode);
				nestedQ.push(pNestMode);
			}

			pNestMode->SetResultIndexKey(nModeIndex);
		}
	}
    TxLogDebugEndMsg();
	return pNodeCollection;
}


KDTreeNodeCollection* KModeChoiceModelDlg::generateModeTree(int nCode, 
												KModeChoiceInfo* pInfo, 
												std::map<CString, int>& paramIndexMap,
												std::map<KMode*, int>& modeIndexMap)
{
    TxLogDebugStartMsg();
    KIOTables*   pIOTables = m_pTarget->Tables();
    KIOTable*          pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);

	KDTreeNodeCollection* pNodeCollection = NULL;

	KMBaseVariable* pVariable = pInfo->Variable();
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeGroup);
	KMGroupVariable* pGVariable = (KMGroupVariable*)pVariable;

	std::queue<KModeTreeNodeMode*> nodeQ;
	std::queue<KDTreeNestedNode*> nestedQ;

	KModeTree* pModeTree = pInfo->ModeTree();
	int nModeNodeCount = pModeTree->ModeNodeCount();
	for(int i = 0; i < nModeNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		nodeQ.push(pModeNode);
		nestedQ.push(NULL);
	}

	pNodeCollection = new KDTreeNodeCollection;

	while(nodeQ.empty() == false)
	{
		KModeTreeNodeMode* pModeNode = nodeQ.front();
		nodeQ.pop();
		KDTreeNestedNode* pNestedNode = nestedQ.front();
		nestedQ.pop();

		KMode* pMode = pModeNode->Mode();
		std::map<KMode*, int>::iterator itModeIndex = modeIndexMap.find(pMode);
		ASSERT(modeIndexMap.end() != itModeIndex);
		int nModeIndex = itModeIndex->second;

		if(pModeNode->HasChild() == false)
		{
			KModeFunction* pFunction = pGVariable->ModeFunction(nCode, pMode->ModeID());

			// collection 또는 nestnode에 추가
			KDTreeModeNode* pTreeModeNode= NULL;
			if(NULL == pNestedNode)
			{
				pTreeModeNode = (KDTreeModeNode*)(pNodeCollection->AddNode(KEMModeChoice::MODE_NODE_TYPE));
			}
			else 
			{
				pTreeModeNode = pNestedNode->AddNewModeNode();
			}

            KParseExpression oParseExpression;
            oParseExpression.SetTable(pTable);
            oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

            std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
            CString              strRealFunction   = oParseExpression.GetRealExpression();

            size_t nxCount = vecUsedColumnName.size();
            for (size_t i=0; i<nxCount; i++)
            {
                std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(vecUsedColumnName[i]);
                if(paramIndexMap.end() != itParamIndex)
                {
                    int nParamIndex = itParamIndex->second;
                    pTreeModeNode->AddNewVariableInfo(vecUsedColumnName[i], nParamIndex);
                }
            }

            pTreeModeNode->SetUtilityFunction(strRealFunction);
            /*
			pTreeModeNode->SetUtilityFunction(pFunction->GetExpression());

			// parameter od index 설정 
			const std::list<KExpressionElement*>& elementList = pFunction->GetExpressionList();
			std::list<KExpressionElement*>::const_iterator itElement, itElementEnd = elementList.end();
			for(itElement = elementList.begin(); itElement != itElementEnd; ++itElement)
			{
				KExpressionElement* pElement = *itElement;
				if(pElement->ElementType() == KEMExpressionElementField)
				{
					KExpressionElementField* pField = (KExpressionElementField*)pElement;
					std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(pField->FieldName());
					if(paramIndexMap.end() != itParamIndex)
					{
						int nParamIndex = itParamIndex->second;
						pTreeModeNode->AddNewVariableInfo(pField->FieldName(), nParamIndex);
					}
				}
			}
            */
			pTreeModeNode->SetResultIndexKey(nModeIndex);
		}
		else
		{
			double dValue = pGVariable->ModeParameter(nCode, pMode->ModeID());

			KDTreeNestedNode* pNestMode = (KDTreeNestedNode*)(pNodeCollection->AddNode(KEMModeChoice::NESTED_NODE_TYPE));
			pNestMode->SetInclusiveValue(dValue);

			int nSubNodeCount = pModeNode->NodeCount();
			for(int j = 0; j < nSubNodeCount; ++j)
			{
				KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pModeNode->Node(j));
				nodeQ.push(pSubNode);
				nestedQ.push(pNestMode);
			}

			pNestMode->SetResultIndexKey(nModeIndex);
		}
	}
    TxLogDebugEndMsg();
	return pNodeCollection;
}

bool KModeChoiceModelDlg::runNonGroupValue(KODKeyDoubleRecords& purposeODRecords, 
    std::map<CString, int>& purposeODIndexMap, 
    std::map<CString, int>& paramODIndexMap, 
    std::map<KMode*, int>& modeODIndexMap, 
    KODKeyDoubleRecords& modeODRecords/*out*/)
{
    TxLogDebugStartMsg();
    bool bResult = true;
    KxModeChoice mc;
    KODKeyDoubleRecords* pParameterOD = prepareParameterODData();
    if(NULL == pParameterOD)
    {
        return false;
    }

    std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
    for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
    {
        KModeChoiceInfo* pInfo = *itInfo;
        KIOColumn* pInputColumn = pInfo->InputColumn();
        std::map<CString, int>::iterator itIndex = purposeODIndexMap.find(pInputColumn->Name());
        ASSERT(purposeODIndexMap.end() != itIndex);
        int nPurposeODIndex = itIndex->second;

        KDTreeNodeCollection* pCollection = generateModeTree(pInfo, paramODIndexMap, modeODIndexMap);
        if(NULL == pCollection)
        {
            bResult = false;
            break;
        }

        int nResult = mc.MuExecute(*pParameterOD, *pCollection, purposeODRecords, nPurposeODIndex, modeODRecords);

        if(NULL != pCollection)
        {
            delete pCollection;
        }

        if(KReturn::R_OK != nResult)
        {
            bResult = false;
            break;
        }
    }

    if(NULL != pParameterOD)
    {
        delete pParameterOD;
    }

    TxLogDebugEndMsg();
    return bResult;
}

bool KModeChoiceModelDlg::runGroupValue(KODKeyDoubleRecords& pPurposeODRecord, 
    std::map<CString, int>& purposeODIndexMap, 
    std::map<CString, int>& paramODIndexMap, 
    std::map<KMode*, int>& modeODIndexMap, 
    KODKeyDoubleRecords& pModeODRecord)
{
    TxLogDebugStartMsg();
    bool bResult = true;
    KxModeChoice mc;
    ASSERT(NULL != m_pVariableGroupColumn);
    ASSERT(m_pVariableGroupColumn->CodeType() == KEMIOCodeTypeIsSingleCode);

    KCodeManager* pCodeManager = m_pTarget->CodeManager();
    KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(m_pVariableGroupColumn->CodeGroup());
    std::vector<int> codeArray;
    pCodeData->GetCodes(codeArray);
    std::vector<int>::iterator itCode, itCodeEnd = codeArray.end();
    for(itCode = codeArray.begin(); itCode != itCodeEnd; ++itCode)
    {
        CString strFilter;
        strFilter.Format(_T(" %s = %d "), m_pVariableGroupColumn->Name(), *itCode);
        KODKeyDoubleRecords* pParameterOD = prepareParameterODData(strFilter);
        if(NULL == pParameterOD)
        {
            return false;
        }

        std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
        for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
        {
            KModeChoiceInfo* pInfo = *itInfo;
            KIOColumn* pInputColumn = pInfo->InputColumn();
            std::map<CString, int>::iterator itIndex = purposeODIndexMap.find(pInputColumn->Name());
            ASSERT(purposeODIndexMap.end() != itIndex);
            int nPurposeODIndex = itIndex->second;

            KDTreeNodeCollection* pCollection = generateModeTree(*itCode, pInfo, paramODIndexMap, modeODIndexMap);
            if(NULL == pCollection)
            {
                bResult = false;
                break;
            }

            int nResult = mc.MuExecute(*pParameterOD, *pCollection, pPurposeODRecord, 
                nPurposeODIndex, pModeODRecord);

            if(NULL != pCollection)
            {
                delete pCollection;
            }

            if(KReturn::R_OK != nResult)
            {
                bResult = false;
                break;
            }
        }

        if(NULL != pParameterOD)
        {
            delete pParameterOD;
        }
    }
    TxLogDebugEndMsg();
    return bResult;
}


bool KModeChoiceModelDlg::addModeODColumns(std::map<KMode*, int>& modeODIndexMap, KIOColumns& columnCollection)
{
    TxLogDebugStartMsg();

    KIOTables*   pIOTables = m_pTarget->Tables();
    KIOTable* pTable = pIOTables->FindTable(TABLE_MODE_OD);
    if(NULL == pTable)
    {
        return false;
    }

    // column 존재 여부 확인 및 추가
    columnCollection.Clear();

    const 
    KIOColumns* pColumns = pTable->Columns();

    std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
    for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
    {
        KMOutputInfo* pOutput       = itOutput->second;
        CString       strColumnName = pOutput->FieldName();

        KIOColumn* pColumn = pColumns->GetColumn(strColumnName);
        if(NULL == pColumn)
        {
            KIOColumn tColumn;
            tColumn.Name(pTable->GetUsableColumnName());
            tColumn.Caption(pOutput->FieldCaption());
            tColumn.CodeType(KEMIOCodeTypeIsNothing);
            tColumn.ColumnType(KEMIOColumnTypeUserDefine);
            tColumn.DataType(KEMIODataTypeDouble);
            tColumn.DisplayOrder(pColumns->ColumnCount()+1);
            tColumn.PrimaryKey(false);
            tColumn.Visible(true);

            pColumn = pTable->AddColumn(tColumn);
            if(NULL == pColumn)
            {
                AfxMessageBox(_T("컬럼 추가에 실패하였습니다."));
                return false;
            }
            pOutput->FieldName(pColumn->Name());
            pColumns = pTable->Columns();
        }
        
        columnCollection.AddNewColumn(pColumn);
    }

    TxLogDebugEndMsg();
    return true;
}


void KModeChoiceModelDlg::recordModeChoiceResult(LPCTSTR strStartDate, LPCTSTR strEndDate)
{
    TxLogDebugStartMsg();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
	for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
	{
		KMode* pMode = itOutput->first;
		KMOutputInfo* pOutput = itOutput->second;
		CString strSQL;
		strSQL.Format(
			_T(" INSERT OR REPLACE INTO modechoice_model ")
			_T(" (mode_group_code, detail_mode_id, mode_od_column_name, start_date, end_date) ")
			_T(" VALUES ('%d', '%d', '%s', '%s', '%s') "),
			pMode->ModeGroup(),
			pMode->ModeID(),
			pOutput->FieldName(), 
			strStartDate, 
			strEndDate
			);

		try
		{
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		catch (...)
		{
			TxLogDebugException();
		}
	}
    TxLogDebugEndMsg();
}



// KModeChoiceModelDlg 메시지 처리기입니다.
BOOL KModeChoiceModelDlg::OnInitDialog()
{
    TxLogDebugStartMsg();
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	//MoveWindow(0,0,500,500);

	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(4, 5);

	m_uiBackImage.MoveWindow(&rcClient);
    m_uiBackImage.SetMarkupText(UIXAML_M_BACKIMAGE);

    m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB); 
    m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 
    m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
    m_uiArrow4.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
    m_uiArrow5.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 
	m_uiArrow6.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRB); 
	m_uiArrow7.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRT); 
	m_uiArrow9.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 
	m_uiArrow10.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	m_uiArrow10.DashStype(DashStyleSolid);
	m_uiArrow10.SetArrowCap(0,0,FALSE);

#ifndef _UNICODE
    SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, L"", L"");
    SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_SELECT_ELASTICITY)->m_hWnd, L"", L"");
#else
    SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, _T(""), _T("") );
	SetWindowTheme( GetDlgItem(IDC_SELECT_ELASTICITY)->m_hWnd, _T(""), _T("") );

#endif

    UINT nID_MODECHOICE_INPUT  = 90923;
    XTPImageManager()->SetIcons(IDB_MODECHOICE_INPUT, &nID_MODECHOICE_INPUT, 1, 0); 

    UINT nID_MODECHOICE_MODEL  = 90925;
    XTPImageManager()->SetIcons(IDB_MODECHOICE_MODEL, &nID_MODECHOICE_MODEL, 1, 0); 

    UINT nID_MODECHOICE_OUTPUT = 90927;
    XTPImageManager()->SetIcons(IDB_MODECHOICE_OUTPUT, &nID_MODECHOICE_OUTPUT, 1, 0); 

    UINT nID_DISTRIBUTION_INPUT  = 90913;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_INPUT, &nID_DISTRIBUTION_INPUT, 1, 0); 
    UINT nID_DISTRIBUTION_MODEL  = 90905;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL, &nID_DISTRIBUTION_MODEL, 1, 0); 
    UINT nID_DISTRIBUTION_OUTPUT = 90917;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_OUTPUT, &nID_DISTRIBUTION_OUTPUT, 1, 0); 

    // 모델
    m_lblModel.SetMarkupText(UIXAML_M_MODEL_SELECTED);
    // Select Demand Stratum(Tijp)
    m_btnDemand.EnableMarkup();
    m_btnDemand.SetWindowText(UIXAML_M_DEMAND_DEFAULT);

    // Decision Tree
    m_btnDecisionTree.EnableMarkup();
    m_btnDecisionTree.SetWindowText(UIXAML_M_DECISIONTREE_FALSE);
    m_btnDecisionTree.EnableWindow(FALSE);

    // Define Variable
    m_btnVariable.EnableMarkup();
    m_btnVariable.SetWindowText(UIXAML_M_VARIABLE_FALSE);
    m_btnVariable.EnableWindow(FALSE);

    // Adjustment
    m_btnAdjustment.EnableMarkup();
    /*m_btnAdjustment.SetWindowText(UIXAML_M_ADJUSTMENT_FALSE); TODO 임시 처리 - jyk:2012-05-11 임시로 visible 상태로 두기 위한*/
	m_btnAdjustment.SetWindowText(UIXAML_M_ADJUSTMENT_SELECTED);
    m_btnAdjustment.EnableWindow(FALSE);

    // OutputDesign
    m_btnOutputDesign.EnableMarkup();
    m_btnOutputDesign.SetWindowText(UIXAML_M_OUTPUT_FALSE);
    m_btnOutputDesign.EnableWindow(FALSE);

	m_btnViewTable.EnableWindow(FALSE);

	m_btnSelectVariable.EnableMarkup();
	m_btnSelectVariable.SetWindowText(UIXAML_M_SELECT_VARIABLE_FALSE);
	m_btnSelectVariable.EnableWindow(FALSE);

	m_btnRegion.EnableMarkup();
	m_btnRegion.SetWindowText(UIXAML_M_REGION_FALSE);
	m_btnRegion.EnableWindow(FALSE);

	m_btnElasticityRun.EnableMarkup();
	m_btnElasticityRun.SetWindowText(UIXAML_M_RUN_ELASTICITY_FALSE);
	m_btnElasticityRun.EnableWindow(FALSE);

	m_btnViewLog.EnableMarkup();
	m_btnViewLog.SetWindowText(UIXAML_M_VIEW_LOG_FALSE);
	m_btnViewLog.EnableWindow(FALSE);

	GetDlgItem( IDC_SELECT_ELASTICITY )->EnableWindow( FALSE );

    SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(KEMModeChoiceNotNestedLogit == m_emMModel)
	{
		m_btnNotNested.SetCheck(BST_CHECKED);
	}
	else
	{
		m_btnNested.SetCheck(BST_CHECKED);
	}

	initPurposeSet();
    KRedrawWindow();
    TxLogDebugEndMsg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KModeChoiceModelDlg::OnRadioClicked(UINT nID)
{
	TxLogDebugStartMsg();
	switch(nID)
	{
	case IDC_RADIO1:	m_emMModel = KEMModeChoiceNotNestedLogit;
		break;
	case IDC_RADIO2:    m_emMModel = KEMModeChoiceNestedLogit;
		break;
	}
    KRedrawWindow();
	TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedDemandstratum()
{
    TxLogDebugStartMsg();
	KMDemandStratumDlg dlg;
	dlg.SetTarget(m_pTarget);
	//dlg.PurposeSet(m_PurposeSet);
	dlg.PurposeList(m_lstPurpose);
	dlg.SelectedPurpose(m_SelectedPurposeSet);
	dlg.InputMap(m_InputMap);

	if(dlg.DoModal() == IDOK)
	{
		clearMInfoList();
		m_SelectedPurposeSet = dlg.SelectedPurpose();
		m_InputMap = dlg.InputMap();
		std::set<KPurpose*, TPurposeComparator>::iterator itPurpose, itEnd = m_SelectedPurposeSet.end();
		for(itPurpose = m_SelectedPurposeSet.begin(); itPurpose != itEnd; ++itPurpose)
		{
			KPurpose* pPurpose = *itPurpose;
			std::map<KPurpose*, KIOColumn*, TPurposeComparator>::iterator itInput = m_InputMap.find(pPurpose);
			ASSERT(m_InputMap.end() != itInput);
			KIOColumn* pInputColumn = itInput->second;

			KModeChoiceInfo* pInfo = new KModeChoiceInfo;
			pInfo->Purpose(pPurpose);
			pInfo->InputColumn(pInputColumn);
			m_MInfoVector.push_back(pInfo);
		}

        m_btnDemand.SetWindowText(UIXAML_M_DEMAND_SELECTED);
        m_btnDecisionTree.SetWindowText(UIXAML_M_DECISIONTREE_DEFAULT);
        m_btnDecisionTree.EnableWindow(TRUE);

		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedDecisiontree()
{
    TxLogDebugStartMsg();
	CXTPPropertySheet ps(_T("Define Tree Structure"));

	ps.SetNavigator(new CXTPPropertyPageListNavigator);
	CXTPPropertyPageListNavigator* pNav = (CXTPPropertyPageListNavigator*)(ps.GetNavigator());

	//std::list<KMDecisionTreePage*> pageList;
	//std::map<int, CString>::iterator itPurpose, itEnd = m_PurposeMap.end();
	//for(itPurpose = m_PurposeMap.begin(); itPurpose != itEnd; ++itPurpose)
	//{
	//	KMDecisionTreePage* pPage = new KMDecisionTreePage;
	//	pPage->SetCaption(itPurpose->second);
	//	pPage->ModeChoiceModel(m_emMModel);
	//	pPage->ModeTree()
	//	ps.AddPage(pPage);
	//	pageList.push_back(pPage);
	//}

	if( m_MInfoVector.size()< 1 )
	{
		AfxMessageBox(_T("선택한 데이터 정보가 없습니다."));
		return;
	}

	std::map<KMDecisionTreePage*, KModeChoiceInfo*> pageMap;
	std::vector<KModeChoiceInfo*>::iterator itMInfo, itMEnd = m_MInfoVector.end();
	for(itMInfo = m_MInfoVector.begin(); itMInfo != itMEnd; ++itMInfo)
	{
		KModeChoiceInfo* pInfo = *itMInfo;
		KPurpose* pPurpose = pInfo->Purpose();

		KMDecisionTreePage* pPage = new KMDecisionTreePage;
		pPage->SetParentSheet(&ps);

		pPage->Target(m_pTarget);
		pPage->SetCaption(pPurpose->PurposeName());
		pPage->ModeChoiceModel(m_emMModel);
		pPage->ModeTree(pInfo->ModeTree());

		ps.AddPage(pPage);
		pageMap.insert(std::make_pair(pPage, pInfo));
		/*pPage->SetActiveWindow();*/
	}

	ps.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	ps.RecalcLayout();

	INT_PTR nResult = ps.DoModal();

	CString strEmptyTreeCheckMsg = _T("");
	int     nEmptyCount = 0;
	std::map<KMDecisionTreePage*, KModeChoiceInfo*>::iterator itPage, itPEnd = pageMap.end();
	for(itPage = pageMap.begin(); itPage != itPEnd; ++itPage)
	{
		KMDecisionTreePage* pPage = itPage->first;
		KModeChoiceInfo* pInfo = itPage->second;

		if(IDOK == nResult)
		{
			pInfo->ModeTree(pPage->ModeTree());

			int nModeTreeCount = pInfo->ModeTree()->ModeNodeCount();
			if( nModeTreeCount< 1 && TRUE == strEmptyTreeCheckMsg.IsEmpty() )
			{
				KPurpose* pPurpose = pInfo->Purpose();
				strEmptyTreeCheckMsg.Format(_T("%s에 대한 Decision Tree 관련 정보가 존재하지 않습니다."), pPurpose->PurposeName());
				nEmptyCount++;
			}
		}

		delete pPage;
	}

	if( nEmptyCount > 0 )
	{
		AfxMessageBox(strEmptyTreeCheckMsg);
		return;
	}

	if(IDOK == nResult)
	{
		initOutputInfo();

        m_btnDecisionTree.SetWindowText(UIXAML_M_DECISIONTREE_SELECTED);
        m_btnVariable.SetWindowText(UIXAML_M_VARIABLE_DEFAULT);
        m_btnVariable.EnableWindow(TRUE);

		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedDefinevariable()
{
    TxLogDebugStartMsg();
	KMDefineVariableDlg dlg;
	dlg.Target(m_pTarget);
	dlg.Group(m_bVariableGroup);
	dlg.GroupColumn(m_pVariableGroupColumn);

	std::vector<KModeChoiceInfo*>::iterator itInfo, itEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo = *itInfo;
		dlg.AddModeChoiceInfo(pInfo);
	}

	if(dlg.DoModal() == IDOK)
	{
		m_bVariableGroup = dlg.Group();
		m_pVariableGroupColumn = dlg.GroupColumn();

        m_btnVariable.SetWindowText(UIXAML_M_VARIABLE_SELECTED);
		m_btnOutputDesign.SetWindowText(UIXAML_M_OUTPUT_DEFAULT);
		m_btnOutputDesign.EnableWindow(TRUE);
		

		RunButtonControl();
	}
	KRedrawWindow();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedAdjustment()
{
    TxLogDebugStartMsg();
	KMAdjustmentDlg dlg;
	dlg.Target(m_pTarget);
	dlg.ApplyDummy(m_bApplyDummy);

	std::vector<KModeChoiceInfo*>::iterator itInfo, itEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo = *itInfo;
		dlg.AddModeChoiceInfo(pInfo);
	}

	if(dlg.DoModal() == IDOK)
	{
		m_bApplyDummy = dlg.ApplyDummy();
		m_btnAdjustment.SetWindowText(UIXAML_M_ADJUSTMENT_SELECTED);
        m_btnOutputDesign.SetWindowText(UIXAML_M_OUTPUT_DEFAULT);
        m_btnOutputDesign.EnableWindow(TRUE);

		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedOutputdesign()
{
    TxLogDebugStartMsg();
	KMOutputDesignDlg dlg;
	dlg.Target(m_pTarget);
	std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
	for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
	{
		dlg.AddOutptuInfo(itOutput->first, itOutput->second);
	}

	if(dlg.DoModal() == IDOK)
	{
		// todo : Output 설정 후 처리할 내용
        m_btnOutputDesign.SetWindowText(UIXAML_M_OUTPUT_SELECTED);

		RunButtonControl();
	}
	/*((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(TRUE);*/
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::RunButtonControl()
{
	UpdateData(TRUE);

	CString strButtonState = _T(""); 
	BOOL bButtonRun = TRUE;

	m_btnDemand.GetWindowText(strButtonState);
	if( strButtonState.CompareNoCase(UIXAML_M_DEMAND_SELECTED) != 0 )
	{
		bButtonRun = FALSE;
	}

	m_btnDecisionTree.GetWindowText(strButtonState);
	if( strButtonState.CompareNoCase(UIXAML_M_DECISIONTREE_SELECTED) != 0 )
	{
		bButtonRun = FALSE;
	}

	m_btnVariable.GetWindowText(strButtonState);
	if( strButtonState.CompareNoCase(UIXAML_M_VARIABLE_SELECTED) != 0 )
	{
		bButtonRun = FALSE;
	}

	m_btnAdjustment.GetWindowText(strButtonState);
	if( strButtonState.CompareNoCase(UIXAML_M_ADJUSTMENT_SELECTED) != 0 )
	{
		bButtonRun = FALSE;
	}

	m_btnOutputDesign.GetWindowText(strButtonState);
	if( strButtonState.CompareNoCase(UIXAML_M_OUTPUT_SELECTED) != 0 )
	{
		bButtonRun = FALSE;
	}

	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(bButtonRun);
}


void KModeChoiceModelDlg::OnBnClickedRun()
{
 	TxLogDebugStartMsg();
	ImChampDir::SetCurrentDirectoryToApp();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(ModeChoiceThreadCaller, this, false, nLang);

	m_btnViewTable.EnableWindow(TRUE);
	GetDlgItem( IDC_SELECT_ELASTICITY )->EnableWindow( TRUE );

	KRedrawWindow();

	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable* pODTable = pIOTables->FindTable(_T("mode_od"));
		pODTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
	TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedClose()
{
	OnCancel();
}


void KModeChoiceModelDlg::OnBnClickedOutputtable()
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pODTable = pIOTables->FindTable(_T("mode_od"));
	pODTable->Notify();
	ASSERT(NULL != pODTable);

	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowIOView( true );
	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pODTable->Name());
		if(NULL == pTableView)
		{
			pTableView = pIOView->CreateNewTableView(pODTable);
			KIOTableController* pController = new KIOTableController(m_pTarget);
			pController->Attach(pODTable, pTableView);
		}
		pIOView->SetActiveTableView(pODTable->Name());
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedOutputgraph()
{
	TxLogDebugStartMsg();
	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowMapView(true);
	//KMapView* pwndMapView = pChildFrame->getMapView();
	//if( (NULL != pwndMapView) && (pwndMapView->getTarget() == NULL) )
	//{
	//	pwndMapView->setTarget(m_pTarget);
	//	pwndMapView->setGeoData(m_pTarget->getGeoData() );
	//	pwndMapView->displayMap();
	//}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


HBRUSH KModeChoiceModelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  여기서 DC의 특성을 변경합니다.       
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_SELECT_ELASTICITY )
    {        
        pDC->SetBkMode(TRANSPARENT);
        //pDC->SetTextColor(0x00600912);
        return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }

    // TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
    return hbr;
}

void KModeChoiceModelDlg::KRedrawWindow()
{
    RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


void KModeChoiceModelDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    KDialogEx::OnActivate(nState, pWndOther, bMinimized);
    KRedrawWindow();
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void KModeChoiceModelDlg::OnBnClickedSelectElasticity()
{
	TxLogDebugStartMsg();
	UpdateData(TRUE);
	if( m_bSelectElasticity == TRUE)
	{
		m_btnSelectVariable.EnableWindow(TRUE);
		m_btnSelectVariable.SetWindowText(UIXAML_M_SELECT_VARIABLE_DEFAULT);
		
	}
	else
	{
		m_btnSelectVariable.EnableWindow(FALSE);
		m_btnRegion.EnableWindow(FALSE);
		m_btnElasticityRun.EnableWindow(FALSE);
		m_btnSelectVariable.SetWindowText(UIXAML_M_SELECT_VARIABLE_FALSE);
		m_btnRegion.SetWindowText(UIXAML_M_REGION_FALSE);
		m_btnElasticityRun.SetWindowTextW(UIXAML_M_RUN_ELASTICITY_FALSE);
	}
	UpdateData(FALSE);
	TxLogDebugEndMsg();
}


void KModeChoiceModelDlg::OnBnClickedRegion()
{
	TxLogDebugStartMsg();
	m_vecOriginZone.clear();
	m_vecDestinationZone.clear();

	KSelectRegionDlg oSelectRegionDlg;
	oSelectRegionDlg.SetTarget(m_pTarget);
	if (oSelectRegionDlg.DoModal() == IDOK)
	{
		oSelectRegionDlg.GetSelectZoneID(m_vecOriginZone, m_vecDestinationZone);
		m_btnElasticityRun.EnableWindow(TRUE);
		m_btnRegion.SetWindowText(UIXAML_M_REGION_SELECTED);
		m_btnElasticityRun.SetWindowTextW(UIXAML_M_RUN_ELASTICITY_DEFAULT);
	}
	TxLogDebugEndMsg();
}

void KModeChoiceModelDlg::OnBnClickedSelectvariable()
{
	TxLogDebugStartMsg();
	std::vector<TUseColumnInfo> vecUseColumnInfo;
	GetParaColumnName(vecUseColumnInfo);
	KSelectVariableDlg oSelectVariableDlg;
	oSelectVariableDlg.SetTarget(m_pTarget);
	oSelectVariableDlg.ParameterInfo(vecUseColumnInfo);
	if (oSelectVariableDlg.DoModal() == IDOK)
	{
		oSelectVariableDlg.GetChangedFunction(m_mapChangeFunction);
		m_btnSelectVariable.SetWindowText(UIXAML_M_SELECT_VARIABLE_SELECTED);

		m_btnRegion.EnableWindow(TRUE);
		m_btnRegion.SetWindowText(UIXAML_M_REGION_DEFAULT);
		
	}
	TxLogDebugEndMsg();
}

void KModeChoiceModelDlg::CalculateModel()
{	
	TxLogDebugStartMsg();
	bool  bResult = false;
	CTime timeStart, timeEnd;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pODTable = pIOTables->FindTable(TABLE_MODE_OD);    
	ASSERT(NULL != pODTable);

	std::map<CString, int> purposeODIndexMap;
	std::map<CString, int> paramODIndexMap;
	std::map<KMode*,  int> modeODIndexMap;

	timeStart = CTime::GetCurrentTime();

	// prepare purpose od data - 목적명 / 인덱스
	preparePurposeODIndex(purposeODIndexMap);
	size_t nODColumnCount = purposeODIndexMap.size();
	KODKeyDoubleRecords     purposeODRecords(nODColumnCount);
	preparePurposeODData(purposeODRecords);

	prepareParameterODIndex(paramODIndexMap);

	KIOColumns columnCollection;
	prepareModeIndexMap(modeODIndexMap);
	KODKeyDoubleRecords    modeODRecords(modeODIndexMap.size());

	if(false == m_bVariableGroup)
	{
		bResult = CalculateNonGroupValue(purposeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
	}
	else 
	{
		//bResult = runGroupValue(modeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
		bResult = false;
	}

	if (bResult == false)
		return; 

	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("수단선택 모형 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Mode Choice Model..."));
	}

	KLauncher::Run(strWindowName, 12);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("수단선택 모형 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Run Mode Choice Model Complete."));
		}
		TxLogDebug(_T("Model Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("수단선택 모형 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Model Stop.."));
        m_nResultCode = DLL_USER_STOPPING;
		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모형 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Model Run Error.."));

		return;
	}

	if (!addModeODColumns(modeODIndexMap, columnCollection))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data Failed."));
		}
		TxLogDebug(_T("ModeOD 정보 입력 실패..."));

		return;
	}
	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("수단 통행 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Mode O/D Data..."));
	}
	if (!ReadOutputFile(modeODRecords, modeODIndexMap))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("수단 통행 데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Saving Output Mode O/D Data Failed."));
		}
		TxLogDebug(_T("ModeOD 정보 입력 실패..."));
	}

	if (bResult)
	{
		bResult = KBulkDBase::BulkODUpsert(pODTable, columnCollection, modeODRecords);
		if (bResult)
		{
			timeEnd = CTime::GetCurrentTime();
			recordModeChoiceResult(timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
				timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		}
	}


}

bool KModeChoiceModelDlg::CalculateNonGroupValue( KODKeyDoubleRecords& purposeODRecords, 
												std::map<CString, int>& purposeODIndexMap, 
												std::map<CString, int>& paramODIndexMap, 
												std::map<KMode*, int>& modeODIndexMap, 
												KODKeyDoubleRecords& modeODRecords/*out*/ )
{
	
	ImChampDir::SetCurrentDirectoryToApp();
	TxLogDebugStartMsg();
	bool bResult = true;

	KODKeyDoubleRecords* pParameterOD = prepareParameterODData();
	if(NULL == pParameterOD)
	{
		return false;
	}

	//Argument 쓰기
	remove("ModeChoice.arg");
	CString strFileName = _T("ModeChoice.arg");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);
 
 	WriteZoneAgument(of);

	WritePurposeCountAgument(of);
 
 	int nModeCount = modeODIndexMap.size();
 

 	WriteModeCountAgument(of, nModeCount);
 
 	std::vector<KModelChoiceArgInfo> vecChoiceArgInfo;
 
 	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
 	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
 	{
 		KModeChoiceInfo* pInfo  = *itInfo;
 		KIOColumn* pInputColumn = pInfo->InputColumn();
 
 		std::map<CString, int>::iterator itIndex = purposeODIndexMap.find(pInputColumn->Name());
 		ASSERT(purposeODIndexMap.end() != itIndex);
 
 		int nPurposeODIndex = itIndex->second;

		bResult = WritePurposeInfoAgument(of, nPurposeODIndex);
 
 		bResult = CalculateModeTree(pInfo, paramODIndexMap, modeODIndexMap, *pParameterOD, nPurposeODIndex, of, nPurposeODIndex);

 	}
 	TxLogDebugVisitor();
 	of.Close();
 
 	WritePurposeODBinary();

	pParameterOD->RemoveAll();

	TxLogDebugEndMsg();
	return true;

}


bool KModeChoiceModelDlg::CalculateModeTree( KModeChoiceInfo* pInfo, std::map<CString, int>& paramIndexMap,
											std::map<KMode*, int>& modeIndexMap, KODKeyDoubleRecords& a_oParameterOD, 
											int a_nPurposeODIndex, CStdioFileEx& of, int a_nPurposeIndex)
{

	bool bResult = true;

	TxLogDebugStartMsg();

	// 정보 저장
	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*          pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);

	KMBaseVariable* pVariable = pInfo->Variable();
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);
	KMSimpleVariable* pSVariable = (KMSimpleVariable*)pVariable;

	KModeTree* pModeTree = pInfo->ModeTree();
	int nModeNodeCount = pModeTree->ModeNodeCount();

	std::vector<TFunctionInfo>	vecFunctionInfo;
	std::vector<int>			vecNotNestedArgInfo;
	std::vector<TNestedArgInfo>	vecTNestedArgInfo;

	for(int i = 0; i < nModeNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		KMode* pMode = pModeNode->Mode();
		std::map<KMode*, int>::iterator itModeIndex = modeIndexMap.find(pMode);
		int nModeIndex = itModeIndex->second;

		if(pModeNode->HasChild() == false)
		{
			KModeFunction* pFunction = pSVariable->ModeFunction(pMode->ModeID());
			KParseExpression oParseExpression;
			oParseExpression.SetTable(pTable);
			oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

			std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
			CString              strRealFunction   = oParseExpression.GetRealExpression();

			TFunctionInfo oFunction;
			oFunction.vecColumnName	 = vecUsedColumnName;
			KParser* oKParser = new KParser;
			oKParser->SetExpr(strRealFunction);
			oFunction.pParser = oKParser;
			vecFunctionInfo.push_back(oFunction);

			//NotNestdArgument 정보 입력
			vecNotNestedArgInfo.push_back(nModeIndex);
			
		}
		else
		{
			TNestedArgInfo oNestedArgInfo;
			oNestedArgInfo.dInclusiveValue = pSVariable->ModeParameter(pMode->ModeID());
			double ddd = pSVariable->ModeParameter(nModeIndex);

			std::vector<int> vecChildArgInfo;

			int nSubNodeCount = pModeNode->NodeCount();
			for(int j = 0; j < nSubNodeCount; ++j)
			{
				KModeTreeNodeMode* pSubModeNode = (KModeTreeNodeMode*)(pModeNode->Node(j));
				KMode* pSubMode = pSubModeNode->Mode();
				std::map<KMode*, int>::iterator itSubModeIndex = modeIndexMap.find(pSubMode);
				int nSubModeIndex = itSubModeIndex->second;

				KModeFunction* pFunction = pSVariable->ModeFunction(pSubMode->ModeID());
				KParseExpression oParseExpression;
				oParseExpression.SetTable(pTable);
				oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

				std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
				CString              strRealFunction   = oParseExpression.GetRealExpression();

				TFunctionInfo oFunction;
				oFunction.strFunction	 = strRealFunction;
				oFunction.vecColumnName	 = vecUsedColumnName;
				KParser* oKParser = new KParser;
				oKParser->SetExpr(strRealFunction);
				oFunction.pParser = oKParser;

				vecFunctionInfo.push_back(oFunction);
				vecChildArgInfo.push_back(nSubModeIndex);
			}

			//Nested Argument 정보 입력
			oNestedArgInfo.nParentIndex		= nModeIndex;
			oNestedArgInfo.vecChildIndex	= vecChildArgInfo;
			vecTNestedArgInfo.push_back(oNestedArgInfo);
		}

	}

	//Argument 입력
	bResult = WriteNotNestedInfoAgument(of, vecNotNestedArgInfo);

	bResult = WriteNestdInfoAgument(of, vecTNestedArgInfo);

	CString strFileName = _T("");
	strFileName.Format(_T("Utility_Mode_Choice%d.dat"), a_nPurposeIndex);

	const char* charFileName = (char*)(LPCTSTR)strFileName;

	remove(charFileName);
	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
	}

	// 수식에 따른 계산 시작 및 Binary 쓰기
	KODKeyDoubleRecordsIter iter = a_oParameterOD.begin();
	KODKeyDoubleRecordsIter end  = a_oParameterOD.end();

	while(iter != end)
	{
		KODKey oKey					= iter->first;
		KSDoubleRecord* recordOne	= iter->second;

		Integer nxOZoneID =  oKey.OriginID;
		Integer nxDZoneID =  oKey.DestinationID;

// 		if (nxOZoneID == 1 || nxDZoneID == 1)
// 		{
// 			AfxMessageBox(_T("에러 "));
// 		}

		ofs.write(reinterpret_cast<char*>( &nxOZoneID ), sizeof(Integer) );
		ofs.write(reinterpret_cast<char*>( &nxDZoneID ), sizeof(Integer) ); 

		int nFunctionIndex = vecFunctionInfo.size();
		for(int i = 0; i < nFunctionIndex; i++)
		{
			TFunctionInfo oFunction				= vecFunctionInfo[i];
			std::vector<CString> vecUseColumn	= oFunction.vecColumnName;
			//CString strRealFunction				= oFunction.strFunction;
			KParser* pParser = oFunction.pParser;

			size_t nxCount   = vecUseColumn.size();
			double* dVariable       = new double[nxCount];
			
			for (size_t j=0; j<nxCount; j++)
			{
				CString strColumnName	= vecUseColumn[j];
				std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(strColumnName);
				if(paramIndexMap.end() != itParamIndex)
				{
					int nParamIndex = itParamIndex->second;
					pParser->DefineVar(strColumnName, &dVariable[j]);
					dVariable[j] = recordOne->GetAt(nParamIndex);
				}
			}
			double dValue = pParser->Eval();
			ofs.write(reinterpret_cast<char*>( &dValue), sizeof(double) );
			delete[] dVariable;
		}

		++iter;
	}

	int nFunctionIndex = vecFunctionInfo.size();
	for(int i = 0; i < nFunctionIndex; i++)
	{
		TFunctionInfo oFunction				= vecFunctionInfo[i];
		KParser* pParser = oFunction.pParser;
		delete pParser;
	}

	ofs.close();
	return bResult;
}


void KModeChoiceModelDlg::GetParaColumnName(std::vector<TUseColumnInfo>& vecUseColumnInfo)
{
	TxLogDebugStartMsg();
	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
	{
		 KModeChoiceInfo* pInfo = *itInfo;

		 KIOColumn* pInputColumn = pInfo->InputColumn();
		 CString strPurposeName  = pInputColumn->Name();
		 KIOTables*   pIOTables = m_pTarget->Tables();
		 KIOTable*          pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);

		 KMBaseVariable* pVariable = pInfo->Variable();
		 ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);
		 KMSimpleVariable* pSVariable = (KMSimpleVariable*)pVariable;

		 KModeTree* pModeTree = pInfo->ModeTree();
		 int nModeNodeCount = pModeTree->ModeNodeCount();

		 for(int i = 0; i < nModeNodeCount; ++i)
		 {
			 KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
			 KMode* pMode = pModeNode->Mode();

			 if(pModeNode->HasChild() == false)
			 {
				 KModeFunction* pFunction = pSVariable->ModeFunction(pMode->ModeID());
				 KParseExpression oParseExpression;
				 oParseExpression.SetTable(pTable);
				 oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

				 std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
				 CString              strRealFunction   = oParseExpression.GetRealExpression();

				 CString strModeName = pMode->ModeName();
				 
				 TUseColumnInfo oUseColumInfo;
				 oUseColumInfo.strPurposeName	 = strPurposeName;
				 oUseColumInfo.strModeNodeName	 = strModeName;
				 oUseColumInfo.vecUsedColumnName = vecUsedColumnName;

				 vecUseColumnInfo.push_back(oUseColumInfo);
				 
			 }
			 else
			 {
				 int nSubNodeCount = pModeNode->NodeCount();
				 for(int j = 0; j < nSubNodeCount; ++j)
				 {
					 KModeTreeNodeMode* pSubModeNode = (KModeTreeNodeMode*)(pModeNode->Node(j));
					 KMode* pSubMode = pSubModeNode->Mode();
		
					 KModeFunction* pFunction = pSVariable->ModeFunction(pSubModeNode->ModeID());
					 KParseExpression oParseExpression;
					 oParseExpression.SetTable(pTable);
					 oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

					 std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
					 CString              strRealFunction   = oParseExpression.GetRealExpression();

					 CString strModeName = pSubMode->ModeName();

					 TUseColumnInfo oUseColumInfo;
					 oUseColumInfo.strPurposeName	 = strPurposeName;
					 oUseColumInfo.strModeNodeName	 = strModeName;
					 oUseColumInfo.vecUsedColumnName = vecUsedColumnName;

					 vecUseColumnInfo.push_back(oUseColumInfo);
				 }

			 }
		 }
	}
	TxLogDebugEndMsg();
}

bool KModeChoiceModelDlg::WriteZoneAgument( CStdioFileEx& of )
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("select count(Zone_id) from zone ");
	try
	{
		int nZoneCount = 0;
        KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);	

		while(pResult->Next())
		{
			nZoneCount = pResult->GetValueInt(0);
		}

		CString strOut = _T("");
		strOut.Format(_T("noZone\t%d\r\n"), nZoneCount);
		TxLogDebug(strOut);
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	
	return true;
}

bool KModeChoiceModelDlg::WritePurposeCountAgument( CStdioFileEx& of )
{
	try
	{
		int nCountPurpose = m_MInfoVector.size();
		CString strOut = _T("");
		strOut.Format(_T("noPurpose\t%d\r\n"), nCountPurpose);
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KModeChoiceModelDlg::WriteModeCountAgument( CStdioFileEx& of, int a_nModeCount )
{
	try
	{
		CString strOut = _T("");
		strOut.Format(_T("noMode\t%d\r\n"), a_nModeCount);
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KModeChoiceModelDlg::WritePurposeInfoAgument( CStdioFileEx& of, int a_nPurposeIndex )
{
	try
	{
		CString strOut = _T("");
		strOut.Format(_T("PurposeIndex\t%d\r\n"), a_nPurposeIndex);
		of.WriteString(strOut);

		CString strFileName = _T("");
		strFileName.Format(_T("FileName\tUtility_Mode_Choice%d.dat\r\n"), a_nPurposeIndex);
		of.WriteString(strFileName);

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KModeChoiceModelDlg::WriteElisticityPurposeInfoAgument( CStdioFileEx& of, int a_nPurpposeIndex )
{
	try
	{
		CString strOut = _T("");
		strOut.Format(_T("PurposeIndex\t%d\r\n"), a_nPurpposeIndex);
		of.WriteString(strOut);

		/*CString strFileName = _T("");
		strFileName.Format(_T("FileName\tUtility_Mode_Choice%d.dat\r\n"), a_nPurpposeIndex);
		of.WriteString(strFileName);*/

		CString strElistFileName = _T("");
		strElistFileName.Format(_T("FileName\tUtility_Elasticity%d.dat\r\n"), a_nPurpposeIndex);
		of.WriteString(strElistFileName);

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KModeChoiceModelDlg::WriteNotNestedInfoAgument( CStdioFileEx& of, std::vector<int> a_vecNotNestedArgInfo )
{
	try
	{
		int nCountNotNested = a_vecNotNestedArgInfo.size();
		if (nCountNotNested == 0)
		{
			return false;
		}
		CString strOut		= _T("");
		strOut.Format(_T("noNotNested\t%d\r\n"), nCountNotNested);
		of.WriteString(strOut);

		for(int i = 0; i < nCountNotNested; i++ )
		{
			int nModeIndex  = a_vecNotNestedArgInfo[i];
			CString strMode = _T("");
			strMode.Format(_T("ModeIndex\t%d\r\n"), nModeIndex);
			of.WriteString(strMode);
		}

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KModeChoiceModelDlg::WriteNestdInfoAgument( CStdioFileEx& of, std::vector<TNestedArgInfo>& a_vecNestedArgInfo )
{
	try
	{
		int nCountNested = a_vecNestedArgInfo.size();

		if (nCountNested == 0)
		{
			CString strNestedCount = _T("");
			strNestedCount.Format(_T("noNested\t%d\r\n"), nCountNested);
			of.WriteString(strNestedCount);
			return true;
		}

		CString strNestedCount = _T("");
		strNestedCount.Format(_T("noNested\t%d\r\n"), nCountNested);
		of.WriteString(strNestedCount);

		for (int i = 0; i< nCountNested; i++)
		{
			CString strNestedIndex = _T("");
			strNestedIndex.Format(_T("NestedIndex\t%d\r\n"), i+1);
			of.WriteString(strNestedIndex);

			TNestedArgInfo oNestedArgInfo = a_vecNestedArgInfo[i];

			std::vector<int> vecChildInfo	= oNestedArgInfo.vecChildIndex;
			
			int nChildCount					= vecChildInfo.size();
			CString strChildCount			= _T("");
			strChildCount.Format(_T("noChild\t%d\r\n"), nChildCount);
			of.WriteString(strChildCount);

			int nParentIndex				= oNestedArgInfo.nParentIndex;
			CString strParentIndex			= _T("");
			strParentIndex.Format(_T("ParentIndex\t%d\r\n"), nParentIndex);
			of.WriteString(strParentIndex);

			for (int j = 0; j < nChildCount; j++)
			{
				int nChildIndex = vecChildInfo[j];
				CString strChildIndex			= _T("");
				strChildIndex.Format(_T("ChildIndex\t%d\r\n"), nChildIndex);
				of.WriteString(strChildIndex);
			}

			double dInclusiveValue			= oNestedArgInfo.dInclusiveValue;
			CString strInclusiveValue		= _T("");
			strInclusiveValue.Format(_T("InclusiveValue\t%f\r\n"), dInclusiveValue);
			of.WriteString(strInclusiveValue);
		}

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;

}

void KModeChoiceModelDlg::WritePurposeODBinary()
{
	KIOColumns PurposeColumnCollection;

	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo  = *itInfo;
		KIOColumn* pInputColumn = pInfo->InputColumn();
		if (pInputColumn)
		{
			int nColumnIndex = PurposeColumnCollection.AddNewColumn(pInputColumn);
		}
	}

	KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), _T("purpose_od"), PurposeColumnCollection, _T("Purposeod.dat"));
}

unsigned __stdcall KModeChoiceModelDlg::ModeChoiceThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("수단선택 모형 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Running Mode Choice Model..."));
	}

	KModeChoiceModelDlg* pDlg = (KModeChoiceModelDlg*)pParameter->GetParameter();;

	pDlg->CalculateModel();

	return 0;
}

void KModeChoiceModelDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KModeChoiceModelDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	AddStatusMessage(CString(strMsg));
}


void KModeChoiceModelDlg::OnBnClickedRunelasticity()
{
	TxLogDebugStartMsg();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(ElasticityThreadCaller, this, false, nLang);

	m_btnElasticityRun.SetWindowText(UIXAML_M_REGION_SELECTED);
	m_btnViewLog.EnableWindow(TRUE);
	m_btnViewLog.SetWindowTextW(UIXAML_M_VIEW_LOG_DEFAULT);
	KRedrawWindow();
	TxLogDebugEndMsg();
}

unsigned __stdcall KModeChoiceModelDlg::ElasticityThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("Elasticity 구동 .... "));

	KModeChoiceModelDlg* pDlg = (KModeChoiceModelDlg*)pParameter->GetParameter();;

	pDlg->CalculateElasticityModel();

	return 0;
}

void KModeChoiceModelDlg::CalculateElasticityModel()
{
	TxLogDebugStartMsg();
	bool  bResult = false;
	CTime timeStart, timeEnd;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pODTable = pIOTables->FindTable(TABLE_MODE_OD);    
	ASSERT(NULL != pODTable);

	std::map<CString, int> purposeODIndexMap;
	std::map<CString, int> paramODIndexMap;
	std::map<KMode*,  int> modeODIndexMap;

	timeStart = CTime::GetCurrentTime();

	// prepare purpose od data - 목적명 / 인덱스
	preparePurposeODIndex(purposeODIndexMap);
	size_t nODColumnCount = purposeODIndexMap.size();
	KODKeyDoubleRecords     purposeODRecords(nODColumnCount);
	preparePurposeODData(purposeODRecords);

	prepareParameterODIndex(paramODIndexMap);

	KIOColumns columnCollection;
	prepareModeIndexMap(modeODIndexMap);
	KODKeyDoubleRecords    modeODRecords(modeODIndexMap.size());

	if(false == m_bVariableGroup)
	{
		bResult = CalculateNonGroupElasticity(purposeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
	}
	else 
	{
		//bResult = runGroupValue(modeODRecords, purposeODIndexMap, paramODIndexMap, modeODIndexMap, modeODRecords);
		bResult = false;
	}

	if (bResult == false)
	{
		return; 
	}

	HINSTANCE hIns = LoadLibrary(_T("FElasticity.dll"));
	if( hIns == NULL )
	{
		AfxMessageBox(_T("FElasticity.dll이 없습니다."));
		return ;
	}

	FModeChoiceProPtr = (FELASTICITY)(GetProcAddress(hIns,"FELASTICITY"));
	if (NULL == FModeChoiceProPtr)
	{
		AfxMessageBox(_T("FELASTICITY 함수가 없습니다"));
		FreeLibrary(hIns);
		return ;
	}

	int nResult = 0;

	try
	{
		nResult = FModeChoiceProPtr((void*)this, (void*)ReceiveStatusMessageFromDll);
	}
	catch (...)
	{
		TxLogDebugException();
		FreeLibrary(hIns);
		return;
	}

	if(NULL != hIns)
	{
		::FreeLibrary(hIns);
		TxLogDebug(_T("FreeLibrary"));
	}

	if( 1 == nResult /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("수단선택 모형 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Run Mode Choice Model Complete."));
		}
		TxLogDebug(_T("Run Ok.."));
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모형 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Run Error.."));
		return;
	}

	return;
}

bool KModeChoiceModelDlg::CalculateNonGroupElasticity( KODKeyDoubleRecords& purposeODRecords, std::map<CString, int>& purposeODIndexMap, 
														std::map<CString, int>& paramODIndexMap, std::map<KMode*, int>& modeODIndexMap,
														KODKeyDoubleRecords& modeODRecords/*out*/ )
{
	TxLogDebugStartMsg();
	bool bResult = true;

	KODKeyDoubleRecords* pParameterOD = prepareParameterODData();
	if(NULL == pParameterOD)
	{
		return false;
	}

	//Argument 쓰기
	remove("ModeChoice.arg");
	CString strFileName = _T("ModeChoice.arg");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	WriteZoneAgument(of);

	WritePurposeCountAgument(of);

	int nModeCount = modeODIndexMap.size();

	WriteModeCountAgument(of, nModeCount);

	std::vector<KModelChoiceArgInfo> vecChoiceArgInfo;

	std::vector<KModeChoiceInfo*>::iterator itInfo, itInfoEnd = m_MInfoVector.end();
	for(itInfo = m_MInfoVector.begin(); itInfo != itInfoEnd; ++itInfo)
	{
		KModeChoiceInfo* pInfo  = *itInfo;
		KIOColumn* pInputColumn = pInfo->InputColumn();

		std::map<CString, int>::iterator itIndex = purposeODIndexMap.find(pInputColumn->Name());
		ASSERT(purposeODIndexMap.end() != itIndex);

		int nPurposeODIndex = itIndex->second;

		bResult = WriteElisticityPurposeInfoAgument(of, nPurposeODIndex);

		bResult = CalculateElasticityTree(pInfo, paramODIndexMap, modeODIndexMap, *pParameterOD, nPurposeODIndex, of, nPurposeODIndex);

	}

	WriteZoneInfoAgument(of);
	TxLogDebugVisitor();
	of.Close();

	WritePurposeODBinary();
	TxLogDebugEndMsg();

	pParameterOD->RemoveAll();
	return true;
}

bool KModeChoiceModelDlg::CalculateElasticityTree( KModeChoiceInfo* pInfo, std::map<CString, int>& paramIndexMap, std::map<KMode*, int>& modeIndexMap, KODKeyDoubleRecords& a_oParameterOD, int a_nPurposeODIndex, CStdioFileEx& of, int a_nPurposeIndex )
{
	bool bResult = true;

	TxLogDebugStartMsg();

	// 정보 저장
	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*          pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);

	KMBaseVariable* pVariable = pInfo->Variable();
	ASSERT(pVariable->ModeVariableType() == KEMModeVariableTypeSimple);
	KMSimpleVariable* pSVariable = (KMSimpleVariable*)pVariable;

	KModeTree* pModeTree = pInfo->ModeTree();
	int nModeNodeCount = pModeTree->ModeNodeCount();
	std::vector<TFunctionInfo>	vecFunctionInfo;
	std::vector<int>			vecNotNestedArgInfo;
	std::vector<TNestedArgInfo>	vecTNestedArgInfo;

	for(int i = 0; i < nModeNodeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = pModeTree->ModeNode(i);
		KMode* pMode = pModeNode->Mode();
		std::map<KMode*, int>::iterator itModeIndex = modeIndexMap.find(pMode);
		int nModeIndex = itModeIndex->second;

		if(pModeNode->HasChild() == false)
		{
			KModeFunction* pFunction = pSVariable->ModeFunction(pMode->ModeID());
			KParseExpression oParseExpression;
			oParseExpression.SetTable(pTable);
			oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

			std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
			CString              strRealFunction   = oParseExpression.GetRealExpression();
			CString				 strChangeFunction = ChangeRealFunction(strRealFunction);

			TFunctionInfo oFunction;
			oFunction.vecColumnName	 = vecUsedColumnName;
			
			KParser* pKParser = new KParser;
			pKParser->SetExpr(strChangeFunction);
			oFunction.pParser = pKParser;

			/*KParser* pChangeParser = new KParser;
			pChangeParser->SetExpr(strChangeFunction);
			oFunction.pChangeParser = pChangeParser;*/

			vecFunctionInfo.push_back(oFunction);

			//NotNestdArgument 정보 입력
			vecNotNestedArgInfo.push_back(nModeIndex);
		}
		else
		{
			TNestedArgInfo oNestedArgInfo;
			oNestedArgInfo.dInclusiveValue = pSVariable->ModeParameter(pMode->ModeID());
			std::vector<int> vecChildArgInfo;

			int nSubNodeCount = pModeNode->NodeCount();
			for(int j = 0; j < nSubNodeCount; ++j)
			{
				KModeTreeNodeMode* pSubModeNode = (KModeTreeNodeMode*)(pModeNode->Node(j));
				KMode* pSubMode = pSubModeNode->Mode();
				std::map<KMode*, int>::iterator itSubModeIndex = modeIndexMap.find(pSubMode);
				int nSubModeIndex = itSubModeIndex->second;

				KModeFunction* pFunction = pSVariable->ModeFunction(pSubMode->ModeID());
				KParseExpression oParseExpression;
				oParseExpression.SetTable(pTable);
				oParseExpression.SetCaptionExpression(pFunction->GetCaptionExpression());

				std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
				CString              strRealFunction   = oParseExpression.GetRealExpression();
				CString				 strChangeFunction = ChangeRealFunction(strRealFunction);

				TFunctionInfo oFunction;
				oFunction.strFunction	 = strRealFunction;
				oFunction.vecColumnName	 = vecUsedColumnName;
				
				KParser* oKParser = new KParser;
				oKParser->SetExpr(strChangeFunction);
				oFunction.pParser = oKParser;

				/*KParser* pChangeParser = new KParser;
				pChangeParser->SetExpr(strChangeFunction);
				oFunction.pChangeParser = pChangeParser;*/

				vecFunctionInfo.push_back(oFunction);
				vecChildArgInfo.push_back(nSubModeIndex);
			}

			//Nested Argument 정보 입력
			oNestedArgInfo.nParentIndex		= nModeIndex;
			oNestedArgInfo.vecChildIndex	= vecChildArgInfo;
			vecTNestedArgInfo.push_back(oNestedArgInfo);
			
		}
	}

	
	//Argument 입력
	bResult = WriteNotNestedInfoAgument(of, vecNotNestedArgInfo);

	bResult = WriteNestdInfoAgument(of, vecTNestedArgInfo);

	CString strEFileName = _T("");
	strEFileName.Format(_T("Utility_Elasticity%d.dat"), a_nPurposeIndex);
	const char* charEFileName = (char*)(LPCTSTR)strEFileName;
	remove(charEFileName);
	ofstream ofse(strEFileName, ios::binary);
	if (! ofse)
	{
		TxLogDebugException();
	}

	// 수식에 따른 계산 시작 및 Binary 쓰기
	KODKeyDoubleRecordsIter iter = a_oParameterOD.begin();
	KODKeyDoubleRecordsIter end  = a_oParameterOD.end();

	while(iter != end)
	{
		KODKey oKey					= iter->first;
		KSDoubleRecord* recordOne	= iter->second;

		Integer nxOZoneID =  oKey.OriginID;
		Integer nxDZoneID =  oKey.DestinationID;

		ofse.write(reinterpret_cast<char*>( &nxOZoneID ), sizeof(Integer) );
		ofse.write(reinterpret_cast<char*>( &nxDZoneID ), sizeof(Integer) ); 

		int nFunctionIndex = vecFunctionInfo.size();
		for(int i = 0; i < nFunctionIndex; i++)
		{
			TFunctionInfo oFunction				= vecFunctionInfo[i];
			std::vector<CString> vecUseColumn	= oFunction.vecColumnName;

			KParser* pParser = oFunction.pParser;

			size_t nxCount   = vecUseColumn.size();
			double* dVariable       = new double[nxCount];

			for (size_t j=0; j<nxCount; j++)
			{
				CString strColumnName	= vecUseColumn[j];
				std::map<CString, int>::iterator itParamIndex = paramIndexMap.find(strColumnName);
				if(paramIndexMap.end() != itParamIndex)
				{
					int nParamIndex = itParamIndex->second;
					pParser->DefineVar(strColumnName, &dVariable[j]);
					dVariable[j] = recordOne->GetAt(nParamIndex);
				}
			}
			double dValue = pParser->Eval();
			ofse.write(reinterpret_cast<char*>( &dValue), sizeof(double) );
			delete[] dVariable;
		}
		++iter;
	}

	int nFunctionIndex = vecFunctionInfo.size();
	for(int i = 0; i < nFunctionIndex; i++)
	{
			TFunctionInfo oFunction				= vecFunctionInfo[i];
			KParser* pParser = oFunction.pParser;
			delete pParser;
	}
	
	ofse.close();
	return bResult;
}

CString KModeChoiceModelDlg::ChangeRealFunction( CString a_strFunction )
{
	std::map<CString, CString>::iterator iter = m_mapChangeFunction.begin();
	std::map<CString, CString>::iterator end =  m_mapChangeFunction.end();
	while(iter != end)
	{
		CString strOriginFunction = iter->first;
		CString strChangeFunction = iter->second;

		a_strFunction.Replace(strOriginFunction, strChangeFunction);
		++iter;
	}
	return a_strFunction;
}

bool KModeChoiceModelDlg::WriteZoneInfoAgument(CStdioFileEx& of)
{
	int nOriginZoneCount = m_vecOriginZone.size();

	CString strOriginCount		= _T("");
	strOriginCount.Format(_T("noOriginZone\t%d\r\n"), nOriginZoneCount);
	of.WriteString(strOriginCount);

	for (int i = 0; i < nOriginZoneCount ; i++)
	{
		Integer nxOZoneID = m_vecOriginZone[i];
		int nIndex = i+ 1;
		CString strZone		= _T("");
		//strZone.Format(_T("ZoneID%d\t%d\r\n"),nIndex, nxOZoneID);
		strZone.Format(_T("%d\r\n"), nxOZoneID);
		of.WriteString(strZone);

	}

	int nDestinationZoneCount = m_vecDestinationZone.size();

	CString strDestinationZoneCount		= _T("");
	strDestinationZoneCount.Format(_T("noDestZone\t%d\r\n"), nDestinationZoneCount);
	of.WriteString(strDestinationZoneCount);

	for (int i = 0; i < nDestinationZoneCount ; i++)
	{
		Integer nxDZoneID = m_vecDestinationZone[i];
		int nIndex = i+ 1;
		CString strZone		= _T("");
		//strZone.Format(_T("ZoneID%d\t%d\r\n"),nIndex, nxDZoneID);
		strZone.Format(_T("%d\r\n"), nxDZoneID);
		of.WriteString(strZone);

	}
	
	return true;
}

bool KModeChoiceModelDlg::ReadOutputFile(KODKeyDoubleRecords& a_oModeODResult, std::map<KMode*, int>& modeODIndexMap)
{
	std::ifstream ifs( _T("ModeOD.dat"), std::ios::binary);

	if (!ifs.is_open())
	{
		throw -1;
	}

	if (ifs)
	{
		TxLogDebug(_T("--- start insert DB result"));
		while(! ifs.eof())
		{
			if (ifs.eof())
			{
				break;
			}

			size_t nCount = modeODIndexMap.size();

			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );

			Integer nxDestination;
			ifs.read( reinterpret_cast<char*>( &nxDestination),        sizeof(Integer)  );

			KODKey oODKey(nxOrigin, nxDestination);
			KSDoubleRecord* pOutPutResult = a_oModeODResult.AddRecord(oODKey);

			for(size_t i = 0; i < nCount; ++i)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );
				pOutPutResult->SetAt(i, dRecord);

			}
		}
		
	}
	else
	{
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}



void KModeChoiceModelDlg::OnBnClickedLogview()
{
	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(MODECHOICE_LOGFILE_NAME))
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("%s 파일이 존재하지 않습니다."), MODECHOICE_LOGFILE_NAME);
		AfxMessageBox(strMsg);
		return;
	}
	ShellExecute(NULL, _T("open"), MODECHOICE_LOGFILE_NAME, NULL, NULL, SW_SHOW);    // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
}

BOOL KModeChoiceModelDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
