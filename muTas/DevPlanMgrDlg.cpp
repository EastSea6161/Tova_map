// DevPlanMgrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanMgrDlg.h"
#include "DevZoneSelectDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"

enum
{
	_0_LIST_COLUMN_INDEX = 0,
	_1_LIST_COLUMN_NAME  = 1
};

enum
{
	_0_LOC_COLUMN_ZONEID = 0,
	_1_LOC_COLUMN_RATE   = 1
};

enum 
{
	_0_ANNUAL_COLUMN_YEAR = 0,
	_1_ANNUAL_COLUMN_RATE = 1
};


enum
{
	_0_FAC_COLUMN_TYPE = 0,
	_1_FAC_COLUMN_SIZE = 1
};


// KDevPlanMgrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanMgrDlg, KResizeDialogEx)

KDevPlanMgrDlg::KDevPlanMgrDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDevPlanMgrDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nxMaxPlanId(0)
{
}

KDevPlanMgrDlg::~KDevPlanMgrDlg()
{
	std::map<Integer, TPlanManageInfo*>::iterator iter    = m_mapPlanInfo.begin();
	std::map<Integer, TPlanManageInfo*>::iterator iterEnd = m_mapPlanInfo.end();
	
	while(iter != iterEnd)
	{
		TPlanManageInfo* pPlanInfo = iter->second;

		delete pPlanInfo;

		++iter;
	}
}

void KDevPlanMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndPlanReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndLocReport);
	DDX_Control(pDX, IDC_REPORT5, m_wndAnnualPlanReport);
	DDX_Control(pDX, IDC_REPORT4, m_wndFacReport);
	DDX_Control(pDX, IDC_CBO_FIELDSELECT, m_cboPlanType);
}


BEGIN_MESSAGE_MAP(KDevPlanMgrDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADDLIST, &KDevPlanMgrDlg::OnBnClickedBtnAddlist)
	ON_BN_CLICKED(IDC_BTN_DELLIST, &KDevPlanMgrDlg::OnBnClickedBtnDellist)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED,   IDC_REPORT1, &KDevPlanMgrDlg::OnPlanSelChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KDevPlanMgrDlg::OnPlanValueChanged)
	ON_CBN_SELCHANGE(IDC_CBO_FIELDSELECT, &KDevPlanMgrDlg::OnCbnSelchangeCboPlanType)
	ON_EN_CHANGE(IDC_EDIT_YEAR,           &KDevPlanMgrDlg::OnEnChangeEditYear)
	ON_BN_CLICKED(IDC_BTN_SELZONE,        &KDevPlanMgrDlg::OnBnClickedBtnSelzone)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT2, &KDevPlanMgrDlg::OnLocValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT5, &KDevPlanMgrDlg::OnAnnualValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT4, &KDevPlanMgrDlg::OnFACValueChanged)
	ON_BN_CLICKED(IDC_BTN_ADDANNUAL, &KDevPlanMgrDlg::OnBnClickedBtnAddannual)
	ON_BN_CLICKED(IDC_BTN_DELANNUAL, &KDevPlanMgrDlg::OnBnClickedBtnDelannual)
	ON_BN_CLICKED(IDOK, &KDevPlanMgrDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KDevPlanMgrDlg 메시지 처리기입니다.


void KDevPlanMgrDlg::SetPlan()
{
	SetDevPlan();
	SetDevLocation();
	SetDevAnnualPlan();
	SetDevUseFacility();
}


void KDevPlanMgrDlg::SetDevPlan()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;

		strSQL.Format (_T(" SELECT plan_id, plan_name, plan_type, completion_year ")
			_T(" FROM dev_plan ")
			_T(" order by plan_id "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		if(nullptr == spResult)
		{
			return;
		}

		while( spResult->Next() )
		{
			TPlanManageInfo* pNewPlanInfo = new TPlanManageInfo;

			pNewPlanInfo->emManage        = EMPlanManageNone;
			Integer nxPlanID              = spResult->GetValueInt64 (0);
			pNewPlanInfo->nxId            = nxPlanID;
			pNewPlanInfo->strName         = spResult->GetValueString(1);
			pNewPlanInfo->nType           = spResult->GetValueInt   (2);
			pNewPlanInfo->nCompletionYear = spResult->GetValueInt   (3);
			pNewPlanInfo->strNewName      = _T("");

			SetDevUseFacilityBase(pNewPlanInfo->mapDevFacility);

			m_mapPlanInfo.insert(std::make_pair(nxPlanID, pNewPlanInfo));

			if(m_nxMaxPlanId < nxPlanID )
			{
				m_nxMaxPlanId = nxPlanID;
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("dev_plan 데이터 조회중 오류가 발생하였습니다."));
	}
}


void KDevPlanMgrDlg::SetDevLocation()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;

		strSQL.Format (_T(" SELECT plan_id, zone_id, ratio ")
			_T(" FROM dev_location ")
			_T(" order by plan_id, zone_id "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		if(nullptr == spResult)
		{
			return;
		}

		std::map<Integer, TPlanManageInfo*>::iterator iter;

		while( spResult->Next() )
		{
			Integer nxPlanID            = spResult->GetValueInt64 (0);

			iter = m_mapPlanInfo.find(nxPlanID);

			if( iter == m_mapPlanInfo.end() )
			{
				continue;
			}

			TPlanManageInfo* pManageInfo  = iter->second;
			std::map<Integer, TDevLocation> &mapDevLocation = pManageInfo->mapDevLocation;

			TDevLocation    oTDevLocation;
			oTDevLocation.nxZoneID       = spResult->GetValueInt64 (1); 
			oTDevLocation.dRate          = spResult->GetValueDouble(2);

			mapDevLocation.insert(std::make_pair(oTDevLocation.nxZoneID, oTDevLocation));
		}
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("dev_location 데이터 조회중 오류가 발생하였습니다."));
	}
}


void KDevPlanMgrDlg::SetDevAnnualPlan()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;

		strSQL.Format (_T(" SELECT plan_id, year, ratio ")
			_T(" FROM dev_annual_plan_resident ")
			_T(" order by plan_id, year "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		if(nullptr == spResult)
		{
			return;
		}

		std::map<Integer, TPlanManageInfo*>::iterator iter;

		Integer nxNextID = -1;
		int     nTempID  = 1;
		while( spResult->Next() )
		{
			Integer nxPlanID            = spResult->GetValueInt64 (0);

			iter = m_mapPlanInfo.find(nxPlanID);

			if( iter == m_mapPlanInfo.end() )
			{
				continue;
			}

			if(nxNextID != nxPlanID)
			{
				nxNextID = nxPlanID;
				nTempID = 1;
			}

			TPlanManageInfo* pManageInfo  = iter->second;
			std::map<int, TDevAnnualPlan> &mapDevAnnualPlan = pManageInfo->mapDevAnnualPlan;

			TDevAnnualPlan    oTDevAnnualPlan;
			oTDevAnnualPlan.nYear          = spResult->GetValueInt   (1);
			oTDevAnnualPlan.dRate          = spResult->GetValueDouble(2);
			oTDevAnnualPlan.nTempID        = nTempID++;

			//mapDevAnnualPlan.insert(std::make_pair(oTDevAnnualPlan.nYear, oTDevAnnualPlan));
			mapDevAnnualPlan.insert(std::make_pair(oTDevAnnualPlan.nTempID, oTDevAnnualPlan));
		}
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("dev_annual_plan_resident 데이터 조회중 오류가 발생하였습니다."));
	}
}


