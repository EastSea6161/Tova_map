// DefaultInterPathGenerator.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultInterPathGenerator.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseDefaultParaInterPathGenerator.h"
#include "DefineNamesModels.h"
#include "DBaseDefaultParaHighwayAssign.h"

enum PathInfo
{
	_0_COLUMN_MODE = 0,
	_1_COLUMN_PATH
};

enum AutoCost
{
	_0_COLUMN_AUTO = 0,
	_1_COLUMN_PRICE,
	_2_COLUMN_RATIO,
	_3_COLUMN_FUEL
};

enum UrbanTransit
{
	_0_COLUMN_TAXI = 0,
	_1_COLUMN_BASEFAIR,
	_2_COLUMN_BASEDIST,
	_3_COLUMN_DISTBASEFARE,
	_4_COLUMN_UNITDIST
};

enum Terminal
{
	_1_COLUMN_ACCESS = 1,
	_2_COLUMN_EGRESS,
	_3_COLUMN_MODETRANSFER
};

//const int nREGION_PATH_AE   = 999;

// KDefaultInterPathGenerator 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultInterPathGenerator, KResizeDialogEx)

KDefaultInterPathGenerator::KDefaultInterPathGenerator(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultInterPathGenerator::IDD, pParent)
	, m_pTarget(a_pTarget)
	,m_strAddAccegr(_T("AccEgrPath"))
	,m_steTooltipAceeEgr(_T("AccessEgressPath"))
{
    if (KmzSystem::GetLanguage() == KEMKorea) {
        m_strAddAccegr = _T("출발/도착 접근경로");
    }
}

KDefaultInterPathGenerator::~KDefaultInterPathGenerator()
{
}

void KDefaultInterPathGenerator::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT_URBAN,      m_wndUrbanPathReport);
	DDX_Control(pDX, IDC_REPORT_URBAN_COST, m_wndUrbanAutoReport);
	DDX_Control(pDX, IDC_REPORT_TRANSIT,    m_wndUrbanTransitReport);
	DDX_Control(pDX, IDC_REPORT_REGIONAL,   m_wndRegionalPathReport);
	DDX_Control(pDX, IDC_REPORT_COST,       m_wndRegionalAutoReport);
	DDX_Control(pDX, IDC_REPORT_TERMINAL,   m_wndRegionalTerminalReport);
}


BEGIN_MESSAGE_MAP(KDefaultInterPathGenerator, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KDefaultInterPathGenerator::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultInterPathGenerator::OnBnClickedCancel)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultInterPathGenerator::OnBnClickedDefault)
END_MESSAGE_MAP()


// KDefaultInterPathGenerator 메시지 처리기입니다.


BOOL KDefaultInterPathGenerator::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndUrbanPathReport,		 TRUE);
	KReportCtrlSetting::Default(m_wndUrbanAutoReport,		 TRUE);
	KReportCtrlSetting::Default(m_wndUrbanTransitReport,	 TRUE);
	KReportCtrlSetting::Default(m_wndRegionalPathReport,     TRUE);
	KReportCtrlSetting::Default(m_wndRegionalAutoReport,	 TRUE);
	KReportCtrlSetting::Default(m_wndRegionalTerminalReport, TRUE);

 	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathSetting(m_mapRegionalPathTimeSetting);
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanPathInfo(m_mapUrbanPathInfo);
 	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanAutoCost(m_mapUrbanAutoCost);
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanTransitCost(m_mapUrbanTransitCost);
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathInfo(m_mapRegionalPathInfo);
 	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalAutoCost(m_mapRegionalAutoCost);
 	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalTerminalTime(m_mapRegionalTerminalTime);

	ResizeComponent();

	InitUrbanPathReportHeader();
	InitUrbanAutoReportHeader();
	InitUrbanTransitReportHeader();
	InitRegionalPathReportHeader();
	InitRegionalAutoReportHeader();
	InitRegionalTerminalReportHeader();

	AutoType iterDefaultTimeSetting = m_mapRegionalPathTimeSetting.begin();
	AutoType endDefaultTimeSetting  = m_mapRegionalPathTimeSetting.end();
	if(iterDefaultTimeSetting != endDefaultTimeSetting)
	{
		AutoType iterUserTimeSetting = m_mapRegionalPathTimeSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserTimeSetting != endDefaultTimeSetting)
		{
			InitRegionalTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitRegionalTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetRegionalTimeSetting();
		SaveRegionalTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterDefaultUrbanPath = m_mapUrbanPathInfo.begin();
	AutoType endDefaultUrbanPath  = m_mapUrbanPathInfo.end();
	if(iterDefaultUrbanPath != endDefaultUrbanPath)
	{
		AutoType iterUserUrbanPath = m_mapUrbanPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserUrbanPath != endDefaultUrbanPath)
		{
			InitUrbanPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitUrbanPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetUrbanPathReportRecord();
		SaveUrbanPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterDefaultUrbanAuto = m_mapUrbanAutoCost.begin();
	AutoType endDefaultUrbanAuto  = m_mapUrbanAutoCost.end();
	if(iterDefaultUrbanAuto != endDefaultUrbanAuto)
	{
		AutoType iterUserUrbanAuto = m_mapUrbanAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserUrbanAuto != endDefaultUrbanAuto)
		{
			InitUrbanAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitUrbanAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetUrbanAutoReportRecord();
		SaveUrbanAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterDefaultUrbanTransit = m_mapUrbanTransitCost.begin();
	AutoType endDefaultUrbanTransit  = m_mapUrbanTransitCost.end();
	if(iterDefaultUrbanTransit != endDefaultUrbanTransit)
	{
		AutoType iterUserUrbanTransit = m_mapUrbanTransitCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserUrbanTransit != endDefaultUrbanTransit)
		{
			InitUrbanTransitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitUrbanTransitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
 		SetUrbanTransitReportRecord();
 		SaveUrbanTransiReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterDefaultRegionalPathInfo = m_mapRegionalPathInfo.begin();
	AutoType endDefaultRegionalPathInfo  = m_mapRegionalPathInfo.end();
	if(iterDefaultRegionalPathInfo != endDefaultRegionalPathInfo)
	{
		AutoType iterUserRegionalAuto = m_mapRegionalPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserRegionalAuto != endDefaultRegionalPathInfo)
		{
			InitRegionalPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitRegionalPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetRegionalPathReportRecord();
		SaveRegionalPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterDefaultRegionalAuto = m_mapRegionalAutoCost.begin();
	AutoType endDefaultRegionalAuto  = m_mapRegionalAutoCost.end();
	if(iterDefaultRegionalAuto != endDefaultRegionalAuto)
	{
		AutoType iterUserRegionalAuto = m_mapRegionalAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserRegionalAuto != endDefaultRegionalAuto)
		{
			InitRegionalAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitRegionalAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetRegionalAutoReportRecord();
		SaveRegionalAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}


	AutoType iterDefaultRegionalTerminal = m_mapRegionalTerminalTime.begin();
	AutoType endDefaultRegionalTerminal  = m_mapRegionalTerminalTime.end();
	if(iterDefaultRegionalTerminal != endDefaultRegionalTerminal)
	{
		AutoType iterUserRegionalTerminal = m_mapRegionalTerminalTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserRegionalTerminal != endDefaultRegionalTerminal)
		{
			InitRegionalTerminalReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitRegionalTerminalReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetRegionalTerminalReportRecord();
		SaveRegionalTerminalReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}
	return TRUE; 
}

void KDefaultInterPathGenerator::ResizeComponent()
{
	SetResize(IDC_REPORT_URBAN,	     SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_REPORT_URBAN_COST, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TRANSIT,	 SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC1717,	     SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_REGIONAL,	 SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_REPORT_COST,	     SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TERMINAL,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC1718,	     SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR7,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,					 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,		         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDDEFAULT,		     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC10,		     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC11,		     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC33,		     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC24,		     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_TAXI,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_RAIL,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_TRANSIT,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_VALTIME,    SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_VALTIME,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_STATIC1719,	     SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
}

void KDefaultInterPathGenerator::InitUrbanPathReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndUrbanPathReport.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Mode"), 55));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("수단명"));
	}

	pColumn = m_wndUrbanPathReport.AddColumn(new CXTPReportColumn(_1_COLUMN_PATH, _T("Max.# of Path"), 45));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("탐색 경로 개수"));
	}
}

void KDefaultInterPathGenerator::InitUrbanAutoReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndUrbanAutoReport.AddColumn(new CXTPReportColumn(_0_COLUMN_AUTO, _T("Fuel Type"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("연료종류"));
	}

	pColumn = m_wndUrbanAutoReport.AddColumn(new CXTPReportColumn(_1_COLUMN_PRICE, _T("Price(ℓ)"), 25));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("가격(￦/ℓ)"));
	}

	pColumn = m_wndUrbanAutoReport.AddColumn(new CXTPReportColumn(_2_COLUMN_RATIO, _T("Ratio"), 25));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("구성비"));
	}

	pColumn = m_wndUrbanAutoReport.AddColumn(new CXTPReportColumn(_3_COLUMN_FUEL, _T("Fuel Efficiency(Km/ℓ)"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("연비(Km/ℓ)"));
	}
}

