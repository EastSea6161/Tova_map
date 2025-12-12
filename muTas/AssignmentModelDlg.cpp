/**
 * @file AssignmentModelDlg.cpp
 * @brief KAssignmentModelDlg 구현 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "kmz_common.h"

#include "KmzApp.h"
#include "AssignmentModelDlg.h"
#include "ATripMatrix.h"
#include "AInputInfo.h"
#include "ARunOptionDlg.h"
#include "FARunOptionDlg.h"
#include "ABPRFnDlg.h"
#include "AODDemandShareDlg.h"
#include "AStoppingCriteriaDlg.h"
#include "AStoppingCriteriaMMDlg.h"
#include "AStoppingOriginBaseUeDlg.h"
#include "AProgressDlg.h"
#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOTableController.h"
#include "IOView.h"
#include "Mode.h"
#include "ModeManager.h"
#include "Target.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "Project.h"
#include "Scenario.h"
#include "KxBulkDbase.h"
#include "XmlManager.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"

#include "KASetTransitAssignOptionsDlg.h"
#include "KUIDefAssignment.h"
#include "InspectionNetwork.h"
#include "Launcher.h"
#include "ImChampDir.h"
#include "WriteCodeAgrument.h"
#include "DefineNamesDefaultPara.h"
#include "AImportDBDlg.h"
#include "DBaseAssignImport.h"
#include "..\QBicCommon\QBicWindowSystem.h"

#include "MultiAssignModelDlg.h"
#include "MultiAssignModelSOBADlg.h"

using namespace UIAssignment;
#if WINVER >= 0x0500
#include <uxtheme.h>
#pragma comment (lib, "UxTheme.lib")
#endif

// Assignment dll
const char*  ASSIGNMENT_PROC    = "FASSIGNMENT";
const TCHAR* ASSIGNMENT_LINKOUT = _T("AssignResult.txt");
const TCHAR* LINK_TRAVELTIME    = _T("travel_time");
const TCHAR* LINK_AUTO          = _T("auto");
const TCHAR* LINK_TAXI          = _T("taxi");

// KAssignmentModelDlg 대화 상자입니다.
CString KAssignmentModelDlg::m_strTargetLocation = _T("");
//KAProgressDlg* KAssignmentModelDlg::m_pProgressDlg = NULL;


IMPLEMENT_DYNAMIC(KAssignmentModelDlg, KDialogEx)

KAssignmentModelDlg::KAssignmentModelDlg(KTarget* a_pTarget,CWnd* pParent /*=NULL*/)
	:	KDialogEx(KAssignmentModelDlg::IDD, pParent),
		m_emAModel(KEMAssignmentModelUserEquilibriumOB),
		m_emODType(KEMAssignmentODDay),
		m_pTarget(a_pTarget),
// 		m_bInput(false),
// 		m_bRunOptions(false),
// 		m_bDefineVDF(false),
// 		m_bTransitAssignOptions(false),
// 		m_bTransitDataExist(false),
// 		m_bODDemandShare(false),
//		m_bStoppingCriteria(false),
		m_nBtnRadioIndex(0),
		m_bUseOverWrite(false),
		m_nModeInVDF(2),
		m_OBVDFDlg(m_pTarget),
		m_bUsedVDFDefault(true),
		m_bUsedInputDefault(true)
{
	m_nIIteration					= 1;
    m_dAnalysisTime					= 10.0;
	m_dValueOfTime					= 0.0;
    m_nHighwayIterationNo			= 100;
    m_dHighwayRelativeGap			= 0.001;
    m_nHighwayRelativeGapExp		= 12;
    m_nTransitIterationNo			= 100;
    m_dTransitNormalizeGap			= 0.1;
    m_dUEOBaseScaleFactor			= 0.0;
	m_nUEOBaseZoneCentroidThrought  = 0;
	m_nScaleFactorCursor			= 0;
}

KAssignmentModelDlg::~KAssignmentModelDlg()
{
	TxLogDebug(_T("다이얼로그 Inite 삭제"));
    ClearInputInfoList();
	TxLogDebug(_T("다이얼로그 종료"));
}


void KAssignmentModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MODEL, m_lblModel);
	DDX_Control(pDX, IDC_INPUT, m_btnInput);
	DDX_Control(pDX, IDC_RUNOPTION, m_btnRunOptions);
	DDX_Control(pDX, IDC_DEFINEVDF, m_btnDefineVDF);
	DDX_Control(pDX, IDC_TRANSITASSIGN, m_btnTransitAssignOptions);
	DDX_Control(pDX, IDC_ODDEMAND, m_btnODDemandShare);
	DDX_Control(pDX, IDC_STOPPINGCRITERIA, m_btnStoppingCriteria);
	DDX_Control(pDX, IDC_OUTPUTDESIGN, m_btnOutputDesign);
	DDX_Control(pDX, IDC_OUTPUTTABLE, m_btnViewTable);
	DDX_Control(pDX, IDC_VIEWLOG,     m_btnViewLog);
	DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow4);
	DDX_Control(pDX, IDC_STATIC_ARROW5, m_uiArrow5);
	DDX_Control(pDX, IDC_STATIC_ARROW6, m_uiArrow6);
	DDX_Control(pDX, IDC_STATIC_ARROW7, m_uiArrow7);
	DDX_Radio(pDX, IDC_RADIO1, m_nBtnRadioIndex);
}

BEGIN_MESSAGE_MAP(KAssignmentModelDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO5, &KAssignmentModelDlg::OnRadioClicked)
	ON_BN_CLICKED(IDC_RUNOPTION, &KAssignmentModelDlg::OnBnClickedRunOptions)
	ON_BN_CLICKED(IDC_INPUT, &KAssignmentModelDlg::OnBnClickedTripMatrix)
	ON_BN_CLICKED(IDC_DEFINEVDF, &KAssignmentModelDlg::OnBnClickedDefineVDF)
	ON_BN_CLICKED(IDC_TRANSITASSIGN, &KAssignmentModelDlg::OnBnClickedTransitassign)
	ON_BN_CLICKED(IDC_ODDEMAND, &KAssignmentModelDlg::OnBnClickedODDemand)
	ON_BN_CLICKED(IDC_STOPPINGCRITERIA, &KAssignmentModelDlg::OnBnClickedStoppingCriteria)
	ON_BN_CLICKED(IDC_OUTPUTDESIGN, &KAssignmentModelDlg::OnBnClickedOutputdesign)
	ON_BN_CLICKED(IDC_RUN, &KAssignmentModelDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_CLOSE, &KAssignmentModelDlg::OnBnClickedClose)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OUTPUTTABLE, &KAssignmentModelDlg::OnBnClickedOutputtable)
	ON_BN_CLICKED(IDC_VIEWLOG, &KAssignmentModelDlg::OnBnClickedViewLog)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_IMPORT, &KAssignmentModelDlg::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_MULTI,  &KAssignmentModelDlg::OnBnClickedBtnMulti)
END_MESSAGE_MAP()

void KAssignmentModelDlg::Target(KTarget* pTarget)
{
	m_pTarget = pTarget;
}

void KAssignmentModelDlg::ClearInputInfoList(void)
{
	std::list<KAInputInfo*>::iterator itInput,itEnd = m_InputList.end();
	for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput = *itInput;
		QBicDelete(pInput);
	}
	m_InputList.clear();
}

void KAssignmentModelDlg::updateOutputColumnLink(KIOTable* pTableLink)
{
	const KIOColumns* pColumns = pTableLink->Columns();
	KIOColumn* pColumn = NULL;

	pColumn = pColumns->GetColumn(LINK_TRAVELTIME);
	if(NULL == pColumn)
	{
		KIOColumn columnTT;
		columnTT.Name(LINK_TRAVELTIME);
		columnTT.Caption(_T("통행시간"));
		columnTT.DataType(KEMIODataTypeDouble);
		columnTT.ColumnType(KEMIOColumnTypeUserDefine);
		columnTT.DisplayOrder(pColumns->ColumnCount() + 1);
		pTableLink->AddColumn(columnTT);
	}

	pColumn = pColumns->GetColumn(LINK_AUTO);
	if(NULL == pColumn)
	{
		KIOColumn columnA;
		columnA.Name(LINK_AUTO);
		columnA.Caption(_T("승용차교통량"));
		columnA.DataType(KEMIODataTypeDouble);
		columnA.ColumnType(KEMIOColumnTypeUserDefine);
		columnA.DisplayOrder(pColumns->ColumnCount() + 1);
		pTableLink->AddColumn(columnA);
	}

	pColumn = pColumns->GetColumn(LINK_TAXI);
	if(NULL == pColumn)
	{
		KIOColumn columnT;
		columnT.Name(LINK_TAXI);
		columnT.Caption(_T("택시교통량"));
		columnT.DataType(KEMIODataTypeDouble);
		columnT.ColumnType(KEMIOColumnTypeUserDefine);
		columnT.DisplayOrder(pColumns->ColumnCount() + 1);
		pTableLink->AddColumn(columnT);
	}
}


void KAssignmentModelDlg::parseUpdateInfo(LPCTSTR strLine, std::vector<STAUpdateInfo>& updateInfoArray)
{
	std::vector<CString> strArray;
	int nSub = 0;
	CString strElement;

	while(AfxExtractSubString(strElement, strLine, nSub++, _T(' ')))
	{
		if(strElement.IsEmpty() == TRUE)
		{
			continue;
		}
		strArray.push_back(strElement);
	}

	if(strArray.size() < 5)
	{
		return;
	}

	STAUpdateInfo info;
	info.nOID = _ttoi64(strArray[0]);
	info.nDID = _ttoi64(strArray[1]);
	info.dTT  = _ttof(strArray[2]);
	info.dA   = _ttof(strArray[3]);
	info.dT   = _ttof(strArray[4]);

	updateInfoArray.push_back(info);
}

bool KAssignmentModelDlg::updateOD(KIOTable* pTableOD)
{
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("O/D 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving O/D Data..."));
	}
	return true;
}


void KAssignmentModelDlg::GetInputModeColumns(KIOColumns& columnCollection)
{
    columnCollection.Clear();

    std::list<KAInputInfo*>::iterator iter = m_InputList.begin();
    std::list<KAInputInfo*>::iterator end  = m_InputList.end();

    while ( iter != end )
    {
        KAInputInfo* pInputInfo = *iter;
        if ( pInputInfo->Selected() )
        {
            KIOColumn* pColumn = pInputInfo->InputColumn();
            if (pColumn)
            {
                int nColumnIndex = columnCollection.AddNewColumn(pColumn);
            }                
        }
        ++iter;
    }
}


