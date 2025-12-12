/**
 * @file DistributionModelDlg.cpp
 * @brief KDistributionModelDlg 구현 파일
 * @author 
 * @date 2011.05.19
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DistributionModelDlg.h"
#include "afxdialogex.h"

#include "DistributionInfo.h"
#include "DBaseODDlg.h"
#include "DImpedanceFunctionDlg.h"
#include "DInputDlg.h"
#include "DOutputDesignDlg.h"
#include "DSelectModelDlg.h"
#include "DSetStoppingDlg.h"
#include "IOTableController.h"
#include "PAPurpose.h"
#include "Purpose.h"
#include "PurposeManager.h"
#include "Target.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "IOTableView.h"
#include "IOView.h"
#include "KBulkDBase.h"
#include "KUIDefDistribution.h"
#include "DistributionFunctionBase.h"
#include "DistributionFunction1.h"
#include "DistributionFunction2.h"
#include "DistributionFunction3.h"
#include "DefineNamesDefaultPara.h"
#include "DefineNamesModels.h"
#include "XmlManager.h"
#include "KEMParameter.h"
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "ImChampDir.h"

#include "Launcher.h"
#include "PurposeInfoCollection.h"
using namespace UIDistribution;


IMPLEMENT_DYNAMIC(KDistributionModelDlg, KDialogEx)

KDistributionModelDlg::KDistributionModelDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDistributionModelDlg::IDD, pParent)
{
	m_emODType = KEMDODTypePurpose;
	m_nFratarIteration = 10;
	m_dFratarGap = 0.01f;
	m_nGravityIteration = 10;
	m_dGravityError = 0.01f;
	m_pTarget = NULL;
	m_pZoneTable = NULL;
}

KDistributionModelDlg::~KDistributionModelDlg()
{
    TxLogDebugStartMsg();
    clearDistributionInfo();
	TxLogDebugEndMsg();
}

void KDistributionModelDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MODEL, m_btnModel);
    DDX_Control(pDX, IDC_INPUT, m_btnInput);
    DDX_Control(pDX, IDC_BASEOD, m_btnBaseOD);
    DDX_Control(pDX, IDC_VARIABLE, m_btnVariable);
    DDX_Control(pDX, IDC_CRITERIA, m_btnCriteria);
    DDX_Control(pDX, IDC_OUTPUTDESIGN, m_btnOutputDesign);
    DDX_Control(pDX, IDC_OUTPUTTABLE, m_btnViewTable);
    DDX_Control(pDX, IDC_BUTTON1, m_btnRun);
    DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
    DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
    DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
    DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
    DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow4);
    DDX_Control(pDX, IDC_STATIC_ARROW6, m_uiArrow6);    
    DDX_Control(pDX, IDC_STATIC_INFORMATION, m_hyperLink);
}


BEGIN_MESSAGE_MAP(KDistributionModelDlg, KDialogEx)
	ON_BN_CLICKED(IDC_MODEL, &KDistributionModelDlg::OnBnClickedModel)
	ON_BN_CLICKED(IDC_INPUT, &KDistributionModelDlg::OnBnClickedInput)
	ON_BN_CLICKED(IDC_BASEOD, &KDistributionModelDlg::OnBnClickedBaseod)
	ON_BN_CLICKED(IDC_VARIABLE, &KDistributionModelDlg::OnBnClickedVariable)
	ON_BN_CLICKED(IDC_CRITERIA, &KDistributionModelDlg::OnBnClickedCriteria)
	ON_BN_CLICKED(IDC_OUTPUTDESIGN, &KDistributionModelDlg::OnBnClickedOutputdesign)
	ON_BN_CLICKED(IDC_BUTTON1, &KDistributionModelDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BUTTON2, &KDistributionModelDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_OUTPUTTABLE, &KDistributionModelDlg::OnBnClickedOutputtable)
    //ON_WM_ACTIVATE() - markup button focus 있을때 외부 클릭 시 버튼 사라짐 문제 때문에 기능 삭제
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


void KDistributionModelDlg::SetTarget(KTarget* pTarget)
{
    TxLogDebugStartMsg();
	m_pTarget = pTarget;
	KIOTables* pIOTables = pTarget->Tables();
	if(NULL != pIOTables)
	{
		m_pZoneTable = pIOTables->FindTable(_T("zone"));
	}

	InitDistributionParaInfo();
	initPurposeList();
	initPurposeInfos();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::initPurposeList(void)
{
    TxLogDebugStartMsg();
	KPurposeManager* pPurposeMgr = m_pTarget->PurposeManager();
	ASSERT(NULL != pPurposeMgr);

	m_PAPurposeList.clear();
	int nPurposeCount = pPurposeMgr->GetPurposeCount();
	for(int i = 0; i < nPurposeCount; ++i)
	{
		KPAPurpose* pPurpose = pPurposeMgr->GetPurpose(i);
		m_PAPurposeList.push_back(pPurpose);
	}
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::initPurposeInfos(void)
{
    TxLogDebugStartMsg();
	clearPurposeInfos();
	std::list<KPAPurpose*>::iterator itPurpose, itEnd = m_PAPurposeList.end();
	for(itPurpose = m_PAPurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPAPurpose* pPurpose = *itPurpose;
		std::map<int, KDistributionInfo*>::iterator itInfo = m_purposeInfoMap.find(pPurpose->PurposeID());
		if(m_purposeInfoMap.end() == itInfo)
		{
			KDistributionInfo* pInfo = new KDistributionInfo;
			pInfo->PurposeGroupID(pPurpose->PurposeGroup());
			pInfo->PurposeID(pPurpose->PurposeID());
			pInfo->PurposeName(pPurpose->PurposeName());
			InitDefultFunction(pInfo);

			m_purposeInfoMap.insert(std::make_pair(pInfo->PurposeID(), pInfo));
		}
	}
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::clearPurposeInfos(void)
{
    TxLogDebugStartMsg();
	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;
		delete pInfo;
	}

	m_purposeInfoMap.clear();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::clearDistributionInfo(void)
{
    TxLogDebugStartMsg();
	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;
		delete pInfo;
	}

	m_purposeInfoMap.clear();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::runDistributionModel(void)
{
    TxLogDebugStartMsg();
    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    bool bResult = true;

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable*        pODTable      = pIOTables->FindTable(_T("purpose_od"));
    ASSERT(NULL != pODTable);

    CTime timeStart = CTime::GetCurrentTime();
    CTime timeEnd   = CTime::GetCurrentTime();
    
    size_t nSelectedCount = 0;
    while ( iter != end )
    {
        KDistributionInfo* pDInfo = iter->second;
        if (pDInfo->Selected())
        {
            nSelectedCount ++;
        }

        ++iter;
    }
    
    KODKeyDoubleRecords resultRecords(nSelectedCount);
    size_t nIndex = 0;

    for(iter = m_purposeInfoMap.begin(); iter != end; ++iter)
    {
        KDistributionInfo* pDInfo = iter->second;
        if(pDInfo->Selected() == false)
        {
            continue;
        }

        KEMDistributionModel emDModel = pDInfo->ModelType();

        if(KEMDistributionModelFratar == emDModel)
        {
            if(runFratar(pDInfo, resultRecords, nIndex) == false)
            {
                bResult = false;
                break;
            }

            nIndex++;
        }
        else
        {
            //if(runGravity(pDInfo) == false)
            //{
            //	bCommit = false;
            //	break;
            //}
        }
        
        
    }

    if ( ! bResult )
    {
        AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
        return;
    }

    KIOColumns columnCollection;

    for(iter = m_purposeInfoMap.begin(); iter != end; ++iter)
    {
        KDistributionInfo* pDInfo = iter->second;      
        if (pDInfo->Selected())
        {    
            // column 존재 여부 확인 및 존재하지 않을 경우 컬럼 생성
            const KIOColumns* pColumns = pODTable->Columns();
            KIOColumn* pOutputColumn = pColumns->GetColumn(pDInfo->OutputPurposeODColumnName());
            if(NULL == pOutputColumn)
            {
                KIOColumn column;
                column.Name(pODTable->GetUsableColumnName());
                column.Caption(pDInfo->OutputPurposeODColumnCaption());
                column.CodeType(KEMIOCodeTypeIsNothing);
                column.ColumnType(KEMIOColumnTypeUserDefine);
                column.DataType(KEMIODataTypeDouble);
                column.DisplayOrder(pColumns->ColumnCount()+1);
                column.PrimaryKey(false);
                column.Visible(true);
                pOutputColumn = pODTable->AddColumn(column);
                if(NULL == pOutputColumn)
                {                    
                    bResult = false;
                    break;
                }

                pDInfo->OutputPurposeODColumnName(pOutputColumn->Name());
            }
            columnCollection.AddNewColumn(pOutputColumn);
        }
    }

    if ( ! bResult )
    {
        AfxMessageBox(_T("컬럼 추가 중 오류가 발생하였습니다."));
        return;
    }

    KBulkDBase oBulkDbase;
    //bResult  = oBulkDbase.BulkODUpdate(pODTable, columnCollection, resultRecords);

    bResult  = oBulkDbase.BulkODUpsert(pODTable, columnCollection, resultRecords);

    if(true == bResult)
    {
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnViewTable.SetWindowText(KR_UIXAML_D_VTABLE_DEFAULT);
		}
		else {
			m_btnViewTable.SetWindowText(UIXAML_D_VTABLE_DEFAULT);
		}
        m_btnViewTable.EnableWindow(TRUE);
       // m_btnDrawGraph.SetWindowText(UIXAML_D_DGRAPH_DEFAULT);
       // m_btnDrawGraph.EnableWindow(TRUE);
    }
    else
    {
        AfxMessageBox(_T("데이터 입력 중 오류가 발생하였습니다."));
    }

    timeEnd = CTime::GetCurrentTime();
    for(iter = m_purposeInfoMap.begin(); iter != end; ++iter)
    {
        KDistributionInfo* pDInfo = iter->second;   
        if (pDInfo->Selected())
        {
            bResult = recordDistributionResult(pDInfo, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
        }        
    }

    TxLogDebugEndMsg();
}


bool KDistributionModelDlg::runFratar(KDistributionInfo* pDInfo, KODKeyDoubleRecords& resultRecord, size_t index)
{
    TxLogDebugStartMsg();
    bool bResult = false;
    KTarget*   pBaseODTarget = pDInfo->BaseODTarget();
    KIOColumn* pBaseODColumn = pDInfo->BaseODColumn();

    KDistributionZoneDataCollection zoneData;
    std::map<KODKey, Double> baseODMap, resultODMap;

    KxDistributionFratar_Old fratar;
    int nResult = 0;

    if((prepareZoneData(pDInfo, zoneData) == true) && 
        (prepareBaseOD(pDInfo, baseODMap) == true))
    {
        if(fratar.MuDistributionFratarExecute(zoneData, baseODMap, m_nFratarIteration, m_dFratarGap/100.0f, resultODMap, ReceiveModelMsg) == 1)
        {
            //bResult = updateDistributionValue(pDInfo, resultODMap);
            resultRecord.SetODKeyValue(resultODMap, index);
            bResult = true;
        }
        else
        {
            bResult = false;
        }
    }
        
    TxLogDebugEndMsg();
    return bResult;
}


bool KDistributionModelDlg::runGravity(KDistributionInfo* pDInfo)
{
    TxLogDebugStartMsg();
	// todo : gravity 모형 구동 구현
    TxLogDebugEndMsg();
	return false;
}


bool KDistributionModelDlg::prepareZoneData(KDistributionInfo* pDInfo, KDistributionZoneDataCollection& zoneData)
{
    TxLogDebugStartMsg();
    KBulkDBase oBulkDbase;
    bool bResult = oBulkDbase.BulkZoneSelect(m_pZoneTable, pDInfo->ZoneProductionColumnName(), pDInfo->ZoneAttractionColumnName(), zoneData);
    TxLogDebugEndMsg();
    return bResult;
}


//deprecated function
bool KDistributionModelDlg::prepareZoneData(KDistributionInfo* pDInfo, KTDZoneArray& zoneData)
{
    TxLogDebugStartMsg();
	ASSERT(NULL != m_pZoneTable);

	KIOColumns reqColumns;
	KIOColumn* pColumn = NULL;
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(_T("zone_id"));
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(pDInfo->ZoneProductionColumnName());
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(pDInfo->ZoneAttractionColumnName());

	KIORecordset* pRecordset = m_pZoneTable->GetRecordset(KEMIORecordsetTypeStatic, &reqColumns, _T(""), _T(""));
	if(NULL == pRecordset)
	{
		return false;
	}

	int nRecordCount = pRecordset->GetRowCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		KIOItem* pItem = NULL;
		pItem = pRow->GetItem(0);
		__int64 nZoneID = pItem->GetValueAsInteger64();
		pItem = pRow->GetItem(1);
		double dP = pItem->GetValueAsDouble();
		pItem = pRow->GetItem(2);
		double dA = pItem->GetValueAsDouble();

		KDistributionZoneData* pZoneData = new KDistributionZoneData(nZoneID, dP, dA);
		zoneData.Add(pZoneData);
	}

	delete pRecordset;
    TxLogDebugEndMsg();
	return true;
}


void KDistributionModelDlg::releaseZoneData(KTDZoneArray& zoneData)
{
    TxLogDebugStartMsg();
	size_t nCount = zoneData.GetCount();
	for(size_t i = 0; i < nCount; ++i)
	{
		KDistributionZoneData* pData = zoneData.GetAt(i);
		delete pData;
	}
    TxLogDebugEndMsg();
}


bool KDistributionModelDlg::prepareBaseOD(KDistributionInfo* pDInfo, KTDBaseODMap& baseODMap)
{
    TxLogDebugStartMsg();
	KTarget* pBaseODTarget = pDInfo->BaseODTarget();
	if(NULL == pBaseODTarget)
	{
		return false;
	}

	KIOTables* pIOTables = pBaseODTarget->Tables();
	KIOTable* pBaseODTable = pIOTables->FindTable(_T("purpose_od"));
	if(NULL == pBaseODTable)
	{
		return false;
	}

	KIOColumns reqColumns;
	KIOColumn* pColumn = NULL;
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(_T("ozone_id"));
	pColumn = reqColumns.AddNewColumn();
	pColumn->Name(_T("dzone_id"));
	reqColumns.AddNewColumn(pDInfo->BaseODColumn());

	KIORecordset* pRecordset = pBaseODTable->GetRecordset(KEMIORecordsetTypeStatic,	&reqColumns, _T(""), _T(""));

	if(NULL == pRecordset)
	{
		return false;
	}

	int nRecordCount = pRecordset->GetRowCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		KIOItem* pItem;
		pItem = pRow->GetItem(0);
		__int64 nOID = pItem->GetValueAsInteger64();

		pItem = pRow->GetItem(1);
		__int64 nDID = pItem->GetValueAsInteger64();

		pItem = pRow->GetItem(2);
		double dOD = pItem->GetValueAsDouble();

		baseODMap.SetAt(KMultiID(nOID, nDID), dOD);
	}

	delete pRecordset;
    TxLogDebugEndMsg();
	return true;
}


bool KDistributionModelDlg::prepareBaseOD(KDistributionInfo* pDInfo, std::map<KODKey, double>& baseODMap)
{
    TxLogDebugStartMsg();

    KTarget* pBaseODTarget = pDInfo->BaseODTarget();
    if(NULL == pBaseODTarget)
    {
        return false;
    }

    KIOTables* pIOTables = pBaseODTarget->Tables();
    KIOTable* pBaseODTable = pIOTables->FindTable(_T("purpose_od"));
    if(NULL == pBaseODTable)
    {
        return false;
    }

    KBulkDBase oBulkDbase;
    bool bResult = oBulkDbase.BulkODSelect(pBaseODTable, pDInfo->BaseODColumn()->Name(), baseODMap);
    
    TxLogDebugEndMsg();
    return bResult;
}


bool KDistributionModelDlg::recordDistributionResult(KDistributionInfo* pDInfo, LPCTSTR strStartDate, LPCTSTR strEndDate)
{
    TxLogDebugStartMsg();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format(
		_T(" INSERT OR REPLACE INTO distribution_model ")
		_T(" (object_group_code, detail_object_id, purpose_od_column_name, start_date, end_date) ")
		_T(" VALUES ('%d', '%d', '%s', '%s', '%s') "),
		pDInfo->PurposeGroupID(), pDInfo->PurposeID(),
		pDInfo->OutputPurposeODColumnName(), strStartDate, strEndDate
		);

    TxLogDebug(strSQL.AllocSysString());

	try
	{
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
        TxLogDebugException();
		return false;
	}

    TxLogDebugEndMsg();
    return true;    
}


void KDistributionModelDlg::initParaODColumnNames()
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
		//if (CheckparaODColumn(pColumn))
		if (nullptr != pColumn &&  KEMIODataTypeDouble ==  pColumn->DataType())
		{
			KSTParaColumn data;
			data.ColumnName = pColumn->Name();
			data.ColumnCaption = pColumn->Caption();

			m_paraODColumns.push_back(data);
		}
         
    }
}


bool KDistributionModelDlg::CheckparaODColumn(KIOColumn* a_pColumn)
{
	if (a_pColumn->DataType() != KEMIODataTypeDouble)
	{
		return false;
	}
	
	CString strColumnName = a_pColumn->Name();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T("select * from parameter_od_info where column_name = '%s'"), strColumnName);
	KResultSetPtr pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
	while(pResultSet->Next())
	{
		int nPurposeCode = pResultSet->GetValueInt(1);
		int nModeCode = pResultSet->GetValueInt(2);
		int nTypecode = pResultSet->GetValueInt(3);

		if (nTypecode == ParameterType_Length)
		{
			return true;
		}
	}

	return false;
}



// KDistributionModelDlg 메시지 처리기입니다.
BOOL KDistributionModelDlg::OnInitDialog()
{
    TxLogDebugStartMsg();
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    initParaODColumnNames();

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(4, 5);

	m_uiBackImage.MoveWindow(&rcClient);
    m_uiBackImage.SetMarkupText(UIXAML_D_BACKIMAGE);

    m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB); 
    m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 
    m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
    m_uiArrow4.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRB, ZEMArrowPositionRT); 
    m_uiArrow6.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 

    UINT nID_DISTRIBUTION_INPUT  = 90913;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_INPUT, &nID_DISTRIBUTION_INPUT, 1, 0); 
    UINT nID_DISTRIBUTION_MODEL  = 90915;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL, &nID_DISTRIBUTION_MODEL, 1, 0); 
    UINT nID_DISTRIBUTION_OUTPUT = 90917;
    XTPImageManager()->SetIcons(IDB_DISTRIBUTION_OUTPUT, &nID_DISTRIBUTION_OUTPUT, 1, 0); 

    m_btnModel.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnModel.SetWindowText(KR_UIXAML_D_MODEL_DEFAULT);
	}
	else {
		m_btnModel.SetWindowText(UIXAML_D_MODEL_DEFAULT);
	}

    m_btnInput.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnInput.SetWindowText(KR_UIXAML_D_INPUT_FALSE);
	}
	else {
		m_btnInput.SetWindowText(UIXAML_D_INPUT_FALSE);
	}
	m_btnInput.EnableWindow(FALSE);

    m_btnBaseOD.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnBaseOD.SetWindowText(KR_UIXAML_D_BASEOD_FALSE);
	}
	else {
		m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_FALSE);
	}
	m_btnBaseOD.EnableWindow(FALSE);

    m_btnVariable.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnVariable.SetWindowText(KR_UIXAML_D_VARIABLE_FALSE);
	}
	else {
		m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_FALSE);
	}
	m_btnVariable.EnableWindow(FALSE);

    m_btnCriteria.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnCriteria.SetWindowText(KR_UIXAML_D_STOP_FALSE);
	}
	else {
		m_btnCriteria.SetWindowText(UIXAML_D_STOP_FALSE);
	}
	m_btnCriteria.EnableWindow(FALSE);

    m_btnOutputDesign.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnOutputDesign.SetWindowText(KR_UIXAML_D_OUTPUT_FALSE);
	}
	else {
		m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_FALSE);
	}
	m_btnOutputDesign.EnableWindow(FALSE);

	m_btnRun.EnableWindow(FALSE);
	m_btnViewTable.EnableWindow(FALSE);

//     m_hyperLink.SetURL(_T("http://www.ktdb.go.kr"));
//     m_hyperLink.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x80, 0x00, 0x80), RGB(0xFF, 0x00, 0x00));
//     m_hyperLink.SetUnderline(true);
//     m_hyperLink.EnableShellExecute(true);
 
    TxLogDebugEndMsg();
    SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 
    KRedrawWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDistributionModelDlg::OnBnClickedModel()
{
    TxLogDebugStartMsg();
	KDSelectModelDlg dlg;
	dlg.DistributionODType(m_emODType);

	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd;
	
	// Purpose info
	itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;
		dlg.AddDistributionInfo(KEMDODTypePurpose, pInfo);
	}

	if(dlg.DoModal() == IDOK)
	{
		m_emODType = dlg.DistributionODType();
		
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnModel.SetWindowText(KR_UIXAML_D_MODEL_SELECTED);
			m_btnInput.SetWindowText(KR_UIXAML_D_INPUT_DEFAULT);
		}
		else {
			m_btnModel.SetWindowText(UIXAML_D_MODEL_SELECTED);
			m_btnInput.SetWindowText(UIXAML_D_INPUT_DEFAULT);
		}
		m_btnInput.EnableWindow(TRUE);

		m_bUseFratar  = false;
		m_bUseGravity = false;

		for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
		{
			KDistributionInfo* pInfo = itInfo->second;
			if(pInfo->Selected() == false)
			{
				continue;
			}

			if(pInfo->ModelType() == KEMDistributionModelFratar)
			{
				m_bUseFratar = true;
			}
			else if(pInfo->ModelType() == KEMDistributionModelGravity)
			{
				m_bUseGravity = true;
			}
		}

		if ( false == m_bUseFratar )
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnBaseOD.SetWindowText(KR_UIXAML_D_BASEOD_FALSE);
			}
			else {
				m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_FALSE);
			}
			m_btnBaseOD.EnableWindow(FALSE);
		}
		if ( false == m_bUseGravity )
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnVariable.SetWindowText(KR_UIXAML_D_VARIABLE_FALSE);
			}
			else {
				m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_FALSE);
			}
			m_btnVariable.EnableWindow(FALSE);
		}
		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedInput()
{
    TxLogDebugStartMsg();
	KDInputDlg dlg;
	dlg.SetTarget(m_pTarget);

	std::map<int, KDistributionInfo*>::iterator itDInfo, itDInfoEnd = m_purposeInfoMap.end();
	for(itDInfo = m_purposeInfoMap.begin(); itDInfo != itDInfoEnd; ++itDInfo)
	{
		if(itDInfo->second->Selected() == true)
		{
			dlg.AddDistributionInfo(itDInfo->second);
		}
	}

	std::list<KPAPurpose*>::iterator itPurpose,itEnd = m_PAPurposeList.end();
	for(itPurpose = m_PAPurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		dlg.AddModelPurpose(*itPurpose);
	}

	if(dlg.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnInput.SetWindowText(KR_UIXAML_D_INPUT_SELECTED);
		}
		else {
			m_btnInput.SetWindowText(UIXAML_D_INPUT_SELECTED);
		}

		if (true == m_bUseFratar)
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnBaseOD.SetWindowText(KR_UIXAML_D_BASEOD_DEFAULT);
			}
			else {
				m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_DEFAULT);
			}
			m_btnBaseOD.EnableWindow(TRUE);
		}
		if( true == m_bUseGravity)
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnVariable.SetWindowText(KR_UIXAML_D_VARIABLE_DEFAULT);
			}
			else {
				m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_DEFAULT);
			}
			m_btnVariable.EnableWindow(TRUE);
		}
		RunButtonControl();
/*
		if (true == m_bUseFratar)
		{
			m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_DEFAULT);
			m_btnBaseOD.EnableWindow(TRUE);
		}
		else
		{
			if( true == m_bUseGravity)
			{
				m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_DEFAULT);
				m_btnVariable.EnableWindow(TRUE);
			}
		}*/
		
		/*std::map<int, KDistributionInfo*>::iterator itInfo, itEnd = m_purposeInfoMap.end();
		for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
		{
			KDistributionInfo* pInfo = itInfo->second;

			if((pInfo->Selected() == true) && 
				(pInfo->ModelType() == KEMDistributionModelFratar))
			{
				bUseFratar = true;
			}
		}
		if (bUseFratar == true)
		{
			m_btnInput.SetWindowText(UIXAML_D_INPUT_SELECTED);

			m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_DEFAULT);
			m_btnBaseOD.EnableWindow(TRUE);

			m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_FALSE);
			m_btnVariable.EnableWindow(FALSE);
		}
		else
		{
			m_btnInput.SetWindowText(UIXAML_D_INPUT_SELECTED);

			m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_FALSE);
			m_btnBaseOD.EnableWindow(FALSE);
			
			m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_DEFAULT);
			m_btnVariable.EnableWindow(TRUE);
		}*/
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedBaseod()
{
    TxLogDebugStartMsg();
	KDBaseODDlg dlg;
	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;

		if((pInfo->Selected() == true) && 
			(pInfo->ModelType() == KEMDistributionModelFratar))
		{
			dlg.AddDistributionInfo(pInfo);
		}
	}

	if(dlg.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnBaseOD.SetWindowText(KR_UIXAML_D_BASEOD_SELECTED);
			m_btnCriteria.SetWindowText(KR_UIXAML_D_STOP_DEFAULT);
		}
		else {
			m_btnBaseOD.SetWindowText(UIXAML_D_BASEOD_SELECTED);
			m_btnCriteria.SetWindowText(UIXAML_D_STOP_DEFAULT);
		}
		m_btnCriteria.EnableWindow(TRUE);    
		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedVariable()
{
    TxLogDebugStartMsg();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDImpedanceFunctionDlg dlg;
	dlg.SetTarget(m_pTarget);
	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;
		if((pInfo->Selected() == true) && 
			(pInfo->ModelType() == KEMDistributionModelGravity))
		{
			dlg.AddDistributionInfo(pInfo);
		}
	}

    dlg.AddParaODColumns(m_paraODColumns);
	dlg.SetDefaultparameter(m_mapDefaultParameter);

	if(dlg.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnVariable.SetWindowText(KR_UIXAML_D_VARIABLE_SELECTED);
			m_btnCriteria.SetWindowText(KR_UIXAML_D_STOP_DEFAULT);
		}
		else {
			m_btnVariable.SetWindowText(UIXAML_D_VARIABLE_SELECTED);
			m_btnCriteria.SetWindowText(UIXAML_D_STOP_DEFAULT);
		}
		m_btnCriteria.EnableWindow(TRUE);   
		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedCriteria()
{
    TxLogDebugStartMsg();
	KDSetStoppingDlg dlg;
	dlg.FratarIteration(m_nFratarIteration);
	dlg.FratarGap(m_dFratarGap);

	dlg.GravityIteration(m_nGravityIteration);
	dlg.GravityError(m_dGravityError);

	if(dlg.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnCriteria.SetWindowText(KR_UIXAML_D_STOP_SELECTED);
		}
		else {
			m_btnCriteria.SetWindowText(UIXAML_D_STOP_SELECTED);
		}
 		m_nFratarIteration = dlg.FratarIteration();
		m_dFratarGap = dlg.FratarGap();
		m_nGravityIteration = dlg.GravityIteration();
		m_dGravityError = dlg.GravityError();

		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnOutputDesign.SetWindowText(KR_UIXAML_D_OUTPUT_DEFAULT);
		}
		else {
			m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_DEFAULT);
		}
        m_btnOutputDesign.EnableWindow(TRUE);
		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedOutputdesign()
{
    TxLogDebugStartMsg();
	KDOutputDesignDlg dlg;
	dlg.SetTarget(m_pTarget);

	std::map<int, KDistributionInfo*>::iterator itInfo, itEnd;

	// Purpose info
	itEnd = m_purposeInfoMap.end();
	for(itInfo = m_purposeInfoMap.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = itInfo->second;
		if(pInfo->Selected() == true)
		{
			dlg.AddDistributionInfo(pInfo);
		}
	}

	if(dlg.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnOutputDesign.SetWindowText(KR_UIXAML_D_OUTPUT_SELECTED);
		}
		else {
			m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_SELECTED);
		}
		m_btnRun.EnableWindow(TRUE);
		RunButtonControl();
	}
    KRedrawWindow();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::RunButtonControl()
{
	UpdateData(TRUE);

	CString strButtonState = _T(""); 
	BOOL bButtonRun = TRUE;

	m_btnModel.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_D_MODEL_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_D_MODEL_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnInput.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_D_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_D_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	
	if( m_bUseFratar )
	{
		m_btnBaseOD.GetWindowText(strButtonState);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if( strButtonState.CompareNoCase(KR_UIXAML_D_BASEOD_SELECTED) != 0 )
			{
				bButtonRun = FALSE;
			}
		}
		else {
			if( strButtonState.CompareNoCase(UIXAML_D_BASEOD_SELECTED) != 0 )
			{
				bButtonRun = FALSE;
			}
		}
	}

	if( m_bUseGravity )
	{
		m_btnVariable.GetWindowText(strButtonState);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if( strButtonState.CompareNoCase(KR_UIXAML_D_VARIABLE_SELECTED) != 0 )
			{
				bButtonRun = FALSE;
			}
		}
		else {
			if( strButtonState.CompareNoCase(UIXAML_D_VARIABLE_SELECTED) != 0 )
			{
				bButtonRun = FALSE;
			}
		}
	}

	m_btnCriteria.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_D_STOP_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_D_STOP_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnOutputDesign.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_D_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_D_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(bButtonRun);
}