void KDefaultInterPathGenerator::InitUrbanTransitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndUrbanTransitReport.AddColumn(new CXTPReportColumn(_0_COLUMN_TAXI, _T("Fuel Type"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("연료종류"));
	}

	pColumn = m_wndUrbanTransitReport.AddColumn(new CXTPReportColumn(_1_COLUMN_BASEFAIR, _T("Base Fare"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("기본 운임"));
	}

	pColumn = m_wndUrbanTransitReport.AddColumn(new CXTPReportColumn(_2_COLUMN_BASEDIST, _T("Base Dist(㎞)"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("~Km까지"));
	}

	pColumn = m_wndUrbanTransitReport.AddColumn(new CXTPReportColumn(_3_COLUMN_DISTBASEFARE, _T("Dist-Base Fare"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("추가 운임"));
	}

	pColumn = m_wndUrbanTransitReport.AddColumn(new CXTPReportColumn(_4_COLUMN_UNITDIST, _T("Unit Dist"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("매 ~Km마다"));
	}
}

void KDefaultInterPathGenerator::InitRegionalPathReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndRegionalPathReport.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Mode"), 50));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("수단명"));
	}

	pColumn = m_wndRegionalPathReport.AddColumn(new CXTPReportColumn(_1_COLUMN_BASEFAIR, _T("Max.# of Path"), 50));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("탐색 경로 개수"));
	}
}

void KDefaultInterPathGenerator::InitRegionalAutoReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndRegionalAutoReport.AddColumn(new CXTPReportColumn(_0_COLUMN_AUTO, _T("Fuel Type"), 20));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("연료종류"));
	}

	pColumn = m_wndRegionalAutoReport.AddColumn(new CXTPReportColumn(_1_COLUMN_PRICE, _T("Price(ℓ)"), 25));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("가격(￦/ℓ)"));
	}

	pColumn = m_wndRegionalAutoReport.AddColumn(new CXTPReportColumn(_2_COLUMN_RATIO, _T("Ratio"), 25));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("구성비"));
	}

	pColumn = m_wndRegionalAutoReport.AddColumn(new CXTPReportColumn(_3_COLUMN_FUEL, _T("Fuel Efficiency(Km/ℓ)"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("연비(Km/ℓ)"));
	}
}