void KAssignmentModelDlg::AddAssignResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rParaODColumnCollection
                                                          , TAssignResultColumnName& resultColumnName, CString a_strColumnInfo)
{

	TxLogDebugStartMsg();
	CString strErrMsg = _T("> Adding Data Column Failed.");

    rLinkColumnCollection.Clear();
    rParaODColumnCollection.Clear();

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
    KIOTable*        pParaODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);

    CString strColumnName, strRunSeq, strDescription;
    strRunSeq.Format(_T("(%d)"), runSeq);

	//★ Link Talbe 
	//Average_time
	strColumnName.Format (_T("AvgTime%s"),  strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("통행시간(분)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("AvgTime_%s"), a_strColumnInfo);
	}
	KIOColumn* pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.AverageTime = pColumn->Name();

	//VC
	strColumnName.Format (_T("V/C%s"),strRunSeq);
	strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.VC = pColumn->Name();

	//SumVolume
	strColumnName.Format (_T("TotVol%s"),  strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("총교통량(PCU)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("TotVol_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.SumVolume = pColumn->Name();

	//Trip
	strColumnName.Format (_T("Trip%s"),  strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("통행량(인)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Trip_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.Trip = pColumn->Name();

	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);

	//Modevolume
	std::vector<CString> vecModeVolumne;
	for (size_t i=0; i<vecModeName.size(); i++)
	{
		CString strColumnCaption = vecModeName[i];
		strColumnName.Format (_T("%s%s"),strColumnCaption, strRunSeq );
		if (KmzSystem::GetLanguage()==KEMKorea) {
			strDescription.Format(_T("%s 교통량(PCU)_%s"),strColumnCaption, a_strColumnInfo);
		}
		else {
			strDescription.Format(_T("%s_%s"),strColumnCaption, a_strColumnInfo);
		}
		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecModeVolumne.push_back(pColumn->Name());
	}
	resultColumnName.ModeVolumn = vecModeVolumne;

	//OD Table
	//ODAverage
	strColumnName.Format (_T("AvgTime%s"),strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("통행시간(분)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("AvgTime_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();

	TxLogDebugEndMsg();
}

//create by ptj(2012.10.11)
void KAssignmentModelDlg::AddAssignOBSingeResultColumn( int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rParaODColumnCollection, TAssignResultColumnName& resultColumnName , CString a_strColumnInfo)
{
	TxLogDebugStartMsg();
	CString strErrMsg = _T("> Adding Data Column Failed.");

	rLinkColumnCollection.Clear();
	rParaODColumnCollection.Clear();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pLinkTable   = pIOTables->FindTable(TABLE_LINK);
	KIOTable*  pParaODTable = pIOTables->FindTable(TABLE_PARAMETER_OD);

	CString strColumnName, strRunSeq, strDescription;
	strRunSeq.Format(_T("(%d)"), runSeq);

    KIOColumn* pColumn = nullptr;
    //★★ Link Table ★★    
    //★ Average Time
	strColumnName.Format(_T("AvgTime%s"), strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용제외)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.AverageTime = pColumn->Name();

    //★ Average Time Toll
    strColumnName.Format (_T("AvgTime+%s"),  strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용포함)_%s"), a_strColumnInfo);
	}
	else {
	    strDescription.Format(_T("Average_Time+_%s"), a_strColumnInfo);
	}
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.AvgTimeToll = pColumn->Name();

    //★ V/C
	strColumnName.Format( _T("V/C%s"),  strRunSeq);
	strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.VC = pColumn->Name();

    //★ Total Volume
    strColumnName.Format( _T("TotVol%s"), strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("총교통량(PCU)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Total_Volume_%s"), a_strColumnInfo);
	}
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.SumVolume = pColumn->Name();

    //★ 수단별 Volume
	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);

	std::vector<CString> vecModeVolumne;
	for (size_t i=0; i<vecModeName.size(); i++)
	{
		CString strColumnCaption = vecModeName[i];
		strColumnName.Format( _T("%s%s"),strColumnCaption,  strRunSeq );
		if (KmzSystem::GetLanguage()==KEMKorea) {
			strDescription.Format(_T("%s 교통량(PCU)_%s"),strColumnCaption, a_strColumnInfo);
		}
		else {
			strDescription.Format(_T("%s_%s"),strColumnCaption, a_strColumnInfo);
		}
		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecModeVolumne.push_back(pColumn->Name());
	}
	resultColumnName.ModeVolumn = vecModeVolumne;
    
    //★ Bus Initial Volume
	if (true == m_RunningOption.IncludeBusInitialVolume())
	{
		strColumnName.Format( _T("BusInitVol%s"),  strRunSeq);
		strDescription.Format(_T("Bus_InitVolumn_%s"), a_strColumnInfo);
		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultColumnName.BusInitialVolumn = pColumn->Name();
	}

    //★ OD Table ★
	strColumnName.Format (_T("AvgTime%s"),  strRunSeq);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용제외)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();
	TxLogDebugEndMsg();
}

void KAssignmentModelDlg::AddAssignOBMultiResultColumn( int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rParaODColumnCollection, TAssignResultColumnName& resultColumnName , CString a_strColumnInfo)
{
	TxLogDebugStartMsg();
	CString strErrMsg = _T("> Adding Data Column Failed.");

	rLinkColumnCollection.Clear();
	rParaODColumnCollection.Clear();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
	KIOTable*        pParaODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);

	CString strColumnName, strRunSeq, strDescription;
	strRunSeq.Format(_T("(%d)"), runSeq);

    KIOColumn* pColumn = nullptr;
	//★★ Link Table ★★
    //★ Average Time
	strColumnName.Format(_T("AvgTime%s"), strRunSeq);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용제외)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.AverageTime = pColumn->Name();

    //★ Avg Teim(w Toll)
    strColumnName.Format(_T("AvgTime+%s"), strRunSeq);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용포함)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Average_Time_%s"),a_strColumnInfo);
	}
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr)
    {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.AvgTimeToll = pColumn->Name();
    
    //★ VDF Time(1~n)
	std::vector<CString> vecVDFTime;
	for (int i = 0; i < m_nModeInVDF; i ++)
	{
		CString strColumnCaption = _T("");
		int nIndex = i + 1; 
		strColumnCaption.Format(_T("%s_%d%s%s"), _T("VDF"), nIndex,_T("Time"), strRunSeq);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strDescription.Format(_T("VDF%d 통행시간(분)(비용제외)_%s"),nIndex,a_strColumnInfo);
		}
		else {
			strDescription.Format(_T("%s_%d%s_%s"),_T("VDF"), nIndex,_T("Time") ,a_strColumnInfo);
		}
		pColumn = CreateResultColumn(strColumnCaption, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecVDFTime.push_back(pColumn->Name());
	}
	resultColumnName.VDFTime = vecVDFTime;

    //★ VDF Toll Time(1~n)
    std::vector<CString> vecVDFTollTime;
    for (int i = 0; i < m_nModeInVDF; i ++)
    {
        CString strColumnCaption = _T("");
        int nIndex = i + 1; 
        strColumnCaption.Format(_T("VDF%dTime+%s"), nIndex, strRunSeq);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strDescription.Format(_T("VDF%d 통행시간(분)(비용포함)_%s"),nIndex,a_strColumnInfo);
		}
		else {
			strDescription.Format(_T("VDF%d_%s"),nIndex,a_strColumnInfo);
		}
        pColumn = CreateResultColumn(strColumnCaption, pLinkTable, strDescription);
        if (pColumn == nullptr)
        {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        vecVDFTollTime.push_back(pColumn->Name());
    }
    resultColumnName.VDFTollTime = vecVDFTollTime;

    //★ V/C
    strColumnName.Format(_T("V/C%s"), strRunSeq);
    strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.VC = pColumn->Name();

    //★ Total Volume
    strColumnName.Format(_T("TotVol%s"), strRunSeq);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strDescription.Format(_T("총교통량(PCU)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Total_Volume_%s"), a_strColumnInfo);
	}
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.SumVolume = pColumn->Name();

    //★ 수단별 Volume
    std::vector<CString> vecModeName;
    GetUseModeName(vecModeName);
    std::vector<CString> vecModeVolumne;
    for (size_t i=0; i<vecModeName.size(); i++)
    {
        CString strColumnCaption =  vecModeName[i];
        strColumnName.Format(_T("%s%s"),strColumnCaption, strRunSeq );
		if (KmzSystem::GetLanguage()==KEMKorea) {
			strDescription.Format(_T("%s 교통량(PCU)_%s"),strColumnCaption, a_strColumnInfo);
		}
		else {
			strDescription.Format(_T("%s_%s"),strColumnCaption ,a_strColumnInfo);
		}
        pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription); 
        if (pColumn == nullptr) {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        vecModeVolumne.push_back(pColumn->Name());
    }
    resultColumnName.ModeVolumn = vecModeVolumne;

    //★ Bus Initial Volume
    if (true == m_RunningOption.IncludeBusInitialVolume()) {
        strColumnName.Format( _T("BusInitVol%s"),  strRunSeq);
        strDescription.Format(_T("Bus_InitVolumn_%s"), a_strColumnInfo);
        pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
        if (pColumn == nullptr) {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        resultColumnName.BusInitialVolumn = pColumn->Name();
    }

	//★★ OD Table ★★
	strColumnName.Format (_T("AvgTime%s"),strRunSeq);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strDescription.Format(_T("평균통행시간(분)(비용제외)_%s"), a_strColumnInfo);
	}
	else {
		strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	}
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();
    
	std::vector<CString> vecODVDFTime;
	for (int i = 0; i < m_nModeInVDF; i ++)
	{
		CString strColumnCaption = _T("");
		int nIndex = i + 1; 
		strColumnCaption.Format(_T("VDF%dTime%s"),  nIndex, strRunSeq);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strDescription.Format(_T("VDF%d 통행시간(분)(비용제외)_%s"),nIndex,a_strColumnInfo);
		}
		else {
			strDescription.Format(  _T("VDF%dTime_%s"), nIndex, a_strColumnInfo);
		}
		pColumn =  CreateResultColumn(strColumnCaption, pParaODTable, strDescription);
		if (pColumn == nullptr) {
			ThrowException(strErrMsg);
		}
		rParaODColumnCollection.AddNewColumn(pColumn);
		vecODVDFTime.push_back(pColumn->Name());
	}

	resultColumnName.ODVDFTime = vecODVDFTime;
	TxLogDebugEndMsg();
}

// Created by nombara(2012.09.16)
void KAssignmentModelDlg::DisplayOffExceptResult( KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection )
{
	return;
// 	KIOTables* pIOTables = m_pTarget->Tables();
// 	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
// 	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);
// 
// 	const 
// 		KIOColumns* pLinkIOColumnCollection = pLinkTable->Columns();
// 
// 	int nColumnCount = pLinkIOColumnCollection->ColumnCount();
// 
// 	for (int i=0; i<nColumnCount; i++)
// 	{
// 		KIOColumn* pColumn = pLinkIOColumnCollection->GetColumn(i);
// 		// Freeze 컬럼은 항상 Display On
// 		if (pColumn->FreezeColumn() != true)
// 		{
// 			// Display Off 항목 중 현재 생성된 항목은 제외
// 			if ( rLinkColumnCollection.GetColumnIndex(pColumn) < 0 )
// 			{
//                 KIOColumn oUpdateColumn = *pColumn;
// 				oUpdateColumn.Visible(false);
// 				pLinkTable->UpdateColumn(oUpdateColumn);
// 			}
// 		}
// 	}
// 
// 	const KIOColumns* pODIOColumnCollection = pODTable->Columns();
// 
// 	nColumnCount = pODIOColumnCollection->ColumnCount();
// 
// 	for (int i=0; i<nColumnCount; i++)
// 	{
// 		KIOColumn* pColumn = pODIOColumnCollection->GetColumn(i);
// 		// Freeze 컬럼은 항상 Display On
// 		if (pColumn->FreezeColumn() != true)
// 		{
// 			// Display Off 항목 중 현재 생성된 항목은 제외
// 			if ( rODColumnCollection.GetColumnIndex(pColumn) < 0 )
// 			{
//                 KIOColumn oUpdateColumn = *pColumn;
//                 oUpdateColumn.Visible(false);
// 				pODTable->UpdateColumn(oUpdateColumn);
// 			}
// 		}
// 	}
}

void KAssignmentModelDlg::UpdateLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection )
{
    TxLogDebugStartMsg();

    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);

    int nColumnCount = rLinkColumnCollection.ColumnCount();
    
	if (nColumnCount == 0 )
        ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs(strLocation, std::ios::binary );    
    try
    {
		if (!ifs)
		{
			ThrowException(_T("File Not Found"));
		}

		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs.is_open())
		{
			throw -1;
		}

        if ( ifs )
		{
			records.ReadBytes(ifs);
		}
		else
		{
			ThrowException(_T("error file read"));
		}
		ifs.close();
		
		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  	
    }
    catch (...)
    {
        TxLogDebugException();
		ifs.close();
        throw 1;
    }
    TxLogDebugEndMsg();
}

void KAssignmentModelDlg::UpdateOBSingleLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );   
	try
	{		 
		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs)
		{
			ThrowException(_T("File Not Found"));
		}

		__int64 nxLinkID;
		double dValue;
		__int64	nxFTNodeID;

		while (!ifs.eof())
		{
			int nIndex = 0;
			ifs.read( reinterpret_cast<char*>(&nxLinkID), sizeof(Integer) );
			if(ifs.eof()) 
				break;

			KSDoubleRecord* pRecord = records.AddRecord(nxLinkID);
			
			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));
			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));
			
            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // AVerageT
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //vdf Time
            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //vdf Toll Time

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VC
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //SumVoulm
			pRecord->SetAt(nIndex,dValue);
			nIndex++;
			
			std::vector<CString> vecModeName;
			GetUseModeName(vecModeName);
			for (size_t i = 0; i < vecModeName.size(); i++)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // 수단별 volume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
            
			if (true == m_RunningOption.IncludeBusInitialVolume())
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // BusInitVolume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else {
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			}
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  

	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void KAssignmentModelDlg::UpdateOBMultiLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables  = m_pTarget->Tables();
	KIOTable*  pLinkTable = pIOTables->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );    

	try
	{		
		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs)
		{
			ThrowException(_T("File Not Found"));
		}

		__int64 nxLinkID;
		double dValue;
		__int64 nxFTNodeID;

		while (!ifs.eof())
		{
			int nIndex = 0;

			ifs.read( reinterpret_cast<char*>(&nxLinkID), sizeof(Integer) );
			if(ifs.eof())
				break;
			KSDoubleRecord* pRecord = records.AddRecord(nxLinkID);

			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));
			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // AVerageT
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            for (int i = 0; i <m_nModeInVDF; i++)
            {
                ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VDF 별 Time
                pRecord->SetAt(nIndex,dValue);
                nIndex++;
            }

            for (int i = 0; i <m_nModeInVDF; i++)
            {
                ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VDF별 Toll Time
                pRecord->SetAt(nIndex,dValue);
                nIndex++;
            }

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VC
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //SumVoulm
			pRecord->SetAt(nIndex,dValue);
			nIndex++;
            
			std::vector<CString> vecModeName;
			GetUseModeName(vecModeName);
			for (size_t i = 0; i < vecModeName.size(); i++)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // 수단별 volume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}

			if (true == m_RunningOption.IncludeBusInitialVolume())
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			}
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}
	TxLogDebugEndMsg();
}


void KAssignmentModelDlg::UpdateModeODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection)
{
    TxLogDebugStartMsg();
    KIOTables* pIOTables = m_pTarget->Tables();
    KIOTable*        pParaODTable  = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();
    
	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));
	
	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs(strLocation, std::ios::binary );

	try
    {
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		
		KxBulkDbase oBulkDBase(spDBaseConnection, pParaODTable, &rParaODColumnCollection);

		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);
			
			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();

		//delete pRecord;
		QBicDelete(pRecord);
    }
	catch(KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		ifs.close();
		throw ex;
	}
	catch (...)
    {
        TxLogDebugException();
        throw 1;
    }    
    TxLogDebugEndMsg();
}

void KAssignmentModelDlg::UpdateOBSingleModeODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );
	try
	{
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		double dValue;

		KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rParaODColumnCollection);
		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);
			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();
		QBicDelete(pRecord);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}    
	TxLogDebugEndMsg();
}

void KAssignmentModelDlg::UpdateOBMultiModeODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection )
{
	TxLogDebugStartMsg();
 	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );
	try
	{
		

		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		//double dValue;

		KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rParaODColumnCollection);
		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);
			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();
		QBicDelete(pRecord);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}    
	TxLogDebugEndMsg();
}


