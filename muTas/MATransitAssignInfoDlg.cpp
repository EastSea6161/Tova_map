// MATransitAssignInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MATransitAssignInfoDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MultiModalInfo.h"
#include "KExRecordItem.h"

#include "DBaseDefaultParaTransitRouteChoice.h"
#include "DBaseDefaultParaMultimodalModeChoice.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"

#include "MultiModalMainDlg.h"

enum
{
	_0_ACOLUMN_FUNCTION = 0,
	_1_ACOLUMN_BUS      = 1,
	_2_ACOLUMN_SUBWAY   = 2,
	_3_ACOLUMN_BUSSUBWAY= 3,
	_4_ACOLUMN_UPPER	= 4
};

enum
{
	_0_BCOLUMN_MODE             = 0,
	_1_BCOLUMN_TYPE             = 1,
	_2_BCOLUMN_BASE_COST        = 2,
	_3_BCOLUMN_BASE_DISTANCE    = 3,
	_4_BCOLUMN_ADD_COST         = 4,
	_5_BCOLUMN_ADD_DISTANCE     = 5,
	_6_BCOLUMN_CHECK_INTERGRATE = 6

};

enum
{
	_0_ROW_IN_VEHTICLE_TIME		= 0,
	_1_ROW_WAITING_TIME         = 1,
	_2_ROW_TRANSFER_TIME	    = 2,
	_3_ROW_ACCESSWALKTIME		= 3,
	_4_ROW_EGRESSWALKTIME		= 4,
	_5_ROW_NOOFTRANSFER		    = 5,
	_6_ROW_COST					= 6,
	_7_ROW_NOOFSTOP				= 7

};

enum
{
	FUNCTION_CASE_BUS    = 1,
	FUNCTION_CASE_SUBWAY = 2
};

const int TRANSIT_TYPE_BUS			= 3;
const int TRANSIT_TYPE_SUBWAY		= 4;
const int TRANSIT_TYPE_BUS_SUBWAY	= 5;
const int TRANSTI_TYPE_UPPERLEVER   = 9;

// KMATransitAssignInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMATransitAssignInfoDlg, KResizeDialogEx)

KMATransitAssignInfoDlg::KMATransitAssignInfoDlg(KTarget* a_pTarget,std::list<KMultiModalInfo>a_lstMultiInfo, std::vector<TMAFnCoefficient> a_vecCofficient, std::vector<TMATransitCostInfo> a_vecCost, TMATransitOptionsBaseInfo& a_oBaseInfo, int a_nMaxRoute, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMATransitAssignInfoDlg::IDD, pParent)
	,m_pTarget(a_pTarget)
	,m_lstMultiInfo(a_lstMultiInfo)
	,m_vecFnCofficient(a_vecCofficient)
	,m_vecTransitCost(a_vecCost)
	,m_oOptionBaseInfo(a_oBaseInfo)
	,m_bBus(false)
	,m_bSubway(false)
	,m_bBusSubway(false)
	,m_bExclusive(true)
	,m_nMaxNo(a_nMaxRoute)
{

}

KMATransitAssignInfoDlg::~KMATransitAssignInfoDlg()
{
}

void KMATransitAssignInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KMATransitAssignInfoDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KMATransitAssignInfoDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDOK, &KMATransitAssignInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &KMATransitAssignInfoDlg::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


// KMATransitAssignInfoDlg 메시지 처리기입니다.


BOOL KMATransitAssignInfoDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	std::list<KMultiModalInfo>::iterator iter = m_lstMultiInfo.begin();
	std::list<KMultiModalInfo>::iterator end  = m_lstMultiInfo.end();

	while (iter != end)
	{
		KMultiModalInfo& oInfo = *iter;
		
        int nType = oInfo.TransitHighwayType();
		if (nType == 1) {
			m_bBus = true;
		} 
		else if (nType == 2) {
			m_bSubway = true;
		}
		else if (nType == 3) {
			m_bBusSubway = true;
		}

        //★ 전용모드 또는 독립모드란 ?
        //★ 모든 수단 OD가 버스와 레일 구분이 없는 경우는 : Transit Mode(일반적인경우) -> m_bExclusive = false
        //★ 사용자가 선택한 수단 중 특정 수단중 버스 Transit-Line 네트워크 또는 레일 Transit-Line 네트워크에 Assign 되어야 할 경우
        if (nType != 0) {
            if(0 == oInfo.Exclusive()) {
                m_bExclusive = false;
            }
        }

		++iter;
	}

	KReportCtrlSetting::Default(m_wndReportACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndReportBCtrl, TRUE, FALSE, FALSE);

	ResizeComponent();
	InitEditButtion();
	InitReportCoefficientHeader();
	InitReportCostHeader();

	UpdateReportCoefficient();
	UpdateReportCost();

	return TRUE; 
}