void KDevPlanMgrDlg::SetDevUseFacility()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;

		strSQL.Format (_T(" SELECT plan_id, facility_type, plan_type, size ")
			_T(" FROM dev_use_facility ")
			_T(" order by plan_id, facility_type, plan_type "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		if(nullptr == spResult)
		{
			return;
		}

		std::map<Integer, TPlanManageInfo*>::iterator iter;


		while( spResult->Next() )
		{
			Integer nxPlanID            = spResult->GetValueInt64 (0);

			iter = m_mapPlanInfo.find(nxPlanID);

			if( iter == m_mapPlanInfo.end() )
			{
				continue;
			}

			TPlanManageInfo* pManageInfo  = iter->second;
			std::map<int, std::map<int, TDevUseFacility>> &mapDevFacility = pManageInfo->mapDevFacility;

			int    nFacilityType = spResult->GetValueInt   (1);
			int    nPlanType     = spResult->GetValueInt   (2);
			double dSize         = spResult->GetValueDouble(3);

			std::map<int, std::map<int, TDevUseFacility>>::iterator iter2 = mapDevFacility.find(nFacilityType);

			if(iter2 != mapDevFacility.end())
			{
				std::map<int, TDevUseFacility> &mapPlanType = iter2->second;
				std::map<int, TDevUseFacility>::iterator iter3 = mapPlanType.find(nPlanType);

				if(iter3 != mapPlanType.end())
				{
					TDevUseFacility &oTDevUseFacility = iter3->second;
					oTDevUseFacility.nFacilityType = nFacilityType;
					oTDevUseFacility.nPlanType     = nPlanType;
					oTDevUseFacility.dSize         = dSize;
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("dev_use_facility 데이터 조회중 오류가 발생하였습니다."));
	}
}


void KDevPlanMgrDlg::SetDevUseFacilityBase(std::map<int, std::map<int, TDevUseFacility>> &a_mapFacilityBase)
{
	for(int i= 0; i< 5; i++)
	{
		int nFacilityType = 0;

		if(i == 0)
			nFacilityType = EMDevTypeResidential;
		else if(i == 1)
			nFacilityType = EMDevTypeIndustry;
		else if(i == 2)
			nFacilityType = EMDevTypeCommerce;
		else if(i == 3)
			nFacilityType = EMDevTypeGreens;
		else
			nFacilityType = EMDevTypeOthers;

		std::map<int, TDevUseFacility> mapPlanType;

		for(int j= 0; j< 5; j++)
		{
			int    nPlanType = 0;
			double dSize     = 0.0;

			if(j == 0)
				nPlanType = EMPlanIndexArea;
			else if(j == 1)
				nPlanType = EMPlanIndexHouseHold;
			else if(j == 2)
				nPlanType = EMPlanIndexStudent;
			else if(j == 3)
				nPlanType = EMPlanIndexPopulation;
			else
				nPlanType = EMPlanIndexEmployee;

			TDevUseFacility oTUseFacility;

			oTUseFacility.nFacilityType = nFacilityType;
			oTUseFacility.nPlanType     = nPlanType;
			oTUseFacility.dSize         = 0.0; 

			mapPlanType.insert(std::make_pair(nPlanType, oTUseFacility));
		}

		a_mapFacilityBase.insert(std::make_pair(nFacilityType, mapPlanType));
	}

	m_mapFacNameCode.insert(std::make_pair(EMDevTypeResidential, TYPE_NAME_RESIDENTIAL));
	m_mapFacNameCode.insert(std::make_pair(EMDevTypeIndustry,    TYPE_NAME_INDUSTRY));
	m_mapFacNameCode.insert(std::make_pair(EMDevTypeCommerce,    TYPE_NAME_COMMERCE));
	m_mapFacNameCode.insert(std::make_pair(EMDevTypeGreens,      TYPE_NAME_GREENS));
	m_mapFacNameCode.insert(std::make_pair(EMDevTypeOthers,      TYPE_NAME_OTHERS));

	m_mapPlanIndexNameCode.insert(std::make_pair(EMPlanIndexArea,       PLANINDEX_NAME_AREA));
	m_mapPlanIndexNameCode.insert(std::make_pair(EMPlanIndexHouseHold,  PLANINDEX_NAME_HOUSEHOLD));
	m_mapPlanIndexNameCode.insert(std::make_pair(EMPlanIndexStudent,    PLANINDEX_NAME_STUDENT));
	m_mapPlanIndexNameCode.insert(std::make_pair(EMPlanIndexPopulation, PLANINDEX_NAME_POPULATION));
	m_mapPlanIndexNameCode.insert(std::make_pair(EMPlanIndexEmployee,   PLANINDEX_NAME_EMPLOYEE));
}


void KDevPlanMgrDlg::ResizeComponent()
{
	SetResize(IDC_REPORT1,			      SZ_TOP_LEFT,      SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR5,			  SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_ADDLIST,			  SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_DELLIST,			  SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	
	SetResize(IDC_SEPRATOR7,			  SZ_TOP_LEFT,      SZ_BOTTOM_LEFT);	

	SetResize(IDC_SEPRATOR3,			  SZ_TOP_LEFT,      SZ_TOP_RIGHT);	

	SetResize(IDC_REPORT2,		    	  SZ_TOP_LEFT,      SZ_MIDDLE_CENTER);
	SetResize(IDC_BTN_SELZONE,		      SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);  
	
	SetResize(IDC_SEPRATOR8,		      SZ_TOP_CENTER,    SZ_MIDDLE_CENTER);  

	SetResize(IDC_STATIC4,		          SZ_TOP_CENTER,    SZ_TOP_CENTER);  
	SetResize(IDC_REPORT5,		          SZ_TOP_CENTER,    SZ_MIDDLE_RIGHT); 

	SetResize(IDC_BTN_ADDANNUAL,		  SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT); 
	SetResize(IDC_BTN_DELANNUAL,		  SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT); 

	SetResize(IDC_SEPRATOR4,		      SZ_MIDDLE_LEFT,   SZ_MIDDLE_RIGHT);  
	 
	SetResize(IDC_STATIC7,		          SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);  
	SetResize(IDC_REPORT4,		          SZ_MIDDLE_LEFT,   SZ_BOTTOM_RIGHT);  

	SetResize(IDC_SEPRATOR9,		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);

	SetResize(IDOK,					      SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


BOOL KDevPlanMgrDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetPlan();

	KReportCtrlSetting::Default(m_wndPlanReport, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_wndPlanReport);
	KReportCtrlSetting::Default(m_wndLocReport, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_wndLocReport);
	KReportCtrlSetting::Default(m_wndAnnualPlanReport, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_wndAnnualPlanReport);
	KReportCtrlSetting::Default(m_wndFacReport, TRUE, FALSE, TRUE);

	InitPlanList();
	UpdatePlanList();

	InitPlanDetail();
	UpdatePlanDetail();

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDevPlanMgrDlg::InitPlanList()
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndPlanReport.AddColumn(new CXTPReportColumn(_0_LIST_COLUMN_INDEX, _T("No."), 20)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	pColumn = m_wndPlanReport.AddColumn(new CXTPReportColumn(_1_LIST_COLUMN_NAME, _T("Plan Name"), 80)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_LEFT);

	m_wndPlanReport.Populate();
}


void KDevPlanMgrDlg::UpdatePlanList(void)
{
	m_wndPlanReport.GetRecords()->RemoveAll();

	std::map<Integer, TPlanManageInfo*>::iterator itPlan, itEnd = m_mapPlanInfo.end();

	int nIdx = 1;
	for(itPlan = m_mapPlanInfo.begin(); itPlan != itEnd; ++itPlan)
	{
		TPlanManageInfo *pPlanInfo = itPlan->second;
		if(EMPlanManageRemove == pPlanInfo->emManage)
		{
			continue;
		}

		CXTPReportRecord* pRecord = m_wndPlanReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = NULL;

		// No.
		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nIdx, _T("%.0f")));

		// Name
		if(EMPlanManageNone == pPlanInfo->emManage)
		{
			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(pPlanInfo->strName));
		}
		else 
		{
			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(pPlanInfo->strNewName));
		}

		pItem->SetItemData((DWORD_PTR)pPlanInfo);

		nIdx++;
	}

	m_wndPlanReport.Populate();

	if(m_wndPlanReport.GetRecords()->GetCount() > 0)
	{
		m_wndPlanReport.GetRows()->GetAt(0)->SetSelected(TRUE);
	}
}


void KDevPlanMgrDlg::InitPlanDetail()
{
	int nCurSel = m_cboPlanType.AddString( _T("---- select ----"));
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeNone);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeNone, nCurSel));
	
	nCurSel = m_cboPlanType.AddString( TYPE_NAME_RESIDENTIAL);
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeResidential);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeResidential, nCurSel));

	nCurSel = m_cboPlanType.AddString( TYPE_NAME_INDUSTRY);
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeIndustry);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeIndustry, nCurSel));

	nCurSel = m_cboPlanType.AddString( TYPE_NAME_COMMERCE);
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeCommerce);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeCommerce, nCurSel));

	nCurSel = m_cboPlanType.AddString( TYPE_NAME_GREENS);
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeGreens);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeGreens, nCurSel));

	nCurSel = m_cboPlanType.AddString( TYPE_NAME_OTHERS);
	m_cboPlanType.SetItemData(nCurSel, EMDevTypeOthers);
	m_mapPlanTypeCursor.insert(std::make_pair(EMDevTypeOthers, nCurSel));

	InitLocationList();
	InitAnnualPlanList();
	InitUseFacilityList();
}


