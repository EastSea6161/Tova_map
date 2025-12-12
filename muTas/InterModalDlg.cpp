
// InterModalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "InterModalDlg.h"
#include "afxdialogex.h"
#include "KUIDefInterModal.h"

#include "InterBaseDlg.h"
#include "InterInputDlg.h"
#include "InterDefineCostDlg.h"
#include "InterDefineVDFDlg.h"

#include "PurposeManager.h"
#include "Target.h"
#include "InterInputInfo.h"
#include "InterModeInputDlg.h"

#include "PurposeInfoCollection.h"
#include "Purpose.h"

#include <sys/stat.h>
#include <fstream>
#include <iostream>

#include "InterDefineCostPrivateSector.h"
#include "InterDefineCostPublicSector.h"
#include "InterDefineCostPrivatePage.h"
#include "InterDefineCostPublicPage.h"

#include "IncludeIOTable.h"
#include "CodeManager.h"
#include "CodeGroup.h"

#include "InterFunctionBase.h"
#include "InterFunctionBPR.h"
#include "InterFunctionUser.h"
#include "InterTransitAssignDlg.h"

#include "OldInterStoppingCriteriaDlg.h"
#include "InterStoppingCriteriaDlg.h"
#include "InterChoiceParamPage.h"
#include "InterChoiceParamBase.h"
#include "InterChoiceParam.h"

#include "InterChoiceParamDlg.h"

#include "InterProgressDlg.h"
#include "KBulkDBase2File.h"

#include "ModeManager.h"
#include "Mode.h"

#include "Project.h"

#include "InterOutputDlg.h"
#include "DBaseConnection.h"
#include "KxBulkDbase.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"

#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOView.h"
#include "IOTableController.h"

#include "InterOutputDesignDlg.h"

//KGSimpleRegressionPage
using namespace UIInterModal;

//KInterProgressDlg* KInterModalDlg::m_pProgressDlg = NULL;

// CInterModalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterModalDlg, CDialog)

KInterModalDlg::KInterModalDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	:	CDialog(KInterModalDlg::IDD, pParent),
	m_emInterModel(KEMInterModalModelDefault),
	m_pTarget(a_pTarget),
	m_pRecordArray(NULL)
{
	m_pVDF = NULL;

	m_nHighwayIterationNo    = 100;
	m_dHighwayRelativeGap    = 1e-012/*1/1000000000000*/;
	m_nHighwayRelativeGapExp = 12;
	m_nTransitIterationNo    = 100;
	m_dTransitNormalizeGap   = 0.1;
	m_dUEOBaseScaleFactor    = 0.5;
	m_nScaleFactorCursor	 = 1;
	
	m_dAnalysisTime 	= 10.0;
	m_nParkFee          = 1000;

	m_dTaxiWaitingTime  = 10.0;
	m_dTaxiStoppingTime = 0.1;

	m_nNoOfRoute               = 5;
	m_nMaxNoOfZoneConnectors   = 5;
	m_dMaxWalkingTimeToStation = 20.0;
	m_dTimeOfBoarding          = 2.0;

	m_nIntermodalIteration = 20;
	m_dIntermodalL2Norm    = 1.0;

	m_TOverWirteAResultInfo.TbUseData = false;
}

KInterModalDlg::~KInterModalDlg()
{
	KLogDebugStartMsg();
	ClearInterInputInfoList();
	ClearInterDefineCostPrivateSectorVector();
	ClearInterDefineCostPublicSectorVector();
	ClearInterChoiceParameterVector();

	if(NULL != m_pVDF)
	{
		delete m_pVDF;
	}
	ClearModeInputInfoList();

	if (NULL != m_pRecordArray)
		delete m_pRecordArray;

	m_pRecordArray = NULL;

	KLogDebugEndMsg();
}

void KInterModalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT, m_btnPurposeInput);
	DDX_Control(pDX, IDC_BUTTON1, m_btnModeInput);
	DDX_Control(pDX, IDC_DEFINECOST, m_btnDefineCost);
	DDX_Control(pDX, IDC_DEFINEVDF, m_btnDefineVDF);
	DDX_Control(pDX, IDC_TRANSITASSIGN, m_btnTransitAssign);
	DDX_Control(pDX, IDC_CHOICEPARA, m_btnChoiceParameter);
	DDX_Control(pDX, IDC_STOPCRITERIA, m_btnStoppingCriteria);
	DDX_Control(pDX, IDC_VIEWTABLE, m_btnViewTable);
	DDX_Control(pDX, IDC_OUTDESIGN, m_btnOutputDesign);
	//DDX_Control(pDX, IDC_MODEL, m_btnInterModal);
	DDX_Control(pDX, IDC_STATIC_MODEL, m_lblInterModal);
	DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW5, m_uiArrow4);
	DDX_Control(pDX, IDC_STATIC_ARROW6, m_uiArrow5);
	DDX_Control(pDX, IDC_STATIC_ARROW7, m_uiArrow6);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow7);
}


BEGIN_MESSAGE_MAP(KInterModalDlg, CDialog)
	ON_BN_CLICKED(IDC_INPUT, &KInterModalDlg::OnBnClickedPurposeInput)
	ON_BN_CLICKED(IDC_DEFINECOST, &KInterModalDlg::OnBnClickedDefineCost)
	ON_BN_CLICKED(IDC_DEFINEVDF, &KInterModalDlg::OnBnClickedDefineVDF)
	ON_BN_CLICKED(IDC_TRANSITASSIGN, &KInterModalDlg::OnBnClickedTransitassign)
	ON_BN_CLICKED(IDC_CHOICEPARA, &KInterModalDlg::OnBnClickedChoiceParam)
	ON_BN_CLICKED(IDC_STOPCRITERIA, &KInterModalDlg::OnBnClickedStopcriteria)
	ON_BN_CLICKED(IDC_VIEWTABLE, &KInterModalDlg::OnBnClickedViewTable)
	ON_BN_CLICKED(IDC_OUTDESIGN, &KInterModalDlg::OnBnClickedOutdesign)
	ON_BN_CLICKED(IDC_RUN, &KInterModalDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_CLOSE, &KInterModalDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON1, &KInterModalDlg::OnBnClickedModeInput)
	ON_BN_CLICKED(IDC_BUTTON2, &KInterModalDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CInterModalDlg 메시지 처리기입니다.


void KInterModalDlg::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
	RunnableCheck();
}

void KInterModalDlg::ClearModeInputInfoList( void )
{
	std::list<KInterModeInputInfo*>::iterator itInput,itEnd = m_modeInputList.end();
	for(itInput = m_modeInputList.begin(); itInput != itEnd; ++itInput)
	{
		KInterModeInputInfo* pInput = *itInput;
		delete pInput;
	}
	m_modeInputList.clear();
}

void KInterModalDlg::ClearInterChoiceParameterVector(void)
{
	int nCountChoiceParameterVector = m_vecInterChoiceParam.size();
	for(int i = 0; i < nCountChoiceParameterVector; i++)
	{
		KInterChoiceParamBase* pInterChoiceParamBase = m_vecInterChoiceParam[i];
		delete pInterChoiceParamBase;
	}

	m_vecInterChoiceParam.clear();
}

void KInterModalDlg::ClearInterInputInfoList(void)
{
	std::list<KInterInputInfo*>::iterator iter, end = m_lstInterInputInfo.end();
	for(iter = m_lstInterInputInfo.begin(); iter != end; ++iter)
	{
		KInterInputInfo* pInterInputInfo = *iter;
		delete pInterInputInfo;
	}

	m_lstInterInputInfo.clear();
}

void KInterModalDlg::ClearInterDefineCostPrivateSectorVector()
{
	int nCountPrivateSectorVector = m_vecInterDefineCostPrivateSector.size();
	for (int i = 0; i < nCountPrivateSectorVector; ++i)
	{
		KInterDefineCostPrivateSector* pInterDefineCostPrivateSector = m_vecInterDefineCostPrivateSector[i];
		delete pInterDefineCostPrivateSector;
	}
	m_vecInterDefineCostPrivateSector.clear();
}

