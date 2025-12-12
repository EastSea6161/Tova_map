// InterTransitModeParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterTransitModeParaDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseAssignment.h"

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
	_0_ROW_IN_VEHTICLE_TIME		= 0,
	_1_ROW_WAITING_TIME         = 1,
	_2_ROW_TRANSFER_TIME	    = 2,
	_3_ROW_ACCESSWALKTIME		= 3,
	_4_ROW_EGRESSWALKTIME		= 4,
	_5_ROW_NOOFTRANSFER		    = 5,
	_6_ROW_COST					= 6,
	_7_ROW_NOOFSTOP				= 7

};

const int TRANSIT_TYPE_BUS			= 3;
const int TRANSIT_TYPE_SUBWAY		= 4;
const int TRANSIT_TYPE_BUS_SUBWAY	= 5;

// KInterTransitModeParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterTransitModeParaDlg, KResizeDialogEx)

KInterTransitModeParaDlg::KInterTransitModeParaDlg(KTarget* a_pTarget, std::vector<TMAFnCoefficient> a_vecCofficient, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterTransitModeParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_vecFnCofficient(a_vecCofficient)
{

}

KInterTransitModeParaDlg::~KInterTransitModeParaDlg()
{
}

void KInterTransitModeParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReportACtrl);
}


BEGIN_MESSAGE_MAP(KInterTransitModeParaDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterTransitModeParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterTransitModeParaDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterTransitModeParaDlg 메시지 처리기입니다.


BOOL KInterTransitModeParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ResizeComponent();
	KReportCtrlSetting::Default(m_wndReportACtrl, TRUE);
	InitReportHeader();
	UpdateReportData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterTransitModeParaDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_ACOLUMN_FUNCTION, _T("Function"),        40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetDrawHeaderRowsVGrid();

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_ACOLUMN_BUS,       _T("Bus"),            35));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_2_ACOLUMN_SUBWAY,    _T("Rail"),         35));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_3_ACOLUMN_BUSSUBWAY,    _T("Bus + Rail"),      35));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KInterTransitModeParaDlg::UpdateReportData()
{
	TMAFnCoefficient oBusCofficient;
	TMAFnCoefficient oSubwayCofficient;
	TMAFnCoefficient oBusSubwayConfficient;

	int nCofficientCount = m_vecFnCofficient.size();
	if (nCofficientCount < 1 )
	{
		AfxMessageBox(_T("생성된 초기 버스 경로 관련 정보가 존재하지 않습니다."));
		return;
	}
	for (int i = 0; i < nCofficientCount; i ++)
	{
		TMAFnCoefficient oCofficient = m_vecFnCofficient[i];
		if (oCofficient.TnMode == TRANSIT_TYPE_BUS)
		{
			oBusCofficient = oCofficient;
		}
		else if(oCofficient.TnMode == TRANSIT_TYPE_SUBWAY)
		{
			oSubwayCofficient = oCofficient;
		}
		else if (oCofficient.TnMode == TRANSIT_TYPE_BUS_SUBWAY)
		{
			oBusSubwayConfficient = oCofficient;
		}
	}

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("In-Vehicle Time")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdInVehcle,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdInVehcle, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdInVehcle, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Waiting Time")));
	pItem->SetAlignment(xtpColumnTextCenter);
	
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdWaitingTime,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdWaitingTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdWaitingTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Transfer Time")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdTransferTime,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdTransferTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdTransferTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Access Walk Time")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdAccessWalkTime,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdAccessWalkTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdAccessWalkTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Egress Walk Time")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdEgressWalkTime,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdEgressWalkTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdEgressWalkTime, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Transfers")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdNoOfTransfer,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdNoOfTransfer, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdNoOfTransfer, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Cost")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdCost,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdCost, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdCost, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Stop At Zone")));
	pItem->SetAlignment(xtpColumnTextCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdNoOfStop,    _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdNoOfStop, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdNoOfStop, _T("%.10f")));
	pItem->SetAlignment(xtpColumnTextRight);

	m_wndReportACtrl.Populate();
}



