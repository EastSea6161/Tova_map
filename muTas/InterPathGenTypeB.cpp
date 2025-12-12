// InterPathGenTypeB.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGenTypeB.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "KExRecordItem.h"
//^#include "DBaseConnector.h"

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

// KInterPathGenTypeB 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGenTypeB, KResizeDialogEx)

KInterPathGenTypeB::KInterPathGenTypeB(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGenTypeB::IDD, pParent)
{

}

KInterPathGenTypeB::~KInterPathGenTypeB()
{
}

void KInterPathGenTypeB::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_AUTO, m_wmAutoReportControl);
	DDX_Control(pDX, IDC_REPORT_TAXI, m_wmTaxiReportControl);
	DDX_Control(pDX, IDC_REPORT_TRANSIT, m_wmTransitReportControl);
}


BEGIN_MESSAGE_MAP(KInterPathGenTypeB, KResizeDialogEx)
END_MESSAGE_MAP()


// KInterPathGenTypeB 메시지 처리기입니다.
BOOL KInterPathGenTypeB::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wmAutoReportControl, TRUE);
	KReportCtrlSetting::Default(m_wmTaxiReportControl, TRUE);
	KReportCtrlSetting::Default(m_wmTransitReportControl, TRUE);

	KDBaseDefineTransitCost::GetTransitModeCostValue2DB(m_pTarget, m_vecTransitCost);
	InitAutoCost();
	InitTaxiCost();

	InitAutoReportHeader();
	InitTaxiREportHeader();
	InitTransitReportheader();

	UpdateAutoReportColumn();
	UpdateTaxiReportColumn();
	UpdateTransitReportHeader();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterPathGenTypeB::SetTarget( KTarget* a_pTarget )
{
	try
	{
		if (a_pTarget == nullptr)
		{
			return;
		}

		m_pTarget = a_pTarget;
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

void KInterPathGenTypeB::InitAutoCost()
{
	m_vecAutoCost.clear();

	TAutoFuel oGasoline;
	oGasoline.nFuelType			= P_FUEL_GASOLINE;
	oGasoline.strFuelName		= P_GASOLINE_NAME;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oGasoline.strFuelName		= P_GASOLINE_NAME_K;
	}
	oGasoline.dPrice			= 2000.0;
	oGasoline.dRation			= 0.5;
	oGasoline.dFuelEfficiency	= 8.00;
	m_vecAutoCost.push_back(oGasoline);

	TAutoFuel oDiesel;
	oDiesel.nFuelType			= P_FUEL_DIESEL;
	oDiesel.strFuelName			= P_DIESEL_NAME;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oDiesel.strFuelName		= P_DIESEL_NAME_K;
	}
	oDiesel.dPrice				= 1700.0;
	oDiesel.dRation				= 0.35;
	oDiesel.dFuelEfficiency		= 12.00;
	m_vecAutoCost.push_back(oDiesel);

	TAutoFuel oLPG;
	oLPG.nFuelType				= P_FUEL_LPG;
	oLPG.strFuelName			= P_LPG_NAME;
	oLPG.dPrice					= 1200.0;
	oLPG.dRation				= 0.15;
	oLPG.dFuelEfficiency		= 6.00;
	m_vecAutoCost.push_back(oLPG);
}

void KInterPathGenTypeB::InitTaxiCost()
{
	m_vecTaxiCost.clear();

	TTaxiFuel oLPG;
	oLPG.nFuelType			= P_FUEL_LPG;
	oLPG.strFuelName		= P_LPG_NAME;
	oLPG.dBaseFare			= 2300.0;
	oLPG.dBaseDistance		= 2.00;
	oLPG.dDistBaseFare		= 100;
	oLPG.dUnitDist			= 0.15;

	m_vecTaxiCost.push_back(oLPG);
}

