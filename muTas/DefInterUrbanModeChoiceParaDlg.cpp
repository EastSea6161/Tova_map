// DefInterUrbanModeChoiceParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefInterUrbanModeChoiceParaDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Scenario.h"
#include "Purpose.h"
#include "DBaseDefaultParaInterUrbanModeChoice.h"
#include "DBaseInterModal.h"
#include "DefineNamesModels.h"
#include "ImChampFrameWindow.h"

#include "DBaseConnector.h"


#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

// KDefInterUrbanModeChoiceParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefInterUrbanModeChoiceParaDlg, KResizeDialogEx)

KDefInterUrbanModeChoiceParaDlg::KDefInterUrbanModeChoiceParaDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefInterUrbanModeChoiceParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_dMeuParameter(0.1)
{
	//MakeDefaultParameter(m_mapDefaultPara);
	//KDBaseDefaultParaInterUrbanModeChoice::LoadDefaultModeChoiceMEU(m_dMeuParameter);

	// 설명변수와 존특성변수 정보를 읽고 그 다음 변수 값을 읽어온다.
	KDBaseDefaultParaInterUrbanModeChoice::ReadVariableInfo(m_mapDefaultVar);
	m_mapVariableInfo = m_mapDefaultVar;

	KDBaseDefaultParaInterUrbanModeChoice::ReadZoneVariableInfo(m_mapDefaultZoneVar);
	m_mapZoneVariableInfo = m_mapDefaultZoneVar;

	KDBaseDefaultParaInterUrbanModeChoice::ReadDefaultModeChoicePara(m_mapDefaultPara);
	KDBaseDefaultParaInterUrbanModeChoice::ReadCurrentModeChoicePara(m_mapCurrentPara);

	ReadZoneColumnInfo();
	
	InitDefaultPurposePara();	// 기본값의 변수 명 설정
	InitCurrentPurposePara();	// 현재 변수의 변수 명 설정

	MakeStringPurposeParaMap();	// 목적문자열을 키로하는 설명변수 맵 생성
}

KDefInterUrbanModeChoiceParaDlg::~KDefInterUrbanModeChoiceParaDlg()
{
}

void KDefInterUrbanModeChoiceParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
}


BEGIN_MESSAGE_MAP(KDefInterUrbanModeChoiceParaDlg, KResizeDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KDefInterUrbanModeChoiceParaDlg::OnSelchangeTabObject)
	ON_BN_CLICKED(IDOK,                &KDefInterUrbanModeChoiceParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,            &KDefInterUrbanModeChoiceParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1,         &KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_SET_VARIABLE, &KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonSetVariable)
END_MESSAGE_MAP()


// KDefInterUrbanModeChoiceParaDlg 메시지 처리기입니다.


//void KDefInterUrbanModeChoiceParaDlg::MakeDefaultParameter(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara)
//{
//	try
//	{
//		a_mapDefaultPara.clear();
//
//		BaseDefaultPara(a_mapDefaultPara); // 빈 틀을 만든다.
//
//		std::map<int, std::map<int, TUrbanChoiceValues>> mapDefaultParaLoadDB;
//		KDBaseDefaultParaInterUrbanModeChoice::LoadDefaultModeChoicePara(mapDefaultParaLoadDB);
//
//		// 빈 틀에 DB 조회 데이터를 채워 준다.
//		AutoType iter  = mapDefaultParaLoadDB.begin();
//		AutoType itEnd = mapDefaultParaLoadDB.end();
//
//		while (iter != itEnd)
//		{
//			int nObjectID = iter->first;
//			std::map<int, TUrbanChoiceValues> &mapLoadDB = iter->second;
//
//			AutoType itBase = a_mapDefaultPara.find(nObjectID);
//			if (itBase != a_mapDefaultPara.end())
//			{
//				std::map<int, TUrbanChoiceValues> &mapBase = itBase->second;
//
//				AutoType iterB  = mapLoadDB.begin();
//				AutoType itEndB = mapLoadDB.end();
//
//				while (iterB != itEndB)
//				{
//					AutoType iterC = mapBase.find(iterB->first);
//					if (iterC != mapBase.end())
//					{
//						TUrbanChoiceValues &oTUrbanChoiceValues_Base   = iterC->second;
//						TUrbanChoiceValues &oTUrbanChoiceValues_LoadDB = iterB->second;
//
//						CString strTempName    = oTUrbanChoiceValues_Base.strCofficientName;
//						CString strTempDisplay = oTUrbanChoiceValues_Base.strCoefficientDisplay;
//
//						oTUrbanChoiceValues_Base = oTUrbanChoiceValues_LoadDB;
//						oTUrbanChoiceValues_Base.strCofficientName     = strTempName;
//						oTUrbanChoiceValues_Base.strCoefficientDisplay  = strTempDisplay;
//					}
//
//					++iterB;
//				}
//			}
//
//			++iter;
//		}
//	}
//	catch (...)
//	{
//		TxLogDebugException();	
//	}
//}