void KInterModalDlg::ClearInterDefineCostPublicSectorVector(void)
{
	int nCountPublicSectorVector = m_vecInterDefineCostPublicSector.size();
	for (int i = 0; i < nCountPublicSectorVector; ++i)
	{
		KInterDefineCostPublicSector* pInterDefineCostPublicSector = m_vecInterDefineCostPublicSector[i];
		delete pInterDefineCostPublicSector;
	}
	m_vecInterDefineCostPublicSector.clear();
}

BOOL KInterModalDlg::OnInitDialog()
{
	KLogDebugStartMsg();
	CDialog::OnInitDialog();

	//출근 1
	std::vector<double> vecDefault;
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0429);
	vecDefault.push_back(-0.056);
	vecDefault.push_back(-0.0421);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0332);
	vecDefault.push_back(-0.0003);
	vecDefault.push_back(4.8404);
	vecDefault.push_back(2.9916);
	vecDefault.push_back(4.8599);
	vecDefault.push_back(5.5813);
	vecDefault.push_back(4.4551);
	vecDefault.push_back(-3.3919);
	vecDefault.push_back(1.111);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(1, vecDefault));
	//등교 2
	vecDefault.clear();

	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0004);
	vecDefault.push_back(4.5177);
	vecDefault.push_back(0.0);
	vecDefault.push_back(5.9961);
	vecDefault.push_back(5.7306);
	vecDefault.push_back(4.4049);
	vecDefault.push_back(0.0);
	vecDefault.push_back(1.486);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(2, vecDefault));
	//학원 3
	vecDefault.clear();

	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0007);
	vecDefault.push_back(7.4764);
	vecDefault.push_back(6.5199);
	vecDefault.push_back(8.5259);
	vecDefault.push_back(8.2434);
	vecDefault.push_back(6.0899);
	vecDefault.push_back(0.0);
	vecDefault.push_back(2.7871);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(3, vecDefault));
	//쇼핑 4
	vecDefault.clear();

	vecDefault.push_back(-3.2469);
	vecDefault.push_back(-0.0038);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-18.7207);
	vecDefault.push_back(-0.0029);
	vecDefault.push_back(-1.1435);
	vecDefault.push_back(-2.2505);
	vecDefault.push_back(-2.6007);
	vecDefault.push_back(0.0);
	vecDefault.push_back(2.4372);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-30.8841);
	vecDefault.push_back(-116.5978);
	vecDefault.push_back(8.0208);
	vecDefault.push_back(4.0367);
	vecDefault.push_back(-149.6566);
	vecDefault.push_back(-196.602);
	vecDefault.push_back(-213.7905);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(4, vecDefault));
	//귀가 5 
	vecDefault.clear();

	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0338);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0974);
	vecDefault.push_back(-0.5986);
	vecDefault.push_back(6.9839);
	vecDefault.push_back(4.412);
	vecDefault.push_back(6.8902);
	vecDefault.push_back(7.4335);
	vecDefault.push_back(5.8088);
	vecDefault.push_back(0.0);
	vecDefault.push_back(2.3141);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(5, vecDefault));
	//기타 6
	vecDefault.clear();

	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0126);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0393);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.042);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.098);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-32.9386);
	vecDefault.push_back(-35.0163);
	vecDefault.push_back(-33.7014);
	vecDefault.push_back(-33.6302);
	vecDefault.push_back(-54.0198);
	vecDefault.push_back(-165.5137);
	vecDefault.push_back(-124.92);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(6, vecDefault));
	//업무 7
	vecDefault.clear();

	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(0.0);
	vecDefault.push_back(-0.0001);
	vecDefault.push_back(6.6528);
	vecDefault.push_back(4.9512);
	vecDefault.push_back(6.3702);
	vecDefault.push_back(6.8377);
	vecDefault.push_back(3.4434);
	vecDefault.push_back(0.0);
	vecDefault.push_back(1.6754);
	vecDefault.push_back(0.0);
	m_mapDefault.insert(std::make_pair(7, vecDefault));


	InitInputInfoList();
	InitParameterColumn();

	InitModeInputInfoList();
	
	InitDefineCostList();
	InitChoiceParamBase();

	m_uiBackImage.MoveWindow(4, 5, 600, 320);    
	m_uiBackImage.SetMarkupText(UIXAML_D_BACKIMAGE);

	UINT nID_DISTRIBUTION_INPUT  = 90913;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_INPUT,  &nID_DISTRIBUTION_INPUT, 1, 0); 
	UINT nID_DISTRIBUTION_MODEL  = 90915;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL,  &nID_DISTRIBUTION_MODEL, 1, 0); 
	UINT nID_DISTRIBUTION_OUTPUT = 90917;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_OUTPUT, &nID_DISTRIBUTION_OUTPUT, 1, 0); 

	m_lblInterModal.SetMarkupText(UIXAML_D_MODEL_DEFAULT);

	/*Step.1 InterModal Input*/
	m_btnPurposeInput.EnableMarkup(TRUE);
	m_btnPurposeInput.SetWindowText(UIXAML_D_PURPOSE_INPUT_DEFAULT);

	m_btnModeInput.EnableMarkup(TRUE);
	m_btnModeInput.SetWindowText(UIXAML_D_MODE_INPUT_FALSE);
	m_btnModeInput.EnableWindow(FALSE);

	/*Step2. InterModal Def Cost*/
	m_btnDefineCost.EnableMarkup(TRUE);
	m_btnDefineCost.SetWindowText(UIXAML_D_DEFINECOST_FALSE);
	m_btnDefineCost.EnableWindow(FALSE);

	/*Step3. InterModal Def VDF*/
	m_btnDefineVDF.EnableMarkup(TRUE);
	m_btnDefineVDF.SetWindowText(UIXAML_D_DEFINEVDF_FALSE);
	m_btnDefineVDF.EnableWindow(FALSE);

	/*Step4. InterModal Transit Assign Options*/
	m_btnTransitAssign.EnableMarkup(FALSE);
	m_btnTransitAssign.SetWindowText(UIXAML_D_TRANASSIGN_FALSE);
	m_btnTransitAssign.EnableWindow(FALSE);
	m_btnTransitAssign.ShowWindow(SW_HIDE);

	/*Step5. Mode/Route choice Param*/
	m_btnChoiceParameter.EnableMarkup(TRUE);
	m_btnChoiceParameter.SetWindowText(UIXAML_D_CHOICEPARAM_FALSE);
	m_btnChoiceParameter.EnableWindow(FALSE);

	/*Step6. Stopping Criteria*/
	m_btnStoppingCriteria.EnableMarkup(TRUE);
	m_btnStoppingCriteria.SetWindowText(UIXAML_D_STOPCRITERIA_FALSE);
	m_btnStoppingCriteria.EnableWindow(FALSE);


	//View Result Table
	m_btnViewTable.EnableMarkup(TRUE);
	m_btnViewTable.SetWindowText(UIXAML_D_VTABLE_FALSE);
	m_btnViewTable.EnableWindow(FALSE);
	
	m_btnOutputDesign.EnableMarkup(TRUE);
	m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_SELECTED);
	m_btnOutputDesign.EnableWindow(TRUE);

	m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB); 
	m_uiArrow2.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB); 
	m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
	m_uiArrow4.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
	m_uiArrow5.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT);
	m_uiArrow6.SetPosition(ZEMArrowPositionRB, ZEMArrowPositionLT); 
	m_uiArrow7.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 

	RunnableCheck();

	KLogDebugEndMsg();
	SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1);
	KRedrawWindow();
	return TRUE; 

}

void KInterModalDlg::InitModeInputInfoList(void)
{
	KModeManager* pModeManager = m_pTarget->ModeManager();
	int nModeCount = pModeManager->GetModeCount();
	for(int i = 0; i < nModeCount; ++i)
	{
		KMode* pMode = pModeManager->GetMode(i);

		KInterModeInputInfo* pModeInputInfo = new KInterModeInputInfo;
		pModeInputInfo->Mode(pMode);
		m_modeInputList.push_back(pModeInputInfo);
	}
}