void KInterPathGenTypeB::InitAutoReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Auto"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_PRICE, _T("Price(ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_RATION, _T("Ratio"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmAutoReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_FUELEFFI, _T("Fuel Efficiency(㎞/ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

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

void KInterPathGenTypeB::InitTaxiREportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Taxi"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_BASEFARE, _T("Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_BASEDIST, _T("Base Dist(㎞)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_DISTBASEFARE, _T("Dist-Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTaxiReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_UNITDIST, _T("Unit Dist"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
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

void KInterPathGenTypeB::InitTransitReportheader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE, _T("Mode"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_TYPE, _T("Type"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_BASEFARE, _T("Base Fare"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_BASEDIST, _T("Base Dist(㎞)"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_DISTBASEFARE, _T("Dist-Base Fare"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_5_COLUMN_UNITDIST, _T("Unit Dist"), 70));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wmTransitReportControl.AddColumn(new CXTPReportColumn(_6_COLUMN_TFP, _T("D.F.P"), 40));
		pColumn->SetTooltip(_T("Discount Fare Policy"));
		pColumn->SetHeaderAlignment(DT_CENTER);
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

void KInterPathGenTypeB::UpdateAutoReportColumn()
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

void KInterPathGenTypeB::UpdateTaxiReportColumn()
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

void KInterPathGenTypeB::UpdateTransitReportHeader()
{
	try
	{
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

		m_wmTransitReportControl.Populate();
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

void KInterPathGenTypeB::ArgumentAutoCost( CStdioFileEx& of )
{
	CXTPReportRecords*          pRecords = m_wmAutoReportControl.GetRecords();
	CXTPReportRecord*           pRecord  = nullptr;
	CXTPReportRecordItemText*   pItemTXT = nullptr;
	CXTPReportRecordItemNumber*   pItemNum = nullptr;

	CString strOut(_T(""));
	int nRecordCount = pRecords->GetCount();
	for (int i =0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);
		pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
		//int nModeID			= pItemTXT->GetItemData();
		CString strModeName = pItemTXT->GetCaption(nullptr); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		double dPrice = pItemNum->GetValue(); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		double dRation = pItemNum->GetValue(); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		double dFuelEff = pItemNum->GetValue(); 

		strOut.Format(_T("Auto%s\t%.2f\t%.2f\t%.2f\r\n"), strModeName, dPrice, dRation, dFuelEff);
		of.WriteString(strOut);
	}
}

void KInterPathGenTypeB::ArgumentTaxiCost( CStdioFileEx& of )
{
	CXTPReportRecords*          pRecords = m_wmTaxiReportControl.GetRecords();
	CXTPReportRecord*           pRecord  = nullptr;
	CXTPReportRecordItemText*   pItemTXT = nullptr;
	CXTPReportRecordItemNumber*   pItemNum = nullptr;

	CString strOut(_T(""));
	int nRecordCount = pRecords->GetCount();
	for (int i =0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);
		pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
		CString strModeName = pItemTXT->GetCaption(nullptr); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		double dBaseFare = pItemNum->GetValue(); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		double dBaseDist = pItemNum->GetValue(); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		double dDistBase = pItemNum->GetValue(); 

		pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
		double dUnitDist = pItemNum->GetValue(); 

		strOut.Format(_T("Taxi\t%.2f\t%.2f\t%.2f\t%.2f\r\n"), dBaseFare, dBaseDist, dDistBase, dUnitDist);
		of.WriteString(strOut);
	}
}

void KInterPathGenTypeB::ArgumentTransitCost( CStdioFileEx& of )
{
	CString strOut(_T(""));

	CXTPReportRecords*           pRecords   = m_wmTransitReportControl.GetRecords();
	CXTPReportRecord*            pRecord    = nullptr;
	CXTPReportRecordItemNumber*  pItemNum   = nullptr;
	CXTPReportRecordItem*        pItem         = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;

	int nRecordCount   = pRecords->GetCount();
	int nBusTypeCount  = 0;
	int nRailTypeCount = 0;
	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);

		pItem                  = pRecord->GetItem(_0_COLUMN_MODE);
		int nTransitType       = (int)pItem->GetItemData();

		if ( TRANSIT_MODE_BUS == nTransitType )
			nBusTypeCount++;
		else if( TRANSIT_MODE_RAIL == nTransitType )
			nRailTypeCount++;
	}
	strOut.Format(_T("detailedLineType\t%d\t%d\r\n"), nBusTypeCount, nRailTypeCount);
	of.WriteString(strOut);

	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);

		pItem                      = pRecord->GetItem(_0_COLUMN_MODE);
		int     nTransitType       = (int)pItem->GetItemData();
		CString strTransitType     = _T("");

		if(TRANSIT_MODE_BUS == nTransitType)
			strTransitType = _T("buscost");
		else if(TRANSIT_MODE_RAIL == nTransitType)
			strTransitType = _T("railcost");

		pItem                      = pRecord->GetItem(_1_COLUMN_TYPE);
		int nDetailTransitType     = (int)pItem->GetItemData();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_BASEFARE);
		double dBaseCost           =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_BASEDIST);
		double dBaseLength         =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_DISTBASEFARE);
		double dAddCost            =  pItemNum->GetValue();

		pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_COLUMN_UNITDIST);
		double dAddLength          =  pItemNum->GetValue();

		pExtItemCheck    = pRecord->GetItem(_6_COLUMN_TFP);
		int nCostMethod  = 0;
		if( TRUE == pExtItemCheck->IsChecked() )
		{
			nCostMethod = 1;
		}
		else
		{
			nCostMethod = 0;
		}

		strOut.Format(_T("%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%d\r\n"), strTransitType, nDetailTransitType, dBaseCost, dBaseLength, dAddCost, dAddLength, nCostMethod);
		of.WriteString(strOut);
	}
}

