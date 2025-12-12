// MultimodalResultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultimodalResultDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//^#include "DBaseConnector.h"
#include "KExRecordItem.h"
//#include "StringChecker.h"
#include "NodeSelectionInfo.h"
#include "ImChampMapDefine.h"
#include "KDBaseAccessibilityModify.h"



const int COLUMN_WIDTH_INFO = 50;	// 상세정보 컬럼 너비


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
	_3_B_COLUMN_TRIP          = 3,
	_4_B_COLUMN_DETAIL        = 4
};
enum 
{
	B_COLUMN_TOTAL_COUNT     = 5
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

const CString KMultimodalResultDlg::DEFAULT_MINSIZE = _T("1.0");
const CString KMultimodalResultDlg::DEFAULT_MAXSIZE = _T("15.0");

// KMultimodalResultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultimodalResultDlg, KResizeDialogEx)

KMultimodalResultDlg::KMultimodalResultDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMultimodalResultDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_dMaxTrip(0.0)
	, m_dMinTrip(0.0)
	, m_dRadius(1000.0)
{
	m_clrBus     = RGB (   0, 128,   0 );
	m_clrRail    = RGB (   0,   0, 255 );
	m_clrWalk    = RGB ( 102, 102, 153 );
	m_clrOutRange= RGB ( 221, 217, 195 );

	m_oStartNodeIdCaption.TID = _T("-1");

	// 2019.03.12 명지대 요청사항 수정
	m_spPathCompareDlg = nullptr;
	// 2019.03.12 명지대 요청사항 수정 끝
}

KMultimodalResultDlg::~KMultimodalResultDlg()
{
	CloseWindowProc();
}

void KMultimodalResultDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                 m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B,                 m_wndReportBCtrl);
	
	DDX_Control(pDX, IDC_CLR_BUS,                  m_cpBusColor);
	DDX_Control(pDX, IDC_CLR_RAIL,                 m_cpRailColor);
	DDX_Control(pDX, IDC_CLR_WALK,                 m_cpWalkColor);
	DDX_Control(pDX, IDC_CLR_OUT,                  m_cpOutRangeColor);
}


BEGIN_MESSAGE_MAP(KMultimodalResultDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,  &KMultimodalResultDlg::OnReportAItemClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,  &KMultimodalResultDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_A, &KMultimodalResultDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_B, &KMultimodalResultDlg::OnItemButtonClick)
	ON_BN_CLICKED(IDC_CHECK_ALL,            &KMultimodalResultDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON1,                   &KMultimodalResultDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_CHECK_B,                   &KMultimodalResultDlg::OnBnClickedCheckB)
	ON_BN_CLICKED(IDC_CHECK_R,                   &KMultimodalResultDlg::OnBnClickedCheckR)
	ON_BN_CLICKED(IDC_CHECK_BR,                  &KMultimodalResultDlg::OnBnClickedCheckBr)
	ON_BN_CLICKED(IDC_BUTTON5,                   &KMultimodalResultDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL,                      &KMultimodalResultDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_OUTRANGE, &KMultimodalResultDlg::OnBnClickedCheckOutrange)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KMultimodalResultDlg::OnBnClickedCheckLabel)

	ON_BN_CLICKED(IDC_BUTTON4, &KMultimodalResultDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON6, &KMultimodalResultDlg::OnBnClickedButtonEndNodeSearch)
	ON_BN_CLICKED(IDC_BTN_COMPARE_PATH, &KMultimodalResultDlg::OnBnClickedBtnComparePath)
END_MESSAGE_MAP()