void KInterModalDlg::InitInputInfoList(void)
{

	ClearInterInputInfoList();

	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter = purposeInfos.begin();
	std::list<KPurpose*>::iterator end  = purposeInfos.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;

		KInterInputInfo* pInterInputInfo = new KInterInputInfo();

		KPurpose* pNewPurpose = new KPurpose(
			pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName());

		pInterInputInfo->SetPurpose(pNewPurpose);

		m_lstInterInputInfo.push_back(pInterInputInfo);

		++iter;
	}

	//Init Intermodal Mode;
	TIntermodalMode oIntermodalMode;

	oIntermodalMode.strModeName  = _T("Auto");
	oIntermodalMode.nNetworkType = 1;
	oIntermodalMode.dPCU 		 = 1.0;
	oIntermodalMode.dOccupancy 	 = 1.2;

	m_vecIntermodalModeInfo.push_back(oIntermodalMode);

	oIntermodalMode.strModeName  = _T("Taxi");
	oIntermodalMode.nNetworkType = 1;
	oIntermodalMode.dPCU 		 = 1.0;
	oIntermodalMode.dOccupancy 	 = 1.4;

	m_vecIntermodalModeInfo.push_back(oIntermodalMode);

	oIntermodalMode.strModeName  = _T("Bus");
	oIntermodalMode.nNetworkType = 2;
	oIntermodalMode.dPCU 		 = 2.0;
	oIntermodalMode.dOccupancy 	 = 14.0;

	m_vecIntermodalModeInfo.push_back(oIntermodalMode);

	oIntermodalMode.strModeName  = _T("Subway");
	oIntermodalMode.nNetworkType = 3;
	oIntermodalMode.dPCU 		 = 0.0;
	oIntermodalMode.dOccupancy 	 = 0.0;

	m_vecIntermodalModeInfo.push_back(oIntermodalMode);
}

void KInterModalDlg::InitDefineCostList(void)
{
	ClearInterDefineCostPrivateSectorVector();
	ClearInterDefineCostPublicSectorVector();

	/*DB select해서 Class에 값 셋팅(default) 해서... vector로 담기... */
	std::vector<CString> tempPrivateData;
	tempPrivateData.push_back(_T("Gasoline"));
	tempPrivateData.push_back(_T("Diesel"));
	tempPrivateData.push_back(_T("LPG"));
	/*DB로 대체*/

	std::vector<double> tempPrice;
	tempPrice.push_back(2000.0);
	tempPrice.push_back(1700.0);
	tempPrice.push_back(1200.0);
	
	std::vector<double> tempRatio;
	tempRatio.push_back(0.5);
	tempRatio.push_back(0.35);
	tempRatio.push_back(0.15);

	std::vector<double> tempFuelEfficiency;
	tempFuelEfficiency.push_back(9.0);
	tempFuelEfficiency.push_back(14.0);
	tempFuelEfficiency.push_back(7.0);

	/*initialize Define Cost Private Sector*/
	int nCountPrivateData = tempPrivateData.size();
	for(int i = 0; i < nCountPrivateData; ++i)
	{
		KInterDefineCostPrivateSector* 	pInterDefineCostPrivateSector = new KInterDefineCostPrivateSector( i+1, tempPrivateData[i]);
		pInterDefineCostPrivateSector->SetPrice(tempPrice[i]);
		pInterDefineCostPrivateSector->SetRatio(tempRatio[i]);
		pInterDefineCostPrivateSector->SetFuelEfficiency(tempFuelEfficiency[i]);

		m_vecInterDefineCostPrivateSector.push_back(pInterDefineCostPrivateSector);
	}

	std::vector<CString> tempPublicData;
	tempPublicData.push_back(_T("Taxi"));
	tempPublicData.push_back(_T("Bus"));
	tempPublicData.push_back(_T("Rail"));

	std::vector<double> tempPublicPrice;
	tempPublicPrice.push_back(2300.0);
	tempPublicPrice.push_back(900.0);
	tempPublicPrice.push_back(900.0);
	std::vector<double> tempPublicDistance;
	tempPublicDistance.push_back(2.0);
	tempPublicDistance.push_back(10.0);
	tempPublicDistance.push_back(12.0);
	std::vector<double> tempPublicAddDistance;
	tempPublicAddDistance.push_back(0.15);
	tempPublicAddDistance.push_back(5.0);
	tempPublicAddDistance.push_back(6.0);
	std::vector<double> tempPublicAddCharge;
	tempPublicAddCharge.push_back(0.0);
	tempPublicAddCharge.push_back(100.0);
	tempPublicAddCharge.push_back(100.0);

	int nCountPublicData = tempPublicData.size();
	for (int i = 0; i < nCountPublicData; ++i)
	{
		KInterDefineCostPublicSector* pInterDefineCostPublicSector = new KInterDefineCostPublicSector(i+1, tempPublicData[i]);

		pInterDefineCostPublicSector->SetBaseCharge(tempPublicPrice[i]);
		pInterDefineCostPublicSector->SetBaseDistance(tempPublicDistance[i]);
		pInterDefineCostPublicSector->SetAddDistance(tempPublicAddDistance[i]);
		pInterDefineCostPublicSector->SetAddCharge(tempPublicAddCharge[i]);

		m_vecInterDefineCostPublicSector.push_back(pInterDefineCostPublicSector);
	}
}

void KInterModalDlg::InitChoiceParamBase(void)
{
	ClearInterChoiceParameterVector();

	std::vector<CString> vecTemp;
	vecTemp.push_back(_T("Travel Time"));
	vecTemp.push_back(_T("Travel Count"));
	vecTemp.push_back(_T("Wait Time"));
	vecTemp.push_back(_T("Transfer Time"));
	vecTemp.push_back(_T("Access Time"));
	vecTemp.push_back(_T("Egress Time"));
	vecTemp.push_back(_T("Dummy"));

	std::list<KInterInputInfo*>::iterator itInput, itEnd = m_lstInterInputInfo.end();
	for(itInput = m_lstInterInputInfo.begin();itInput != itEnd; itInput++)
	{
		KInterInputInfo* pInputInfo = *itInput;

		LPCTSTR strPurposeName = pInputInfo->GetPurpose()->PurposeName();

		KInterChoiceParamBase* pBase = new KInterChoiceParamBase();

		pBase->Caption(strPurposeName);

		std::vector<KInterChoiceParam*> vecInterChoiceParam = pBase->GetInterChoiceParam();

		int nCntVecTemp = vecTemp.size();

		for (int j = 0; j < nCntVecTemp; j++)
		{
			KInterChoiceParam* pChoiceParam = new KInterChoiceParam(vecTemp[j]);

			vecInterChoiceParam.push_back(pChoiceParam);
		}

		pBase->SetInterChoiceParam(vecInterChoiceParam);

		m_vecInterChoiceParam.push_back(pBase);
	}
}

/*Step.1 */
void KInterModalDlg::OnBnClickedPurposeInput()
{
	KInterInputDlg dlg(m_pTarget, m_lstInterInputInfo);

	if(dlg.DoModal() == IDOK)
	{

		int nIndex(0);

		std::list<KInterInputInfo*>::iterator itInput, itEnd = m_lstInterInputInfo.end();
		for(itInput = m_lstInterInputInfo.begin();itInput != itEnd; itInput++, ++nIndex)
		{
			KInterInputInfo* pInputInfo = *itInput;

			KInterChoiceParamBase* pBase = m_vecInterChoiceParam[nIndex];

			if(pInputInfo->GetSelected())
			{
				pBase->Selected(true);
			}
			else
			{
				pBase->Selected(false);
				pBase->InitChoiceParamDataVector();
			}
		}

		m_btnPurposeInput.SetWindowText(UIXAML_D_PURPOSE_INPUT_SELECTED);

		m_btnModeInput.SetWindowText(UIXAML_D_MODE_INPUT_DEFAULT);
		m_btnModeInput.EnableWindow(TRUE);

		InitParameterValue();
	}

	KRedrawWindow();
}