void KInterPathGenTypeB::SaveResultAutoCost2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();
	try
	{
		CString strDELSQL(_T(""));
		strDELSQL.Format(_T("delete from %s"), TABLE_INTER_AUTO_COST_INFO);
		spDBaseConnection->ExecuteUpdate(strDELSQL);

		CXTPReportRecords*          pRecords = m_wmAutoReportControl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;
		CXTPReportRecordItemNumber*   pItemNum = nullptr;

		CString strOut(_T(""));
		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetCaption(nullptr); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dPrice = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			double dRation = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			double dFuelEff = pItemNum->GetValue(); 

			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(id, name, price, ration, fule) Values (%d, '%s',%.2f, %.2f, %.2f)"), 
				TABLE_INTER_AUTO_COST_INFO, nModeID, strModeName, dPrice, dRation, dFuelEff);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
}

void KInterPathGenTypeB::SaveResultTaxiCost2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();
	try
	{
		CString strDELSQL(_T(""));
		strDELSQL.Format(_T("delete from %s"), TABLE_INTER_TAXI_COST_INFO);
		spDBaseConnection->ExecuteUpdate(strDELSQL);

		CXTPReportRecords*          pRecords = m_wmTaxiReportControl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;
		CXTPReportRecordItemNumber*   pItemNum = nullptr;

		CString strOut(_T(""));
		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nMode = pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetCaption(nullptr); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dBaseFare = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			double dBaseDist = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			double dDistBase = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			double dUnitDist = pItemNum->GetValue(); 

			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(id, name, base_fare,  base_dist, add_fare, add_dist) Values (%d, '%s', %.2f, %.2f, %.2f, %.2f)"), 
				TABLE_INTER_TAXI_COST_INFO, nMode, strModeName, dBaseFare, dBaseDist, dDistBase, dUnitDist);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGenTypeB::SaveResultTransitCost2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDELSQL(_T(""));
		strDELSQL.Format(_T("delete from %s"), TABLE_BUS_PATH_COST);
		spDBaseConnection->ExecuteUpdate(strDELSQL);

		CXTPReportRecords*           pRecords   = m_wmTransitReportControl.GetRecords();
		CXTPReportRecord*            pRecord    = nullptr;
		CXTPReportRecordItemNumber*  pItemNum   = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportRecordItem*        pExtItemCheck = nullptr;

		int nRecordCount   = pRecords->GetCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem                      = pRecord->GetItem(_0_COLUMN_MODE);
			int     nTransitType       = (int)pItem->GetItemData();
			CString strTransitType     = _T("");

			if(TRANSIT_MODE_BUS == nTransitType)
				strTransitType = _T("buscost");
			else if(TRANSIT_MODE_RAIL == nTransitType)
				strTransitType = _T("railcost");

			pItem                      = pRecord->GetItem(_1_COLUMN_TYPE);
			int nDetailTransitType     = (int)pItem->GetItemData();

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_BASEFARE);
			double dBaseCost           =  pItemNum->GetValue();

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_BASEDIST);
			double dBaseLength         =  pItemNum->GetValue();

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_DISTBASEFARE);
			double dAddCost            =  pItemNum->GetValue();

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_COLUMN_UNITDIST);
			double dAddLength          =  pItemNum->GetValue();

			pExtItemCheck    = pRecord->GetItem(_6_COLUMN_TFP);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				nCostMethod = 1;
			}
			else
			{
				nCostMethod = 0;
			}

			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(transit_type, transit_detail_type, base_cost, base_length, add_cost, add_length, cost_method) Values (%d, %d, %.2f, %.2f, %.2f, %.2f, %d)")
				,TABLE_BUS_PATH_COST, nTransitType, nDetailTransitType, dBaseCost, dBaseLength, dAddCost, dAddLength, nCostMethod);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}