void KInterTransitModeParaDlg::OnBnClickedOk()
{
	try
	{
		CXTPReportRecords*          pRecords = m_wndReportACtrl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;


		TMAFnCoefficient oExistBusFnCofficient;
		TMAFnCoefficient oExistSubwayFnCofficient;
		TMAFnCoefficient oExistBusSubwayFnCofficient;

		int nCount = m_vecFnCofficient.size();
		for (int i = 0; i < nCount ; i ++)
		{
			TMAFnCoefficient oCofficient  = m_vecFnCofficient[i];
			if (oCofficient.TnMode == TRANSIT_TYPE_BUS )
			{
				oExistBusFnCofficient = oCofficient;
			}
			if (oCofficient.TnMode == TRANSIT_TYPE_SUBWAY)
			{
				oExistSubwayFnCofficient = oCofficient;
			}
			if (oCofficient.TnMode == TRANSIT_TYPE_BUS_SUBWAY)
			{
				oExistBusSubwayFnCofficient = oCofficient;
			}
		}

		m_vecFnCofficient.clear();

		TMAFnCoefficient oBusFnCofficient;
		oBusFnCofficient.TnMode = TRANSIT_TYPE_BUS;
		TMAFnCoefficient oSubwayFnCofficient;
		oSubwayFnCofficient.TnMode = TRANSIT_TYPE_SUBWAY;
		TMAFnCoefficient oBusSubwayFnCofficient;
		oBusSubwayFnCofficient.TnMode = TRANSIT_TYPE_BUS_SUBWAY;

		int nRecordCount   = pRecords->GetCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			double dBusCoeff(0.0);
			double dSubwayCoeff(0.0);
			double dBusSubwayCodeff(0.0);
			double dUpperCodeff(0.0);

			pRecord			= pRecords->GetAt(i);
			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_ACOLUMN_BUS);
			dBusCoeff		= pItemNum->GetValue();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_ACOLUMN_SUBWAY);
			dSubwayCoeff	= pItemNum->GetValue();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_ACOLUMN_BUSSUBWAY);
			dBusSubwayCodeff	= pItemNum->GetValue();


			if (_0_ROW_IN_VEHTICLE_TIME == i)
			{
				oBusFnCofficient.TdInVehcle = dBusCoeff;
				oSubwayFnCofficient.TdInVehcle = dSubwayCoeff;
				oBusSubwayFnCofficient.TdInVehcle = dBusSubwayCodeff;
			}
			else if (_1_ROW_WAITING_TIME == i)
			{
				oBusFnCofficient.TdWaitingTime = dBusCoeff;
				oSubwayFnCofficient.TdWaitingTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdWaitingTime = dBusSubwayCodeff;
			}
			else if (_2_ROW_TRANSFER_TIME == i)
			{
				oBusFnCofficient.TdTransferTime = dBusCoeff;
				oSubwayFnCofficient.TdTransferTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdTransferTime = dBusSubwayCodeff;
			}
			else if (_3_ROW_ACCESSWALKTIME == i)
			{
				oBusFnCofficient.TdAccessWalkTime = dBusCoeff;
				oSubwayFnCofficient.TdAccessWalkTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdAccessWalkTime = dBusSubwayCodeff;
			}
			else if (_4_ROW_EGRESSWALKTIME == i)
			{
				oBusFnCofficient.TdEgressWalkTime = dBusCoeff;
				oSubwayFnCofficient.TdEgressWalkTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdEgressWalkTime = dBusSubwayCodeff;
			}
			else if (_5_ROW_NOOFTRANSFER == i)
			{
				oBusFnCofficient.TdNoOfTransfer = dBusCoeff;
				oSubwayFnCofficient.TdNoOfTransfer = dSubwayCoeff;
				oBusSubwayFnCofficient.TdNoOfTransfer = dBusSubwayCodeff;
			}
			else if (_6_ROW_COST == i)
			{
				oBusFnCofficient.TdCost = dBusCoeff;
				oSubwayFnCofficient.TdCost = dSubwayCoeff;
				oBusSubwayFnCofficient.TdCost = dBusSubwayCodeff;
			}
			else if (_7_ROW_NOOFSTOP == i)
			{
				oBusFnCofficient.TdNoOfStop = dBusCoeff;
				oSubwayFnCofficient.TdNoOfStop = dSubwayCoeff;
				oBusSubwayFnCofficient.TdNoOfStop = dBusSubwayCodeff;
			}
		}

		m_vecFnCofficient.push_back(oBusFnCofficient);
		m_vecFnCofficient.push_back(oSubwayFnCofficient);
		m_vecFnCofficient.push_back(oBusSubwayFnCofficient);
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
	
	KResizeDialogEx::OnOK();
}


void KInterTransitModeParaDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterTransitModeParaDlg::GetInterTransitModeValue( std::vector<TMAFnCoefficient>& a_vecFnCofficient )
{
	a_vecFnCofficient = m_vecFnCofficient;
}

void KInterTransitModeParaDlg::ResizeComponent()
{
	SetResize(IDC_REPORT,				      SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR11,				  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}
