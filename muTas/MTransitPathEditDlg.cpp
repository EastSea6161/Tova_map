// MTransitPathEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MTransitPathEditDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"
#include "NodeSelectionInfo.h"
#include "ImChampMapDefine.h"
#include "KDBaseAccessibilityModify.h"



const int COLUMN_WIDTH_USE = 55;	// 사용여부 컬럼 너비
const int COLUMN_WIDTH_INFO = 50;	// 상세정보 컬럼 너비


namespace MTransitENUM
{
	enum
	{
		_0_A_COLUMN_SELECT     = 0,
		_1_A_COLUMN_ZONEID     = 1,
		_2_A_COLUMN_ISBUS      = 2,
		_3_A_COLUMN_ISRAIL     = 3,
		_4_A_COLUMN_ISBUSRAIL  = 4
	};

	enum
	{
		_0_B_COLUMN_ZONEID        = 0,
		_1_B_COLUMN_TOTAL_TIME    = 1,
		_2_B_COLUMN_LENGTH        = 2,
		_3_B_COLUMN_COST          = 3,
		_4_B_COLUMN_USE           = 4,
		_5_B_COLUMN_DETAIL        = 5
	};
	enum 
	{
		B_COLUMN_TOTAL_COUNT     = 6
	};

	enum
	{
		EXPANDALL_LIMIT_COUNT    = 10 
	};

	enum KENUMMode
	{
		KENUMModeBus      = 1,
		KENUMModeRail     = 2,
		KENUMModeBusRail  = 3
	};
}

using namespace MTransitENUM;
// KMTransitPathEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMTransitPathEditDlg, KResizeDialogEx)

KMTransitPathEditDlg::KMTransitPathEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMTransitPathEditDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_dRadius(1000.0)
{
	m_oStartNodeIdCaption.TID = _T("-1");

	m_spPathCompareDlg = nullptr;
}

KMTransitPathEditDlg::~KMTransitPathEditDlg()
{
	CloseWindowProc();
}

void KMTransitPathEditDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                 m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B,                 m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KMTransitPathEditDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,                      &KMTransitPathEditDlg::OnReportAItemClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,                      &KMTransitPathEditDlg::OnReportBItemClick)
	
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_A, &KMTransitPathEditDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_B, &KMTransitPathEditDlg::OnReportBCheckItem)

	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_B, &KMTransitPathEditDlg::OnItemButtonClick)
	
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT_B, &KMTransitPathEditDlg::OnReportBValueChanged)

	ON_BN_CLICKED(IDC_CHECK_ALL,                           &KMTransitPathEditDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON1,                             &KMTransitPathEditDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_CHECK_B,                   &KMTransitPathEditDlg::OnBnClickedCheckB)
	ON_BN_CLICKED(IDC_CHECK_R,                   &KMTransitPathEditDlg::OnBnClickedCheckR)
	ON_BN_CLICKED(IDC_CHECK_BR,                  &KMTransitPathEditDlg::OnBnClickedCheckBr)
	ON_BN_CLICKED(IDC_BUTTON5,     &KMTransitPathEditDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL,        &KMTransitPathEditDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KMTransitPathEditDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_BUTTON3, &KMTransitPathEditDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON4, &KMTransitPathEditDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON6, &KMTransitPathEditDlg::OnBnClickedButtonEndNodeSearch)
	ON_BN_CLICKED(IDC_BTN_COMPARE_PATH, &KMTransitPathEditDlg::OnBnClickedBtnComparePath)
END_MESSAGE_MAP()


// KMTransitPathEditDlg 메시지 처리기입니다.
void KMTransitPathEditDlg::InitializeNode()
{
	CWaitCursor wc;

	m_mapBusAssing.clear();
	m_mapRailAssing.clear();
	m_mapBusRailAssing.clear();

	GetNodeInfo();

	m_oStartNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT1, _T(""));

	m_setSearchedID.clear();
	m_strSearchedIDs = _T("");
	SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);

	m_mapEndZoneNodeData.clear();
	m_vecEndZoneNodeIDCaption.clear();

	m_wndReportACtrl.ResetContent();
	m_wndReportBCtrl.ResetContent();

	UpdateSearchDataCount();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);
}


bool KMTransitPathEditDlg::GetNodeInfo(void)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;
		strSQL.Format (_T(" SELECT node_id, name FROM %s "), TABLE_NODE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			throw 1;
		}
		m_mapNodeName.clear();

		Integer nxNodeId(0);
		CString strNodeName(_T(""));

		while( pResult->Next() )
		{
			nxNodeId	    = pResult->GetValueInt64 (0);
			strNodeName     = pResult->GetValueString(1);

			m_mapNodeName.insert(std::make_pair(nxNodeId, strNodeName));
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name, full_name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return false;
		}

		m_mapZoneNodeData.clear();
		m_vecZoneNodeIDCaption.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));
		CString strNodeFullName(_T(""));

		while( pResult->Next() )
		{
			nxNodeId	    = pResult->GetValueInt64 (0);
			strNodeName     = pResult->GetValueString(1);
			strNodeFullName	= pResult->GetValueString(2);

			m_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;
			
			m_vecZoneNodeIDCaption.push_back(oIDCaption);
		}
	}
	catch (...)
	{
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KMTransitPathEditDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		AutoType iter  = a_mapNodeData.find(nxZoneID);
		AutoType itEnd = a_mapNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TID      = a_strZoneID;
		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(strName);

		a_strNodeViewData.Format(_T("%s [%s]"), a_strZoneID, strName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMTransitPathEditDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KMTransitPathEditDlg::UpdateSearchDataCount()
{
	try
	{
		CString strCount(_T(""));

		int nAllDestinationCnt    = (int)m_mapEndZoneNodeData.size();
		int nSearchDestinationCnt = (int)m_setSearchedID.size();
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCount.Format(_T("경로 탐색 결과(선택/전체): %d / %d"), nSearchDestinationCnt, nAllDestinationCnt);
		}
		else {
			strCount.Format(_T("Records:  %d / %d"), nSearchDestinationCnt, nAllDestinationCnt);
		}

		SetDlgItemText(IDC_STATIC6, strCount);	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KMTransitPathEditDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	double dRadiousKm(0.0);
	KDBaseAccessibilityModify::GetZoneRadius(m_pTarget, dRadiousKm);
	if (0.0 == dRadiousKm) {
		m_dRadius = 1000.0;
	}
	else {
		m_dRadius = dRadiousKm * 1000;
	}

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// 2019.04.02 : 편집 부분에서는 경로비교를 안함.
	GetDlgItem(IDC_BTN_COMPARE_PATH)->ShowWindow(SW_HIDE);

	CWaitCursor wc;

	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateSearchDataCount();

	KReportCtrlSetting::DefaultC(m_wndReportACtrl);
	KReportCtrlSetting::DefaultC(m_wndReportBCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportBCtrl);

	InitReportAHeader();
	InitReportBHeader();

	ResizeComponent();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMTransitPathEditDlg::InitReportAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT,       _T("Select"),     60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_A_COLUMN_ZONEID,       _T("Destination"),    40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("종점"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_2_A_COLUMN_ISBUS,        _T("Bus"),        20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_3_A_COLUMN_ISRAIL,       _T("Rail"),       20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시철도"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_4_A_COLUMN_ISBUSRAIL,    _T("Bus+Rail"),   20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스+도시철도"));
    }

	m_wndReportACtrl.Populate();
}


