/**
 * @file KTimeODGenerationDlg.cpp
 * @brief KTimeODGenerationDlg 구현 파일
 * @author jyk@qbicware.com
 * @date 2012.05.14
 * @remark
 */

#include "stdafx.h"
#include "KmzApp.h"
#include "TimeODGenerationDlg.h"
#include "afxdialogex.h"
#include "Mode.h"
#include "Purpose.h"
#include "Target.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "KUIDefTimeOD.h"

#include "TimeODInputDlg.h"
#include "TimeODZoneCharacterDlg.h"
#include "TimeODDefPurposeMode.h"
#include "TimeODOutputDesignDlg.h"
#include "DefineNames.h"
#include "KBulkDBase2File.h"
#include "KBulkDBase.h"
#include "ImChampDir.h"

#include "Launcher.h"

using namespace UITimeOD;

// KTimeODGenerationDlg 대화 상자입니다.

#if WINVER >= 0x0500
#include <uxtheme.h>
#pragma comment (lib, "UxTheme.lib")
#endif

IMPLEMENT_DYNAMIC(KTimeODGenerationDlg, KDialogEx)

bool KTimeODGenerationDlg::m_bRunTimeODSucceess = false;

KTimeODGenerationDlg::KTimeODGenerationDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTimeODGenerationDlg::IDD, pParent),
	m_emTimeOD(KEMTimeODHourlyTotal),
	m_pTarget(NULL)
{
	m_nFromTime  = 0;
	m_nToTime    = 0;
	m_nZoneCount = 0;
}

KTimeODGenerationDlg::~KTimeODGenerationDlg()
{
}

void KTimeODGenerationDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_btnHourlyTotal);
	DDX_Control(pDX, IDC_RADIO2, m_btnHourly);
	DDX_Control(pDX, IDC_STATIC_TIMEOD,  m_lblTimeOD);
	DDX_Control(pDX, IDC_STATIC_SETTIME, m_lblSetTime);

	DDX_Control(pDX, IDC_STATIC_BACK,   m_uiBackImage);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow4);    
	DDX_Control(pDX, IDC_STATIC_ARROW5, m_uiArrow5);
	DDX_Control(pDX, IDC_INPUT,                 m_btnInput);
	DDX_Control(pDX, IDC_ZONE_CHARACTERIZATION, m_btnZoneCharacter);
	DDX_Control(pDX, IDC_DEFINEPURPOSENMODE,    m_btnDefinePurposeMode);
	DDX_Control(pDX, IDC_OUTPUTDESIGN,          m_btnOutputDesign);
	DDX_Control(pDX, IDC_OUTPUTTABLE,           m_btnViewTable);
	DDX_Control(pDX, IDC_COMBO1, m_cboFromTime);
	DDX_Control(pDX, IDC_COMBO2, m_cboToTime);
}


BEGIN_MESSAGE_MAP(KTimeODGenerationDlg, KDialogEx)
 	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, OnRadioClicked)
 	ON_BN_CLICKED(IDC_INPUT,                 &KTimeODGenerationDlg::OnBnClickedInput)
 	ON_BN_CLICKED(IDC_ZONE_CHARACTERIZATION, &KTimeODGenerationDlg::OnBnClickedCharacterization)
	ON_BN_CLICKED(IDC_DEFINEPURPOSENMODE,    &KTimeODGenerationDlg::OnBnClickedDefinePurposeNmode)
 	ON_BN_CLICKED(IDC_OUTPUTDESIGN,          &KTimeODGenerationDlg::OnBnClickedOutputdesign)
 	ON_BN_CLICKED(IDC_RUN,                   &KTimeODGenerationDlg::OnBnClickedRun)
 	ON_BN_CLICKED(IDC_OUTPUTTABLE,           &KTimeODGenerationDlg::OnBnClickedViewTripRate)
	ON_BN_CLICKED(IDC_CLOSE,                 &KTimeODGenerationDlg::OnBnClickedClose)
	ON_WM_CTLCOLOR()
	//ON_WM_ACTIVATE() - markup button focus 있을때 외부 클릭 시 버튼 사라짐 문제 때문에 기능 삭제
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


void KTimeODGenerationDlg::SetTarget(KTarget* pTarget)
{
	TxLogDebugStartMsg();
	m_pTarget = pTarget;
	TxLogDebugEndMsg();
}


BOOL KTimeODGenerationDlg::OnInitDialog()
{
	TxLogDebugStartMsg();
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	InitPurposeData();
	InitModeData();
	InitZoneCharacterData();
	InitZoneCount();
	
	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);


	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(4, 5);

	m_uiBackImage.MoveWindow(&rcClient);
	m_uiBackImage.SetMarkupText(UIXAML_TIMEOD_BACKIMAGE);

	m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB); 
	m_uiArrow2.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
	m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT); 
	m_uiArrow4.SetPosition(ZEMArrowPositionCB, ZEMArrowPositionCT); 
	m_uiArrow5.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC); 

#ifndef _UNICODE
	SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, L"", L"");
	SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, L"", L"" );
#else
	SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, _T(""), _T("") );