void KDefaultInterPathGenerator::InitRegionalTerminalReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndRegionalTerminalReport.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Mode"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("수단명"));
	}

	pColumn = m_wndRegionalTerminalReport.AddColumn(new CXTPReportColumn(_1_COLUMN_ACCESS, _T("Access"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("탑승수속시간"));
	}

	pColumn = m_wndRegionalTerminalReport.AddColumn(new CXTPReportColumn(_2_COLUMN_EGRESS, _T("Egress"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("하차시간"));
	}

	pColumn = m_wndRegionalTerminalReport.AddColumn(new CXTPReportColumn(_3_COLUMN_MODETRANSFER, _T("Mode Transfer"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("환승 가능 여부"));
	}
}

void KDefaultInterPathGenerator::InitRegionalTimeSetting(int a_nMasterCode)
{
	try
	{
		AutoType iterDefault = m_mapRegionalPathTimeSetting.find(a_nMasterCode);
		AutoType endDefault  = m_mapRegionalPathTimeSetting.end();
		if(iterDefault != endDefault)
		{
			KPathTimeSetting oPathTimeSetting = iterDefault->second;

			double dTaxitime    = oPathTimeSetting.dTaxiWaitingTime;
			double dHighwayTime = oPathTimeSetting.dHighwayTransitTime;
			double dRailTime    = oPathTimeSetting.dRailTransitTime;
			double dValueofTime = oPathTimeSetting.dInterValueofTime;

			CString strValue(_T(""));
			CEdit* pEdit;
			strValue.Format(_T("%.2f"), dTaxitime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXI);
			SetDlgItemText(IDC_EDIT_TAXI, strValue);

			strValue.Format(_T("%.2f"), dHighwayTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRANSIT);
			SetDlgItemText(IDC_EDIT_TRANSIT, strValue);

			strValue.Format(_T("%.2f"), dRailTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RAIL);
			SetDlgItemText(IDC_EDIT_RAIL, strValue);

			strValue.Format(_T("%.1f"), dValueofTime);
			GetDlgItem(IDC_EDIT_VALTIME)->SetWindowText(strValue);
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

void KDefaultInterPathGenerator::InitUrbanPathReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndUrbanPathReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord	= nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		AutoType iterDefault = m_mapUrbanPathInfo.find(a_nMasterCode);
		AutoType endDefault  = m_mapUrbanPathInfo.end();
		if(iterDefault != endDefault)
		{
			std::vector<TUrbanPathInfo>& vecTurbanPathInfo = iterDefault->second;
			size_t nPathCount = vecTurbanPathInfo.size();
			for(size_t i = 0; i < nPathCount; i++)
			{
				TUrbanPathInfo oTurbanPathInfo = vecTurbanPathInfo[i];
				CString strMode = oTurbanPathInfo.strMode;
				int nMaxPath = oTurbanPathInfo.nMaxofPath;

				pRecord = m_wndUrbanPathReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(FALSE);
				pItem->SetItemData((DWORD_PTR)oTurbanPathInfo.nMode);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nMaxPath));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(TRUE);
			}
			m_wndUrbanPathReport.Populate();
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

void KDefaultInterPathGenerator::InitUrbanAutoReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndUrbanAutoReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord = nullptr;
		CXTPReportRecordItem*	pItem   = nullptr;

		AutoType iterDefault = m_mapUrbanAutoCost.find(a_nMasterCode);
		AutoType endDefault  = m_mapUrbanAutoCost.end();
		if(iterDefault != endDefault)
		{
			std::vector<TInterAutoCost>& vecTInterAutoCost = iterDefault->second;
			size_t nAutoCount = vecTInterAutoCost.size();
			for(size_t i = 0; i < nAutoCount; i++)
			{
				TInterAutoCost oTInterAutoCost = vecTInterAutoCost[i];
				CString strAuto = oTInterAutoCost.strName;
				double  dPrice  = oTInterAutoCost.dPrice;
				double  dRatio  = oTInterAutoCost.dRation;
				double  dFuel   = oTInterAutoCost.dfule;

				pRecord = m_wndUrbanAutoReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAuto));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPrice, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRatio, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dFuel, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);
			}
			m_wndUrbanAutoReport.Populate();
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

void KDefaultInterPathGenerator::InitUrbanTransitReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndUrbanTransitReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord	= nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		AutoType iterDefault = m_mapUrbanTransitCost.find(a_nMasterCode);
		AutoType endDefault  = m_mapUrbanTransitCost.end();
		if(iterDefault != endDefault)
		{
			std::vector<TInterTaxiCost>& vecTInterTaxiCost = iterDefault->second;
			size_t nPathCount = vecTInterTaxiCost.size();
			for(size_t i = 0; i < nPathCount; i++)
			{
				TInterTaxiCost oTInterTaxiCost = vecTInterTaxiCost[i];
				CString strTaxi                = oTInterTaxiCost.strName;
				double  dBaseFare              = oTInterTaxiCost.dBasefare;
				double  dBaseDist              = oTInterTaxiCost.dBaseDist;
				double  dDistBaseFare          = oTInterTaxiCost.dAddFare;
				double  dUnitDist              = oTInterTaxiCost.dAddDist;

				pRecord = m_wndUrbanTransitReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTaxi));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBaseFare, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBaseDist, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dDistBaseFare, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dUnitDist, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);
			}
			m_wndUrbanTransitReport.Populate();
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

void KDefaultInterPathGenerator::InitRegionalPathReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndRegionalPathReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndRegionalPathReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		CString	strDefineModeName = _T(""); 
		CString strMode           = _T("");
		int    nDefineMaxPath     = 0;
		int    nAccEgrMaxPath     = 0;

		KDBaseDefaultParaInterPathGenerator::SetRegionalModeInfo(m_pTarget, m_mapRegionalModeInfo);

		AutoType iterCheckMode = m_mapRegionalModeInfo.begin();
		AutoType endCheckMode  = m_mapRegionalModeInfo.end();
		if(iterCheckMode != endCheckMode)
		{
			size_t nModeCount = m_mapRegionalModeInfo.size();
			for(size_t a = 0; a < nModeCount; a++)
			{
				CString strChecKModeName = iterCheckMode->second;

				AutoType iterDefault = m_mapRegionalPathInfo.find(a_nMasterCode);
				AutoType endDefault  = m_mapRegionalPathInfo.end();
				if(iterDefault != endDefault)
				{
					std::vector<TRegionPathInfo>& vecTRegionPathInfo = iterDefault->second;
					size_t nAutoCount = vecTRegionPathInfo.size();
					for(size_t i = 0; i < nAutoCount; i++)
					{
						TRegionPathInfo oTRegionPathInfo = vecTRegionPathInfo[i];
						CString strMode = oTRegionPathInfo.strMode;
						int    nMaxPath = oTRegionPathInfo.nMaxofPath;

						if(strChecKModeName == strMode)
						{
							strDefineModeName = strMode;
							nDefineMaxPath    = nMaxPath;
						}
						if(m_strAddAccegr == strMode)
						{
							nAccEgrMaxPath = nMaxPath;
						}
					}
				}
				if(strDefineModeName == strChecKModeName)
				{
					pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strChecKModeName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nDefineMaxPath));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);
				}
				else
				{
					nDefineMaxPath = 5;

					pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strChecKModeName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nDefineMaxPath));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);
				}
				iterCheckMode++;
			}
		}
			pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (m_strAddAccegr));
			pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(FALSE);
			pItem ->SetTooltip(m_steTooltipAceeEgr);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("출발/도착 접근경로"));
				pItem ->SetTooltip(_T("출발/도착 접근경로"));
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nAccEgrMaxPath));
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(TRUE);

			m_wndRegionalPathReport.Populate();
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

// void KDefaultInterPathGenerator::InitRegionalPathReportRecord(int a_nMasterCode)
// {
// 	try
// 	{
// 		m_wndRegionalPathReport.ResetContent(TRUE);
// 
// 		CXTPReportRecords*		pRecords		 = m_wndRegionalPathReport.GetRecords();
// 		CXTPReportRecord*		pRecord			 = nullptr;
// 		CXTPReportRecordItem*	pItem			 = nullptr;
// 
// 		AutoType iterDefault = m_mapRegionalPathInfo.find(a_nMasterCode);
// 		AutoType endDefault  = m_mapRegionalPathInfo.end();
// 		if(iterDefault != endDefault)
// 		{
// 			std::vector<TRegionPathInfo>& vecTRegionPathInfo = iterDefault->second;
// 			size_t nAutoCount = vecTRegionPathInfo.size();
// 			for(size_t i = 0; i < nAutoCount; i++)
// 			{
// 				TRegionPathInfo oTRegionPathInfo = vecTRegionPathInfo[i];
// 				CString strMode = oTRegionPathInfo.strMode;
// 				int    nMaxPath = oTRegionPathInfo.nMaxofPath;
// 
// 
// 				pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());
// 
// 				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
// 				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
// 				pItem ->SetAlignment(xtpColumnTextCenter);
// 				pItem ->SetEditable(FALSE);
// 
// 				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nMaxPath));
// 				pItem ->SetAlignment(xtpColumnTextCenter);
// 				pItem ->SetEditable(TRUE);
// 			}
// 			m_wndRegionalPathReport.Populate();
// 		}
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebug(ex->GetErrorMessage());
// 	}
// 	catch(...)
// 	{
// 		TxLogDebugException();
// 	}
// }

