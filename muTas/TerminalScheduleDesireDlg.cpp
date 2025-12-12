// TerminalScheduleDesireDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TerminalScheduleDesireDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//^#include "DBaseConnector.h"
//#include "StringChecker.h"
#include "ImChampMapDefine.h"

namespace TerminalScheduleENUM
{
	enum
	{
		_0_COLUMN_CHECK         = 0,
		_1_COLUMN_TERMINAL_ID   = 1,
		_2_COLUMN_NAME          = 2,
		_3_COLUMN_VALUE         = 3
	};
}

using namespace TerminalScheduleENUM;
// KTerminalScheduleDesireDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTerminalScheduleDesireDlg, KResizeDialogEx)

KTerminalScheduleDesireDlg::KTerminalScheduleDesireDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTerminalScheduleDesireDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_strTripFieldName(_T(""))
{
	m_oStartNodeIdCaption.TID = _T("-1");
	m_clrLine   = RGB( 51, 153, 102);
}

KTerminalScheduleDesireDlg::~KTerminalScheduleDesireDlg()
{
	ClearMapDraw(true);
	if (nullptr != m_spImTasIDFind)
	{
		m_spImTasIDFind.reset();
	}
}

void KTerminalScheduleDesireDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_DEST,            m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_MODE,             m_cboMode);
	DDX_Control(pDX, IDC_CLR_LINE,               m_cpLineColor);
}


BEGIN_MESSAGE_MAP(KTerminalScheduleDesireDlg, KResizeDialogEx)
	ON_BN_CLICKED   (IDC_BUTTON4,                                &KTerminalScheduleDesireDlg::OnBnClickedButtonStartNodeSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE,                             &KTerminalScheduleDesireDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED   (IDC_CHECK_DESTINATION,                      &KTerminalScheduleDesireDlg::OnBnClickedCheckDestinationAll)
	ON_NOTIFY       (XTP_NM_REPORT_CHECKED,     IDC_REPORT_DEST, &KTerminalScheduleDesireDlg::OnReportCheckItem)
	ON_NOTIFY       (NM_CLICK,                  IDC_REPORT_DEST, &KTerminalScheduleDesireDlg::OnReportItemClick)

	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KTerminalScheduleDesireDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KTerminalScheduleDesireDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KTerminalScheduleDesireDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KTerminalScheduleDesireDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTerminalScheduleDesireDlg 메시지 처리기입니다.

void KTerminalScheduleDesireDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KTerminalScheduleDesireDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}
}


void KTerminalScheduleDesireDlg::Initialize()
{
	std::map<int, CString> mapRegTransitType;
	LoadRegTransitType(mapRegTransitType);
	InitComboMode     (mapRegTransitType);
	GetPassengerNumField();

	m_oStartNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT1, _T(""));	

	SetDlgItemText(IDC_EDIT_MIN_VALUE, _T(""));	
	SetDlgItemText(IDC_EDIT_MAX_VALUE, _T(""));	
	
	m_wndReportCtrl.ResetContent();
	m_wndReportCtrl.Populate();
}


