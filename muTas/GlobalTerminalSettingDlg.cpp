// GlobalTermialSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GlobalTerminalSettingDlg.h"
#include "CustomReportRecordItem.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "BulkDBaseNode.h"
//^#include "ParseString.h"


enum
{
	_0_COLUMN_TARGETREGION = 0,
	_1_COLUMN_TRANSIT_COMPLEX,
	_2_COLUMN_INTER_REGIONAL_RAIL,
	_3_COLUMN_BUS_TERMINAL,
	_4_COLUMN_AIR_TERMINAL,
	_5_COLUMN_MARINE_PORT
};

const int     nTerminalSize			= 18;
const int	  nTerminalCharIndex	= 74;

#define UM_EXPRESSIONEDIT		WM_USER + 1001

// KGlobalTermialSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGlobalTerminalSettingDlg, KResizeDialogEx)

KGlobalTerminalSettingDlg::KGlobalTerminalSettingDlg(KTarget* a_pTargt, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGlobalTerminalSettingDlg::IDD, pParent),
	m_pTarget(a_pTargt),
	m_pMapView(a_pMapView)
{
	m_clrTransitComplex		= RGB( 34, 116,   28);
	m_clrInterRailColor		= RGB(255,   0,   0);
	m_clrBusTerminalColor   = RGB( 90, 141, 243);
	m_clrAirTerminal		= RGB(150, 112,   7);
	m_clrMarinPort			= RGB(217,  65, 197);

	KBulkDBaseNode::Nodes(m_pTarget, m_mapNodeName);
}

KGlobalTerminalSettingDlg::~KGlobalTerminalSettingDlg()
{
	if (nullptr != m_spImTasMultiIDFind)
	{
		m_spImTasMultiIDFind.reset();
	}

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pTable    = pIOTables->FindTable(TABLE_NODE);
	pTable->Unregister(this);
}

void KGlobalTerminalSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);

	DDX_Control(pDX, IDC_BUTTON_TR,              m_cpTransitComplexColor );
	DDX_Control(pDX, IDC_BUTTON_IRRS,            m_cpInterRailColor );
	DDX_Control(pDX, IDC_BUTTON_BT,              m_cpBusTerminalColor );
	DDX_Control(pDX, IDC_BUTTON_AT,              m_cpAirTerminalColor );
	DDX_Control(pDX, IDC_BUTTON_MP,              m_cpMarinPortColor );
}


BEGIN_MESSAGE_MAP(KGlobalTerminalSettingDlg, KResizeDialogEx)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,	  IDC_REPORT,	OnReportCheckItem)
	ON_BN_CLICKED(IDOK, &KGlobalTerminalSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KGlobalTerminalSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_TC, &KGlobalTerminalSettingDlg::OnBnClickedCheckTc)
	ON_BN_CLICKED(IDC_CHECK_IRRS, &KGlobalTerminalSettingDlg::OnBnClickedCheckIrrs)
	ON_BN_CLICKED(IDC_CHECK_BT, &KGlobalTerminalSettingDlg::OnBnClickedCheckBt)
	ON_BN_CLICKED(IDC_CHECK_AT, &KGlobalTerminalSettingDlg::OnBnClickedCheckAt)
	ON_BN_CLICKED(IDC_CHECK_MP, &KGlobalTerminalSettingDlg::OnBnClickedCheckMp)
	ON_BN_CLICKED(IDC_BUTTON_TR, &KGlobalTerminalSettingDlg::OnBnClickedButtonTr)
	ON_BN_CLICKED(IDC_BUTTON_IRRS, &KGlobalTerminalSettingDlg::OnBnClickedButtonIrrs)
	ON_BN_CLICKED(IDC_BUTTON_BT, &KGlobalTerminalSettingDlg::OnBnClickedButtonBt)
	ON_BN_CLICKED(IDC_BUTTON_AT, &KGlobalTerminalSettingDlg::OnBnClickedButtonAt)
	ON_BN_CLICKED(IDC_BUTTON_MP, &KGlobalTerminalSettingDlg::OnBnClickedButtonMp)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KGlobalTerminalSettingDlg::OnBnClickedCheckLabel)

	ON_CPN_XTP_SELENDOK(IDC_BUTTON_TR, &KGlobalTerminalSettingDlg::OnSelEndOkTR)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_IRRS, &KGlobalTerminalSettingDlg::OnSelEndOkIRRS)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_BT, &KGlobalTerminalSettingDlg::OnSelEndOkBT)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_AT, &KGlobalTerminalSettingDlg::OnSelEndOkAT)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_MP, &KGlobalTerminalSettingDlg::OnSelEndOkMP)