void KDevPlanMgrDlg::InitLocationList()
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndLocReport.AddColumn(new CXTPReportColumn(_0_LOC_COLUMN_ZONEID, _T("Zone ID"), 50)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	pColumn = m_wndLocReport.AddColumn(new CXTPReportColumn(_1_LOC_COLUMN_RATE, _T("Rate(%)"), 50)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_CENTER);

	m_wndLocReport.Populate();
}


void KDevPlanMgrDlg::InitAnnualPlanList()
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndAnnualPlanReport.AddColumn(new CXTPReportColumn(_0_ANNUAL_COLUMN_YEAR, _T("Year"), 50)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_CENTER);

	pColumn = m_wndAnnualPlanReport.AddColumn(new CXTPReportColumn(_1_ANNUAL_COLUMN_RATE, _T("Rate(%)"), 50)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_CENTER);

	m_wndAnnualPlanReport.Populate();
}


void KDevPlanMgrDlg::InitUseFacilityList()
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndFacReport.AddColumn(new CXTPReportColumn(_0_FAC_COLUMN_TYPE, _T("Type"), 60)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_LEFT);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_wndFacReport.AddColumn(new CXTPReportColumn(_1_FAC_COLUMN_SIZE, _T("Size"), 40)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_CENTER);

	m_wndFacReport.Populate();
}



void KDevPlanMgrDlg::UpdatePlanDetail()
{
	TPlanManageInfo* pPlanInfo = NULL;
	pPlanInfo = GetCurPlanInfo();

	//ASSERT(NULL != pPlanInfo);
	if(NULL == pPlanInfo)
	{
		return;
	}
	
	// Plan name
	CString strPlanName(_T(""));
	if(EMPlanManageNone == pPlanInfo->emManage)
		strPlanName.Format(_T("Plan Name: %s"), pPlanInfo->strName);
	else
		strPlanName.Format(_T("Plan Name: %s"), pPlanInfo->strNewName);

	SetDlgItemText(IDC_STATIC_NAME, strPlanName);
	
	// completion year
	SetDlgItemInt (IDC_EDIT_YEAR,   pPlanInfo->nCompletionYear);

	// plan type
	std::map<int, int>::iterator iter = m_mapPlanTypeCursor.find(pPlanInfo->nType);
	if(iter != m_mapPlanTypeCursor.end())
	{
	   int nCboCursor =	iter->second;
	   m_cboPlanType.SetCurSel(nCboCursor);
	   UpdateData(FALSE);
	}

	// Location 
	DisplayDevZone();

	// Annual Plan
	DisplayDevAnnualPlan();

	// Use Facility
	DisplayDevUseFacility();
}


TPlanManageInfo* KDevPlanMgrDlg::GetCurPlanInfo()
{
	CXTPReportRow*   pPlanRow  = m_wndPlanReport.GetSelectedRows()->GetAt(0);

	if(NULL != pPlanRow)
	{
		CXTPReportRecord*     pPlanRecord = pPlanRow->GetRecord();
		CXTPReportRecordItem* pPlanItem   = pPlanRecord->GetItem(_1_LIST_COLUMN_NAME);

		return (TPlanManageInfo*)(pPlanItem->GetItemData());
	}

	return NULL;
}


void KDevPlanMgrDlg::OnBnClickedBtnAddlist()
{
	TPlanManageInfo* pNewPlanInfo = new TPlanManageInfo;
	pNewPlanInfo->emManage        = EMPlanManageNew;
	pNewPlanInfo->strNewName      = _T("");
	pNewPlanInfo->nxId            = ++m_nxMaxPlanId;
	pNewPlanInfo->nType           = EMDevTypeNone;
	pNewPlanInfo->nCompletionYear = 0;

	SetDevUseFacilityBase(pNewPlanInfo->mapDevFacility);

	m_mapPlanInfo.insert(std::make_pair(pNewPlanInfo->nxId, pNewPlanInfo));

	int nTotalRowCount = m_wndPlanReport.GetRows()->GetCount();

	CXTPReportRecord*     pRecord = m_wndPlanReport.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber( (double)(nTotalRowCount +1), _T("%.0f") ));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetItemData((DWORD_PTR)pNewPlanInfo);
	
	m_wndPlanReport.Populate();

	nTotalRowCount = m_wndPlanReport.GetRows()->GetCount();
	CXTPReportRow* pRow = m_wndPlanReport.GetRows()->GetAt(nTotalRowCount - 1);
	pRow->SetSelected(TRUE);
	m_wndPlanReport.SetFocusedRow(pRow);

	m_wndPlanReport.SetTopRow(nTotalRowCount - 1);

	UpdatePlanDetail();
}