// 2018.11.19 명지대 요청 적용
//void KDefInterUrbanModeChoiceParaDlg::BaseDefaultPara(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultParaBase)
//{
//	try
//	{
//		std::map<int, CString> mapCoefficientName; 
//		std::map<int, CString> mapCoefficientDisplay; 
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::INVEHICLE_TIME_CODE,        KInterCofficientPara::INVEHICLE_TIME_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::TRANSIT_WAITING_TIME_CODE,  KInterCofficientPara::TRANSIT_WAITING_TIME_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_TIME_CODE, KInterCofficientPara::TRANSIT_TRANSFER_TIME_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::COST_CODE,                  KInterCofficientPara::COST_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::PARKING_FEE_CODE,           KInterCofficientPara::PARKING_FEE_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::AE_WALKING_TIME_CODE,       KInterCofficientPara::AE_WALKING_TIME_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_TIME_CODE,     KInterCofficientPara::AE_INVEHICLE_TIME_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_COST_CODE,     KInterCofficientPara::AE_INVEHICLE_COST_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_NUM_CODE,  KInterCofficientPara::TRANSIT_TRANSFER_NUM_NAME));
//		mapCoefficientName.insert(std::make_pair(KInterCofficientPara::DUMMY_CODE,                 KInterCofficientPara::DUMMY_NAME));
//
//		if (KmzSystem::GetLanguage() == KEMKorea) {
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::INVEHICLE_TIME_CODE,        KInterCofficientPara::INVEHICLE_TIME_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_WAITING_TIME_CODE,  KInterCofficientPara::TRANSIT_WAITING_TIME_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_TIME_CODE, KInterCofficientPara::TRANSIT_TRANSFER_TIME_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::COST_CODE,                  KInterCofficientPara::COST_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::PARKING_FEE_CODE,           KInterCofficientPara::PARKING_FEE_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_WALKING_TIME_CODE,       KInterCofficientPara::AE_WALKING_TIME_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_TIME_CODE,     KInterCofficientPara::AE_INVEHICLE_TIME_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_COST_CODE,     KInterCofficientPara::AE_INVEHICLE_COST_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_NUM_CODE,  KInterCofficientPara::TRANSIT_TRANSFER_NUM_DISPLAY_K));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::DUMMY_CODE,                 KInterCofficientPara::DUMMY_DISPLAY_K));
//		}
//		else
//		{
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::INVEHICLE_TIME_CODE,        KInterCofficientPara::INVEHICLE_TIME_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_WAITING_TIME_CODE,  KInterCofficientPara::TRANSIT_WAITING_TIME_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_TIME_CODE, KInterCofficientPara::TRANSIT_TRANSFER_TIME_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::COST_CODE,                  KInterCofficientPara::COST_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::PARKING_FEE_CODE,           KInterCofficientPara::PARKING_FEE_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_WALKING_TIME_CODE,       KInterCofficientPara::AE_WALKING_TIME_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_TIME_CODE,     KInterCofficientPara::AE_INVEHICLE_TIME_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::AE_INVEHICLE_COST_CODE,     KInterCofficientPara::AE_INVEHICLE_COST_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::TRANSIT_TRANSFER_NUM_CODE,  KInterCofficientPara::TRANSIT_TRANSFER_NUM_DISPLAY));
//			mapCoefficientDisplay.insert(std::make_pair(KInterCofficientPara::DUMMY_CODE,                 KInterCofficientPara::DUMMY_DISPLAY));
//		}
//
//		KProject*              pProject   = ImChampFrameWindow::GetProject();
//		std::vector<KPurpose*> vecPurpose =  pProject->GetPurpose();
//
//		for (size_t i= 0; i< vecPurpose.size(); i++)
//		{
//			KPurpose* pPurpose   = vecPurpose[i];
//			int       nPurposeID = pPurpose->PurposeID();
//
//			m_mapPurposeName.insert(std::make_pair(nPurposeID, pPurpose->PurposeName()));
//
//			std::map<int, TUrbanChoiceValues> mapUrbanChoiceValues;
//
//			AutoType  iter       = mapCoefficientName.begin();
//			AutoType  itEnd      = mapCoefficientName.end();
//
//			while (iter != itEnd)
//			{
//				int nCoefficientID = iter->first;
//
//				TUrbanChoiceValues oTUrbanChoiceValues = {};
//				oTUrbanChoiceValues.nCofficientCode   = nCoefficientID;
//				oTUrbanChoiceValues.strCofficientName = iter->second;
//
//				AutoType iterB = mapCoefficientDisplay.find(nCoefficientID);
//				if (iterB != mapCoefficientDisplay.end())
//					oTUrbanChoiceValues.strCoefficientDisplay = iterB->second;
//				else
//					oTUrbanChoiceValues.strCoefficientDisplay = iter->second;
//
//				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
//
//				++iter;
//			}
//
//			a_mapDefaultParaBase.insert(std::make_pair(nPurposeID, mapUrbanChoiceValues));
//		}
//	}
//	catch (...)
//	{
//		TxLogDebugException();	
//	}
//};
//void KDefInterUrbanModeChoiceParaDlg::BaseDefaultPara(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultParaBase)
//{
//	try
//	{
//
//		std::map<int, CString> mapCoefficientName;
//
//		KDBaseConPtr spDBConnection = KDBase::GetProjectDBConnection();
//		if (nullptr == spDBConnection)
//		{
//			return;
//		}
//
//		CString strSQL = _T("SELECT ID, VarName, Selected "
//			" FROM UrbanModeChoiceVariable "
//			" WHERE Selected = 1 "
//			" ORDER BY ID;");
//
//		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
//		if (nullptr == spResultset)
//		{
//			return;
//		}
//
//		while (spResultset->Next())
//		{
//			int id = spResultset->GetValueInt(0);
//			CString name = spResultset->GetValueString(1);
//			mapCoefficientName.insert({ id, name });
//		}
//
//
//		KProject*              pProject = ImChampFrameWindow::GetProject();
//		std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();
//
//		for (size_t i = 0; i< vecPurpose.size(); i++)
//		{
//			KPurpose* pPurpose = vecPurpose[i];
//			int       nPurposeID = pPurpose->PurposeID();
//
//			m_mapPurposeName.insert(std::make_pair(nPurposeID, pPurpose->PurposeName()));
//
//			std::map<int, TUrbanChoiceValues> mapUrbanChoiceValues;
//
//			AutoType  iter = mapCoefficientName.begin();
//			AutoType  itEnd = mapCoefficientName.end();
//
//			while (iter != itEnd)
//			{
//				int nCoefficientID = iter->first;
//
//				TUrbanChoiceValues oTUrbanChoiceValues = {};
//				oTUrbanChoiceValues.nCofficientCode = nCoefficientID;
//				oTUrbanChoiceValues.strCofficientName = iter->second;
//				oTUrbanChoiceValues.strCoefficientDisplay = iter->second;
//
//				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
//
//				++iter;
//			}
//
//			a_mapDefaultParaBase.insert(std::make_pair(nPurposeID, mapUrbanChoiceValues));
//		}
//	}
//	catch (...)
//	{
//		TxLogDebugException();
//	}
//};