END_MESSAGE_MAP()


// KGlobalTermialSettingDlg 메시지 처리기입니다.

void KGlobalTerminalSettingDlg::ActivatedFrameWndNotifyProcess()
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
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KGlobalTerminalSettingDlg::DeActivatedFrameWndNotifyProcess()
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
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KGlobalTerminalSettingDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		ResizeComponent();

		KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

		KColorPickerSetting::DefaultCtrl(m_cpTransitComplexColor,		m_clrTransitComplex);
		KColorPickerSetting::DefaultCtrl(m_cpInterRailColor,			m_clrInterRailColor);
		KColorPickerSetting::DefaultCtrl(m_cpBusTerminalColor,			m_clrBusTerminalColor);
		KColorPickerSetting::DefaultCtrl(m_cpAirTerminalColor,			m_clrAirTerminal);
		KColorPickerSetting::DefaultCtrl(m_cpMarinPortColor,			m_clrMarinPort);

		CheckDlgButton(IDC_CHECK_TC, TRUE);
		CheckDlgButton(IDC_CHECK_IRRS, TRUE);
		CheckDlgButton(IDC_CHECK_BT, TRUE);
		CheckDlgButton(IDC_CHECK_AT, TRUE);
		CheckDlgButton(IDC_CHECK_MP, TRUE);

		InitReportHeader();
		UpdateReportData();
		MathingCode();

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_NODE);
		pTable->Register(this);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KGlobalTerminalSettingDlg::ModelessOKFeedback( UINT nDialogID )
{
    if (KImChampMultiIDFind::IDD == nDialogID)
    {
        if (m_spImTasMultiIDFind)
		{
			CString strccc = m_spImTasMultiIDFind->GetSelectedID();
			if (strccc == _T(""))
			{
				m_pItem->SetCaption(_T(""));
			}
			else
			{
				m_pItem->SetCaption(m_spImTasMultiIDFind->GetSelectedID());
			}
			m_wndReportCtrl.RedrawControl();

			ClearAllMap();
			DrawZoneTerminal();
		}
    }
}

void KGlobalTerminalSettingDlg::OnBnClickedButton1()
{
    std::vector<KIDCaption> vecIDCaption;
    std::vector<Integer>    vecID;

    KIDCaption oIDCaption;
    for (int i=0; i<300; i++)
    {
        CString strTemp(_T(""));
        strTemp.Format(_T("%d"), i);

        oIDCaption.TID = strTemp;
        oIDCaption.TCaption = strTemp; /*이름*/

        vecIDCaption.push_back(oIDCaption);
        vecID.push_back(i);
    }    

    // 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
    m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
    m_spImTasMultiIDFind->SetModelessReceiver(this);

    m_spImTasMultiIDFind->SetIDCaption(vecIDCaption);
    m_spImTasMultiIDFind->SetSelectedID(_T(""));

    // 지도 선택 옵션
    m_spImTasMultiIDFind->SetMapNodeSelectInfo(m_pMapView, vecID);
    
    m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
    m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
}