void KTerminalScheduleDesireDlg::LoadRegTransitType(std::map<int, CString> &a_mapRegTransitType)
{
	a_mapRegTransitType.clear();
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Append      (_T(" SELECT TBL1.mode, TBL2.KCode_Display "));
		strSQL.Append      (_T(" FROM "));
		strSQL.AppendFormat(_T(" (SELECT mode FROM %s GROUP BY mode)TBL1 "), TABLE_TERMINAL_SCHEDULE);
		strSQL.Append      (_T(" LEFT OUTER JOIN "));
		strSQL.AppendFormat(_T(" (SELECT KCode_Key, KCode_Display FROM %s where KCode_Group_Key = %d)TBL2 "), TABLE_KCODE_DEFINE, TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
		strSQL.Append      (_T(" ON TBL1.mode = TBL2.KCode_Key ORDER BY TBL1.mode "));

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			int     nMode  	  = pResult->GetValueInt   (0);
			CString strName   = pResult->GetValueString(1);

			a_mapRegTransitType.insert(std::make_pair(nMode, strName));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleDesireDlg::InitComboMode( std::map<int, CString> &a_mapRegTransitType )
{
	m_cboMode.ResetContent();

	try
	{
		AutoType iter  = a_mapRegTransitType.begin();
		AutoType itEnd = a_mapRegTransitType.end();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_cboMode.AddString(_T("전체"));
		}
		else {
			m_cboMode.AddString(_T("All"));
		}

		CString strView(_T(""));
		int     nCur(0);

		while (iter != itEnd)
		{
			int     nModeID     = iter->first;
			CString strModeName = iter->second;

			strView.Format(_T("%d: %s"), nModeID, strModeName);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strView.Format(_T("%s"), strModeName);
			}
			nCur = m_cboMode.AddString(strView);
			m_cboMode.SetItemData(nCur, nModeID);

			++iter;
		}

		if(a_mapRegTransitType.size() < 1)
		{
			m_cboMode.ResetContent();
		}
		else
		{
			m_cboMode.SetCurSel(0);	
		}

		UpdateData(FALSE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KTerminalScheduleDesireDlg::GetOriginTerminalInfo(void)
{
	TxLogDebugStartMsg();

	m_mapNodeData.clear();
	m_vecNodeIDCaption.clear();

	int nCurSel = m_cboMode.GetCurSel();
	if (nCurSel < 0)
	{
		return false;
	}

	CString strWhereMode(_T(""));
	if (nCurSel > 0) // 0은 All
	{
		int nModeID = (int)m_cboMode.GetItemData(nCurSel);
		strWhereMode.Format(_T(" WHERE mode = %d "), nModeID);
	}

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Append      (_T(" SELECT o_terminal, name "));
		strSQL.Append      (_T(" FROM "));
		strSQL.AppendFormat(_T(" (SELECT node_id, name FROM %s)TBL1, "), TABLE_NODE);
		strSQL.AppendFormat(_T(" (SELECT o_terminal FROM %s %s GROUP BY o_terminal ORDER BY o_terminal)TBL2 "), TABLE_TERMINAL_SCHEDULE, strWhereMode);
		strSQL.Append      (_T(" WHERE tbl1.node_id = tbl2.o_terminal "));

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));

		while( pResult->Next() )
		{
			nxNodeId	    = pResult->GetValueInt64 (0);
			strNodeName     = pResult->GetValueString(1);

			m_mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecNodeIDCaption.push_back(oIDCaption);
		}
	}
	catch (...)
	{
		m_mapNodeData.clear();
		m_vecNodeIDCaption.clear();

		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KTerminalScheduleDesireDlg::GetPassengerNumField(void)
{
	TxLogDebugStartMsg();
	m_strTripFieldName = _T("");
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" SELECT terminal_trip FROM %s where type = 6 ORDER BY run_seq DESC "), TABLE_ASSIGN_MODEL);
		
		KResultSetPtr pResult  = spDBaseConnection->ExecuteQuery(strSQL);

		if( pResult->Next() )
		{
			m_strTripFieldName = pResult->GetValueString(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
     return;
	}

	TxLogDebugEndMsg();
}


void KTerminalScheduleDesireDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				CString strID(_T(""));
				strID.Format(_T("%I64d"),  m_spImTasIDFind->GetSelectedID());

				CString strNodeView(_T(""));
				SetNodeViewData(strID, strNodeView, m_oStartNodeIdCaption);
				SetDlgItemText(IDC_EDIT1,  strNodeView);

				CheckDlgButton(IDC_CHECK_DESTINATION, BST_CHECKED);

				GetDesTerminalList();
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KTerminalScheduleDesireDlg::SetNodeViewData(CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		AutoType iter  = m_mapNodeData.find(nxZoneID);
		AutoType itEnd = m_mapNodeData.end();

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


void KTerminalScheduleDesireDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KTerminalScheduleDesireDlg::GetDesTerminalList()
{
	int nCurSel = m_cboMode.GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}

	Integer nxOrgTerminalID = GetSelStartNodeId();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strWhereMode(_T(""));
		if (nCurSel > 0) // 0 은 All
		{
			int nModeID = (int)m_cboMode.GetItemData(nCurSel);
			strWhereMode.AppendFormat(_T(" AND mode = %d "), nModeID);
		}

		CString strSQL(_T(""));
		strSQL.Append			(_T(" SELECT TBL2.d_terminal, TBL1.name, TBL2.sum_trip FROM "));
		strSQL.AppendFormat			(_T(" (SELECT node_id, name FROM %s)TBL1, "), TABLE_NODE);
		strSQL.AppendFormat			(_T(" (SELECT d_terminal, SUM(%s) AS sum_trip "), m_strTripFieldName);
		strSQL.AppendFormat				(_T(" FROM %s "), TABLE_TERMINAL_SCHEDULE);
		strSQL.AppendFormat				(_T(" WHERE o_terminal = %I64d "), nxOrgTerminalID);
		strSQL.AppendFormat				(_T(" %s "), strWhereMode);
		strSQL.Append					(_T(" GROUP BY d_terminal)TBL2 "));
		strSQL.Append				(_T(" WHERE TBL1.node_id = TBL2.d_terminal "));

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		std::vector<TDesTerminalInfo> vecDesTerminalInfo;
		while( pResult->Next() )
		{
			TDesTerminalInfo oTDesTerminalInfo;

			oTDesTerminalInfo.nxTerminalID	  = pResult->GetValueInt64 (0);
			oTDesTerminalInfo.strName         = pResult->GetValueString(1);
			oTDesTerminalInfo.dTerminalTrip   = pResult->GetValueDouble(2);

			vecDesTerminalInfo.push_back(oTDesTerminalInfo);
		}

		UpdateReportData(vecDesTerminalInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleDesireDlg::UpdateReportData(std::vector<TDesTerminalInfo> &a_vecDesTerminalInfo)
{
	try
	{
		m_wndReportCtrl.ResetContent();

		BOOL bCheck(FALSE);
		
		if (IsDlgButtonChecked(IDC_CHECK_DESTINATION) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString               strTerminalID(_T(""));

		size_t nxCount = a_vecDesTerminalInfo.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			TDesTerminalInfo &oTDesTerminalInfo = a_vecDesTerminalInfo[i];

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetItemData((DWORD_PTR)oTDesTerminalInfo.nxTerminalID);

			strTerminalID.Format(_T("%I64d"), oTDesTerminalInfo.nxTerminalID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTerminalID));
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTDesTerminalInfo.strName));
			pItem->SetAlignment(DT_LEFT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTDesTerminalInfo.dTerminalTrip, _T("%.1f")));
			pItem->SetAlignment(DT_CENTER);
		}

		m_wndReportCtrl.Populate();

		UpdateMinMax();

		DoneDraw(false);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTerminalScheduleDesireDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AllowColumnsort(m_wndReportCtrl);
	InitReportHeader();

	KColorPickerSetting::DefaultCtrl(m_cpLineColor,     m_clrLine);

	SetDlgItemText(IDC_EDIT_MINSIZE, _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE, _T("15.0"));

	CheckDlgButton(IDC_CHECK_DESTINATION, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL,       BST_UNCHECKED);

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTerminalScheduleDesireDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,        _T("Select"),      40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_TERMINAL_ID,  _T("Terminal ID"), 35));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 노드 ID"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME,         _T("Name"),        45));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 명칭"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_VALUE,        _T("Trip"),        25));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행량(인)"));
    }
}


void KTerminalScheduleDesireDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC8,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
		SetResize(IDC_EDIT1,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON4,	     		  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);	
		SetResize(IDC_SEPRATOR5,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);		

		SetResize(IDC_REPORT_DEST,			  SZ_TOP_LEFT,   SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR8,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_MIN_VALUE,		  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_MAX_VALUE,		  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC20,		          SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC21,		          SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC14,		          SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MIN_VALUE,		  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MAX_VALUE,		  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MINSIZE,		      SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MAXSIZE,		      SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_CLR_LINE,		          SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_SEPRATOR9,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_BUTTON_DRAW,		      SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON_CLEAR,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_LABEL,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTerminalScheduleDesireDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			ClickedEditStartNode();

			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KTerminalScheduleDesireDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KTerminalScheduleDesireDlg::SelectStartNodeList()
{
	try
	{
		GetOriginTerminalInfo();

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		m_spImTasIDFind->SetIDCaption(m_vecNodeIDCaption);
		// 		Integer nxNodeID = _ttoi64(m_oStartNodeIdCaption.TID);
		// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
		m_spImTasIDFind->SetSelectedID(-1);
		// 지도 선택 옵션
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


Integer KTerminalScheduleDesireDlg::GetSelStartNodeId()
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


void KTerminalScheduleDesireDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KTerminalScheduleDesireDlg::OnCbnSelchangeComboMode()
{
	if (nullptr != m_spImTasIDFind)
	{
		m_spImTasIDFind.reset();
	}
	
	GetDesTerminalList();
}


void KTerminalScheduleDesireDlg::OnBnClickedCheckDestinationAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_DESTINATION) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem = pRecord->GetItem(_0_COLUMN_CHECK);
			pItem->SetChecked(bCheck);
		}

		m_wndReportCtrl.RedrawControl();

		//UpdateMinMax();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleDesireDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

		if (!pItemNotify->pRow || !pItemNotify->pColumn)
			return;

		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if( _1_COLUMN_TERMINAL_ID == nIndex || _2_COLUMN_NAME == nIndex) 
		{
			CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_CHECK);
			Integer nxTerminalID = (Integer)pItem->GetItemData();

			m_pMapView->PanToFeature(KLayerID::Node(), nxTerminalID);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KTerminalScheduleDesireDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	
	//UpdateMinMax();

	DoneDraw(false);
}