void KDefaultInterPathGenerator::InitRegionalAutoReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndRegionalAutoReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords = m_wndRegionalAutoReport.GetRecords();
		CXTPReportRecord*		pRecord	 = nullptr;
		CXTPReportRecordItem*	pItem	 = nullptr;

		AutoType iterDefault = m_mapRegionalAutoCost.find(a_nMasterCode);
		AutoType endDefault  = m_mapRegionalAutoCost.end();
		if(iterDefault != endDefault)
		{
			std::vector<TInterAutoCost>& vecTInterAutoCost = iterDefault->second;
			size_t nAutoCount = vecTInterAutoCost.size();
			for(size_t i = 0; i < nAutoCount; i++)
			{
				TInterAutoCost oTInterAutoCost = vecTInterAutoCost[i];
				CString strAuto = oTInterAutoCost.strName;
				double  dPrice  = oTInterAutoCost.dPrice;
				double  dRatio  = oTInterAutoCost.dRation;
				double  dFuel   = oTInterAutoCost.dfule;

				pRecord = m_wndRegionalAutoReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAuto));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPrice, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRatio, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dFuel, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);
			}
			m_wndRegionalAutoReport.Populate();
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

void KDefaultInterPathGenerator::InitRegionalTerminalReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndRegionalTerminalReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndRegionalTerminalReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		CString strCheckModeName  = _T("");
		CString strDefineModeName = _T("");
		double  dDefineAccess     = (0.0);
		double  dDefineEgress     = (0.0);
		int		nDefModeTransfer  = 0;

		KDBaseDefaultParaInterPathGenerator::SetRegionalTerminalTime(m_pTarget, m_mapSetRegionalTerminalTime);

		AutoType iterCheckTerminal = m_mapSetRegionalTerminalTime.begin();
		AutoType endCheckTerminal  = m_mapSetRegionalTerminalTime.end();
		if(iterCheckTerminal != endCheckTerminal)
		{
			size_t nModeCount = m_mapSetRegionalTerminalTime.size();
			for(size_t a = 0; a < nModeCount; a++)
			{
				TTerminalProcessTime oTTerminalProcessTime = iterCheckTerminal->second;
				strCheckModeName    =oTTerminalProcessTime.strModeName;
				double dCheckAccess = oTTerminalProcessTime.dAccess;
				double dCheckEgress = oTTerminalProcessTime.dEgress;

				AutoType iterDefault = m_mapRegionalTerminalTime.find(a_nMasterCode);
				AutoType endDefault  = m_mapRegionalTerminalTime.end();
				if(iterDefault != endDefault)
				{
					std::vector<TTerminalProcessTime>& vecTTerminalProcessTime = iterDefault->second;
					size_t nTerminalCount = vecTTerminalProcessTime.size();
					for(size_t i = 0; i < nTerminalCount; i++)
					{
						TTerminalProcessTime oTTerminalProcessTime = vecTTerminalProcessTime[i];
						CString strModeName  = oTTerminalProcessTime.strModeName;
						double dAccess       = oTTerminalProcessTime.dAccess;
						double dEgress       = oTTerminalProcessTime.dEgress;
						int	   nModeTransfer = oTTerminalProcessTime.nModeTransfer;

						if(strCheckModeName == strModeName)
						{
							strDefineModeName = strModeName;
							dDefineAccess     = dAccess;
							dDefineEgress     = dEgress;
							nDefModeTransfer  = nModeTransfer;
						}
					}
				}
				if(strCheckModeName == strDefineModeName)
				{
					pRecord = m_wndRegionalTerminalReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strDefineModeName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dDefineAccess, _T("%.0f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dDefineEgress, _T("%.0f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItem());
					pItem->HasCheckbox(TRUE);
					pItem->SetEditable(TRUE);
					pItem->SetAlignment(xtpColumnIconCenter);
					if (nDefModeTransfer == 1)
					{
						pItem->SetChecked(TRUE);
					}
				}
				else
				{
					pRecord = m_wndRegionalTerminalReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCheckModeName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dCheckAccess, _T("%.0f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dCheckEgress, _T("%.0f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItem());
					pItem->HasCheckbox(TRUE);
					pItem->SetEditable(TRUE);
					pItem->SetAlignment(xtpColumnIconCenter);
					if (nDefModeTransfer == 1)
					{
						pItem->SetChecked(TRUE);
					}
				}
				iterCheckTerminal++;
			}
			m_wndRegionalTerminalReport.Populate();
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

// void KDefaultInterPathGenerator::InitRegionalTerminalReportRecord(int a_nMasterCode)
// {
// 	try
// 	{
// 		m_wndRegionalTerminalReport.ResetContent(TRUE);
// 
// 		CXTPReportRecords*		pRecords		 = m_wndRegionalTerminalReport.GetRecords();
// 		CXTPReportRecord*		pRecord			 = nullptr;
// 		CXTPReportRecordItem*	pItem			 = nullptr;
// 
// 		AutoType iterDefault = m_mapRegionalTerminalTime.find(a_nMasterCode);
// 		AutoType endDefault  = m_mapRegionalTerminalTime.end();
// 		if(iterDefault != endDefault)
// 		{
// 			std::vector<TTerminalProcessTime>& vecTTerminalProcessTime = iterDefault->second;
// 			size_t nTerminalCount = vecTTerminalProcessTime.size();
// 			for(size_t i = 0; i < nTerminalCount; i++)
// 			{
// 				TTerminalProcessTime oTTerminalProcessTime = vecTTerminalProcessTime[i];
// 				CString strModeName  = oTTerminalProcessTime.strModeName;
// 				double dAccess = oTTerminalProcessTime.dAccess;
// 				double dEgress = oTTerminalProcessTime.dEgress;
// 
// 				pRecord = m_wndRegionalTerminalReport.AddRecord(new CXTPReportRecord());
// 
// 				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strModeName));
// 				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
// 				pItem ->SetAlignment(xtpColumnTextCenter);
// 				pItem ->SetEditable(FALSE);
// 
// 				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAccess, _T("%.0f")));
// 				pItem ->SetAlignment(xtpColumnTextCenter);
// 				pItem ->SetEditable(TRUE);
// 
// 				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dEgress, _T("%.0f")));
// 				pItem ->SetAlignment(xtpColumnTextCenter);
// 				pItem ->SetEditable(TRUE);
// 			}
// 			m_wndRegionalTerminalReport.Populate();
// 		}
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebug(ex->GetErrorMessage());
// 	}
// 	catch(...)
// 	{
// 		TxLogDebugException();
// 	}
// }

void KDefaultInterPathGenerator::SetRegionalTimeSetting()
{
	try
	{
		double dTaxitime    = 5.0;
		double dHighwayTime = 0.2;
		double dRailTime    = 0.5;
		double dInterVoT    = 254.22;

		CString strValue(_T(""));
		CEdit* pEdit;
		strValue.Format(_T("%.2f"), dTaxitime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXI);
		SetDlgItemText(IDC_EDIT_TAXI, strValue);

		strValue.Format(_T("%.2f"), dHighwayTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRANSIT);
		SetDlgItemText(IDC_EDIT_TRANSIT, strValue);

		strValue.Format(_T("%.2f"), dRailTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RAIL);
		SetDlgItemText(IDC_EDIT_RAIL, strValue);

		strValue.Format(_T("%.2f"), dInterVoT);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VALTIME);
		SetDlgItemText(IDC_EDIT_VALTIME, strValue);

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

void KDefaultInterPathGenerator::SetUrbanPathReportRecord()
{
	try
	{
		m_wndUrbanPathReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord	= nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		KDBaseDefaultParaInterPathGenerator::SetUrbanModeData(m_mapSetUrbanPathInfo);

		AutoType iterSetPath = m_mapSetUrbanPathInfo.begin();
		AutoType endSetPath  = m_mapSetUrbanPathInfo.end();
		if(iterSetPath != endSetPath)
		{
			size_t nPathCount = m_mapSetUrbanPathInfo.size();
			for(size_t i = 0; i < nPathCount; i++)
			{
				TUrbanPathInfo oTUrbanPathInfo = iterSetPath->second;

				CString strMode = oTUrbanPathInfo.strMode;
				int nMaxPath    = oTUrbanPathInfo.nMaxofPath;

				pRecord = m_wndUrbanPathReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(FALSE);
				pItem->SetItemData((DWORD_PTR)oTUrbanPathInfo.nMode);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nMaxPath));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				iterSetPath++;
			}
			m_wndUrbanPathReport.Populate();
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

void KDefaultInterPathGenerator::SetUrbanAutoReportRecord()
{
	try
	{
		m_wndUrbanAutoReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord	= nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		KDBaseDefaultParaInterPathGenerator::SetAutoCost(m_mapSetAutoCost);

		AutoType iterSetAuto = m_mapSetAutoCost.begin();
		AutoType endSetAuto  = m_mapSetAutoCost.end();
		if(iterSetAuto != endSetAuto)
		{
			size_t nAutoCount = m_mapSetAutoCost.size();
			for(size_t i = 0; i < nAutoCount; i++)
			{
				TInterAutoCost oTInterAutoCost = iterSetAuto->second;
				CString strAuto = oTInterAutoCost.strName;
				double  dPrice  = oTInterAutoCost.dPrice;
				double  dRatio  = oTInterAutoCost.dRation;
				double  dFuel   = oTInterAutoCost.dfule;

				pRecord = m_wndUrbanAutoReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAuto));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPrice, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRatio, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dFuel, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				iterSetAuto++;
			}
			m_wndUrbanAutoReport.Populate();
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

void KDefaultInterPathGenerator::SetUrbanTransitReportRecord()
{
	try
	{
		m_wndUrbanTransitReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord	= nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		CString strTaxi       = P_LPG_NAME;
		double  dBaseFare     = 2300.0;
		double  dBaseDist     = 2.0;
		double  dDistBaseFare = 100.0;
		double  dUnitDist     = 0.15;

		pRecord = m_wndUrbanTransitReport.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTaxi));
		pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem ->SetAlignment(xtpColumnTextCenter);
		pItem ->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBaseFare, _T("%.2f")));
		pItem ->SetAlignment(xtpColumnTextCenter);
		pItem ->SetEditable(TRUE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBaseDist, _T("%.2f")));
		pItem ->SetAlignment(xtpColumnTextCenter);
		pItem ->SetEditable(TRUE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dDistBaseFare, _T("%.2f")));
		pItem ->SetAlignment(xtpColumnTextCenter);
		pItem ->SetEditable(TRUE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dUnitDist, _T("%.2f")));
		pItem ->SetAlignment(xtpColumnTextCenter);
		pItem ->SetEditable(TRUE);

		m_wndUrbanTransitReport.Populate();
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