void KDefInterUrbanModeChoiceParaDlg::InitDefaultPurposePara()
{
	try
	{
		KProject*              pProject = ImChampFrameWindow::GetProject();
		std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();


		for (size_t i = 0; i< vecPurpose.size(); i++)
		{
			KPurpose* pPurpose = vecPurpose[i];
			int       nPurposeID = pPurpose->PurposeID();

			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itObj = m_mapDefaultPara.find(nPurposeID);
			if (itObj != m_mapDefaultPara.end())
			{
				std::map<int, TUrbanChoiceValues>& mapPara = itObj->second;
				for (auto& a : mapPara)
				{
					std::map<int, TDescriptionVariableInfo>::iterator itVar = m_mapVariableInfo.find(a.second.nCofficientCode);
					if (itVar != m_mapVariableInfo.end())
					{
						a.second.strCofficientName = itVar->second.strVariableName;
						a.second.strCoefficientDisplay = itVar->second.strVariableName;
					}
				}


			}
			else
			{
				// 빈 데이터 추가
				std::map<int, TUrbanChoiceValues> mapPara;
				//for (auto& aVI : m_mapVariableInfo)
				//{
				//	if (aVI.second.bSelected)
				//	{
				//		TUrbanChoiceValues para;
				//		para.nCofficientCode = aVI.second.nId;
				//		para.strCofficientName = aVI.second.strVariableName;
				//		para.strCoefficientDisplay = aVI.second.strVariableName;
				//		para.dAuto = 0;
				//		para.dTaxi = 0;
				//		para.dHighwayTransit = 0;
				//		para.dRailTransit = 0;
				//		para.dCompositMode = 0;
				//		para.dKissAndRide = 0;
				//		para.dParkAndRide = 0;
				//		para.dTaxiAndTransit = 0;
				//		mapPara.insert({ para.nCofficientCode, para });
				//	}
				//}

				m_mapDefaultPara.insert({ nPurposeID, mapPara });
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceParaDlg::InitCurrentPurposePara()
{
	try
	{
		KProject*              pProject = ImChampFrameWindow::GetProject();
		std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();

		for (size_t i = 0; i< vecPurpose.size(); i++)
		{
			KPurpose* pPurpose = vecPurpose[i];
			int       nPurposeID = pPurpose->PurposeID();

			m_mapPurposeName.insert(std::make_pair(nPurposeID, pPurpose->PurposeName()));

			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itObj = m_mapCurrentPara.find(nPurposeID);
			if (itObj != m_mapCurrentPara.end())
			{
				std::map<int, TUrbanChoiceValues>& mapPara = itObj->second;
				for (auto& a : mapPara)
				{
					std::map<int, TDescriptionVariableInfo>::iterator itVar = m_mapVariableInfo.find(a.second.nCofficientCode);
					if (itVar != m_mapVariableInfo.end())
					{
						a.second.strCofficientName = itVar->second.strVariableName;
						a.second.strCoefficientDisplay = itVar->second.strVariableName;
					}
					else
					{
						std::map<int, TZoneVariableInfo>::iterator itZV = m_mapZoneVariableInfo.find(a.second.nCofficientCode);
						if (m_mapZoneVariableInfo.end() != itZV)
						{
							a.second.strCofficientName = itZV->second.strVariableName + _T(":[") + itZV->second.strZoneColDispName + _T("]");
							a.second.strCoefficientDisplay = a.second.strCofficientName;
						}
					}
				}
			}
			else
			{
				std::map<int, TUrbanChoiceValues> mapPara;
				for (auto& aVI : m_mapVariableInfo)
				{
					if (aVI.second.bSelected)
					{
						TUrbanChoiceValues para;
						para.nCofficientCode = aVI.second.nId;
						para.strCofficientName = aVI.second.strVariableName;
						para.strCoefficientDisplay = aVI.second.strVariableName;
						para.dAuto = 0;
						para.dTaxi = 0;
						para.dHighwayTransit = 0;
						para.dRailTransit = 0;
						para.dCompositMode = 0;
						para.dKissAndRide = 0;
						para.dParkAndRide = 0;
						para.dTaxiAndTransit = 0;
						para.strObjName = pPurpose->PurposeName();
						para.strObjNameEng = para.strObjName;
						mapPara.insert({ para.nCofficientCode, para });
					}
				}

				m_mapCurrentPara.insert({ nPurposeID, mapPara });
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceParaDlg::MakeStringPurposeParaMap()
{
	m_mapStrPurposeDefaultPara.clear();

	try
	{
		// 먼저 default 목적ID-목적명 맵을 생성
		std::map<int, CString> mapPurpose;

		for (auto& aDP : m_mapDefaultPara)
		{
			std::map<int, TUrbanChoiceValues>& mapUCV = aDP.second;
			for (auto& aUCV : mapUCV)
			{
				if(KmzSystem::GetLanguage() == KEMKorea)
				{
					mapPurpose.insert({ aDP.first, aUCV.second.strObjName });
				}
				else
				{
					mapPurpose.insert({ aDP.first, aUCV.second.strObjNameEng });
				}
			}
		}

		for (auto& aPName : mapPurpose)
		{
			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itObj = m_mapDefaultPara.find(aPName.first);
			if (itObj != m_mapDefaultPara.end())
			{
				std::map<int, TUrbanChoiceValues>& mapPara = itObj->second;
				m_mapStrPurposeDefaultPara.insert({ aPName.second, mapPara });
			}
			else
			{
				// 빈 데이터 추가
				std::map<int, TUrbanChoiceValues> mapPara;
				m_mapStrPurposeDefaultPara.insert({ aPName.second, mapPara });
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}



BOOL KDefInterUrbanModeChoiceParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strMeuParameter(_T(""));
	strMeuParameter.Format(_T("%g"), m_dMeuParameter);
	SetDlgItemText(IDC_EDIT_MEU, strMeuParameter);

	CreateChildPage();

	DisplayTabControl(0);

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDefInterUrbanModeChoiceParaDlg::CreateChildPage()
{
	try
	{
		int nTabIndex(0);

		// 목적별로 현재의 변수를 가져온다
		for (auto& aPurpose : m_mapPurposeName)
		{
			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itCurrentPara = m_mapCurrentPara.find(aPurpose.first);
			ASSERT(m_mapCurrentPara.end() != itCurrentPara);
			int     nObjectID = aPurpose.first;
			std::map<int, TUrbanChoiceValues> &mapModeChoicePara = itCurrentPara->second;
			CString strObjectName = aPurpose.second;

			CRect oTabRect;
			m_ctrlTab.GetClientRect(&oTabRect);

			KDefaultInterModeChoiceParaSubPagePtr spSubPage = KDefaultInterModeChoiceParaSubPagePtr(new KDefaultInterModeChoiceParaSubPage(nObjectID, strObjectName));
			spSubPage->SetParameterData(mapModeChoicePara);
			spSubPage->Create(IDD_8511_Default_Inter_ModeChoiceParaSubPage, &m_ctrlTab);
			spSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width() - TAB_WIDTH_MARGIN, oTabRect.Height() - TAB_HEIGHT_MARGIN, SWP_NOZORDER);

			m_ctrlTab.InsertItem(nTabIndex, strObjectName);
			m_mapSubPage.insert(std::make_pair(nTabIndex, spSubPage));
			nTabIndex++;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefInterUrbanModeChoiceParaDlg::ResizeComponet()
{
	SetResize(IDC_TAB1,				SZ_TOP_LEFT,       SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR11,		SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_SET_VARIABLE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,   		SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_COPY,      SZ_TOP_RIGHT,      SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PASTE,     SZ_TOP_RIGHT,      SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SHEET,     SZ_TOP_RIGHT,      SZ_TOP_RIGHT);
}


void KDefInterUrbanModeChoiceParaDlg::OnSize(UINT nType, int cx, int cy)
{
	KResizeDialogEx::OnSize(nType, cx, cy);

	SubPageRedrawWindow();
}


void KDefInterUrbanModeChoiceParaDlg::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_spCurrentShowClientDlg)
		{        
			CRect oTabRect;
			m_ctrlTab.GetClientRect(&oTabRect);	

			m_spCurrentShowClientDlg->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefInterUrbanModeChoiceParaDlg::OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	DisplayTabControl(m_ctrlTab.GetCurSel());
	*pResult = 0;
}


void KDefInterUrbanModeChoiceParaDlg::DisplayTabControl( int a_nTabIndex )
{
	try
	{
		if (nullptr != m_spCurrentShowClientDlg)
		{
			m_spCurrentShowClientDlg->ShowWindow(SW_HIDE);
		}

	
		AutoType iter = m_mapSubPage.find(a_nTabIndex);
		if (iter != m_mapSubPage.end())
		{
			m_spCurrentShowClientDlg = iter->second;
			m_spCurrentShowClientDlg->ShowWindow(SW_SHOW);
			SubPageRedrawWindow();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonDefault()
{
	try
	{
		CString strMeuParameter(_T(""));
		strMeuParameter.Format(_T("%g"), m_dMeuParameter);
		SetDlgItemText(IDC_EDIT_MEU, strMeuParameter);

		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KDefaultInterModeChoiceParaSubPagePtr spPage = iter->second;
			spPage->UpdateReportData();

			++iter;
		}

		m_mapVariableInfo = m_mapDefaultVar;
		m_mapZoneVariableInfo = m_mapDefaultZoneVar;



		// 2019.04.04 : 초기값은 마지막 설정된 값으로 변경한다.
		//for (auto& a1 : m_mapVariableInfo)
		//{
		//	a1.second.bSelected = a1.second.bDefault;
		//}

		//for (auto& a2 : m_mapZoneVariableInfo)
		//{
		//	a2.second.bSelected = false;
		//}


		//for(std::map<int, KDefaultInterModeChoiceParaSubPagePtr>::iterator itPage = m_mapSubPage.begin(); 
		//	itPage != m_mapSubPage.end(); ++itPage)
		//{
		//	KDefaultInterModeChoiceParaSubPagePtr spPage = itPage->second;
		//	int nObjId = spPage->GetPageID();

		//	bool bEmpty = false;
		//	std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itDefault = m_mapDefaultPara.find(nObjId);
		//	if (m_mapDefaultPara.end() == itDefault)
		//	{
		//		bEmpty = true;
		//	}

		//	std::map<int, TUrbanChoiceValues> mapNewPara;
		//	for (auto& aDV : m_mapVariableInfo)
		//	{
		//		if (aDV.second.bDefault)
		//		{
		//			if (bEmpty)
		//			{
		//				TUrbanChoiceValues v;
		//				v.nCofficientCode = aDV.second.nId;
		//				v.strCofficientName = aDV.second.strVariableName;
		//				v.strCoefficientDisplay = aDV.second.strVariableName;
		//				v.dAuto = 0;
		//				v.dTaxi = 0;
		//				v.dHighwayTransit = 0;
		//				v.dRailTransit = 0;
		//				v.dCompositMode = 0;
		//				v.dParkAndRide = 0;
		//				v.dKissAndRide = 0;
		//				v.dTaxiAndTransit = 0;
		//				mapNewPara.insert({ aDV.second.nId, v });
		//			}
		//			else
		//			{
		//				std::map<int, TUrbanChoiceValues>::iterator itPara = itDefault->second.find(aDV.second.nId);
		//				if (itDefault->second.end() != itPara)
		//				{
		//					mapNewPara.insert({ aDV.second.nId, itPara->second });
		//				}
		//				else
		//				{
		//					TUrbanChoiceValues v;
		//					v.nCofficientCode = aDV.second.nId;
		//					v.strCofficientName = aDV.second.strVariableName;
		//					v.strCoefficientDisplay = aDV.second.strVariableName;
		//					v.dAuto = 0;
		//					v.dTaxi = 0;
		//					v.dHighwayTransit = 0;
		//					v.dRailTransit = 0;
		//					v.dCompositMode = 0;
		//					v.dParkAndRide = 0;
		//					v.dKissAndRide = 0;
		//					v.dTaxiAndTransit = 0;
		//					mapNewPara.insert({ aDV.second.nId, v });
		//				}
		//			}
		//		}
		//		
		//	}	// for (auto& aDV : m_mapVariableInfo)

		//	spPage->UpdateReportData(mapNewPara);
		//}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefInterUrbanModeChoiceParaDlg::OnBnClickedOk()
{
	if (!InvalidInputInfo())
	{
		return;
	}

	if (m_arrConflictTarget.size() > 0)
	{
		CString msg = _T("아래 시나리오에 선택한 존 특성변수 컬럼이 존재하지만 데이터 유형이 상이합니다.\n\n");
		for (CString& strConflict : m_arrConflictTarget)
		{
			msg += strConflict;
		}
		msg += _T("\n존 DB 테이블에서 선택한 컬럼을 확인하시고 다시 존 특성변수를 설정해 주시길 바랍니다.");
		AfxMessageBox(msg);
		return;
	}

	if (m_mapMissingTarget.size() > 0)
	{
		int nNumber = 1;
		CString msg = _T(" 아래 시나리오에 선택한 존 특성변수 컬럼이 존재하지 않습니다.\n\n");
		for (std::map<int, STArgsColumnMissingTarget>::iterator itMissing = m_mapMissingTarget.begin();
			itMissing != m_mapMissingTarget.end(); ++itMissing)
		{
			STArgsColumnMissingTarget& argsCMT = itMissing->second;
			CString strMissing;
			strMissing.Format(_T("%d. %s - %s\n"), nNumber++, argsCMT.pScenario->GetName(), argsCMT.pTarget->GetName());
			msg += strMissing;
		}
		msg += _T("\n현재 활성화된 시나리오의 존 특성변수 컬럼과 동일한 형태로 자동생성됩니다.\n");
		msg += _T("계속 하시겠습니까 ? ");
		if (AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
	}


	if (SaveVariables())
	{
		SaveParameter();

		ProcessMissingZoneColumn();
	}

	KResizeDialogEx::OnOK();
}


bool KDefInterUrbanModeChoiceParaDlg::InvalidInputInfo()
{
	try
	{
		CString strMeuParameter(_T(""));

		GetDlgItemText(IDC_EDIT_MEU, strMeuParameter);
		strMeuParameter.Trim();
		SetDlgItemText(IDC_EDIT_MEU, strMeuParameter);

		if (strMeuParameter.Compare(_T("")) == 0)
		{
			AfxMessageBox(_T("포괄가치 계수를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}

		if (!QBicStringChecker::IsNumeric(strMeuParameter))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}



void KDefInterUrbanModeChoiceParaDlg::SaveParameter()
{
	CString strMeuParameter(_T(""));
	GetDlgItemText(IDC_EDIT_MEU, strMeuParameter);

	int    nMasterCode(1);
	double dMeuParameterSave = _ttof(strMeuParameter);
	std::map<int, std::map<int, TUrbanChoiceValues>> mapDefaultParaSaveDB;
	
	KDBaseDefaultParaInterUrbanModeChoice::DeleteInsertModeChoiceMEU(nMasterCode, dMeuParameterSave);
	
	try
	{
		AutoType iter  = m_mapSubPage.begin();
		AutoType itEnd = m_mapSubPage.end();

		while (iter != itEnd)
		{
			KDefaultInterModeChoiceParaSubPagePtr spPage = iter->second;
			int nObjectID = spPage->GetPageID();

			std::map<int, TUrbanChoiceValues> mapInterModeChoicePara;// = spPage;
			spPage->GetNewParameterData(mapInterModeChoicePara);

			mapDefaultParaSaveDB.insert(std::make_pair(nObjectID, mapInterModeChoicePara));

			++iter;
		}
		
		KDBaseDefaultParaInterUrbanModeChoice::DeleteInsertModeChoicePara(nMasterCode, mapDefaultParaSaveDB);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	/* close 하지 않는다면
	//m_dMeuParameter = dMeuParameterSave;
	//m_mapDefaultPara.clear();
	//m_mapDefaultPara.insert(mapDefaultParaSaveDB.begin(), mapDefaultParaSaveDB.end());
	*/
}


void KDefInterUrbanModeChoiceParaDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonCopy()
{
	try
	{
		m_mapTmpInterModeChoicePara.clear();
		
		int nTabIndex = m_ctrlTab.GetCurSel();

		std::map<int, KDefaultInterModeChoiceParaSubPagePtr>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->GetNewParameterData(m_mapTmpInterModeChoicePara);
		}
		else
		{
			throw 1;
		}

		GetDlgItem(IDC_BUTTON_PASTE)->EnableWindow(TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonPaste()
{
	try
	{
		int nTabIndex = m_ctrlTab.GetCurSel();

		std::map<int, KDefaultInterModeChoiceParaSubPagePtr>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->UpdateReportData(m_mapTmpInterModeChoicePara);
		}
		else
		{
			throw 1;
		}

		AfxMessageBox(_T("복사되었습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KDefInterUrbanModeChoiceParaDlg::OnBnClickedButtonSetVariable()
{
	if (SelectAndUpdateVariables())
	{
		// 선택된 변수의 값을 구해온 후 각 서브페이지로 저장
		for (std::map<int, KDefaultInterModeChoiceParaSubPagePtr>::iterator itPage = m_mapSubPage.begin();
			itPage != m_mapSubPage.end(); ++itPage)
		{
			KDefaultInterModeChoiceParaSubPagePtr spSubPage = itPage->second;
			int nObjId = spSubPage->GetPageID();

			// 현재 페이지의 설정값을 가져온 후 선택 항목이 없으면 추가하고, 선택되지 않은 항목이 있으면 제거
			std::map<int, TUrbanChoiceValues> mapNewPara;	// 현재 페이지의 설정값
			spSubPage->GetNewParameterData(mapNewPara);

			// 현재 페이지의 기본값을 가져온다.
			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itCurrentPara = m_mapCurrentPara.find(nObjId);
			std::map<int, std::map<int, TUrbanChoiceValues>>::iterator itDefaultPara = m_mapDefaultPara.find(nObjId);


			// 설명변수 항목 갱신
			for (auto& aDV : m_mapVariableInfo)
			{
				std::map<int, TUrbanChoiceValues>::iterator itNewPara = mapNewPara.find(aDV.second.nId);
				if (aDV.second.bSelected)
				{
					bool bFind = false;
					// 선택된 변수가 현재 페이지에 없으면 추가
					if (itNewPara == mapNewPara.end())
					{
						if (m_mapCurrentPara.end() != itCurrentPara)
						{
							std::map<int, TUrbanChoiceValues>& mapCurPara = itCurrentPara->second;
							std::map<int, TUrbanChoiceValues>::iterator itCurPara = mapCurPara.find(aDV.second.nId);
							if (mapCurPara.end() != itCurPara)
							{
								mapNewPara.insert({ itCurPara->first, itCurPara->second });
								bFind = true;
							}
						}

						if (!bFind && m_mapDefaultPara.end() != itDefaultPara)
						{
							std::map<int, TUrbanChoiceValues>& mapDefPara = itDefaultPara->second;
							std::map<int, TUrbanChoiceValues>::iterator itDefPara = mapDefPara.find(aDV.second.nId);
							if (mapDefPara.end() != itDefPara)
							{
								mapNewPara.insert({ itDefPara->first, itDefPara->second });
								bFind = true;
							}
						}

						if (!bFind)
						{
							TUrbanChoiceValues np;
							np.nCofficientCode = aDV.second.nId;
							np.strCofficientName = aDV.second.strVariableName;
							np.strCoefficientDisplay = aDV.second.strVariableName;
							np.dAuto = 0;
							np.dTaxi = 0;
							np.dHighwayTransit = 0;
							np.dRailTransit = 0;
							np.dCompositMode = 0;
							np.dParkAndRide = 0;
							np.dKissAndRide = 0;
							np.dTaxiAndTransit = 0;
							mapNewPara.insert({ aDV.second.nId, np });
						}
					}
				}
				else
				{
					// 선택되지 않은 변수가 현재 페이지에 있으면 제거
					if (itNewPara != mapNewPara.end())
					{
						mapNewPara.erase(itNewPara);
					}
				}
			}	// for (auto& aDV : m_mapVariableInfo)

			// 존특성 변수 항목 추가
			for (auto& aZV : m_mapZoneVariableInfo)
			{
				std::map<int, TUrbanChoiceValues>::iterator itNewPara = mapNewPara.find(aZV.second.nId);
				if (aZV.second.bSelected)
				{
					if (mapNewPara.end() != itNewPara)
					{
						// 2019.04.08 : "존특성변수명은 변수명 : 선택된 컬럼명" 으로 표출
						itNewPara->second.strCofficientName = aZV.second.strVariableName + _T(":[") + aZV.second.strZoneColDispName + _T("]");
						itNewPara->second.strCoefficientDisplay = itNewPara->second.strCofficientName;
					}
					else
					{
						bool bFindZoneVar = false;
						if (m_mapCurrentPara.end() != itCurrentPara)
						{
							std::map<int, TUrbanChoiceValues>& mapCurPara = itCurrentPara->second;
							std::map<int, TUrbanChoiceValues>::iterator itCurPara = mapCurPara.find(aZV.second.nId);
							if (mapCurPara.end() != itCurPara)
							{
								// 2019.04.08 : "존특성변수명은 변수명 : 선택된 컬럼명" 으로 표출
								itCurPara->second.strCofficientName = aZV.second.strVariableName + _T(":[") + aZV.second.strZoneColDispName + _T("]");
								itCurPara->second.strCoefficientDisplay = itCurPara->second.strCofficientName;
								mapNewPara.insert({ itCurPara->first, itCurPara->second });
								bFindZoneVar = true;
							}
						}

						if (!bFindZoneVar)
						{
							TUrbanChoiceValues np;
							np.nCofficientCode = aZV.second.nId;
							np.strCofficientName = aZV.second.strVariableName + _T(":[") + aZV.second.strZoneColDispName + _T("]");
							np.strCoefficientDisplay = np.strCofficientName;
							np.dAuto = 0;
							np.dTaxi = 0;
							np.dHighwayTransit = 0;
							np.dRailTransit = 0;
							np.dCompositMode = 0;
							np.dParkAndRide = 0;
							np.dKissAndRide = 0;
							np.dTaxiAndTransit = 0;
							mapNewPara.insert({ aZV.second.nId, np });
						}
					}
				}
				else
				{
					if (mapNewPara.end() != itNewPara)
					{
						mapNewPara.erase(itNewPara);
					}
				}
			}

			spSubPage->UpdateReportData(mapNewPara);
		}
	}
}



bool KDefInterUrbanModeChoiceParaDlg::UpdateDBSelectedVariables(KDBaseConPtr spDBConn)
{
	try
	{
		if (nullptr == spDBConn)
		{
			return false;
		}

		// 먼저 전체 변수의 선택여부를 선택 안함을 설정
		CString strSQL = _T("UPDATE UrbanModeChoiceVariable SET Selected = 0");
		spDBConn->ExecuteUpdate(strSQL);

		for (auto& aDV : m_mapVariableInfo)
		{
			if (aDV.second.bSelected)
			{
				strSQL.Format(_T("UPDATE UrbanModeChoiceVariable SET Selected = 1 WHERE ID = %d"), aDV.second.nId);
				spDBConn->ExecuteUpdate(strSQL);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDefInterUrbanModeChoiceParaDlg::UpdateDBSelectedZoneVariables(KDBaseConPtr spDBConn)
{
	try
	{
		if (nullptr == spDBConn)
		{
			return false;
		}

		// 먼저 전체 변수의 선택여부를 선택 안함을 설정
		CString strSQL = _T("UPDATE UrbanModeChoiceVariableZone "
			" SET Selected = 0, ZoneColName = '', ZoneColDispName = '', ZoneColDataType = 0, ZoneColCodeType = -1, ZoneColDescription = '-'");
		spDBConn->ExecuteUpdate(strSQL);

		for(auto& aZV : m_mapZoneVariableInfo)
		{
			if (aZV.second.bSelected)
			{
				strSQL.Format(_T("UPDATE UrbanModeChoiceVariableZone "
					" SET Selected = 1, ZoneColName = '%s', ZoneColDispName = '%s', ZoneColDataType=%d, ZoneColCodeType=%d, ZoneColDescription = '%s'"
					" WHERE ID = %d"),
					aZV.second.strZoneColName, aZV.second.strZoneColDispName, aZV.second.nZoneColDataType, aZV.second.nZoneColCodeType, 
					aZV.second.strZoneColDesc, aZV.second.nId);

				spDBConn->ExecuteUpdate(strSQL);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KDefInterUrbanModeChoiceParaDlg::SelectAndUpdateVariables()
{
	KDefInterUrbanModeChoiceVariableSettingDlg dlg(m_pTarget);
	dlg.SetDescriptionVariableInfo(m_mapVariableInfo);
	dlg.SetZoneVariableInfo(m_mapZoneVariableInfo);
	dlg.SetUserZoneColumnInfo(m_mapUserZoneColumnInfo);

	if (dlg.DoModal() == IDOK)
	{
		std::vector<TDescriptionVariableInfo> arrSelectedVariable = dlg.GetSelectedVariables();
		std::vector<TZoneVariableInfo> arrSelectedZoneVariable = dlg.GetSelectedZoneVariables();

		// 설명변수 선택 항목을 갱신
		for (auto& aDV : m_mapVariableInfo)
		{
			aDV.second.bSelected = false;
		}

		for (TDescriptionVariableInfo& selDV : arrSelectedVariable)
		{
			std::map<int, TDescriptionVariableInfo>::iterator itDest = m_mapVariableInfo.find(selDV.nId);
			if (m_mapVariableInfo.end() != itDest)
			{
				itDest->second.bSelected = true;
			}
			else
			{
				CString strLog;
				strLog.Format(_T("Description Variable not exists : varId=%d"), selDV.nId);
				TxLogDebug((LPCTSTR)strLog);
			}
		}

		// 존특성 변수 선택 항목을 갱신
		for (auto& aZV : m_mapZoneVariableInfo)
		{
			aZV.second.bSelected = false;
			aZV.second.strZoneColName = _T("");
			aZV.second.strZoneColDispName = _T("-");
			aZV.second.nZoneColDataType = 0;
			aZV.second.nZoneColCodeType = -1;
			aZV.second.strZoneColDesc = "-";
		}

		for (TZoneVariableInfo& selZV : arrSelectedZoneVariable)
		{
			std::map<int, TZoneVariableInfo>::iterator itDest = m_mapZoneVariableInfo.find(selZV.nId);
			if (m_mapZoneVariableInfo.end() != itDest)
			{
				itDest->second.bSelected = true;
				itDest->second.strZoneColDispName = selZV.strZoneColDispName;
				itDest->second.nZoneColDataType = selZV.nZoneColDataType;
				itDest->second.nZoneColCodeType = selZV.nZoneColCodeType;
				itDest->second.strZoneColDesc = selZV.strZoneColDesc;
			}
			else
			{
				CString strLog;
				strLog.Format(_T("Zone Variable not exists : varId=%d"), selZV.nId);
				TxLogDebug((LPCTSTR)strLog)
			}
		}

		CheckZoneColumnConflictAndMissing(arrSelectedZoneVariable);
	}
	else
	{
		return false;
	}
	

	return true;
}


bool KDefInterUrbanModeChoiceParaDlg::SaveVariables() 
{
	bool bResult = false;
	KDBaseConPtr spDBConn = KDBase::GetProjectDBConnection();
	try
	{
		spDBConn->BeginTransaction();

		if (UpdateDBSelectedVariables(spDBConn) && UpdateDBSelectedZoneVariables(spDBConn))
		{
			spDBConn->Commit();
			bResult = true;
		}
		else
		{
			spDBConn->RollBack();
			throw std::exception();
		}
	}
	catch (...)
	{
		spDBConn->RollBack();
		AfxMessageBox(_T("설명변수 데이터 갱신 작업 중 오류가 발생하였습니다."));
	}

	return bResult;
}



bool KDefInterUrbanModeChoiceParaDlg::ProcessMissingZoneColumn()
{
	if (m_mapMissingTarget.empty())
	{
		return true;
	}

	CWaitCursor cw;
	bool bFailed = false;

	KIOTables* pSrcIOTables = m_pTarget->Tables();
	KIOTable* pSrcIOTable = pSrcIOTables->FindTable(TABLE_ZONE);
	KIOColumns* pSrcColumns = pSrcIOTable->Columns();

	for (auto& aCMT : m_mapMissingTarget)
	{
		STArgsColumnMissingTarget& argsCMT = aCMT.second;
		if (NULL == argsCMT.pScenario || NULL == argsCMT.pTarget || argsCMT.arrMissingCol.empty())
		{
			continue;
		}

		KIOTables* pIOTables = argsCMT.pTarget->Tables();
		ASSERT(NULL != pIOTables);

		KIOTable*  pIOTable = pIOTables->FindTable(TABLE_ZONE);
		ASSERT(NULL != pIOTable);

		pIOTable->BeginTransaction();

		if (AddMissingColumns(pSrcIOTable, argsCMT) == false)
		{
			pIOTable->Rollback();
			CString strError;
			strError.Format(_T("Failed to add column : Scenario=%s, Target=%s"),
				argsCMT.pScenario->GetName(), argsCMT.pTarget->GetName());
			AfxMessageBox(strError);
			continue;
		}

		pIOTable->Commit();
	}

	return true;
}



bool KDefInterUrbanModeChoiceParaDlg::AddMissingColumns(KIOTable* pSrcIOTable, STArgsColumnMissingTarget& argsCMT)
{
	if (NULL == pSrcIOTable)
	{
		return false;
	}

	KIOTables* pIOTables = argsCMT.pTarget->Tables();
	if (NULL == pIOTables)
	{
		return false;
	}

	KIOTable*  pIOTable = pIOTables->FindTable(TABLE_ZONE);
	if (NULL == pIOTable)
	{
		return false;
	}

	KIOColumns* pSrcColumns = pSrcIOTable->Columns();
	if (NULL == pSrcColumns)
	{
		return false;
	}

	std::vector<CString> arrSrcColumnNames;
	std::vector<CString> arrDestColumnNames;
	std::vector<KEMIODataType> arrColumnDataTypes;

	int nDispOrder = GetNextColumnDisplayOrder(pIOTable);

	int nSrcColumnCount = pSrcColumns->ColumnCount();
	try
	{
		// #01. 먼저 누락된 컬럼을 추가
		for (CString& strMissingColCaption : argsCMT.arrMissingCol)
		{
			if (IsColumnCaptionExists(pIOTable, strMissingColCaption))
			{
				CString strError;
				strError.Format(_T("%s - %s에 추가 하려는 컬럼명 %s가 이미 존재함."), 
					argsCMT.pScenario->GetName(), argsCMT.pTarget->GetName(), strMissingColCaption);
				AfxMessageBox(strError);
				continue;
				//throw KExceptionPtr(new KException(strError));
			}

			for (int i = 0; i < nSrcColumnCount; ++i)
			{
				KIOColumn* pSrcColumn = pSrcColumns->GetColumn(i);
				CString strSrcCaption = pSrcColumn->Caption();
				if (strMissingColCaption.CompareNoCase(strSrcCaption) == 0)
				{
					KIOColumn colNew(*pSrcColumn);
					colNew.Name(pIOTable->GetUsableColumnName());
					colNew.DisplayOrder(nDispOrder++);


					if (colNew.DataType() != KEMIODataTypeInteger && colNew.DataType() != KEMIODataTypeDouble)
					{
						throw KExceptionPtr(new KException(_T("추가하려는 데이터 타입이 Integer 또는 Double이 아님")));
					}
					arrColumnDataTypes.push_back(colNew.DataType());

					pIOTable->AddColumn(colNew);
					arrSrcColumnNames.push_back(pSrcColumn->Name());
					arrDestColumnNames.push_back(colNew.Name());
				}
			}
		}

		
		// #02. 추가된 컬럼에 데이터 입력
		// 원본과 대상의 컬럼 목록 문자열 생성
		int nColCount = arrDestColumnNames.size();
		CString strSrcColumns = MakeSQLColumnNameString(arrSrcColumnNames);
		CString strDestColumns = MakeSQLColumnNameString(arrDestColumnNames);

		
		CString strSQLSelect = _T("SELECT zone_id, name, ") + strSrcColumns + _T(" FROM zone");


		CString strSQLUpdate = _T("UPDATE zone SET (") + strDestColumns + _T(") = (");
		for (std::vector<CString>::size_type i = 0; i < arrDestColumnNames.size(); ++i)
		{
			strSQLUpdate += _T("?, ");
		}

		strSQLUpdate.Delete(strSQLUpdate.GetLength() - 2, 2);
		strSQLUpdate += _T(") WHERE zone_id = ? AND (SELECT IFNULL(name, '') FROM zone WHERE zone_id = ?) = ?");
		
		KDBaseConPtr spDBConnSrc = m_pTarget->GetDBaseConnection();
		KDBaseConPtr spDBConnDest = pIOTable->GetDBaseConnection();
		if (nullptr == spDBConnSrc || nullptr == spDBConnDest)
		{
			return false;
		}

		KResultSetPtr spResultset = spDBConnSrc->ExecuteQuery(strSQLSelect);
		if (nullptr == spResultset)
		{
			return false;
		}

		KPreparedStatementPtr spPrepared = spDBConnDest->PrepareStatement(strSQLUpdate);

		while (spResultset->Next())
		{
			Integer nZoneId = spResultset->GetValueInt64(0);
			CString strZoneName = spResultset->GetValueString(1);

			std::vector<boost::variant<int, double>> arrValue;
			for (int i = 0; i < nColCount; ++i)
			{
				boost::variant<int, double> val;
				if (arrColumnDataTypes[i] == KEMIODataTypeInteger)
				{
					val = spResultset->GetValueInt(i + 2);
				}
				else
				{
					val = spResultset->GetValueDouble(i + 2);
				}
				arrValue.push_back(val);
			}

			for (int i = 0; i < nColCount; ++i)
			{
				if (arrColumnDataTypes[i] == KEMIODataTypeInteger)
				{
					spPrepared->BindInt(i + 1, boost::get<int>(arrValue[i]));
				}
				else
				{
					spPrepared->BindDouble(i + 1, boost::get<double>(arrValue[i]));
				}
			}

			spPrepared->BindInt64(nColCount + 1, nZoneId);
			spPrepared->BindInt64(nColCount + 2, nZoneId);
			spPrepared->BindText(nColCount + 3, strZoneName);

			spPrepared->ExecuteUpdate();
			spPrepared->Reset();
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}



int KDefInterUrbanModeChoiceParaDlg::GetNextColumnDisplayOrder(KIOTable* pIOTable)
{
	if (NULL == pIOTable || pIOTable->Columns() == NULL)
	{
		return 0;
	}

	int nOrder = 0;
	const KIOColumns* pColumns = pIOTable->Columns();
	int nCount = pColumns->ColumnCount();
	for (int i = 0; i < nCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if (NULL == pColumn)
		{
			continue;
		}

		if (nOrder < pColumn->DisplayOrder())
		{
			nOrder = pColumn->DisplayOrder();
		}
	}

	return nOrder + 1;
}


CString KDefInterUrbanModeChoiceParaDlg::MakeSQLColumnNameString(std::vector<CString>& arrColumnNames)
{
	CString strResult;
	for (CString& str : arrColumnNames)
	{
		strResult += (str + _T(", "));
	}

	if (strResult.GetLength() > 2)
	{
		strResult.Delete(strResult.GetLength() - 2, 2);
	}

	return strResult;
}


bool KDefInterUrbanModeChoiceParaDlg::IsColumnCaptionExists(KIOTable* pTable, CString& strCaption)
{
	if (NULL == pTable || pTable->Columns() == NULL)
	{
		return false;
	}

	KIOColumns* pColumns = pTable->Columns();
	for (int i = 0; i < pColumns->ColumnCount(); ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if ((NULL != pColumn) && (strCaption.CompareNoCase(pColumn->Caption()) == 0))
		{
			return true;
		}
	}
	
	return false;
}


void KDefInterUrbanModeChoiceParaDlg::ReadZoneColumnInfo()
{
	m_mapUserZoneColumnInfo.clear();

	//// 빈 컬럼정보를 추가 
	//TZoneUserColumnInfo infoEmpty;
	//infoEmpty.nId = 0;
	//infoEmpty.strColumnName = _T("");
	//if (KmzSystem::GetLanguage() == KEMKorea)
	//{
	//	infoEmpty.strDisplayName = _T("해당 없음");
	//}
	//else
	//{
	//	infoEmpty.strDisplayName = _T("None");
	//}
	//infoEmpty.strDescription = _T("-");

	//m_mapUserZoneColumnInfo.insert({ 0, infoEmpty });

	try
	{
		if (nullptr == m_pTarget)
		{
			return;
		}

		KDBaseConPtr spDBConnection = m_pTarget->GetDBaseConnection();
		if (nullptr == spDBConnection)
		{
			return;
		}

		CString strSQL = _T("SELECT Column_Name, Display_Name, Column_Info, Data_Type, Code_Type FROM Column_Description "
			" WHERE Table_Name = 'zone' "
			" AND Column_Type = 1 "
			" AND Data_Type IN (1, 2) "
			" AND Code_Type = -1"
			" AND Use_Flag = 1"
			" AND Column_Name NOT IN (SELECT Zone_Column_Name FROM Generation_Model)"
		);

		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
		if (nullptr == spResultset)
		{
			return;
		}

		int nIndex = 1;
		while (spResultset->Next())
		{
			TZoneUserColumnInfo info;
			info.nId = nIndex++;
			info.strColumnName = spResultset->GetValueString(0);
			info.strDisplayName = spResultset->GetValueString(1);
			info.strDescription = spResultset->GetValueString(2);
			info.nDataType = spResultset->GetValueInt(3);
			info.nCodeType = spResultset->GetValueInt(4);
			m_mapUserZoneColumnInfo.insert({ info.nId, info });
		}
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


std::vector<TZoneUserColumnInfo> KDefInterUrbanModeChoiceParaDlg::ReadTargetZoneColumn(KDBaseConPtr spDBConn)
{
	std::vector<TZoneUserColumnInfo> arrZoneUserColumn;

	try
	{
		CString strSQL = _T("SELECT Column_Name, Display_Name, Column_Info, Data_Type, Code_Type FROM Column_Description "
			" WHERE Table_Name = 'zone'");

		KResultSetPtr spResultset = spDBConn->ExecuteQuery(strSQL);
		if (nullptr == spResultset)
		{
			return arrZoneUserColumn;
		}

		int nIndex = 1;
		while (spResultset->Next())
		{
			TZoneUserColumnInfo info;
			info.nId = nIndex++;
			info.strColumnName = spResultset->GetValueString(0);
			info.strDisplayName = spResultset->GetValueString(1);
			info.strDescription = spResultset->GetValueString(2);
			info.nDataType = spResultset->GetValueInt(3);
			info.nCodeType = spResultset->GetValueInt(4);
			arrZoneUserColumn.push_back(info);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return arrZoneUserColumn;
}


void KDefInterUrbanModeChoiceParaDlg::CheckZoneColumnConflictAndMissing(std::vector<TZoneVariableInfo>& arrSelectedZoneVariable)
{
	m_arrConflictTarget.clear();
	m_mapMissingTarget.clear();

	KScenario* pThisScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject = (KProject*)pThisScenario->GetParentObject();

	int nNumber = 1;

	KTDScenarioMap mapScenario = pProject->GetScenarioMap();
	for (KTDScenarioMap::iterator itScenario = mapScenario.begin();
		itScenario != mapScenario.end(); ++itScenario)
	{
		if (NULL == itScenario->second)
		{
			continue;
		}

		KScenario* pScenario = itScenario->second;
		KTDTargetMap mapTarget = pScenario->GetTargetMap();
		for (KTDTargetMap::iterator itTarget = mapTarget.begin();
			itTarget != mapTarget.end(); ++itTarget)
		{
			if (NULL == itTarget->second)
			{
				continue;
			}
			KTarget* pTarget = itTarget->second;
			KDBaseConnectionPtr spDBConn = pTarget->GetDBaseConnection();
			std::vector<TZoneUserColumnInfo> arrTargetZoneColumns = ReadTargetZoneColumn(spDBConn);
			std::set<CString> setAddedColumn;

			for (TZoneVariableInfo& zvi : arrSelectedZoneVariable)
			{
				// 이미 추가된 컬럼은 건너뜀
				if (setAddedColumn.find(zvi.strZoneColDispName) != setAddedColumn.end())
				{
					continue;
				}

				bool bMissing = true; // 컬럼 누락 체크 확인 변수
				for (TZoneUserColumnInfo& dst : arrTargetZoneColumns)
				{
					// 컬럼제목(caption)은 동일한데, 데이터 타입이 다르거나 코드 유형이 다르면 충돌로 판단
					if (zvi.strZoneColDispName == dst.strDisplayName)
					{
						if ((zvi.nZoneColDataType != dst.nDataType) || (zvi.nZoneColCodeType != dst.nCodeType))
						{
							CString strConflict;
							strConflict.Format(_T("%d. %s - %s\n"), nNumber++, pScenario->GetName(), pTarget->GetName());
							m_arrConflictTarget.push_back(strConflict);
						}

						bMissing = false;
					}
				}


				if (bMissing)
				{
					int nTargetId = pTarget->GetObjectID();
					std::map<int, STArgsColumnMissingTarget>::iterator itMissing = m_mapMissingTarget.find(nTargetId);
					if (m_mapMissingTarget.end() == itMissing)
					{
						STArgsColumnMissingTarget argCMT;
						argCMT.pScenario = pScenario;
						argCMT.pTarget = pTarget;
						argCMT.arrMissingCol.push_back(zvi.strZoneColDispName);

						m_mapMissingTarget.insert({ nTargetId, argCMT });
					}
					else
					{
						itMissing->second.arrMissingCol.push_back(zvi.strZoneColDispName);
					}
					setAddedColumn.insert(zvi.strZoneColDispName);
				}
			}
		}
	}
}