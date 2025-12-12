// ParameterDefineCostDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ParameterDefineCostDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "KExRecordItem.h"

//Auto
enum
{
	_0_COLUMN_MODE = 0,
	_1_COLUMN_PRICE,
	_2_COLUMN_RATION,
	_3_COLUMN_FUELEFFI
};

//Taxi
enum
{
	_1_COLUMN_BASEFARE = 1,
	_2_COLUMN_BASEDIST,
	_3_COLUMN_DISTBASEFARE,
	_4_COLUMN_UNITDIST
};

//Truck
enum
{
	_2_COLUMN_FUELEFFIC = 2
};

//Transit
enum
{
	_1_COLUMN_TYPE = 1,
	_2_COLUMN_BASEFARE,
	_3_COLUMN_BASEDIST,
	_4_COLUMN_DISTBASEFARE,
	_5_COLUMN_UNITDIST,
	_6_COLUMN_TFP
};




// KParameterDefineCostDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KParameterDefineCostDlg, KResizeDialogEx)

KParameterDefineCostDlg::KParameterDefineCostDlg(std::vector<TParaOutputInfo> a_vecOutputInfo, KTarget* a_pTarget,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KParameterDefineCostDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_vecOutputInfo(a_vecOutputInfo)
	, m_bUseTransit(false)
	, m_bUseAuto(false)
	, m_bUseTaxi(false)
	, m_bUseTruck(false)
	, m_bUseBus(false)
	, m_bUseBusRail(false)
	, m_bUseRail(false)

{

}

KParameterDefineCostDlg::~KParameterDefineCostDlg()
{
}

void KParameterDefineCostDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_AUTO, m_wmAutoReportControl);
	DDX_Control(pDX, IDC_REPORT_TAXI, m_wmTaxiReportControl);
	DDX_Control(pDX, IDC_REPORT_TRUCK, m_wmTruckReportControl);
	DDX_Control(pDX, IDC_REPORT_TRANSIT, m_wmTransitReportControl);
}


BEGIN_MESSAGE_MAP(KParameterDefineCostDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KParameterDefineCostDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void KParameterDefineCostDlg::SetCostInfo( std::vector<TAutoFuel> a_vecAutoCost, std::vector<TTaxiFuel> a_vecTaxiCost, std::vector<TTruckFuel> a_vecTruckCost, std::vector<TTransitCost> a_vecTransitCost )
{
	try
	{
		m_vecAutoCost		= a_vecAutoCost;
		m_vecTaxiCost		= a_vecTaxiCost;
		m_vecTruckCost		= a_vecTruckCost;
		m_vecTransitCost	= a_vecTransitCost;
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


// KParameterDefineCostDlg 메시지 처리기입니다.


BOOL KParameterDefineCostDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KReportCtrlSetting::Default(m_wmAutoReportControl,		TRUE);
		KReportCtrlSetting::Default(m_wmTaxiReportControl,		TRUE);
		KReportCtrlSetting::Default(m_wmTruckReportControl,		TRUE);
		KReportCtrlSetting::Default(m_wmTransitReportControl,	TRUE);

		CheckUseMode();

		InitAutoReportHeader();
		InitTaxiReportHeader();
		InitTruckReportHeader();
		InitTransitReportHeader();

		UpdateAutoReportColumn();
		UpdateTaxiReportHeader();
		UpdateTruckReportHeader();
		UpdateTransitReportHeader();
		
		ResizeComponent();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
	return TRUE; 
}

void KParameterDefineCostDlg::InitAutoReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Fuel Type"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_PRICE, _T("Price(ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("가격(￦/ℓ)"));
        }

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_RATION, _T("Ration"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("구성비"));
        }

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_FUELEFFI, _T("Fuel Efficiency(㎞/ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연비(㎞/ℓ)"));
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

void KParameterDefineCostDlg::InitTaxiReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Taxi"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_BASEFARE, _T("Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("기본운임"));
        }

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_BASEDIST, _T("Base Dist(㎞)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("~Km까지"));
        }

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_DISTBASEFARE, _T("Dist-Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("추가운임"));
        }

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_UNITDIST, _T("Unit Dist"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("매 ~Km마다"));
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

void KParameterDefineCostDlg::InitTruckReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmTruckReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Truck"), 70,FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wmTruckReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_PRICE, _T("Price(ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("가격(￦/ℓ)"));
        }

		pColumn = m_wmTruckReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_FUELEFFIC, _T("Fuel Efficiency(㎞/ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연비(㎞/ℓ)"));
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