void KTerminalScheduleDesireDlg::UpdateMinMax()
{
	try
	{
		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int     nCheckedTerminalCount(0);
		int     nUnCheckedTerminalCount(0);
		Integer nxTerminalID(0);
		CString strCheckedTerminalIDs(_T(""));
		CString strUnCheckedTerminalIDs(_T(""));
		CString strDestTerminalIDs(_T(""));
		
#pragma region 체크, 비체크된 TerminalID 수집
		int nTotalCount = pRecords->GetCount();
		for (int i= 0; i< nTotalCount; i++)
		{
			pRecord      = pRecords->GetAt(i);
			pItem        = pRecord->GetItem(_0_COLUMN_CHECK);
			nxTerminalID = (Integer)pItem->GetItemData();

#pragma region 체크항목에 대한 min max 버전 소스
// 			if (pItem->IsChecked() == TRUE)
// 			{
// 				if (nCheckedTerminalCount >0)
// 					strCheckedTerminalIDs.Append(_T(", "));
// 				
// 				strCheckedTerminalIDs.AppendFormat(_T("%I64d"), nxTerminalID);
// 				nCheckedTerminalCount++;
// 			}
// 			else
// 			{
// 				if (nUnCheckedTerminalCount >0)
// 					strUnCheckedTerminalIDs.Append(_T(", "));
// 
// 				strUnCheckedTerminalIDs.AppendFormat(_T("%I64d"), nxTerminalID);
// 				nUnCheckedTerminalCount++;
// 			}
#pragma endregion 체크항목에 대한 min max 버전 소스

			if (i >0)
			{
				strDestTerminalIDs.Append(_T(", "));
			}
			strDestTerminalIDs.AppendFormat(_T("%I64d"), nxTerminalID);
		}
#pragma endregion 체크, 비체크된 TerminalID 수집

#pragma region 체크항목에 대한 min max 버전 소스
// 		if (nUnCheckedTerminalCount > 0)
// 			CheckDlgButton(IDC_CHECK_DESTINATION, BST_UNCHECKED);
// 
// 		if (nCheckedTerminalCount< 1)
// 			throw 1;
#pragma endregion 체크항목에 대한 min max 버전 소스

		int nCurSel = m_cboMode.GetCurSel();
		if (nCurSel < 0)
			throw 1;

		CString strWhereMode(_T(""));
		if (nCurSel > 0) // 0 은 All
		{
			int nModeID = (int)m_cboMode.GetItemData(nCurSel);
			strWhereMode.AppendFormat(_T(" AND mode = %d "), nModeID);
		}

		CString strWhereDTerminal(_T(""));
#pragma region 체크항목에 대한 min max 버전 소스
// 		if (nCheckedTerminalCount != nTotalCount)
// 		{
// 			if (nCheckedTerminalCount >= nUnCheckedTerminalCount)
// 			{
// 				strWhereDTerminal.AppendFormat(_T(" AND d_terminal not in (%s) "), strUnCheckedTerminalIDs);
// 			}
// 			else
// 			{
// 				strWhereDTerminal.AppendFormat(_T(" AND d_terminal in (%s) "), strCheckedTerminalIDs);
// 			}
// 		}
#pragma endregion 체크항목에 대한 min max 버전 소스
		strWhereDTerminal.AppendFormat(_T(" AND d_terminal in (%s) "), strDestTerminalIDs);

		Integer nxOrgTerminalID = GetSelStartNodeId();
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Append      (_T(" Select min(TSum), max(Tsum) From "));
		strSQL.Append	   (_T(" ( "));
		strSQL.AppendFormat		(_T(" Select d_terminal, sum(%s) as TSum From %s "), m_strTripFieldName, TABLE_TERMINAL_SCHEDULE);
		strSQL.AppendFormat		(_T(" Where o_terminal = %I64d "), nxOrgTerminalID);
		strSQL.AppendFormat		(_T(" %s "), strWhereDTerminal);
		strSQL.AppendFormat		(_T(" %s "), strWhereMode);
		strSQL.Append   		(_T(" Group By d_terminal "));
		strSQL.Append      (_T(" ) "));

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strMinVal("");
		CString strMaxVal("");
		if( pResult->Next() )
		{
			double dMinValue = pResult->GetValueDouble(0);
			double dMaxValue = pResult->GetValueDouble(1);

 			dMinValue -= 0.1;
 			dMaxValue += 0.1;

			if (dMinValue < 0.0)
			{
				dMinValue = 0.0;
			}

			strMinVal.Format(_T("%g"), dMinValue);
			strMaxVal.Format(_T("%g"), dMaxValue);
		}

		SetDlgItemText(IDC_EDIT_MIN_VALUE, strMinVal);
		SetDlgItemText(IDC_EDIT_MAX_VALUE, strMaxVal);
	}
	catch (...)
	{
		SetDlgItemText(IDC_EDIT_MIN_VALUE, _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE, _T(""));
		TxLogDebugException();	
	}
}