void KDevPlanMgrDlg::OnBnClickedBtnDellist()
{
	CXTPReportRow* pPlanRow = m_wndPlanReport.GetSelectedRows()->GetAt(0);
	if(NULL == pPlanRow)
	{
		return;
	}

	int nIndex =  pPlanRow->GetIndex();

	CXTPReportRecord*     pPlanRecord  = pPlanRow->GetRecord();
	CXTPReportRecordItem* pPlanItem    = pPlanRecord->GetItem(_1_LIST_COLUMN_NAME);
	TPlanManageInfo*      pPlanInfo    = (TPlanManageInfo*)(pPlanItem->GetItemData());

	if(EMPlanManageNew == pPlanInfo->emManage)
	{

		//delete pPlanInfo;
		pPlanInfo->emManage = EMPlanManageRemove;
		m_wndPlanReport.GetRecords()->RemoveRecord(pPlanRecord);
		m_wndPlanReport.Populate();
	}
	else
	{

		pPlanInfo->emManage = EMPlanManageRemove;
		UpdatePlanList();
	}

	if(nIndex > 1)	// 선택 행을 삭제 한 후 focus는 이전 행으로 셋팅
	{

		CXTPReportRow* pRow = m_wndPlanReport.GetRows()->GetAt(nIndex - 1);
		pRow->SetSelected(TRUE);
		m_wndPlanReport.SetFocusedRow(pRow);
		m_wndPlanReport.SetTopRow(nIndex - 1);
	}
}


void KDevPlanMgrDlg::OnPlanSelChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	UpdatePlanDetail();
}


void KDevPlanMgrDlg::OnPlanValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CString strPlan = pItemNotify->pItem->GetCaption(NULL);

	TPlanManageInfo* pEditInfo = (TPlanManageInfo*)(pItemNotify->pItem->GetItemData());

	bool bDuplicate = false;

	std::map<Integer, TPlanManageInfo*>::iterator itPlan, itEnd = m_mapPlanInfo.end();
	for(itPlan = m_mapPlanInfo.begin(); itPlan != itEnd; ++itPlan)
	{
		TPlanManageInfo* pPlanInfo = itPlan->second;
		if(EMPlanManageNone == pPlanInfo->emManage)
		{
			if(pPlanInfo->strName.Compare(strPlan) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
		else if((EMPlanManageNew == pPlanInfo->emManage) || (EMPlanManageModify == pPlanInfo->emManage))
		{
			if(pPlanInfo->strNewName.Compare(strPlan) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
	}

	if(true == bDuplicate)
	{
		AfxMessageBox(_T("입력하신 이름의 Plan 명칭이 이미 존재 합니다.\n다시 입력 해 주세요."));
		XTP_REPORTRECORDITEM_ARGS itemArgs(&m_wndPlanReport, pItemNotify->pRow, pItemNotify->pColumn);
		m_wndPlanReport.EditItem(&itemArgs);
		CXTPReportInplaceEdit* pEdit = m_wndPlanReport.GetInplaceEdit();
		if(NULL != pEdit)
		{
			pEdit->SetSel(0, -1);
		}
	}
	else
	{
		strPlan.Trim();

		pEditInfo->strNewName = strPlan;
		if(EMPlanManageNone == pEditInfo->emManage)
		{
			pEditInfo->emManage = EMPlanManageModify;
			pEditInfo->setModifyMode.insert(EMModifyModePlanName);
		}
	}
}


void KDevPlanMgrDlg::OnCbnSelchangeCboPlanType()
{
	TPlanManageInfo* pPlanInfo = NULL;
	pPlanInfo = GetCurPlanInfo();

	/*ASSERT(NULL != pPlanInfo);*/
	if( NULL == pPlanInfo )
	{
		return;
	}

	int nCurSel = m_cboPlanType.GetCurSel();
	pPlanInfo->nType = (int)m_cboPlanType.GetItemData(nCurSel);

	pPlanInfo->setModifyMode.insert(EMModifyModePlanType);
}


void KDevPlanMgrDlg::OnEnChangeEditYear()
{
	TPlanManageInfo* pPlanInfo = NULL;
	pPlanInfo = GetCurPlanInfo();

	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	pPlanInfo->nCompletionYear = GetDlgItemInt(IDC_EDIT_YEAR);

	pPlanInfo->setModifyMode.insert(EMModifyModeCompletionYear);
}


void KDevPlanMgrDlg::OnBnClickedBtnSelzone()
{
	TPlanManageInfo* pPlanInfo = NULL;
	pPlanInfo = GetCurPlanInfo();

	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<Integer, TDevLocation> &mapDevLocation = pPlanInfo->mapDevLocation;

	std::map<Integer, TDevLocation>::iterator iter = mapDevLocation.begin();
	std::map<Integer, TDevLocation>::iterator end  = mapDevLocation.end();
	std::vector<Integer> vecZoneID;
	while(iter != end)
	{
		vecZoneID.push_back(iter->first);
		++iter;
	}

	KDevZoneSelectDlg oZoneSelectDlg;
	oZoneSelectDlg.SetTarget(m_pTarget);
	oZoneSelectDlg.SetInitialSelectedZone(vecZoneID);

	if(oZoneSelectDlg.DoModal() != IDOK)
	{
		return;
	}

	iter = mapDevLocation.begin();
	end  = mapDevLocation.end();

	vecZoneID = oZoneSelectDlg.GetSelectedZone();

	std::set<Integer> setSelectedZone;
	// 더하기
	size_t nxCount = vecZoneID.size();
	for(size_t i= 0; i< nxCount; i++)
	{
		iter = mapDevLocation.find(vecZoneID[i]);
		if(iter == end)
		{
			TDevLocation oTDevLocation;
			oTDevLocation.nxZoneID = vecZoneID[i];

			mapDevLocation.insert(std::make_pair(vecZoneID[i], oTDevLocation));
		}

		setSelectedZone.insert(vecZoneID[i]);
	}
	//빼기
	std::set<Integer>::iterator iterSelectedZone = setSelectedZone.begin();
	std::set<Integer>::iterator endSelectedZone  = setSelectedZone.end();

	iter = mapDevLocation.begin();
	end  = mapDevLocation.end();

	while(iter != end)
	{
		Integer nxZoneID = iter->first;
		iterSelectedZone = setSelectedZone.find(nxZoneID);
		if (iterSelectedZone == endSelectedZone)
		{
			mapDevLocation.erase(iter);
			iter = mapDevLocation.begin();
			end  = mapDevLocation.end();
		}
		else
		{
			++iter;
		}
	}

	// default rate 할당
	iter = mapDevLocation.begin();
	end  = mapDevLocation.end();

	size_t idx = 0;
	double dRate = (int)(100.0 / nxCount);

	while(iter != end)
	{
		TDevLocation &oTDevLocation = iter->second;

		if(idx == nxCount -1)
		{
			oTDevLocation.dRate = 100.0 - dRate*idx;
		}
		else
		{
			oTDevLocation.dRate = dRate;
		}

		++idx;
		++iter;
	}

	DisplayDevZone();

	pPlanInfo->setModifyMode.insert(EMModifyModeLocation);
}


void KDevPlanMgrDlg::DisplayDevZone()
{
	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();

	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<Integer, TDevLocation> &mapDevLocation = pPlanInfo->mapDevLocation;

	std::map<Integer, TDevLocation>::iterator iter = mapDevLocation.begin();
	std::map<Integer, TDevLocation>::iterator end  = mapDevLocation.end();

	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pRecordItem   = NULL;
	CString               strTemp       = _T("");
	
	m_wndLocReport.ResetContent(FALSE);
		
	while (iter != end)
	{
		Integer       nxZoneID      = iter->first;
		TDevLocation &oTDevLocation = iter->second;

		strTemp.Format(_T("%I64d"), nxZoneID);

		pRecord = m_wndLocReport.AddRecord(new CXTPReportRecord());
		
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pRecordItem->SetItemData((DWORD_PTR)nxZoneID);
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);

		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTDevLocation.dRate, _T("%.2f")));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(TRUE);

		++iter;
	}
	
	m_wndLocReport.Populate();
}