// KAssignmentModelDlg 메시지 처리기입니다.
BOOL KAssignmentModelDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ASSERT(NULL != m_pTarget);

	m_strTargetLocation = m_pTarget->GetLocation();

	UINT nID = 1;
    
	switch(m_emAModel)
	{
	case KEMAssignmentModelAllorNothing           : nID = IDC_RADIO1; break;
	case KEMAssignmentModelIncremental            : nID = IDC_RADIO2; break;
	case KEMAssignmentModelUserEquilibriumFW      : nID = IDC_RADIO3; break;
	case KEMAssignmentModelUserEquilibriumOB      : nID = IDC_RADIO4; break;
	case KEMAssignmentModelSegment                : nID = IDC_RADIO5; break;
	//case KEMAssignmentModelUserEquilibriumOBMulti : nID = IDC_RADIO6; break;
	}

    CheckRadioButton(IDC_RADIO1, IDC_RADIO5, nID);
	OnRadioClicked(nID);

	InitInputInfoList();
	m_TOverWirteAResultInfo.TbUseData = false;

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(4, 5);

	m_uiBackImage.MoveWindow(&rcClient); 
    m_uiBackImage.SetMarkupText(UIXAML_A_BACKIMAGE);

    m_uiArrow1.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRB); 
    m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
    m_uiArrow3.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRT);
    m_uiArrow4.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRT);

    m_uiArrow5.SetPosition(ZEMArrowPositionCB, ZEMArrowPositionRT);
    m_uiArrow6.SetPosition(ZEMArrowPositionCB, ZEMArrowPositionLT);

    m_uiArrow7.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

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
	CWnd* pPlaceholder = GetDlgItem(IDC_STATIC_MODEL);
	CRect rc;
	pPlaceholder->GetWindowRect(&rc);
	ScreenToClient(&rc);
	int w = rc.Width() - 4;
	int h = rc.Height() - 4;
	UINT dpi = GetDpiForWindow(*pPlaceholder);
	double fontSize = 11 * (dpi / 96.0);

	m_lblModel.MoveWindow(&rc);

	CString modelXamlKR;
	modelXamlKR.Format(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("  <StackPanel  HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='%f'>")
		_T("    <TextBlock TextAlignment='Center' Padding='0, 15, 0, 15' FontWeight='Bold' Foreground='#003366'>도로 통행배정 모형</TextBlock>")
		_T("    <Border Width='%d' Height='1' Background='#6d896a' />")
		_T("    <Border Width='%d' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>"),
		w, h, fontSize, w, w
	);

	CString modelXamlEN;
	modelXamlEN.Format(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='%f'>")
		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")
		_T("    <Border Height='1' Background='#6d896a' />")
		_T("    <Border Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>"),
		w, h, fontSize
	);

	if (KmzSystem::GetLanguage()==KEMKorea) {
		//CString krUiXamlAModelSelected = BUILD_MODEL_UIXAML(&m_lblModel, KR_UIXAML_A_MODEL_SELECTED);
		m_lblModel.SetMarkupText(modelXamlKR);
	}
	else {
		//CString UiXamlAModelSelected = BUILD_MODEL_UIXAML(&m_lblModel, UIXAML_A_MODEL_SELECTED);
		m_lblModel.SetMarkupText(modelXamlEN);
	}
    SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO3)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO4)->m_hWnd, _T(""), _T(""));
    SetWindowTheme( GetDlgItem(IDC_RADIO5)->m_hWnd, _T(""), _T(""));
	//SetWindowTheme( GetDlgItem(IDC_RADIO6)->m_hWnd, _T(""), _T(""));


    if (KmzSystem::IsPassengerSystem())
    {
		GetDlgItem(IDC_BTN_IMPORT)->ShowWindow(SW_HIDE);        
    }

    if (KmzSystem::IsFreightSystem())
    {
		GetDlgItem(IDC_BTN_MULTI)->ShowWindow(SW_HIDE);

		//GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_IMPORT)->SetWindowText(_T("Import Passenger"));
    }
	else
	{
		//명지대 내부버전이 아닌한 Segment Version은 숨김
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);
	GetDlgItem(IDC_RUN)->EnableWindow(FALSE);

	InitModelButton();

    SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 
    KRedrawWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KAssignmentModelDlg::InitInputInfoList(void)
{
    ClearInputInfoList();

	KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);
	AutoType iterDefaultTime = m_mapDefaultODTime.begin();
	AutoType endDefaultTime  = m_mapDefaultODTime.end();
	if(iterDefaultTime != endDefaultTime)
	{
		AutoType iterUserTime = m_mapDefaultODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserTime != endDefaultTime)
		{
			KODTime oODTime = iterUserTime->second;
			m_dAnalysisTime = oODTime.dTimeDuration;
		}
		else
		{
			KODTime oODTime = iterDefaultTime->second;
			m_dAnalysisTime = oODTime.dTimeDuration;
		}
	}

    if (KmzSystem::IsPassengerSystem())
    {
		KMode*						 pMode	      = nullptr;
		KModeManager*				 pModeManager = m_pTarget->ModeManager();
		int							 nModeID     (0);
		CString						 strModeName (_T(""));
		double						 dOccupancy  (0.0);
		double						 dPCE		 (0.0);

		KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);
		KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);

		AutoType iterDefaultMode = m_mapDefaultOccupancyPCEMode.begin();
		AutoType endDefaultMode  = m_mapDefaultOccupancyPCEMode.end();

		AutoType iterGetMode = m_mapModeChocieColumns.begin();
		AutoType endGetMode  = m_mapModeChocieColumns.end();
		size_t nModeColumnCount	 = m_mapModeChocieColumns.size();

		if((iterDefaultMode != endDefaultMode) && (iterGetMode != endGetMode))
		{
			//Default 값이 있는 경우 수행
			AutoType iterUserMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserMode != endDefaultMode)//User Code가 있는 경우
			{
				for(size_t i = 0; i < nModeColumnCount; i++)
				{
					nModeID = iterGetMode->first;

					int nModeCount = pModeManager->GetModeCount();
					for(int i = 0; i < nModeCount; i++)
					{
						int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
						if (nModeIDInManager == nModeID)
						{
							pMode = pModeManager->GetMode(i);

							KATripMatrix::GetDefaultModeInfo(KDefaultParameterMasterCode::USER_MASETER_CODE, m_mapGetDefaultModeInfo);

							AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
							AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
							if(iterDefaultMode != endDefaultMode)
							{
								std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
								KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
								dOccupancy							= oOccupancyPCEMode.dOccupancy;
								dPCE								= oOccupancyPCEMode.dPCE;
							}
							else
							{
								dOccupancy = 1.0;
								dPCE	   = 1.0;
							}

							KAInputInfo* pInputInfo = new KAInputInfo;
							pInputInfo->Mode(pMode);
							pInputInfo->Occupancy(dOccupancy);
							pInputInfo->PCU(dPCE);
							m_InputList.push_back(pInputInfo);

						}
					}
					iterGetMode++;
				}
			}
			else//User Code가 없고 System Code만 있는 경우
			{
				for(size_t i = 0; i < nModeColumnCount; i++)
				{
					nModeID = iterGetMode->first;

					int nModeCount = pModeManager->GetModeCount();
					for(int i = 0; i < nModeCount; i++)
					{
						int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
						if (nModeIDInManager == nModeID)
						{
							pMode = pModeManager->GetMode(i);

							KATripMatrix::GetDefaultModeInfo(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, m_mapGetDefaultModeInfo);

							AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
							AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
							if(iterDefaultMode != endDefaultMode)
							{
								std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
								KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
								dOccupancy							= oOccupancyPCEMode.dOccupancy;
								dPCE								= oOccupancyPCEMode.dPCE;
							}
							else
							{
								dOccupancy = 1.0;
								dPCE	   = 1.0;
							}
							KAInputInfo* pInputInfo = new KAInputInfo;
							pInputInfo->Mode(pMode);
							pInputInfo->Occupancy(dOccupancy);
							pInputInfo->PCU(dPCE);
							m_InputList.push_back(pInputInfo);
						}
					}
					iterGetMode++;
				}
			}
		}
		else
		{
			for(size_t i = 0; i < nModeColumnCount; i++)
			{
				nModeID = iterGetMode->first;

				int nModeCount = pModeManager->GetModeCount();
				for(int i = 0; i < nModeCount; i++)
				{
					int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
					if (nModeIDInManager == nModeID)
					{
						pMode = pModeManager->GetMode(i);
					}
				}
				dOccupancy = 1.0;
				dPCE	   = 1.0;

				KAInputInfo* pInputInfo = new KAInputInfo;
				pInputInfo->Mode(pMode);
				pInputInfo->Occupancy(dOccupancy);
				pInputInfo->PCU(dPCE);
				m_InputList.push_back(pInputInfo);	
			
				iterGetMode++;
			}
		}
    }
    else
    {
		std::vector<KFAModeInfo> vecFAModeInfo;
		KDBaseAssignImport::GetFmodeInfo( m_pTarget, vecFAModeInfo);
		KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);

		size_t nCount = vecFAModeInfo.size();
		for (size_t i = 0; i < nCount; i++)
		{
			KFAModeInfo oInfo = vecFAModeInfo[i];
			KMode* pMode = new KModeFreight(oInfo.nFmode_id, oInfo.strFomdeName);
			KAInputInfo* pInputInfo = new KAInputInfo;
			pInputInfo->Mode(pMode);

			std::vector<KOccupancyPCEMode> vecOccupancy;
			AutoType ufind = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			AutoType sfind = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			AutoType end   = m_mapDefaultOccupancyPCEMode.end();
			if (ufind != end)
			{
				vecOccupancy = ufind->second;
			}
			else if (sfind != end)
			{
				vecOccupancy = sfind->second;
			}

			pInputInfo->Occupancy(1.0);
			pInputInfo->PCU(1.0);

			for (size_t i = 0; i < vecOccupancy.size(); i++)
			{
				KOccupancyPCEMode oOccuInfo = vecOccupancy[i];
				if (oOccuInfo.nModeID == pMode->ModeID())
				{
					pInputInfo->Occupancy(oOccuInfo.dOccupancy);
					pInputInfo->PCU(oOccuInfo.dPCE);
				}
			}

			m_InputList.push_back(pInputInfo);
		}
    }
}

void KAssignmentModelDlg::OnRadioClicked(UINT nID)
{
	switch(nID)
	{
	case IDC_RADIO1: m_emAModel = KEMAssignmentModelAllorNothing;
		break;
	case IDC_RADIO2: m_emAModel = KEMAssignmentModelIncremental;
		break;
	case IDC_RADIO3: m_emAModel = KEMAssignmentModelUserEquilibriumFW;
		break;
	case IDC_RADIO4: m_emAModel = KEMAssignmentModelUserEquilibriumOB;
		break;
	case IDC_RADIO5: m_emAModel = KEMAssignmentModelSegment;
		break;
	}

	InitModelButton();

	GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);

	KRedrawWindow();
}

// Step2.
void KAssignmentModelDlg::OnBnClickedTripMatrix()
{
	KATripMatrix dlg(m_pTarget);
 	dlg.AssignmentODType(m_emODType);
    dlg.AnalysisTime(m_dAnalysisTime);
 	dlg.InputList(m_InputList);
 	dlg.SetModel(m_emAModel);

	// 2019.04.15 : 수단 변경 여부 확인 후 변경되었으면 만약 실행옵션의 웜스타트가 설정된 경우 해당 옵션을 다시 설정하도록 한다.
	bool bChangedInput = false;
	std::vector<bool> arrBeforeInput;
	for (KAInputInfo* pInput : m_InputList)
	{
		arrBeforeInput.push_back(pInput->Selected());
	}

	int nnn = m_OBVDFDlg.GetModeInVdf();

	if(dlg.DoModal() == IDOK)
	{
		std::vector<bool> arrAfterInput;
		for (KAInputInfo* pInput : m_InputList)
		{
			arrAfterInput.push_back(pInput->Selected());
		}
		if (arrBeforeInput.size() != arrAfterInput.size())
		{
			bChangedInput = true;
		}
		else
		{
			for (int i = 0; i < arrBeforeInput.size(); ++i)
			{
				if (arrBeforeInput[i] != arrAfterInput[i])
				{
					bChangedInput = true;
					break;
				}
			}
		}



		m_emODType      = dlg.AssignmentODType();
        m_dAnalysisTime = dlg.AnalysisTime();
		//m_InputList		= dlg.InputInfo();
		m_bUsedVDFDefault  = true;
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnInput.SetWindowText(KR_UIXAML_A_INPUT_SELECTED);
		}
		else {
			m_btnInput.SetWindowText(UIXAML_A_INPUT_SELECTED);
		}

		if (!m_btnDefineVDF.IsWindowEnabled())
		{
			m_btnDefineVDF.EnableWindow(TRUE);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_DEFAULT);
			}
			else {
				m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_DEFAULT);
			}
		}
		else
		{
			//OBUE Model 이면서 VDF를 Multi로 지정했을때는 VDF를 다시 설정하도록 함
			if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
			{
				if (m_OBVDFDlg.GetModeInVdf() > 1)
				{
					if (KmzSystem::GetLanguage()==KEMKorea) {
						m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_DEFAULT);
					}
					else {
						m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_DEFAULT);
					}
					GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);
				}
			}
		}

		if (m_btnRunOptions.IsWindowEnabled())
		{
			m_RunningOption.IncludeWarmStart(false);
			m_RunningOption.Scnario(NULL);
			m_RunningOption.Target(NULL);

			if (KmzSystem::GetLanguage() == KEMKorea) {
				m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_DEFAULT);
			}
			else {
				m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_DEFAULT);
			}
			GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);
		}

		if (m_btnOutputDesign.IsWindowEnabled())
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_DEFAULT);
			}
			else {
				m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_DEFAULT);
			}
			GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);
		}
		
		m_bUsedInputDefault = false;
	}

	KRedrawWindow();
}

void KAssignmentModelDlg::OnBnClickedDefineVDF()
{
	int nCntSelInputMode = 0;
	std::list<KAInputInfo*>::iterator itInput, itEnd = m_InputList.end();
	for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput  = *itInput;

		if (pInput->Selected())
		{
			++nCntSelInputMode;
		}
	}

	if (nCntSelInputMode == 0)
	{
		AfxMessageBox(_T("선택한 통행수단이 없습니다."));
		return;
	}

	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
	{
		m_OBVDFDlg.InputList(m_InputList);
		m_OBVDFDlg.UsedDefault(m_bUsedVDFDefault);
		if (m_OBVDFDlg.DoModal() == IDOK)
		{
			m_OBVDFDlg.GetMultiVDFInfo(m_oTOBMultiVDF);
			m_nModeInVDF = m_OBVDFDlg.GetModeInVdf();
			m_bUsedVDFDefault = false;
			if(m_nModeInVDF > 1)
			{
				bool bUsePreLoad = false;
				std::list<KAInputInfo*>::iterator itInput, itEnd = m_InputList.end();
				for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
				{
					KAInputInfo* pInput  = *itInput;
					if (true == pInput->Selected() && true == pInput->PreLoad())
					{
						bUsePreLoad = true;
						pInput->PreLoad(false);
					}

				}
				if (bUsePreLoad == true)
				{
					AfxMessageBox(_T("수단별 통행지체함수 개수가 2개 이상입니다.\n수단별 기종점 통행자료에서 설정한 사전 배정 선택이 자동으로 해제되었습니다."));
				}
			}

			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_SELECTED);
			}
			else {
				m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_SELECTED);
			}

			if (!m_btnRunOptions.IsWindowEnabled())
			{
				m_btnRunOptions.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_DEFAULT);
				}
				else {
					m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_DEFAULT);
				}
			}			
		}
	}
	else
	{
		oKABPRFnDlg.Target(m_pTarget);
		oKABPRFnDlg.UsedDefault(m_bUsedVDFDefault);
		if(oKABPRFnDlg.DoModal() == IDOK)
		{
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_SELECTED);
			}
			else {
				m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_SELECTED);
			}

			if (!m_btnRunOptions.IsWindowEnabled())
			{
				m_btnRunOptions.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_DEFAULT);
				} 
				else {
					m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_DEFAULT);
				}
			}

			m_bUsedVDFDefault = false;
		}
	}
	KRedrawWindow();
}