void KDistributionModelDlg::OnBnClickedRun()
{
 	TxLogDebugStartMsg();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

    QBicSimpleProgressThread::ExecuteThread(DistributiontModelThreadCaller, this, false, nLang);

	m_btnViewTable.EnableWindow(TRUE);
	KRedrawWindow();

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable* pODTable = pIOTables->FindTable(_T("purpose_od"));
    pODTable->Notify();

	TxLogDebugEndMsg();

}

// Distribution thread function
unsigned __stdcall KDistributionModelDlg::DistributiontModelThreadCaller(void* pParam)
{
    QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;
    
    QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("통행분포 모형 실행 중..."));
	 }
	 else {
		pProgressWindow->SetStatus(_T("Running Distribution Model..."));
	 }
    
    KDistributionModelDlg* pDlg = (KDistributionModelDlg*)pParameter->GetParameter();;

    //pDlg->runDistributionModel();
    pDlg->RunDistributionFortran();

    return 0;
}

void KDistributionModelDlg::RunDistributionFortran()
{
    TxLogDebugStartMsg();

    CTime timeStart = CTime::GetCurrentTime();

    if ( !CreateArgument())
    {
        return;
    }

	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("통행발생 모형 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Trip Distribution Model..."));
	}

	KLauncher::Run(strWindowName, 33);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("통행발생 모형 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Run Trip Distribution Model Complete."));
		}
		TxLogDebug(_T("Model Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("통행발생 모형 실행을 중단하였습니다."));
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

    // 데이터 결과 입력
    if (! WriteResult( timeStart ))
    {
        AfxMessageBox(_T("결과 저장에 실패하였습니다. "));
    }

    TxLogDebugEndMsg();
}

void KDistributionModelDlg::ReceiveModelMsg( CAtlString strMsg)
{
    if(NULL != g_pProgressDlg)
    {
        g_pProgressDlg->SetStatus(strMsg);
    }
}

void KDistributionModelDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
    QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
    if (spProgressWindow)
    {
        spProgressWindow->SetStatus(CString(strMsg));
    }
    
    //ReceiveModelMsg(CAtlString(strMsg));
}