void KDevPlanMgrDlg::OnLocValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM*    pItemNotify  = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	
	CXTPReportRecord*           pRecord      = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItemNumber* pItemZoneID  = (CXTPReportRecordItemNumber*)pRecord->GetItem(_0_LOC_COLUMN_ZONEID); 
	CXTPReportRecordItemNumber* pItemRate    = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_LOC_COLUMN_RATE);
	
	Integer nxZoneID = (Integer)pItemZoneID->GetItemData(); 
	double  dRate    = pItemRate->GetValue();

	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<Integer, TDevLocation> &mapDevLocation = pPlanInfo->mapDevLocation;
	std::map<Integer, TDevLocation>::iterator iter  = mapDevLocation.find(nxZoneID);

	if(iter == mapDevLocation.end())
	{
		return;
	}

	TDevLocation &oTDevLocation = iter->second;
	oTDevLocation.dRate         = dRate;

	pPlanInfo->setModifyMode.insert(EMModifyModeLocation);
}


void KDevPlanMgrDlg::DisplayDevAnnualPlan()
{
	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, TDevAnnualPlan> &mapAnnualPlan = pPlanInfo->mapDevAnnualPlan;

	std::map<int, TDevAnnualPlan>::iterator iter = mapAnnualPlan.begin();
	std::map<int, TDevAnnualPlan>::iterator end  = mapAnnualPlan.end();

	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pRecordItem   = NULL;

	m_wndAnnualPlanReport.ResetContent(FALSE);

	while (iter != end)
	{
		/*int            nYear            = iter->first;*/
		TDevAnnualPlan &oTDevAnnualPaln = iter->second;

		pRecord = m_wndAnnualPlanReport.AddRecord(new CXTPReportRecord());

		pRecordItem = pRecord->AddItem( new CXTPReportRecordItemNumber((double)oTDevAnnualPaln.nYear, _T("%.0f")) );
		pRecordItem->SetItemData((DWORD_PTR)oTDevAnnualPaln.nTempID);
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(TRUE);

		pRecordItem = pRecord->AddItem( new CXTPReportRecordItemNumber(oTDevAnnualPaln.dRate, _T("%.2f")) );
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(TRUE);

		++iter;
	}

	m_wndAnnualPlanReport.Populate();
}


void KDevPlanMgrDlg::OnAnnualValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM*    pItemNotify  = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	CXTPReportRecord*           pRecord      = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItemNumber* pItemYear    = (CXTPReportRecordItemNumber*)pRecord->GetItem(_0_ANNUAL_COLUMN_YEAR); 
	CXTPReportRecordItemNumber* pItemRate    = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_ANNUAL_COLUMN_RATE);

	int    nIndex  = pItemNotify->pColumn->GetItemIndex();
	int    nTempID = (int)pItemYear->GetItemData(); 
	int    nYear   = (int)pItemYear->GetValue();
	double dRate   = pItemRate->GetValue();

	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, TDevAnnualPlan> &mapDevAnnualPlan = pPlanInfo->mapDevAnnualPlan;
	std::map<int, TDevAnnualPlan>::iterator iter    = mapDevAnnualPlan.find(nTempID);

	if(iter == mapDevAnnualPlan.end())
	{
		return;
	}

	TDevAnnualPlan &oCurDevAnnualPlan    = iter->second;

	if( _0_ANNUAL_COLUMN_YEAR == nIndex )
	{
		bool bDuplicate = false;

		std::map<int, TDevAnnualPlan>::iterator itAnnual, itEnd = mapDevAnnualPlan.end();
		for(itAnnual = mapDevAnnualPlan.begin(); itAnnual != itEnd; ++itAnnual)
		{
			TDevAnnualPlan &oTAnnualPlanInfo = itAnnual->second;

			if(oTAnnualPlanInfo.nYear == nYear)
			{
				bDuplicate = true;
				break;
			}
		}

		if(true == bDuplicate)
		{
			AfxMessageBox(_T("입력하신 년도가 이미 존재 합니다.\n다시 입력 해 주세요."));
			XTP_REPORTRECORDITEM_ARGS itemArgs(&m_wndAnnualPlanReport, pItemNotify->pRow, pItemNotify->pColumn);
			m_wndAnnualPlanReport.EditItem(&itemArgs);
			CXTPReportInplaceEdit* pEdit = m_wndAnnualPlanReport.GetInplaceEdit();
			if(NULL != pEdit)
			{
				pEdit->SetSel(0, -1);
			}
		}

		oCurDevAnnualPlan.nYear = nYear;
	}
	else if(_1_ANNUAL_COLUMN_RATE == nIndex)
	{
		oCurDevAnnualPlan.dRate = dRate;
	}

	pPlanInfo->setModifyMode.insert(EMModifyModeAnnualPlan);
}


