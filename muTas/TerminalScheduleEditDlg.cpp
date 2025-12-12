// TerminalScheduleEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TerminalScheduleEditDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "CustomReportRecordItem.h"
#include "ImChampMapDefine.h"
#include "MapView.h"

namespace TerminalScheduleENUM
{
	enum
	{
		_0_COLUMN_SELECT     = 0,
		_1_COLUMN_ID         = 1,
		_2_COLUMN_NAME       = 2,
		_3_COLUMN_MODE       = 3,
		_4_COLUMN_TIME       = 4,
		_5_COLUMN_COST       = 5,
		_6_COLUMN_FREQ       = 6,
		_7_COLUMN_HEAD		 = 7
	};
}

using namespace TerminalScheduleENUM;
// KTerminalScheduleEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTerminalScheduleEditDlg, KResizeDialogEx)

KTerminalScheduleEditDlg::KTerminalScheduleEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTerminalScheduleEditDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{
	m_oTerminalNodeIdCaption.TID = _T("-1");

	RegisterUsedTerminalType();

	m_clrOrg     = RGB (   255, 0,   0 );
	m_clrDest    = RGB (   0,   0, 255 );
}

KTerminalScheduleEditDlg::~KTerminalScheduleEditDlg()
{
	CloseWindowProc();
	m_backBrush.DeleteObject();
}

void KTerminalScheduleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO1, m_cboTerminalType);
	DDX_Control(pDX, IDC_COMBO2, m_cboLineLabel);
	DDX_Control(pDX, IDC_BUTTON_ORIG,                 m_cpOrgColor);
	DDX_Control(pDX, IDC_BUTTON_DEST,                 m_cpDestColor);
}


BEGIN_MESSAGE_MAP(KTerminalScheduleEditDlg, KResizeDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON3, &KTerminalScheduleEditDlg::OnBnClickedButtonSearchTerminal)

	ON_BN_CLICKED(IDC_BUTTON4,    &KTerminalScheduleEditDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL,       &KTerminalScheduleEditDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1,     &KTerminalScheduleEditDlg::OnBnClickedCheckSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,      IDC_REPORT, &KTerminalScheduleEditDlg::OnReportCheckItem)
	ON_NOTIFY(NM_CLICK,                   IDC_REPORT, &KTerminalScheduleEditDlg::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KTerminalScheduleEditDlg::OnReportItemValueChanged)
	ON_BN_CLICKED(IDC_BUTTON2,    &KTerminalScheduleEditDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON1,    &KTerminalScheduleEditDlg::OnBnClickedButtonAdd)
	
	ON_BN_CLICKED(IDC_CHECK2,     &KTerminalScheduleEditDlg::OnBnClickedCheckLabel)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KTerminalScheduleEditDlg::OnCbnSelchangeComboLabel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTerminalScheduleEditDlg::OnCbnSelchangeComboType)

	ON_CPN_XTP_SELENDOK(IDC_BUTTON_ORIG, &KTerminalScheduleEditDlg::OnSelEndOkOrgClr)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_DEST, &KTerminalScheduleEditDlg::OnSelEndOkDestClr)
END_MESSAGE_MAP()


// KTerminalScheduleEditDlg 메시지 처리기입니다.