void KDefaultInterPathGenerator::SetRegionalPathReportRecord()
{
	try
	{
		m_wndRegionalPathReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndRegionalPathReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		CString strCodeName = _T("");
		int nMaxPath        = 5;

		KDBaseDefaultParaInterPathGenerator::SetRegionalModeInfo(m_pTarget, m_mapRegionalModeInfo);

		AutoType iterSetRegionalMode = m_mapRegionalModeInfo.begin();
		AutoType endSetRegionalMode  = m_mapRegionalModeInfo.end();
		if(iterSetRegionalMode != endSetRegionalMode)
		{
			size_t nAutoCount = m_mapRegionalModeInfo.size();
			for(size_t i = 0; i < nAutoCount; i++)
			{
				int nCodeKey        = iterSetRegionalMode->first;
				strCodeName = iterSetRegionalMode->second;

				pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCodeName));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nMaxPath));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				iterSetRegionalMode++;
			}
			pRecord = m_wndRegionalPathReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (m_strAddAccegr));
			pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(FALSE);
			pItem ->SetTooltip(m_steTooltipAceeEgr);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("출발/도착 접근경로"));
				pItem ->SetTooltip(_T("출발/도착 접근경로"));
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nMaxPath));
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(TRUE);

			m_wndRegionalPathReport.Populate();
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