void KMTransitPathEditDlg::InitReportBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;
	CXTPReportRecordItemEditOptions* pEditOptions = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_ZONEID,       _T("Destination-Transit Path"),    38));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("경로 구분"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_TOTAL_TIME,   _T("Time"),       14));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시간"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_LENGTH,       _T("Length"),     14));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("거리"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_COST,         _T("Cost"),       16));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비용"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_USE,          _T("Use"), COLUMN_WIDTH_USE, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("사용여부"));
    }

	pEditOptions = pColumn->GetEditOptions();
	pEditOptions->AddComboButton(TRUE);
	pEditOptions->m_bAllowEdit = TRUE;
	pEditOptions->m_bExpandOnSelect = TRUE;
	InputGridComboData(pEditOptions);

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_5_B_COLUMN_DETAIL,       _T("Info"), COLUMN_WIDTH_INFO, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("상세정보"));
    }

	m_wndReportBCtrl.Populate();
}


void KMTransitPathEditDlg::InputGridComboData(CXTPReportRecordItemEditOptions* a_pEditOptions)
{
    if (KmzSystem::GetLanguage() == KEMKorea) {
        a_pEditOptions->AddConstraint(_T("씀"),   (DWORD_PTR)0 ); 
        a_pEditOptions->AddConstraint(_T("안 씀"), (DWORD_PTR)1 );
    }
    else {
        a_pEditOptions->AddConstraint(_T("Use"),   (DWORD_PTR)0 ); 
        a_pEditOptions->AddConstraint(_T("Unuse"), (DWORD_PTR)1 );
    }	
}

void KMTransitPathEditDlg::ResizeComponent()
{
	SetResize(IDC_EDIT1,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON4,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_SEPRATOR5,   SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_EDIT2,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,     SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC6,     SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_A,    SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR8,   SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_B,    SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR7,   SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON5,     SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,     SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL, SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	
	SetResize(IDC_BTN_COMPARE_PATH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,     SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,        SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC_USE,  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);    
}


unsigned __stdcall KMTransitPathEditDlg::CollectDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("생성된 대중교통 경로 데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Generated Transit Path Data..."));
	}

	KMTransitPathEditDlg*     pDlg = (KMTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->CollectData();

	return 0;
}


void KMTransitPathEditDlg::CollectData()
{
	TxLogDebugStartMsg();
	try
	{
		if ( !ReadOutPutFile() )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모형 결과 파일 읽기에 실패하였습니다..."));
			}
			else {
				AddStatusMessage(_T("Reading Output File Failed..."));
			}
			ThrowException(_T("Output 정보 입력 실패"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KMTransitPathEditDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

 	m_nxStartNodeID = GetSelStartNodeId();
	if(-1 == m_nxStartNodeID)
	{
		return false;
	}

	CString strDataFileName(_T(""));
	CString strSavePath(_T(""));

	try
	{
		strDataFileName.Format(_T("TransitPath\\%I64d_BusPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ReadBusPathFile(strSavePath);

		strDataFileName.Format(_T("TransitPath\\%I64d_RailPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ReadRailPathFile(strSavePath);
	
		strDataFileName.Format(_T("TransitPath\\%I64d_BusRailPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ReadBusRailPathFile(strSavePath);
	}
	catch (KExceptionPtr ex)
	{
		//AfxMessageBox(ex->GetErrorMessage());
		TxLogDebugException();
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();

	return true;
}


Integer KMTransitPathEditDlg::GetSelStartNodeId()
{
	Integer nxNodeId      = -1;

	try
	{
		nxNodeId = _ttoi64(m_oStartNodeIdCaption.TID);
	}
	catch (KExceptionPtr ex)
	{
		return -1;
		TxLogDebugException();
	}
	catch (...)
	{
		return -1;
		TxLogDebugException();
	}

	return nxNodeId;
}


void KMTransitPathEditDlg::ReadBusPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
	m_mapBusAssing.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOrginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );
				szUseFlagDataPoint                                                         += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KMTransitEditInfo oBusAssignInfo;
					oBusAssignInfo.emTransitAssignMode = KEMTransitAssignBus;
					oBusAssignInfo.nxOrginID           = nxOrginID;
					oBusAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                         += sizeof(int);
					if(ifs.eof())break;

					std::vector< TIndexResultTransitInfo > &vecResultBusInfo = oBusAssignInfo.vecResultBusInfo;
					for (int i= 0; i< BUSPATH_ASSIGNSET_COUNT; i++)
					{
						TIndexResultTransitInfo oTIndexResultTransitInfo;
						oTIndexResultTransitInfo.oIndexKey = KODKey((i+1),0);
						std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
							szUseFlagDataPoint                                                     += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);

							if(0 == oTMResultBusInfo.nxTransitID)
								continue;

							vecLowResultBusInfo.push_back(oTMResultBusInfo);
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TEditBusSubData) );
					szUseFlagDataPoint                                                             += sizeof(TEditBusSubData);
					oBusAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nUseFlag)),                   sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int); 
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
					AutoType iter  = m_mapBusAssing.find(oBusAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapBusAssing.end();

					if( iter != itEnd )
					{
						std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
						vecBusAssignInfo.push_back(oBusAssignInfo);
					}
					else
					{
						std::vector<KMTransitEditInfo> vecBusAssignInfo;
						vecBusAssignInfo.push_back(oBusAssignInfo);
						m_mapBusAssing.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}

				nCount++;
			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapBusAssing.clear();
		ifs.close();

		m_bResult = false;
		m_strErrMsg =_T("ReadBusPathFile 오류");

		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::ReadRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
	m_mapRailAssing.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KMTransitEditInfo oRailAssignInfo;
					oRailAssignInfo.emTransitAssignMode = KEMTransitAssignRail;
					oRailAssignInfo.nxOrginID           = nxOriginID;
					oRailAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);
					if(ifs.eof())break;

					int nBoardingCnt(0); //탑승횟수
					ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);

					if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					std::vector< TIndexResultTransitInfo > &vecResultBusInfo = oRailAssignInfo.vecResultBusInfo;
					for (int i= 0; i< nBoardingCnt; i++)
					{
						int nDuplicateCnt(0); //중복 노선 갯수
						ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
						szUseFlagDataPoint                                                      += sizeof(int);
						if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						TIndexResultTransitInfo oTIndexResultTransitInfo;
						oTIndexResultTransitInfo.oIndexKey = KODKey((1+i),0);
						std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< nDuplicateCnt; k++)
						{
							TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
							szUseFlagDataPoint                                                     += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);

							if(0 == oTMResultBusInfo.nxTransitID)
								continue;

							vecLowResultBusInfo.push_back(oTMResultBusInfo);
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TEditRailSubData) );
					szUseFlagDataPoint                                                             += sizeof(TEditRailSubData);
					oRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nUseFlag)),                  sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
					AutoType iter  = m_mapRailAssing.find(oRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapRailAssing.end();

					if( iter != itEnd )
					{
						std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
						vecBusAssignInfo.push_back(oRailAssignInfo);
					}
					else
					{
						std::vector<KMTransitEditInfo> vecBusAssignInfo;
						vecBusAssignInfo.push_back(oRailAssignInfo);
						m_mapRailAssing.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecBusAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapRailAssing.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadRailPathFile 오류");
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::ReadBusRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusRailAssing.clear();
	size_t szUseFlagDataPoint(0);
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KMTransitEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emTransitAssignMode = KEMTransitAssignBusRail;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
					if(ifs.eof())break;

					std::vector< TIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResultBusInfo;
					for (int i= 0; i< BUSRAILPATH_ASSIGNSET_COUNT; i++)
					{
						TIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
							szUseFlagDataPoint                                                     += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                     += sizeof(int);

							if(0 == oTMResultBusInfo.nxTransitID)
								continue;

							vecLowResultBusInfo.push_back(oTMResultBusInfo);

							if(oTMResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
							{
								nRailIndex = i;
							}
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTBusSubData)),           sizeof(TEditBusSubData) );
					szUseFlagDataPoint                                                             += sizeof(TEditBusSubData);
					oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
						szUseFlagDataPoint                                                             += sizeof(int);

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
								szUseFlagDataPoint                                                      += sizeof(int);

								if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
								{
									throw 1;
								}

								TIndexResultTransitInfo oTIndexResultTransitInfo;
								std::vector<TMResultBusInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
								for (int k= 0; k< nDuplicateCnt; k++)
								{
									TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail, RGB(255,255,255), false};

									ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
									szUseFlagDataPoint                                                     += sizeof(Integer);
									ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
									szUseFlagDataPoint                                                     += sizeof(int);
									ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
									szUseFlagDataPoint                                                     += sizeof(int);

									if(0 == oTMResultBusInfo.nxTransitID)
										continue;

									if(m > 0 && 0 == i)
									{
										oTMResultBusInfo.bDuplicationRail = true;
									}

									vecLowResultRailInfo.push_back(oTMResultBusInfo);
								}
								vecResultRailInfo.push_back(oTIndexResultTransitInfo);
							}
							vecMultiResultRailInfo.push_back(vecResultRailInfo);
						}