#endif

	UINT nID_TIMEOD  = 90905;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL, &nID_TIMEOD, 1, 0); 

	UINT nID_TIMEOD_OUTPUT = 90927;
	XTPImageManager()->SetIcons(IDB_MODECHOICE_OUTPUT, &nID_TIMEOD_OUTPUT, 1, 0); 

	// 모델
	CWnd* pPlaceholder = GetDlgItem(IDC_STATIC_TIMEOD);
	CRect rc;
	pPlaceholder->GetWindowRect(&rc);
	ScreenToClient(&rc);
	int w = rc.Width() - 4;
	int h = rc.Height() - 4;
	UINT dpi = GetDpiForWindow(*pPlaceholder);
	double fontSize = 11 * (dpi / 96.0);

	m_lblTimeOD.MoveWindow(&rc);

	CString timeODXamlKR;
	timeODXamlKR.Format(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='%f'>")
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#003366'>시간대별 O/D 생성 모형</TextBlock>")
		_T("    <Border Width='%d' Height='1' Background='#6d896a' />")
		_T("    <Border Width='%d' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>"),
		w, h, fontSize, w, w
	);

	CString timeODXamlEN;
	timeODXamlEN.Format(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='%f'>")
		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")
		_T("    <Border Width='%d' Height='1' Background='#6d896a' />")
		_T("    <Border Width='%d' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>"),
		w, h, fontSize, w, w
	);

	

	// TimeOD
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_lblTimeOD.SetMarkupText (timeODXamlKR);
	}
	else {
		m_lblTimeOD.SetMarkupText (timeODXamlEN);
	}
	m_lblSetTime.SetMarkupText(UIXAML_TIMEOD_SETTIME_SELECTED);

	// Input
	m_btnInput.EnableMarkup();
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnInput.SetWindowText(KR_UIXAML_TIMEOD_INPUT_DEFAULT);
	}
	else {
		m_btnInput.SetWindowText(UIXAML_TIMEOD_INPUT_DEFAULT);
	}
	
	m_btnZoneCharacter.EnableWindow(TRUE);

	// Zone Character
	m_btnZoneCharacter.EnableMarkup();
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnZoneCharacter.SetWindowText(KR_UIXAML_TIMEOD_ZONECHARACTER_FALSE);
	}
	else {
		m_btnZoneCharacter.SetWindowText(UIXAML_TIMEOD_ZONECHARACTER_FALSE);
	}
	m_btnZoneCharacter.EnableWindow(TRUE);

	// Define Purpose / Mode
	m_btnDefinePurposeMode.EnableMarkup();
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnDefinePurposeMode.SetWindowText(KR_UIXAML_TIMEOD_PURPOSENMODE_FALSE);
	}
	else {
		m_btnDefinePurposeMode.SetWindowText(UIXAML_TIMEOD_PURPOSENMODE_FALSE);
	}
	
	m_btnDefinePurposeMode.EnableWindow(TRUE);

	// OutputDesign
	m_btnOutputDesign.EnableMarkup();
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnOutputDesign.SetWindowText(KR_UIXAML_TIMEOD_OUTPUT_FALSE);
	}
	else {
		m_btnOutputDesign.SetWindowText(UIXAML_TIMEOD_OUTPUT_FALSE);
	} 

	m_btnOutputDesign.EnableWindow(FALSE);

	m_btnViewTable.EnableWindow(FALSE);

	SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 

	InitTimeComboData();

	if(KEMTimeODHourlyTotal == m_emTimeOD)
	{
		m_btnHourlyTotal.SetCheck(BST_CHECKED);
	}
	else
	{
		m_btnHourly.SetCheck(BST_CHECKED);
	}
	ODTimeControl();

	KRedrawWindow();
	TxLogDebugEndMsg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTimeODGenerationDlg::InitTimeComboData()
{
	for( int i= 0; i< 24; i++ )
	{
		int nInputValue = i;	    // 0 ~ 23

		CString strValue = _T("");
		strValue.Format(_T("%d"), nInputValue);

		if(nInputValue < 10)
		{
			strValue = _T("0") + strValue;
		}

		int nCurSel = m_cboFromTime.InsertString(i, strValue);
		m_cboFromTime.SetItemData(nCurSel, (DWORD_PTR)nInputValue);
	}
	m_cboFromTime.SetCurSel(0);

	for( int i= 0; i< 24; i++ )
	{
		int nInputValue = i + 1;	// 1 ~ 24

		CString strValue = _T("");
		strValue.Format(_T("%d"), nInputValue);

		if(nInputValue < 10)
		{
			strValue = _T("0") + strValue;
		}

		int nCurSel = m_cboToTime.InsertString(i, strValue);
		m_cboToTime.SetItemData(nCurSel, (DWORD_PTR)nInputValue);
	}
	m_cboToTime.SetCurSel(23);
}