// Step3.
void KAssignmentModelDlg::OnBnClickedRunOptions()
{

	if (KmzSystem::IsPassengerSystem())
	{
		KARunOptionDlg dlg;

		dlg.SetTarget(m_pTarget);
		dlg.RunningOption(m_RunningOption);
		dlg.SetAssignMode(m_emAModel);
		dlg.SetVDFCount(m_nModeInVDF);
		dlg.SetInputIinfo(m_InputList);

		if(dlg.DoModal() == IDOK)
		{
			m_RunningOption = dlg.RunningOption();
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_SELECTED);
			}
			else {
				m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_SELECTED);
			}

			if (m_emAModel == KEMAssignmentModelAllorNothing)
			{
				if (!m_btnOutputDesign.IsWindowEnabled())
				{
					m_btnOutputDesign.EnableWindow(TRUE);
					if (KmzSystem::GetLanguage()==KEMKorea) {
						m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_DEFAULT);
					}
					else {
						m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_DEFAULT);
					}
				}				
			}
			else if (m_emAModel == KEMAssignmentModelIncremental )
			{
				if (!m_btnODDemandShare.IsWindowEnabled())
				{
					m_btnODDemandShare.EnableWindow(TRUE);
					if (KmzSystem::GetLanguage()==KEMKorea) {
						m_btnODDemandShare.SetWindowText(KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT);
					}
					else {
						m_btnODDemandShare.SetWindowText(UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT);
					}
				}				
			}
			else if (m_emAModel == KEMAssignmentModelUserEquilibriumFW || m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
			{
				if (!m_btnStoppingCriteria.IsWindowEnabled())
				{
					m_btnStoppingCriteria.EnableWindow(TRUE);
					if (KmzSystem::GetLanguage()==KEMKorea) {
						m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_DEFAULT);
					}
					else {
						m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_DEFAULT);
					}
				}				
			}
			else if (m_emAModel == KEMAssignmentModelMultiModal)
			{
				if (!m_btnODDemandShare.IsWindowEnabled())
				{
					m_btnTransitAssignOptions.EnableWindow(TRUE);
					m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_DEFAULT);
				}				
			}
		}
	}
	else
	{
		KFARunOptionDlg oDlg(m_pTarget, m_emAModel, m_nModeInVDF);
		oDlg.RunningOption(m_RunningOption);

		if (oDlg.DoModal() == IDOK)
		{
			m_RunningOption = oDlg.RunningOption();
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_SELECTED);
			}
			else {
				m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_SELECTED);
			}

			if (m_emAModel == KEMAssignmentModelAllorNothing)
			{
				m_btnOutputDesign.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_DEFAULT);
				}
				else {
					m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_DEFAULT);
				}
			}
			else if (m_emAModel == KEMAssignmentModelIncremental )
			{
				m_btnODDemandShare.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnODDemandShare.SetWindowText(KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT);
				}
				else {
					m_btnODDemandShare.SetWindowText(UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT);
				}
			}
			else if (m_emAModel == KEMAssignmentModelUserEquilibriumFW)
			{
				m_btnStoppingCriteria.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_DEFAULT);
				}
				else {
					m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_DEFAULT);
				}
			}
		}
	}

	KRedrawWindow();
}

void KAssignmentModelDlg::OnBnClickedTransitassign()
{
	if (KEMAssignmentModelUserEquilibriumOB == m_emAModel || KEMAssignmentModelMultiModal == m_emAModel)
    {
        if (oKASetTransitAssignOptionsDlg.DoModal() == IDOK)
        {
            m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_SELECTED);
        }
    }

	if (!m_btnStoppingCriteria.IsWindowEnabled())
	{
		m_btnStoppingCriteria.EnableWindow(TRUE);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_DEFAULT);
		}
		else {
			m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_DEFAULT);
		}
	}	

	KRedrawWindow();
}


void KAssignmentModelDlg::OnBnClickedODDemand()
{
	if(KEMAssignmentModelIncremental == m_emAModel)
	{
		KAODDemandShareDlg dlg;

		dlg.Iteration(m_nIIteration);

		if(dlg.DoModal() == IDOK)
		{
			m_nIIteration		= dlg.Iteration();
			m_TripRateList		= dlg.RateList();
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnODDemandShare.SetWindowText(KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_SELECTED);
			}
			else {
				m_btnODDemandShare.SetWindowText(UIXAML_A_TRANSIT_ODDEMAND_SHARE_SELECTED);
			}

			if (!m_btnOutputDesign.IsWindowEnabled())
			{
				m_btnOutputDesign.EnableWindow(TRUE);
				if (KmzSystem::GetLanguage()==KEMKorea) {
					m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_DEFAULT);
				}
				else {
					m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_DEFAULT);
				}
			}		
		}
	}
		
	KRedrawWindow();
}


void KAssignmentModelDlg::OnBnClickedStoppingCriteria()
{
    bool bOk = false;

	if(KEMAssignmentModelUserEquilibriumFW == m_emAModel)
	{
		KAStoppingCriteriaDlg dlg;
		
		// todo : set Stopping Criteria dialog
		dlg.NumberOfIteration(m_nHighwayIterationNo);
		dlg.Gap(m_dHighwayRelativeGap);
		dlg.AssignmentMode(m_emAModel);

		if(dlg.DoModal() == IDOK)
		{
			m_nHighwayIterationNo	= dlg.NumberOfIteration();
			m_dHighwayRelativeGap	= dlg.Gap();
            bOk = true;
		}
	}
	else if(KEMAssignmentModelUserEquilibriumOB == m_emAModel 
		|| KEMAssignmentModelUserEquilibriumOBMulti == m_emAModel
		|| KEMAssignmentModelSegment == m_emAModel)
	{
        KAStoppingOriginBaseUeDlg dlg;
        dlg.NumberOfIteration(m_nHighwayIterationNo);
        dlg.RelativeGap(m_dHighwayRelativeGap);
        dlg.ZoneCentroidThrought(m_nUEOBaseZoneCentroidThrought);
        dlg.ScaleFactor(m_dUEOBaseScaleFactor);
		dlg.ScaleFactorCursor(m_nScaleFactorCursor);
		
        dlg.RelativeGapExp(m_nHighwayRelativeGapExp);
        
        if(dlg.DoModal() == IDOK)
        {
            m_nUEOBaseZoneCentroidThrought	= dlg.ZoneCentroidThrought();
			m_nHighwayIterationNo			= dlg.NumberOfIteration();
            m_dHighwayRelativeGap			= dlg.RelativeGap();
            m_dUEOBaseScaleFactor			= dlg.ScaleFactor();
            m_nHighwayRelativeGapExp		= dlg.RelativeGapExp();
			m_nScaleFactorCursor			= dlg.ScaleFactorCursor();
            bOk = true;
        }
	}
	else if(KEMAssignmentModelMultiModal == m_emAModel)
	{
		KAStoppingCriteriaMMDlg dlg;

		dlg.HighwayIteration(m_nHighwayIterationNo);
		dlg.RelativeGap(m_dHighwayRelativeGap);
		dlg.TransitIteration(m_nTransitIterationNo);
		dlg.NormalizedGap(m_dTransitNormalizeGap);

		if(dlg.DoModal() == IDOK)
		{
			m_nHighwayIterationNo	= dlg.HighwayIteration();
			m_dHighwayRelativeGap	= dlg.RelativeGap();
			m_nTransitIterationNo	= dlg.TransitIteration();
			m_dTransitNormalizeGap	= dlg.NormalizedGap();

            bOk = true;
		}
	}

    if (bOk)
    {
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_SELECTED);
		}
		else {
			m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_SELECTED);
		}

		if (!m_btnOutputDesign.IsWindowEnabled())
		{
			m_btnOutputDesign.EnableWindow(TRUE);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_DEFAULT);
			}
			else {
				m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_DEFAULT);
			}
		}		
    }
	
	KRedrawWindow();
}


void KAssignmentModelDlg::OnBnClickedOutputdesign()
{
	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);
	m_oOuputInfomationDlg.SetMode(m_pTarget, vecModeName);
	m_oOuputInfomationDlg.SetModelType(m_nModeInVDF, m_emAModel);
	m_oOuputInfomationDlg.SetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
	m_oOuputInfomationDlg.SetBusInitalVolume(m_RunningOption.IncludeBusInitialVolume());
	
	if (m_oOuputInfomationDlg.DoModal() == IDOK)
	{
		m_TOverWirteAResultInfo.TbUseData = true;
		m_oOuputInfomationDlg.GetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_SELECTED);
		}
		else {
			m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_SELECTED);
		}
		
		GetDlgItem(IDC_RUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);
		EnableMultiAssign();
	}
}

void KAssignmentModelDlg::OnBnClickedRun()
{
	m_nResultCode = DLL_CREATE_ARGFILE_ERROR;

	bool bCheck = true;
	
	try
	{
		ThreadPara oPara(this);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		if (true) {
			oPara.TKeyInt[0] = 0; // 온전한 성공여부 1:성공, 0:실패

			RThreadInfo.Init();
			QBicListSimpleProgressThread::ExecuteThread(AssignmentModelThreadProc, &oPara, bCheck, nLang);

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			}
			// 성공

			if (1 == oPara.TKeyInt[0]) { // 완전 성공
				KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
				pLinkTable->Notify();
				KIOTable* pModeODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
				pModeODTable->Notify();
			}
		}
	}
	catch (int& ex) {
		CString strError(_T(""));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strError.Format(_T("오류 : %d"), ex);
		}
		else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	}
	catch (...) {
		TxLogDebugException();
	}

	if (DLL_PLANNED_TERMINATION == m_nResultCode)
	{
		InitializationModelData();
		m_btnViewTable.EnableWindow(TRUE);
		m_btnViewLog.EnableWindow(TRUE);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnViewTable.SetWindowTextW(KR_UIXAML_A_VIEWTABLE_DEFAULT);
			m_btnViewLog.SetWindowTextW(KR_UIXAML_A_VIEWLOG_DEFAULT);
		}
		else {
			m_btnViewTable.SetWindowTextW(UIXAML_A_VIEWTABLE_DEFAULT);
			m_btnViewLog.SetWindowTextW(UIXAML_A_VIEWLOG_DEFAULT);
		}
		
        if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
        {//32Bit 또는 64Bit / VDF(멀티)인지 확인 필요-멀티는 무조건 32Bit(18.07.13 일자로 FOriginBasedUE_MC_64.dll 추가됨에 따라 멀티도 64bit 가능)
            if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
            {
                CopyFile(_T("assignment.arg"), ConvertSavePath(_T("assignment.arg")), FALSE);
            }
            else
            {
                CString strFileName = ImChampDir::GetAppDirectory() + _T("\\x64\\assignment.arg");
                CopyFile(strFileName, ConvertSavePath(_T("assignment.arg")), FALSE);
            }            
        }
        else
        {
            CopyFile(_T("assignment.arg"), ConvertSavePath(_T("assignment.arg")), FALSE);	
        }
	}
	else
	{
		m_btnViewTable.EnableWindow(TRUE);
		m_btnViewLog.EnableWindow(TRUE);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnViewTable.SetWindowTextW(KR_UIXAML_A_VIEWTABLE_DEFAULT);
			m_btnViewLog.SetWindowTextW(KR_UIXAML_A_VIEWLOG_DEFAULT);
		}
		else {
			m_btnViewTable.SetWindowTextW(UIXAML_A_VIEWTABLE_DEFAULT);
			m_btnViewLog.SetWindowTextW(UIXAML_A_VIEWLOG_DEFAULT);
		}
	}

	GetDlgItem(IDC_RUN)->EnableWindow(FALSE);

	KRedrawWindow();
}

void KAssignmentModelDlg::OnBnClickedBtnMulti()
{
	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB && m_RunningOption.IncludeWarmStart())
	{
		MultiAssignModelSOBADlg oDlg(m_pTarget, m_emAModel, m_nModeInVDF, this);
		oDlg.ODType(m_emODType);
		oDlg.AnalysisTime(m_dAnalysisTime);
		oDlg.RunningOption(m_RunningOption);
		oDlg.StoppingCriteria(m_nHighwayIterationNo, m_dHighwayRelativeGap, m_nUEOBaseZoneCentroidThrought, m_dUEOBaseScaleFactor,
			m_nTransitIterationNo, m_dTransitNormalizeGap);
		oDlg.TripRateList(m_TripRateList);

		if (oDlg.DoModal() == IDOK)
		{
			OnBnClickedClose();
		}
	}
	else
	{
		KMultiAssignModelDlg oDlg(m_pTarget, m_emAModel, m_nModeInVDF, this);
		oDlg.ODType(m_emODType);
		oDlg.AnalysisTime(m_dAnalysisTime);
		oDlg.RunningOption(m_RunningOption);
		oDlg.StoppingCriteria(m_nHighwayIterationNo, m_dHighwayRelativeGap, m_nUEOBaseZoneCentroidThrought, m_dUEOBaseScaleFactor,
			m_nTransitIterationNo, m_dTransitNormalizeGap);
		oDlg.TripRateList(m_TripRateList);

		if (oDlg.DoModal() == IDOK)
		{
			OnBnClickedClose();
		}
	}
}