void KTerminalScheduleDesireDlg::OnBnClickedButtonDraw()
{
	DoneDraw(true); 
}


void KTerminalScheduleDesireDlg::DoneDraw(bool a_bShowErrMsg) 
{
	try
	{
		if (UserDrawInputCheck(a_bShowErrMsg) == false)
		{
			ClearMapDraw(true);
			return;
		}
		ClearMapDraw(false);

		CWaitCursor cw;

		TPathVolumeSettingInfo oTVolumeSettingInfo;
		oTVolumeSettingInfo.bUseOutRange = false;
		oTVolumeSettingInfo.clrOutRange  = RGB(255, 255, 255);
		oTVolumeSettingInfo.clrPath      = m_cpLineColor.GetColor();

		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
		oTVolumeSettingInfo.dMinTrip     = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
		oTVolumeSettingInfo.dMaxTrip     = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		oTVolumeSettingInfo.dMinSize     = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		oTVolumeSettingInfo.dMaxSize     = _ttof(strValue);

		std::vector<TUserLineInfo> vecDrawUserLineInfo;
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		GetDrawVolumeInfo(oTVolumeSettingInfo, vecDrawUserLineInfo, vecDrawNodeInfo);

		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		m_pMapView->TxDrawCompositeUserLine(vecDrawUserLineInfo, bLabel, false);
		m_pMapView->TxDrawCompositeNode    (vecDrawNodeInfo,     bLabel, true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KTerminalScheduleDesireDlg::UserDrawInputCheck(bool a_bShowErrMsg)
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		if (IsCheckedDesTerminal() == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("선택한 종점 터미널 노드가 존재하지 않습니다."));
			}
			
			return false;
		}

		//min value 체크
		GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MAX_VALUE)->SetFocus();		
			return false;
		}
		dMaxValue = _ttof(strValue);

		//IDC_EDIT_MINSIZE
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
		}
		dMinSize = _ttof(strValue);

		//IDC_EDIT_MAXSIZE
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MAXSIZE)->SetFocus();		
			return false;
		}
		dMaxSize = _ttof(strValue);

		if (dMinValue > dMaxValue)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
			return false;
		}

		if (dMinSize > dMaxSize)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			}
			
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
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
	return true;
}


bool KTerminalScheduleDesireDlg::IsCheckedDesTerminal()
{
	try
	{
		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			pRecord      = pRecords->GetAt(i);
			pItem        = pRecord->GetItem(_0_COLUMN_CHECK);
			if (pItem->IsChecked() == TRUE)
			{
				return true;
			}
		}
	}
	catch (...)
	{
		return false;
		TxLogDebugException();	
	}

	return false;
}


void KTerminalScheduleDesireDlg::GetDrawVolumeInfo(TPathVolumeSettingInfo &a_oSettingInfo, std::vector<TUserLineInfo> &a_vecDrawUserLineInfo, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo)
{
	a_vecDrawUserLineInfo.clear();
	a_vecDrawNodeInfo.clear();

	Integer nxOrgTerminalID = GetSelStartNodeId();

	try
	{
		CXTPReportRecords*          pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		Integer  nxDesTerminalID(0);
		CString  strPassengers  (_T(""));
		double   dPassengers    (0);
		COLORREF clrLine        (m_cpLineColor.GetColor());
		double   dMaxVal        (a_oSettingInfo.dMaxTrip);
		double   dMinVal        (a_oSettingInfo.dMinTrip);

		TDrawNodeInfo oTDrawNodeInfo;
		oTDrawNodeInfo.TColorRGB       = RGB(255,165,0);
		oTDrawNodeInfo.TSize           = 15.0;
		oTDrawNodeInfo.TSymbol = 72;

		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			pRecord      = pRecords->GetAt(i);
			pItem        = pRecord->GetItem(_0_COLUMN_CHECK);
			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}
			nxDesTerminalID = (Integer)pItem->GetItemData();

			pItem          = pRecord->GetItem(_3_COLUMN_VALUE);
			strPassengers  = pItem->GetCaption();
			pItemNum       = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_VALUE);
			dPassengers    = pItemNum->GetValue();

