// AccessTermianlEditerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AccessTermianlEditerDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "CustomReportRecordItem.h"

#include "ImChampMapDefine.h"

enum
{
	_0_COLUMN_CHECK	= 0,
	_1_COLUMN_TERMINAL_ID,
	_2_COLUMN_NAME,
	_3_COLUMN_TYPE,
	_4_COLUMN_LENGTH
};


const int	  nATEZoneSize				= 15;
const int     nATStationSize			= 15;
const int	  nATEZoneCharacterIndex	= 66;
const int	  nATEStationCharIndex		= 74;

const COLORREF colATETargetColor		= RGB(255,0,0);
const COLORREF colATESelColor			= RGB(0,0,255);
const COLORREF colATEZoneColor			= RGB(0,0,0);
const COLORREF colATELineColor			= RGB(63,134,0);

// KAccessTermianlEditerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAccessTermianlEditerDlg, KResizeDialogEx)

KAccessTermianlEditerDlg::KAccessTermianlEditerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAccessTermianlEditerDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_pMapView(a_pMapView),
	m_nxZoneID(0),
	m_bSelZone(false)

{
	m_oSelectZoneIDCaption.TID = _T("-1");
}

KAccessTermianlEditerDlg::~KAccessTermianlEditerDlg()
{
}

void KAccessTermianlEditerDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO6, m_cboTerminalType);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KAccessTermianlEditerDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &KAccessTermianlEditerDlg::OnBnClickedButtonSelect)
	ON_NOTIFY(NM_DBLCLK,                  IDC_REPORT,  OnReportItemDoubleClickNode)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,	  IDC_REPORT,	OnReportCheckItemNode)
	//ON_BN_CLICKED(IDC_BUTTON_MAP, &KAccessTermianlEditerDlg::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON7, &KAccessTermianlEditerDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON6, &KAccessTermianlEditerDlg::OnBnClickedButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO6, &KAccessTermianlEditerDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KAccessTermianlEditerDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_BUTTON5, &KAccessTermianlEditerDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON4, &KAccessTermianlEditerDlg::OnBnClickedButtonSAVE)
	ON_BN_CLICKED(IDCANCEL, &KAccessTermianlEditerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KAccessTermianlEditerDlg 메시지 처리기입니다.


BOOL KAccessTermianlEditerDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{

		ResizeComponet();
		KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

		KDBaseInterModal::GetZoenNodeInfoforImTasIDFind(m_pTarget, m_vecZoneNodeIDCaption, m_mapZoneNodeData);
		MatchingCode();
		InitTypeComboBox();
		KDBaseInterModal::GetTermianlBoundary    (m_pTarget, m_mapTerminalBoundary);
		KDBaseInterModal::GetAllNodeInfo         (m_pTarget, m_mapAllNodeInfo);
		KDBaseInterModal::GetTerminalHaveSchedule(m_pTarget, m_setTerminalHaveSchedule);

		ChangeTerminalUserCodetoSystemCode();
		
		InitReportHeader();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		TxLogDebugException();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAccessTermianlEditerDlg::ModelessOKFeedback( UINT nDialogID )
{
	try
	{
		bool bRefresh(false);
		if (KImChampIDFind::IDD == nDialogID)
		{
			Integer nxID = m_spImTasIDFind->GetSelectedID();
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			CString strName(_T(""));
			AutoType iter = m_mapZoneNodeData.find(nxID);
			AutoType end  = m_mapZoneNodeData.end();
			if (iter != end)
			{
				strName = iter->second;
			}

			m_oSelectZoneIDCaption.TID	    = strID;
			m_oSelectZoneIDCaption.TCaption = strName;

			if (strName == _T(""))
			{
				strName = _T(" - ");
			}

			CString strSelectZone(_T(""));
			strSelectZone.Format(_T("%s[%s]"), strID, strName);

			m_pMapView->PanToFeature(KLayerID::Node(), nxID, bRefresh);

			SetDlgItemText(IDC_EDIT1, strSelectZone);

			bRefresh = true;
			ChangeReportControl(bRefresh);
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

void KAccessTermianlEditerDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{

}

void KAccessTermianlEditerDlg::MapSelectionFeedback(Integer a_nxNodeID)
{
	try
	{
		bool bRefresh(false);
		ClearAllMap(bRefresh);
		AddReportControl(a_nxNodeID);
		DrawZonedNodeSymbol(bRefresh);
		DrawNodeLineSymbol(bRefresh);

		bRefresh = true;
		if (bRefresh) {
			m_pMapView->MapRefresh();
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

void KAccessTermianlEditerDlg::MapSelectionNoneFeedback()
{

}

void KAccessTermianlEditerDlg::InitTypeComboBox()
{
	try
	{
		int i(0);
		int ZeroCur(0);
		AutoType iter = m_mapTerminalCode.begin();
		AutoType end  = m_mapTerminalCode.end();
		while(iter != end)
		{
			if (0 == i)
			{
				ZeroCur =m_cboTerminalType.AddString(iter->second);
				m_cboTerminalType.SetItemData(ZeroCur, iter->first);
			}
			else
			{
				int nComboIndex = m_cboTerminalType.AddString(iter->second);
				m_cboTerminalType.SetItemData(nComboIndex, iter->first);
			}
			++iter;
			i++;
		}

		m_cboTerminalType.SetCurSel(ZeroCur);
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

void KAccessTermianlEditerDlg::MatchingCode()
{
	try
	{
		std::vector<int> vecNodeType;
		std::map<int, int> mapSystemCode;
		std::map<int, CString> mapTerminalCode;

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		CString strTerminalName = pTTypeCodeGroup->SingleCodeValue(TRANSITCOMPLEX);
		mapTerminalCode.insert(std::make_pair(TRANSITCOMPLEX, strTerminalName));

		strTerminalName = pTTypeCodeGroup->SingleCodeValue(INTER_REGIONAL_RAILSTATION);
		mapTerminalCode.insert(std::make_pair(INTER_REGIONAL_RAILSTATION, strTerminalName));

		strTerminalName = pTTypeCodeGroup->SingleCodeValue(BUS_TRMINAL);
		mapTerminalCode.insert(std::make_pair(BUS_TRMINAL, strTerminalName));

		strTerminalName = pTTypeCodeGroup->SingleCodeValue(AIR_TERMIANL);
		mapTerminalCode.insert(std::make_pair(AIR_TERMIANL, strTerminalName));

		strTerminalName = pTTypeCodeGroup->SingleCodeValue(MARIN_PORT);
		mapTerminalCode.insert(std::make_pair(MARIN_PORT, strTerminalName));

		KDBaseInterModal::GetNetworkTypePairData(m_pTarget, mapSystemCode);
		KDBaseInterModal::GetUsingNodeType(m_pTarget, vecNodeType);

		size_t nCount = vecNodeType.size();
		for (size_t i = 0; i < nCount; i++)
		{
			int nUsingCode = vecNodeType[i];
			AutoType FindPair = mapSystemCode.find(nUsingCode);
			AutoType endPair  = mapSystemCode.end();
			if (FindPair != endPair)
			{
				int nSystemCode = FindPair->second;
				AutoType FindTerminal = mapTerminalCode.find(nSystemCode);
				AutoType EndTermaial  = mapTerminalCode.end();

				if (FindTerminal != EndTermaial)
				{
					m_mapTerminalCode.insert(std::make_pair(FindTerminal->first, FindTerminal->second));
					m_mapMatingCode.insert(std::make_pair(nUsingCode, nSystemCode));
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

void KAccessTermianlEditerDlg::SelectZoneList()
{
	try
	{
		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);
		m_spImTasIDFind->SetSelectedID(-1);
		m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
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

void KAccessTermianlEditerDlg::SetZoneNodeViewData( CString a_strZone_ID, CString& a_strNodeViewData )
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZone_ID);

		AutoType iter  = m_mapZoneNodeData.find(nxZoneID);
		AutoType itEnd = m_mapZoneNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		m_oSelectZoneIDCaption.TID      = a_strZone_ID;
		m_oSelectZoneIDCaption.TCaption = strName;

		if (TRUE == strName.IsEmpty())
		{
			strName = _T(" - ");
		}
		a_strNodeViewData.Format(_T("%s [%s]"), m_oSelectZoneIDCaption.TID, strName);

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


void KAccessTermianlEditerDlg::OnBnClickedButtonSelect()
{
	try
	{
		SelectZoneList();
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

void KAccessTermianlEditerDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK, _T("Select"), 15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_TERMINAL_ID, _T("Terminal_ID"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 노드 ID"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Name"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 이름"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_TYPE, _T("Type"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 타입"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_LENGTH, _T("Distance"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("거리"));
    }
}


BOOL KAccessTermianlEditerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			SelectZoneList();
			return TRUE;
		}		
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}

void KAccessTermianlEditerDlg::ChangeReportControl( bool a_bRefresh )
{
	try
	{
		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		m_nxZoneID    = _ttoi64(m_oSelectZoneIDCaption.TID);

		KDBaseInterModal::GetTerminalDataInZone(m_pTarget, m_nxZoneID, nTerminalType, m_mapTermianlNodeInfo);

		UpdateReportData();

		DrawZonedNodeSymbol(false);

		DrawNodeLineSymbol(false);
		
		if (a_bRefresh) {
			m_pMapView->MapRefresh();
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

void KAccessTermianlEditerDlg::UpdateReportData()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = m_mapTermianlNodeInfo.begin();
		AutoType end  = m_mapTermianlNodeInfo.end();

		while(iter != end)
		{
			TAccessTerminalNodeInfo oInfo = iter->second;

			CXTPReportRecord*		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);

			Integer nxTerminalID = iter->first;
			CString strTerminalID(_T(""));
			strTerminalID.Format(_T("%I64d"), nxTerminalID);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTerminalID));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);


			CString strNodeName(_T(""));
			AutoType iterNode = m_mapAllNodeInfo.find(nxTerminalID);
			AutoType endNode  = m_mapAllNodeInfo.end();
			if (iterNode != endNode)
			{
				TInterNodeInNameInfo oInfo = iterNode->second;
				strNodeName = oInfo.name;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeName));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			CString strType = _T(" - ");
			AutoType FindType = m_mapTerminalCode.find(oInfo.nType);
			AutoType EndType  = m_mapTerminalCode.end();
			if (FindType != EndType)
			{
				strType = FindType->second;
			}
			
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strType));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dLength, _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			++iter;
		}

		m_wndReportCtrl.Populate();

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

void KAccessTermianlEditerDlg::DrawZonedNodeSymbol( bool a_bRefresh )
{
	try
	{
		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		std::vector<TDrawZoneAccess> vecZoneAccess;
		TDrawZoneAccess oDrawZoneAccess;
		oDrawZoneAccess.TNodeID			= m_nxZoneID;
		oDrawZoneAccess.TSymbol = nATEZoneCharacterIndex;
		oDrawZoneAccess.TSize			= nATEZoneSize;
		oDrawZoneAccess.TColorRGB		= colATESelColor;

		double dBoundary = 1;
		AutoType iter = m_mapTerminalBoundary.find(nTerminalType);
		AutoType end  = m_mapTerminalBoundary.end();
		if (iter != end)
		{
			dBoundary = iter->second;
		}

		oDrawZoneAccess.TRadius = dBoundary;

		vecZoneAccess.push_back(oDrawZoneAccess);

		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
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

void KAccessTermianlEditerDlg::ChangeTerminalUserCodetoSystemCode( void )
{
	m_mapTerminalSystemCode.clear();

	AutoType iter = m_mapAllNodeInfo.begin();
	AutoType end  = m_mapAllNodeInfo.end();

	while(iter!= end)
	{
		TInterNodeInNameInfo oInfo = iter->second;
		AutoType FindType = m_mapMatingCode.find(oInfo.node_type);
		AutoType EndType  = m_mapMatingCode.end();
		if (FindType == EndType)
		{
			++iter;
			continue;
		}

		AutoType itHaveSchedule = m_setTerminalHaveSchedule.find(oInfo.node_id);
		AutoType itEndSchedule  = m_setTerminalHaveSchedule.end();
		if (itHaveSchedule == itEndSchedule)
		{
			++iter;
			continue;
		}

		m_mapTerminalSystemCode.insert(std::make_pair(oInfo.node_id, FindType->second));

		++iter;
	}
}

void KAccessTermianlEditerDlg::DrawNodeLineSymbol( bool a_bRefresh )
{
	try
	{
		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		std::set<Integer> setUseTerminalID;
		GetReportControlValue(setUseTerminalID);
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		std::vector<TUserLineInfo> vecDrawLineInfo;
		//Zone 설정

		Integer nxZoneID = _ttoi64(m_oSelectZoneIDCaption.TID);

		TDrawNodeInfo oZoneInfo;
		oZoneInfo.TSymbol = nATEZoneCharacterIndex;
		oZoneInfo.TColorRGB		  = colATEZoneColor;
		oZoneInfo.TNodeID		  = nxZoneID;
		oZoneInfo.TLabel		  = m_oSelectZoneIDCaption.TID;
		oZoneInfo.TSize			  = nATEZoneSize;
		vecDrawNodeInfo.push_back(oZoneInfo);

		TMapPoint oStartPoint;
		AutoType findzone = m_mapAllNodeInfo.find(nxZoneID);
		AutoType end  = m_mapAllNodeInfo.end();
		if (findzone != end)
		{
			TInterNodeInNameInfo oInfo = findzone->second;
			oStartPoint.Tx = oInfo.x;
			oStartPoint.Ty = oInfo.y;
		}


		//Station설정
		AutoType iterTNode = m_mapTerminalSystemCode.begin();
		AutoType endTNode  = m_mapTerminalSystemCode.end();
		while(iterTNode != endTNode)
		{
			TDrawNodeInfo oNodeStation;

			if (iterTNode->second != nTerminalType)
			{
				iterTNode++;
				continue;
			}

			AutoType iterUseT = setUseTerminalID.find(iterTNode->first);
			AutoType endUseT  = setUseTerminalID.end();

			if (iterUseT != endUseT)
			{
				oNodeStation.TNodeID			= iterTNode->first;
				CString strLable(_T(""));
				strLable.Format(_T("%I64d"), oNodeStation.TNodeID);
				oNodeStation.TLabel				= strLable;
				oNodeStation.TSymbol	= nATEStationCharIndex;
				oNodeStation.TColorRGB			= colATETargetColor;
				oNodeStation.TSize				= nATEZoneSize;
				vecDrawNodeInfo.push_back(oNodeStation);

				TMapPoint oEndPoint;
				AutoType findnode = m_mapAllNodeInfo.find(iterTNode->first);
				if (findnode!= end)
				{
					TInterNodeInNameInfo oInfo = findnode->second;
					oEndPoint.Tx = oInfo.x;
					oEndPoint.Ty = oInfo.y;
				}

				TUserLineInfo oLineDraw;
				std::vector<TMapPoint> vecPointInfo;
				vecPointInfo.push_back(oStartPoint);
				vecPointInfo.push_back(oEndPoint);

				oLineDraw.TColorRGB = colATELineColor;
				oLineDraw.TLabel    = _T("");
				oLineDraw.TPoints	= vecPointInfo;
				oLineDraw.TWidth	= 3;

				vecDrawLineInfo.push_back(oLineDraw);
			}
			iterTNode++;
		}

		bool bShowLabel(false);
		if ( IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			bShowLabel = true;
		}

		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, bShowLabel, a_bRefresh);
		m_pMapView->TxDrawCompositeUserLine(vecDrawLineInfo, bShowLabel, a_bRefresh); 
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

void KAccessTermianlEditerDlg::GetReportControlValue( std::set<Integer>& a_setValue )
{
	try
	{
		a_setValue.clear();

		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		pRecords	= m_wndReportCtrl.GetRecords();
		int nRecord = pRecords->GetCount();

		for (int i = 0; i < nRecord; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TERMINAL_ID);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);

			a_setValue.insert(nxNodeID);
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

void KAccessTermianlEditerDlg::ClearAllMap( bool a_bRefresh )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit(true);
		m_pMapView->TxClearCompositeThemes(a_bRefresh);
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


void KAccessTermianlEditerDlg::OnBnClickedButtonAdd()
{
	try
	{
		bool bRefresh(false);
		ClearAllMap(bRefresh);
		bRefresh = true;
		SelectTerminalData(bRefresh);
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


void KAccessTermianlEditerDlg::OnBnClickedButtonDel()
{
	try
	{
		bool bRefresh(false);
		std::vector<CXTPReportRecord*> vecRecrod;
		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pExtItemCheck = nullptr;

		pRecords			= m_wndReportCtrl.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_CHECK);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_wndReportCtrl.GetRecords()->RemoveRecord(pRecord);
			m_wndReportCtrl.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("삭제 가능한 데이터가 존재하지 않습니다."));
		}

		ClearAllMap(bRefresh);
		DrawZonedNodeSymbol(bRefresh);
		DrawNodeLineSymbol(bRefresh);

		bRefresh = true;
		if (bRefresh) {
			m_pMapView->MapRefresh();
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

void KAccessTermianlEditerDlg::SelectTerminalData( bool a_bRefresh )
{
	try
	{
		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		std::set<Integer> setUseTerminalID;
		GetReportControlValue(setUseTerminalID);

		std::vector<TDrawNodeInfo> vecTDrawNodeInfo;
		std::vector<TNodeSelectionInfo>  vecTNodeTerminal;

		Integer nxZoneID = _ttoi64(m_oSelectZoneIDCaption.TID);

		TDrawNodeInfo oZoneInfo;
		oZoneInfo.TSymbol = nATEZoneCharacterIndex;
		oZoneInfo.TColorRGB		  = colATEZoneColor;
		oZoneInfo.TNodeID		  = nxZoneID;
		oZoneInfo.TLabel		  = m_oSelectZoneIDCaption.TID;
		oZoneInfo.TSize			  = nATEZoneSize;
		vecTDrawNodeInfo.push_back(oZoneInfo);

		TPreferenceSymbol &oNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		AutoType iterTNode = m_mapTerminalSystemCode.begin();
		AutoType endTNode  = m_mapTerminalSystemCode.end();
		while(iterTNode != endTNode)
		{
			
			if (iterTNode->second != nTerminalType)
			{
				iterTNode++;
				continue;
			}

			AutoType iterUseT = setUseTerminalID.find(iterTNode->first);
			AutoType endUseT  = setUseTerminalID.end();

			if (iterUseT != endUseT)
			{
				TDrawNodeInfo oNodeStation;
				oNodeStation.TNodeID			= iterTNode->first;
				oNodeStation.TSymbol	= nATEStationCharIndex;
				oNodeStation.TColorRGB			= colATETargetColor;
				oNodeStation.TSize				= nATEZoneSize;
				
				CString strTerminalID = _T("");
				strTerminalID.Format(_T("%I64d"), iterTNode->first);
				oNodeStation.TLabel				= strTerminalID; 
				vecTDrawNodeInfo.push_back(oNodeStation);
			}
			else
			{
				TNodeSelectionInfo oNodeStation;
				oNodeStation.TNodeID = iterTNode->first;
				oNodeStation.TType	 = 0;
				oNodeStation.TCharacterIndex = oNodeSymbol.nCharIndex;
				oNodeStation.TColorRGB		 = oNodeSymbol.clrSymbol;
				oNodeStation.TSize           = oNodeSymbol.dSize;

				CString strZoneID(_T(""));
				strZoneID.Format(_T("%I64d"), iterTNode->first);
				oNodeStation.TLabel = strZoneID;
				vecTNodeTerminal.push_back(oNodeStation);

			}
			iterTNode++;
		}

		bool bShowLabel(false);
		if ( IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			bShowLabel = true;
		}

		m_pMapView->MapNodeSelectAction(this, vecTNodeTerminal, bShowLabel, true, 5000000);
		m_pMapView->TxDrawCompositeNode(vecTDrawNodeInfo, bShowLabel, false);
		DrawZonedNodeSymbol(false);
		if (a_bRefresh) {
			m_pMapView->MapRefresh();
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

void KAccessTermianlEditerDlg::AddReportControl(Integer a_nxTermianlNode)
{
	try
	{
		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();

		CXTPReportRecord*		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		CString strNodeID(_T(""));
		strNodeID.Format(_T("%I64d"), a_nxTermianlNode);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeID));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		CString strNodeName(_T(""));
		AutoType iterNode = m_mapAllNodeInfo.find(a_nxTermianlNode);
		AutoType endNode  = m_mapAllNodeInfo.end();
		if (iterNode != endNode)
		{
			strNodeName = iterNode->second.name;
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeName));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);


		AutoType iter = m_mapTerminalCode.find(nTerminalType);
		AutoType end  = m_mapTerminalCode.end();

		CString strTerminalName = _T("");
		if (iter != end)
		{
			strTerminalName = iter->second;
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTerminalName));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		AutoType FindZoneNode = m_mapAllNodeInfo.find(m_nxZoneID);
		AutoType FindTerminal = m_mapAllNodeInfo.find(a_nxTermianlNode);
		AutoType FindEnd	  = m_mapAllNodeInfo.end();

		double x1(0.0);
		double x2(0.0);
		double y1(0.0);
		double y2(0.0);

		if (FindZoneNode != FindEnd)
		{
			TInterNodeInNameInfo oInfo = FindZoneNode->second;
			x1 = oInfo.x;
			y1 = oInfo.y;
		}

		if (FindTerminal != FindEnd)
		{
			TInterNodeInNameInfo oInfo = FindTerminal->second;
			x2 = oInfo.x;
			y2 = oInfo.y;
		}

		double dist = sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));
		dist = dist / 1000;

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dist, _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		m_wndReportCtrl.Populate();
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


void KAccessTermianlEditerDlg::OnCbnSelchangeComboType()
{
	try
	{
		Integer nxID = _ttoi64(m_oSelectZoneIDCaption.TID);

		if (nxID < 1) {
			return;
		} else {
			bool bRefresh(false);
			ClearAllMap(bRefresh);
			bRefresh = true;
			ChangeReportControl(bRefresh);
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


void KAccessTermianlEditerDlg::OnBnClickedCheckLabel()
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


void KAccessTermianlEditerDlg::OnBnClickedButtonClear()
{
	try
	{
		ClearAllMap(true);
		m_wndReportCtrl.ResetContent(TRUE);
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


void KAccessTermianlEditerDlg::OnBnClickedButtonSAVE()
{
	if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	try
	{
		std::vector<TAccessTerminalNodeInfo> vecTerminalNodeInfo;

		int	nCurSel			=  m_cboTerminalType.GetCurSel();
		int nTerminalType	=  m_cboTerminalType.GetItemData(nCurSel);

		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CXTPReportRecordItemNumber*	 pItemNumber   = nullptr;

		pRecords			= m_wndReportCtrl.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TAccessTerminalNodeInfo oTNodeInfo;

			pRecord		= pRecords->GetAt(i);
			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TERMINAL_ID);
			CString strTerminalID = pItemText->GetValue();
			Integer nxTerminalID = _ttoi64(strTerminalID);

			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_LENGTH);
			double dLength = pItemNumber->GetValue();

			oTNodeInfo.nxZoneID		= m_nxZoneID;
			oTNodeInfo.nType		= nTerminalType;
			oTNodeInfo.nxTerminalID = nxTerminalID;
			oTNodeInfo.dLength		= dLength;

			vecTerminalNodeInfo.push_back(oTNodeInfo);
		}

		KDBaseInterModal::SaveAccessTerminalEditor(m_pTarget, m_nxZoneID, nTerminalType, vecTerminalNodeInfo);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}

	KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ACCESS_TERMINAL_NODE);
	pTable->Notify();

	AfxMessageBox(_T("변경사항이 저장되었습니다."));

}


void KAccessTermianlEditerDlg::OnBnClickedCancel()
{
	ClearAllMap(true);
	KResizeDialogEx::OnCancel();
}

void KAccessTermianlEditerDlg::ResizeComponet()
{
	SetResize(IDC_COMBO6,					  SZ_TOP_LEFT,  SZ_TOP_LEFT);	
	SetResize(IDC_SEPRATOR4,				  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,				      SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);		
	
	SetResize(IDC_BUTTON7,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);		
	SetResize(IDC_BUTTON6,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,			      SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON5,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR3,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON4,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}

void KAccessTermianlEditerDlg::OnReportItemDoubleClickNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		Integer nxNodeID(0);
		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 1)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
			CString strTerminalID = pItem->GetValue();
			nxNodeID = _ttoi64(strTerminalID);
		}

		m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
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

void KAccessTermianlEditerDlg::OnReportCheckItemNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 0)
		{
			if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(0)->IsChecked() )
			{
				CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
				CString strNodeID = pItemText->GetValue();
				Integer nxNodeID = _ttoi64(strNodeID);
				m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
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

void KAccessTermianlEditerDlg::ModelessCancleFeedback(UINT nDialogID)
{
	
}