bool KDistributionModelDlg::WriteResult(CTime& timeStart)
{
    TxLogDebugStartMsg();
    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    bool bResult = true;

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PURPOSE_OD);
    ASSERT(NULL != pODTable);
    const 
        KIOColumns* pColumns = pODTable->Columns();

    KIOColumns columnCollection;
    size_t nSelectedCount = 0;

    while ( iter != end )
    {
        KDistributionInfo* pDInfo = iter->second;
        if (pDInfo->Selected())
        {
            nSelectedCount ++;

            // column 존재 여부 확인 및 존재하지 않을 경우 컬럼 생성            
            KIOColumn* pOutputColumn = pColumns->GetColumn(pDInfo->OutputPurposeODColumnName());
            if(NULL == pOutputColumn)
            {
                KIOColumn column;
                column.Name(pODTable->GetUsableColumnName());
                column.Caption(pDInfo->OutputPurposeODColumnCaption());
                column.CodeType(KEMIOCodeTypeIsNothing);
                column.ColumnType(KEMIOColumnTypeUserDefine);
                column.DataType(KEMIODataTypeDouble);
                column.DisplayOrder(pColumns->ColumnCount()+1);
                column.PrimaryKey(false);
                column.Visible(true);
                pOutputColumn = pODTable->AddColumn(column);
                if(NULL == pOutputColumn)
                {      
                    TxLogDebugException();

                    bResult = false;
                    break;
                }

                pDInfo->OutputPurposeODColumnName(pOutputColumn->Name());
            }
            columnCollection.AddNewColumn(pOutputColumn);
        }

        ++iter;
    }

    KODKeyDoubleRecords resultRecords(nSelectedCount);
    CString strOut;
    strOut.Format(_T("Count : %d"), nSelectedCount);

    TxLogDebug(strOut.AllocSysString());

    try
    {
		ImChampDir::SetCurrentDirectoryToApp();

        std::ifstream ifs( _T("dout_od.dat"), std::ios::binary );  
        if ( ifs ) 
        {      
            resultRecords.ReadBytes(ifs);
        }
        else
        {
            TxLogDebugException();
            return false;
        }

        ifs.close();

        if ( ! KBulkDBase::BulkODUpsert(pODTable, columnCollection, resultRecords) )
        {
            TxLogDebugException();
            return false;
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    CTime timeEnd   = CTime::GetCurrentTime();
    for(iter = m_purposeInfoMap.begin(); iter != end; ++iter)
    {
        KDistributionInfo* pDInfo = iter->second;  
        if (pDInfo->Selected()) 
        {
            bResult = recordDistributionResult(pDInfo, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
        }
    }


    TxLogDebugEndMsg();
    return true;
}

bool KDistributionModelDlg::CreateArgument()
{
	ImChampDir::SetCurrentDirectoryToApp();

    CString strFileName = _T("distribution.arg");
    UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
    CStdioFileEx of(strFileName, nOpenMode);

    bool bResult = false;

    // Step 1. OD Type : 현재는 목적 OD 만... 정의
    bResult = ArgumentWriteODType(of);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteODType) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    // WriteModel
    bResult = ArgumentWriteModel(of);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteModel) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    KIOColumns zoneColumnCollection;
    bResult = ArgumentWriteInput(of, zoneColumnCollection);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteInput) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    KIOColumns baseODColumnCollection;
    bResult = ArgumentWriteBaseOD(of, baseODColumnCollection);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteBaseOD) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    // ArgumentWriteImpedance( CStdioFileEx& of, KIOColumnCollection& paraODColumnCollection )
    KIOColumns paraODColumnCollection;
    bResult = ArgumentWriteImpedance(of, paraODColumnCollection);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteImpedance) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    //ArgumentWriteStopping
    bResult = ArgumentWriteStopping(of);
    if (!bResult)
    {
        of.Close();
        AfxMessageBox(_T("Argument 생성(ArgumentWriteStopping) 중 오류가 발생하였습니다. !!!"));
        return false;
    }
    of.Close();

    bResult = ArgumentWriteFileZone(zoneColumnCollection);
    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(ArgumentWriteFileZone) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    //ArgumentWriteFileBaseOD
    bResult = ArgumentWriteFileBaseOD(baseODColumnCollection);
    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(ArgumentWriteFileBaseOD) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    bResult = ArgumentWriteFileParaOD(paraODColumnCollection);
    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(ArgumentWriteFileParaOD) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    return true;
}