unsigned __stdcall KAssignmentModelDlg::AssignmentModelThreadProc(void* pParam)
{
	QBicListSimpleProgressParameter* pParameter = (QBicListSimpleProgressParameter*)pParam;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KAssignmentModelDlg* pDlg = (KAssignmentModelDlg*)pPara->TWindow;

			pDlg->RunAssignmentModel(pPara);
		}
	}

	return 1;
}


void KAssignmentModelDlg::RunAssignmentModel(ThreadPara* pPara)
{	
	//Incldue WarmStart일 경우에는 Bush 파일 삭제 안함 자기 자신의 Bush 파일을 이용하여 WarmStart할 경우도 있기 때문
	if (m_RunningOption.IncludeWarmStart() == false) {
		DeleteBushFile();
	}

	CString strModelFolder;
	strModelFolder.Format(_T("%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);
	CreateDirectory(strModelFolder, NULL);
	CreateDirectory(ConvertSavePath(_T("")), NULL);

    TxLogDebugStartMsg();
	// Step 1: Argument 생성
	AddStatusMessage(_T("> Creating Argument File..."));
	KIOColumns  modeColumnCollection;

	CString strNetworkCodeFolder(_T(""));
	strNetworkCodeFolder.Format(_T("%s\\%s"), DLL_MODEL_FOLDER_NAME, DLL_HIGHWAYASSIGN_FOLDER_NAME);

	try 
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, strNetworkCodeFolder);
	} catch (...) {
		TxLogDebugWarning();
		return;
	}

	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
	{
		if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) {
			AddStatusMessage(_T("> 32bit Process"));
		} else {
			AddStatusMessage(_T("> 64bit Process"));
		}

		if ( ! CreateOBArgument(modeColumnCollection) ) {
			TxLogDebugWarning();
			return;
		}
	}
	else
	{
		if ( ! CreateArgument(modeColumnCollection) )
		{
			TxLogDebugWarning();
			return;
		}
	}

	AddStatusMessage(_T("> Create Argument File End."));

	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		return;
	}

	// Step 2: Dll Load
	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	AddStatusMessage(_T("> Assignment Start..."));

	CTime timeStart = CTime::GetCurrentTime();

	CTime timeDoneDll = CTime::GetCurrentTime();
	CString strDllStartTime = _T("");
	strDllStartTime.Format(_T("[Start][%s]"), timeDoneDll.Format(_T("%Y-%m-%d %H:%M:%S")));
	//iteDllLog(strDllStartTime);

	// 라이브러리 로드
	if(KEMAssignmentModelUserEquilibriumOB == m_emAModel) {		
		if (m_nModeInVDF < 2) //OBUE Single 일때
		{
			if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) { //32bit				
				if (m_RunningOption.IncludeWarmStart() == true) {
					//Warm Start 일때
					KLauncher::Run(strWindowName, 31);
				} else {
					//Not Warm Start
					KLauncher::Run(strWindowName, 34);
				}
				
				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
				{//AddStatusMessage(_T("> OBUE Assignment Complete"));
				} else if (DLL_LOAD_ERROR == m_nResultCode) {
					AddStatusMessage(_T("> Loading DLL File Error."));
					return;
				} else if (DLL_USER_STOPPING == m_nResultCode) {
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				} else {
					if (QBicListSimpleProgressThread::IsUserStopped()) {
						AddStatusMessage(_T("> User Stopped..."));
						TxLogDebug(_T("User Stopped..."));
					} else {
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						TxLogDebug(_T("Assignment Run Error..."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용
				if (m_RunningOption.m_nIteration != 0) {
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 35);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/) {
						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					} else if (DLL_USER_STOPPING == m_nResultCode) {
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					} else if (DLL_LOAD_ERROR == m_nResultCode)	{
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					} else {
						if (QBicListSimpleProgressThread::IsUserStopped()) {
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stopped..."));
						} else {
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 36);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)					{
						//AddStatusMessage(_T(">Balancing P2 Complete..."));
					} else if (DLL_LOAD_ERROR == m_nResultCode) {
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					} else if (DLL_USER_STOPPING == m_nResultCode) {
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					} else 	{
						if (QBicListSimpleProgressThread::IsUserStopped()) {
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						} else {
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
			else
			{//64bit				
				if (m_RunningOption.IncludeWarmStart() == true) { // Warm Start 					
					KLauncher::Run64(strWindowName, 1004);
				} else { //Not Warm Start
					KLauncher::Run64(strWindowName, 1001);
				}

				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)				{
					//AddStatusMessage(_T("> OBUE Assignment Complete"));
				} else if (DLL_LOAD_ERROR == m_nResultCode) {
					AddStatusMessage(_T("> Loading DLL File Error."));
					return;
				} else if (DLL_USER_STOPPING == m_nResultCode) {
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				} else {
					if (QBicListSimpleProgressThread::IsUserStopped()) {
						AddStatusMessage(_T("> User Stopped..."));
						TxLogDebug(_T("User Stop.."));
					} else {
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						TxLogDebug(_T("Assignment Run Error.."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용 
				if (m_RunningOption.m_nIteration != 0) {
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1002);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					} else if (DLL_USER_STOPPING == m_nResultCode) {
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					} else if (DLL_LOAD_ERROR == m_nResultCode) {
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					} else {
						if (QBicListSimpleProgressThread::IsUserStopped()) {
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						} else {
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1003);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{//AddStatusMessage(_T(">Balancing P2 Complete..."));
					} else if (DLL_LOAD_ERROR == m_nResultCode) {
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					} else if (DLL_USER_STOPPING == m_nResultCode) {
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					} else {
						if (QBicListSimpleProgressThread::IsUserStopped()) {
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						} else {
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
		}
		else
		{ //OBUE Muliti 일때
		    
            // 32bit
            if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) {

                KLauncher::Run(strWindowName, 4);
            }
            else {

                KLauncher::Run64(strWindowName, 1007);
            }
		}
	}
	else if (KEMAssignmentModelSegment == m_emAModel)
	{
		if (m_nModeInVDF < 2)
		{
			if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
			{
				//Not Warm Start
				KLauncher::Run(strWindowName, 41);

				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
				{
					//AddStatusMessage(_T("> OBUE Assignment Complete"));
				}
				else if (DLL_LOAD_ERROR == m_nResultCode)
				{
					AddStatusMessage(_T("> Loading DLL File Error."));
					return;
				}
				else if (DLL_USER_STOPPING == m_nResultCode)
				{
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				}
				else
				{
					if (QBicListSimpleProgressThread::IsUserStopped())
					{
						AddStatusMessage(_T("> User Stopped..."));
						TxLogDebug(_T("User Stop.."));
					}
					else
					{
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						TxLogDebug(_T("Assignment Run Error.."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용
				if (m_RunningOption.m_nIteration != 0)
				{
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 35);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 36);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P2 Complete..."));
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
			else //64Bit
			{
				//Not Warm Start
				KLauncher::Run64(strWindowName, 1005);
				
				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
				{
					//AddStatusMessage(_T("> OBUE Assignment Complete"));
				}
				else if (DLL_LOAD_ERROR == m_nResultCode)
				{
					AddStatusMessage(_T("> Loading DLL File Error."));
					return;
				}
				else if (DLL_USER_STOPPING == m_nResultCode)
				{
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				}
				else
				{
					if (QBicListSimpleProgressThread::IsUserStopped())
					{
						AddStatusMessage(_T("> User Stopped..."));
						TxLogDebug(_T("User Stop.."));
					}
					else
					{
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						TxLogDebug(_T("Assignment Run Error.."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용 
				if (m_RunningOption.m_nIteration != 0)
				{
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1002);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1003);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P2 Complete..."));
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						return;
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
		}
		else
		{
			//Segment Multi일때는 OBUE와 동일하게 처리

            // 32bit
            if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) {

                KLauncher::Run(strWindowName, 4);
            }
            else {

                KLauncher::Run64(strWindowName, 1007);
            }
		}
	}
	else
	{
		//일반 Assign
		KLauncher::Run(strWindowName, 2);
	}


	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		return;
	}

	timeDoneDll = CTime::GetCurrentTime();
	CString strDllEndTime = _T("");
	strDllEndTime.Format(_T("[ End ][%s]"), timeDoneDll.Format(_T("%Y-%m-%d %H:%M:%S")));
	//WriteDllLog(strDllEndTime);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		AddStatusMessage(_T("> Assignment End."));
		TxLogDebug(_T("Model Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		TxLogDebug(_T("Model Stop.."));
		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Assignment DLL Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));
		return;
	}
	else
	{
		if (QBicListSimpleProgressThread::IsUserStopped())
		{
			AddStatusMessage(_T("> User Stopped..."));
			TxLogDebug(_T("User Stop.."));
		}
		else
		{
			AddStatusMessage(_T("> Assignment Module End with Error..."));
			TxLogDebug(_T("Model Run Error.."));
		}
		return;
	}

	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		return;
	}

	/*Step 5. 결과 정보 저장*/
	AddStatusMessage(_T("> Saving Output Data..."));
	
	/*Step 5.1 Column 추가*/
	//AddStatusMessage(_T("> Column 추가 ..."));
	KIOColumns rLinkColumnCollection, rParaODColumnCollection;
	TAssignResultColumnName oresultColumnName;
	InitTAssingReulstColumn(oresultColumnName);
    
	int nRunSeq(0);

	CString strDescription = _T("");
	strDescription = m_TOverWirteAResultInfo.TstrDescription;

	try
	{
		nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);

		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
		{
			if (m_nModeInVDF < 2) {
				AddAssignOBSingeResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, oresultColumnName, strDescription);
			}
			else {
				AddAssignOBMultiResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, oresultColumnName, strDescription);
			}
		} 
        else
		{
			AddAssignResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, oresultColumnName, strDescription);
		}
	} catch(KExceptionPtr ex) {
		TxLogDebug(ex->GetErrorMessage());
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AfxMessageBox(_T("데이터 컬럼 추가 시 예상치 못한 오류가 발생하였습니다."));
		return;
	} catch(...) {
		TxLogDebugException();
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AfxMessageBox(_T("데이터 컬럼 추가 시 예상치 못한 오류가 발생하였습니다."));
		return;
	}

	/*Step 5.3 UpdateDataBase*/
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.1 링크 정보 입력 ...*/		
		//추가 
		AddStatusMessage(_T("> Saving Output Data(Link Table)..."));
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment )
		{
			if (m_nModeInVDF < 2)
			{
				UpdateOBSingleLinkFile2DB(spDBaseConnection, rLinkColumnCollection);
			}
			else
			{
				UpdateOBMultiLinkFile2DB(spDBaseConnection, rLinkColumnCollection);
			}
		}
		else
		{
			UpdateLinkFile2DB(spDBaseConnection, rLinkColumnCollection);
		}

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.2 수단 OD 정보 입력 ...*/
		AddStatusMessage(_T("> Saving Output Data(Attribute Table)..."));
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
		{
			if (m_nModeInVDF < 2)
			{
				UpdateOBSingleModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
			}
			else
			{
				UpdateOBMultiModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
			}
		}
		else
		{
			UpdateModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
		}

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.4. 배정모형 종료정보 업데이트 ...*/
		CTime timeEnd = CTime::GetCurrentTime();

		nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);
		
		if (m_bUseOverWrite == true)
		{
			//OverWrite일경우 기존 데이터 삭제
			DeleteOverWriteAssignModel(spDBaseConnection);
		}

		int nType = 1;
		if (m_emAModel == KEMAssignmentModelIncremental)
		{
			nType = 2;
		}
		else if (m_emAModel == KEMAssignmentModelUserEquilibriumFW)
		{
			nType = 3;
		}
		else if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelUserEquilibriumOBMulti || m_emAModel == KEMAssignmentModelSegment)
		{
			nType = 4;
		}

		//AssignModel 정보 입력
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
		{
			if (m_nModeInVDF < 2)
			{
				KDBaseAssignment::UpdateOBSingleARunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")),
					strDescription, nType);
			}
			else
			{
				KDBaseAssignment::UpdateOBMultiARunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")),
					strDescription, nType);
			}
		}
		else
		{
			KDBaseAssignment::UpdateAssignRunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), 
				strDescription, nType);
		}

		int nIncludeTurn = 0;
		if (m_RunningOption.IncludeTurnRestriction() == true)
		{
			nIncludeTurn = 1;
		}

		//Include Path Analysis 일 경우 추가 정보 DB에 Write
		if (m_RunningOption.IncludePathAnalysis())
		{
			KDBaseAssignment::UpdateAssignPathmodel(spDBaseConnection, nRunSeq, nIncludeTurn, m_nUEOBaseZoneCentroidThrought ,oresultColumnName.AverageTime, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
			KDBaseAssignment::UpdateAssignpathInfo(spDBaseConnection, m_InputList, oresultColumnName);
		}

		//Turn Volume 정보 저장을 위하여 DB에 Write
		KDBaseAssignment::UpdateAssignTurnVolumeInfo(spDBaseConnection, m_InputList, m_RunningOption);

		rLinkColumnCollection.Clear();
		rParaODColumnCollection.Clear();
		spDBaseConnection->Commit();

	}
	catch(KExceptionPtr ex)
	{
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AddStatusMessage(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		spDBaseConnection->RollBack();
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AddStatusMessage(_T("Assignment Running Failed."));
		TxLogDebugException();
		return;
	}

	try
	{
		/*Column 삭제*/
		if (true == DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection))
		{
			if (m_bUseOverWrite == true)
			{
				KDBaseAssignment::DeleteOverWriteColumn(m_pTarget, m_TOverWirteAResultInfo);
			}
		}

		pPara->TKeyInt[0] = 1; // 성공
		AddStatusMessage(_T("> Assignment Complete."));
		TxLogDebugEndMsg();
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}

bool KAssignmentModelDlg::DeleteOverWriteAssignModel(KDBaseConPtr spDBaseConnection)
{

	int nRunSeq = m_TOverWirteAResultInfo.TnRunSeq;

	CString strQuery = _T("");
	strQuery.Format(_T("Delete from assign_model where run_seq = %d "), nRunSeq);

	spDBaseConnection->ExecuteUpdate(strQuery);
	
	return true;
}

void KAssignmentModelDlg::AddStatusMessage(CString strMsg)
{
	QBicListSimpleProgressPtr spProgressWindow = QBicListSimpleProgressThread::SimpleProgress();
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(CString(strMsg));
	}
}


bool KAssignmentModelDlg::CreateOBArgument( KIOColumns& modeColumnCollection )
{
	if (! ClearFiles())
        return false;

	CString strFileName(_T(""));

	if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) {
		strFileName = ImChampDir::GetAppDirectory() + _T("\\assignment.arg");
	} else {
		strFileName = ImChampDir::GetAppDirectory() + _T("\\x64\\assignment.arg");
	}
	
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	bool bResult = false;
	// Step 0. Project Info
	bResult = WriteInfoArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteInfoArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 1. Model
	bResult = WriteModelArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteModelArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 2. Input
	bResult = WriteInputOBArgument(of, modeColumnCollection);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step3. Run Options
	bResult = WriteRunOptionsArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteRunOptionsArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	//Setp3-1 Bush 경로
	bResult = WriteBushTempLocation(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteBushTemp) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step4. VDF
	bResult = WriteOBVDfArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteVDFArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step5. Transit Assign Option
	bResult = WriteTransitAssignOptionArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step6. Set OD Demand Share
	bResult = WriteODShareArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteODShareArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step7. Stopping Criteria
	bResult = WriteStoppingCriteria(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteStoppingCriteria) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = WriteFolderArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step8 Warm Start
	if (true == m_RunningOption.IncludeWarmStart())
	{
		bResult = WriteFolderWarmStart(of);
		if (!bResult)
		{
			of.Close();
			AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
	}
	// Step9. Balancing
	bResult = WriteBalancing(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(Balancing) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step10. 출력제목
	bResult = WriteModelTitle(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(출력 제목) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	//include 데이터 생성
	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;

	KDBaseAssignment::GetAssignmentTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);
	KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);
    {
        CString strTemp(_T(""));
        strTemp.Format(_T("Node Count : %d / Link Count : %d"), setUsableNodeData.size(), setUsableLinkData.size());
        TxLogDebug((LPCTSTR)strTemp);
    }
    
	// 노드파일 생성
	//bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection());
	CString strNodeLocation = ConvertSavePath(_T("node.dat"));
	bResult	= KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), strNodeLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 링크파일 생성
	CString strLinkLocation = ConvertSavePath(_T("link.dat"));
	bResult = m_OBVDFDlg.ExportDB2LinkFileExcludeType(m_pTarget->GetDBaseConnection(), setUsableLinkData, strLinkLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 수단파일 생성
	CString strODLocation = ConvertSavePath(_T("mode_od.dat"));
    if (KmzSystem::IsPassengerSystem())
    {
        bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), _T("mode_od"), modeColumnCollection, strODLocation);
    }
    else
    {
        bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_VEHICLE_MODE_OD, modeColumnCollection, strODLocation);
    }    

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 회전정보파일생성 등등
	if ( m_RunningOption.IncludeTurnRestriction() )
	{
		//bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection());
		CString strTurnLocation = ConvertSavePath(_T("turn.dat"));
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(),  strTurnLocation);
	}
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 라인정보 생성
	CString strTransitLineLocation = ConvertSavePath(_T("line.dat"));
	CString strTransitInfoLocation = ConvertSavePath(_T("line_info.dat"));
	bResult = KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(),strTransitInfoLocation,strTransitLineLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(Transit) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	return true;
}