// KMultimodalResultDlg 메시지 처리기입니다.
void KMultimodalResultDlg::InitializeNode()
{
	CWaitCursor wc;

	m_mapBusAssign.clear();
	m_mapRailAssign.clear();
	m_mapBusRailAssign.clear();

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


bool KMultimodalResultDlg::GetNodeInfo(void)
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


void KMultimodalResultDlg::InsertNodeComboCtrl( CComboBox* a_pcboNode, const Integer& a_nxNodeId, const CString& a_strNodeData)
{
	int nCur = a_pcboNode->InsertString(-1, a_strNodeData);
	a_pcboNode->SetItemData(nCur, (DWORD_PTR)a_nxNodeId );
}


void KMultimodalResultDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
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


void KMultimodalResultDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KMultimodalResultDlg::UpdateSearchDataCount()
{
	try
	{
		CString strCount(_T(""));

		int nAllDestinationCnt    = (int)m_mapEndZoneNodeData.size();
		int nSearchDestinationCnt = (int)m_setSearchedID.size();

		strCount.Format(_T("Records:  %d / %d"), nSearchDestinationCnt, nAllDestinationCnt);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCount.Format(_T("경로 탐색 결과(선택/전체) : %d / %d"), nSearchDestinationCnt, nAllDestinationCnt);
		}
		SetDlgItemText(IDC_STATIC6, strCount);	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KMultimodalResultDlg::OnInitDialog()
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

	CWaitCursor wc;

	KColorPickerSetting::DefaultCtrl(m_cpBusColor,      m_clrBus);
	KColorPickerSetting::DefaultCtrl(m_cpRailColor,     m_clrRail);
	KColorPickerSetting::DefaultCtrl(m_cpWalkColor,     m_clrWalk);
	KColorPickerSetting::DefaultCtrl(m_cpOutRangeColor, m_clrOutRange);
	
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

	CheckDlgButton(IDC_CHECK_B,  BST_CHECKED);
	CheckDlgButton(IDC_CHECK_R,  BST_CHECKED);
	CheckDlgButton(IDC_CHECK_BR, BST_CHECKED);
	SetDlgItemText(IDC_EDIT_MINSIZE, DEFAULT_MINSIZE);
	SetDlgItemText(IDC_EDIT_MAXSIZE, DEFAULT_MAXSIZE);
	UpdateOutRange();
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateSearchDataCount();

	KReportCtrlSetting::DefaultC(m_wndReportACtrl);
	KReportCtrlSetting::DefaultC(m_wndReportBCtrl);

	InitReportAHeader();
	InitReportBHeader();
	
	ResizeComponent();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMultimodalResultDlg::InitReportAHeader()
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


void KMultimodalResultDlg::InitReportBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_ZONEID,       _T("Destination-Transit Path"),    55));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("경로 구분"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_TOTAL_TIME,   _T("Time"),       15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시간"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_LENGTH,       _T("Length"),     15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("거리"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_TRIP,         _T("Trip"),       15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행량(인)"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_DETAIL,       _T("Info"), COLUMN_WIDTH_INFO, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("상세정보"));
    }

	m_wndReportBCtrl.Populate();
}


void KMultimodalResultDlg::ResizeComponent()
{
	SetResize(IDC_EDIT1,          			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON4,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR5,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_EDIT2,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC6,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR8,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_B,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR4,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC18,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC19,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC20,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC21,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINTRIP,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXTRIP,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXSIZE,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_OUTRANGE,       		  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_OUT,       	      	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	  
	SetResize(IDC_STATIC14,          		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC15,         		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC16,       	    	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_BUS,       		          SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_RAIL,       		          SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_WALK,          		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR7,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON5,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_COMPARE_PATH,				  SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,       		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


unsigned __stdcall KMultimodalResultDlg::CollectDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("대중교통 이용경로 통행량 데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Multimodal Assign Result Data..."));
	}

	KMultimodalResultDlg*     pDlg = (KMultimodalResultDlg*)pParameter->GetParameter();
	pDlg->CollectData();

	return 0;
}


void KMultimodalResultDlg::CollectData()
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


void KMultimodalResultDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KMultimodalResultDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	Integer nxNodeID = GetSelStartNodeId();
	if(-1 == nxNodeID)
	{
		return false;
	}

	CString strDataFileName(_T(""));
	CString strSavePath(_T(""));

	try
	{
		strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusPathAssign.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);
		ReadBusPathFile(strSavePath);

		strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_RailPathAssign.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);
		ReadRailPathFile(strSavePath);

		strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusRailPathAssign.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);
		ReadBusRailPathFile(strSavePath);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
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


Integer KMultimodalResultDlg::GetSelStartNodeId()
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