// Step 1 : 목적, 수단 구분
bool KDistributionModelDlg::ArgumentWriteODType(CStdioFileEx& of)
{
    CString strOut;

    try
    {
        strOut.Format(_T("type\t%d\r\n"), 1);
        of.WriteString(strOut);
        strOut.Format(_T("%d\r\n"), 1); // 1: 목적, 2: 수단
        of.WriteString(strOut);
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

// Step 2. Write Model
bool KDistributionModelDlg::ArgumentWriteModel(CStdioFileEx& of)
{
    CStringArray strOutArray;
    CString      strOut;

    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    int nSelectedCount = 0;
    while ( iter != end )
    {
        KDistributionInfo* pDistributionInfo = iter->second;
        if (pDistributionInfo->Selected())
        {
            ++nSelectedCount;
            strOut.Format(_T("%d\t%d\r\n"), nSelectedCount, pDistributionInfo->ModelType());
            strOutArray.Add(strOut);
        }

        ++iter;
    }

    try
    {
        // 모델 - 줄수
        strOut.Format(_T("model\t%d\r\n"), nSelectedCount);
        of.WriteString(strOut);

        for (int i=0; i<nSelectedCount; i++)
        {
            of.WriteString(strOutArray.GetAt(i));
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

// Step 3. Input
bool KDistributionModelDlg::ArgumentWriteInput( CStdioFileEx& of, KIOColumns& zoneColumnCollection )
{
    zoneColumnCollection.Clear();

    CStringArray strOutArray;
    CString      strOut;

    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    int nSelectedCount = 0;
    while ( iter != end )
    {
        KDistributionInfo* pDistributionInfo = iter->second;
        if (pDistributionInfo->Selected())
        {
            int nPIndex = AddColumn(zoneColumnCollection, pDistributionInfo->ZoneProductionColumnName());
            int nAIndex = AddColumn(zoneColumnCollection, pDistributionInfo->ZoneAttractionColumnName());
            ++nSelectedCount;
            //strOut.Format(_T("%d\t%d\t%d\r\n"), nSelectedCount, nSelectedCount*2-1, nSelectedCount*2 );
            strOut.Format(_T("%d\t%d\t%d\r\n"), nSelectedCount, nPIndex+1, nAIndex+1 );
            strOutArray.Add(strOut);
        }

        ++iter;
    }

    try
    {
        // input - 줄수
        strOut.Format(_T("input\t%d\r\n"), nSelectedCount);
        of.WriteString(strOut);

        for (int i=0; i<nSelectedCount; i++)
        {
            of.WriteString(strOutArray.GetAt(i));
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

bool KDistributionModelDlg::ArgumentWriteFileZone(KIOColumns& zoneColumnCollection)
{
    int nCount = zoneColumnCollection.ColumnCount();
    if (nCount < 1)
        return false;

    std::ofstream ofs(_T("d_zone.dat"), std::ios::binary);

    try
    {
        KIDKeyDoubleRecords records(nCount);
        KBulkDBase::BulkZoneSelect(m_pZoneTable, zoneColumnCollection, records);

        records.WriteBytes(ofs);
        ofs.close();
    }
    catch (...)
    {
        ofs.close();
        TxLogDebugException();
    }    

    return true;
}

// Step 4
bool KDistributionModelDlg::ArgumentWriteBaseOD( CStdioFileEx& of, KIOColumns& baseODColumnCollection )
{
    baseODColumnCollection.Clear();

    CStringArray strOutArray;
    CString      strOut;

    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    //int nFratarCount   = 0;
    int nSelectedCount = 0;

    while ( iter != end )
    {
        KDistributionInfo* pDistributionInfo = iter->second;
        if (pDistributionInfo->Selected())
        {
            ++nSelectedCount;

            if ( pDistributionInfo->ModelType() == KEMDistributionModelFratar)
            {

                int nBaseODIndex = AddColumn(baseODColumnCollection, pDistributionInfo->BaseODColumn()->Name());
                // 1번 인덱스
                strOut.Format(_T("%d\t%d\r\n"), nSelectedCount, nBaseODIndex + 1 );
            }
            else
            {
                strOut.Format(_T("%d\t%d\r\n"), nSelectedCount, 0 );
            }

            strOutArray.Add(strOut);
        }

        ++iter;
    }

    try
    {
        // input - 줄수
        strOut.Format(_T("baseod\t%d\r\n"), nSelectedCount);
        of.WriteString(strOut);

        for (int i=0; i<nSelectedCount; i++)
        {
            of.WriteString(strOutArray.GetAt(i));
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

bool KDistributionModelDlg::ArgumentWriteFileBaseOD( KIOColumns& baseODColumnCollection )
{
    int nCount = baseODColumnCollection.ColumnCount();    

    try
    {
        std::ofstream ofs(_T("d_base_od.dat"), std::ios::binary);
        if (nCount > 0)
        {        
            KODKeyDoubleRecords records(nCount);
            KBulkDBase::BulkODSelect(m_pTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, baseODColumnCollection, records);

            records.WriteBytes(ofs);
        }
        ofs.close();
    }
    catch (...)
    {
        TxLogDebugException();
    }    

    return true;
}

// Step 5
bool KDistributionModelDlg::ArgumentWriteImpedance( CStdioFileEx& of, KIOColumns& paraODColumnCollection )
{
    paraODColumnCollection.Clear();

    CStringArray strOutArray;
    CString      strOut;

    std::map<int, KDistributionInfo*>::iterator iter = m_purposeInfoMap.begin();
    std::map<int, KDistributionInfo*>::iterator end  = m_purposeInfoMap.end();

    int nIndex = 1;
    while (iter != end)
    {
        KDistributionInfo* pInfo = iter->second;

        if (pInfo->Selected())
        {
            if (pInfo->ModelType() == KEMDistributionModelFratar)
            {
                strOut.Format(_T("%d\t%d\t0\t0.0\t0.0\t0.0\r\n"), nIndex, 0);
                strOutArray.Add(strOut);
            }
            else
            {
                KDistributionFunctionBase* pBase = pInfo->ImpedanceFunction();
                if ( pBase->FunctionType() == KEMDistributionFunction1)
                {
                    KDistributionFunction1* pFunction1 = (KDistributionFunction1*)pBase;
                    int nColumnIndex = AddColumn(paraODColumnCollection, pFunction1->RijColumnName());
                    double A = 0.0;
                    double B = pFunction1->Parameter2();
                    double C = 0.0;

                    strOut.Format(_T("%d\t%d\t%d\t%f\t%f\t%f\r\n"), nIndex, pBase->FunctionType(), nColumnIndex + 1, A, B, C);
                    strOutArray.Add(strOut);
                }
                if ( pBase->FunctionType() == KEMDistributionFunction2)
                {
                    KDistributionFunction2* pFunction2 = (KDistributionFunction2*)pBase;
                    int nColumnIndex = AddColumn(paraODColumnCollection, pFunction2->RijColumnName());
                    double A = pFunction2->Parameter1();
                    double B = pFunction2->Parameter2();
                    double C = 0.0;

                    strOut.Format(_T("%d\t%d\t%d\t%f\t%f\t%f\r\n"), nIndex, pBase->FunctionType(), nColumnIndex + 1, A, B, C);
                    strOutArray.Add(strOut);
                }
                if ( pBase->FunctionType() == KEMDistributionFunction3)
                {
                    KDistributionFunction3* pFunction3 = (KDistributionFunction3*)pBase;
                    int nColumnIndex = AddColumn(paraODColumnCollection, pFunction3->RijColumnName());
                    double A = pFunction3->Parameter1();
                    double B = pFunction3->Parameter2();
                    double C = pFunction3->Parameter3();

                    strOut.Format(_T("%d\t%d\t%d\t%f\t%f\t%f\r\n"), nIndex, pBase->FunctionType(), nColumnIndex + 1, A, B, C);
                    strOutArray.Add(strOut);
                }
            }
            nIndex++;
        }

        ++iter;
    }

    try
    {
        // input - 줄수
        strOut.Format(_T("impedance\t%d\r\n"), strOutArray.GetCount());
        of.WriteString(strOut);

        for (int i=0; i<strOutArray.GetCount(); i++)
        {
            of.WriteString(strOutArray.GetAt(i));
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }
    return true;
}

bool KDistributionModelDlg::ArgumentWriteFileParaOD( KIOColumns& paraODColumnCollection )
{
    int nCount = paraODColumnCollection.ColumnCount();

    try
    {
        std::ofstream ofs(_T("d_para_od.dat"), std::ios::binary);
        if (nCount > 0)
        {
            KODKeyDoubleRecords records(nCount);
            KBulkDBase::BulkODSelect(m_pTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, paraODColumnCollection, records);

            records.WriteBytes(ofs);
        }

        ofs.close();
    }
    catch (...)
    {
        TxLogDebugException();
    }    

    return true;
}

bool KDistributionModelDlg::ArgumentWriteStopping( CStdioFileEx& of )
{
    CString strOut;
    try
    {
        strOut.Format(_T("stopping\t%d\r\n"), 1);
        of.WriteString(strOut);

        strOut.Format(_T("%d\t%f\t%d\t%f"), m_nFratarIteration, m_dFratarGap, m_nGravityIteration, m_dGravityError);
        of.WriteString(strOut);
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }
    return true;
}

int KDistributionModelDlg::AddColumn( KIOColumns& columnCollection, CString strColumn )
{
    int nColumnIndex = columnCollection.GetColumnIndex(strColumn);
    if (nColumnIndex < 0)
    {
        KIOColumn* pColumn = NULL;    
        pColumn = columnCollection.AddNewColumn();    
        pColumn->Name(strColumn);
    }

    nColumnIndex = columnCollection.GetColumnIndex(strColumn);
    return nColumnIndex;
}


void KDistributionModelDlg::OnBnClickedClose()
{
    TxLogDebugStartMsg();
	OnCancel();
    TxLogDebugEndMsg();
}


void KDistributionModelDlg::OnBnClickedOutputtable()
{
    TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pODTable = pIOTables->FindTable(_T("purpose_od"));
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


void KDistributionModelDlg::KRedrawWindow()
{
    RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


void KDistributionModelDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    KDialogEx::OnActivate(nState, pWndOther, bMinimized);
    KRedrawWindow();
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void KDistributionModelDlg::InitDistributionParaInfo( void )
{
	m_mapDefaultParameter.clear();

	try
	{
		std::map<int, std::map<int, KDistributionParaInfo>> mapGetParaInfo;
		KDBaseDefaultDistribution::GetDefaultDistributionParameter(mapGetParaInfo);

		KPurposeInfoCollection oPurposeCollection(m_pTarget);

		AutoType iter = oPurposeCollection.begin();
		AutoType end  = oPurposeCollection.end();
		while(iter != end)
		{
			KPurpose* pPurpose = *iter;

			AutoType ofind = mapGetParaInfo.find(pPurpose->PurposeID());
			AutoType oend  = mapGetParaInfo.end();
			if (ofind != oend)
			{
				std::map<int, KDistributionParaInfo> mapParaInfo = ofind->second;

				AutoType msfind = mapParaInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
				AutoType mufind = mapParaInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
				AutoType mend   = mapParaInfo.end();

				if (mufind != mend)
				{
					KDistributionParaInfo oPara = mufind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapDefaultParameter.insert(std::make_pair(pPurpose->PurposeID(), oPara));

				}
				else if(msfind != mend)
				{
					KDistributionParaInfo oPara = msfind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapDefaultParameter.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
				else
				{
					KDistributionParaInfo oPara;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					oPara.TnPurposeID	  = pPurpose->PurposeID();
					oPara.TnFunctionType  = KDistrbutionFunction::Function1_Code;
					oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
					m_mapDefaultParameter.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
			}
			else
			{
				KDistributionParaInfo oPara;
				oPara.TstrPurposeName = pPurpose->PurposeName();
				oPara.TnPurposeID	  = pPurpose->PurposeID();
				oPara.TnFunctionType  = KDistrbutionFunction::Function1_Code;
				oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
				m_mapDefaultParameter.insert(std::make_pair(pPurpose->PurposeID(), oPara));
			}
			++iter;
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
}

void KDistributionModelDlg::InitDefultFunction( KDistributionInfo* a_pDInfo )
{
	try
	{
		AutoType find = m_mapDefaultParameter.find(a_pDInfo->PurposeID());
		AutoType end  = m_mapDefaultParameter.end();
		if (find != end)
		{
			KDistributionParaInfo oParaInfo = find->second;

			KIOColumn* pUseColumn = nullptr;
			KIOTable* pODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
			const KIOColumns* pColumns = pODTable->Columns();
			int nColumnCount = pColumns->ColumnCount();
			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pODColumn = pColumns->GetColumn(i);
				if (nullptr != pODColumn &&  KEMIODataTypeDouble ==  pODColumn->DataType())
				{
					CString strCaption = pODColumn->Caption();
					if (oParaInfo.TstrVariable.CompareNoCase(strCaption) == 0)
					{
						pUseColumn = pODColumn;
					}
				}
			}

			if (oParaInfo.TnFunctionType == KDistrbutionFunction::Function1_Code)
			{
				a_pDInfo->SetImpedanceFunctionType(KEMDistributionFunction1);
				KDistributionFunction1* pFunction = (KDistributionFunction1*)a_pDInfo->ImpedanceFunction();
				pFunction->Parameter2(oParaInfo.TdParameterBeta);
				if (pUseColumn != nullptr)
				{
					pFunction->RijColumnName(pUseColumn->Name());
				}
			}
			else if(oParaInfo.TnFunctionType == KDistrbutionFunction::Function2_Code)
			{
				a_pDInfo->SetImpedanceFunctionType(KEMDistributionFunction2);
				KDistributionFunction2* pFunction = (KDistributionFunction2*)a_pDInfo->ImpedanceFunction();
				pFunction->Parameter1(oParaInfo.TdParameterAlpa);
				pFunction->Parameter2(oParaInfo.TdParameterBeta);
				if (pUseColumn != nullptr)
				{
					pFunction->RijColumnName(pUseColumn->Name());
				}
			}
			else if (oParaInfo.TnFunctionType == KDistrbutionFunction::Function3_Code)
			{
				a_pDInfo->SetImpedanceFunctionType(KEMDistributionFunction3);
				KDistributionFunction3* pFunction = (KDistributionFunction3*)a_pDInfo->ImpedanceFunction();
				pFunction->Parameter1(oParaInfo.TdParameterAlpa);
				pFunction->Parameter2(oParaInfo.TdParameterBeta);
				pFunction->Parameter3(oParaInfo.TdParameterTheta);
				if (pUseColumn != nullptr)
				{
					pFunction->RijColumnName(pUseColumn->Name());
				}
			}
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
}


BOOL KDistributionModelDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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


void KDistributionModelDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}