void KMATransitAssignInfoDlg::InitReportCoefficientHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(0, _T("Coefficient"), 30)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }	

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("설명변수"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(1, _T("Highway"), 34)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }	

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스 경로"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(2, _T("Rail"), 34)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시철도 경로"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(3, _T("Composite"), 37)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스+도시철도 경로"));
    }
   
    pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(4, _T("Highway(Upper)"), 34)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스 수단"));
    }

    pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(5, _T("Rail(Upper)"), 34)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시철도 수단"));
    }

    pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(6, _T("Composite(Upper)"), 37)); {
        pColumn->SetHeaderAlignment(DT_CENTER);
    }
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스+도시철도 수단"));
    }
}


void KMATransitAssignInfoDlg::InitReportCostHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_BCOLUMN_MODE, _T("Mode"),                25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입명"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_BCOLUMN_TYPE, _T("Type"),                25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입 명칭"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_BCOLUMN_BASE_COST, _T("Base Fare"),        15));
	pColumn->SetTooltip(_T("Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기본운임"));
		pColumn->SetTooltip(_T("기본운임"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_BCOLUMN_BASE_DISTANCE, _T("Base Dist"),    15));
	pColumn->SetTooltip(_T("Base Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("~Km까지"));
		pColumn->SetTooltip(_T("~Km까지"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_BCOLUMN_ADD_COST, _T("Dist-Base Fare"),         15));
	pColumn->SetTooltip(_T("Dist-Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("추가운임"));
		pColumn->SetTooltip(_T("추가운임"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_5_BCOLUMN_ADD_DISTANCE, _T("Uint Dist"),     15));
	pColumn->SetTooltip(_T("Uint Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("매 ~Km마다"));
		pColumn->SetTooltip(_T("매 ~Km마다"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_6_BCOLUMN_CHECK_INTERGRATE, _T("D.F.P"), 15));
	pColumn->SetTooltip(_T("Discount Fare Policy"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통합환승할인"));
		pColumn->SetTooltip(_T("통합환승활인제 적용"));
    }
}


void KMATransitAssignInfoDlg::UpdateReportCoefficient( void )
{
    int nTxCnt = (int)m_vecFnCofficient.size();
    if (nTxCnt <= 0)
        return;

    m_wndReportACtrl.ResetContent();

    CXTPReportRecord*     pRecord = nullptr;
    CXTPReportRecordItem* pItem   = nullptr;

    //★ In-Vehicle Time
    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("In-Vehicle Time"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("차내통행시간"));
    }
    
    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdInVehcle;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    //★ Waiting Time
    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Waiting Time"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("대기시간"));
    }

    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdWaitingTime;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Transfer Time"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("환승시간"));
    }

    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdTransferTime;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord()); 
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Access Walk Time"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("출발 접근시간"));
    }
    
    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdAccessWalkTime;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Egress Walk Time"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("도착 접근시간"));
    }

    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdEgressWalkTime;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Transfers"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("환승횟수"));
    }

    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdNoOfTransfer;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Cost"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("통행비용"));
    }
    
    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdCost;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord()); 
    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Stop At Zone"))); {
        pItem->SetAlignment(xtpColumnTextCenter);
        pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
        pItem->SetEditable(FALSE);
    }
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("역/정류장 개수"));
    }

    for (int i=0; i<nTxCnt; i++) {
        TMAFnCoefficient& oInfo = m_vecFnCofficient[i];
        double dValue(0); {
            dValue = oInfo.TdNoOfStop;
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.10f")));
        pItem->SetAlignment(xtpColumnTextRight);

        if (m_bExclusive && i >= 3) {
            pItem->SetCaption(_T("-"));
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
        }
    }

    m_wndReportACtrl.Populate();
}