void KMultimodalResultDlg::ReadBusPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusAssign.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oBusAssignInfo;
				oBusAssignInfo.emTransitAssignMode = KEMTransitAssignBus;
			#pragma region 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
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
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TBusSubData) );
			#pragma endregion 정보 취합

			#pragma region 취합 정보 등록
				AutoType iter  = m_mapBusAssign.find(oBusAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapBusAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusAssignInfo);
					m_mapBusAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
			#pragma endregion 취합 정보 등록

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
		m_mapBusAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadBusPathFile 오류");
		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMultimodalResultDlg::ReadRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapRailAssign.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oRailAssignInfo;
				oRailAssignInfo.emTransitAssignMode = KEMTransitAssignRail;
			#pragma region 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
				if(ifs.eof())break;

				int nBoardingCnt(0); //탑승횟수
				ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

				if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				std::vector< TIndexResultTransitInfo > &vecResultBusInfo = oRailAssignInfo.vecResultBusInfo;
				for (int i= 0; i< nBoardingCnt; i++)
				{
					int nDuplicateCnt(0); //중복 노선 갯수
					ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

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
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TRailSubData) );
			#pragma endregion 정보 취합

			#pragma region 취합 정보 등록
				AutoType iter  = m_mapRailAssign.find(oRailAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oRailAssignInfo);
					m_mapRailAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
			#pragma endregion 취합 정보 등록

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
		m_mapRailAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadRailPathFile 오류");
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMultimodalResultDlg::ReadBusRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusRailAssign.clear();

	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oBusRailAssignInfo;
				oBusRailAssignInfo.emTransitAssignMode = KEMTransitAssignBusRail;
				int nRailIndex = -1;

#pragma region 버스 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
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
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

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

				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTBusSubData)),           sizeof(TBusSubData) );
#pragma endregion 버스 정보 취합

				std::vector< std::vector< TIndexResultTransitInfo > > vecMultiResultRailInfo;
				if (nRailIndex != -1)
				{
#pragma region 레일 정보 취합(multi rail)
					int nRailCount(0);
					ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

					if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					for (int m= 0; m< nRailCount; m++)
					{
						Integer nxNoneUseOriginID;
						Integer nxNoneUseDestinationID;
						int     nNoneUsePathID;

						ifs.read( reinterpret_cast<char*>( &(nxNoneUseOriginID)),                      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nxNoneUseDestinationID)),                 sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

						int nBoardingCnt(0); //탑승횟수
						ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
						if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						std::vector< TIndexResultTransitInfo > vecResultRailInfo;
						for (int i= 0; i< nBoardingCnt; i++)
						{
							int nDuplicateCnt(0); //중복 노선 갯수
							ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
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
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

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

						ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTRailSubData)),             sizeof(TRailSubData) ); // 맨 마지막의 RailSubData 저장

						vecMultiResultRailInfo.push_back(vecResultRailInfo);
					}