void KTimeODGenerationDlg::InitPurposeData()
{
	for ( int i= 1; i<= PURPOSEDATA_COUNT; i++ )
	{
		TPurposeBase oTPurposeBase;

		KEMTimeODDefaultPurpose &emPurpose      = oTPurposeBase.emPurpose;
		CString                 &strPurposeName = oTPurposeBase.strPurposeName;

		switch(i)
		{
		case KEMTimeODDefaultPurposeOffice:
			emPurpose      = KEMTimeODDefaultPurposeOffice;
			strPurposeName = PURPOSE_NAME_OFFICE;
			break;

		case KEMTimeODDefaultPurposeSchool:
			emPurpose      = KEMTimeODDefaultPurposeSchool;
			strPurposeName = PURPOSE_NAME_SCHOOL;
			break;

		case KEMTimeODDefaultPurposeInstitute:
			emPurpose      = KEMTimeODDefaultPurposeInstitute;
			strPurposeName = PURPOSE_NAME_INSTITUTE;
			break;

		case KEMTimeODDefaultPurposeShop:
			emPurpose      = KEMTimeODDefaultPurposeShop;
			strPurposeName = PURPOSE_NAME_SHOP;
			break;

		case KEMTimeODDefaultPurposeHome:
			emPurpose      = KEMTimeODDefaultPurposeHome;
			strPurposeName = PURPOSE_NAME_HOME;
			break;

		case KEMTimeODDefaultPurposeBusiness:
			emPurpose      = KEMTimeODDefaultPurposeBusiness;
			strPurposeName = PURPOSE_NAME_BUSINESS;
			break;

		case KEMTimeODDefaultPurposeEtc:
			emPurpose      = KEMTimeODDefaultPurposeEtc;
			strPurposeName = PURPOSE_NAME_ETC;
			break;

		default:
			continue;
			break;
		}

		TSurveyPurpose oTSurveyPurpose;
		oTSurveyPurpose.oTPurpose = oTPurposeBase;

		m_vecSurveyPurpose.push_back(oTSurveyPurpose);
	}
}


void KTimeODGenerationDlg::InitModeData()
{
	for ( int i= 1; i<= MODEDATA_COUNT; i++ )
	{
		TModeBase         oTModeBase; 
		KEMTimeODDefaultMode &emMode      = oTModeBase.emMode;
		CString        &strFieldName      = oTModeBase.strModeName;

		switch(i)
		{
		case KEMInputModeAuto:
			emMode       = KEMInputModeAuto;
			strFieldName = MODENAME_AUTO_EN;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strFieldName = MODENAME_AUTO;
			}
			break;

		case KEMInputModeTaxi:
			emMode       = KEMInputModeTaxi;
			strFieldName = MODENAME_TAXI;
			break;

		case KEMInputModeBus:
			emMode       = KEMInputModeBus;
			strFieldName = MODENAME_BUS;
			break;

		case KEMInputModeRail:
			emMode       = KEMInputModeRail;
			strFieldName = MODENAME_RAIL;
			break;

		default:
			continue;
			break;
		}

		TInputModeInfo oTInputMode;
		oTInputMode.oTMode              = oTModeBase;
		oTInputMode.bSelected           = FALSE;
		oTInputMode.pModeODColumn       = NULL;
		oTInputMode.pParaODColumn       = NULL;

		m_vecInputModeInfo.push_back(oTInputMode);

		if(FALSE == oTInputMode.bSelected)
		{
			continue;
		}

		TSurveyMode oTSurveyMode;
		oTSurveyMode.oTMode = oTModeBase;

		m_vecSurveyMode.push_back(oTSurveyMode);
	}
}


void KTimeODGenerationDlg::InitZoneCharacterData()
{
	for( int i= 0; i< ZONE_CHARACTER_COUNT; i++ )
	{
		TZoneCharacterInfo oTZoneCharacter;
		oTZoneCharacter.bSelected       = FALSE;
		oTZoneCharacter.pSedColumn      = NULL;
		oTZoneCharacter.nClassNum       = KEMClassStartNum;

		m_vecZoneCharacterInfo.push_back(oTZoneCharacter);
	}
}


void KTimeODGenerationDlg::InitZoneCount()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT COUNT(*) AS Count FROM %s "), TABLE_ZONE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return;
	}

	while( spResult->Next() )
	{
		m_nZoneCount	= spResult->GetValueInt(0);
	}
}