void KGlobalTerminalSettingDlg::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TARGETREGION, _T("Target Region"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("지역 구분"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_TRANSIT_COMPLEX, _T("Transit_Complex"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("복합환승센터"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_INTER_REGIONAL_RAIL, _T("Inter-Regional_Rail_Station"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("지역간철도역"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_BUS_TERMINAL, _T("Bus_Terminal"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("버스터미널"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_AIR_TERMINAL, _T("Air_Terminal"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("공항"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_MARINE_PORT, _T("Marine_Port"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("항만"));
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

void KGlobalTerminalSettingDlg::UpdateReportData( void )
{
	try
	{
		std::vector<int>	vecTargetRegionCode;
		std::map<int, TGlobalTerminalInfo> mapGlobalTerminal;
		m_wndReportCtrl.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;
		CXTPReportRecordItemControl* pButton = nullptr;

		KDBaseInterModal::GetGrolblalTerminalSettingInfo(m_pTarget, mapGlobalTerminal);

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		pTTypeCodeGroup->GetCodes(vecTargetRegionCode);

		size_t nCount = vecTargetRegionCode.size();
		for (size_t i = 0; i < nCount; i++)
		{
			int nCode = vecTargetRegionCode[i];
			
			if (nCode == 0)
			{
				continue;
			}
			
			CXTPReportRecord*		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);

			CString strCodeName = pTTypeCodeGroup->SingleCodeValue(nCode);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCodeName));
			pItem->SetItemData(nCode);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			//pItem->SetEditable(FALSE);

			AutoType find = mapGlobalTerminal.find(nCode);
			AutoType end  = mapGlobalTerminal.end();
			if (find != end)
			{
				TGlobalTerminalInfo oInfo = find->second;

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetCaption(oInfo.strTransit_Complex);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetCaption(oInfo.strInter_Region_Rail);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetCaption(oInfo.strBus_Terminal);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetCaption(oInfo.strAir_Terminal);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetCaption(oInfo.strMarine_Port);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				if(NULL != pButton)
				{
					pButton->SetAlignment(xtpItemControlUnknown);
					pButton->SetCaption(_T(">>"));
					pButton->SetSize(CSize(22, 0));
				}
			}

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

LRESULT KGlobalTerminalSettingDlg::OnExpressionEdit( WPARAM a_wParam, LPARAM a_lParam )
{
	m_pColumn = (CXTPReportColumn *)a_wParam;
	m_pItem = (CXTPReportRecordItem*)a_wParam;

	return 0;
}

void KGlobalTerminalSettingDlg::OnItemButtonClick( NMHDR * a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	m_pItem = nullptr;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if (nIndex == _1_COLUMN_TRANSIT_COMPLEX)
	{
		m_pItem = pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_TRANSIT_COMPLEX);
		size_t nCount = m_vecTransitComplexID.size();
		if (nCount < 1 )
		{
			AfxMessageBox(_T("노드 테이블에 복합환승센터 타입인 데이터가 존재하지 않습니다."));
			return;
		}
		CString strCaption = m_pItem->GetCaption();
		ClickedButton(m_vecTransitComplexID, m_vecKIDTransitComplex, strCaption);
	}
	else if (nIndex == _2_COLUMN_INTER_REGIONAL_RAIL)
	{
		m_pItem = pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_INTER_REGIONAL_RAIL);
		size_t nCount = m_vecInterRegionRailID.size();
		if (nCount < 1 )
		{
			AfxMessageBox(_T("노드 테이블에 지역간 철도역 타입인 데이터가 존재하지 않습니다."));
			return;
		}
		CString strCaption = m_pItem->GetCaption();
		ClickedButton(m_vecInterRegionRailID, m_vecKIDInterRegionRail, strCaption);
	}
	else if (nIndex == _3_COLUMN_BUS_TERMINAL)
	{
		m_pItem = pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_BUS_TERMINAL);
		size_t nCount = m_vecBusTerminalID.size();
		if (nCount < 1 )
		{
			AfxMessageBox(_T("노드 테이블에 버스터미널 타입인 데이터가 존재하지 않습니다."));
			return;
		}
		CString strCaption = m_pItem->GetCaption();
		ClickedButton(m_vecBusTerminalID, m_vecKIDBusTerminal, strCaption);
	}
	else if (nIndex == _4_COLUMN_AIR_TERMINAL)
	{
		m_pItem = pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_AIR_TERMINAL);
		size_t nCount = m_vecAirTerminalID.size();
		if (nCount < 1 )
		{
			AfxMessageBox(_T("노드 테이블에 공항 타입인 데이터가 존재하지 않습니다."));
			return;
		}
		CString strCaption = m_pItem->GetCaption();
		ClickedButton(m_vecAirTerminalID, m_vecKIDAirTerminal, strCaption);
	}
	else if (nIndex == _5_COLUMN_MARINE_PORT)
	{
		m_pItem = pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_MARINE_PORT);
		size_t nCount = m_vecMarinPortID.size();
		if (nCount < 1 )
		{
			AfxMessageBox(_T("노드 테이블에 항만 타입인 데이터가 존재하지 않습니다."));
			return;
		}
		CString strCaption = m_pItem->GetCaption();
		ClickedButton(m_vecMarinPortID, m_vecKIDMarinPort, strCaption);
	}
}

void KGlobalTerminalSettingDlg::MathingCode( void )
{
	try
	{
		std::map<int, int> mapSystemCode;
		std::map<int, int> mapMatchingSystemcode;
		std::set<Integer>  setTerminalHaveSchedule;

		m_vecKIDTransitComplex.clear();
		m_vecKIDInterRegionRail.clear();
		m_vecKIDBusTerminal.clear();
		m_vecKIDAirTerminal.clear();
		m_vecKIDMarinPort.clear();

		m_vecTransitComplexID.clear();
		m_vecInterRegionRailID.clear();
		m_vecBusTerminalID.clear();
		m_vecAirTerminalID.clear();
		m_vecMarinPortID.clear();

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup*   pTTypeCodeGroup	= pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		KDBaseInterModal::GetAllNodeTerminalInfo (m_pTarget, m_mapAllNodeTypeInfo);
		KDBaseInterModal::GetNetworkTypePairData (m_pTarget, mapSystemCode);
		KDBaseInterModal::GetTerminalHaveSchedule(m_pTarget, setTerminalHaveSchedule);

		AutoType iter = m_mapAllNodeTypeInfo.begin();
		AutoType end  = m_mapAllNodeTypeInfo.end();
		while(iter != end)
		{
			TInterTerminalNodeInfo oNodeInfo = iter->second;

			AutoType find = mapSystemCode.find(oNodeInfo.node_type);
			AutoType fend = mapSystemCode.end();
			if (find == fend)
			{
				++iter;
				continue;
			}

			AutoType itHaveSchedule = setTerminalHaveSchedule.find(oNodeInfo.node_id);
			AutoType itEndSchedule  = setTerminalHaveSchedule.end();
			if (itHaveSchedule == itEndSchedule)
			{
				++iter;
				continue;
			}

			KIDCaption oCaption;
			oCaption.TCaption = oNodeInfo.strName;

			CString strID(_T(""));
			strID.Format(_T("%I64d"), oNodeInfo.node_id);

			oCaption.TID = strID;

			int nSystemCode = find->second;
			if (TRANSITCOMPLEX == nSystemCode)
			{
				m_vecKIDTransitComplex.push_back(oCaption);
				m_vecTransitComplexID.push_back(oNodeInfo.node_id);
			}
			else if (INTER_REGIONAL_RAILSTATION == find->second)
			{
				m_vecKIDInterRegionRail.push_back(oCaption);
				m_vecInterRegionRailID.push_back(oNodeInfo.node_id);
			}
			else if (BUS_TRMINAL == find->second)
			{
				m_vecKIDBusTerminal.push_back(oCaption);
				m_vecBusTerminalID.push_back(oNodeInfo.node_id);
			}
			else if (AIR_TERMIANL == find->second)
			{
				m_vecKIDAirTerminal.push_back(oCaption);
				m_vecAirTerminalID.push_back(oNodeInfo.node_id);
			}
			else if (MARIN_PORT == find->second)
			{
				m_vecKIDMarinPort.push_back(oCaption);
				m_vecMarinPortID.push_back(oNodeInfo.node_id);
			}
			
			++iter;
		}
		int n = 1;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
	
}

void KGlobalTerminalSettingDlg::ClickedButton( std::vector<Integer> a_vecID, std::vector<KIDCaption> a_vecKIDCaption, CString a_strSelect )
{
	try
	{
		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(a_vecKIDCaption);
		m_spImTasMultiIDFind->SetSelectedID(a_strSelect);

		// 지도 선택 옵션
		m_spImTasMultiIDFind->SetMapNodeSelectInfo(m_pMapView, a_vecID);

		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
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


void KGlobalTerminalSettingDlg::OnBnClickedOk()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();
	try
	{
		KDBaseInterModal::DeleteGlobalRegionDB(spDBaseConnection);

		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CXTPReportRecordItemNumber*	 pItemNumber   = nullptr;

		pRecords			= m_wndReportCtrl.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			std::vector<Integer> vecRegionZone;
			std::vector<Integer> vecTerminalID;
			TGlobalTerminalInfo oGlobalTarminalInfo;

			pRecord		= pRecords->GetAt(i);
			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_TARGETREGION);
			int nRegionCode = pItemText->GetItemData();
			oGlobalTarminalInfo.nTargetRigion = nRegionCode;

			KDBaseInterModal::GetRegionInZoneData(spDBaseConnection, nRegionCode, vecRegionZone);

			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TRANSIT_COMPLEX);
			CString strTCCaption = pItemText->GetCaption(nullptr);
			ResultUpdateValue(strTCCaption, vecTerminalID);
			KDBaseInterModal::UpdateGlobalRegionTR2Db(spDBaseConnection, TRANSITCOMPLEX, vecRegionZone, vecTerminalID,m_mapAllNodeTypeInfo);
			oGlobalTarminalInfo.strTransit_Complex = strTCCaption;

			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_INTER_REGIONAL_RAIL);
			CString strIRRCaption = pItemText->GetCaption(nullptr);
			ResultUpdateValue(strIRRCaption, vecTerminalID);
			KDBaseInterModal::UpdateGlobalRegionTR2Db(spDBaseConnection, INTER_REGIONAL_RAILSTATION, vecRegionZone, vecTerminalID,m_mapAllNodeTypeInfo);
			oGlobalTarminalInfo.strInter_Region_Rail = strIRRCaption;

			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_3_COLUMN_BUS_TERMINAL);
			CString strBTCaption = pItemText->GetCaption(nullptr);
			ResultUpdateValue(strBTCaption, vecTerminalID);
			KDBaseInterModal::UpdateGlobalRegionTR2Db(spDBaseConnection, BUS_TRMINAL, vecRegionZone, vecTerminalID,m_mapAllNodeTypeInfo);
			oGlobalTarminalInfo.strBus_Terminal =strBTCaption;

			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_4_COLUMN_AIR_TERMINAL);
			CString strATCaption = pItemText->GetCaption(nullptr);
			ResultUpdateValue(strATCaption, vecTerminalID);
			KDBaseInterModal::UpdateGlobalRegionTR2Db(spDBaseConnection, AIR_TERMIANL, vecRegionZone, vecTerminalID,m_mapAllNodeTypeInfo);
			oGlobalTarminalInfo.strAir_Terminal = strATCaption;

			pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_5_COLUMN_MARINE_PORT);
			CString strMPCaption = pItemText->GetCaption(nullptr);
			ResultUpdateValue(strMPCaption, vecTerminalID);
			KDBaseInterModal::UpdateGlobalRegionTR2Db(spDBaseConnection, MARIN_PORT, vecRegionZone, vecTerminalID,m_mapAllNodeTypeInfo);
			oGlobalTarminalInfo.strMarine_Port = strMPCaption;

			KDBaseInterModal::SaveGloblaRegionSetting(spDBaseConnection, oGlobalTarminalInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return;
	}

	spDBaseConnection->Commit();
	AfxMessageBox(_T("적용되었습니다."));


	if (m_spImTasMultiIDFind != nullptr)
	{
		m_spImTasMultiIDFind.reset();
	}

	ClearAllMap();

	KResizeDialogEx::OnOK();
}


void KGlobalTerminalSettingDlg::OnBnClickedCancel()
{
	if (nullptr != m_spImTasMultiIDFind)
	{
		m_spImTasMultiIDFind.reset();
	}
	ClearAllMap();

	KResizeDialogEx::OnCancel();
}

void KGlobalTerminalSettingDlg::ResultUpdateValue( CString a_strCaption, std::vector<Integer>& a_vecTerminalID )
{
	try
	{
		a_vecTerminalID.clear();

		CString strSelectID = a_strCaption.Trim();
		if (strSelectID.GetLength() == 0)
			return;

		std::vector<CString> vecSelectID = KParseString::ParseString(strSelectID, _T(','));

		size_t nxCount = vecSelectID.size();
		for (size_t i=0; i<nxCount; i++)
		{
			CString strValue = vecSelectID[i];
			Integer nxValue = _ttoi64(strValue);
			a_vecTerminalID.push_back(nxValue);
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

void KGlobalTerminalSettingDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR4,				  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,				     SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);	
	SetResize(IDC_SEPRATOR3,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR5,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC6,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_TC,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_TR,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_IRRS,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_IRRS,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_BT,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BT,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_AT,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_AT,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_MP,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MP,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}

void KGlobalTerminalSettingDlg::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		CXTPReportRecordItem*        pItem         = nullptr;

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _0_COLUMN_TARGETREGION)
		{
			pItem = pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_TARGETREGION);
			if (TRUE == pItem->IsChecked())
			{
				ClearReportCheck();
				pItem->SetChecked(TRUE);
				ClearAllMap();
				DrawZoneTerminal();
			}
			else
			{
				ClearAllMap();
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

void KGlobalTerminalSettingDlg::ClearReportCheck( void )
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			if (TRUE == pRecord->GetItem(_0_COLUMN_TARGETREGION)->IsChecked())
			{
				pItem = pRecord->GetItem(_0_COLUMN_TARGETREGION);
				pItem->SetChecked(FALSE);
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

void KGlobalTerminalSettingDlg::DrawZoneTerminal()
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		std::vector<TDrawNodeInfo> vecDrawNodeInfo;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			if (TRUE == pRecord->GetItem(_0_COLUMN_TARGETREGION)->IsChecked())
			{
				pItem = pRecord->GetItem(_0_COLUMN_TARGETREGION);
				int nTRCode = pItem->GetItemData();
				DrawZoneDemacation(nTRCode);

				std::vector<Integer> vecTerminalID;
				
				if(IsDlgButtonChecked(IDC_CHECK_TC) == BST_CHECKED)
				{
					pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TRANSIT_COMPLEX);
					CString strTCCaption = pItemText->GetCaption(nullptr);
					ResultUpdateValue(strTCCaption, vecTerminalID);
					COLORREF clrTR = m_cpTransitComplexColor.GetColor();
					DrawTermianlNode(vecTerminalID, clrTR, vecDrawNodeInfo);
				}

				if(IsDlgButtonChecked(IDC_CHECK_IRRS) == BST_CHECKED)
				{
					pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_INTER_REGIONAL_RAIL);
					CString strIRRCaption = pItemText->GetCaption(nullptr);
					ResultUpdateValue(strIRRCaption, vecTerminalID);
					COLORREF clrIRR = m_cpInterRailColor.GetColor();
					DrawTermianlNode(vecTerminalID, clrIRR, vecDrawNodeInfo);
				}

				if (IsDlgButtonChecked(IDC_CHECK_BT) == BST_CHECKED)
				{
					pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_3_COLUMN_BUS_TERMINAL);
					CString strBTCaption = pItemText->GetCaption(nullptr);
					ResultUpdateValue(strBTCaption, vecTerminalID);
					COLORREF clrBT = m_cpBusTerminalColor.GetColor();
					DrawTermianlNode(vecTerminalID, clrBT, vecDrawNodeInfo);
				}

				if (IsDlgButtonChecked(IDC_CHECK_AT) == BST_CHECKED)
				{
					pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_4_COLUMN_AIR_TERMINAL);
					CString strATCaption = pItemText->GetCaption(nullptr);
					ResultUpdateValue(strATCaption, vecTerminalID);
					COLORREF clrAT = m_cpAirTerminalColor.GetColor();
					DrawTermianlNode(vecTerminalID, clrAT, vecDrawNodeInfo);
				}
				
				if (IsDlgButtonChecked(IDC_CHECK_MP) == BST_CHECKED)
				{
					pItemText	= (CXTPReportRecordItemText*)pRecord->GetItem(_5_COLUMN_MARINE_PORT);
					CString strMPCaption = pItemText->GetCaption(nullptr);
					ResultUpdateValue(strMPCaption, vecTerminalID);
					COLORREF clrMP = m_cpMarinPortColor.GetColor();
					DrawTermianlNode(vecTerminalID, clrMP, vecDrawNodeInfo);
				}
			}
		}

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, true, false);
		}
		else
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false, false);
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