#pragma endregion 레일 정보 취합
				}

				std::vector< TIndexResultTransitInfo > &vecResultTotalBusInfo = oBusRailAssignInfo.vecResultBusInfo;
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
				AutoType iter  = m_mapBusRailAssign.find(oBusRailAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapBusRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
					m_mapBusRailAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
#pragma endregion 취합 정보 등록

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
		m_mapBusRailAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadBusRailPathFile 오류");
		/*ThrowException(_T("ReadBusRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KMultimodalResultDlg::SetMaxTrip(double &a_dAssignTrip)
{
	if( a_dAssignTrip > 0 )
	{
		m_dMaxTrip += a_dAssignTrip;
	}
}


void KMultimodalResultDlg::SetMinTrip(double &a_dAssignTrip)
{
	if( a_dAssignTrip > 0 )
	{
		if(0.0 == m_dMinTrip)
		{
			m_dMinTrip = a_dAssignTrip;
		}

		if(m_dMinTrip > a_dAssignTrip)
		{
			m_dMinTrip = a_dAssignTrip;
		}
	}
}


void KMultimodalResultDlg::GetDestinationNodeInfo()
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
		KIDCaption oIDCaption;
		CString    strZoneID(_T(""));

		AutoType iter  = m_mapZoneNodeData.begin();
		AutoType itEnd = m_mapZoneNodeData.end();

		while(iter != itEnd)
		{
			Integer nxDzoneID    = iter->first;
			CString &strZoneName = iter->second;

			std::vector<KMBusAssignInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMBusAssignInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMBusAssignInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData    (nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData   (nxDzoneID);
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


void KMultimodalResultDlg::UpdateReportAData()
{
	m_wndReportACtrl.ResetContent();

	BOOL bAllSelected(FALSE);
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

			std::vector<KMBusAssignInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMBusAssignInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMBusAssignInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData(nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData(nxDzoneID);
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


std::vector<KMBusAssignInfo>* KMultimodalResultDlg::IsHaveBusAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


std::vector<KMBusAssignInfo>* KMultimodalResultDlg::IsHaveRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


std::vector<KMBusAssignInfo>* KMultimodalResultDlg::IsHaveBusRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


void KMultimodalResultDlg::UpdateReportBData()
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

//	AllClearPath();

	CWaitCursor wc;
	BOOL bUsedBus     = IsDlgButtonChecked(IDC_CHECK_B);
	BOOL bUsedRail    = IsDlgButtonChecked(IDC_CHECK_R);
	BOOL bUsedBusRail = IsDlgButtonChecked(IDC_CHECK_BR);
	std::vector<BOOL> vecUsedTraint;
	vecUsedTraint.push_back(bUsedBus);
	vecUsedTraint.push_back(bUsedRail);
	vecUsedTraint.push_back(bUsedBusRail);

	m_dMinTrip = 0.0;
	m_dMaxTrip = 0.0;

	try
	{
		CXTPReportRecord*      pParentRecord    = nullptr;
		CXTPReportRecords*     pSeParentRecords = nullptr;
		CXTPReportRecord*      pSeParentRecord  = nullptr;
		CXTPReportRecords*     pThParentRecords = nullptr;
		CXTPReportRecord*      pThParentRecord  = nullptr;
		CXTPReportRecordItem*  pItem            = nullptr;

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
			std::vector<KMBusAssignInfo>*    pvecBusAssignInfo     = (std::vector<KMBusAssignInfo>*)pItemA->GetItemData();
			pItemA            = pARecord->GetItem(_3_A_COLUMN_ISRAIL);
			std::vector<KMBusAssignInfo>*    pvecRailAssignInfo    = (std::vector<KMBusAssignInfo>*)pItemA->GetItemData();
			pItemA            = pARecord->GetItem(_4_A_COLUMN_ISBUSRAIL);
			std::vector<KMBusAssignInfo>*    pvecBusRailAssignInfo = (std::vector<KMBusAssignInfo>*)pItemA->GetItemData();

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
				pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
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

				int nMode;
				CString strType(_T(""));
				std::vector<KMBusAssignInfo>* pvecAssignInfo = nullptr;
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
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemText(strType));
				for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
				{
					pSeParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				}
				pItem->SetAlignment(DT_LEFT);
				pItem->HasCheckbox(TRUE);
				pItem->SetChecked(TRUE);
				pItem->SetItemData((DWORD_PTR)nMode);

// 				if(nullptr == pvecAssignInfo)
// 					continue;

				pThParentRecords = pSeParentRecord->GetChilds();

				for (size_t m = 0; m < (*pvecAssignInfo).size(); m++)
				{
					KMBusAssignInfo &oBusAssignInfo = (*pvecAssignInfo)[m];

					pThParentRecord = pThParentRecords->Add(new CXTPReportRecord());
					//#3
					CString strIndex(_T(""));
					CString strDisplay(_T(""));
					strIndex.Format(_T("%d"), m+1);
					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemText());
					pItem->SetAlignment(DT_LEFT);
					pItem->HasCheckbox(TRUE);
					pItem->SetChecked(TRUE);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strDisplay.Format(_T("경로%d"), m+1);
					}
					else {
						strDisplay.Format(_T("#P%d"), m+1);
					}
					pItem->SetCaption(strDisplay);
					pItem->SetItemData((DWORD_PTR)&oBusAssignInfo);

					//#4
					double dTrip(0.0);
					double dTotalTime(0.0);
					double dLength(0.0);
					if(0 == k)
					{
						dTrip            = oBusAssignInfo.oTBusSubData.dAssignTrip;
						dLength          = oBusAssignInfo.oTBusSubData.dLength;
						SumTotalTime(true, oBusAssignInfo, dTotalTime);
					}
					else if(1 == k)
					{
						dTrip            = oBusAssignInfo.oTRailSubData.dAssignTrip;
						dLength          = oBusAssignInfo.oTRailSubData.dLength;
						SumTotalTime(false, oBusAssignInfo, dTotalTime);
					}
					else
					{
						dTrip            = oBusAssignInfo.oTBusSubData.dAssignTrip;
						dLength          = oBusAssignInfo.oTBusSubData.dLength;
						SumTotalTime(true, oBusAssignInfo, dTotalTime);
					}

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);

					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dTrip, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
					SetMaxTrip(dTrip);
					SetMinTrip(dTrip);

					//#5
					pItem = pThParentRecord->AddItem(new CXTPReportRecordItem());
					CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
					pButton->SetAlignment(xtpItemControlUnknown);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						pButton->SetCaption(_T("보기"));
						pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
					}
					else {
						pButton->SetCaption(_T("Info"));
						pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
					}
				}
			}
		}

		m_wndReportBCtrl.Populate();
		if(nSelectCount < EXPANDALL_LIMIT_COUNT) // all 체크한 데이터를 모두 expandall 하기엔 부하가 걸린다.
		{
			m_wndReportBCtrl.ExpandAll(TRUE);
		}
		
		UpdateMinMaxTrip();
		if (m_setSearchedID.size() > 0) {
			DoneDraw(true);
		}
		else {
			DoneDraw(false);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		m_dMinTrip = 0.0;
		m_dMaxTrip = 0.0;
		UpdateMinMaxTrip();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		m_dMinTrip = 0.0;
		m_dMaxTrip = 0.0;
		UpdateMinMaxTrip();
		return;
	} 
}


void KMultimodalResultDlg::SumTotalTime(bool a_bBus, KMBusAssignInfo &a_oBusAssignInfo, double &a_dTotalTime)
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


void KMultimodalResultDlg::UpdateMinMaxTrip()
{
	CString strMinTrip(_T(""));
	CString strMaxTrip(_T(""));

	double dMinTrip = m_dMinTrip - 0.5;
	if(dMinTrip < 0.0)
		dMinTrip = 0.0;

	double dMaxTrip = m_dMaxTrip + 0.6;

	strMinTrip.Format(_T("%.0f"), dMinTrip);
	strMaxTrip.Format(_T("%.0f"), dMaxTrip);

	/*SetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);*/
	SetDlgItemText(IDC_EDIT_MINTRIP, _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);
}