void KMATransitAssignInfoDlg::UpdateReportCost( void )
{
	m_wndReportBCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (size_t i= 0; i< m_vecTransitCost.size(); i++)
	{
		TMATransitCostInfo &oMATransitPathCost = m_vecTransitCost[i];

		CString strDetailName = oMATransitPathCost.TstrTypeName;
		int     nDetailTyep	  = oMATransitPathCost.TnType;
		m_mapTransitName.insert(std::make_pair(nDetailTyep, strDetailName));

		CString strTransitName(_T(""));
		if ( TRANSIT_TYPE_BUS == oMATransitPathCost.TnMode )
		{
			
			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());
			if (m_bBus == false && m_bBusSubway == false)
			{
				pRecord->SetVisible(FALSE);
			}
			strTransitName = _T("Bus_Transit");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitName = _T("버스 타입");
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTransitName));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetItemData(oMATransitPathCost.TnMode);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strDetailTransit(_T(""));
			strDetailTransit.Format(_T("%d : %s"), oMATransitPathCost.TnType, oMATransitPathCost.TstrTypeName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDetailTransit));;
			pItem->SetAlignment(xtpColumnTextLeft);
			pItem->SetItemData(oMATransitPathCost.TnType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdBasicRate,    _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdBasicDistance,  _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdAddRate,     _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdAddDistance,		_T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			if( 1 == oMATransitPathCost.TnMergeRate)
				pItem->SetChecked(TRUE);
			else
				pItem->SetChecked(FALSE);

			
		}
		else if( TRANSIT_TYPE_SUBWAY == oMATransitPathCost.TnMode)
		{
		
			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());
			if (m_bSubway == false && m_bBusSubway == false)
			{
				pRecord->SetVisible(FALSE);
			}
			strTransitName = _T("Rail_Transit");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitName = _T("철도 타입");
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTransitName));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetItemData(oMATransitPathCost.TnMode);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strDetailTransit(_T(""));
			strDetailTransit.Format(_T("%d : %s"), oMATransitPathCost.TnType, oMATransitPathCost.TstrTypeName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDetailTransit));;
			pItem->SetAlignment(xtpColumnTextLeft);
			pItem->SetItemData(oMATransitPathCost.TnType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);


			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdBasicRate,    _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdBasicDistance,  _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdAddRate,     _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMATransitPathCost.TdAddDistance,		_T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			if( 1 == oMATransitPathCost.TnMergeRate)
				pItem->SetChecked(TRUE);
			else
				pItem->SetChecked(FALSE);
			
		}
	}

	m_wndReportBCtrl.Populate();
}


void KMATransitAssignInfoDlg::InitEditButtion( void )
{
	CString strMaxNo;
	strMaxNo.Format(_T("%d"), m_oOptionBaseInfo.TnMaxofNo);
	CButton* pMaxButton = (CButton*)GetDlgItem(IDC_EDIT_MAXPATH);
	pMaxButton->SetWindowTextW(strMaxNo);

	CString strBATime;
	strBATime.Format(_T("%.1f"), m_oOptionBaseInfo.TdDWTimeInBus);
	CButton* pTimeButton = (CButton*)GetDlgItem(IDC_EDIT_TIME);
	pTimeButton->SetWindowTextW(strBATime);

	CString strAlpha;
	strAlpha.Format(_T("%.2f"), m_oOptionBaseInfo.TdAlpha);
	CButton* pALPHAButton = (CButton*)GetDlgItem(IDC_EDIT_ALPHA);
	pALPHAButton->SetWindowTextW(strAlpha);

	CString strBeta;
	strBeta.Format(_T("%.2f"), m_oOptionBaseInfo.TdBeta);
	CButton* pBETAButton = (CButton*)GetDlgItem(IDC_EDIT_BETA);
	pBETAButton->SetWindowTextW(strBeta);

	CString strDWell;
	strDWell.Format(_T("%.2f"), m_oOptionBaseInfo.TdDWTimeInRail);
	CButton* pDWellButton = (CButton*)GetDlgItem(IDC_EDIT_DWTIME);
	pDWellButton->SetWindowTextW(strDWell);

	CString strMEU;
	strMEU.Format(_T("%.7f"), m_oOptionBaseInfo.TdMEU);
	CButton* pMEUButton = (CButton*)GetDlgItem(IDC_EDIT_MEU);
	pMEUButton->SetWindowTextW(strMEU);

	if (m_bExclusive == true)
	{
		pMEUButton->EnableWindow(FALSE);
	}
}