void KGlobalTerminalSettingDlg::DrawZoneDemacation( int a_nTRCode )
{
	try
	{
		std::set<Integer> setZoneID;
		KDBaseInterModal::GetTargetRegionZone(m_pTarget, a_nTRCode, setZoneID);

		std::vector<TDrawZoneAccess> vecZoneAccess;
		AutoType iter = setZoneID.begin();
		AutoType end  = setZoneID.end();
		while(iter != end)
		{
			TDrawZoneAccess oDrawZoneAccess;
			oDrawZoneAccess.TNodeID = *iter;
			oDrawZoneAccess.TRadius = 0.1;
			vecZoneAccess.push_back(oDrawZoneAccess);
			++iter;
		}

		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, true);
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

void KGlobalTerminalSettingDlg::DrawTermianlNode( std::vector<Integer> a_vecTerminalID, COLORREF a_clrTerminal, std::vector<TDrawNodeInfo>& ar_vecDrawNodeInfo )
{
	try
	{

		size_t nCount = a_vecTerminalID.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TDrawNodeInfo oNodeInfo;

			Integer nxTerminalID = a_vecTerminalID[i];
			CString strTerminal(_T(""));
			strTerminal.Format(_T("%I64d"), nxTerminalID);

			CString strName(_T("-"));
			AutoType iter = m_mapNodeName.find(nxTerminalID);
			if (iter != m_mapNodeName.end())
				strName = iter->second;
			strTerminal.AppendFormat(_T("[%s]"), strName);

			oNodeInfo.TSymbol = nTerminalCharIndex;
			oNodeInfo.TColorRGB		  = a_clrTerminal;
			oNodeInfo.TLabel		  = strTerminal;
			oNodeInfo.TSize			  = nTerminalSize;
			oNodeInfo.TNodeID		  = nxTerminalID;

			ar_vecDrawNodeInfo.push_back(oNodeInfo);
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

void KGlobalTerminalSettingDlg::ClearAllMap()
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();
		m_pMapView->TxClearCompositeThemes();
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


void KGlobalTerminalSettingDlg::OnBnClickedCheckTc()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedCheckIrrs()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedCheckBt()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedCheckAt()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedCheckMp()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedButtonTr()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedButtonIrrs()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedButtonBt()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedButtonAt()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedButtonMp()
{
	ClearAllMap();
	DrawZoneTerminal();
}


void KGlobalTerminalSettingDlg::OnBnClickedCheckLabel()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::OnSelEndOkIRRS()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::OnSelEndOkTR()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::OnSelEndOkBT()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::OnSelEndOkAT()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::OnSelEndOkMP()
{
	ClearAllMap();
	DrawZoneTerminal();
}

void KGlobalTerminalSettingDlg::ModelessCancleFeedback(UINT nDialogID)
{

}

void KGlobalTerminalSettingDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			KBulkDBaseNode::Nodes(m_pTarget, m_mapNodeName);
			MathingCode();
			ClearAllMap();
			DrawZoneTerminal();
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