void KDevPlanMgrDlg::OnBnClickedBtnAddannual()
{
	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, TDevAnnualPlan> &mapDevAnnualPlan = pPlanInfo->mapDevAnnualPlan;

	std::map<int, TDevAnnualPlan>::iterator iter = mapDevAnnualPlan.begin();
	std::map<int, TDevAnnualPlan>::iterator end  = mapDevAnnualPlan.end();

	int nMaxTempID = -1;
	while( iter != end )
	{
		int nTempID = iter->first;

		if(nTempID > nMaxTempID)
		{
			nMaxTempID = nTempID;
		}

		++iter;
	}
	nMaxTempID++;

	TDevAnnualPlan oTDevAnnualPlan;
	oTDevAnnualPlan.nTempID = nMaxTempID;
	oTDevAnnualPlan.nYear   = 0;
	oTDevAnnualPlan.dRate   = 0.0;

	mapDevAnnualPlan.insert(std::make_pair(oTDevAnnualPlan.nTempID, oTDevAnnualPlan));

	// default rate 할당
	iter = mapDevAnnualPlan.begin();
	end  = mapDevAnnualPlan.end();

	size_t nxCount = mapDevAnnualPlan.size();
	size_t idx     = 0;
	double dRate = (int)(100.0 / nxCount);

	while(iter != end)
	{
		TDevAnnualPlan &oTDevAnnualPlan = iter->second;

		if(idx == nxCount -1)
		{
			oTDevAnnualPlan.dRate = 100.0 - dRate*idx;
		}
		else
		{
			oTDevAnnualPlan.dRate = dRate;
		}

		++idx;
		++iter;
	}

	DisplayDevAnnualPlan();

	int nTotalRowCount  = m_wndAnnualPlanReport.GetRows()->GetCount();
	CXTPReportRow* pRow = m_wndAnnualPlanReport.GetRows()->GetAt(nTotalRowCount - 1);
	pRow->SetSelected(TRUE);
	m_wndAnnualPlanReport.SetFocusedRow(pRow);

	m_wndAnnualPlanReport.SetTopRow(nTotalRowCount - 1);

	pPlanInfo->setModifyMode.insert(EMModifyModeAnnualPlan);
}


void KDevPlanMgrDlg::OnBnClickedBtnDelannual()
{
	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, TDevAnnualPlan> &mapDevAnnualPlan = pPlanInfo->mapDevAnnualPlan;
		
	CXTPReportRow* pAnnualRow = m_wndAnnualPlanReport.GetSelectedRows()->GetAt(0);
	if(NULL == pAnnualRow)
	{
		return;
	}

	int nIndex = pAnnualRow->GetIndex();

	CXTPReportRecord*     pAnnualRecord = pAnnualRow->GetRecord();
	CXTPReportRecordItem* pItemYear     = pAnnualRecord->GetItem(_0_ANNUAL_COLUMN_YEAR);
	int                   nTempID       = (int)pItemYear->GetItemData(); 

	std::map<int, TDevAnnualPlan>::iterator iterDel = mapDevAnnualPlan.find(nTempID);

	m_wndAnnualPlanReport.GetRecords()->RemoveRecord(pAnnualRecord);
	m_wndAnnualPlanReport.Populate();

	if( iterDel != mapDevAnnualPlan.end() )
	{
		mapDevAnnualPlan.erase(iterDel);
	}

	// default rate 할당
	std::map<int, TDevAnnualPlan>::iterator itbegin = mapDevAnnualPlan.begin();
	std::map<int, TDevAnnualPlan>::iterator itend   = mapDevAnnualPlan.end();

	size_t nxCount = mapDevAnnualPlan.size();
	size_t idx     = 0;
	double dRate = (int)(100.0 / nxCount);

	while(itbegin != itend)
	{
		TDevAnnualPlan &oTDevAnnualPlan = itbegin->second;

		if(idx == nxCount -1)
		{
			oTDevAnnualPlan.dRate = 100.0 - dRate*idx;
		}
		else
		{
			oTDevAnnualPlan.dRate = dRate;
		}

		++idx;
		++itbegin;
	}

	DisplayDevAnnualPlan();

	if(nIndex > 1)	// 선택 행을 삭제 한 후 focus는 이전 행으로 셋팅
	{

		CXTPReportRow* pRow = m_wndAnnualPlanReport.GetRows()->GetAt(nIndex - 1);
		pRow->SetSelected(TRUE);
		m_wndAnnualPlanReport.SetFocusedRow(pRow);
		m_wndAnnualPlanReport.SetTopRow(nIndex - 1);
	}

	pPlanInfo->setModifyMode.insert(EMModifyModeAnnualPlan);
}


void KDevPlanMgrDlg::DisplayDevUseFacility()
{
	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, std::map<int, TDevUseFacility>> &mapDevFacility = pPlanInfo->mapDevFacility;

	std::map<int, std::map<int, TDevUseFacility>>::iterator itMain    = mapDevFacility.begin();
	std::map<int, std::map<int, TDevUseFacility>>::iterator itMainEnd = mapDevFacility.end();

	m_wndFacReport.ResetContent(FALSE);

	while( itMain != itMainEnd )
	{
		CString strRouteName = GetFacCodeName(itMain->first);

		CXTPReportRecord*      pParentRecord = m_wndFacReport.AddRecord(new CXTPReportRecord());
		CXTPReportRecordItem * pParentItem   = pParentRecord->AddItem(new CXTPReportRecordItemText(strRouteName));
											   pParentRecord->AddItem(new CXTPReportRecordItem());

		CXTPReportRecords*     pRecords = pParentRecord->GetChilds();
		CXTPReportRecord*      pRecord = NULL;
		CXTPReportRecordItem*  pItem   = NULL;
		
		std::map<int, TDevUseFacility> &mapPlanType = itMain->second;

		std::map<int, TDevUseFacility>::iterator itSub    = mapPlanType.begin();
		std::map<int, TDevUseFacility>::iterator itSubEnd = mapPlanType.end();

		while( itSub != itSubEnd )
		{
			pRecord = pRecords->Add(new CXTPReportRecord());

			CString strSubName = GetPlanIndexCodeName(itSub->first);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText( strSubName ));
			pItem->SetAlignment(xtpColumnTextLeft);
			pItem->SetItemData((DWORD_PTR)itMain->first);

			TDevUseFacility &oTDevUseFacility = itSub->second;

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oTDevUseFacility.dSize,  _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetItemData((DWORD_PTR)itSub->first);

			++itSub;
		}

		++itMain;
	}

	m_wndFacReport.Populate();
	m_wndFacReport.ExpandAll(TRUE);
}


void KDevPlanMgrDlg::OnFACValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM*    pItemNotify  = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	CXTPReportRecord*           pRecord      = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem*       pItemType    = (CXTPReportRecordItemNumber*)pRecord->GetItem(_0_FAC_COLUMN_TYPE); 
	CXTPReportRecordItemNumber* pItemSize    = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_FAC_COLUMN_SIZE);


	int    nIndex  = pItemNotify->pColumn->GetItemIndex();
	int    nFacilityType = (int)pItemType->GetItemData(); 
	int    nPlanIndex    = (int)pItemSize->GetItemData();
	double dSize         = pItemSize->GetValue();

	TPlanManageInfo* pPlanInfo = GetCurPlanInfo();
	/*ASSERT(NULL != pPlanInfo);*/

	if( NULL == pPlanInfo )
	{
		return;
	}

	std::map<int, std::map<int, TDevUseFacility>> &mapDevFacility    = pPlanInfo->mapDevFacility;
	std::map<int, std::map<int, TDevUseFacility>>::iterator iterMain = mapDevFacility.find(nFacilityType);

	if(iterMain == mapDevFacility.end())
	{
		return;
	}

	std::map<int, TDevUseFacility> &mapPlanType      = iterMain->second;
	std::map<int, TDevUseFacility>::iterator iterSub = mapPlanType.find(nPlanIndex);

	if(iterSub == mapPlanType.end())
	{
		return;
	}

	TDevUseFacility &oTDevUseFacility = iterSub->second;
	oTDevUseFacility.dSize = dSize;

	pPlanInfo->setModifyMode.insert(EMModifyModeFacility);
}