void KMultimodalResultDlg::OnBnClickedButtonDraw()
{
	TxLogDebugStartMsg();
	DoneDraw(true);
	TxLogDebugEndMsg();
}

void KMultimodalResultDlg::DoneDraw(bool a_bShowErrMsg) 
{
	TxLogDebugStartMsg();
	try
	{
		InvalidateInputInfo(a_bShowErrMsg);

		AllClearPath(false);

		CWaitCursor cw;
		SelectDrawData();
		//QBicSimpleProgressThread::ExecuteThread(SelectDrawDataThreadCaller, this);

		bool bLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false, false);

		m_pMapView->TxDrawCompositeLink    (m_oMapResult.vecDrawLinkInfo, bLabel, false);
		//존->정류장 도보제거 요청 : 2013.03.03-명지대학교
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, bLabel, false);

		DrawDestZoneAccess(false); // org와 Dest들의 존엑세스를 그린다.

		m_pMapView->MapRefresh();
	}
	catch(...)
	{
		return;
	}
	TxLogDebugEndMsg();
}

void KMultimodalResultDlg::OnBnClickedCheckLabel()
{
    try
    {
        if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
        {
            m_pMapView->TxDrawShowCompositeLinkLablel(true, false);
            m_pMapView->TxDrawShowCompositeUserLineLablel(true, false);
        }
        else
        {
            m_pMapView->TxDrawShowCompositeLinkLablel(false, false);
            m_pMapView->TxDrawShowCompositeUserLineLablel(false, false);
        }

        m_pMapView->MapRefresh();
    }
    catch(...)
    {
        TxLogDebugException();
    }
}


unsigned __stdcall KMultimodalResultDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KMultimodalResultDlg*     pDlg = (KMultimodalResultDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


void KMultimodalResultDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::map<Integer, std::map<int, std::vector<KMBusAssignInfo>*>> mapDestMAssignInfo;

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

			std::vector<KMBusAssignInfo>* pvecBusAssignInfo     = nullptr;
			std::vector<KMBusAssignInfo>* pvecRailAssignInfo    = nullptr;
			std::vector<KMBusAssignInfo>* pvecBusRailAssignInfo = nullptr;
			pvecBusAssignInfo     = IsHaveBusAssignData    (nxDzoneID);
			pvecRailAssignInfo    = IsHaveRailAssignData   (nxDzoneID);
			pvecBusRailAssignInfo = IsHaveBusRailAssignData(nxDzoneID);

			if (nullptr == pvecBusAssignInfo && nullptr == pvecRailAssignInfo && nullptr == pvecBusRailAssignInfo) {
				continue;
			}

			std::map<int, std::vector<KMBusAssignInfo>*> &mapModeMAssignInfo = mapDestMAssignInfo[nxDzoneID];

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
		//KMultiModalResultInfo::ResultBusPathTripInfo(m_pTarget, vecMBusAssignInfo, oMMapResultInput);
		KMultiModalResultInfo::ResultBusPathTripInfo2(m_pTarget, mapDestMAssignInfo, OUT m_mapDestResultInputValue);
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

unsigned __stdcall KMultimodalResultDlg::SelectDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KMultimodalResultDlg*     pDlg = (KMultimodalResultDlg*)pParameter->GetParameter();
	pDlg->SelectDrawData();

	return 0;
}