void KInterModalDlg::OnBnClickedModeInput()
{
	KInterModeInputDlg oDlg(m_pTarget);

	oDlg.IntermodalModeInfo(m_vecIntermodalModeInfo);
	oDlg.ModeInputList(m_modeInputList);
	oDlg.AnalysisTime(m_dAnalysisTime);

	if (IDOK == oDlg.DoModal())
	{
		m_vecIntermodalModeInfo = oDlg.IntermodalModeInfo();
		m_dAnalysisTime 		= oDlg.AnalysisTime();

		m_btnModeInput.SetWindowText(UIXAML_D_MODE_INPUT_SELECTED);

		m_btnDefineCost.SetWindowText(UIXAML_D_DEFINECOST_DEFAULT);
		m_btnDefineCost.EnableWindow(TRUE);
	}
	KRedrawWindow();
}
/*Step. 2*/
void KInterModalDlg::OnBnClickedDefineCost()
{
	CXTPPropertySheet ps(_T("Define Cost"));

	ps.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CXTPPropertyPageListNavigator* pListNavi = new CXTPPropertyPageListNavigator();
	pListNavi->SetListStyle(xtpListBoxStandard);
	ps.SetNavigator( pListNavi );

	KInterDefineCostPrivatePage pPrivatePage(m_vecInterDefineCostPrivateSector);
	pPrivatePage.ParkFee(m_nParkFee);
	pPrivatePage.SetCaption(_T("Private Sector"));
	ps.AddPage(&pPrivatePage);

	KInterDefineCostPublicPage pPublicPage(m_vecInterDefineCostPublicSector);
	pPublicPage.MaxNoOfZoneConnectors(m_nMaxNoOfZoneConnectors);
	pPublicPage.MaxWalkingTimeToStation(m_dMaxWalkingTimeToStation);
	pPublicPage.TimeOfBoarding(m_dTimeOfBoarding);
	pPublicPage.NoOfRoute(m_nNoOfRoute);
	pPublicPage.TaxiStoppingTime(m_dTaxiStoppingTime);
	pPublicPage.TaxiWaitingTime(m_dTaxiWaitingTime);
	pPublicPage.SetCaption(_T("Public Sector"));
	ps.AddPage(&pPublicPage);

	ps.SetPageSize(CSize(588, 0)); // in dialog units

	if(ps.DoModal() == IDOK){
		pPrivatePage.GetRecords();
		m_nParkFee = pPrivatePage.ParkFee();

		pPublicPage.GetRecords();
		m_nMaxNoOfZoneConnectors   = pPublicPage.MaxNoOfZoneConnectors();
		m_dMaxWalkingTimeToStation = pPublicPage.MaxWalkingTimeToStation();
		m_dTimeOfBoarding          = pPublicPage.TimeOfBoarding();
		m_nNoOfRoute               = pPublicPage.NoOfRoute();
		m_dTaxiWaitingTime         = pPublicPage.TaxiWaitingTime();
		m_dTaxiStoppingTime        = pPublicPage.TaxiStoppingTime();

		m_btnDefineCost.SetWindowText(UIXAML_D_DEFINECOST_SELECTED);

		m_btnDefineVDF.SetWindowText(UIXAML_D_DEFINEVDF_DEFAULT);
		m_btnDefineVDF.EnableWindow(TRUE);
	}
	KRedrawWindow();
}

/*Step. 3*/
void KInterModalDlg::OnBnClickedDefineVDF()
{
	m_oKInterBPRFnDlg.Target(m_pTarget);

	if (IDOK == m_oKInterBPRFnDlg.DoModal())
	{
		m_btnDefineVDF.SetWindowText(UIXAML_D_DEFINEVDF_SELECTED);

		m_btnChoiceParameter.SetWindowText(UIXAML_D_CHOICEPARAM_DEFAULT);
		m_btnChoiceParameter.EnableWindow(TRUE);
	}

	KRedrawWindow();
}

/*Step. 4*/
void KInterModalDlg::OnBnClickedTransitassign()
{
	if(oTransitAssignDlg.DoModal() == IDOK)
	{
		m_btnTransitAssign.SetWindowText(UIXAML_D_TRANASSIGN_SELECTED);

		m_btnChoiceParameter.SetWindowText(UIXAML_D_CHOICEPARAM_DEFAULT);
		m_btnChoiceParameter.EnableWindow(TRUE);
	}

	KRedrawWindow();
}

/*Step. 5*/
void KInterModalDlg::OnBnClickedChoiceParam()
{
	KInterChoiceParamDlg oDlg;

	oDlg.PurposeInputInfo(m_lstInterInputInfo);
	oDlg.ParameterColumn(m_vecParameterColumn);
	oDlg.ParameterValue(m_pRecordArray);

	if (IDOK == oDlg.DoModal())
	{
		m_btnChoiceParameter.SetWindowText(UIXAML_D_CHOICEPARAM_SELECTED);

		m_btnStoppingCriteria.SetWindowText(UIXAML_D_STOPCRITERIA_DEFAULT);
		m_btnStoppingCriteria.EnableWindow(TRUE);
	}
	KRedrawWindow();
}

/*Step. 6*/
void KInterModalDlg::OnBnClickedStopcriteria()
{

	KInterStoppingCriteriaDlg oDlg;
	oDlg.NumberOfIteration(m_nHighwayIterationNo);
	oDlg.RelativeGap(m_dHighwayRelativeGap);
	oDlg.RelativeGapExp(m_nHighwayRelativeGapExp);
	oDlg.ScaleFactor(m_dUEOBaseScaleFactor);
	oDlg.ScaleFactorCursor(m_nScaleFactorCursor);
	oDlg.IntermodalIteration(m_nIntermodalIteration);
	oDlg.IntermodalL2Norm(m_dIntermodalL2Norm);
	
	if (oDlg.DoModal() == IDOK)
	{
		m_nHighwayIterationNo			= oDlg.NumberOfIteration();
		m_dHighwayRelativeGap			= oDlg.RelativeGap();
		m_dUEOBaseScaleFactor			= oDlg.ScaleFactor();
		m_nHighwayRelativeGapExp		= oDlg.RelativeGapExp();
		m_nScaleFactorCursor			= oDlg.ScaleFactorCursor();

		m_nIntermodalIteration          = oDlg.IntermodalIteration();
		m_dIntermodalL2Norm             = oDlg.IntermodalL2Norm();

		m_btnStoppingCriteria.SetWindowText(UIXAML_D_STOPCRITERIA_SELECTED);

// 		m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_DEFAULT);
// 		m_btnOutputDesign.EnableWindow(TRUE);
	}

	KRedrawWindow();
}


void KInterModalDlg::OnBnClickedViewTable()
{
	KLogDebugStartMsg();
	KIOTableManager* pTableManager = m_pTarget->Tables();
	KIOTable* pLinkTable = pTableManager->FindTable(TABLE_LINK);
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
	KLogDebugEndMsg();
}


void KInterModalDlg::OnBnClickedOutdesign()
{
	KLogDebugStartMsg();

	m_oOutputDesigndlg.SetTarget(m_pTarget);
	m_oOutputDesigndlg.SetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
	if (m_oOutputDesigndlg.DoModal() == IDOK)
	{
		m_TOverWirteAResultInfo.TbUseData = true;
		m_oOutputDesigndlg.GetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
		m_btnOutputDesign.SetWindowText(UIXAML_D_OUTPUT_SELECTED);
		m_btnOutputDesign.EnableWindow(TRUE);
	}

	KRedrawWindow();
	KLogDebugEndMsg();
}


void KInterModalDlg::OnBnClickedClose()
{
	OnCancel();
}