#pragma endregion 레일 정보 취합
					}
					
					std::vector< TIndexResultTransitInfo> &vecResultTotalBusInfo = oBusRailAssignInfo.vecResultBusInfo;
					Integer nSingleIdx = 0;
					for (size_t i= 0; i< vecResultBusInfo.size(); i++)
					{
						if(i != nRailIndex)
						{
							TIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
							oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
							vecResultTotalBusInfo.push_back(oTIndexResultTransitInfo);
						}
						else
						{
							size_t nxSize = vecMultiResultRailInfo.size();
							for (size_t m= 0; m< nxSize; m++)
							{
								std::vector< TIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

								for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
								{
									TIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

									nSingleIdx = (i+k+1);
									if (1 == nxSize)
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
									}
									else
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
									}

									vecResultTotalBusInfo.push_back(vecTempResultRailInfo[k]);
								}
							}
						}
					}

#pragma region 취합 정보 등록
					AutoType iter  = m_mapBusRailAssing.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapBusRailAssing.end();

					if( iter != itEnd )
					{
						std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
						vecBusAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KMTransitEditInfo> vecBusAssignInfo;
						vecBusAssignInfo.push_back(oBusRailAssignInfo);
						m_mapBusRailAssing.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapBusRailAssing.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadBusRailPathFile 오류");
		/*ThrowException(_T("ReadBusRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::GetDestinationNodeInfo()
{
	m_vecEndZoneNodeIDCaption.clear();
	m_mapEndZoneNodeData.clear();

	m_bResult = true;
	m_strErrMsg =_T("");

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(CollectDataThreadCaller, this, false, nLang);

	if (!m_bResult)
	{
		//AfxMessageBox(m_strErrMsg);
	}

	try
	{
		AutoType iter  = m_mapZoneNodeData.begin();
		AutoType itEnd = m_mapZoneNodeData.end();

		KIDCaption oIDCaption;
		CString    strZoneID(_T(""));

		while(iter != itEnd)
		{
			Integer nxDzoneID    = iter->first;
			CString &strZoneName = iter->second;

			std::vector<KMTransitEditInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMTransitEditInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMTransitEditInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData(nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData(nxDzoneID);
			pvecBusRailAssignInfo = IsHaveBusRailAssignData(nxDzoneID);

			if (nullptr == pvecBusAssignInfo && nullptr == pvecRailAssignInfo && nullptr == pvecBusRailAssignInfo)
			{
				++iter;
				continue;
			}

			strZoneID.Format(_T("%I64d"), nxDzoneID);
			oIDCaption.TID      = strZoneID;
			oIDCaption.TCaption = strZoneName;

			m_vecEndZoneNodeIDCaption.push_back(oIDCaption);
			m_mapEndZoneNodeData.insert(std::make_pair(nxDzoneID, strZoneName));

			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		m_vecEndZoneNodeIDCaption.clear();
		m_mapEndZoneNodeData.clear();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		m_vecEndZoneNodeIDCaption.clear();
		m_mapEndZoneNodeData.clear();
		return;
	} 
}


void KMTransitPathEditDlg::UpdateReportAData()
{
	m_wndReportACtrl.ResetContent();

	bool bAllSelected(FALSE);
	if( IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED )
		bAllSelected = TRUE;
	else
		bAllSelected = FALSE;

	try
	{
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString strZoneID(_T(""));
		CString strDZoneNode(_T(""));

		AutoType iter  = m_mapEndZoneNodeData.begin();
		AutoType itEnd = m_mapEndZoneNodeData.end();

		while(iter != itEnd)
		{
			Integer nxDzoneID    = iter->first;
			CString &strZoneName = iter->second;
			strZoneID.Format(_T("%I64d"), nxDzoneID);

			if (m_setSearchedID.find(strZoneID) == m_setSearchedID.end())
			{
				++iter;
				continue;
			}

			std::vector<KMTransitEditInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMTransitEditInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMTransitEditInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData    (nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData   (nxDzoneID);
			pvecBusRailAssignInfo = IsHaveBusRailAssignData(nxDzoneID);

			if (nullptr == pvecBusAssignInfo && nullptr == pvecRailAssignInfo && nullptr == pvecBusRailAssignInfo)
			{
				++iter;
				continue;
			}

			pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(bAllSelected);

			if(strZoneName.Compare(_T("")) == 0)
				strZoneName = _T("-");
			strDZoneNode.Format(_T("%I64d[%s]"), nxDzoneID, strZoneName);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDZoneNode));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nxDzoneID);

			CString strCheck(_T(""));			
			strCheck = (pvecBusAssignInfo) == nullptr ? _T("-") : _T("v");
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)pvecBusAssignInfo);

			strCheck = (pvecRailAssignInfo) == nullptr ? _T("-") : _T("v");
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)pvecRailAssignInfo);

			strCheck = (pvecBusRailAssignInfo) == nullptr ? _T("-") : _T("v");
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)pvecBusRailAssignInfo);

			++iter;
		}

		m_wndReportACtrl.Populate();

		UpdateReportBData();
		UpdatePathCompareButtonState();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


std::vector<KMTransitEditInfo>* KMTransitPathEditDlg::IsHaveBusAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusAssing.find(a_nxZoneID);
	AutoType itEnd = m_mapBusAssing.end();

	if(iter != itEnd)
	{
		std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}

std::vector<KMTransitEditInfo>* KMTransitPathEditDlg::IsHaveRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapRailAssing.find(a_nxZoneID);
	AutoType itEnd = m_mapRailAssing.end();

	if(iter != itEnd)
	{
		std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


std::vector<KMTransitEditInfo>* KMTransitPathEditDlg::IsHaveBusRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusRailAssing.find(a_nxZoneID);
	AutoType itEnd = m_mapBusRailAssing.end();

	if(iter != itEnd)
	{
		std::vector<KMTransitEditInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


void KMTransitPathEditDlg::UpdateReportBData()
{
	m_wndReportBCtrl.ResetContent();

	if(nullptr != m_spMultiModalResultDetailDlg)
	{
		m_spMultiModalResultDetailDlg.reset();
	}

	// 2019.03.12 명지대 요청사항 수정
	if (nullptr != m_spPathCompareDlg)
	{
		m_spPathCompareDlg.reset();
	}
	// 2019.03.12 명지대 요청사항 수정 끝
//	AllClearPath(true);

	CWaitCursor wc;
	BOOL bUsedBus     = IsDlgButtonChecked(IDC_CHECK_B);
	BOOL bUsedRail    = IsDlgButtonChecked(IDC_CHECK_R);
	BOOL bUsedBusRail = IsDlgButtonChecked(IDC_CHECK_BR);
	std::vector<BOOL> vecUsedTraint;
	vecUsedTraint.push_back(bUsedBus);
	vecUsedTraint.push_back(bUsedRail);
	vecUsedTraint.push_back(bUsedBusRail);

	try
	{
		CXTPReportRecord*         pParentRecord    = nullptr;
		CXTPReportRecords*        pSeParentRecords = nullptr;
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecords*        pThParentRecords = nullptr;
		CXTPReportRecord*         pThParentRecord  = nullptr;
		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		CXTPReportRecords* pARecords = m_wndReportACtrl.GetRecords();
		int nCount       = pARecords->GetCount();
		int nSelectCount = 0;

		for (int i= 0; i<nCount; i++)
		{
			CXTPReportRecord*     pARecord  = pARecords->GetAt(i);
			CXTPReportRecordItem* pItemA    = pARecord->GetItem(_0_A_COLUMN_SELECT);

			if( pItemA->IsChecked() == FALSE )
				continue;

			nSelectCount++;

			pItemA            = pARecord->GetItem(_1_A_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItemA->GetItemData();
			pItemA            = pARecord->GetItem(_2_A_COLUMN_ISBUS);
			std::vector<KMTransitEditInfo>*    pvecBusAssignInfo     = (std::vector<KMTransitEditInfo>*)pItemA->GetItemData();
			pItemA            = pARecord->GetItem(_3_A_COLUMN_ISRAIL);
			std::vector<KMTransitEditInfo>*    pvecRailAssignInfo    = (std::vector<KMTransitEditInfo>*)pItemA->GetItemData();
			pItemA            = pARecord->GetItem(_4_A_COLUMN_ISBUSRAIL);
			std::vector<KMTransitEditInfo>*    pvecBusRailAssignInfo = (std::vector<KMTransitEditInfo>*)pItemA->GetItemData();

			//#1 존 ID
			pParentRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			CString strZoneName(_T(""));
			AutoType iter  = m_mapZoneNodeData.find(nxDZoneID);
			AutoType itEnd = m_mapZoneNodeData.end();
			if(iter != itEnd)
			{
				strZoneName = iter->second;
			}
			if(strZoneName.Compare(_T("")) == 0)
				strZoneName = _T("-");

			CString strDZone(_T(""));
			strDZone.Format(_T("%I64d[%s]"), nxDZoneID, strZoneName);
			pItem = pParentRecord->AddItem(new CXTPReportRecordItemText(strDZone));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pItem->SetAlignment(DT_LEFT);
			pItem->SetEditable(FALSE);
			pItem->SetItemData((DWORD_PTR)nxDZoneID);

			//#2 Type
			pSeParentRecords = pParentRecord->GetChilds();
			for (size_t k= 0; k< vecUsedTraint.size(); k++)
			{
				if(FALSE == vecUsedTraint[k])
					continue;

				int     nMode;
				CString strType(_T(""));
				std::vector<KMTransitEditInfo>* pvecAssignInfo = nullptr;
				if(0 == k)
				{
					strType = _T("Bus");
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strType = _T("버스");
					}
					nMode = KENUMModeBus;
					pvecAssignInfo = pvecBusAssignInfo;
				}
				else if(1 == k)
				{
					strType = _T("Rail");
                    if (KmzSystem::GetLanguage() == KEMKorea) {
                        strType = _T("도시철도");
                    }

					nMode = KENUMModeRail;
					pvecAssignInfo = pvecRailAssignInfo;
				}
				else
				{
					strType = _T("Bus+Rail");
                    if (KmzSystem::GetLanguage() == KEMKorea) {
                        strType = _T("버스+도시철도");
                    }
					nMode = KENUMModeBusRail;
					pvecAssignInfo = pvecBusRailAssignInfo;
				}

				if(nullptr == pvecAssignInfo)
					continue;

				pSeParentRecord = pSeParentRecords->Add(new CXTPReportRecord());
				pParentCheckItem = pSeParentRecord->AddItem(new CXTPReportRecordItemText(strType));
				for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
				{
					CXTPReportRecordItem*            pItemEmpty   = pSeParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
					CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
					pEditOptions->RemoveButtons();
					pItemEmpty->SetEditable(FALSE);
				}
				pParentCheckItem->SetAlignment(DT_LEFT);
				pParentCheckItem->HasCheckbox(TRUE);
				pParentCheckItem->SetItemData((DWORD_PTR)nMode);

// 				if(nullptr == pvecAssignInfo)
// 					continue;

				pThParentRecords = pSeParentRecord->GetChilds();
				int nChildCheckCount = 0;
				for (size_t m = 0; m < (*pvecAssignInfo).size(); m++)
				{
					KMTransitEditInfo &oBusAssignInfo = (*pvecAssignInfo)[m];

					pThParentRecord = pThParentRecords->Add(new CXTPReportRecord());
					//#3
					CString strDisplay(_T(""));
					CString strIndex(_T(""));
					
					strIndex.Format(_T("%d"),     m+1);
					pItemText = (CXTPReportRecordItemText*)pThParentRecord->AddItem(new CXTPReportRecordItemText());
					pItemText->SetAlignment(DT_LEFT);
					pItemText->HasCheckbox(TRUE);
					if( 0 == oBusAssignInfo.nUseFlag )// 이상 무
					{
						pItemText->SetChecked(TRUE);
						strDisplay.Format(_T("#P%d"), m+1);
                        if (KmzSystem::GetLanguage() == KEMKorea) {
                            strDisplay.Format(_T("경로%d"), m+1);
                        }

						nChildCheckCount++;
					}
					else
					{
						pItemText->SetChecked(FALSE);
						strDisplay.Format(_T("#P%d"), m+1);
                        if (KmzSystem::GetLanguage() == KEMKorea) {
                            strDisplay.Format(_T("경로%d"), m+1);
                        }
					}
					
					pItemText->SetItemData((DWORD_PTR)&oBusAssignInfo);
					pItemText->SetValue(strIndex);
					pItemText->SetCaption(strDisplay);

					//#4
					double dCost(0.0);
					double dTotalTime(0.0);
					double dLength(0.0);
					if(0 == k)
					{
						dCost            = oBusAssignInfo.oTBusSubData.dCost;
						dLength          = oBusAssignInfo.oTBusSubData.dLength;
						SumTotalTime(true, oBusAssignInfo, dTotalTime);
					}
					else if(1 == k)
					{
						dCost            = oBusAssignInfo.oTRailSubData.dCost;
						dLength          = oBusAssignInfo.oTRailSubData.dLength;
						SumTotalTime(false, oBusAssignInfo, dTotalTime);
					}
					else
					{
						dCost            = oBusAssignInfo.oTBusSubData.dCost;
						dLength          = oBusAssignInfo.oTBusSubData.dLength;
						SumTotalTime(true, oBusAssignInfo, dTotalTime);
					}

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dCost, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);

					pItem = pThParentRecord->AddItem(new CCustomReportRecordItem);
					pItem->SetAlignment(DT_LEFT);
					if( 0 == oBusAssignInfo.nUseFlag )// 이상 무
					{
						if (KmzSystem::GetLanguage() == KEMKorea) {
							pItem->SetCaption(_T("씀"));
						}
						else {
							pItem->SetCaption(_T("Use"));
						}
						pItem->SetItemData((DWORD_PTR)0);
					}
					else
					{
						if (KmzSystem::GetLanguage() == KEMKorea) {
							pItem->SetCaption(_T("안씀"));
						}
						else {
							pItem->SetCaption(_T("Unuse"));
						}
						
						pItem->SetItemData((DWORD_PTR)1);
					}

					//#5
					pItem = pThParentRecord->AddItem(new CXTPReportRecordItem());
					CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                    if (KmzSystem::GetLanguage() == KEMKorea) {
                        pButton->SetAlignment(xtpItemControlUnknown);
                        pButton->SetCaption(_T("보기"));
                        pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
                    }
                    else {
                        pButton->SetAlignment(xtpItemControlUnknown);
                        pButton->SetCaption(_T("Info"));
                        pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
                    }					
				}

				if (nChildCheckCount > 0)
					pParentCheckItem->SetChecked(TRUE);
				else
					pParentCheckItem->SetChecked(FALSE);
			}
		}

		m_wndReportBCtrl.Populate();
		if(nSelectCount < EXPANDALL_LIMIT_COUNT) // all 체크한 데이터를 모두 expandall 하기엔 부하가 걸린다.
		{
			m_wndReportBCtrl.ExpandAll(TRUE);
		}

		OnBnClickedButtonDraw();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


void KMTransitPathEditDlg::SumTotalTime(bool a_bBus, KMTransitEditInfo &a_oBusAssignInfo, double &a_dTotalTime)
{
	if (a_bBus)
	{
		a_dTotalTime = a_oBusAssignInfo.oTBusSubData.dInVehTime 
			+ a_oBusAssignInfo.oTBusSubData.dWaitTime 
			+ a_oBusAssignInfo.oTBusSubData.dTransferTime 
			+ a_oBusAssignInfo.oTBusSubData.dAccessWalkTime 
			+ a_oBusAssignInfo.oTBusSubData.dEgressWalkTime;
	}
	else
	{
		a_dTotalTime = a_oBusAssignInfo.oTRailSubData.dInVehTime 
			+ a_oBusAssignInfo.oTRailSubData.dWaitTime 
			+ a_oBusAssignInfo.oTRailSubData.dTransferTime 
			+ a_oBusAssignInfo.oTRailSubData.dAccessWalkTime 
			+ a_oBusAssignInfo.oTRailSubData.dEgressWalkTime;
	}
}


void KMTransitPathEditDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxOrgZoneID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxOrgZoneID);

				CString    strNodeView(_T(""));
				KIDCaption oIDCaption;
				SetNodeViewData(m_mapZoneNodeData, strID, strNodeView, oIDCaption);

				m_oStartNodeIdCaption = oIDCaption;
				SetDlgItemText(IDC_EDIT1,  strNodeView);
				
				bool bRefresh(false);
				AllClearPath(bRefresh);
				
#pragma region 도착 정보 초기화
				m_setSearchedID.clear();
				m_strSearchedIDs = _T("");
				SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);
				if (nullptr != m_spImTasMultiIDFind)
				{
					m_spImTasMultiIDFind.reset();
				}
#pragma endregion 도착 정보 초기화

				m_wndReportACtrl.ResetContent();
				m_wndReportBCtrl.ResetContent();
				GetDestinationNodeInfo();
				UpdateSearchDataCount();
				GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);
				
				if (m_pMapView)
				    m_pMapView->PanToFeature(KLayerID::Node(), nxOrgZoneID, bRefresh);

				bRefresh = true;
 				DrawOrgZoneAccess(nxOrgZoneID, bRefresh);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
	else if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				// all check
				CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
				CheckDlgButton(IDC_CHECK_B, BST_CHECKED);
				CheckDlgButton(IDC_CHECK_R, BST_CHECKED);
				CheckDlgButton(IDC_CHECK_BR, BST_CHECKED);

				bool bRefresh(false);
				m_setSearchedID.clear();
				m_setSearchedID  = m_spImTasMultiIDFind->GetSelectedIDSet();
				m_strSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();
				SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);

				// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
				int nLang = 1;
				if (KmzSystem::GetLanguage() == KEMKorea)
				{
					nLang = 0;
				}

				QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this, false, nLang);

				UpdateReportAData();

				UpdateSearchDataCount();
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KMTransitPathEditDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::map<Integer, std::map<int, std::vector<KMTransitEditInfo>*>> mapDestMAssignInfo;

		AutoType iter  = m_mapEndZoneNodeData.begin();
		AutoType itEnd = m_mapEndZoneNodeData.end();

		CString strZoneID(_T(""));
		for (; iter != itEnd; ++iter)
		{
			Integer nxDzoneID = iter->first;
			strZoneID.Format(_T("%I64d"), nxDzoneID);

			if (m_setSearchedID.find(strZoneID) == m_setSearchedID.end()) {
				continue;
			}

			std::vector<KMTransitEditInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMTransitEditInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMTransitEditInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData    (nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData   (nxDzoneID);
			pvecBusRailAssignInfo = IsHaveBusRailAssignData(nxDzoneID);

			if (nullptr == pvecBusAssignInfo && nullptr == pvecRailAssignInfo && nullptr == pvecBusRailAssignInfo) {
				continue;
			}

			std::map<int, std::vector<KMTransitEditInfo>*> &mapModeMAssignInfo = mapDestMAssignInfo[nxDzoneID];

			if (pvecBusAssignInfo != nullptr) {
				mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeBus, pvecBusAssignInfo));
			}

			if (pvecRailAssignInfo != nullptr) {
				mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeRail, pvecRailAssignInfo));
			}

			if (pvecBusRailAssignInfo != nullptr) {
				mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeBusRail, pvecBusRailAssignInfo));
			}
		}

		m_mapDestResultInputValue.clear();
		KMultiModalResultInfo::ResultTransitEditInfo2(m_pTarget, mapDestMAssignInfo, OUT m_mapDestResultInputValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh)
{
	CString strNodeLable(_T(""));
	strNodeLable.Format(_T("%I64d"), a_nxID);

	std::vector<TDrawZoneAccess> vecZoneAccess;

	TDrawZoneAccess oDrawZoneAccess;
	oDrawZoneAccess.TNodeID         = a_nxID;
	oDrawZoneAccess.TSymbol			= 48;//임시 모양
	oDrawZoneAccess.TColorRGB       = RGB(255,0,0);
	oDrawZoneAccess.TLabel          = strNodeLable;
	oDrawZoneAccess.TSize           = 18.0; //임시 사이즈
	oDrawZoneAccess.TRadius         = m_dRadius;

	vecZoneAccess.push_back(oDrawZoneAccess);

	m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
}


void KMTransitPathEditDlg::DrawDestZoneAccess(bool a_bRefresh)
{
	CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
	int nCount = pRecords->GetCount();

	if (nCount < 1) {
		return;
	}
	
	std::vector<TDrawZoneAccess> vecZoneAccess;

	Integer nxOrgZoneID = _ttoi64(m_oStartNodeIdCaption.TID);

	// Org 그리기
	TDrawZoneAccess oDrawZoneAccess;
	oDrawZoneAccess.TNodeID         = nxOrgZoneID;
	oDrawZoneAccess.TSymbol			= 48;//임시 모양
	oDrawZoneAccess.TColorRGB       = RGB(255,0,0);
	oDrawZoneAccess.TLabel          = m_oStartNodeIdCaption.TID;
	oDrawZoneAccess.TSize           = 18.0; //임시 사이즈
	oDrawZoneAccess.TRadius         = m_dRadius;

	vecZoneAccess.push_back(oDrawZoneAccess);

	// Dest 그리기
	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);
		if (pItem->IsChecked() == FALSE) {
			continue;
		}

		pItem             = pRecord->GetItem(_1_A_COLUMN_ZONEID);
		Integer nxDZoneID = (Integer)pItem->GetItemData();

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), nxDZoneID);

		oDrawZoneAccess.TNodeID         = nxDZoneID;
		oDrawZoneAccess.TColorRGB       = RGB(0,0,205);
		oDrawZoneAccess.TLabel          = strNodeLable;

		vecZoneAccess.push_back(oDrawZoneAccess);
	}

	m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
}


void KMTransitPathEditDlg::OnBnClickedButtonClear()
{
	AllClearPath(true);
}


void KMTransitPathEditDlg::AllClearPath(bool a_bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(a_bRefresh);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMTransitPathEditDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ALL);    

		CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
		int nCount = pRecords->GetCount();

		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_wndReportACtrl.RedrawControl();

		UpdateReportBData();
		UpdatePathCompareButtonState();
	}
	catch (...)
	{
		return;
	}
}


void KMTransitPathEditDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_A_COLUMN_SELECT == nIndex)
	{
		CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
		int nTotalCount = pRecords->GetCount();

		int nCheckedCount = 0;
		for (int i=0; i<nTotalCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if (pItem->IsChecked() == TRUE)
				nCheckedCount++;
		}

		if(nCheckedCount == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
		if(nCheckedCount < nTotalCount || 0 == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);

		UpdateReportBData();
		UpdatePathCompareButtonState();
	}
	else
	{
		CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pItem   = pRecord->GetItem(_1_A_COLUMN_ZONEID);
		Integer               nxNodeID= pItem->GetItemData();
        
		PanToNodeFeature(nxNodeID);
	}
}


void KMTransitPathEditDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{


}


void KMTransitPathEditDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_B_COLUMN_ZONEID == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItem->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == TRUE)
			{
				BOOL bParentChecked = pItem->IsChecked();

				CXTPReportRecords* pChdRecords = pRecord->GetChilds();
				int nCount = pChdRecords->GetCount();
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*          pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItemText*  pChdItem   = (CXTPReportRecordItemText*)pChdRecord->GetItem(_0_B_COLUMN_ZONEID);
					pChdItem->SetChecked(bParentChecked);
					CString strIndex         = pChdItem->GetValue();
					CString strCaption(_T(""));

					KMTransitEditInfo* pBusAssignInfo = (KMTransitEditInfo*)pChdItem->GetItemData();
					if (TRUE == bParentChecked)
					{
						strCaption.Format(_T("#P%s"), strIndex);
						if (KmzSystem::GetLanguage() == KEMKorea) {
							strCaption.Format(_T("경로%s"), strIndex);
						}
						pChdItem->SetCaption(strCaption);
					}
					else
					{
						strCaption.Format(_T("#P%s"), strIndex);
						if (KmzSystem::GetLanguage() == KEMKorea) {
							strCaption.Format(_T("경로%s"), strIndex);
						}
						pChdItem->SetCaption(strCaption);
					}
				}
				m_wndReportBCtrl.RedrawControl();
			}
			else
			{
				BOOL bChdChecked = pItem->IsChecked();
				KMTransitEditInfo* pBusAssignInfo = (KMTransitEditInfo*)pItem->GetItemData();

				CString strIndex = pItem->GetValue();
				CString strCaption(_T(""));
				if (TRUE == bChdChecked)
				{
					strCaption.Format(_T("#P%s"), strIndex);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strCaption.Format(_T("경로%s"), strIndex);
					}
					pItem->SetCaption(strCaption);
				}
				else
				{
					strCaption.Format(_T("#P%s"), strIndex);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strCaption.Format(_T("경로%s"), strIndex);
					}
					pItem->SetCaption(strCaption);
				}
				m_wndReportBCtrl.RedrawControl();
			}

			CXTPReportRecord*   pParentRecord = pRecord->GetParentRecord();
			BOOL bCheked = pItem->IsChecked();

			CXTPReportRecords*  pChdRecords   = pParentRecord->GetChilds();
			int nCount        = pChdRecords->GetCount();
			int nCheckedCount = 0;
			for (int i= 0; i< nCount; i++)
			{
				CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
				CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_B_COLUMN_ZONEID);
				if( pChdItem->IsChecked() == TRUE)
				{
					nCheckedCount++;
				}
			}

			CXTPReportRecordItem*  pParentItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			pParentItem->SetChecked(bCheked);

			if (nCheckedCount > 0)
			{
				pParentItem->SetChecked(TRUE);
			}
		}
		catch (...)
		{
			return;
		}

		OnBnClickedButtonDraw();
	}
	else if ( _4_B_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_4_B_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;
			
			if(pRecord->HasChildren() == FALSE)
			{
				int                nUseFlag         = pItem->GetItemData();
				KMTransitEditInfo* pTransitEditInfo = (KMTransitEditInfo*)pItemCheck->GetItemData(); 
				if(0 == nUseFlag)
				{
					pTransitEditInfo->nUseFlag = 0;
				}
				else
				{
					pTransitEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KMTransitPathEditDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	//OnReportBItemClick 안에 있는 내용을 이쪽으로 이동 시켜야 한다.

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	try
	{
		if ( nIndex == _0_B_COLUMN_ZONEID )
		{
			int nTest = 0;
			nTest = 9;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMTransitPathEditDlg::OnReportBValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( _4_B_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_4_B_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == FALSE)
			{
				int                nUseFlag         = pItem->GetItemData();
				KMTransitEditInfo* pTransitEditInfo = (KMTransitEditInfo*)pItemCheck->GetItemData(); 
				if(0 == nUseFlag)
				{
					pTransitEditInfo->nUseFlag = 0;
				}
				else
				{
					pTransitEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KMTransitPathEditDlg::PanToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
		m_pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMTransitPathEditDlg::OnBnClickedCheckB()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMTransitPathEditDlg::OnBnClickedCheckR()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMTransitPathEditDlg::OnBnClickedCheckBr()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMTransitPathEditDlg::OnBnClickedButtonDraw()
{
	TxLogDebugStartMsg();
	try
	{
		AllClearPath(false);

		CWaitCursor cw;
		SelectDrawData();
		//QBicSimpleProgressThread::ExecuteThread(SelectDrawDataThreadCaller, this);

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, true,  false);
		else
			m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false, false);

		m_pMapView->TxDrawCompositeLink    (m_oMapResult.vecDrawLinkInfo, false, false);
		//존->정류장 도보제거 요청 : 2013.03.03-명지대학교
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, false, false);

		DrawDestZoneAccess(false); // org와 Dest들의 존엑세스를 그린다.

		m_pMapView->MapRefresh();
	}
	catch(...)
	{
		return;
	}
	TxLogDebugEndMsg();
}


unsigned __stdcall KMTransitPathEditDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KMTransitPathEditDlg*     pDlg = (KMTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


unsigned __stdcall KMTransitPathEditDlg::SelectDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KMTransitPathEditDlg*     pDlg = (KMTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->SelectDrawData();

	return 0;
}


void KMTransitPathEditDlg::SelectDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::map<Integer, std::map<int, std::set<int>>> mapZoneModePIndex;

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
			pItem             = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if( pItem->GetHasCheckbox() == FALSE )
					continue;
				if( pItem->IsChecked() == FALSE )
					continue;

				int nMode = (int)pItem->GetItemData();

				CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
				for (int m= 0; m<  pThParentRecords->GetCount(); m++)
				{
					CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
					pItem            = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

					if( pItem->IsChecked() == FALSE )
						continue;

					std::map<int, std::set<int>> &mapModePIndex = mapZoneModePIndex[nxDZoneID];
					std::set<int>                &setPIndex     = mapModePIndex[nMode];
					setPIndex.insert(m);
				}
			}
		}

		KMultiModalResultInfo::ResultTransitEditInfo3(m_pTarget, m_mapDestResultInputValue, mapZoneModePIndex, OUT m_oMapResult);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}


void KMTransitPathEditDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if(_5_B_COLUMN_DETAIL == pItem->GetIndex())
	{
		CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_B_COLUMN_ZONEID);
		KMTransitEditInfo*    pBusAssignInfo = (KMTransitEditInfo*)pItemB->GetItemData();
		CString               strPathIdex    = pItemB->GetCaption(NULL);

		CRect rect;
		GetWindowRect(rect);

		if(nullptr != m_spMultiModalResultDetailDlg)
		{
			m_spMultiModalResultDetailDlg->InitializeEditData(strPathIdex, pBusAssignInfo, rect);
			m_spMultiModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spMultiModalResultDetailDlg = KMultiModalResultDetailDlgPtr(new KMultiModalResultDetailDlg(m_pTarget, false, m_mapNodeName, this));
			m_spMultiModalResultDetailDlg->Create(KMultiModalResultDetailDlg::IDD, this);
			m_spMultiModalResultDetailDlg->InitializeEditData(strPathIdex, pBusAssignInfo, rect);
			m_spMultiModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
	}
}


void KMTransitPathEditDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}


void KMTransitPathEditDlg::CloseWindowProc()
{
	AllClearPath(true);

	if (nullptr != m_spMultiModalResultDetailDlg)
		m_spMultiModalResultDetailDlg.reset();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	if (nullptr != m_spPathCompareDlg)
	{
		m_spPathCompareDlg.reset();
	}
}


void KMTransitPathEditDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spMultiModalResultDetailDlg)
	{
		if(!m_spMultiModalResultDetailDlg->IsUserClosed())
		{
			if(!m_spMultiModalResultDetailDlg->IsWindowVisible())
				m_spMultiModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
		m_spMultiModalResultDetailDlg->ActivatedFrameWndNotifyProcess();
	}

	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(!m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
		}
	}

	// 2019.03.12 명지대 요청사항 수정
	if (nullptr != m_spPathCompareDlg)
	{
		if (!m_spPathCompareDlg->IsUserClosed())
		{
			if (!m_spPathCompareDlg->IsWindowVisible())
			{
				m_spPathCompareDlg->ShowWindow(SW_SHOW);
			}
		}
	}
	// 2019.03.12 명지대 요청사항 수정 끝
}


void KMTransitPathEditDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spMultiModalResultDetailDlg)
	{
		if(!m_spMultiModalResultDetailDlg->IsUserClosed())
		{
			if(m_spMultiModalResultDetailDlg->IsWindowVisible())
				m_spMultiModalResultDetailDlg->ShowWindow(SW_HIDE);
		}
		m_spMultiModalResultDetailDlg->DeActivatedFrameWndNotifyProcess();
	}

	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
		}
	}

	// 2019.03.12 명지대 요청사항 수정
	if (nullptr != m_spPathCompareDlg)
	{
		if (!m_spPathCompareDlg->IsUserClosed())
		{
			if (m_spPathCompareDlg->IsWindowVisible())
			{
				m_spPathCompareDlg->ShowWindow(SW_HIDE);
			}
		}
	}
	// 2019.03.12 명지대 요청사항 수정 끝
}


void KMTransitPathEditDlg::OnBnClickedCheckLabel()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeNodeLabel(true);
		}
		else
		{
			m_pMapView->TxDrawCompositeNodeLabel(false);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMTransitPathEditDlg::OnBnClickedButtonApply()
{
	if(-1 == m_nxStartNodeID)
	{
		return;
	}

	if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	CString strDataFileName(_T(""));
	CString strSavePath(_T(""));

	try
	{
		strDataFileName.Format(_T("TransitPath\\%I64d_BusPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ApplyChangedUseFlag(strSavePath, m_mapBusAssing);

		strDataFileName.Format(_T("TransitPath\\%I64d_RailPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ApplyChangedUseFlag(strSavePath, m_mapRailAssing);

		strDataFileName.Format(_T("TransitPath\\%I64d_BusRailPath.dat"), m_nxStartNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, strDataFileName);
		ApplyChangedUseFlag(strSavePath, m_mapBusRailAssing);

        AfxMessageBox(_T("변경사항이 저장되었습니다."));
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMTransitPathEditDlg::ApplyChangedUseFlag(CString a_strFileName, std::map<Integer, std::vector<KMTransitEditInfo>> &a_mapTransitAssing)
{
	std::ofstream ofs(a_strFileName, std::ios::binary | std::ios::out | std::ios::in);
	try
	{
		if (! ofs)
		{
			return;
		}

		AutoType iter  = a_mapTransitAssing.begin();
		AutoType itEnd = a_mapTransitAssing.end();

		while(iter != itEnd)
		{
			std::vector<KMTransitEditInfo> &vecMTransitEditInfo = iter->second;

			for (size_t i= 0; i< vecMTransitEditInfo.size(); i++)
			{
				KMTransitEditInfo &oTransitEditInfo = vecMTransitEditInfo[i];

				ofs.seekp(oTransitEditInfo.szUseFlagDataPoint, std::ios::beg);
				ofs.write(reinterpret_cast<char*>(&oTransitEditInfo.nUseFlag), sizeof(int));
			}
			++iter;
		}

		ofs.close();
	}
	catch(...)
	{
		ofs.close();
		TxLogDebugException();
	}
}


void KMTransitPathEditDlg::TempApply()
{
// 	CString strFileName = _T("jykTest.dat");
// 	/*std::ofstream ofs(strFileName, std::ios::binary | std::ios::app | std::ios::ate );*/
// 
// 	//std::ofstream ofs(strFileName,std::ios::out | std::ios::in | std::ios::app /*| std::ios::out| std::ios::ate | std::ios::app*/);
// 	std::ofstream ofs(strFileName,std::ios::binary | std::ios::out | std::ios::in);
// 	if (! ofs)
// 	{
// 		return;
// 	}
// 
// 
// 	double     nValue1(9);
// 	double nxValue2(99);
// 	double  dValue3(999.0);
// 	/*ofs.seekp( 1 * sizeof(int), std::ios_base::beg );*/
// 	//ofs.seekp( 1 * sizeof(int), std::ios_base::beg  );
// 	//ofs.seekp( 1 * sizeof(Integer), std::ios_base::cur );
// 	ofs.seekp( 1 * sizeof(double));
// 
// 	//ofs.write(reinterpret_cast<char*>(&nValue1),  sizeof(int));
// 	ofs.write(reinterpret_cast<char*>(&nxValue2), sizeof(double));
// 
// 	ofs.seekp(24, std::ios::cur);
// 
// 	//ofs.write(reinterpret_cast<char*>(&nValue1),  sizeof(int));
// 	ofs.write(reinterpret_cast<char*>(&nxValue2), sizeof(double));
// 	//ofs.write(reinterpret_cast<char*>(&dValue3),  sizeof(double));
// 
// 	ofs.close();
// 
// 
// 	return;
}