void KParameterDefineCostDlg::InitTransitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Mode"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시스템 타입명"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_TYPE, _T("Type"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("타입"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_BASEFARE, _T("Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("기본운임"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_BASEDIST, _T("Base Dist(㎞)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("~Km까지"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_DISTBASEFARE, _T("Dist-Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("추가운임"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_5_COLUMN_UNITDIST, _T("Unit Dist"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("~Km 마다"));
        }

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_6_COLUMN_TFP, _T("D.F.P"), 100));
		pColumn->SetTooltip(_T("Discount Fare Policy"));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통합환승할인"));
			pColumn->SetTooltip(_T("통합환승할인 적용"));
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

void KParameterDefineCostDlg::CheckUseMode()
{
	try
	{
		int nCount = m_vecOutputInfo.size();
		for (int i = 0;  i < nCount; i++)
		{
			TParaOutputInfo oInfo = m_vecOutputInfo[i];
			if (true == oInfo.bUseData)
			{
				if (oInfo.nMode == P_MODE_AUTO)
				{
					m_bUseAuto = true;
				}
				else if (oInfo.nMode == P_MODE_TAXI)
				{
					m_bUseTaxi = true;
				}
				else if (oInfo.nMode == P_MODE_TRUCK)
				{
					m_bUseTruck = true;
				}
				else if (oInfo.nMode == P_MODE_BUS)
				{
					m_bUseTransit = true;
					m_bUseBus = true;
				}
				else if ( oInfo.nMode == P_MODE_RAIL )
				{
					m_bUseTransit = true;
					m_bUseRail = true;
				}
				else if (oInfo.nMode == P_MODE_BUS_RAIL)
				{
					m_bUseTransit = true;
					m_bUseBusRail = true;
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

void KParameterDefineCostDlg::UpdateAutoReportColumn()
{
	try
	{
		int nCount = m_vecAutoCost.size();

		for (int i = 0; i < nCount; i++)
		{
			TAutoFuel oAutoInfo = m_vecAutoCost[i];
			CXTPReportRecord*		pRecord = m_wmAutoReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oAutoInfo.strFuelName));
			pItem->SetItemData(oAutoInfo.nFuelType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			if (false == m_bUseAuto)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAutoInfo.dPrice, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAutoInfo.dRation, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAutoInfo.dFuelEfficiency, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

			}
		}

		m_wmAutoReportControl.Populate();
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

void KParameterDefineCostDlg::UpdateTaxiReportHeader()
{
	try
	{
		int nCount = m_vecTaxiCost.size();

		for (int i = 0; i < nCount; i++)
		{
			TTaxiFuel oTaxiInfo = m_vecTaxiCost[i];
			CXTPReportRecord*		pRecord = m_wmTaxiReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTaxiInfo.strFuelName));
			pItem->SetItemData(oTaxiInfo.nFuelType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			if (false == m_bUseTaxi)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxiInfo.dBaseFare, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxiInfo.dBaseDistance, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxiInfo.dDistBaseFare, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxiInfo.dUnitDist, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

			}
		}

		m_wmTaxiReportControl.Populate();
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

void KParameterDefineCostDlg::UpdateTruckReportHeader()
{
	try
	{
		int nCount = m_vecTruckCost.size();

		for (int i = 0; i < nCount; i++)
		{
			TTruckFuel oTruckInfo = m_vecTruckCost[i];
			CXTPReportRecord*		pRecord = m_wmTruckReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTruckInfo.strFuelName));
			pItem->SetItemData(oTruckInfo.nFuelType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			
			if (false == m_bUseTruck)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTruckInfo.dPrice, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTruckInfo.dFuelEfficiency, _T("%.2f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);

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

	m_wmTruckReportControl.Populate();
}

void KParameterDefineCostDlg::UpdateTransitReportHeader()
{
	try
	{

		if (false == m_bUseTransit)
		{
			CXTPReportRecord*		pRecord = m_wmTransitReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			m_wmTransitReportControl.Populate();
			return;
		}

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup*	  pModeCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_MODE_CODE_KEY);
		KCodeGroup*	  pTypeCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_TYPE_CODE_KEY);

		int nCount = m_vecTransitCost.size();
		for (int i = 0; i < nCount; i++)
		{
			TTransitCost oTransitInfo = m_vecTransitCost[i];
			CXTPReportRecord*		pRecord = m_wmTransitReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			CString strModeName = pModeCodeGroup->SingleCodeValue(oTransitInfo.nMode);
			CString strTypeName = pTypeCodeGroup->SingleCodeValue(oTransitInfo.nTType);
			CString strTypeCode(_T(""));
			strTypeCode.Format(_T("%d:%s"), oTransitInfo.nTType, strTypeName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
			pItem->SetItemData(oTransitInfo.nMode);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTypeCode));
			pItem->SetItemData(oTransitInfo.nTType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTbasicRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTbasicDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTAddRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTAddDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			if( 1 == oTransitInfo.nTMergeRate )
				pItem->SetChecked(TRUE);
			else
				pItem->SetChecked(FALSE);
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
	m_wmTransitReportControl.Populate();
}

void KParameterDefineCostDlg::GetAutoCostResult()
{
	try
	{
		CXTPReportRecords* pRecords = m_wmAutoReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*  pRecord					= pRecords->GetAt(i);
			CXTPReportRecordItemNumber*  pItemNum		= nullptr;
			CXTPReportRecordItem* pItem					= pRecord->GetItem(_0_COLUMN_MODE);
			int nFuelType								= pItem->GetItemData();

			size_t nCount = m_vecAutoCost.size();
			for (size_t k = 0; k < nCount ; k++)
			{
				TAutoFuel& oFuel = m_vecAutoCost[k];
				if (oFuel.nFuelType == nFuelType)
				{
					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PRICE);
					oFuel.dPrice		  = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_RATION);
					oFuel.dRation		  = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_FUELEFFI);
					oFuel.dFuelEfficiency = pItemNum->GetValue();
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

void KParameterDefineCostDlg::GetTaxiCostResult()
{
	try
	{
		CXTPReportRecords* pRecords = m_wmTaxiReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*  pRecord					= pRecords->GetAt(i);
			CXTPReportRecordItemNumber*  pItemNum		= nullptr;
			CXTPReportRecordItem* pItem					= pRecord->GetItem(_0_COLUMN_MODE);
			int nFuelType								= pItem->GetItemData();

			size_t nCount = m_vecTaxiCost.size();
			for (size_t k = 0; k < nCount ; k++)
			{
				TTaxiFuel& oFuel = m_vecTaxiCost[k];
				
				if (oFuel.nFuelType == nFuelType)
				{
					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_BASEFARE);
					oFuel.dBaseFare		  = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_BASEDIST);
					oFuel.dBaseDistance	  = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_DISTBASEFARE);
					oFuel.dDistBaseFare = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_UNITDIST);
					oFuel.dUnitDist		  = pItemNum->GetValue();
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

void KParameterDefineCostDlg::GetTruckCostResult()
{
	try
	{
		CXTPReportRecords* pRecords = m_wmTruckReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*  pRecord					= pRecords->GetAt(i);
			CXTPReportRecordItemNumber*  pItemNum		= nullptr;
			CXTPReportRecordItem* pItem					= pRecord->GetItem(_0_COLUMN_MODE);
			int nFuelType								= pItem->GetItemData();

			size_t nCount = m_vecTruckCost.size();
			for (size_t k = 0; k < nCount ; k++)
			{
				TTruckFuel& oFuel = m_vecTruckCost[k];
				if (oFuel.nFuelType == nFuelType)
				{
					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_PRICE);
					oFuel.dPrice		  = pItemNum->GetValue();

					pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_FUELEFFIC);
					oFuel.dFuelEfficiency = pItemNum->GetValue();
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

void KParameterDefineCostDlg::GetTransitCostResult()
{
	try
	{
		CXTPReportRecords* pRecords = m_wmTransitReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*  pRecord					= pRecords->GetAt(i);
			CXTPReportRecordItemNumber*  pItemNum		= nullptr;
			
			CXTPReportRecordItem* pItem					= pRecord->GetItem(_0_COLUMN_MODE);
			int nMode = pItem->GetItemData();

			pItem = pRecord->GetItem(_1_COLUMN_TYPE);
			int nType = pItem->GetItemData();

			size_t nCount = m_vecTransitCost.size();
			for (size_t k = 0 ; k < nCount ; k++)
			{
				TTransitCost& oTransitCost = m_vecTransitCost[k];
				if (oTransitCost.nMode == nMode && oTransitCost.nTType == nType)
				{
					pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_BASEFARE);
					oTransitCost.dTbasicRate	= pItemNum->GetValue();

					pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_BASEDIST);
					oTransitCost.dTbasicDistance= pItemNum->GetValue();

					pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_DISTBASEFARE);
					oTransitCost.dTAddRate		= pItemNum->GetValue();

					pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_COLUMN_UNITDIST);
					oTransitCost.dTAddDistance	= pItemNum->GetValue();

					pItem						= pRecord->GetItem(_6_COLUMN_TFP);
					if (TRUE == pItem->GetCheckedState())
					{
						oTransitCost.nTMergeRate = 1;
					}
					else
					{
						oTransitCost.nTMergeRate = 0;
					}
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




void KParameterDefineCostDlg::OnBnClickedOk()
{
	try
	{
		if (true == m_bUseAuto)
		{
			GetAutoCostResult();
		}
		
		if (true == m_bUseTaxi)
		{
			GetTaxiCostResult();
		}

		if (true == m_bUseTruck)
		{
			GetTruckCostResult();
		}
		
		if (true == m_bUseTransit)
		{
			GetTransitCostResult();
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
	KResizeDialogEx::OnOK();
}

void KParameterDefineCostDlg::GetCostInfo( std::vector<TAutoFuel>& a_vecAutoCost, std::vector<TTaxiFuel>& a_vecTaxiCost, std::vector<TTruckFuel>& a_vecTruckCost, std::vector<TTransitCost>& a_vecTransitCost )
{
	try
	{
		if (true == m_bUseAuto)
		{
			a_vecAutoCost	= m_vecAutoCost;
		}
		
		if (true == m_bUseTaxi)
		{
			a_vecTaxiCost	= m_vecTaxiCost;
		}
		
		if (true == m_bUseTruck)
		{
			a_vecTruckCost	= m_vecTruckCost;
		}
		
		if (true == m_bUseTransit)
		{
			a_vecTransitCost= m_vecTransitCost;	
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

void KParameterDefineCostDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR2,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_AUTO,  				  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TAXI,       			  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TRUCK,       			  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR3,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TRANSIT,       		  SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1,       			      SZ_BOTTOM_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDOK,       						 SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}