void KInterModalDlg::AddStatusMessage(CString strMsg)
{
// 	if(NULL != m_pProgressDlg)
// 	{
// 		m_pProgressDlg->SetStatus(strMsg);
// 	}

	KListSimpleProgressPtr spProgressWindow = KListSimpleProgressThread::SimpleProgress();
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(CString(strMsg));
	}
}

bool KInterModalDlg::ClearFiles()
{
	KLogDebugStartMsg();

	int nResult = -1;
	try
	{
		//delete input files
		nResult = remove("urbanIntermodal.arg");
		
		//delete outputfiles
		
	}
	catch (...)
	{
		KLogDebugException();
		return false;
	}
	KLogDebugEndMsg();
	return true;
}



/*Step4. Transit Assign Option*/
bool KInterModalDlg::WriteTransitAssignOptionArgs(CStdioFileEx& of)
{
	try
	{
		oTransitAssignDlg.WriteInputArgument(of);
	}
	catch (...)
	{
		KLogDebugException();
		return false;
	}
	return true;
}

/*Step5. Mode/Route Choice Parameter*/
bool KInterModalDlg::WriteModeRouteChoiceArgs(CStdioFileEx& of)
{
	CString strOut;

	try
	{
		int nCountVector = m_vecInterChoiceParam.size();
		int nCountSelected(0);
		for(int i = 0; i < nCountVector; ++i)
		{
			KInterChoiceParamBase* pBase = m_vecInterChoiceParam[i];
			if(pBase->Selected())
			{
				++nCountSelected;
			}
		}
		strOut.Format(_T("choice_parameter\t%d\r\n"), nCountSelected * 7);
		of.WriteString(strOut);
		for(int i = 0; i < nCountVector; ++i)
		{
			KInterChoiceParamBase* pBase = m_vecInterChoiceParam[i];

			if(!pBase->Selected())
			{
				continue;
			}
			std::vector<KInterChoiceParam*> vecChoiceParam = pBase->GetInterChoiceParam();
			int nCountParam = vecChoiceParam.size();
			for(int j = 0; j < nCountParam; ++j)
			{
				KInterChoiceParam* pParam = vecChoiceParam[j];
				strOut.Format(_T("%d\t%d\t%f\t%f\t%f\t%f\r\n"), 
					i+1, j+1, pParam->Auto(), pParam->Taxi(), pParam->Bus(), pParam->Rail());
				of.WriteString(strOut);
			}
		}
	}
	catch(...)
	{
		of.Close();
		KLogDebugException();
		return false;
	}

	return true;
}

/*Step6. StoppingCriteria*/
bool KInterModalDlg::WriteStoppingCriteria(CStdioFileEx& of)
{
	CString strOut;
	try
	{
		strOut.Format(_T("stopping_criteria\t%d\r\n"), 1);
		of.WriteString(strOut);

		strOut.Format(_T("%d\t%f\t%d\t%f\r\n"), 
			m_nHighwayIterationNo, m_dHighwayRelativeGap, m_nTransitIterationNo, m_dTransitNormalizeGap);
		of.WriteString(strOut);
	}
	catch(...)
	{
		of.Close();
		KLogDebugException();
		return false;
	}
	return true;
}

void KInterModalDlg::OnBnClickedRun()
{
	if( KListSimpleProgressThread::ExecuteThread(InterModalModelThreadProc, this, true) == IDOK)
	{
		//정상
	}
	else
	{
		//유저
	}
}


unsigned __stdcall KInterModalDlg::InterModalModelThreadProc(void* pParam)
{
	KListSimpleProgressParameter* pParameter = (KListSimpleProgressParameter*)pParam;

	KInterModalDlg* pDlg = (KInterModalDlg*)pParameter->GetParameter();;
	pDlg->RunInterModalModel();

	return 0;
}



void KInterModalDlg::InitParameterValue( void )
{
	int nColumnCount = 0;
	std::list<KInterInputInfo*>::iterator iter, end = m_lstInterInputInfo.end();

	for (iter = m_lstInterInputInfo.begin(); iter != end; ++iter)
	{
		KInterInputInfo* pInputInfo = *iter;

		if (!pInputInfo->GetSelected())
			continue;

		++nColumnCount;
	}

	if (NULL != m_pRecordArray)
	{
		delete m_pRecordArray;
		m_pRecordArray = NULL;
	}

	m_pRecordArray = new KSDoubleRecordArray(nColumnCount);

	int nRecordCount = m_vecParameterColumn.size();

	for (int i = 0; i < nRecordCount; ++i)
	{
		KSDoubleRecord* pReocrd = m_pRecordArray->AddNewRecord();

		int j = 0;

		std::list<KInterInputInfo*>::iterator iter, end = m_lstInterInputInfo.end();

		for (iter = m_lstInterInputInfo.begin(); iter != end; ++iter)
		{
			KInterInputInfo* pInputInfo = *iter;

			if (!pInputInfo->GetSelected())
				continue;

			KPurpose* pPurpose = pInputInfo->GetPurpose();

			std::map<int, std::vector<double>>::iterator find = m_mapDefault.find(pPurpose->PurposeID());

			if (find != m_mapDefault.end())
			{
				std::vector<double> vecDefault = find->second;
				pReocrd->SetAt(j++, vecDefault[i]);
			}

		}
	}
}

void KInterModalDlg::InitParameterColumn( void )
{
	m_vecParameterColumn.clear();

	m_vecParameterColumn.push_back(_T("UVAutoTime"));
	m_vecParameterColumn.push_back(_T("UVAutoCost"));
	m_vecParameterColumn.push_back(_T("UVParkingFee"));
	m_vecParameterColumn.push_back(_T("UVTaxiTime"));
	m_vecParameterColumn.push_back(_T("UVTaxiCost"));
	m_vecParameterColumn.push_back(_T("UVTransitWalk"));
	m_vecParameterColumn.push_back(_T("UVWaitTime"));
	m_vecParameterColumn.push_back(_T("UVTransferTime"));
	m_vecParameterColumn.push_back(_T("UVTransferNo"));
	m_vecParameterColumn.push_back(_T("UVInVehTime"));
	m_vecParameterColumn.push_back(_T("UVCost"));
	m_vecParameterColumn.push_back(_T("UVAutoDummy"));
	m_vecParameterColumn.push_back(_T("UVTaxiDummy"));
	m_vecParameterColumn.push_back(_T("UVBusDummy"));
	m_vecParameterColumn.push_back(_T("UVRailDummy"));
	m_vecParameterColumn.push_back(_T("UVTransitDummy"));
	m_vecParameterColumn.push_back(_T("UVParkAndRideDummy"));
	m_vecParameterColumn.push_back(_T("UVKissAndRidDummy"));
	m_vecParameterColumn.push_back(_T("UVTaxiAndTransitDummy"));
}

void KInterModalDlg::WriteModelInfoArgument(CStdioFileEx& of)
{
	CString   strOut;
	KProject* pProject = (KProject*)((CKmzApp*)AfxGetApp())->GetProject();

	try
	{
		strOut.Format(_T("%s_%s_%s\r\n"), 
			pProject->GetName(), m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear());
		of.WriteString(strOut);

		strOut.Format(_T("model\t6\r\n"));
		of.WriteString(strOut);

		strOut.Format(_T("TimeOfAnalysis\t%f\r\n"), m_dAnalysisTime);
		of.WriteString(strOut);

	}
	catch(...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteModelInfoArg) 중 오류가 발생하였습니다!"));
	}

}