bool KAssignmentModelDlg::CreateArgument(KIOColumns& modeColumnCollection)
{
    if (! ClearFiles())
    {
        return false;
    }

	ImChampDir::SetCurrentDirectoryToApp();

    CString strFileName = ImChampDir::GetAppDirectory() + _T("\\assignment.arg");
    UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
    CStdioFileEx of(strFileName, nOpenMode);

    bool bResult = false;
	
	// Step 0. Project Info
	bResult = WriteInfoArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteInfoArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

    // Step 1. Model
    bResult = WriteModelArgument(of);
    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(WriteModelArgument) 중 오류가 발생하였습니다. !!!"));
        return false;
    }
    // Step 2. Input
	bResult = WriteInputArgument(of, modeColumnCollection);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step3. Run Options
	bResult = WriteRunOptionsArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteRunOptionsArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

    // Step4. VDF
    bResult = WriteVDFArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteVDFArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
    // Step5. Transit Assign Option
	bResult = WriteTransitAssignOptionArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
    // Step6. Set OD Demand Share
	bResult = WriteODShareArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteODShareArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
    // Step7. Stopping Criteria
	bResult = WriteStoppingCriteria(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteStoppingCriteria) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = WriteFolderArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 출력제목
	bResult = WriteModelTitle(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(출력 제목) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
	of.Close();

	//include 데이터 생성

	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;

	
	KDBaseAssignment::GetHighwayAssignmentTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);

	KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);


	// 노드파일 생성
	//bResult = KBulkDBase2File::ExportDB2NodeFileExcludeNodeOff(m_pTarget->GetDBaseConnection());

	CString strNodeLocation = ConvertSavePath(_T("node.dat"));

	//bResult = KBulkDBase2File::ExportDB2NodeFileExcludeType(m_pTarget->GetDBaseConnection(), setUsableNodeData, strNodeLocation);

	bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), strNodeLocation);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 링크파일 생성

	CString strAddQuery = oKABPRFnDlg.GetAddQuery();
	CString strLinkLocation = ConvertSavePath(_T("link.dat"));
	bResult	= oKABPRFnDlg.ExportDB2LinkExcludeType(m_pTarget->GetDBaseConnection(), strAddQuery, setUsableLinkData, strLinkLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

    // 수단파일 생성
	CString strODLocation = ConvertSavePath(_T("mode_od.dat"));
    if (KmzSystem::IsPassengerSystem())
    {
        bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), _T("mode_od"), modeColumnCollection, strODLocation);
    }
    else
    {
        bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_VEHICLE_MODE_OD, modeColumnCollection, strODLocation);
    }    

    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    // 회전정보파일생성 등등
    if ( m_RunningOption.IncludeTurnRestriction() )
    {
        //bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection());
		CString strTurnLocation = ConvertSavePath(_T("turn.dat"));
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), strTurnLocation);
    }
    if (!bResult)
    {
        AfxMessageBox(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
        return false;
    }

    // Transit 파일 생성
   
	CString strTransitLineLocation = ConvertSavePath(_T("line.dat"));
	CString strTransitInfoLocation = ConvertSavePath(_T("line_info.dat"));
	bResult = KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(),strTransitInfoLocation,strTransitLineLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(Transit) 중 오류가 발생하였습니다. !!!"));
		return false;
	}       

	if (KmzSystem::IsPassengerSystem() == true && KEMAssignmentModelUserEquilibriumFW == m_emAModel && true == m_RunningOption.IncludeIntersectionDelay())
	{
		CString strInterType = ConvertSavePath(_T("InterType.dat"));
		KDBaseAssignment::InterSectionTypeDB2File(m_pTarget->GetDBaseConnection(), strInterType);

		CString strMovemnet = ConvertSavePath(_T("Movement.dat"));
		KDBaseAssignment::InterSectionMovementDB2File(m_pTarget->GetDBaseConnection(), strMovemnet);

		CString strBoundType = ConvertSavePath(_T("BoundTurnType.dat"));
		KDBaseAssignment::InterSectionBoundTurnTypeDB2File(m_pTarget->GetDBaseConnection(), strBoundType);

		CString strPhaseOper = ConvertSavePath(_T("PhaseOper.dat"));
		KDBaseAssignment::InterSectionPhaseOperDB2File(m_pTarget->GetDBaseConnection(), strPhaseOper);
	}

    return true;
}