void KTerminalScheduleEditDlg::ActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(!m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
			}
		}

		if(nullptr != m_spImTasIDFind)
		{
			if(!m_spImTasIDFind->IsUserClosed())
			{
				if(!m_spImTasIDFind->IsWindowVisible())
					m_spImTasIDFind->ShowWindow(SW_SHOW);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::DeActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
			}
		}

		if(nullptr != m_spImTasIDFind)
		{
			if(!m_spImTasIDFind->IsUserClosed())
			{
				if(m_spImTasIDFind->IsWindowVisible())
					m_spImTasIDFind->ShowWindow(SW_HIDE);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KTerminalScheduleEditDlg::CloseWindowProc()
{
	try
	{
		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		if (nullptr != m_spImTasIDFind)
			m_spImTasIDFind.reset();

		ClearAllMap(true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}



void KTerminalScheduleEditDlg::InitializeSet()
{
	try
	{
		GetTerminalNodeInfo();
		GetRegionalMode();
		GetRegionModeDefineDB();
		KBulkDBase::BulkNodeSelectAll(m_pTarget, m_mapAllNodeInfo);

		InputGridComboData();

		m_oTerminalNodeIdCaption.TID = _T("-1");
		SetDlgItemText(IDC_EDIT1, _T(""));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::GetRegionModeDefineDB()
{
	try
	{
		m_mapResionMathingCode.clear();

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("Select Terminal_type, regional_mode From %s "), TABLE_REGIONAL_MODE_DEFINE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		std::vector<int> vecTranistComplex;

		while(spResultSet->Next())
		{
			int nTerminalCode = spResultSet->GetValueInt(0);
			int nRegionalCode = spResultSet->GetValueInt(1);

			AutoType iter  = m_mapResionMathingCode.find(nTerminalCode);
			AutoType itEnd = m_mapResionMathingCode.end();

			if (iter != itEnd)
			{
				std::vector<int> &vecRegionalCode = iter->second;
				vecRegionalCode.push_back(nRegionalCode);
				vecTranistComplex.push_back(nRegionalCode);
			}
			else
			{
				std::vector<int> vecRegionalCode;
				vecRegionalCode.push_back(nRegionalCode);
				vecTranistComplex.push_back(nRegionalCode);

				m_mapResionMathingCode.insert(std::make_pair(nTerminalCode, vecRegionalCode));
			}
		}

		//Transit Complex는 모든 터미널 데이터를 가지고 있어야됨
		AutoType find		= m_mapResionMathingCode.find(TRANSITCOMPLEX);
		AutoType findend	= m_mapResionMathingCode.end();
		if (find != findend)
		{
			std::vector<int> & vecRegionalCode = find->second;
			vecRegionalCode = vecTranistComplex;
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


void KTerminalScheduleEditDlg::GetRegionalMode()
{
	TxLogDebugStartMsg();

	try
	{
		m_vecTransitModeCode.clear();

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.AppendFormat (_T(" select regional_mode from %s "), TABLE_REGIONAL_MODE_DEFINE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		int nModeCode(0);
		while( spResult->Next() )
		{
			nModeCode	    = spResult->GetValueInt (0);
			m_vecTransitModeCode.push_back(nModeCode);
		}
	}
	catch (...)
	{
		m_vecTransitModeCode.clear();
		TxLogDebugException();		
	}

	TxLogDebugEndMsg();
}


bool KTerminalScheduleEditDlg::GetTerminalNodeInfo()
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		AutoType iter  = m_setRegTerminalType.begin();
		AutoType itEnd = m_setRegTerminalType.end();

		CString strTypes(_T(""));
		int nIdx(0);
		while (iter != itEnd)
		{
			int nCode     = *iter;

			if (nIdx > 0)
			{
				strTypes.Append(_T(","));
			}
			strTypes.AppendFormat (_T(" %d "),  nCode);

			++nIdx;
			++iter;
		}

		CString strSQL(_T(""));
		strSQL.Append(_T(" SELECT node_id, name, system_code_key, schedule_cnt "));
		strSQL.Append(_T(" FROM "));
		strSQL.Append(_T(" (SELECT TBL1.node_id as node_id, TBL1.name as name, TBL2.system_code_key as system_code_key FROM "));
		strSQL.AppendFormat(_T(" (SELECT node_id, name, node_type FROM %s) TBL1, "), TABLE_NODE);
		strSQL.AppendFormat(_T(" (SELECT user_code_key, system_code_key FROM %s "),  TABLE_DEFINE_SYSTEM_CODE);
		strSQL.AppendFormat(_T("  WHERE  user_code_group = %d "),   USER_NODECODE_GROUP_KEY);
		strSQL.AppendFormat(_T("  AND    system_code_group = %d "), SYSTEM_NODECODE_GROUP_KEY);
		strSQL.AppendFormat(_T("  AND    System_code_key IN (%s))TBL2 "), strTypes);
		strSQL.Append(_T(" WHERE TBL1.node_type = TBL2.user_code_key)TBL3 "));
		strSQL.Append(_T(" LEFT OUTER JOIN "));
		strSQL.Append(_T(" (SELECT o_terminal, count(*) schedule_cnt FROM terminal_schedule group by o_terminal)TBL4 "));
		strSQL.Append(_T(" ON "));
		strSQL.Append(_T(" TBL3.node_id = TBL4.o_terminal "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		m_mapNodeData.clear();
		m_vecTerminalIDCaption.clear();

		m_mapTypeNodeData.clear();
		m_mapTypeNodeIDCaption.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));
		int     nType(0);
		int     nScheduleCnt(0);
		CString strScheduleCnt(_T(""));

		while( spResult->Next() )
		{
			nxNodeId            = spResult->GetValueInt64 (0);
			strNodeName         = spResult->GetValueString(1);
			nType               = spResult->GetValueInt   (2);
			nScheduleCnt        = spResult->GetValueInt   (3);

			m_mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			AutoType iter  = m_mapTypeNodeData.find(nType);
			AutoType itEnd = m_mapTypeNodeData.end();

			if (iter != itEnd)
			{
				KTDNodeDataMap &mapNodeData = iter->second;
				mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));
			}
			else
			{
				KTDNodeDataMap mapNodeData;
				mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));
				m_mapTypeNodeData.insert(std::make_pair(nType, mapNodeData));
			}

			KID2ColumnCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			strScheduleCnt.Format(_T("%d"), nScheduleCnt);

			oIDCaption.TID       = strNodeID;
			oIDCaption.TCaption  = strNodeName;
			oIDCaption.TCaption2 = strScheduleCnt;

			m_vecTerminalIDCaption.push_back(oIDCaption);

			AutoType iterB  = m_mapTypeNodeIDCaption.find(nType);
			AutoType itEndB = m_mapTypeNodeIDCaption.end();

			if (iterB != itEndB)
			{
				std::vector<KID2ColumnCaption> &vecTerminalIDCaption = iterB->second;
				vecTerminalIDCaption.push_back(oIDCaption);
			}
			else
			{
				std::vector<KID2ColumnCaption> vecTerminalIDCaption;
				vecTerminalIDCaption.push_back(oIDCaption);
				m_mapTypeNodeIDCaption.insert(std::make_pair(nType, vecTerminalIDCaption));
			}
		}
	}
	catch (...)
	{
		m_mapNodeData.clear();
		m_mapTypeNodeData.clear();
		m_mapTypeNodeIDCaption.clear();

		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KTerminalScheduleEditDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind2Column::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxID);

				CString strIDView(_T(""));
				SetNodeViewData(strID, strIDView, m_oTerminalNodeIdCaption);
				SetDlgItemText(IDC_EDIT1,  strIDView);

				//ClearAllMap(true);
				UpdateSchedule(nxID);

				/*
				if(nullptr != m_spImTasMultiIDFind)
				{
					if(!m_spImTasMultiIDFind->IsUserClosed())
					{
						if(m_spImTasMultiIDFind->IsWindowVisible())
						{
							OnBnClickedButtonAdd(); // 선택한 터미널이 제거된 리스트를 재 오픈 해야함
						}
					}
				}
				*/
				m_spImTasMultiIDFind.reset();
			}
			catch (...)
			{
				ClearAllMap(true);
				TxLogDebugException();				
			}
		}
	}
	else if (KImChampMultiIDFind2Column::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				CString           strIDs = m_spImTasMultiIDFind->GetSelectedID();
				std::set<CString> setID  = m_spImTasMultiIDFind->GetSelectedIDSet();
				AddReportData(setID);

				DrawTerminalLine();
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KTerminalScheduleEditDlg::ClearAllMap(bool bRefresh)
{
	try
	{
		m_pMapView->ClearScreenDrawEdit(false);
		m_pMapView->TxClearCompositeThemes(false);

        if (bRefresh)
            m_pMapView->MapRefresh();
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


void KTerminalScheduleEditDlg::UpdateSchedule(Integer a_nxTerminalID)
{
	try
	{
		InputGridComboData();

		std::vector<TTerminalScheduleInfo> vecSchedule;
		GetTerminalScheduleData(a_nxTerminalID, vecSchedule);
		UpdateReportData(vecSchedule);

		DrawTerminalLine(); 
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::GetTerminalScheduleData(Integer a_nxTerminalID, std::vector<TTerminalScheduleInfo> &a_vecSchedule)
{
	TxLogDebugStartMsg();

	try
	{
		a_vecSchedule.clear();

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" select d_terminal, name, mode, travel_time, cost, frequency, headway from %s where o_Terminal = %I64d "), TABLE_TERMINAL_SCHEDULE, a_nxTerminalID);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			TTerminalScheduleInfo oTScheduleInfo;

			oTScheduleInfo.nxDestTerminalID	    = spResult->GetValueInt64 (0);
			oTScheduleInfo.strName              = spResult->GetValueString(1);
			oTScheduleInfo.nMode                = spResult->GetValueInt   (2);
			oTScheduleInfo.dTravelTime          = spResult->GetValueDouble(3);
			oTScheduleInfo.dCost                = spResult->GetValueDouble(4);
			oTScheduleInfo.nFrequency           = spResult->GetValueInt(5);
			oTScheduleInfo.dHeadway				= spResult->GetValueDouble(6);

			a_vecSchedule.push_back(oTScheduleInfo);
		}
	}
	catch (...)
	{
		a_vecSchedule.clear();
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KTerminalScheduleEditDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR6,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_CHECK1,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK2,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO2,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4, 			              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KTerminalScheduleEditDlg::RegisterUsedTerminalType()
{
	try
	{
		m_setRegTerminalType.clear();

		m_setRegTerminalType.insert(TRANSITCOMPLEX);
		m_setRegTerminalType.insert(INTER_REGIONAL_RAILSTATION);
		m_setRegTerminalType.insert(BUS_TRMINAL);
		m_setRegTerminalType.insert(INTER_REGIONAL_BUS_TRANSFER);
		m_setRegTerminalType.insert(AIR_TERMIANL);
		m_setRegTerminalType.insert(MARIN_PORT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::InitComboTerminalType()
{
	try
	{
		m_cboTerminalType.ResetContent();

		std::map<int, CString> mapModeCode;
		KCodeManager*          pCodeMgr		= m_pTarget->CodeManager();
		KCodeGroup*	           pCodeGroup	= pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		CString strName(_T(""));
		int nCur(0);
		int nCode(0);

		AutoType iter  = m_setRegTerminalType.begin();
		AutoType itEnd = m_setRegTerminalType.end();

		while (iter != itEnd)
		{
			nCode     = *iter;
			strName   = pCodeGroup->SingleCodeValue(nCode);
			nCur      = m_cboTerminalType.AddString(strName);
			m_cboTerminalType.SetItemData(nCur, nCode);

			++iter;
		}

		if (m_setRegTerminalType.size() > 0)
		{
			m_cboTerminalType.SetCurSel(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::InitComboLabel()
{
	try
	{
		m_cboLineLabel.ResetContent();

		int nCur(0);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboLineLabel.AddString(_T("운행시간"));
		}
		else {
			nCur = m_cboLineLabel.AddString(_T("TravelTime"));
		}
		m_cboLineLabel.SetItemData(nCur, _4_COLUMN_TIME);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboLineLabel.AddString(_T("요금"));
		}
		else {
			nCur = m_cboLineLabel.AddString(_T("Cost"));
		}
		m_cboLineLabel.SetItemData(nCur, _5_COLUMN_COST);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboLineLabel.AddString(_T("운행횟수"));
		}
		else {
			nCur = m_cboLineLabel.AddString(_T("Frequency"));
		}
		m_cboLineLabel.SetItemData(nCur, _6_COLUMN_FREQ);

		m_cboLineLabel.SetCurSel(2);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTerminalScheduleEditDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_ctrlReport);
	InitReportHeader();
	m_ctrlReport.Populate();

	InitComboTerminalType();
	InitComboLabel();

	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	KColorPickerSetting::DefaultCtrl(m_cpOrgColor,      m_clrOrg);
	KColorPickerSetting::DefaultCtrl(m_cpDestColor,     m_clrDest);

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTerminalScheduleEditDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,  _T("Select"),              40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_ID,      _T("Destination Terminal"),      17));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도착 터미널"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME,    _T("Name"),                18));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("노선"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_MODE,    _T("Mode"),                20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("수단 명칭"));
        }

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		pEditOptions->AddComboButton(TRUE);
		pEditOptions->m_bAllowEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_4_COLUMN_TIME,    _T("TravelTime"),          15));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("운행시간"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_5_COLUMN_COST,    _T("Cost"),                15));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("요금"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_6_COLUMN_FREQ,    _T("Frequency"),           15));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("운행횟수"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_7_COLUMN_HEAD,	_T("Head Way"), 15));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("배차간격"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::InputGridComboData()
{
	try
	{
		m_ctrlReport.ResetContent();
		
		CXTPReportColumn*                pColumn      = m_ctrlReport.GetColumns()->GetAt(_3_COLUMN_MODE);
		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();

		KCodeManager* pCodeMgr		   = m_pTarget->CodeManager();
		KCodeGroup*	  pCodeGroup	   = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

		pEditOptions->GetConstraints()->RemoveAll();

		int nCurSel       = m_cboTerminalType.GetCurSel();
		int nTerminalType = m_cboTerminalType.GetItemData(nCurSel);

		AutoType iter     = m_mapResionMathingCode.find(nTerminalType);
		AutoType itEnd    = m_mapResionMathingCode.end();

		if (iter != itEnd)
		{
			std::vector<int> &vecRegionalCode = iter->second;

			int nCount = vecRegionalCode.size();
			for (int i= 0; i <nCount; i++)
			{
				int     nCode      = vecRegionalCode[i];
				CString strCaption = pCodeGroup->SingleCodeValue(nCode);
				pEditOptions->AddConstraint(strCaption, nCode);
			}
		}

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::UpdateReportData(std::vector<TTerminalScheduleInfo> &a_vecSchedule)
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bSelectAll = TRUE;
		}

		m_ctrlReport.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		for (size_t i= 0; i< a_vecSchedule.size(); i++)
		{
			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			TTerminalScheduleInfo &oTTerminalScheduleInfo = a_vecSchedule[i];

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(TRUE);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bSelectAll);
			pItem->SetAlignment(xtpColumnIconCenter);

			Integer &nxDestTerminalID = oTTerminalScheduleInfo.nxDestTerminalID; 
			CString strDestID(_T(""));
			strDestID.Format(_T("%I64d"), nxDestTerminalID);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strDestID));
			pItem->SetItemData((DWORD_PTR) nxDestTerminalID);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTTerminalScheduleInfo.strName));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_LEFT);

			pItem   = pRecord->AddItem(new CCustomReportRecordItem);
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR)oTTerminalScheduleInfo.nMode);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalScheduleInfo.dTravelTime, _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalScheduleInfo.dCost,       _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oTTerminalScheduleInfo.nFrequency,  _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalScheduleInfo.dHeadway,	  _T("%.3f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

		}

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::AddReportData( std::set<CString> &a_setID)
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bSelectAll = TRUE;
		}

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = a_setID.begin();
		AutoType itEnd = a_setID.end();

		while (iter != itEnd)
		{
			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			CString strID = *iter;

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(TRUE);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bSelectAll);
			pItem->SetAlignment(xtpColumnIconCenter);

			Integer nxDestTerminalID = _ttoi64(strID); 

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetItemData((DWORD_PTR) nxDestTerminalID);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_LEFT);

			pItem   = pRecord->AddItem(new CCustomReportRecordItem);
			pItem->SetAlignment(DT_LEFT);
			//pItem->SetItemData((DWORD_PTR)oTTerminalScheduleInfo.nMode);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0,  _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0,  _T("%.1f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		if (a_setID.size() > 0)
		{
			m_ctrlReport.Populate();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::SetNodeViewData(CString a_strNodeID, CString &a_strNodeViewData, KID2ColumnCaption &a_oIDCaption)
{
	try
	{
		Integer nxNodeID = _ttoi64(a_strNodeID);

		AutoType iter  = m_mapNodeData.find(nxNodeID);
		AutoType itEnd = m_mapNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TID      = a_strNodeID;
		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(strName);

		a_strNodeViewData.Format(_T("%s [%s]"), a_strNodeID, strName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


Integer KTerminalScheduleEditDlg::GetSelTerminalNodeId()
{
	Integer nxNodeId      = -1;

	try
	{
		nxNodeId = _ttoi64(m_oTerminalNodeIdCaption.TID);
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



void KTerminalScheduleEditDlg::OnBnClickedButtonSave()
{
	try
	{
		if (GetSelTerminalNodeId() < 1)
		{
			AfxMessageBox(_T("터미널 노드를 선택해 주세요."));

			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
			return;

		SaveScheduleData();

		AfxMessageBox(_T("변경사항이 저장되었습니다."));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::SaveScheduleData()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	Integer nxOTerminalID = GetSelTerminalNodeId();
	if (nxOTerminalID < 0)
	{
		return;
	}

	CString strSQLSelect(_T(""));
	strSQLSelect.Format(_T(" SELECT max(seq)+1 FROM %s "), TABLE_TERMINAL_SCHEDULE);

	CString strSQLDelete(_T(""));
	strSQLDelete.Format(_T(" DELETE FROM %s WHERE o_terminal = %I64d "),TABLE_TERMINAL_SCHEDULE, nxOTerminalID);
	
	CString strSQLInsert(_T(""));
	strSQLInsert.AppendFormat(_T(" Insert Into %s "), TABLE_TERMINAL_SCHEDULE);
	strSQLInsert.Append      (_T(" ( seq, o_terminal, d_terminal, name, mode, travel_time, cost, frequency, headway "));
	strSQLInsert.Append      (_T(" ) Values (?, ?, ?, ?, ?, ?, ?, ?, ?) "));
	  
	try
	{
		std::vector<TTerminalScheduleInfo> vecSchedule;
		GetReportData(vecSchedule);

		spDBaseConnection->BeginTransaction();

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQLSelect);
		int nMaxSeq(0);
		if ( pResult->Next() )
		{
			nMaxSeq	    = pResult->GetValueInt (0);
		}
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		if (vecSchedule.size() > 0)
		{
			KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

			for( size_t i= 0; i< vecSchedule.size(); i++ )
			{
				TTerminalScheduleInfo &oRecord = vecSchedule[i];

				spPrepareStatement->BindInt        ( 1, nMaxSeq++                );
				spPrepareStatement->BindInt64      ( 2, nxOTerminalID            );
				spPrepareStatement->BindInt64      ( 3, oRecord.nxDestTerminalID );
				spPrepareStatement->BindText       ( 4, oRecord.strName          );
				spPrepareStatement->BindInt        ( 5, oRecord.nMode            );
				spPrepareStatement->BindDouble     ( 6, oRecord.dTravelTime      );
				spPrepareStatement->BindDouble     ( 7, oRecord.dCost            );
				spPrepareStatement->BindInt		   ( 8, oRecord.nFrequency       );
				spPrepareStatement->BindDouble	   ( 9, oRecord.dHeadway         ); 

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}

		spDBaseConnection->Commit();
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();	
		return;
	}

	try
	{
		KIOTables* pTables = m_pTarget->Tables();
		KIOTable*  pTable  = pTables->FindTable(TABLE_TERMINAL_SCHEDULE);

		pTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::GetReportData(std::vector<TTerminalScheduleInfo> &a_vecSchedule)
{
	try
	{
		CXTPReportRecords*          pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		size_t nxCount = pRecords->GetCount(); 
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord    = pRecords->GetAt(i);

			TTerminalScheduleInfo oTTerminalScheduleInfo;

			pItem      = pRecord->GetItem(_1_COLUMN_ID);
			oTTerminalScheduleInfo.nxDestTerminalID = pItem->GetItemData();

			pItem      = pRecord->GetItem(_2_COLUMN_NAME);
			oTTerminalScheduleInfo.strName = pItem->GetCaption();

			pItem      = pRecord->GetItem(_3_COLUMN_MODE);
			oTTerminalScheduleInfo.nMode = pItem->GetItemData();

			pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_TIME);
			oTTerminalScheduleInfo.dTravelTime = pItemNum->GetValue();

			pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_COLUMN_COST);
			oTTerminalScheduleInfo.dCost = pItemNum->GetValue();

			pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_6_COLUMN_FREQ);
			oTTerminalScheduleInfo.nFrequency = (int)pItemNum->GetValue();

			pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_7_COLUMN_HEAD);
			oTTerminalScheduleInfo.dHeadway	= pItemNum->GetValue();

			a_vecSchedule.push_back(oTTerminalScheduleInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}



void KTerminalScheduleEditDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bSelectAll = TRUE;
		}

		CXTPReportRecords*          pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;

		size_t nxCount = pRecords->GetCount(); 
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord    = pRecords->GetAt(i);


			pItem      = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_COLUMN_SELECT == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(nIndex);

			if(pItem->IsChecked() == FALSE)
			{
				CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KTerminalScheduleEditDlg::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		if ( _1_COLUMN_ID == nColumnIndex || _0_COLUMN_SELECT == nColumnIndex )
		{
			CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_1_COLUMN_ID);            
			Integer               nxID    = (Integer)pItem->GetItemData();
			if (m_pMapView != nullptr)
			{
				m_pMapView->PanToFeature(KLayerID::Node(), nxID);
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}    
}


void KTerminalScheduleEditDlg::OnReportItemValueChanged( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (_6_COLUMN_FREQ == nIndex)
		{
			DrawTerminalLine();
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


void KTerminalScheduleEditDlg::OnBnClickedButtonDelete()
{
	try
	{
		CXTPReportRecords*          pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;

		std::vector<CXTPReportRecord*> vecDelRecord;

		size_t nxCount = pRecords->GetCount(); 
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord    = pRecords->GetAt(i);

			pItem      = pRecord->GetItem(_0_COLUMN_SELECT);
			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			vecDelRecord.push_back(pRecord);
		}

		size_t nxDelRecordCount = vecDelRecord.size();

		if (nxDelRecordCount < 1)
		{
			return;
		}

		/*if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
		{
			return;
		}*/

		for (size_t i= 0; i< nxDelRecordCount; i++)
		{
			CXTPReportRecord* pRecord = vecDelRecord[i];
			m_ctrlReport.RemoveRecordEx(pRecord);
		}

		m_ctrlReport.Populate();
		DrawTerminalLine();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::OnBnClickedButtonAdd()
{
	try
	{
		if (GetSelTerminalNodeId() < 1)
		{
			AfxMessageBox(_T("터미널 노드를 선택해 주세요."));

			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		int nCurSel = m_cboTerminalType.GetCurSel();
		if (nCurSel < 0)
		{
			return;
		}

		int nType = m_cboTerminalType.GetItemData(nCurSel);
		KTDNodeDataMap                 mapNodeData;
		std::vector<KID2ColumnCaption> vecNodeIDCaption;

        // 2014-09-24 : Transit Complex는 모든 타입의 Destination이 된다.
		GetSelectedTypeNodeData     (nType, mapNodeData, 1);
		GetSelectedTypeNodeIDCaption(nType, vecNodeIDCaption, 1);

		FilterSelfOrginID           (vecNodeIDCaption, mapNodeData);

		CWaitCursor cw;

		m_spImTasMultiIDFind = KImChampMultiIDFind2ColumnPtr(new KImChampMultiIDFind2Column(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(vecNodeIDCaption);
		m_spImTasMultiIDFind->SetTitleName(_T("Select Destination Terminal"));
		m_spImTasMultiIDFind->SetReportTitle(_T("Select Destination Terminal ID :"));
		m_spImTasMultiIDFind->SetSecondColumnname(_T("No. of Schedule"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_spImTasMultiIDFind->SetTitleName(_T("도착 터미널 선택"));
		m_spImTasMultiIDFind->SetReportTitle(_T("도착 터미널 노드 ID 선택 :"));
		m_spImTasMultiIDFind->SetSecondColumnname(_T("운행 노선수"));
		}
		m_spImTasMultiIDFind->SetSelectedID(_T(""));

		// 지도 선택 옵션
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, mapNodeData, ImChampMapDefineSymbol::NodeSymbol);

		m_spImTasMultiIDFind->Create(KImChampMultiIDFind2Column::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KTerminalScheduleEditDlg::FilterSelfOrginID(std::vector<KID2ColumnCaption> &a_vecTerminal, KTDNodeDataMap &a_mapNodeDataInfo)
{
	try
	{
		Integer nxOriginTerminalID = GetSelTerminalNodeId();

		AutoType iter  = a_mapNodeDataInfo.find(nxOriginTerminalID);
		AutoType itEnd = a_mapNodeDataInfo.end();
		if (iter != itEnd)
		{
			a_mapNodeDataInfo.erase(iter);
		}

		std::vector<KID2ColumnCaption> vecTempTerminal;

		size_t nxCount = a_vecTerminal.size();
		for (size_t i= 0; i< nxCount; i++)
		{
		 	KID2ColumnCaption &oID2ColumnCaption = a_vecTerminal[i];
			Integer           nxTerminalID       = _ttoi64(oID2ColumnCaption.TID);

			if (nxOriginTerminalID == nxTerminalID)
			{
				continue;
			}
			vecTempTerminal.push_back(oID2ColumnCaption);
		}

		a_vecTerminal.clear();
		a_vecTerminal = vecTempTerminal;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::OnBnClickedButtonSearchTerminal()
{
	SelectTerminalNodeList();
}


BOOL KTerminalScheduleEditDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			ClickedEditTerminalNode();

			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KTerminalScheduleEditDlg::ClickedEditTerminalNode()
{
	SelectTerminalNodeList();
}


void KTerminalScheduleEditDlg::SelectTerminalNodeList()
{
	try
	{
		int nCurSel = m_cboTerminalType.GetCurSel();
		if (nCurSel < 0)
		{
			AfxMessageBox(_T("터미널 타입을 선택해 주세요."));
			return;
		}

		int nType = m_cboTerminalType.GetItemData(nCurSel);

		KTDNodeDataMap                 mapNodeData;
		std::vector<KID2ColumnCaption> vecNodeIDCaption;

		GetSelectedTypeNodeData     (nType, mapNodeData);
		GetSelectedTypeNodeIDCaption(nType, vecNodeIDCaption);

		if (vecNodeIDCaption.size() < 1)
		{
			AfxMessageBox(_T("선택한 타입의 터미널 노드 정보가 존재하지 않습니다."));
			return;
		}

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFind2ColumnPtr(new KImChampIDFind2Column(this));
		m_spImTasIDFind->SetModelessReceiver(this);
		m_spImTasIDFind->SetTitleName(_T("Select Origin Terminal"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_spImTasIDFind->SetTitleName(_T("출발 터미널 선택"));
		}
		m_spImTasIDFind->SetIDCaption(vecNodeIDCaption);
		m_spImTasIDFind->SetSelectedID(-1);

		m_spImTasIDFind->SetSecondColumnname(_T("No. of Schedule"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_spImTasIDFind->SetSecondColumnname(_T("운행 노선수"));
		}

		// 지도 선택 옵션
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		m_spImTasIDFind->Create(KImChampIDFind2Column::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KTerminalScheduleEditDlg::GetSelectedTypeNodeData(int a_nType, KTDNodeDataMap &a_mapNodeData, int nTerminalType)
{
	try
	{
        // nombara(2014-09-12) : 삭제 정보를 갱신하기 위해서
        GetTerminalNodeInfo();
        // end nombara

        // nombara(2014-09-24)        
        // nTerminalType : 0(터미널선택), 1 : Destination 선택
        a_mapNodeData.clear();
        if (nTerminalType == 1)
        {
            // 모든 터미널은 Transit Complex로 갈 수 있다.
            // Transit Complex는 모든 터미널 타입과 Transit Complex로 갈 수 있다.
            
            if (a_nType == TRANSITCOMPLEX)
            {
                auto iter = m_mapTypeNodeData.begin();
                auto end  = m_mapTypeNodeData.end();

                while(iter != end)
                {
                    std::map<Integer, CString>& mapNodeData = iter->second;
                    a_mapNodeData.insert(mapNodeData.begin(), mapNodeData.end());
                    ++iter;
                }
            }
            else
            {//자신과 동일한 타입 + Transit Complex
                auto iter = m_mapTypeNodeData.begin();
                auto end  = m_mapTypeNodeData.end();

                while (iter != end)
                {
                    int nType = iter->first;
                    if (a_nType == nType)
                    {
                        std::map<Integer, CString>& mapNodeData = iter->second;
                        a_mapNodeData.insert(mapNodeData.begin(), mapNodeData.end());
                    }

                    if (TRANSITCOMPLEX == nType)
                    {
                        std::map<Integer, CString>& mapNodeData = iter->second;
                        a_mapNodeData.insert(mapNodeData.begin(), mapNodeData.end());
                    }

                    ++iter;
                }                
            }
        }
        else
        {
            AutoType iter  = m_mapTypeNodeData.find(a_nType);
            AutoType itEnd = m_mapTypeNodeData.end();

            if (iter != itEnd)
            {
                a_mapNodeData = iter->second;
            }
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::GetSelectedTypeNodeIDCaption(int a_nType, std::vector<KID2ColumnCaption> &a_vecNodeIDCaption, int nTerminalType)
{
	try
	{
        a_vecNodeIDCaption.clear();

        if (nTerminalType == 1)
        {
            // 모든 터미널은 Transit Complex로 갈 수 있다.
            // Transit Complex는 모든 터미널 타입과 Transit Complex로 갈 수 있다.
            // std::map<int, std::vector<KID2ColumnCaption>> m_mapTypeNodeIDCaption;
            if (a_nType == TRANSITCOMPLEX)
            {
                auto iter = m_mapTypeNodeIDCaption.begin();
                auto end  = m_mapTypeNodeIDCaption.end();

                while (iter != end)
                {
                    std::vector<KID2ColumnCaption>& vecColumnCaption = iter->second;
                    for (size_t i=0; i<vecColumnCaption.size(); i++)
                    {
                        a_vecNodeIDCaption.push_back(vecColumnCaption[i]);
                    }
                    ++iter;
                }
            }
            else
            {//자신과 동일한 타입 + Transit Complex
                auto iter = m_mapTypeNodeIDCaption.begin();
                auto end  = m_mapTypeNodeIDCaption.end();

                while (iter != end)
                {
                    int nType = iter->first;
                    if (a_nType == nType)
                    {
                        std::vector<KID2ColumnCaption>& vecColumnCaption = iter->second;
                        for (size_t i=0; i<vecColumnCaption.size(); i++)
                        {
                            a_vecNodeIDCaption.push_back(vecColumnCaption[i]);
                        }
                    }

                    if (TRANSITCOMPLEX == nType)
                    {
                        std::vector<KID2ColumnCaption>& vecColumnCaption = iter->second;
                        for (size_t i=0; i<vecColumnCaption.size(); i++)
                        {
                            a_vecNodeIDCaption.push_back(vecColumnCaption[i]);
                        }
                    }

                    ++iter;
                }      
            }
        }
        else
        {
            AutoType iter  = m_mapTypeNodeIDCaption.find(a_nType);
            AutoType itEnd = m_mapTypeNodeIDCaption.end();

            if (iter != itEnd)
            {
                a_vecNodeIDCaption = iter->second;
            }
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::DrawTerminalLine()
{
	try
	{
		bool bRefresh(false);

		if (GetSelTerminalNodeId() < 1)
		{
			throw 1;
		}

		Integer nxStartNodeID = GetSelTerminalNodeId();

		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		std::vector<TUserLineInfo> vecDrawLineInfo;

 		GetDestTerminalPointLine  (nxStartNodeID, vecDrawNodeInfo, vecDrawLineInfo);
		if (vecDrawNodeInfo.size() < 1)
			throw 1;

		TPreferenceSymbol &oTNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		TDrawNodeInfo oStartNodeInfo; // 시작 지점
		oStartNodeInfo.TNodeID	 = nxStartNodeID;
		oStartNodeInfo.TSymbol	 = oTNodeSymbol.nCharIndex;
		oStartNodeInfo.TSize	 = oTNodeSymbol.dSize;
		oStartNodeInfo.TColorRGB = m_cpOrgColor.GetColor();
		oStartNodeInfo.TLabel	 = _T("");

		vecDrawNodeInfo.push_back(oStartNodeInfo);

		bool bLineLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
		{
			bLineLabel = true;
		}
		
		ClearAllMap(bRefresh);
		
		m_pMapView->TxDrawCompositeNode    (vecDrawNodeInfo,     false,        bRefresh);
		bRefresh = true;
		m_pMapView->TxDrawCompositeUserLine(vecDrawLineInfo,     bLineLabel,   bRefresh);
	}
	catch (...)
	{
		ClearAllMap(true);
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::GetDestTerminalPointLine(Integer a_nxStartNodeID, std::vector<TDrawNodeInfo> &a_vecDrawDestNodeInfo, std::vector<TUserLineInfo> &a_vecDrawLine)
{
	try
	{
		UpdateData(TRUE);

		int nCurSel       = m_cboLineLabel.GetCurSel();
		int nColTripIndex = (int)m_cboLineLabel.GetItemData(nCurSel);

		a_vecDrawDestNodeInfo.clear(); 
		a_vecDrawLine.clear();

		//DrawLine 시작점
		TMapPoint oStartPoint;
		AutoType iter     = m_mapAllNodeInfo.find(a_nxStartNodeID);
		AutoType itEnd    = m_mapAllNodeInfo.end();
		if (iter != itEnd) {
			TFixedNodeInfo oInfo = iter->second;
			oStartPoint.Tx = oInfo.x;
			oStartPoint.Ty = oInfo.y;
		}
		else {
			return;
		}

		TPreferenceSymbol &oTNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;
		TDrawNodeInfo      oDestDrawNodeInfo;

		oDestDrawNodeInfo.TSymbol	  = oTNodeSymbol.nCharIndex;
		oDestDrawNodeInfo.TSize				  = oTNodeSymbol.dSize;
		oDestDrawNodeInfo.TColorRGB			  = m_cpDestColor.GetColor();

		CXTPReportRecords*          pRecords  = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
// 			if (pItem->IsChecked() == FALSE)
// 				continue;

			pItem   = pRecord->GetItem(_1_COLUMN_ID);
			Integer nxDestNodeID = (Integer)pItem->GetItemData();
			oDestDrawNodeInfo.TNodeID = nxDestNodeID;

			pItemNum             = (CXTPReportRecordItemNumber*)pRecord->GetItem(nColTripIndex);
			double dTrip         = pItemNum->GetValue();

			CString  strLabel(_T(""));
			strLabel.Format(_T("%.1f"), dTrip);

			a_vecDrawDestNodeInfo.push_back(oDestDrawNodeInfo);

			TMapPoint oEndPoint;
			AutoType iterFind = m_mapAllNodeInfo.find(nxDestNodeID);
			if (iterFind != itEnd) {
				TFixedNodeInfo oInfo = iterFind->second;
				oEndPoint.Tx = oInfo.x;
				oEndPoint.Ty = oInfo.y;
			}
			else {
				continue;
			}

			std::vector<TMapPoint>	vecPointInfo;
			vecPointInfo.push_back(oStartPoint);
			vecPointInfo.push_back(oEndPoint);

			TUserLineInfo oLineDraw;

			oLineDraw.TColorRGB = RGB (   0, 128,   0 );;
			oLineDraw.TWidth	= 1;
			oLineDraw.TPoints	= vecPointInfo;
			oLineDraw.TLabel    = strLabel;

			a_vecDrawLine.push_back(oLineDraw);
		}
	}
	catch (...)
	{
		a_vecDrawDestNodeInfo.clear(); 
		a_vecDrawLine.clear();
		TxLogDebugException();	
	}
}


HBRUSH KTerminalScheduleEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if(ID == IDC_EDIT1)
    {   
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }

	return hbr;
}

void KTerminalScheduleEditDlg::ModelessCancleFeedback(UINT nDialogID)
{
	if (KImChampIDFind2Column::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				ClearAllMap(true);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KTerminalScheduleEditDlg::OnBnClickedCheckLabel()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
		{
			m_pMapView->TxDrawShowCompositeUserLineLablel(true);
		}
		else
		{
			m_pMapView->TxDrawShowCompositeUserLineLablel(false);
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


void KTerminalScheduleEditDlg::OnCbnSelchangeComboLabel()
{
	DrawTerminalLine();
}


void KTerminalScheduleEditDlg::OnCbnSelchangeComboType()
{
	try
	{
		m_oTerminalNodeIdCaption.TID = _T("-1");
		SetDlgItemText(IDC_EDIT1,  _T(""));

		m_ctrlReport.ResetContent();
		ClearAllMap(true);

		m_spImTasIDFind.reset();
		m_spImTasMultiIDFind.reset();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalScheduleEditDlg::OnSelEndOkOrgClr()
{
	DrawTerminalLine();
}


void KTerminalScheduleEditDlg::OnSelEndOkDestClr()
{
	DrawTerminalLine();
}