void KTimeODGenerationDlg::OnRadioClicked(UINT nID)
{
	TxLogDebugStartMsg();
	switch(nID)
	{
	case IDC_RADIO1:	
		m_emTimeOD = KEMTimeODHourlyTotal;
		ODTimeControl();

		break;

	case IDC_RADIO2:    
		m_emTimeOD = KEMTimeODHourly;
		ODTimeControl();

		break;
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KTimeODGenerationDlg::ODTimeControl()
{
	CWnd* pPlaceholder = GetDlgItem(IDC_STATIC_SETTIME);
	CRect rc;
	pPlaceholder->GetWindowRect(&rc);
	ScreenToClient(&rc);
	int w = rc.Width() - 4;
	int h = rc.Height() - 4;

	CString xamlSettimeSelected;
	xamlSettimeSelected.Format
	(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("</Border>"),
		w, h
	);

	CString xamlSettimeFalse;
	xamlSettimeFalse.Format
	(
		_T("<Border Width='%d' Height='%d' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("</Border>"),
		w, h
	);

	if(KEMTimeODHourlyTotal == m_emTimeOD)
	{
		m_cboFromTime.EnableWindow(FALSE);
		m_cboToTime.EnableWindow(FALSE);
		m_cboFromTime.SetCurSel(-1);
		m_cboToTime.SetCurSel(-1);

		m_lblSetTime.SetMarkupText(xamlSettimeFalse);
	}
	else
	{
		m_cboFromTime.EnableWindow(TRUE);
		m_cboToTime.EnableWindow(TRUE);
		m_cboFromTime.SetCurSel(0);
		m_cboToTime.SetCurSel(23);

		m_lblSetTime.SetMarkupText(xamlSettimeSelected);
	}
}


void KTimeODGenerationDlg::OnBnClickedInput()
{
	TxLogDebugStartMsg();
	KTimeODInputDlg dlg;
	dlg.SetTarget(m_pTarget);
 	dlg.InputModeInfo(m_vecInputModeInfo);

	if(dlg.DoModal() == IDOK)
	{
//		clearMInfoList();
		
		m_vecInputModeInfo = dlg.InputModeInfo();

		m_vecSurveyMode.clear();
		m_vecOutputModeInfo.clear();
		

		// init Survey Mode
		for(size_t i= 0; i< m_vecInputModeInfo.size(); i++)
		{
			if(FALSE == m_vecInputModeInfo[i].bSelected)
			{
				continue;
			}
			
			TSurveyMode     oTSurveyMode;
			oTSurveyMode.oTMode     = m_vecInputModeInfo[i].oTMode;
						
			TOutputModeInfo oTOutputModeInfo;
			oTOutputModeInfo.oTMode = m_vecInputModeInfo[i].oTMode;
			oTOutputModeInfo.pModeODColumn = m_vecInputModeInfo[i].pModeODColumn;

			m_vecSurveyMode.push_back(oTSurveyMode);
			m_vecOutputModeInfo.push_back(oTOutputModeInfo);
		}

		/*std::set<KPurpose*>::iterator itPurpose, itEnd = m_SelectedPurposeSet.end();
		for(itPurpose = m_SelectedPurposeSet.begin(); itPurpose != itEnd; ++itPurpose)
		{
			KPurpose* pPurpose = *itPurpose;
			std::map<KPurpose*, KIOColumn*>::iterator itInput = m_InputMap.find(pPurpose);
			ASSERT(m_InputMap.end() != itInput);
			KIOColumn* pInputColumn = itInput->second;

			KModeChoiceInfo* pInfo = new KModeChoiceInfo;
			pInfo->Purpose(pPurpose);
			pInfo->InputColumn(pInputColumn);
			m_MInfoList.push_back(pInfo);
		}*/

		CString strBtnStatus(_T(""));
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnInput.SetWindowText(KR_UIXAML_TIMEOD_INPUT_SELECTED);
		}
		else {
			m_btnInput.SetWindowText(UIXAML_TIMEOD_INPUT_SELECTED);
		}
		
		m_btnZoneCharacter.GetWindowText(strBtnStatus);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if(strBtnStatus.Compare(KR_UIXAML_TIMEOD_ZONECHARACTER_FALSE) == 0)
			{
				m_btnZoneCharacter.SetWindowText(KR_UIXAML_TIMEOD_ZONECHARACTER_DEFAULT);
				m_btnZoneCharacter.EnableWindow(TRUE);
			}
		}
		else {
			if(strBtnStatus.Compare(UIXAML_TIMEOD_ZONECHARACTER_FALSE) == 0)
			{
				m_btnZoneCharacter.SetWindowText(UIXAML_TIMEOD_ZONECHARACTER_DEFAULT);
				m_btnZoneCharacter.EnableWindow(TRUE);
			}
		}
		
		m_btnDefinePurposeMode.GetWindowText(strBtnStatus);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if(strBtnStatus.Compare(KR_UIXAML_TIMEOD_PURPOSENMODE_FALSE) != 0)
			{
				m_btnDefinePurposeMode.SetWindowText(KR_UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
				m_btnDefinePurposeMode.EnableWindow(TRUE);
			}
		}
		else {
			if(strBtnStatus.Compare(UIXAML_TIMEOD_PURPOSENMODE_FALSE) != 0)
			{
				m_btnDefinePurposeMode.SetWindowText(UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
				m_btnDefinePurposeMode.EnableWindow(TRUE);
			}
		}

		m_btnOutputDesign.GetWindowText(strBtnStatus);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if(strBtnStatus.Compare(KR_UIXAML_TIMEOD_OUTPUT_FALSE) != 0)
			{
				m_btnOutputDesign.SetWindowText(KR_UIXAML_TIMEOD_OUTPUT_DEFAULT);
				m_btnOutputDesign.EnableWindow(TRUE);
			}
		}
		else {
			if(strBtnStatus.Compare(UIXAML_TIMEOD_OUTPUT_FALSE) != 0)
			{
				m_btnOutputDesign.SetWindowText(UIXAML_TIMEOD_OUTPUT_DEFAULT);
				m_btnOutputDesign.EnableWindow(TRUE);
			}
		}

		RunButtonControl();
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KTimeODGenerationDlg::OnBnClickedCharacterization()
{
	TxLogDebugStartMsg();
	KTimeODZoneCharacterDlg dlg;
 	dlg.SetTarget(m_pTarget);
 	dlg.ZoneCharacterInfo(m_vecZoneCharacterInfo);

	if(dlg.DoModal() == IDOK)
	{
//		clearMInfoList();
		m_vecZoneCharacterInfo = dlg.ZoneCharacterInfo();

		/*std::set<KPurpose*>::iterator itPurpose, itEnd = m_SelectedPurposeSet.end();
		for(itPurpose = m_SelectedPurposeSet.begin(); itPurpose != itEnd; ++itPurpose)
		{
			KPurpose* pPurpose = *itPurpose;
			std::map<KPurpose*, KIOColumn*>::iterator itInput = m_InputMap.find(pPurpose);
			ASSERT(m_InputMap.end() != itInput);
			KIOColumn* pInputColumn = itInput->second;

			KModeChoiceInfo* pInfo = new KModeChoiceInfo;
			pInfo->Purpose(pPurpose);
			pInfo->InputColumn(pInputColumn);
			m_MInfoList.push_back(pInfo);
		}*/

		CString strBtnStatus(_T(""));
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnZoneCharacter.SetWindowText(KR_UIXAML_TIMEOD_ZONECHARACTER_SELECTED);
		}
		else {
			m_btnZoneCharacter.SetWindowText(UIXAML_TIMEOD_ZONECHARACTER_SELECTED);
		}
				
		m_btnDefinePurposeMode.GetWindowText(strBtnStatus);
		if (KmzSystem::GetLanguage()==KEMKorea) {
			if(strBtnStatus.Compare(KR_UIXAML_TIMEOD_PURPOSENMODE_FALSE) == 0)
			{
				m_btnDefinePurposeMode.SetWindowText(KR_UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
				m_btnDefinePurposeMode.EnableWindow(TRUE);
			}
		}
		else {
			if(strBtnStatus.Compare(UIXAML_TIMEOD_PURPOSENMODE_FALSE) == 0)
			{
				m_btnDefinePurposeMode.SetWindowText(UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
				m_btnDefinePurposeMode.EnableWindow(TRUE);
			}
		}

		RunButtonControl();
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KTimeODGenerationDlg::OnBnClickedDefinePurposeNmode()
{
	TxLogDebugStartMsg();
	KTimeODDefPurposeModeDlg dlg;
	
	dlg.TotalPurposeSurveyCodes(m_setTotalPurpseSurveyCode);
	dlg.TotalModeSurveyCodes   (m_setTotalModeSurveyCode);
	dlg.PurposeSurvey(m_vecSurveyPurpose);
	dlg.ModeSurvey   (m_vecSurveyMode);
	
	if(dlg.DoModal() == IDOK)
	{
		m_vecSurveyPurpose         = dlg.PurposeSurvey();
		m_vecSurveyMode            = dlg.ModeSurvey();
		m_setTotalPurpseSurveyCode = dlg.TotalPurposeSurveyCodes();
		m_setTotalModeSurveyCode   = dlg.TotalModeSurveyCodes();

		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnDefinePurposeMode.SetWindowText(KR_UIXAML_TIMEOD_PURPOSENMODE_SELECTED);
			m_btnOutputDesign.SetWindowText(KR_UIXAML_TIMEOD_OUTPUT_DEFAULT);
		}
		else {
			m_btnDefinePurposeMode.SetWindowText(UIXAML_TIMEOD_PURPOSENMODE_SELECTED);
			m_btnOutputDesign.SetWindowText(UIXAML_TIMEOD_OUTPUT_DEFAULT);
		}
		m_btnOutputDesign.EnableWindow(TRUE);

		RunButtonControl();
	}
	else
	{
		m_setTotalPurpseSurveyCode = dlg.TotalPurposeSurveyCodes();
		m_setTotalModeSurveyCode   = dlg.TotalModeSurveyCodes();

		if( true == IsEmptySurveyCodeSet() )
		{
			CString strBtnStatus(_T(""));
			if (KmzSystem::GetLanguage()==KEMKorea) {
				m_btnDefinePurposeMode.SetWindowText(KR_UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
			}
			else {
				m_btnDefinePurposeMode.SetWindowText(UIXAML_TIMEOD_PURPOSENMODE_DEFAULT);
			}

			m_btnOutputDesign.GetWindowText(strBtnStatus);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				if(strBtnStatus.Compare(KR_UIXAML_TIMEOD_OUTPUT_SELECTED) == 0)
				{
					m_btnOutputDesign.SetWindowText(KR_UIXAML_TIMEOD_OUTPUT_DEFAULT);
					m_btnOutputDesign.EnableWindow(TRUE);
				}
			}
			else {
				if(strBtnStatus.Compare(UIXAML_TIMEOD_OUTPUT_SELECTED) == 0)
				{
					m_btnOutputDesign.SetWindowText(UIXAML_TIMEOD_OUTPUT_DEFAULT);
					m_btnOutputDesign.EnableWindow(TRUE);
				}
			}
		}

		RunButtonControl();
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}


bool KTimeODGenerationDlg::IsEmptySurveyCodeSet()
{
	for( size_t i= 0; i< m_vecSurveyPurpose.size(); i++ )
	{
		TSurveyPurpose   &oTSurveyPurpose = m_vecSurveyPurpose[i];
		KTDSurveyCodeSet &setSurveyCode   = oTSurveyPurpose.setPurposeSurveyCode;

		if( setSurveyCode.size()< 1 )
		{
			return true;
		}
	}

	for( size_t i= 0; i< m_vecSurveyMode.size(); i++ )
	{
		TSurveyMode      &oTSurveyMode  = m_vecSurveyMode[i];
		KTDSurveyCodeSet &setSurveyCode = oTSurveyMode.setModeSurveyCode;

		if( setSurveyCode.size()< 1 )
		{
			return true;
		}
	}

	return false;
}


void KTimeODGenerationDlg::OnBnClickedOutputdesign()
{
	TxLogDebugStartMsg();
	KTimeODOutputDesignDlg dlg;
	dlg.SetTarget(m_pTarget);
	dlg.OutputModeInfo( m_vecOutputModeInfo );

	if(dlg.DoModal() == IDOK)
	{
		m_vecOutputModeInfo = dlg.OutputModeInfo();
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnOutputDesign.SetWindowText(KR_UIXAML_TIMEOD_OUTPUT_SELECTED);
		}
		else {
			m_btnOutputDesign.SetWindowText(UIXAML_TIMEOD_OUTPUT_SELECTED);
		}

		RunButtonControl();
	}

	KRedrawWindow();
	TxLogDebugEndMsg();
}


HBRUSH KTimeODGenerationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT ID = pWnd->GetDlgCtrlID();
	if( ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_STATIC_FROM || ID == IDC_STATIC_TO )
	{        
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(0x00600912);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	return hbr;
}

void KTimeODGenerationDlg::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


void KTimeODGenerationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	KDialogEx::OnActivate(nState, pWndOther, bMinimized);
	KRedrawWindow();
}


void KTimeODGenerationDlg::OnBnClickedRun()
{
	TxLogDebugStartMsg();
	UpdateData(TRUE);

	if( KEMTimeODHourly == m_emTimeOD )
	{
		int nCur    = m_cboFromTime.GetCurSel();
		m_nFromTime = m_cboFromTime.GetItemData(nCur);

		nCur        = m_cboToTime.GetCurSel();
		m_nToTime   = m_cboToTime.GetItemData(nCur);

		if(m_nFromTime >= m_nToTime)
		{
			AfxMessageBox(_T("종료 시간을 시작 시간보다 큰 값으로 설정해 주세요."));
			m_cboToTime.SetFocus();
			return;
		}
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(TimeODThreadCaller, this, false, nLang);

	if( m_bRunTimeODSucceess )
	{
		m_btnViewTable.EnableWindow(TRUE);
	}
	
	KRedrawWindow();
	TxLogDebugEndMsg();
}


void KTimeODGenerationDlg::OnBnClickedViewTripRate()
{
	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(TIMEOD_TRIPRATE_FILENAME) )
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("%s 파일이 존재하지 않습니다."), TIMEOD_TRIPRATE_FILENAME);
		AfxMessageBox(strMsg);
		return;
	}

	ShellExecute(NULL, _T("open"), TIMEOD_TRIPRATE_FILENAME, NULL, NULL, SW_SHOW);    // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
}


unsigned __stdcall KTimeODGenerationDlg::TimeODThreadCaller(void* pParam)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("시간대별 O/D 생성 모형 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Time O/D Generation Model..."));
	}

	KTimeODGenerationDlg* pDlg = (KTimeODGenerationDlg*)pParameter->GetParameter();;

	m_bRunTimeODSucceess = pDlg->RunTimeODGeneration();

	return 0;

}