void KInterModalDlg::WriteInputArgument(
	CStdioFileEx& of, KIOColumnCollection& purposeColumnCollection)
{
	CString strOut;

	try
	{
		int nPurposeCount 	  = 0;
		
		//purpose
		std::list<KInterInputInfo*>::iterator itInput, itEnd = m_lstInterInputInfo.end();
		
		for(itInput = m_lstInterInputInfo.begin();itInput != itEnd; itInput++)
		{
			KInterInputInfo* pInputInfo = *itInput;

			if(!pInputInfo->GetSelected())
				continue;
			++nPurposeCount;

			purposeColumnCollection.AddNewColumn( pInputInfo->InputColumn() );
		}

		//mode

		strOut.Format(_T("noPurpose\t%d\r\n"), nPurposeCount);
		of.WriteString(strOut);

		strOut.Format(_T("noMode\t%d\r\n"), 4);
		of.WriteString(strOut);

		strOut.Format(_T("noPreLoadMode\t%d\r\n"), 0);
		of.WriteString(strOut);

		int nIntermodalMode = m_vecIntermodalModeInfo.size();

		for (int i = 0; i < nIntermodalMode; ++i)
		{
			TIntermodalMode oIntermodalMode = m_vecIntermodalModeInfo[i];

			strOut.Format(_T("%s\t%f\t%f\t%d\r\n"),
				oIntermodalMode.strModeName, oIntermodalMode.dOccupancy,
				oIntermodalMode.dPCU, oIntermodalMode.nNetworkType);

			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteInputArg) 중 오류가 발생하였습니다!"));
	}
}

void KInterModalDlg::WriteVDFArgument( CStdioFileEx& of )
{
	try
	{
		m_oKInterBPRFnDlg.WriteInputArgument(of);
	}
	catch (...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteVDFArgument) 중 오류가 발생하였습니다!"));
	}
}

void KInterModalDlg::WriteCostArgument(CStdioFileEx& of)
{
	CString strOut;

	try
	{
		int nCountPrivateSector = m_vecInterDefineCostPrivateSector.size();

		//PrivateSector
		for (int i = 0; i < nCountPrivateSector; ++i)
		{
			KInterDefineCostPrivateSector* pPrivateSector = m_vecInterDefineCostPrivateSector[i];

			strOut.Format(_T("Auto%s\t%f\t%f\t%f\r\n"), 
				pPrivateSector->GetName(), pPrivateSector->GetPrice(), 
				pPrivateSector->GetRatio(), pPrivateSector->GetFuelEfficiency());
			of.WriteString(strOut);
		}

		strOut.Format(_T("AutoParkFee\t%d\r\n"), m_nParkFee);
		of.WriteString(strOut);

		strOut.Format(_T("%s\t%f\t%f\r\n"), _T("TaxiTimeCost"), m_dTaxiWaitingTime, m_dTaxiStoppingTime);
		of.WriteString(strOut);

		int nCountPublicSector = m_vecInterDefineCostPublicSector.size();
		//PublicSector
		for (int i = 0; i < nCountPublicSector; ++i)
		{
			KInterDefineCostPublicSector* pPublicSector = m_vecInterDefineCostPublicSector[i];
			
			strOut.Format(_T("%s\t%f\t%f\t%f\t%f\r\n"),
				pPublicSector->GetModeName(), pPublicSector->GetBaseCharge(), 
				pPublicSector->GetBaseDistance(), pPublicSector->GetAddDistance(), 
				pPublicSector->GetAddCharge());
			of.WriteString(strOut);
		}

		strOut.Format(_T("%s\t%d\r\n"), _T("maxTransitPath"), m_nNoOfRoute);
		of.WriteString(strOut);

		strOut.Format(_T("%s\t%d\r\n"), _T("maxZoneConnect"), m_nMaxNoOfZoneConnectors);
		of.WriteString(strOut);

		strOut.Format(_T("%s\t%f\r\n"), _T("maxWalkTime"), m_dMaxWalkingTimeToStation);
		of.WriteString(strOut);

		strOut.Format(_T("%s\t%f\r\n"), _T("TimeofBoardAlight"), m_dTimeOfBoarding);
		of.WriteString(strOut);
	}
	catch(...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteCostArgument) 중 오류가 발생하였습니다!"));
	}
}

void KInterModalDlg::WriteParamArgument( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		int nRecordCount = m_vecParameterColumn.size();

		for (int i = 0; i < nRecordCount; ++i)
		{
			strOut = m_vecParameterColumn[i];
			
			KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(i);

			size_t nCount = pRecord->GetCount();

			CString strTemp;
			
			for (size_t j = 0; j < nCount; ++j)
			{
				double dValue = pRecord->GetAt(j);
				strTemp.Format(_T("\t%.5f"), dValue);

				strOut += strTemp;
			}
			strOut += _T("\r\n");
			of.WriteString(strOut);
		}
	}
	catch (...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteParamArgument) 중 오류가 발생하였습니다!"));
	}
}

void KInterModalDlg::WriteCriteriaArgument( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		strOut.Format(_T("UEIteration\t%d\t%g\t%f\r\n")
			,m_nHighwayIterationNo, m_dHighwayRelativeGap, m_dUEOBaseScaleFactor);
		of.WriteString(strOut);

		strOut.Format(_T("IMIteration\t%d\t%f\r\n"), m_nIntermodalIteration, m_dIntermodalL2Norm);
		of.WriteString(strOut);
	}
	catch(...)
	{
		KLogDebugException();
		ThrowException(_T("Argument 생성(WriteCriteriaArgument) 중 오류가 발생하였습니다!"));
	}
}