void KMATransitAssignInfoDlg::ResizeComponent()
{
	SetResize(IDC_EDIT_MAXPATH,				  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_TIME,				  SZ_TOP_LEFT, SZ_TOP_LEFT);	
	SetResize(IDC_EDIT_ALPHA,				  SZ_TOP_LEFT, SZ_TOP_LEFT);	
	SetResize(IDC_EDIT_BETA,				  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_DWTIME,				  SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC1,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC7,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC8,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC9,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC10,					  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC11,					  SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_REPORT,				      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT2,				      SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);	
	
	SetResize(IDC_SEPRATOR9,				  SZ_TOP_LEFT,  SZ_TOP_RIGHT);	
	SetResize(IDC_SEPRATOR10,				  SZ_TOP_LEFT,  SZ_TOP_RIGHT);	
	SetResize(IDC_SEPRATOR11,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_BUTTON_DEFAULT,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KMATransitAssignInfoDlg::OnBnClickedClose()
{
	KResizeDialogEx::OnCancel();
}


void KMATransitAssignInfoDlg::GetCoefficientValue( void )
{
	CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
	CXTPReportRecord*  pRecord  = nullptr;
	CXTPReportRecordItemNumber* pItem= nullptr;

    //★ 오류 체크를 하지 않는다. 오류가 나면 죽는것이 좋다.

    int nTxCnt = m_vecFnCofficient.size();
    int nRow(-1);

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdInVehcle = dValue;
        }
    }
	
    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdWaitingTime = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdTransferTime = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdAccessWalkTime = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdEgressWalkTime = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdNoOfTransfer = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdCost = dValue;
        }
    }

    pRecord = pRecords->GetAt(++nRow);
    for (int i=0; i<nTxCnt; i++) {
        pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(i + 1);
        double dValue(0); {
            dValue = pItem->GetValue();
        }

        TMAFnCoefficient& oInfo = m_vecFnCofficient[i]; {
            oInfo.TdNoOfStop = dValue;
        }
    }
}


void KMATransitAssignInfoDlg::GetTransitCostValue( void )
{
	CXTPReportRecords*          pRecords = m_wndReportBCtrl.GetRecords();
	CXTPReportRecord*           pRecord  = nullptr;
	CXTPReportRecordItemNumber* pItemNum = nullptr;
	CXTPReportRecordItem*        pItem         = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;

	m_vecTransitCost.clear();

	int nRecordCount   = pRecords->GetCount();
	for (int i= 0; i< nRecordCount; i++)
	{
		TMATransitCostInfo oTransitCostInfo;

		pRecord			           = pRecords->GetAt(i);
		pItem                      = pRecord->GetItem(_0_BCOLUMN_MODE);
		int     nTransitType       = (int)pItem->GetItemData();
		
		pItem                      = pRecord->GetItem(_1_BCOLUMN_TYPE);
		int nDetailTransitType     = (int)pItem->GetItemData();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_BCOLUMN_BASE_COST);
		double dBaseCost           =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_BCOLUMN_BASE_DISTANCE);
		double dBaseLength         =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_BCOLUMN_ADD_COST);
		double dAddCost            =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_BCOLUMN_ADD_DISTANCE);
		double dAddLength          =  pItemNum->GetValue();


		pExtItemCheck    = pRecord->GetItem(_6_BCOLUMN_CHECK_INTERGRATE);
		int nCostMethod  = 0;
		if( TRUE == pExtItemCheck->IsChecked() )
		{
			nCostMethod = 1;
		}
		else
		{
			nCostMethod = 0;
		}

		oTransitCostInfo.TnMode				= nTransitType;
		oTransitCostInfo.TnType				= nDetailTransitType;
		oTransitCostInfo.TdBasicRate		= dBaseCost;
		oTransitCostInfo.TdBasicDistance	= dBaseLength;
		oTransitCostInfo.TdAddRate			= dAddCost;
		oTransitCostInfo.TdAddDistance		= dAddLength;
		oTransitCostInfo.TnMergeRate		= nCostMethod;
		
		AutoType iter = m_mapTransitName.find(nDetailTransitType);
		AutoType end  = m_mapTransitName.end();
		if (iter != end)
		{
			oTransitCostInfo.TstrTypeName = iter->second;
		}
		else
		{
			oTransitCostInfo.TstrTypeName = _T("x");
		}

		m_vecTransitCost.push_back(oTransitCostInfo);
	}
}