void KMultimodalResultDlg::SelectDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		CString strMinSize(_T(""));
		CString strMaxSize(_T(""));
		GetDlgItemText(IDC_EDIT_MINSIZE, strMinSize);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strMaxSize);
		double dMinSize = _ttof(strMinSize);
		double dMaxSize = _ttof(strMaxSize);

		CString strMinTrip(_T(""));
		CString strMaxTrip(_T(""));
		GetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);
		GetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);
		double dMinTrip = _ttof(strMinTrip);
		double dMaxTrip = _ttof(strMaxTrip);

		TMResultSettingInfo oTMResltSettingInfo;
		oTMResltSettingInfo.clrBus      = m_cpBusColor.GetColor();
		oTMResltSettingInfo.clrRail     = m_cpRailColor.GetColor();
		oTMResltSettingInfo.clrWalk     = m_cpWalkColor.GetColor();
		oTMResltSettingInfo.clrOutRange = m_cpOutRangeColor.GetColor();
		oTMResltSettingInfo.dMinTrip    = dMinTrip;
		oTMResltSettingInfo.dMaxTrip    = dMaxTrip;
		oTMResltSettingInfo.dMinSize    = dMinSize;
		oTMResltSettingInfo.dMaxSize    = dMaxSize;
		if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
			oTMResltSettingInfo.bUseOutRange = TRUE;
		else
			oTMResltSettingInfo.bUseOutRange = FALSE;

		std::map<Integer, std::map<int, std::set<int>>> mapZoneModePIndex;

		try
		{
			CXTPReportRecordItem*  pItem    = nullptr;
			CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
			int nCount = pRecords->GetCount();
			for (int i= 0; i< nCount; i++)
			{
				CXTPReportRecord*  pParentRecord    = pRecords->GetAt(i);
				pItem             = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
				Integer nxDZoneID = (Integer)pItem->GetItemData();

				CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
				if (nullptr == pSeParentRecords)
				{
					continue;
				}

				for (int k= 0; k< pSeParentRecords->GetCount(); k++)
				{
					CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
					pItem            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

					if( pItem->GetHasCheckbox() == FALSE )
						continue;
					if( pItem->IsChecked() == FALSE )
						continue;

					int nMode = (int)pItem->GetItemData();

					CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
					if (nullptr == pThParentRecords)
					{
						continue;
					}

					for (int m= 0; m< pThParentRecords->GetCount(); m++)
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
		}
		catch (...)
		{
			TxLogDebugException();	
		}

		KMultiModalResultInfo::ResultBusPathDrawInfo3(m_pTarget, m_mapDestResultInputValue, mapZoneModePIndex, oTMResltSettingInfo, OUT m_oMapResult);
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



void KMultimodalResultDlg::InvalidateInputInfo(bool a_bShowErrMsg)
{
	try
	{
		CString strMinTrip(_T(""));
		CString strMaxTrip(_T(""));
		GetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);
		GetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);

		if (QBicStringChecker::IsNumeric(strMinTrip) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (QBicStringChecker::IsNumeric(strMaxTrip) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}

		double dMinTrip = _ttof(strMinTrip);
		if (dMinTrip < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		double dMaxTrip = _ttof(strMaxTrip);
		if (dMaxTrip < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (dMinTrip > dMaxTrip)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("최소값보다 큰 값을 입력해 주세요."));
			throw oControlMessage;
		}

		CString strMinSize(_T(""));
		CString strMaxSize(_T(""));
		GetDlgItemText(IDC_EDIT_MINSIZE, strMinSize);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strMaxSize);

		if (QBicStringChecker::IsNumeric(strMinSize) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (QBicStringChecker::IsNumeric(strMaxSize) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}

		double dMinSize = _ttof(strMinSize);
		if (dMinSize < 0.0)
		{ 
			KControlMessage oControlMessage(IDC_EDIT_MINSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		double dMaxSize = _ttof(strMaxSize);
		if (dMaxSize < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (dMinSize >= dMaxSize)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("최소값보다 큰 값을 입력해 주세요."));
			throw oControlMessage;
		}
	}
	catch(KControlMessage oControlMessage)
	{
		if (a_bShowErrMsg) {
			AfxMessageBox(oControlMessage.strMsg);
		}
		GetDlgItem(oControlMessage.nID)->SetFocus();
		throw 1;
	}
	catch(...)
	{
		throw 1;
	}
}


void KMultimodalResultDlg::OnBnClickedButtonClear()
{
	/************************************************************************/
	/* Clear버튼 클릭 시 모든 체크 옵션 선택이 취소되고 선택한 Origin/Destination에 Access Boundary만 표출되어야 함.                                                                     */
	/************************************************************************/
	// all check
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_B, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_R, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_BR, BST_UNCHECKED);

	OnBnClickedCheckAll();
}


void KMultimodalResultDlg::AllClearPath(bool a_bRefresh)
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


void KMultimodalResultDlg::OnBnClickedCheckAll()
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


void KMultimodalResultDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KMultimodalResultDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{


}


void KMultimodalResultDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_B_COLUMN_ZONEID == nIndex)
	{
		try
		{
			CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItem->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == TRUE)
			{
				BOOL bParentChecked = pItem->IsChecked();

				CXTPReportRecords* pChdRecords = pRecord->GetChilds();
				int nCount = pChdRecords->GetCount();
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_B_COLUMN_ZONEID);
					pChdItem->SetChecked(bParentChecked);
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
			
		#pragma region Update Min Max Trip
// 			m_dMinTrip = 0.0;
// 			m_dMaxTrip = 0.0;
// 			double dTrip(0.0);
// 			CXTPReportRecordItem*  pItemTrip    = nullptr;
// 			CXTPReportRecords*     pTripRecords = m_wndReportBCtrl.GetRecords();
// 			for (int i= 0; i< pTripRecords->GetCount(); i++)
// 			{
// 				CXTPReportRecord*  pParentRecord    = pTripRecords->GetAt(i);
// 				CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
// 				for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
// 				{
// 					CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
// 					pItemTrip            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);
// 
// 					if( pItemTrip->GetHasCheckbox() == FALSE )
// 						continue;
// 					if( pItemTrip->IsChecked() == FALSE )
// 						continue;
// 
// 					CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
// 					for (int m= 0; m<  pThParentRecords->GetCount(); m++)
// 					{
// 						CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
// 						pItemTrip            = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);
// 
// 						if( pItemTrip->IsChecked() == FALSE )
// 							continue;
// 						KMBusAssignInfo* pAssignInfo = (KMBusAssignInfo*)pItemTrip->GetItemData();
// 
// 						KEMTransitAssignMode &emTransitAssignMode = pAssignInfo->emTransitAssignMode;
// 						
// 						if(KEMTransitAssignBus == emTransitAssignMode)
// 						{
// 							dTrip = pAssignInfo->oTBusSubData.dAssignTrip;
// 						}
// 						else if(KEMTransitAssignRail == emTransitAssignMode)
// 						{
// 							dTrip = pAssignInfo->oTRailSubData.dAssignTrip;
// 						}
// 						else
// 						{
// 							dTrip = pAssignInfo->oTBusSubData.dAssignTrip;
// 						}
// 						SetMaxTrip(dTrip);
// 						SetMinTrip(dTrip);
// 					}
// 				}
// 			}
// 			UpdateMinMaxTrip();
		#pragma endregion Update Min Max Trip
			
			if (m_setSearchedID.size() > 0) {
				DoneDraw(true);
			}
			else {
				DoneDraw(false);
			}
		}
		catch (...)
		{
			return;
		}
		UpdatePathCompareButtonState();
	}
}


void KMultimodalResultDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
		m_pMapView->ZoomToFeature(KLayerID::Node(), a_nxNodeID);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMultimodalResultDlg::PanToNodeFeature( Integer &a_nxNodeID )
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


void KMultimodalResultDlg::OnBnClickedCheckB()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMultimodalResultDlg::OnBnClickedCheckR()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMultimodalResultDlg::OnBnClickedCheckBr()
{
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KMultimodalResultDlg::OnBnClickedCheckOutrange()
{
	UpdateOutRange();
}


void KMultimodalResultDlg::UpdateOutRange()
{
	if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
	{
		GetDlgItem(IDC_CLR_OUT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CLR_OUT)->ShowWindow(SW_HIDE);
	}
}


void KMultimodalResultDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if(_4_B_COLUMN_DETAIL == pItem->GetIndex())
	{
		CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_B_COLUMN_ZONEID);
		KMBusAssignInfo*      pBusAssignInfo = (KMBusAssignInfo*)pItemB->GetItemData();
		CString               strPathIdex    = pItemB->GetCaption(NULL);
		
		CRect rect;
		GetWindowRect(rect);

		if(nullptr != m_spMultiModalResultDetailDlg)
		{
			m_spMultiModalResultDetailDlg->InitializeData(strPathIdex, pBusAssignInfo, rect);
			m_spMultiModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spMultiModalResultDetailDlg = KMultiModalResultDetailDlgPtr(new KMultiModalResultDetailDlg(m_pTarget, true, m_mapNodeName, this));
			m_spMultiModalResultDetailDlg->Create(KMultiModalResultDetailDlg::IDD, this);
			m_spMultiModalResultDetailDlg->InitializeData(strPathIdex, pBusAssignInfo, rect);
			m_spMultiModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
	}
}


void KMultimodalResultDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KMultimodalResultDlg::CloseWindowProc()
{
	AllClearPath(true);

	if (nullptr != m_spMultiModalResultDetailDlg)
		m_spMultiModalResultDetailDlg.reset();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	// 2019.03.12 명지대 요청사항 수정
	if (nullptr != m_spPathCompareDlg)
	{
		m_spPathCompareDlg.reset();
	}
	// 2019.03.12 명지대 요청사항 수정 끝
}


void KMultimodalResultDlg::ActivatedFrameWndNotifyProcess()
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


void KMultimodalResultDlg::DeActivatedFrameWndNotifyProcess()
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


BOOL KMultimodalResultDlg::PreTranslateMessage(MSG* pMsg)
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


void KMultimodalResultDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KMultimodalResultDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KMultimodalResultDlg::SelectStartNodeList()
{
	try
	{
		CWaitCursor cw;

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


void KMultimodalResultDlg::OnBnClickedButtonEndNodeSearch()
{
	SelectEndNodeList();
}


void KMultimodalResultDlg::SelectEndNodeList()
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


void KMultimodalResultDlg::ModelessOKFeedback( UINT nDialogID)
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

				if (m_mapEndZoneNodeData.size() < 1)
				{
					AfxMessageBox(_T("검색한 종점 관련 정보가 존재하지 않습니다."));
				}
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

void KMultimodalResultDlg::DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh)
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


void KMultimodalResultDlg::DrawDestZoneAccess(bool a_bRefresh)
{
	CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
	int nCount = pRecords->GetCount();

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

void KMultimodalResultDlg::ModelessCancleFeedback(UINT nDialogID)
{

}


// 2019.03.11 명지대 요청사항 수정
void KMultimodalResultDlg::OnBnClickedBtnComparePath()
{
	std::vector<KMBusAssignInfoWrapper> arr;
	try
	{
		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*  pParentRecord = pRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
			{
				continue;
			}

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
				if (nullptr == pThParentRecords)
				{
					continue;
				}

				for (int m = 0; m< pThParentRecords->GetCount(); m++)
				{
					CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
					pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

					if (pItem->IsChecked() == FALSE)
						continue;

					KMBusAssignInfo* pInfo = (KMBusAssignInfo*)pItem->GetItemData();
					if (NULL != pInfo)
					{
						KMBusAssignInfoWrapper wr;
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

	m_spPathCompareDlg = std::make_shared<KSelectedPathCompareUrbanDlg>(false, EMPathCompareMode_Multimodal, this);
	m_spPathCompareDlg->SetMultimodalResultInfos(arr);
	m_spPathCompareDlg->Create(IDD_6929_KSelectedPathCompareUrban, this);
	m_spPathCompareDlg->ShowWindow(SW_SHOW);
}
// 2019.03.11 명지대 요청사항 수정 끝




void KMultimodalResultDlg::UpdatePathCompareButtonState() 
{
	BOOL bEnableButton = FALSE;
	CXTPReportRecordItem*  pItem = nullptr;
	CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
	int nCount = pRecords->GetCount();
	for (int i = 0; i< nCount; i++)
	{
		CXTPReportRecord*  pParentRecord = pRecords->GetAt(i);
		pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);

		CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
		if (nullptr == pSeParentRecords)
		{
			continue;
		}

		for (int k = 0; k< pSeParentRecords->GetCount(); k++)
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

			//CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
			//if (nullptr == pThParentRecords)
			//{
			//	continue;
			//}

			//for (int m = 0; m< pThParentRecords->GetCount(); m++)
			//{
			//	CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
			//	pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			//	if (pItem->IsChecked() == FALSE)
			//	{
			//		continue;
			//	}
			//	else
			//	{
			//		bEnableButton = TRUE;
			//	}
			//}
		}
	}

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(bEnableButton);
}