bool KAssignmentModelDlg::ClearFiles()
{
    TxLogDebugStartMsg();
    // 생성 파일 삭제
    int nResult = -1;

    try
    {
        nResult = remove("assignment.arg");
		nResult = remove("x64\\assignment.arg");
        nResult = remove((CStringA)ConvertSavePath(_T("node.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("link.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("para_od.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("turn.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("line_info.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("line.dat")));

        // 결과 파일 삭제
        nResult = remove((CStringA)ConvertSavePath(_T("out_link.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("out_od.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("line_info_out.dat")));
        nResult = remove((CStringA)ConvertSavePath(_T("line_out.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("turnvolume.dat")));
		//nResult = remove((CStringA)ConvertSavePath(_T("Assignment_Log.txt")));
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }    

    return true;
    TxLogDebugEndMsg();
}


// Step 0. ProjectInfo
bool KAssignmentModelDlg::WriteInfoArgument( CStdioFileEx& of )
{
	CString strOut;

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();

	try
	{
		strOut.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear());
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


// Step 1. Model
bool KAssignmentModelDlg::WriteModelArgument(CStdioFileEx& of)
{
    CString strOut;

    try
    {
        strOut.Format(_T("model\t1\r\n"));
        of.WriteString(strOut);
        strOut.Format(_T("%d\r\n"), m_emAModel);
        of.WriteString(strOut);
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

// Step 2. Input
bool KAssignmentModelDlg::WriteInputArgument(CStdioFileEx& of, KIOColumns& selectedModes)
{    
    //KIOColumn* pColumn = NULL;    
    CStringArray strOutArray;
    CString      strOut;

    selectedModes.Clear();

    try
    {
        // Step 2.1 Select OD Type
        strOut.Format(_T("%d\t%f\r\n"), m_emODType, m_dAnalysisTime);
        strOutArray.Add(strOut);
        // Step 2.2 Select Demand
        std::list<KAInputInfo*>::iterator iter = m_InputList.begin();
        std::list<KAInputInfo*>::iterator end  = m_InputList.end();

        int nSelectedCount = 0;
        while ( iter != end )
        {
            KAInputInfo* pInputInfo = *iter;
            if ( pInputInfo->Selected() )
            {
                KIOColumn* pColumn = pInputInfo->InputColumn();
                if (pColumn)
                {
                    int nColumnIndex = selectedModes.AddNewColumn(pColumn);
                    strOut = pInputInfo->GetArgumentInfo(nColumnIndex + 1); //OZone(0) / DZone(0) 1를 시작...
                    strOutArray.Add(strOut);  
                    nSelectedCount++;
                }                
            }
            ++iter;
        }

        if (nSelectedCount == 0)
        {
            strOut.Format(_T("input\t%d\r\n"), 0);
            of.WriteString(strOut);
            return false;
        }

        int nCount = strOutArray.GetCount();
        strOut.Format(_T("input\t%d\r\n"), nCount);
        of.WriteString(strOut);

        for (int i=0; i<nCount; i++)
        {
			if (i > 0) { // 수단 OD 데이터 컬럼명
				KIOColumn* pColumn = selectedModes.GetColumn(i - 1);
				strOut.Format(_T("%s\r\n"), pColumn->Caption());
				of.WriteString(strOut);
			}
			CString strTemp = strOutArray[i];
            of.WriteString(strOutArray[i]);
        }
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}


bool KAssignmentModelDlg::WriteInputOBArgument( CStdioFileEx& of, KIOColumns& selectedModes )
{
	//KIOColumn* pColumn = NULL;    
	CStringArray strOutArray;
	CString      strOut;

	selectedModes.Clear();

	try
	{
		// Step 2.1 Select OD Type
		strOut.Format(_T("%d\t%f\r\n"), m_emODType, m_dAnalysisTime);
		strOutArray.Add(strOut);
		// Step 2.2 Select Demand
		std::list<KAInputInfo*>::iterator iter = m_InputList.begin();
		std::list<KAInputInfo*>::iterator end  = m_InputList.end();

		int nSelectedCount = 0;
		while ( iter != end )
		{
			KAInputInfo* pInputInfo = *iter;
			if ( pInputInfo->Selected() )
			{
				KIOColumn* pColumn = pInputInfo->InputColumn();
				if (pColumn)
				{
					int nColumnIndex = selectedModes.AddNewColumn(pColumn);
					int nModeID = pInputInfo->Mode()->ModeID();
					strOut = pInputInfo->GetOBArgumentInfo(nColumnIndex + 1, m_oTOBMultiVDF.TmapVDFModeMating, m_nModeInVDF, nModeID); //OZone(0) / DZone(0) 1를 시작...
					strOutArray.Add(strOut);  
					nSelectedCount++;
				}                
			}
			++iter;
		}

		if (nSelectedCount == 0)
		{
			strOut.Format(_T("input\t%d\r\n"), 0);
			of.WriteString(strOut);
			return false;
		}

		int nCount = strOutArray.GetCount();
		strOut.Format(_T("input\t%d\r\n"), nCount);
		of.WriteString(strOut);

		for (int i=0; i<nCount; i++)
		{
			if (i > 0) { // 수단 OD 데이터 컬럼명
				KIOColumn* pColumn = selectedModes.GetColumn(i - 1);
				strOut.Format(_T("%s\r\n"), pColumn->Caption());
				of.WriteString(strOut);
			}

			CString strTemp = strOutArray[i];
			of.WriteString(strOutArray[i]);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

// Step3. Run Options
bool KAssignmentModelDlg::WriteRunOptionsArgument(CStdioFileEx& of)
{
    CString strOut;
    try
    {
        strOut.Format(_T("run_options\t%d\r\n"), 1);
        of.WriteString(strOut);

        of.WriteString(m_RunningOption.GetArgumentInfo());
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

// Step3-1. Bush File Location Make & WrieteTemp
bool KAssignmentModelDlg::WriteBushTempLocation( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		CString strTargetLocation = m_pTarget->GetLocation();
		CString strTempLocation = strTargetLocation + _T("\\") + BUSH_PATHVOLUMN_FOLDER_NAME;

		//DeletePath(strTempLocation);

		CreateDirectory(strTempLocation, NULL);

		strOut.Format(_T("%s\r\n"), strTempLocation);

		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

// Step4. VDF
bool KAssignmentModelDlg::WriteVDFArgument(CStdioFileEx& of)
{
	bool bResult = false;

	try
	{
		bResult = oKABPRFnDlg.WriteInputArgument(of);
	}
	catch(...)
	{
		return false;
	}

	return bResult;
}
 

bool KAssignmentModelDlg::WriteOBVDfArgument( CStdioFileEx& of )
{
	bool bResult = false;

	try
	{
		bResult = m_OBVDFDlg.WriteInputArgument(of);
	}
	catch(...)
	{
		return false;
	}

	return bResult;
}

// Step 5
bool KAssignmentModelDlg::WriteTransitAssignOptionArgument(CStdioFileEx& of)
{
    // TODO : 다이얼로그 구현 후 처리
    CString strOut;
    try
    {
        if (KEMAssignmentModelMultiModal == m_emAModel || KEMAssignmentModelUserEquilibriumOBMulti == m_emAModel || KEMAssignmentModelSegment == m_emAModel)
        {
            oKASetTransitAssignOptionsDlg.WriteInputArgument(of);
        }
        else
        {
            strOut.Format(_T("transit_assign_options\t%d\r\n"), 0);
            of.WriteString(strOut);
        }        
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }    

    return true;
}

// Step 6
bool KAssignmentModelDlg::WriteODShareArgument(CStdioFileEx& of)
{
    CString strOut;

    try
    {
        int  nCount = m_TripRateList.size();

        if (nCount <= 0)
        {
            strOut.Format(_T("od_share\t%d\r\n"), 0);
            of.WriteString(strOut);
            return true;
        }

        strOut.Format(_T("od_share\t%d\r\n"), nCount + 1);
        of.WriteString(strOut);

        strOut.Format(_T("%d\r\n"), nCount);
        of.WriteString(strOut);

        std::list<double>::iterator iter = m_TripRateList.begin();
        std::list<double>::iterator end  = m_TripRateList.end();

        int nSeq = 0;
        while (iter != end)
        {
            strOut.Format(_T("%d\t%f\r\n"), ++nSeq, *iter);
            of.WriteString(strOut);
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
 
// Step 7 Stopping Criteria
bool KAssignmentModelDlg::WriteStoppingCriteria(CStdioFileEx& of)
{
    CString strOut;
    try
    {
        strOut.Format(_T("stopping_criteria\t%d\r\n"), 1);
        of.WriteString(strOut);
        if ( KEMAssignmentModelUserEquilibriumOB == m_emAModel 
			|| KEMAssignmentModelUserEquilibriumOBMulti == m_emAModel
			|| KEMAssignmentModelSegment == m_emAModel)
        {
            strOut.Format(_T("%d\t%g\t%d\t%f\r\n"), 
                               m_nHighwayIterationNo,  m_dHighwayRelativeGap, 
                               m_nUEOBaseZoneCentroidThrought, m_dUEOBaseScaleFactor);
        }
        else
        {
            strOut.Format(_T("%d\t%g\t%d\t%f\r\n"), 
                               m_nHighwayIterationNo,  m_dHighwayRelativeGap, 
                               m_nTransitIterationNo,  m_dTransitNormalizeGap);
        }
        
        of.WriteString(strOut);
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }  
    return true;
}


bool KAssignmentModelDlg::WriteFolderArgument( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"),m_pTarget->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + DLL_HIGHWAYASSIGN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool KAssignmentModelDlg::WriteFolderWarmStart( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		// 2019.04.29 추가 : Base 네트워크 명칭 라인 추가, [프로젝트명]_[시나리오명]_[분석년도명] 형식
		CString strBaseNetwork;	// Base 네트워크 명칭

		KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
		KProject* pProject = (KProject*)pScenario->GetParentObject();

		strBaseNetwork.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
		of.WriteString(strBaseNetwork);


		strFolder.Format(_T("%s"),m_RunningOption.Target()->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + DLL_HIGHWAYASSIGN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);

		strFolder.Format(_T("%s"),m_RunningOption.Target()->GetLocation());
		strFolder = strFolder + _T("\\") +BUSH_PATHVOLUMN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		return false;
	}

	return true;
}



bool KAssignmentModelDlg::WriteBalancing( CStdioFileEx& of )
{
	CString strOut(_T(""));
	try
	{
		strOut.Format(_T("%d\t%E\r\n"), m_RunningOption.m_nIteration, m_RunningOption.m_dMaximumPathRate);
		of.WriteString(strOut);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool KAssignmentModelDlg::WriteModelTitle( CStdioFileEx& of )
{
	CString strOut(_T(""));
	try
	{
		strOut.Format(_T("%s\r\n"), m_TOverWirteAResultInfo.TstrDescription);
		of.WriteString(strOut);
	}
	catch (...)
	{
		return false;
	}

	return true;
}


void KAssignmentModelDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	CString strDllMsg(strMsg);

	QBicListSimpleProgressPtr spProgressWindow = QBicListSimpleProgressThread::SimpleProgress();
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(strDllMsg);
	}
}


void KAssignmentModelDlg::WriteDllLog(CString &a_strContent)
{
	CString strFileName = m_strTargetLocation + _T("\\") + ASSIGNMENT_LOGFILE_NAME;
	UINT nOpenMode = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText; 

	CStdioFileEx of(strFileName, nOpenMode);
	of.SeekToEnd();
	of.WriteString(a_strContent);
	of.WriteString(_T("\r\n"));
	of.Close();
}


void KAssignmentModelDlg::OnBnClickedClose()
{
	TxLogDebug(_T("Close 완료"));
	OnCancel();
}


void KAssignmentModelDlg::KRedrawWindow()
{
    RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


HBRUSH KAssignmentModelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_RADIO3 
		|| ID == IDC_RADIO4 || ID == IDC_RADIO5 || ID == IDC_RADIO6 )
    {        
        pDC->SetBkMode(TRANSPARENT);
        //pDC->SetTextColor(0x00600912);
        return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
    // TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
    return hbr;
}


void KAssignmentModelDlg::OnBnClickedOutputtable()
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pLinkTable = pIOTables->FindTable(TABLE_LINK);
	ASSERT(NULL != pLinkTable);

	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowIOView( true );
	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pLinkTable->Name());
		if(NULL == pTableView)
		{
			pTableView = pIOView->CreateNewTableView(pLinkTable);
			KIOTableController* pController = new KIOTableController(m_pTarget);
			pController->Attach(pLinkTable, pTableView);
		}
		pIOView->SetActiveTableView(pLinkTable->Name());
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KAssignmentModelDlg::OnBnClickedViewLog()
{
	CString     strFileName(_T(""));
	strFileName = ConvertSavePath(ASSIGN_LOGFILE_NAME);

	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(strFileName) )
	{
		CString strMsg(_T("출력된 Log 파일이 존재하지 않습니다."));
		AfxMessageBox(strMsg);
		return;
	}

	ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW);   
}

void KAssignmentModelDlg::DefualtIncremental( KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement )
{
	TxLogDebugStartMsg();
	TiXmlElement* pODDemandElement = a_pXmlManager->AccessElment(_T("OD-Demand"), a_pModelElement);
	TiXmlElement* pIterationElement = a_pXmlManager->AccessElment(_T("Iteration"), pODDemandElement);
	int nIterValue = a_pXmlManager->GetAttributeInt(_T("Value"), pIterationElement);
	m_nIIteration = nIterValue;

	TxLogDebugEndMsg();
}


void KAssignmentModelDlg::DefualtUserFW( KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement )
{
	TxLogDebugStartMsg();
	TiXmlElement* pStoppingElement   = a_pXmlManager->AccessElment(_T("Stopping-Criteria"), a_pModelElement);
	TiXmlElement* pExpressionElement = a_pXmlManager->AccessElment(_T("Expression"), pStoppingElement);
	int nIteration = a_pXmlManager->GetAttributeInt(_T("Iteration"),pExpressionElement);
	double dGap    = a_pXmlManager->GetAttributeDouble(_T("Gap"), pExpressionElement); 

	m_nHighwayIterationNo = nIteration;
	m_dHighwayRelativeGap = dGap;
	TxLogDebugEndMsg();
}


void KAssignmentModelDlg::DefualtUserOB( KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement )
{
	TxLogDebugStartMsg();
	TiXmlElement* pStoppingElement    = a_pXmlManager->AccessElment(_T("Stopping-Criteria"), a_pModelElement);
	TiXmlElement* pExpressionElement  = a_pXmlManager->AccessElment(_T("Expression"), pStoppingElement);
	
	int nIteration       = a_pXmlManager->GetAttributeInt(_T("Iteration"), pExpressionElement);
	int nGap             = a_pXmlManager->GetAttributeInt(_T("Gap"), pExpressionElement);
	int nCentroidThrough = a_pXmlManager->GetAttributeInt(_T("CenteroidThrough"), pExpressionElement);
	int nScaleFactorMode = a_pXmlManager->GetAttributeInt(_T("ScaleFactorMode"), pExpressionElement);
	
	m_nHighwayIterationNo          = nIteration;
	m_nHighwayRelativeGapExp       = nGap;
	m_nUEOBaseZoneCentroidThrought = nCentroidThrough;
	m_nScaleFactorCursor           = nScaleFactorMode;

	if (nScaleFactorMode == 1)
	{
		TiXmlElement* pScaleFacterElement = a_pXmlManager->AccessElment(_T("ScaleFactor"), pExpressionElement);
		double dScaleValue = a_pXmlManager->GetAttributeDouble(_T("Value"), pScaleFacterElement);
		m_dUEOBaseScaleFactor = dScaleValue;
	}
	TxLogDebugEndMsg();
}


void KAssignmentModelDlg::DefualtMultiModal( KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement )
{
	TxLogDebugStartMsg();
	//Transit Assign 
	TiXmlElement* pTransitAssignElement = a_pXmlManager->AccessElment(_T("Transit-Assign"), a_pModelElement);
	TiXmlElement* pMainElement          = a_pXmlManager->AccessElment(_T("MainValue"), pTransitAssignElement);
	TiXmlElement* pUtilityElement       = a_pXmlManager->AccessElment(_T("UtilityFuntion"), pTransitAssignElement);
	TiXmlElement* pSingleRouteElement   = a_pXmlManager->AccessElment(_T("SingleRoute"), pTransitAssignElement);

	int nSingleMultiSelete = a_pXmlManager->GetAttributeInt(_T("Single-Multi"), pMainElement);
	int nNoOfRoute         = a_pXmlManager->GetAttributeInt(_T("NoOfRoute"), pMainElement);
	double dInvehicel      = a_pXmlManager->GetAttributeDouble(_T("InVehicle"),pUtilityElement);
	double dWating         = a_pXmlManager->GetAttributeDouble(_T("Waiting"), pUtilityElement);
	double dTransfer       = a_pXmlManager->GetAttributeDouble(_T("Transfer"), pUtilityElement);
	double dAccessWalk     = a_pXmlManager->GetAttributeDouble(_T("AccessWalk"), pUtilityElement);
	double dEgressWalk     = a_pXmlManager->GetAttributeDouble(_T("EgressWalk"), pUtilityElement);
	double dNoOfTransfer   = a_pXmlManager->GetAttributeDouble(_T("No-Transfer"), pUtilityElement);
	double dCost           = a_pXmlManager->GetAttributeDouble(_T("Cost"),pUtilityElement);
	double dFailBord       = a_pXmlManager->GetAttributeDouble(_T("FailBoard"),pUtilityElement);
	double dSeatOccupancy  = a_pXmlManager->GetAttributeDouble(_T("SeatOccupancy"),pUtilityElement);
	int nZoneConnertors    = a_pXmlManager->GetAttributeInt(_T("ZoneConnectors"), pSingleRouteElement);
	double dWalktoStation  = a_pXmlManager->GetAttributeDouble(_T("WalktoStation"), pSingleRouteElement);
	double dTimeOfBording  = a_pXmlManager->GetAttributeDouble(_T("TimeOfBording"),pSingleRouteElement);

	oKASetTransitAssignOptionsDlg.RadioAssignRule(nSingleMultiSelete);
	oKASetTransitAssignOptionsDlg.NoOfRoute(nNoOfRoute);
	oKASetTransitAssignOptionsDlg.InvehicleTime(dInvehicel);
	oKASetTransitAssignOptionsDlg.WaitingTime(dWating);
	oKASetTransitAssignOptionsDlg.TransferTime(dTransfer);
	oKASetTransitAssignOptionsDlg.AccessWalkTime(dAccessWalk);
	oKASetTransitAssignOptionsDlg.EgressWalkTime(dEgressWalk);
	oKASetTransitAssignOptionsDlg.NoOfTransfers(dNoOfTransfer);
	oKASetTransitAssignOptionsDlg.Cost(dCost);
	oKASetTransitAssignOptionsDlg.FailToBoard(dFailBord);
	oKASetTransitAssignOptionsDlg.SeatOccupancy(dSeatOccupancy);
	oKASetTransitAssignOptionsDlg.MaxNoOfZoneConnectors(nZoneConnertors);
	oKASetTransitAssignOptionsDlg.MaxWalkingTimeToStation(dWalktoStation);
	oKASetTransitAssignOptionsDlg.TimeOfBoarding(dTimeOfBording);

	//Stopping Criteria
	TiXmlElement* pStoppingElement = a_pXmlManager->AccessElment(_T("Stopping-Criteria"), a_pModelElement);
	TiXmlElement* pHighWayElement  = a_pXmlManager->AccessElment(_T("HighWay"), pStoppingElement);
	TiXmlElement* pTransitElement  = a_pXmlManager->AccessElment(_T("Transit"), pStoppingElement);

	int nHighIter    = a_pXmlManager->GetAttributeInt(_T("Iteration"), pHighWayElement);
	double dHighGap  = a_pXmlManager->GetAttributeDouble(_T("Gap"), pHighWayElement);
	int nTransIter   = a_pXmlManager->GetAttributeInt(_T("Iteration"), pTransitElement);
	double dTransGap = a_pXmlManager->GetAttributeDouble(_T("Gap"), pTransitElement);


	m_nHighwayIterationNo  = nHighIter;
	m_dHighwayRelativeGap  = dHighGap;
	m_nTransitIterationNo  = nTransIter;
	m_dTransitNormalizeGap = dTransGap;
	TxLogDebugEndMsg();

}

void KAssignmentModelDlg::DeletePath( CString strPath )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(strPath.Right(1) != _T("\\"))
		strPath += _T("\\");

	strPath += _T("*.*");
	bContinue = finder.FindFile(strPath);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeletePath(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}

	finder.Close();

	strPath = strPath.Left(strPath.ReverseFind('\\'));  
	::RemoveDirectory((LPCTSTR)strPath);

}


void KAssignmentModelDlg::InitTAssingReulstColumn( TAssignResultColumnName& a_oRuslutColumn )
{
	a_oRuslutColumn.AverageTime			= _T("");
	a_oRuslutColumn.VC					= _T("");
	a_oRuslutColumn.SumVolume			= _T("");
	a_oRuslutColumn.Trip				= _T("");
	a_oRuslutColumn.AvgTimeToll			= _T("");
	a_oRuslutColumn.BusInitialVolumn	= _T("");
	a_oRuslutColumn.ModeVolumn.clear();
	a_oRuslutColumn.VDFTime.clear();
	a_oRuslutColumn.VDFTollTime.clear();

	a_oRuslutColumn.ODAvgTime			= _T("");
	a_oRuslutColumn.ODAvgBusTime		= _T("");
	a_oRuslutColumn.ODAvgSubwayTime		= _T("");
	a_oRuslutColumn.ODBusDirect			= _T("");
	a_oRuslutColumn.ODBusOneTransfer	= _T("");
	a_oRuslutColumn.ODBustwoTransfer    = _T("");
	a_oRuslutColumn.ODAVgBusSubwayTime	= _T("");
	a_oRuslutColumn.ODVDFTime.clear();

	a_oRuslutColumn.ODMBus				= _T("");
	a_oRuslutColumn.ODMRail				= _T("");
	a_oRuslutColumn.ODMBusRail			= _T("");

	a_oRuslutColumn.LinfoPassenger		= _T("");

	a_oRuslutColumn.LineBoardNet		= _T("");
	a_oRuslutColumn.LineBoardTransfer	= _T("");
	a_oRuslutColumn.LineBoardTotal		= _T("");
	a_oRuslutColumn.LineAlightNet		= _T("");
	a_oRuslutColumn.LineAlightTransfer	= _T("");
	a_oRuslutColumn.LineAlightTotal		= _T("");
	a_oRuslutColumn.LinePassenger		= _T("");

	a_oRuslutColumn.NodeBusNetBorad			= _T("");
	a_oRuslutColumn.NodeBusTransferBoard	= _T("");
	a_oRuslutColumn.NodeBusSumBoard			= _T("");
	a_oRuslutColumn.NodeBusNetAlight		= _T("");
	a_oRuslutColumn.NodeBusTransferAlight	= _T("");
	a_oRuslutColumn.NodeBusSumAlight		= _T("");

	a_oRuslutColumn.TerminalTrip			= _T("");

}


KIOColumn* KAssignmentModelDlg::CreateResultColumn( CString a_strColumnName, KIOTable* a_pTable , CString a_strDescription)
{
	const KIOColumns* pColumCollection = a_pTable->Columns();

	int nMaxDisplayCount(0);
	int nCount = pColumCollection->ColumnCount();

	if (0 == nCount)
	{
		nMaxDisplayCount = 1;
	}
	else
	{
		for (int i = 0; i < nCount; i++)
		{
			KIOColumn* pColumn = pColumCollection->GetColumn(i);
			int nDisplayCount = pColumn->DisplayOrder();
			if (nMaxDisplayCount < nDisplayCount)
			{
				nMaxDisplayCount = nDisplayCount;
			}
		}
	}

	KIOColumn oColumn;
	oColumn.Name(a_pTable->GetUsableColumnName());
	oColumn.Caption(a_strColumnName);
	oColumn.CodeType(KEMIOCodeTypeIsNothing);
	oColumn.ColumnType(KEMIOColumnTypeUserDefine);
	oColumn.DataType(KEMIODataTypeDouble);
	oColumn.DisplayOrder(nMaxDisplayCount+1);
	oColumn.ColumnInfo(a_strDescription);
	oColumn.PrimaryKey(false);
	oColumn.Visible(true);

	KIOColumn* pColumn = a_pTable->AddColumn(oColumn);
	return pColumn;
}


bool KAssignmentModelDlg::DeleteCreateColumn( KIOColumns& rLinkColumnCollecion, KIOColumns& rParaODColumnCollecion )
{
	int nLinkAddColumnCollections	= rLinkColumnCollecion.ColumnCount();
	int nModeODAddColumnCollections = rParaODColumnCollecion.ColumnCount();

	if( 0 != nLinkAddColumnCollections ||  0 != nModeODAddColumnCollections )
	{
		TxLogDebug(_T("Delete Added Column"));

		KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
		for (int i = 0; i < nLinkAddColumnCollections; ++i)
		{
			pLinkTable->RemoveColumn(rLinkColumnCollecion.GetColumn(i)->Name());
		}

		KIOTable* pModeODTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
		for (int i = 0; i < nModeODAddColumnCollections; ++i)
		{
			pModeODTable->RemoveColumn(rParaODColumnCollecion.GetColumn(i)->Name());
		}

		return false;
	}

	return true;
}


CString KAssignmentModelDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s\\%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_HIGHWAYASSIGN_FOLDER_NAME,a_strFileName);
	return strSavePath;
}


BOOL KAssignmentModelDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);

		if (m_nResultCode == KLAUNCHER_USER_FORCE_STOP)
			theApp.KillKlauncher();
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


void KAssignmentModelDlg::InitializationModelData()
{
	// Input

	m_btnInput.EnableMarkup(TRUE);
	m_btnInput.EnableWindow(TRUE);

	// Run Options
	m_btnRunOptions.EnableMarkup(TRUE);
	m_btnRunOptions.EnableWindow(FALSE);

	// Define of VDF
	m_btnDefineVDF.EnableMarkup(TRUE);
	m_btnDefineVDF.EnableWindow(FALSE);

	// Transit Assign Options
	m_btnTransitAssignOptions.EnableMarkup(TRUE);
	m_btnTransitAssignOptions.EnableWindow(FALSE);

	// OD Demand Share per Iteration
	m_btnODDemandShare.EnableMarkup(TRUE);
	m_btnODDemandShare.EnableWindow(FALSE);

	// Stopping Criteria
	m_btnStoppingCriteria.EnableMarkup(TRUE);
	m_btnStoppingCriteria.EnableWindow(FALSE);

	// Output Design
	m_btnOutputDesign.EnableMarkup(TRUE);
	m_btnOutputDesign.EnableWindow(FALSE);

	// Output Table
	m_btnViewTable.EnableMarkup(TRUE);
	m_btnViewTable.EnableWindow(FALSE);

	// View dll Log
	m_btnViewLog.EnableMarkup(TRUE);
	m_btnViewLog.EnableWindow(FALSE);

	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnInput.SetWindowText(KR_UIXAML_A_INPUT_DEFAULT);
		m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_FALSE);
		m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_FALSE);
		m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_FALSE);
		m_btnODDemandShare.SetWindowText(KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE);
		m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_FALSE);
		m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_FALSE);
		m_btnViewTable.SetWindowText(KR_UIXAML_A_VIEWTABLE_FALSE);
		m_btnViewLog.SetWindowText(KR_UIXAML_A_VIEWLOG_FALSE);
	}
	else {
		m_btnInput.SetWindowText(UIXAML_A_INPUT_DEFAULT);
		m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_FALSE);
		m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_FALSE);
		m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_FALSE);
		m_btnODDemandShare.SetWindowText(UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE);
		m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_FALSE);
		m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_FALSE);
		m_btnViewTable.SetWindowText(UIXAML_A_VIEWTABLE_FALSE);
		m_btnViewLog.SetWindowText(UIXAML_A_VIEWLOG_FALSE);
	}

	KRedrawWindow();

	if (KmzSystem::IsFreightSystem())
	{
		InitInputInfoList();
	}
	else
	{
		//여객의 경우 Trip Matrix 에서 설정한 TimeDuration, Occupancy, Pce, Preload Check값들은 Run 이후에도 유지하도록 함
		std::list<KAInputInfo*>::iterator bIter = m_InputList.begin();
	
		while (bIter != m_InputList.end())
		{
			KAInputInfo* pInfo = *bIter;
			pInfo->Selected(false);

			++bIter;
		}
	}	

	oKABPRFnDlg.InitButtonContol();
	m_OBVDFDlg.InitButtonControl();
	
	m_nIIteration					= 1;
	//m_dAnalysisTime					= 10.0;
	m_dValueOfTime					= 0.0;
	m_nHighwayIterationNo			= 100;
	m_dHighwayRelativeGap			= 0.001;
	m_nHighwayRelativeGapExp		= 12;
	m_nTransitIterationNo			= 100;
	m_dTransitNormalizeGap			= 0.1;
	m_dUEOBaseScaleFactor			= 0.0;
	m_nUEOBaseZoneCentroidThrought  = 0;
	m_nScaleFactorCursor			= 0;
	m_nBtnRadioIndex				= 0;
	m_bUseOverWrite					= false;
}


bool KAssignmentModelDlg::FindLogFile()
{
	CFileFind   cFileFinder;
	CString strFileName = ConvertSavePath(ASSIGN_LOGFILE_NAME);
	if(FALSE == cFileFinder.FindFile(strFileName) )
	{
		return false;
	}

	return true;
}

void KAssignmentModelDlg::InitModelButton()
{
	try
	{
		// Input
		m_btnInput.EnableMarkup(TRUE);
		m_btnInput.EnableWindow(TRUE);

		// Run Options
		m_btnRunOptions.EnableMarkup(TRUE);
		m_btnRunOptions.EnableWindow(FALSE);

		// Define of VDF
		m_btnDefineVDF.EnableMarkup(TRUE);
		m_btnDefineVDF.EnableWindow(FALSE);

		// Transit Assign Options
		m_btnTransitAssignOptions.EnableMarkup(TRUE);
		m_btnTransitAssignOptions.EnableWindow(FALSE);

		// OD Demand Share per Iteration
		m_btnODDemandShare.EnableMarkup(TRUE);
		m_btnODDemandShare.EnableWindow(FALSE);

		// Stopping Criteria
		m_btnStoppingCriteria.EnableMarkup(TRUE);
		m_btnStoppingCriteria.EnableWindow(FALSE);

		// Output Design
		m_btnOutputDesign.EnableMarkup(TRUE);
		m_btnOutputDesign.EnableWindow(FALSE);

		// Output Table
		m_btnViewTable.EnableMarkup(TRUE);
		m_btnViewTable.EnableWindow(FALSE);

		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnInput.SetWindowText(KR_UIXAML_A_INPUT_DEFAULT);
			m_btnRunOptions.SetWindowText(KR_UIXAML_A_RUN_OPTIONS_FALSE);
			m_btnDefineVDF.SetWindowText(KR_UIXAML_A_DEFINE_VDF_FALSE);
			m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_FALSE);
			m_btnODDemandShare.SetWindowText(KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE);
			m_btnStoppingCriteria.SetWindowText(KR_UIXAML_A_STOPPING_CRITERIA_FALSE);
			m_btnOutputDesign.SetWindowText(KR_UIXAML_A_OUTPUT_FALSE);
			m_btnViewTable.SetWindowText(KR_UIXAML_A_VIEWTABLE_FALSE);
		}
		else {
			m_btnInput.SetWindowText(UIXAML_A_INPUT_DEFAULT);
			m_btnRunOptions.SetWindowText(UIXAML_A_RUN_OPTIONS_FALSE);
			m_btnDefineVDF.SetWindowText(UIXAML_A_DEFINE_VDF_FALSE);
			m_btnTransitAssignOptions.SetWindowText(UIXAML_A_TRANSIT_ASSIGN_OPTIONS_FALSE);
			m_btnODDemandShare.SetWindowText(UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE);
			m_btnStoppingCriteria.SetWindowText(UIXAML_A_STOPPING_CRITERIA_FALSE);
			m_btnOutputDesign.SetWindowText(UIXAML_A_OUTPUT_FALSE);
			m_btnViewTable.SetWindowText(UIXAML_A_VIEWTABLE_FALSE);
		}


		// View dll Log
		if (false == FindLogFile())
		{
			m_btnViewLog.EnableMarkup(TRUE);
			m_btnViewLog.EnableWindow(FALSE);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnViewLog.SetWindowText(KR_UIXAML_A_VIEWLOG_FALSE);
			}
			else {
				m_btnViewLog.SetWindowText(UIXAML_A_VIEWLOG_FALSE);
			}
		}
		else
		{
			m_btnViewLog.EnableMarkup(TRUE);
			m_btnViewLog.EnableWindow(TRUE);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnViewLog.SetWindowText(KR_UIXAML_A_VIEWLOG_DEFAULT);
			}
			else {
				m_btnViewLog.SetWindowText(UIXAML_A_VIEWLOG_DEFAULT);
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

void KAssignmentModelDlg::OnBnClickedBtnImport()
{
	try
	{
		KAImportDBDlg oDlg(m_pTarget);
		if (oDlg.DoModal() == IDOK)
		{
			InitInputInfoList();
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KAssignmentModelDlg::GetUseModeName( std::vector<CString>& a_vecModeName )
{
	try
	{
		AutoType iter = m_InputList.begin();
		AutoType end  = m_InputList.end();
		while(iter != end)
		{
			KAInputInfo* oInfo = *iter;
			if (oInfo->Selected() == true)
			{
				a_vecModeName.push_back(oInfo->Mode()->ModeName());
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

bool KAssignmentModelDlg::DeleteBushFile()
{
	try
	{
		CString strTargetLocation = m_pTarget->GetLocation();
		CString strTempLocation = strTargetLocation + _T("\\") + BUSH_PATHVOLUMN_FOLDER_NAME;

		DeletePath(strTempLocation);

		KDBaseAssignment::DeleteAssignPathInfo(m_pTarget->GetDBaseConnection());
		KDBaseAssignment::DeleteAssignTurnVolumnInfo(m_pTarget->GetDBaseConnection());

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

bool KAssignmentModelDlg::CheckResultState()
{
	try
	{
		
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

void KAssignmentModelDlg::EnableMultiAssign( void )
{
    GetDlgItem(IDC_BTN_MULTI)->EnableWindow(FALSE);

	if (!KmzSystem::IsPassengerSystem())
		return;

	//if (m_RunningOption.IncludeWarmStart())
	//	return;

	if (m_emAModel == KEMAssignmentModelSegment)
		return;
	
	GetDlgItem(IDC_BTN_MULTI)->EnableWindow(TRUE);
}