bool KMATransitAssignInfoDlg::GetOptioBaseInfo()
{
	double  dInputValue(0.0);
	int     nMaxRoutes        = GetDlgItemInt(IDC_EDIT_MAXPATH);

	if(nMaxRoutes < 2)
	{
		AfxMessageBox(_T("1보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MAXPATH)->SetFocus();
		return false;
	}

	CString strAlpha(_T(""));
	GetDlgItemText(IDC_EDIT_ALPHA, strAlpha);
	CString strBeta(_T(""));
	GetDlgItemText(IDC_EDIT_BETA, strBeta);
	CString strDWtime(_T(""));
	GetDlgItemText(IDC_EDIT_DWTIME, strDWtime);
	CString strMEU(_T(""));
	GetDlgItemText(IDC_EDIT_MEU, strMEU);
	CString strBATime(_T(""));
	GetDlgItemText(IDC_EDIT_TIME, strBATime);


	if (!QBicStringChecker::IsNumeric(strAlpha))
	{
		AfxMessageBox(_T("계수(Alpha)에 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_ALPHA)->SetFocus();
		return false;
	}

	if (!QBicStringChecker::IsNumeric(strBeta))
	{
		AfxMessageBox(_T("지수(Beta)에 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_BETA)->SetFocus();
		return false;
	}

	if (!QBicStringChecker::IsNumeric(strMEU))
	{
		AfxMessageBox(_T("포괄가치 계수를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MEU)->SetFocus();
		return false;
	}

	if (!QBicStringChecker::IsNumeric(strBATime))
	{
		AfxMessageBox(_T("버스 승객 1인당 평균 승하차시간을 입력해 주세요."));
		GetDlgItem(IDC_EDIT_TIME)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strBATime);
	if (dInputValue < 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_TIME)->SetFocus();
		return false;
	}

	if (!QBicStringChecker::IsNumeric(strDWtime))
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strDWtime);
	if (dInputValue < 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
		return false;
	}


	m_oOptionBaseInfo.TnMaxofNo					= nMaxRoutes;
	m_oOptionBaseInfo.TdDWTimeInBus	= _ttof(strBATime);
	m_oOptionBaseInfo.TdAlpha					= _ttof(strAlpha);
	m_oOptionBaseInfo.TdBeta					= _ttof(strBeta);
	m_oOptionBaseInfo.TdDWTimeInRail					= _ttof(strDWtime);
	m_oOptionBaseInfo.TdMEU						= _ttof(strMEU);
	return true;
}

void KMATransitAssignInfoDlg::GetAssignInfoValue( std::vector<TMAFnCoefficient>& a_TCofficient, std::vector<TMATransitCostInfo>& a_CostInfo, TMATransitOptionsBaseInfo& a_oOptionBaseInfo )
{
	a_TCofficient		= m_vecFnCofficient;
	a_CostInfo			= m_vecTransitCost;
	a_oOptionBaseInfo	= m_oOptionBaseInfo;
}

void KMATransitAssignInfoDlg::OnBnClickedOk()
{
	CString strMaxNo(_T(""));
	CButton* pMaxButton = (CButton*)GetDlgItem(IDC_EDIT_MAXPATH);
	pMaxButton->GetWindowTextW(strMaxNo);

	int nMaxNo = _ttoi(strMaxNo);

	if (nMaxNo < 2)
	{
		AfxMessageBox(_T("1보다 큰 숫자를 입력해 주세요."));
		pMaxButton->SetFocus();
		return;
	}

	if (nMaxNo > m_nMaxNo)
	{
		CString strMsg;
		strMsg.Format(_T("Max No.of Routes 의 값이 %d 보다 큽니다."), m_nMaxNo);
		AfxMessageBox(strMsg);
		pMaxButton->SetFocus();
		return;
	}

	if (false == GetOptioBaseInfo())
	{
		return;
	}

	GetCoefficientValue();
	GetTransitCostValue();

	KResizeDialogEx::OnOK();
}


void KMATransitAssignInfoDlg::OnBnClickedButtonDefault()
{
	try
	{
        KMultiModalMainDlg::UtilityFnCoefficient(m_vecFnCofficient);

        TransitInfoSetting();
		WatingVDFSetting();

		UpdateReportCoefficient();
		InitEditButtion();

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

void KMATransitAssignInfoDlg::TransitInfoSetting()
{
    KMultiModalMainDlg::TransitInfoSetting(m_oOptionBaseInfo);
}

void KMATransitAssignInfoDlg::WatingVDFSetting()
{
	try
	{
		std::map<int, KPathGenerateInvehicle > mapWatingVDFInfo;
		KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateInvehicle(mapWatingVDFInfo);

		AutoType findUser		= mapWatingVDFInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType findSystem		= mapWatingVDFInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		AutoType findend		= mapWatingVDFInfo.end();

		if (findUser != findend)
		{
			KPathGenerateInvehicle oInfo = findUser->second;
			m_oOptionBaseInfo.TdAlpha = oInfo.dAlpha;
			m_oOptionBaseInfo.TdBeta  = oInfo.dBeta;
		}
		else if (findSystem != findend)
		{
			KPathGenerateInvehicle oInfo = findSystem->second;
			m_oOptionBaseInfo.TdAlpha = oInfo.dAlpha;
			m_oOptionBaseInfo.TdBeta  = oInfo.dBeta;
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