void KInterModalDlg::CreateArguments( void )
{
	KLogDebugStartMsg();
	AddStatusMessage(_T("Create Argument..."));

	if (!ClearFiles())
		ThrowException(_T("기존 파일삭제 중 오류가 발생했습니다."));

	CString strFileName = _T("urbanIntermodal.arg");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	try
	{
		KIOColumnCollection purposeColumnCollection;

		WriteModelInfoArgument(of);
		WriteInputArgument(of, purposeColumnCollection);

		//Default Output...
		of.WriteString(_T("TurnRestriction\t0\r\n"));
		of.WriteString(_T("BusInitVolume\t1\r\n"));

		WriteVDFArgument(of);
		WriteCostArgument(of);
		WriteParamArgument(of);
		WriteCriteriaArgument(of);

		//Default Output Value...
		of.WriteString(_T("UtilitySelect_min_1_avg_2\t1\r\n"));
		of.WriteString(_T("AutoPathWrite\t0\r\n"));

		of.WriteString(_T("TransitPathWrite\t1\r\n"));
		of.WriteString(_T("InterModalPathWrite\t0\r\n"));
		of.WriteString(_T("ProbabilityWrite\t0\r\n"));

		of.WriteString(_T("UtilityValueWrite\t0\r\n"));
		of.WriteString(_T("AlternativeODWrite\t0\r\n"));
		of.WriteString(_T("UtilityVariableWrite\t0\r\n"));

		of.WriteString(_T("NotConvergeODCell\t0\r\n"));
		of.WriteString(_T("ModeODWrite\t0\r\n"));
		of.WriteString(_T("RailLinkTripWrite\t0\r\n"));

		of.WriteString(_T("LinkTimeWrite\t0\r\n"));

		of.Close();
		
		bool bResult = false;
		
		CString strQuery = m_oKInterBPRFnDlg.GetAddQuery();
		KIOColumnCollection userColumnsLink;

		if (KListSimpleProgressThread::IsUserStopped() == true)
			return;

		bResult = KInterBPRFnDlg::ExportDB2LinkFile(m_pTarget->GetDB(), userColumnsLink, strQuery);

		if (!bResult)
		{
			AfxMessageBox(_T("Argument 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
			return;
		}
		
		if (KListSimpleProgressThread::IsUserStopped() == true)
			return;
		
		// 노드파일 생성
		bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDB());
		if (!bResult)
		{
			AfxMessageBox(_T("Binary 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
			return;
		}

		if (KListSimpleProgressThread::IsUserStopped() == true)
			return;

		// 회전정보파일 생성
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDB());
		if (!bResult)
		{
			AfxMessageBox(_T("Binary 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
			return;
		}

		if (KListSimpleProgressThread::IsUserStopped() == true)
			return;

		//line.dat, line_info.dat 생성.
		bResult = KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDB());
		if (!bResult)
		{
			AfxMessageBox(_T("Binary 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
			return;
		}

		if (KListSimpleProgressThread::IsUserStopped() == true)
			return;

		// purpose_od 파일 생성
		bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDB(), _T("purpose_od"),
			purposeColumnCollection, _T("purpose_od.dat"));
		if (!bResult)
		{
			AfxMessageBox(_T("Argument 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
			return;
		}
		AddStatusMessage(_T("Create Argument End"));

		//Dll Load

		if (NULL != m_hUrbanIntermodal)
			::FreeLibrary(m_hUrbanIntermodal);
		
		m_hUrbanIntermodal = ::LoadLibrary(_T("FUrbanIntermodal"));

		if (NULL == m_hUrbanIntermodal)
		{
			AfxMessageBox(_T("Dll이 없습니다."));
			return;
		}

		UrbanIntermodalProcPtr = 
			(FURBANINTERMODAL)GetProcAddress(m_hUrbanIntermodal, "FURBANINTERMODAL");

		if(NULL == UrbanIntermodalProcPtr)
		{
			AfxMessageBox(_T("FURBANINTERMODAL 함수가 없습니다. "));
			::FreeLibrary(m_hUrbanIntermodal);
			return;
		}

		//Run Assign
		AddStatusMessage(_T("> Assignment Start ..."));

		CTime timeStart = CTime::GetCurrentTime();

		int nResult = UrbanIntermodalProcPtr((void*)this, (void*)ReceiveStatusMessageFromDll);

		// Step 4: 배정모형 종료 -> 라이브러리 Unload
		if(NULL != m_hUrbanIntermodal)
		{
			::FreeLibrary(m_hUrbanIntermodal);
			KLogDebug(_T("FreeLibrary "));
		}

		if(nResult == 1 /*OK*/)
		{
			AddStatusMessage(_T("> Assignment End ..."));
			KLogDebug(_T("Model Run Ok.."));
		}
		else
		{
			AddStatusMessage(_T("> Assignment End(Error) ..."));
			KLogDebug(_T("Model Run Error.."));

			return;   
		}

		/*Step 5. 결과 정보 저장*/
		AddStatusMessage(_T("> Save Results."));

		KIOColumnCollection         rLinkColumnCollection, rModeODColumnCollection;
		TAssignResultColumnName resultLinkColumnName;

		int nRunSeq(0);

		try
		{
			if (m_bUseOverWrite == true)
			{
				nRunSeq = m_TOverWirteAResultInfo.TnRunSeq;
			}
			else
			{
				nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);
			}
			
			AddAssignResultColumn(nRunSeq, rLinkColumnCollection, rModeODColumnCollection, resultLinkColumnName,
								  m_TOverWirteAResultInfo.TstrDescription);
		}
		catch(KExceptionPtr ex)
		{
			KLogDebug(ex->GetErrorMessage());
			AfxMessageBox(_T("> Column 추가 실패 ..."));
			return;
		}
		catch(...)
		{
			KLogDebugException();
			AfxMessageBox(_T("> Column 추가 실패 ..."));
			return;
		}

		/*Step 5.3 UpdateDataBase*/
		KDBaseConnection* pDBaseConnection = new KDBaseConnection(m_pTarget->GetDB());

		try
		{
			pDBaseConnection->BeginTransaction();

			if (KListSimpleProgressThread::IsUserStopped() == true)
				return;

			/*Step 5.3.1 링크 정보 입력 ...*/
			UpdateLinkFile2DB(pDBaseConnection, rLinkColumnCollection);

			if (KListSimpleProgressThread::IsUserStopped() == true)
				return;

			/*Step 5.3.2 수단 OD 정보 입력 ...*/
			UpdateModeODFile2DB(pDBaseConnection, rModeODColumnCollection);


			/*strp 5.3.3 OverWrite일 경우 데이터 삭제 */
			if (m_bUseOverWrite == true)
			{
				DeleteOverWriteInterModal(pDBaseConnection);
			}

			/*Step 5.3.4. 배정모형 종료정보 업데이트 ...*/

			int nType = 6; //

			CTime timeEnd = CTime::GetCurrentTime();
			KDBaseAssignment::UpdateAssignRunSeq(
				pDBaseConnection, nRunSeq, resultLinkColumnName, 
				timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
				timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), m_TOverWirteAResultInfo.TstrDescription, nType);

			rLinkColumnCollection.Clear();
			rModeODColumnCollection.Clear();

			pDBaseConnection->Commit();
		}
		catch(KExceptionPtr ex)
		{
			AddStatusMessage(_T("구동 중 오류가 발생했습니다."));
			pDBaseConnection->RollBack();
			KLogDebug(ex->GetErrorMessage());
		}
		catch(...)
		{
			AddStatusMessage(_T("구동 중 오류가 발생했습니다."));
			pDBaseConnection->RollBack();
			KLogDebugException();
		}

		delete pDBaseConnection;

		/*Column 삭제*/
		int nLinkAddColumnCollections	= rLinkColumnCollection.ColumnCount();
		int nModeODAddColumnCollections = rModeODColumnCollection.ColumnCount();

		if( 0 != nLinkAddColumnCollections ||  0 != nModeODAddColumnCollections)
		{
			KLogDebug(_T("Delete Added Column"));
			KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);

			for (int i = 0; i < nLinkAddColumnCollections; ++i)
			{
				pLinkTable->RemoveColumn(rLinkColumnCollection.GetColumn(i)->Name());
			}
			KIOTable* pModeODTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);

			for (int i = 0; i < nModeODAddColumnCollections; ++i)
			{
				pModeODTable->RemoveColumn(rLinkColumnCollection.GetColumn(i)->Name());
			}
			pLinkTable->Notify();
			pModeODTable->Notify();
		}
		else if (m_bUseOverWrite == true)
		{
			DeleteOverWriteColumn();
		}

		AddStatusMessage(_T("> Complete"));

		m_btnViewTable.SetWindowText(UIXAML_D_VTABLE_DEFAULT);
		m_btnViewTable.EnableWindow(TRUE);

	}
	catch (KExceptionPtr ex)
	{
		of.Close();
		throw ex;
	}
	catch (...)
	{
		of.Close();
		throw 1;
	}
	KLogDebugEndMsg();
}


void KInterModalDlg::RunInterModalModel()
{
	KLogDebugStartMsg();
	try
	{
		CreateArguments();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{

	}
	KLogDebugEndMsg();
}

void KInterModalDlg::RunnableCheck( void )
{
	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);

	CString strBtnState;

	m_btnPurposeInput.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_PURPOSE_INPUT_SELECTED) != 0)
		return;

	m_btnModeInput.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_MODE_INPUT_SELECTED) != 0)
		return;

	m_btnDefineCost.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_DEFINECOST_SELECTED) != 0)
		return;

	m_btnDefineVDF.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_DEFINEVDF_SELECTED) != 0)
		return;

	m_btnChoiceParameter.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_CHOICEPARAM_SELECTED) != 0)
		return;

	m_btnStoppingCriteria.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_STOPCRITERIA_SELECTED) != 0)
		return;

	m_btnOutputDesign.GetWindowText(strBtnState);

	if (strBtnState.CompareNoCase(UIXAML_D_OUTPUT_SELECTED) != 0)
		return;

	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(TRUE);
}

void KInterModalDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
// 	CString strDllMsg(strMsg);
// 
// 	if(NULL != m_pProgressDlg)
// 	{
// 		m_pProgressDlg->SetStatus(strDllMsg);
// 	}
	CString strDllMsg(strMsg);

	//if(NULL != m_pProgressDlg)
	//{
	//	m_pProgressDlg->SetStatus(strDllMsg);
	//}

	KListSimpleProgressPtr spProgressWindow = KListSimpleProgressThread::SimpleProgress();
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(strDllMsg);
	}
}