CString KDevPlanMgrDlg::GetFacCodeName(int a_nFacilityType)
{
	CString strName(_T(""));

	std::map<int, CString>::iterator itFacType = m_mapFacNameCode.find(a_nFacilityType);
	if(itFacType != m_mapFacNameCode.end())
	{
		return itFacType->second;
	}

	return strName;
}


CString KDevPlanMgrDlg::GetPlanIndexCodeName(int a_nPlanIndex)
{
	CString strName(_T(""));

	std::map<int, CString>::iterator itIndex = m_mapPlanIndexNameCode.find(a_nPlanIndex);
	if(itIndex != m_mapPlanIndexNameCode.end())
	{
		return itIndex->second;
	}

	return strName;
}


void KDevPlanMgrDlg::OnBnClickedOk()
{
	CString strWrongMsg(_T(""));

	if( !InputCheck(strWrongMsg) )
	{
		AfxMessageBox(strWrongMsg);
		return ;
	}

	UpsertDevPlan();
	UpsertDevLocation();
	UpsertDevAnnualPlan();
	UpsertDevUseFacility(); 

	KResizeDialogEx::OnOK();
}


bool KDevPlanMgrDlg::InputCheck(CString &a_strMsg)
{
	CXTPReportRows*       pRows   = m_wndPlanReport.GetRows();
	CXTPReportRow*        pRow    = NULL;
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL; 

	for(int i= 0; i< pRows->GetCount(); i++)
	{
		pRow    = pRows->GetAt(i);
		pRecord = pRow->GetRecord();
		pItem   = pRecord->GetItem(_1_LIST_COLUMN_NAME);

		TPlanManageInfo* pManageInfo = (TPlanManageInfo*)(pItem->GetItemData()); 

		bool bWrongInput = false;

		CString strPlanName = pItem->GetCaption(NULL);
		strPlanName.Trim();
		if( !bWrongInput && strPlanName.Compare(_T("")) == 0 )
		{
			bWrongInput = true;
			a_strMsg = _T("WRONG 'Name'\r\nPlan Name 을 입력 해 주세요.");
		}

		if( !bWrongInput && EMDevTypeNone == pManageInfo->nType )
		{
			bWrongInput = true;
			a_strMsg = _T("WRONG 'Type'\r\nPlan Type 을 선택 해 주세요.");
		}

		if( !bWrongInput && (pManageInfo->nCompletionYear < 1900 ||  pManageInfo->nCompletionYear > 2999))
		{
			bWrongInput = true;
			a_strMsg = _T("WRONG 'Completion Year'\r\n년도 설정을 올바르게 해 주세요. (1900 ~ 2999)");
		}

		if( !bWrongInput && pManageInfo->mapDevLocation.size() < 1 )
		{
			bWrongInput = true;
			a_strMsg = _T("WRONG 'Location'\r\nZone 을 선택 해 주세요.");
		}

		if( !bWrongInput )
		{
			std::map<Integer, TDevLocation> &mapDevLocation = pManageInfo->mapDevLocation;
			std::map<Integer, TDevLocation>::iterator  iter = mapDevLocation.begin();
			
			double dSum = 0.0;

			while(iter != mapDevLocation.end())
			{
				TDevLocation &oTDevLocation = iter->second;
				dSum += oTDevLocation.dRate;
				++iter;
			}

			if( dSum != 100.0 )
			{
				bWrongInput = true;
				a_strMsg = _T("WRONG 'Location'\r\nRate의 합을 100(%)으로 맞춰 주세요.");
			}
		}

		if( !bWrongInput && pManageInfo->mapDevAnnualPlan.size() < 1 )
		{
			bWrongInput = true;
			a_strMsg = _T("WRONG 'Annual Plan of Resident'\r\n년도 를 추가 해 주세요.");
		}

		if( !bWrongInput )
		{
			std::map<int, TDevAnnualPlan> &mapDevAnnualPlan = pManageInfo->mapDevAnnualPlan;
			std::map<int, TDevAnnualPlan>::iterator  iter   = mapDevAnnualPlan.begin();

			double dSum = 0.0;
			std::set<int> setYear;

			while(iter != mapDevAnnualPlan.end())
			{
				TDevAnnualPlan &oTDevAnnualPlan = iter->second;

				dSum += oTDevAnnualPlan.dRate;
				setYear.insert(oTDevAnnualPlan.nYear);

				++iter;
			}

			if( dSum != 100.0 )
			{
				bWrongInput = true;
				a_strMsg = _T("WRONG 'Annual Plan of Resident'\r\nRate의 합을 100(%)으로 맞춰 주세요.");
			}

			if( setYear.size() != mapDevAnnualPlan.size() )
			{
				bWrongInput = true;
				a_strMsg = _T("WRONG 'Annual Plan of Resident'\r\n중복된 Annual Plan 년도가 존재 합니다.");
			}
		}

		if(bWrongInput)
		{
			pRow->SetSelected(TRUE);
			m_wndPlanReport.SetFocusedRow(pRow);
			m_wndPlanReport.SetTopRow(i);

			UpdatePlanDetail();
			return false;
		}
	}

	return true;
}


void KDevPlanMgrDlg::UpsertDevPlan()
{
	// Delete Sql
	CString strSQLDelete(_T(" Delete From dev_plan Where plan_id = ? "));

	// Insert Sql
	CString strSQLInsert(_T(" Insert Into dev_plan "));
	strSQLInsert +=      _T(" ( plan_id, plan_name, plan_type, completion_year ");  
	strSQLInsert +=      _T(" ) Values ( ?, ?, ?, ? ) ");

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{  
		spDBaseConnection->BeginTransaction();

		// Delete 
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLDelete);
		
		std::map<Integer, TPlanManageInfo*>::iterator iter, itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool bDelete = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageNew == emManage )
			{
				continue;
			}
			else if( EMPlanManageModify == emManage || EMPlanManageRemove == emManage )
			{
				bDelete = true;
			}
			else if( EMPlanManageNone == emManage )
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModePlanType) != setModifyMode.end() )
				{
					bDelete = true;
				}

				if( setModifyMode.find(EMModifyModeCompletionYear) != setModifyMode.end() )
				{
					bDelete = true;
				}
			}

			if( !bDelete)
			{
				continue;
			}

			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		// Insert
		spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);
				
		itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool             bInsert         = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			
			Integer          nxPlanID        = iter->first;
			CString          strName         = pPlanManageInfo->strName;
			int             &nType           = pPlanManageInfo->nType;
			int             &nCompletionYear = pPlanManageInfo->nCompletionYear;

			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageRemove == emManage )
			{
				continue;
			}
			else if( EMPlanManageNew == emManage || EMPlanManageModify == emManage )
			{
				strName = pPlanManageInfo->strNewName;
				bInsert = true;
			}
			else if( EMPlanManageNone == emManage )
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModePlanType) != setModifyMode.end() )
				{
					bInsert = true;
				}

				if( setModifyMode.find(EMModifyModeCompletionYear) != setModifyMode.end() )
				{
					bInsert = true;
				}
			}

			if( !bInsert)
			{
				continue;
			}

			spPrepareStatement->BindInt64(1, nxPlanID);
			spPrepareStatement->BindText (2, strName);
			spPrepareStatement->BindInt  (3, nType);
			spPrepareStatement->BindInt  (4, nCompletionYear);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}    

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return;
}