#pragma region UserLine Data 수집
			KODKey oODTerminal(nxOrgTerminalID, nxDesTerminalID);
			
			TUserLineInfo oTerminalLine;
			oTerminalLine.TLabel    = strPassengers;
			Node2XYCoordinate(m_pTarget, oODTerminal, oTerminalLine.TPoints);

			if (dPassengers > dMaxVal)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					continue;
				}
				oTerminalLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oTerminalLine.TWidth	= a_oSettingInfo.dMinSize;
			}
			else if (dPassengers < dMinVal)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					continue;
				}
				oTerminalLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oTerminalLine.TWidth	= a_oSettingInfo.dMinSize;
			}
			else
			{
				oTerminalLine.TColorRGB = clrLine;
				double dWidth(0.0);
				CalculateValue2Width(dPassengers, a_oSettingInfo, dWidth);
				oTerminalLine.TWidth    = dWidth;
			}

			a_vecDrawUserLineInfo.push_back(oTerminalLine);
#pragma endregion UserLine Data 수집

#pragma region Node Data 수집
			oTDrawNodeInfo.TNodeID         = nxDesTerminalID;
			CString &strDesTerminalID      = oTDrawNodeInfo.TLabel; 
			strDesTerminalID.Format(_T("%I64d"), nxDesTerminalID);

			a_vecDrawNodeInfo.push_back(oTDrawNodeInfo);
#pragma endregion Node Data 수집
		}

		if (nCount > 0)
		{
			oTDrawNodeInfo.TColorRGB       = RGB(255,  0,  0);
			oTDrawNodeInfo.TSize           = 20.0;
			oTDrawNodeInfo.TNodeID         = nxOrgTerminalID;
			CString &strOrgTerminalID      = oTDrawNodeInfo.TLabel; 
			strOrgTerminalID.Format(_T("%I64d"), nxOrgTerminalID);

			a_vecDrawNodeInfo.push_back(oTDrawNodeInfo);
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


void KTerminalScheduleDesireDlg::Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint )
{
	a_vecMapPoint.clear();

	Integer nxStartID = oODKey.OriginID;
	Integer nxEndID   = oODKey.DestinationID;

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL (_T(""));
		strSQL.Format(_T(" Select x, y From %s Where node_id in (%I64d) "), TABLE_NODE, nxStartID);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			double dx = spResult->GetValueDouble(0);
			double dy = spResult->GetValueDouble(1);

			TMapPoint oPoint;
			oPoint.Tx = dx;
			oPoint.Ty = dy;

			a_vecMapPoint.push_back(oPoint);
		}

		strSQL.Format(_T(" Select x, y From %s Where node_id in (%I64d) "), TABLE_NODE, nxEndID);

		spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			double dx = spResult->GetValueDouble(0);
			double dy = spResult->GetValueDouble(1);

			TMapPoint oPoint;
			oPoint.Tx = dx;
			oPoint.Ty = dy;

			a_vecMapPoint.push_back(oPoint);
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


void KTerminalScheduleDesireDlg::CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


void KTerminalScheduleDesireDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bShowLabel(false);

		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bShowLabel = true;
		else
			bShowLabel = false;

		m_pMapView->TxDrawCompositeNodeLabel    (bShowLabel);
		m_pMapView->TxDrawShowCompositeUserLineLablel(bShowLabel);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleDesireDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(true);
}


void KTerminalScheduleDesireDlg::ClearMapDraw(bool a_bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(a_bRefresh);
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


void KTerminalScheduleDesireDlg::OnBnClickedCancel()
{
	ClearMapDraw(true);
	if (nullptr != m_spImTasIDFind)
	{
		m_spImTasIDFind.reset();
	}
	
	KResizeDialogEx::OnCancel();
}

void KTerminalScheduleDesireDlg::ModelessCancleFeedback(UINT nDialogID)
{

}