bool KTimeODGenerationDlg::RunTimeODGeneration()
{
	TxLogDebugStartMsg();

	if( !CreateArgument())
	{
		return false;
	}

// 	HINSTANCE hIns = LoadLibrary(_T("FTimeODGeneration.dll"));
// 	if( hIns == NULL )
// 	{
// 		AfxMessageBox(_T("FTimeODGeneration.dll이 없습니다."));
// 		return false;
// 	}
// 	
// 	TimeODProcPtr = (FTIMEODGENERATION)(GetProcAddress(hIns, "FTIMEODGENERATION"));
// 
// 	if( NULL == TimeODProcPtr )
// 	{
// 		AfxMessageBox(_T("FTIMEODGENERATION 함수가 없습니다."));
// 		FreeLibrary(hIns);
// 		return false;
// 	}
// 
// 	AddStatusMessage(_T("> Start ..."));
// 	int nResult = 0;
// 	try
// 	{        
// 		nResult = TimeODProcPtr((void*)this, (void*)ReceiveStatusMessageFromDll);
// 	}
// 	catch (...)
// 	{
// 		AfxMessageBox(_T("TimeOD 구동 중에 오류가 발생하였습니다. "));
// 		TxLogDebugException();
// 		FreeLibrary(hIns);
// 		return false;
// 	}

	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("시간대별 O/D 생성 모형 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Time O/D Generation Model..."));
	}

	KLauncher::Run(strWindowName, 11);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("시간대별 O/D 생성 모형 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Time O/D Generation Model Complete."));
		}
		TxLogDebug(_T("Calculate statistics Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모형 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Model Stop.."));

		return false;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return false;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모형 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Analysis Time OD Run Error.."));
		return false;
	}

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}
	if (! ReadOutPutFile() )
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data Failed."));
		}
		TxLogDebug(_T("Output TimeOD 정보 입력 실패..."));

		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KTimeODGenerationDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	try
	{
		std::ifstream ifs( T_DAT_OUT_TIMEOD, std::ios::binary );    

		KODKeyDoubleRecords oODResult(m_vecOutputModeInfo.size());

		KIOTables*   pIOTables = m_pTarget->Tables();
		KIOTable* pTable = pIOTables->FindTable(TABLE_MODE_OD);
		if(NULL == pTable)
		{
			return false;
		}

		const KIOColumns* pModeODColumns = pTable->Columns();
		KIOColumns        oModeODColumns;
		
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert TimeOD result"));
			int nCount = 0;
			
			KIOColumn* pOutputColumnMode = NULL;
			// column 추가
			for(size_t i= 0; i<  m_vecOutputModeInfo.size(); i++)
			{
				TOutputModeInfo &oTModeInfo = m_vecOutputModeInfo[i];
				
				KIOColumn column;

				column.Name(pTable->GetUsableColumnName());
				column.Caption(oTModeInfo.strOutputName);
				column.CodeType(KEMIOCodeTypeIsNothing);
				column.ColumnType(KEMIOColumnTypeUserDefine);
				column.DataType(KEMIODataTypeDouble);
				column.DisplayOrder(pModeODColumns->ColumnCount()+1);
				column.PrimaryKey(false);
				column.Visible(true);

				pOutputColumnMode = pTable->AddColumn(column);
				if(NULL == pOutputColumnMode)
				{
					AfxMessageBox(_T("컬럼 추가에 실패하였습니다."));
					return false;
				}
				pModeODColumns = pTable->Columns();

				oModeODColumns.AddNewColumn(pOutputColumnMode);
			}
			// 추가한 컬럼에 데이터 insert
			while(! ifs.eof())
			{
				if (ifs.eof())
				{
					break;
				}

				Integer nxOrigin;
                ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );
				
				Integer nxDestination;
				ifs.read( reinterpret_cast<char*>( &nxDestination),        sizeof(Integer)  );

				KODKey oODKey(nxOrigin, nxDestination);
				KSDoubleRecord* pOutPutResult = oODResult.AddRecord(oODKey);

				for(size_t i= 0; i<  m_vecOutputModeInfo.size(); i++)
				{
					double dRecord = 0.0;
					ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );
					pOutPutResult->SetAt(i, dRecord);
				}
				nCount++;
			}
			CString strResult = _T("");
			strResult.Format(_T("--- end insert TimeOD result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			ifs.close();
			TxLogDebugException();
			return false;
		}

		KBulkDBase::BulkODUpsert(pTable, oModeODColumns, oODResult);
		ifs.close();
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


bool KTimeODGenerationDlg::CreateArgument()
{
	TxLogDebugStartMsg();
	ImChampDir::SetCurrentDirectoryToApp();
	if(! ClearFiles())
	{
		return false;
	}
	//TxLogDebug("END CLEAR FILE ------ ");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_TIMEOD, nOpenMode);

	bool bResult = false;

	// .arg 작성
	bResult = ArgumentWriteCaluateType(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteCaluateType) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWriteModeODParameter(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteModeODParameter) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
	bResult = ArgumentWriteZoneCharacter(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteZoneCharacter) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWritePurpose(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWritePurpose) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWriteMode(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteMode) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWriteZone(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteZone) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	// SED.dat 생성
	KIOColumns oSedVectorColumns;
	for( size_t i= 0; i< m_vecZoneCharacterInfo.size(); i++ )
	{
		TZoneCharacterInfo &oTZoneCharacterInfo = m_vecZoneCharacterInfo[i];

		if(FALSE == oTZoneCharacterInfo.bSelected)
		{
			continue;
		}

		KIOColumn*          pSedColumn          = oTZoneCharacterInfo.pSedColumn;

		oSedVectorColumns.AddNewColumn(pSedColumn);
	}

	bResult = KBulkDBase2File::ExportDB2SedVecFile(m_pTarget->GetDBaseConnection(), oSedVectorColumns, T_DAT_IN_SED);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2SedVecFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// MODEOD.dat 생성
	KIOColumns oModeODColumns;
	KIOColumns oParaODColumns;
	for( size_t i= 0; i< m_vecInputModeInfo.size(); i++ )
	{
		TInputModeInfo &oTInputModeInfo = m_vecInputModeInfo[i];
		
		if(FALSE == oTInputModeInfo.bSelected)
		{
			continue;
		}

		KIOColumn*      pModeODColumn   = oTInputModeInfo.pModeODColumn;
		KIOColumn*      pParaODColumn   = oTInputModeInfo.pParaODColumn;

		oModeODColumns.AddNewColumn(pModeODColumn);
		oParaODColumns.AddNewColumn(pParaODColumn);
	}
	
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_MODE_OD, oModeODColumns, T_DAT_IN_MODEOD);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// ODParameter.dat 생성
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, oParaODColumns, T_DAT_IN_ODPARAMETER);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KTimeODGenerationDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove(ARG_TIMEOD);
		nResult = remove(DAT_IN_SED);
		nResult = remove(DAT_IN_MODEOD);
		nResult = remove(DAT_IN_ODPARAMETER);

		// 결과 파일 삭제 
		nResult = remove(DAT_OUT_TIMEOD);
		nResult = remove(TXT_OUT_TRIPRATE);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KTimeODGenerationDlg::ArgumentWriteCaluateType(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		strOut.Format(_T("CalculateType_FromTime_ToTime\t%d\t%d\t%d\r\n"), m_emTimeOD, m_nFromTime, m_nToTime);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTimeODGenerationDlg::ArgumentWriteModeODParameter(CStdioFileEx& of)
{
	std::vector<TInputModeInfo> vecInputModeInfo;

	for( size_t i= 0; i< m_vecInputModeInfo.size(); i++ )
	{
		TInputModeInfo &oTInputModeInfo = m_vecInputModeInfo[i];
		if( FALSE == oTInputModeInfo.bSelected )
		{
			continue;
		}
		vecInputModeInfo.push_back(oTInputModeInfo);
	}

	CString strOut = _T("");
	
	try
	{
		size_t nInfoCount = vecInputModeInfo.size();
		strOut.Format( _T("nMode_ODParameter\t%d\r\n"), nInfoCount );
		of.WriteString(strOut);

		for( size_t i= 0; i< nInfoCount; i++ )
		{
			TInputModeInfo &oTInputModeInfo = vecInputModeInfo[i];
			/*strOut.Format(_T("%d\r\n"), oTInputModeInfo.oTMode.emMode); 2012-06-06 전용 ID 에서 index 형식으로 변경*/
			strOut.Format(_T("%d\r\n"), (i+1)); // 1 ~ n
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTimeODGenerationDlg::ArgumentWriteZoneCharacter(CStdioFileEx& of)
{
	std::vector<TZoneCharacterInfo> vecZoneCharacterInfo;

	for( size_t i= 0; i< m_vecZoneCharacterInfo.size(); i++ )
	{
		TZoneCharacterInfo &oTZoneCharacterInfo = m_vecZoneCharacterInfo[i];
		if( FALSE == oTZoneCharacterInfo.bSelected )
		{
			continue;
		}
		vecZoneCharacterInfo.push_back(oTZoneCharacterInfo);
	}

	CString strOut = _T("");

	try
	{
		size_t nInfoCount = vecZoneCharacterInfo.size();
		strOut.Format( _T("SEDClassNo_nCharacter\t%d\r\n"), nInfoCount );
		of.WriteString(strOut);

		for( size_t i= 0; i< nInfoCount; i++ )
		{
			TZoneCharacterInfo &oTZoneCharacterInfo = vecZoneCharacterInfo[i];
			strOut.Format(_T("%d\t%d\r\n"), (i+1),    oTZoneCharacterInfo.nClassNum);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTimeODGenerationDlg::ArgumentWritePurpose(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		size_t nInfoCount = m_vecSurveyPurpose.size();
		strOut.Format( _T("Purpose\t%d\r\n"), nInfoCount );
		of.WriteString(strOut);

		for( size_t i= 0; i< nInfoCount; i++ )
		{
			CString strPurposeID(_T(""));
			CString strCodeCount(_T(""));
			CString strCodes(_T(""));

			TSurveyPurpose   &oTSurveyPurpose = m_vecSurveyPurpose[i];
			KTDSurveyCodeSet &setSurveyCode   = oTSurveyPurpose.setPurposeSurveyCode;
			
			/*strPurposeID.Format(_T("%d"),       oTSurveyPurpose.oTPurpose.emPurpose); 2012-06-06 전용 ID 에서 index 형식으로 변경*/
			strPurposeID.Format(_T("%d"),       (i+1)); // 1 ~ n
			strCodeCount.Format(_T("%d"),    setSurveyCode.size());

			KTDSurveyCodeSet::iterator iter = setSurveyCode.begin();
			KTDSurveyCodeSet::iterator end  = setSurveyCode.end();
			while(iter != end)
			{
				int     nCode = *iter;
				CString strCode(_T(""));

				strCode.Format(_T("%d\t"), nCode);

				strCodes += strCode;

				++iter;
			}

			strOut.Format(_T("%s\t%s\t%s\r\n"), strPurposeID, strCodeCount, strCodes);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTimeODGenerationDlg::ArgumentWriteMode(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		size_t nInfoCount = m_vecSurveyMode.size();
		strOut.Format( _T("Mode\t%d\r\n"), nInfoCount );
		of.WriteString(strOut);

		for( size_t i= 0; i< nInfoCount; i++ )
		{
			CString strModeID(_T(""));
			CString strCodeCount(_T(""));
			CString strCodes(_T(""));

			TSurveyMode      &oTSurveyMode    = m_vecSurveyMode[i];
			KTDSurveyCodeSet &setSurveyCode   = oTSurveyMode.setModeSurveyCode;

			/*strModeID.Format(   _T("%d"),       oTSurveyMode.oTMode.emMode); 2012-06-06 전용 ID 에서 index 형식으로 변경*/
			strModeID.Format(   _T("%d"),       (i+1)); // i ~ n
			strCodeCount.Format(_T("%d"),       setSurveyCode.size());

			KTDSurveyCodeSet::iterator iter = setSurveyCode.begin();
			KTDSurveyCodeSet::iterator end  = setSurveyCode.end();
			while(iter != end)
			{
				int     nCode = *iter;
				CString strCode(_T(""));

				strCode.Format(_T("%d\t"), nCode);

				strCodes += strCode;

				++iter;
			}

			strOut.Format(_T("%s\t%s\t%s\r\n"), strModeID, strCodeCount, strCodes);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTimeODGenerationDlg::ArgumentWriteZone(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		strOut.Format( _T("nZone\t%d\r\n"), m_nZoneCount );
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


void KTimeODGenerationDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KTimeODGenerationDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


void KTimeODGenerationDlg::OnBnClickedClose()
{
	OnCancel();
}


void KTimeODGenerationDlg::RunButtonControl()
{
	UpdateData(TRUE);

	CString strButtonState = _T(""); 
	BOOL bButtonRun = TRUE;

	m_btnInput.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_TIMEOD_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_TIMEOD_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnZoneCharacter.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_TIMEOD_ZONECHARACTER_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_TIMEOD_ZONECHARACTER_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnDefinePurposeMode.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_TIMEOD_PURPOSENMODE_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_TIMEOD_PURPOSENMODE_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnOutputDesign.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_TIMEOD_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_TIMEOD_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(bButtonRun);
}

BOOL KTimeODGenerationDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
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