void KInterModalDlg::AddAssignResultColumn( int runSeq, 
	KIOColumnCollection& rLinkColumnCollection, 
	KIOColumnCollection& rODColumnCollection, 
	TAssignResultColumnName& resultLinkColumnName,
	CString strDescription)
{
	KLogDebugStartMsg();
	CString strErrMsg = _T("> Column 추가 실패 ...");

	rLinkColumnCollection.Clear();
	rODColumnCollection.Clear();

	KIOTableManager* pTableManager = m_pTarget->Tables();
	KIOTable*        pLinkTable    = pTableManager->FindTable(TABLE_LINK);
	KIOTable*        pODTable      = pTableManager->FindTable(TABLE_MODE_OD);

	CString strColumnName, strRunSeq;
	strRunSeq.Format(_T("(%d)"), runSeq);

	// Link Table
	KIOColumn* pColumn = NULL;
	pColumn = pLinkTable->AddDoubleValueColumn(_T("Average_Time") + strRunSeq);
	if (pColumn)
	{
		pColumn->ColumnInfo(strDescription);
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultLinkColumnName.AverageTime = pColumn->Name();
	}
	else
	{
		ThrowException(strErrMsg);
	}

	pColumn = pLinkTable->AddDoubleValueColumn(_T("V/C") + strRunSeq);
	if (pColumn)
	{
		pColumn->ColumnInfo(strDescription);
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultLinkColumnName.VC = pColumn->Name();
	}
	else
	{
		ThrowException(strErrMsg);
	}

	pColumn = pLinkTable->AddDoubleValueColumn(_T("Sum_Volume") + strRunSeq);
	if (pColumn)
	{
		pColumn->ColumnInfo(strDescription);
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultLinkColumnName.SumVolume = pColumn->Name();
	}
	else
	{
		ThrowException(strErrMsg);
	}

	pColumn = pLinkTable->AddDoubleValueColumn(_T("Trip") + strRunSeq);
	if (pColumn)
	{
		pColumn->ColumnInfo(strDescription);
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultLinkColumnName.Trip = pColumn->Name();
	}
	else
	{
		ThrowException(strErrMsg);
	}

// 	KIOColumnCollection modeColumnCollection;
// 	GetInputModeColumns(modeColumnCollection);
// 
// 	for (int i=0; i<modeColumnCollection.ColumnCount(); i++)
// 	{
// 		pColumn = pLinkTable->AddDoubleValueColumn(modeColumnCollection.GetColumn(i)->Caption() + strRunSeq);
// 		if (pColumn)
// 		{
// 			rLinkColumnCollection.AddNewColumn(pColumn);
// 		}
// 		else
// 		{
// 			ThrowException(strErrMsg);
// 		}
// 	}

	// ModeOD Table
	pColumn = pODTable->AddDoubleValueColumn(_T("Average_Time") + strRunSeq);
	if (pColumn)
	{
		pColumn->ColumnInfo(strDescription);
		rODColumnCollection.AddNewColumn(pColumn);
	}
	else
	{
		ThrowException(strErrMsg);
	}

	pLinkTable->Notify();
	pODTable->Notify();
	// Transit
	// Transit_Links
	KLogDebugEndMsg();
}

void KInterModalDlg::UpdateLinkFile2DB( 
	KDBaseConnection* a_pDBaseConnection, KIOColumnCollection& rLinkColumnCollection )
{
	KLogDebugStartMsg();
	KIOTableManager* pTableManager = m_pTarget->Tables();
	KIOTable*        pLinkTable    = pTableManager->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	std::ifstream ifs( _T("out_link.dat"), std::ios::binary );    

	try
	{		
		KIDKeyDoubleRecords records(nColumnCount);

		if ( ifs )
		{
			records.ReadBytes(ifs);
		}
		else
		{
			ThrowException(_T("error file read"));
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(
			a_pDBaseConnection, pLinkTable, rLinkColumnCollection, records);       
	}
	catch (...)
	{
		KLogDebugException();
		ifs.close();
		throw 1;
	}
	KLogDebugEndMsg();
}

void KInterModalDlg::UpdateModeODFile2DB( 
	KDBaseConnection* a_pDBaseConnection, KIOColumnCollection& rModeODColumnCollection )
{
	KLogDebugStartMsg();
	KIOTableManager* pTableManager = m_pTarget->Tables();
	KIOTable*        pODTable      = pTableManager->FindTable(TABLE_MODE_OD);
	int nColumnCount = rModeODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	std::ifstream ifs( _T("out_od.dat"), std::ios::binary );

	try
	{
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;

		KxBulkDbase oBulkDBase(a_pDBaseConnection, pODTable, &rModeODColumnCollection);

		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);

			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		oBulkDBase.DeprecatedFnExecute();
		ifs.close();
		delete pRecord;
	}
	catch(KExceptionPtr ex)
	{
		KLogDebugException();
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		ifs.close();
		throw 1;
	}    
	KLogDebugEndMsg();
}


void KInterModalDlg::OnBnClickedButton2()
{
	m_oOutputDesigndlg.SetTarget(m_pTarget);
	m_oOutputDesigndlg.SetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
	if (m_oOutputDesigndlg.DoModal() == IDOK)
	{
		m_TOverWirteAResultInfo.TbUseData = true;
		m_oOutputDesigndlg.GetOutputInfo(m_bUseOverWrite, m_TOverWirteAResultInfo);
	}
}

bool KInterModalDlg::DeleteOverWriteInterModal( KDBaseConnection* a_pDBBaseConnection )
{
	int nRunSeq = m_TOverWirteAResultInfo.TnRunSeq;

	CString strQuery = _T("");
	strQuery.Format(_T("Delete from assign_model where run_seq = %d "), nRunSeq);

	a_pDBBaseConnection->ExecuteUpdate(strQuery);

	return true;
}

bool KInterModalDlg::DeleteOverWriteColumn()
{
	KLogDebug(_T("Delete OverWrite Column"));
	KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);

	KIOTable* pModeTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);

	CString strVC = m_TOverWirteAResultInfo.TstrVC;
	if (strVC != _T(""))
	{
		pLinkTable->RemoveColumn(strVC);
	}


	CString strAveTime = m_TOverWirteAResultInfo.TstrAverageTime;
	if (strAveTime != _T(""))
	{
		pLinkTable->RemoveColumn(strAveTime);
	}


	CString strSumVolumn = m_TOverWirteAResultInfo.TstrSumVolumn;
	if (strSumVolumn != _T(""))
	{
		pLinkTable->RemoveColumn(strSumVolumn);
	}


	CString strAverTollTime = m_TOverWirteAResultInfo.TstrAverageTime;
	if (strAverTollTime != _T(""))
	{
		pLinkTable->RemoveColumn(strAverTollTime);
	}

	CString strTrip = m_TOverWirteAResultInfo.TstrTrip;
	if (strTrip != _T(""))
	{
		pLinkTable->RemoveColumn(strTrip);
	}

	std::vector<CString> vecParse;
	int nCount = ParseStringArr(m_TOverWirteAResultInfo.TstrVDFTime, vecParse);
	for (int i = 0; i < nCount; i++)
	{
		CString strColumnName = vecParse[i];
		pLinkTable->RemoveColumn(strColumnName);
	}


	nCount = ParseStringArr(m_TOverWirteAResultInfo.TstrModeVolume, vecParse);
	for (int i = 0; i < nCount; i++)
	{
		CString strColumnName = vecParse[i];
		pLinkTable->RemoveColumn(strColumnName);
	}


	nCount = ParseStringArr(m_TOverWirteAResultInfo.TstrVDFTimeToll, vecParse);
	for (int i = 0; i < nCount; i++)
	{
		CString strColumnName = vecParse[i];
		pLinkTable->RemoveColumn(strColumnName);
	}


	CString strODAverageTime = m_TOverWirteAResultInfo.TstrODAveTime;
	if (strODAverageTime != _T(""))
	{
		pModeTable->RemoveColumn(strODAverageTime);
	} 


	nCount = ParseStringArr(m_TOverWirteAResultInfo.TstrODVDFTime, vecParse);
	for (int i = 0; i < nCount; i++)
	{
		CString strColumnName = vecParse[i];
		pModeTable->RemoveColumn(strColumnName);
	}

	return true;
}