BOOL KMTransitPathEditDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			ClickedEditStartNode();

			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT2)->m_hWnd)
		{
			SelectEndNodeList();
			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KMTransitPathEditDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KMTransitPathEditDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KMTransitPathEditDlg::SelectStartNodeList()
{
	try
	{
		CWaitCursor cw;

		if (nullptr != m_spImTasIDFind)
			m_spImTasIDFind.reset();

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

// 		Integer nxNodeID = _ttoi64(m_oStartNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
		m_spImTasIDFind->SetSelectedID(-1);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KMTransitPathEditDlg::OnBnClickedButtonEndNodeSearch()
{
	SelectEndNodeList();
}


void KMTransitPathEditDlg::SelectEndNodeList()
{
	try
	{
		CWaitCursor cw;

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();
		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(m_vecEndZoneNodeIDCaption);
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapEndZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

		m_spImTasMultiIDFind->SetSelectedID(m_strSearchedIDs);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KMTransitPathEditDlg::ModelessCancleFeedback(UINT nDialogID)
{

}


void KMTransitPathEditDlg::OnBnClickedBtnComparePath()
{
	std::vector<KMTransitEditInfoWrapper> arr;

	try
	{
		std::map<Integer, std::map<int, std::set<int>>> mapZoneModePIndex;

		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k = 0; k< pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if (pItem->GetHasCheckbox() == FALSE)
					continue;
				if (pItem->IsChecked() == FALSE)
					continue;

				int nMode = (int)pItem->GetItemData();

				CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
				for (int m = 0; m< pThParentRecords->GetCount(); m++)
				{
					CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
					pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

					if (pItem->IsChecked() == FALSE)
						continue;

					KMTransitEditInfo* pInfo = (KMTransitEditInfo*)pItem->GetItemData();
					if (NULL != pInfo)
					{
						KMTransitEditInfoWrapper wr;
						std::map<Integer, CString>::iterator itNode = m_mapZoneNodeData.find(pInfo->nxDestinationID);
						if (m_mapZoneNodeData.end() != itNode)
						{
							wr.strDestNode.Format(_T("%I64d [%s]"), pInfo->nxDestinationID, itNode->second);
						}
						else
						{
							wr.strDestNode.Format(_T("%I64d [-]"), pInfo->nxDestinationID);
						}
						wr.strPathName = pItem->GetCaption();
						wr.pInfo = pInfo;
						arr.push_back(wr);
					}
				}
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}


	if (nullptr != m_spPathCompareDlg)
	{
		m_spPathCompareDlg.reset();
	}

	m_spPathCompareDlg = std::make_shared<KSelectedPathCompareUrbanDlg>(true, EMPathCompareMode_Multimodal, this);
	m_spPathCompareDlg->SetTransitEditInfos(arr);
	m_spPathCompareDlg->Create(IDD_6929_KSelectedPathCompareUrban, this);
	m_spPathCompareDlg->ShowWindow(SW_SHOW);

}


void KMTransitPathEditDlg::UpdatePathCompareButtonState()
{
	BOOL bEnableButton = FALSE;
	CXTPReportRecordItem*  pItem = nullptr;
	CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
	int nCount = pRecords->GetCount();
	for (int i = 0; i< nCount; i++)
	{
		CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
		pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);

		CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
		for (int k = 0; k < pSeParentRecords->GetCount(); k++)
		{
			CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
			pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			if ((pItem->GetHasCheckbox() == FALSE) || (pItem->IsChecked() == FALSE))
			{
				continue;
			}
			else
			{
				bEnableButton = TRUE;
			}

			CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
			for (int m = 0; m< pThParentRecords->GetCount(); m++)
			{
				CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
				pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if (pItem->IsChecked() == FALSE)
				{
					continue;
				}
				else
				{
					bEnableButton = TRUE;
				}
			}
		}
	}

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(bEnableButton);
}