void KDevPlanMgrDlg::UpsertDevLocation()
{
	// Delete Sql
	CString strSQLDelete(_T(" Delete From dev_location Where plan_id = ? "));

	// Insert Sql
	CString strSQLInsert(_T(" Insert Into dev_location "));
	strSQLInsert +=      _T(" ( plan_id, zone_id, ratio ");  
	strSQLInsert +=      _T(" ) Values ( ?, ?, ? ) ");

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();  
	try
	{  
		spDBaseConnection->BeginTransaction();

		// Delete 
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, TPlanManageInfo*>::iterator iter, itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool bDelete = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageNew == emManage )
			{
				continue;
			}
			else if( EMPlanManageRemove == emManage )
			{
				bDelete = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage)
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeLocation) != setModifyMode.end() )
				{
					bDelete = true;
				}
			}

			if( !bDelete)
			{
				continue;
			}

			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		// Insert
		spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool             bInsert         = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageRemove == emManage )
			{
				continue;
			}
			else if( EMPlanManageNew == emManage )
			{
				bInsert = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage )
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeLocation) != setModifyMode.end() )
				{
					bInsert = true;
				}
			}

			if( !bInsert)
			{
				continue;
			}

			Integer nxPlanID = iter->first;
			std::map<Integer, TDevLocation> &mapDevLocation = pPlanManageInfo->mapDevLocation;
			std::map<Integer, TDevLocation>::iterator itLoc = mapDevLocation.begin();

			while( itLoc != mapDevLocation.end() )
			{
				TDevLocation &oTDevLocation = itLoc->second;

				spPrepareStatement->BindInt64 (1, nxPlanID);
				spPrepareStatement->BindInt64 (2, itLoc->first );
				spPrepareStatement->BindDouble(3, oTDevLocation.dRate);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++itLoc;
			}
		}    

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return;
}


void KDevPlanMgrDlg::UpsertDevAnnualPlan()
{
	// Delete Sql
	CString strSQLDelete(_T(" Delete From dev_annual_plan_resident Where plan_id = ? "));

	// Insert Sql
	CString strSQLInsert(_T(" Insert Into dev_annual_plan_resident "));
	strSQLInsert +=      _T(" ( plan_id, year, ratio ");  
	strSQLInsert +=      _T(" ) Values ( ?, ?, ? ) ");

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{  
		spDBaseConnection->BeginTransaction();

		// Delete 
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, TPlanManageInfo*>::iterator iter, itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool bDelete = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageNew == emManage )
			{
				continue;
			}
			else if( EMPlanManageRemove == emManage )
			{
				bDelete = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage)
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeAnnualPlan) != setModifyMode.end() )
				{
					bDelete = true;
				}
			}

			if( !bDelete)
			{
				continue;
			}

			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		// Insert
		spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool             bInsert         = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageRemove == emManage )
			{
				continue;
			}
			else if( EMPlanManageNew == emManage )
			{
				bInsert = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage )
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeAnnualPlan) != setModifyMode.end() )
				{
					bInsert = true;
				}
			}

			if( !bInsert)
			{
				continue;
			}

			Integer nxPlanID = iter->first;
			std::map<int, TDevAnnualPlan> &mapDevAnnualPlan  = pPlanManageInfo->mapDevAnnualPlan;
			std::map<int, TDevAnnualPlan>::iterator itAnnual = mapDevAnnualPlan.begin();

			while( itAnnual != mapDevAnnualPlan.end() )
			{
				TDevAnnualPlan &oTDevAnnualPlan = itAnnual->second;

				spPrepareStatement->BindInt64 (1, nxPlanID);
				spPrepareStatement->BindInt   (2, oTDevAnnualPlan.nYear);
				spPrepareStatement->BindDouble(3, oTDevAnnualPlan.dRate);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++itAnnual;
			}
		}    

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return;
}


void KDevPlanMgrDlg::UpsertDevUseFacility()
{
	// Delete Sql
	CString strSQLDelete(_T(" Delete From dev_use_facility Where plan_id = ? "));

	// Insert Sql
	CString strSQLInsert(_T(" Insert Into dev_use_facility "));
	strSQLInsert +=      _T(" ( plan_id, facility_type, plan_type, size ");  
	strSQLInsert +=      _T(" ) Values ( ?, ?, ?, ? ) ");

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{  
		spDBaseConnection->BeginTransaction();

		// Delete 
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, TPlanManageInfo*>::iterator iter, itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool bDelete = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageNew == emManage )
			{
				continue;
			}
			else if( EMPlanManageRemove == emManage )
			{
				bDelete = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage)
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeFacility) != setModifyMode.end() )
				{
					bDelete = true;
				}
			}

			if( !bDelete)
			{
				continue;
			}

			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		// Insert
		spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		itEnd = m_mapPlanInfo.end();
		for(iter = m_mapPlanInfo.begin(); iter != itEnd; ++iter)
		{
			bool             bInsert         = false;
			TPlanManageInfo* pPlanManageInfo = iter->second;
			EMPlanManage    &emManage        = pPlanManageInfo->emManage;

			if( EMPlanManageRemove == emManage )
			{
				continue;
			}
			else if( EMPlanManageNew == emManage )
			{
				bInsert = true;
			}
			else if( EMPlanManageNone == emManage || EMPlanManageModify == emManage )
			{
				std::set<EMModifyMode> &setModifyMode = pPlanManageInfo->setModifyMode; 

				if( setModifyMode.find(EMModifyModeFacility) != setModifyMode.end() )
				{
					bInsert = true;
				}
			}

			if( !bInsert)
			{
				continue;
			}

			Integer nxPlanID = iter->first;
			std::map<int, std::map<int, TDevUseFacility>> &mapDevFacility = pPlanManageInfo->mapDevFacility;
			std::map<int, std::map<int, TDevUseFacility>>::iterator itFac = mapDevFacility.begin();

			while( itFac != mapDevFacility.end() )
			{
				int nFacilityType = itFac->first;
				std::map<int, TDevUseFacility> &mapPlanIndex       = itFac->second;
				std::map<int, TDevUseFacility>::iterator itPlanIdx = mapPlanIndex.begin();

				while( itPlanIdx != mapPlanIndex.end() )
				{
					int nPlanIndex = itPlanIdx->first;
					TDevUseFacility &oTDevUseFacility = itPlanIdx->second;

					spPrepareStatement->BindInt64 (1, nxPlanID);
					spPrepareStatement->BindInt   (2, nFacilityType);
					spPrepareStatement->BindInt   (3, nPlanIndex);
					spPrepareStatement->BindDouble(4, oTDevUseFacility.dSize);

					spPrepareStatement->ExecuteUpdate();
					spPrepareStatement->Reset();

					++itPlanIdx;
				}
				
				++itFac;
			}
		}    

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return;
}