void KDefaultInterPathGenerator::SetRegionalAutoReportRecord()
{
	try
	{
		m_wndRegionalAutoReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndRegionalAutoReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		KDBaseDefaultParaInterPathGenerator::SetAutoCost(m_mapSetAutoCost);

		AutoType iterSetAuto = m_mapSetAutoCost.begin();
		AutoType endSetAuto  = m_mapSetAutoCost.end();
		if(iterSetAuto != endSetAuto)
		{
			size_t nAutoCount = m_mapSetAutoCost.size();
			for(size_t i = 0; i < nAutoCount; i++)
			{
				TInterAutoCost oTInterAutoCost = iterSetAuto->second;
				CString strAuto = oTInterAutoCost.strName;
				double  dPrice  = oTInterAutoCost.dPrice;
				double  dRatio  = oTInterAutoCost.dRation;
				double  dFuel   = oTInterAutoCost.dfule;

				pRecord = m_wndRegionalAutoReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAuto));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPrice, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRatio, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dFuel, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				iterSetAuto++;
			}
			m_wndRegionalAutoReport.Populate();
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

void KDefaultInterPathGenerator::SetRegionalTerminalReportRecord()
{
	try
	{
		m_wndRegionalTerminalReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndRegionalTerminalReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		CString strCodeName = _T("");
		int nMaxPath        = 5;

		KDBaseDefaultParaInterPathGenerator::SetRegionalTerminalTime(m_pTarget, m_mapSetRegionalTerminalTime);

		AutoType iterSetRegionalTerminal = m_mapSetRegionalTerminalTime.begin();
		AutoType endSetRegionalTerminal  = m_mapSetRegionalTerminalTime.end();
		if(iterSetRegionalTerminal != endSetRegionalTerminal)
		{
			size_t nTerminalCount = m_mapSetRegionalTerminalTime.size();
			for(size_t i = 0; i < nTerminalCount; i++)
			{
				int nCodeKey        = iterSetRegionalTerminal->first;
				TTerminalProcessTime oTTerminalProcessTime = iterSetRegionalTerminal->second;
				CString strModeName		= oTTerminalProcessTime.strModeName;
				double dAccess			= oTTerminalProcessTime.dAccess;
				double dEgress			= oTTerminalProcessTime.dEgress;
				int	   nModeTransfer	= oTTerminalProcessTime.nModeTransfer;
				
				pRecord = m_wndRegionalTerminalReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strModeName));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAccess, _T("%.0f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dEgress, _T("%.0f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItem());
				pItem->HasCheckbox(TRUE);
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				if (nModeTransfer == 1)
				{
					pItem->SetChecked(TRUE);
				}

				iterSetRegionalTerminal++;
			}
			m_wndRegionalTerminalReport.Populate();
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

bool KDefaultInterPathGenerator::InvalidateUrbanAutoInputInfo()
{
	CXTPReportRecords*          pRecords  = m_wndUrbanAutoReport.GetRecords();
	CXTPReportRecord*           pRecord   = nullptr;
	CXTPReportRecordItemNumber* pItemNum  = nullptr;
	
	CString strRatio  (_T(""));
	double	dRatio    (0.0);	
	int     nSumRatio (0);

	CString strSetAuto (_T(""));
	int     nAutoId    (0);

	size_t	 nRecordCount = pRecords->GetCount();

	for (size_t i = 0; i < nRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);

		pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_RATIO);
		strRatio  = pItemNum ->GetCaption(NULL);
		dRatio    = _ttof(strRatio);

        /************************************************************************/
        /* pItemNum 에서 값을 가져오면
        가령 0.35 라고 Grid 상에는 0.35라고 보이지만
        dRatio 변수에 담고 나면 0.344444444444444444449 라고 담아지게 되는 경우가 발생한다.
        이를 위해 변환 로직 수행
        (Ratio 인 즉 소수점 두자인 데이터를 컨트롤하고 있는 상황이기때문에 단위를 % 로 변환한후 100% 인지 확인하는 방식으로 0.344444444449 => 35%  )
        */
        /************************************************************************/

        int nRatio = (int)(dRatio * 100.0 + 0.5);

        nSumRatio = nSumRatio + nRatio;
	}
	if (nSumRatio < 100)
	{
		AfxMessageBox(_T("비율의 합이 1보다 작습니다."));
		return false;
	}
	else if (nSumRatio > 100)
	{
		AfxMessageBox(_T("비율의 합이 1보다 큽니다."));
		return false;
	}

	double dValueOfTime(1);
	std::map<int, KODTime> mapODTime;
	KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(mapODTime);
	AutoType Sfind = mapODTime.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	AutoType Ufind = mapODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
	AutoType end  = mapODTime.end();
	if (Sfind != end)
	{
		KODTime oInfo = Sfind->second;
		dValueOfTime = oInfo.dTravelTime;
	}
	else if (Ufind != end)
	{
		KODTime oInfo = Ufind->second;
		dValueOfTime = oInfo.dTravelTime;
	}

	CString strValTime(_T(""));
	GetDlgItemText(IDC_EDIT_VALTIME, strValTime);
	double dInterValTime = _ttof(strValTime);

	if (dValueOfTime == 1 && dInterValTime < 1)
	{
		AfxMessageBox(_T("유료도로 통행료 시간가치(VOT)가 1이므로 링크통행시간 값은 1이상이 되어야 합니다."));
		return false;
	}

	if (dValueOfTime > 1 && dInterValTime != 1)
	{
		if (AfxMessageBox(_T("유료도로 통행료 시간가치(VOT)가 1보다 크기 때문에 링크통행시간 값은 1로 저장됩니다."), MB_YESNO) == IDYES )
		{
			GetDlgItem(IDC_EDIT_VALTIME)->SetWindowText(_T("1"));
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool KDefaultInterPathGenerator::InvalidateRegionalAutoInputInfo()
{
	CXTPReportRecords*          pRecords  = m_wndRegionalAutoReport.GetRecords();
	CXTPReportRecord*           pRecord   = nullptr;
	CXTPReportRecordItemNumber* pItemNum  = nullptr;

	CString strRatio  (_T(""));
	double	dRatio    (0.0);	
	int     nSumRatio (0);

	CString strSetAuto (_T(""));
	int     nAutoId    (0);

	size_t	 nRecordCount = pRecords->GetCount();

	for (size_t i = 0; i < nRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);

		pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_RATIO);
		strRatio  = pItemNum ->GetCaption(NULL);
		dRatio    = _ttof(strRatio);

        /************************************************************************/
        /* pItemNum 에서 값을 가져오면
        가령 0.35 라고 Grid 상에는 0.35라고 보이지만
        dRatio 변수에 담고 나면 0.344444444444444444449 라고 담아지게 되는 경우가 발생한다.
        이를 위해 변환 로직 수행
        (Ratio 인 즉 소수점 두자인 데이터를 컨트롤하고 있는 상황이기때문에 단위를 % 로 변환한후 100% 인지 확인하는 방식으로 0.344444444449 => 35%  )
        */
        /************************************************************************/

        int nRatio = (int)(dRatio * 100.0 + 0.5);

        nSumRatio = nSumRatio + nRatio;
	}

	if (nSumRatio < 100)
	{
		AfxMessageBox(_T("비율의 합이 1보다 작습니다."));
		return false;
	}
	else if (nSumRatio > 100)
	{
		AfxMessageBox(_T("비율의 합이 1보다 큽니다."));
		return false;
	}
	return true;
}

void KDefaultInterPathGenerator::SaveRegionalTimeSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strTaxiTime  (_T(""));
		GetDlgItemText(IDC_EDIT_TAXI, strTaxiTime);
		double dTaxiTime = _ttof(strTaxiTime);

		CString strHigwayTime (_T(""));
		GetDlgItemText(IDC_EDIT_TRANSIT,  strHigwayTime);
		double dHigwayTime = _ttof(strHigwayTime);

		CString strRailTime (_T(""));
		GetDlgItemText(IDC_EDIT_RAIL,  strRailTime);
		double dRailTime = _ttof(strRailTime);

		CString strValTime(_T(""));
		GetDlgItemText(IDC_EDIT_VALTIME, strValTime);
		double dValTime = _ttof(strValTime);

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_SETTING);
		strSQL.AppendFormat(_T(" ( master_code, taxi_time, highway_time, rail_time, inter_vot ) "));
		strSQL.AppendFormat(_T(" VALUES ( %d, %.2f, %.2f, %.2f, %f )"), a_nMasterCode, dTaxiTime, dHigwayTime, dRailTime, dValTime);

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveUrbanPathReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_PATH_INFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_PATH_INFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, name, no_path ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_PATH_INFO);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndUrbanPathReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strMode    (_T(""));
		CString strMaxPath (_T(""));
		int		nMaxPath   (1);
		int		nModeId	   (0);

		
		int	nRecordCount = pRecords->GetCount();
		
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);

			pItemText  = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_MODE);
			strMode    = pItemText ->GetCaption(NULL);

			nModeId    = (int)pItemText->GetItemData();

			pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PATH);
			strMaxPath = pItemNum ->GetCaption(NULL);
			nMaxPath   = _ttoi(strMaxPath);

			spPrepareStatement->BindInt    (1, a_nMasterCode);
			spPrepareStatement->BindInt    (2, nModeId);
			spPrepareStatement->BindText   (3, strMode);
			spPrepareStatement->BindInt    (4, nMaxPath);
			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveUrbanAutoReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_AUTO_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_AUTO_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, auto, price, ratio, fuel ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_AUTO_COST);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndUrbanAutoReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strAuto   (_T(""));
		CString strPrice  (_T(""));
		double	dPrice    (0.0);	
		CString strRatio  (_T(""));
		double	dRatio    (0.0);	
		CString strFuel   (_T(""));
		double	dFuel     (0.0);	

		CString strSetAuto (_T(""));
		int     nAutoId    (0);

		KDBaseDefaultParaInterPathGenerator::SetAutoCost(m_mapSetAutoCost);

		AutoType iterSetAuto = m_mapSetAutoCost.begin();
		AutoType endSetAuto  = m_mapSetAutoCost.end();
		size_t nAutoCostCount = m_mapSetAutoCost.size();

		if(iterSetAuto != endSetAuto)
		{
			int	nRecordCount = pRecords->GetCount();

			for (int i = 0; i < nRecordCount; i++)
			{
				pRecord   = pRecords->GetAt(i);

				pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_AUTO);
				strAuto   = pItemText ->GetCaption(NULL);

				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PRICE);
				strPrice  = pItemNum ->GetCaption(NULL);
				dPrice    = _ttof(strPrice);

				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_RATIO);
				strRatio  = pItemNum ->GetCaption(NULL);
				dRatio    = _ttof(strRatio);

				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_FUEL);
				strFuel   = pItemNum ->GetCaption(NULL);
				dFuel     = _ttof(strFuel);

				for(size_t a = 0; a < nAutoCostCount; a++)
				{
					TInterAutoCost oTInterAutoCost = iterSetAuto->second;
					strSetAuto = oTInterAutoCost.strName;

					if(strSetAuto == strAuto)
					{
						nAutoId = iterSetAuto->first;

						break;
					}
					iterSetAuto++;
				}
				spPrepareStatement->BindInt    (1, a_nMasterCode);
				spPrepareStatement->BindInt    (2, nAutoId);
				spPrepareStatement->BindText   (3, strAuto);
				spPrepareStatement->BindDouble (4, dPrice);
				spPrepareStatement->BindDouble (5, dRatio);
				spPrepareStatement->BindDouble (6, dFuel);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveUrbanTransiReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_TRANSIT_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_TRANSIT_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, taxi, base_fare, base_dist, dist_base_fare, unit_dist ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_TRANSIT_COST);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndUrbanTransitReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strTaxi         (_T(""));
		CString strBaseFare     (_T(""));
		double	dBaseFare       (0.0);	
		CString strBaseDist     (_T(""));
		double	dBaseDist       (0.0);	
		CString strDistBaseFare (_T(""));
		double	dDistBaseFare   (0.0);	
		CString strUnitDist     (_T(""));
		double	dUnitDist       (0.0);	

		int	nRecordCount = pRecords->GetCount();

		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord         = pRecords->GetAt(i);

			pItemText       = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_TAXI);
			strTaxi         = pItemText ->GetCaption(NULL);

			pItemNum        = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_BASEFAIR);
			strBaseFare     = pItemNum ->GetCaption(NULL);
			dBaseFare       = _ttof(strBaseFare);

			pItemNum        = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_BASEDIST);
			strBaseDist     = pItemNum ->GetCaption(NULL);
			dBaseDist       = _ttof(strBaseDist);

			pItemNum        = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_DISTBASEFARE);
			strDistBaseFare = pItemNum ->GetCaption(NULL);
			dDistBaseFare   = _ttof(strDistBaseFare);


			pItemNum        = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_UNITDIST);
			strUnitDist     = pItemNum ->GetCaption(NULL);
			dUnitDist       = _ttof(strUnitDist);

			spPrepareStatement->BindInt    (1, a_nMasterCode);
			spPrepareStatement->BindInt    (2, i+1);
			spPrepareStatement->BindText   (3, strTaxi);
			spPrepareStatement->BindDouble (4, dBaseFare);
			spPrepareStatement->BindDouble (5, dBaseDist);
			spPrepareStatement->BindDouble (6, dDistBaseFare);
			spPrepareStatement->BindDouble (7, dUnitDist);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveRegionalPathReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_INFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_INFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, name, no_path ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_INFO);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndRegionalPathReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strMode   (_T(""));
		CString strNoPath (_T(""));
		int	    nNoPath   (0);

		CString strSetModeName (_T(""));
		int     nModeId		   (0);
	
		AutoType iter = m_mapRegionalModeInfo.begin();
		AutoType end  = m_mapRegionalModeInfo.end();
		size_t nModeCount = m_mapRegionalModeInfo.size();

		if(iter != end)
		{
			int	nRecordCount = pRecords->GetCount();

			for (int i = 0; i < nRecordCount; i++)
			{
				pRecord   = pRecords->GetAt(i);

				pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_MODE);
				strMode   = pItemText ->GetCaption(NULL);

				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PATH);
				strNoPath  = pItemNum ->GetCaption(NULL);
				nNoPath    = _ttoi(strNoPath);

				for(iter = m_mapRegionalModeInfo.begin(); iter != end; ++iter) {
					strSetModeName = iter->second;

					if(strMode == m_strAddAccegr)
					{
						nModeId = KInterRegionModeID::ACC_EGR_CODE;
						break;
					}
		
					if(strSetModeName == strMode)
					{
						nModeId = iter->first;
						break;
					}
				}

				spPrepareStatement->BindInt    (1, a_nMasterCode);
				spPrepareStatement->BindInt    (2, nModeId);
				spPrepareStatement->BindText   (3, strMode);
				spPrepareStatement->BindDouble (4, nNoPath);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveRegionalAutoReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_AUTO_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_AUTO_COST);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, auto, price, ratio, fuel ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_AUTO_COST);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndRegionalAutoReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strAuto  (_T(""));
		CString strPrice (_T(""));
		double	dPrice   (0.0);	
		CString strRatio (_T(""));
		double	dRatio   (0.0);	
		CString strFuel  (_T(""));
		double	dFuel    (0.0);	


		CString strSetAuto (_T(""));
		int     nAutoId    (0);

		KDBaseDefaultParaInterPathGenerator::SetAutoCost(m_mapSetAutoCost);

		AutoType iterSetAuto = m_mapSetAutoCost.begin();
		AutoType endSetAuto  = m_mapSetAutoCost.end();
		size_t nAutoCostCount = m_mapSetAutoCost.size();

		if(iterSetAuto != endSetAuto)
		{
			int	nRecordCount = pRecords->GetCount();
			for (int i = 0; i < nRecordCount; i++)
			{
				pRecord   = pRecords->GetAt(i);

				pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_AUTO);
				strAuto   = pItemText ->GetCaption(NULL);

				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PRICE);
				strPrice  = pItemNum ->GetCaption(NULL);
				dPrice    = _ttof(strPrice);


				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_RATIO);
				strRatio  = pItemNum ->GetCaption(NULL);
				dRatio    = _ttof(strRatio);


				pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_FUEL);
				strFuel   = pItemNum ->GetCaption(NULL);
				dFuel     = _ttof(strFuel);

				for(size_t a = 0; a < nAutoCostCount; a++)
				{
					TInterAutoCost oTInterAutoCost = iterSetAuto->second;
					strSetAuto = oTInterAutoCost.strName;

					if(strSetAuto == strAuto)
					{
						nAutoId = iterSetAuto->first;
						break;
					}
					iterSetAuto++;
				}

				spPrepareStatement->BindInt    (1, a_nMasterCode);
				spPrepareStatement->BindInt    (2, nAutoId);
				spPrepareStatement->BindText   (3, strAuto);
				spPrepareStatement->BindDouble (4, dPrice);
				spPrepareStatement->BindDouble (5, dRatio);
				spPrepareStatement->BindDouble (6, dFuel);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::SaveRegionalTerminalReportRecord(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, id, name, access_time, egress_time, mode_transfer ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndRegionalTerminalReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		CString strMode   (_T(""));
		CString strAccess (_T(""));
		int	    nAccess   (0);
		CString strEgress (_T(""));
		int	    nEgress   (0);

		int	nRecordCount = pRecords->GetCount();

		CString strSetModeName (_T(""));
		int     nModeId		   (0);

		AutoType iterSetRegionalMode = m_mapRegionalModeInfo.begin();
		AutoType endSetRegionalMode  = m_mapRegionalModeInfo.end();
		size_t nModeCount = m_mapRegionalModeInfo.size();

		if(iterSetRegionalMode != endSetRegionalMode)
		{

			for (int i = 0; i < nRecordCount; i++)
			{
				pRecord    = pRecords->GetAt(i);

				pItemText  = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_MODE);
				strMode    = pItemText ->GetCaption(NULL);

				pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_ACCESS);
				strAccess  = pItemNum ->GetCaption(NULL);
				nAccess    = _ttoi(strAccess);

				pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_EGRESS);
				strEgress  = pItemNum ->GetCaption(NULL);
				nEgress    = _ttoi(strEgress);

				int		nModeTransfer(0);
				pItem				= pRecord->GetItem(_3_COLUMN_MODETRANSFER);
				if (pItem->IsChecked() == TRUE)
				{
					nModeTransfer = 1;
				}

				for(size_t a = 0; a < nModeCount; a++)
				{
					strSetModeName = iterSetRegionalMode->second;

					if(strSetModeName == strMode)
					{
						nModeId = iterSetRegionalMode->first;
						break;
					}
					iterSetRegionalMode++;
				}

				spPrepareStatement->BindInt  (1, a_nMasterCode);
				spPrepareStatement->BindInt  (2, nModeId);
				spPrepareStatement->BindText (3, strMode);
				spPrepareStatement->BindInt  (4, nAccess);
				spPrepareStatement->BindInt  (5, nEgress);
				spPrepareStatement->BindInt	 (6, nModeTransfer);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultInterPathGenerator::OnBnClickedOk()
{
	try
	{
		if ( !InvalidateUrbanAutoInputInfo())
		{
			return;
		}

		if ( !InvalidateRegionalAutoInputInfo())
		{
			return;
		}

		SaveRegionalTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveUrbanPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveUrbanAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveUrbanTransiReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveRegionalPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveRegionalAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveRegionalTerminalReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	KResizeDialogEx::OnOK();
}


void KDefaultInterPathGenerator::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KDefaultInterPathGenerator::OnBnClickedDefault()
{
	try
	{
		AutoType iterDefaultTimeSetting = m_mapRegionalPathTimeSetting.begin();
		AutoType endDefaultTimeSetting  = m_mapRegionalPathTimeSetting.end();
		if(iterDefaultTimeSetting != endDefaultTimeSetting)
		{
			AutoType iterUserTimeSetting = m_mapRegionalPathTimeSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserTimeSetting != endDefaultTimeSetting)
			{
				InitRegionalTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitRegionalTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultUrbanPath = m_mapUrbanPathInfo.begin();
		AutoType endDefaultUrbanPath  = m_mapUrbanPathInfo.end();
		if(iterDefaultUrbanPath != endDefaultUrbanPath)
		{
			AutoType iterUserUrbanPath = m_mapUrbanPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserUrbanPath != endDefaultUrbanPath)
			{
				InitUrbanPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitUrbanPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultUrbanAuto = m_mapUrbanAutoCost.begin();
		AutoType endDefaultUrbanAuto  = m_mapUrbanAutoCost.end();
		if(iterDefaultUrbanAuto != endDefaultUrbanAuto)
		{
			AutoType iterUserUrbanAuto = m_mapUrbanAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserUrbanAuto != endDefaultUrbanAuto)
			{
				InitUrbanAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitUrbanAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultUrbanTransit = m_mapUrbanTransitCost.begin();
		AutoType endDefaultUrbanTransit  = m_mapUrbanTransitCost.end();
		if(iterDefaultUrbanTransit != endDefaultUrbanTransit)
		{
			AutoType iterUserUrbanTransit = m_mapUrbanTransitCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserUrbanTransit != endDefaultUrbanTransit)
			{
				InitUrbanTransitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitUrbanTransitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultRegionalPathInfo = m_mapRegionalPathInfo.begin();
		AutoType endDefaultRegionalPathInfo  = m_mapRegionalPathInfo.end();
		if(iterDefaultRegionalPathInfo != endDefaultRegionalPathInfo)
		{
			AutoType iterUserRegionalAuto = m_mapRegionalPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserRegionalAuto != endDefaultRegionalPathInfo)
			{
				InitRegionalPathReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitRegionalPathReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultRegionalAuto = m_mapRegionalAutoCost.begin();
		AutoType endDefaultRegionalAuto  = m_mapRegionalAutoCost.end();
		if(iterDefaultRegionalAuto != endDefaultRegionalAuto)
		{
			AutoType iterUserRegionalAuto = m_mapRegionalAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserRegionalAuto != endDefaultRegionalAuto)
			{
				InitRegionalAutoReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitRegionalAutoReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		AutoType iterDefaultRegionalTerminal = m_mapRegionalTerminalTime.begin();
		AutoType endDefaultRegionalTerminal  = m_mapRegionalTerminalTime.end();
		if(iterDefaultRegionalTerminal != endDefaultRegionalTerminal)
		{
			AutoType iterUserRegionalTerminal = m_mapRegionalTerminalTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterUserRegionalTerminal != endDefaultRegionalTerminal)
			{
				InitRegionalTerminalReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitRegionalTerminalReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
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
