// GenerationBusPathStep2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationBusPathStep2Dlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Target.h"
#include "KExRecordItem.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "Launcher.h"
#include "WriteCodeAgrument.h"
#include "ImChampDir.h"

enum
{
	_0_ACOLUMN_FUNCTION		= 0,
	_1_ACOLUMN_BUS			= 1,
	_2_ACOLUMN_SUBWAY		= 2,
	_3_ACOLUMN_BUSSUBWAY	= 3
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
	FUNCTION_CASE_BUS    = 1,
	FUNCTION_CASE_SUBWAY = 2
};

const int DEFAULT_MAXROUTES             = 100;
const int TRANSIT_TYPE_BUS              = 3;
const int TRANSIT_TYPE_SUBWAY           = 4;
const int TRANSTI_TYPE_BUSSUBWAY		= 5;
const int CODE_GROUP_KEY_TRANSIT_DETAIL = 114;
const int DECIMAL_PLACES                = 10;


// KGenerationBusPathStep2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationBusPathStep2Dlg, KResizeDialogEx)

	KGenerationBusPathStep2Dlg::KGenerationBusPathStep2Dlg(KProject* a_pProject, KTarget* a_pTarget, int a_nTimeMethod, std::list<TGenerationPathInfo> a_lstPathInfo,
	KIOColumn* a_pIOColumnUseLink, CWnd* pParent/*=NULL*/)
	: KResizeDialogEx(KGenerationBusPathStep2Dlg::IDD, pParent)
	, m_pProject(a_pProject)
	, m_pTarget(a_pTarget)
	, m_strOutputFolerPath(_T(""))
	, m_bRunResult(false)
	, m_nTimeMethod(a_nTimeMethod)
	, m_pIOColumnUseLink(a_pIOColumnUseLink)
	, m_lstGenerationpathInfo(a_lstPathInfo)
	, m_bUseBus(false)
	, m_bUseSubway(false)
	, m_bUseBusSubway(false)
{

}

KGenerationBusPathStep2Dlg::~KGenerationBusPathStep2Dlg()
{
}

void KGenerationBusPathStep2Dlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KGenerationBusPathStep2Dlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KGenerationBusPathStep2Dlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BUTTON1, &KGenerationBusPathStep2Dlg::OnBnClickedButtonBack)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &KGenerationBusPathStep2Dlg::OnBnClickedDefault)
END_MESSAGE_MAP()


// KGenerationBusPathStep2Dlg 메시지 처리기입니다.

BOOL KGenerationBusPathStep2Dlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(mapDefaultPathGenerateConstraint);
	KDBaseDefaultParaMultimodalPathGeneration::LoadDefaultMultimodalPathInfo(mapDefaultMultimodalPathInfo);

	AutoType iter = mapDefaultPathGenerateConstraint.find(1);
	AutoType end  = mapDefaultPathGenerateConstraint.end();

		if (iter != end)
		{
			DefaultConstraintConditions();
			DefultPathInfo();
		}
		else
		{
			InitSystemConstraintConditions();
			InitSystemPathInfo();
		}
				
		// default value
		std::vector<TTransitPathCost>      vecTransitPathCost;
		//LoadBusPathInput(oTBaseSetData);
		LoadAccessClassInfo(m_vecLoadAccessClassInfo);
		DefaultBusPathCost(vecTransitPathCost);
		
		KReportCtrlSetting::DefaultC(m_wndReportBCtrl, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportBCtrl);
		GetUseModeData();
		InitReportB();
		InitReportBRecord(vecTransitPathCost);
		ResizeComponent();

		m_strOutputFolerPath = ConvertSavePath(_T(""));

		m_argFilePath = ImChampDir::GetAppDirectory() + _T("\\x64\\GenerateTransitPath.arg");

	return TRUE;  
}


void KGenerationBusPathStep2Dlg::DefaultBusPathCost(std::vector<TTransitPathCost> &a_vecTransitPathCost)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	a_vecTransitPathCost.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT type, detail_type, TBL2.KCode_Display AS Detail_Display ")
			_T(" FROM  ")
			_T(" (SELECT type, detail_type FROM Transit GROUP BY type, detail_type) TBL1  ")
			_T(" LEFT OUTER JOIN  ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL2   ")
			_T(" ON TBL2.KCode_Key = TBL1.detail_type "), CODE_GROUP_KEY_TRANSIT_DETAIL);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TTransitPathCost oTTransitPathCost;
			
			int nTransitType                      = pResult->GetValueInt   (0);
			oTTransitPathCost.nTransitType        = nTransitType; 
			oTTransitPathCost.nTransitDetailType  = pResult->GetValueInt   (1);
			CString strDetailName                 = pResult->GetValueString(2);
			if( strDetailName.Compare(_T("")) == 0 )
			{
				strDetailName=_T("(x)");
			}
			oTTransitPathCost.strDetailName       = strDetailName;

			a_vecTransitPathCost.push_back(oTTransitPathCost);
		}

		int nCount = a_vecTransitPathCost.size();
		for (int i = 0 ; i < nCount; i++)
		{
			TTransitPathCost oTTransitPathCost = a_vecTransitPathCost[i];
			CString strCostSQL = _T("");
			strCostSQL.Format(_T("Select basic_rate, basic_distance, add_rate, add_distance, merge_rete From %s Where type = %d"), TABLE_TRANSIT_COST ,oTTransitPathCost.nTransitDetailType);
			KResultSetPtr pResultCost = spDBaseConnection->ExecuteQuery(strCostSQL);
			while(pResultCost->Next())
			{
				oTTransitPathCost.dBaseCost		= pResultCost->GetValueDouble(0);
				oTTransitPathCost.dBaseLength	= pResultCost->GetValueDouble(1);
				oTTransitPathCost.dAddCost		= pResultCost->GetValueDouble(2);
				oTTransitPathCost.dAddLength	= pResultCost->GetValueDouble(3);
				oTTransitPathCost.nCostMethod	= pResultCost->GetValueInt(4);
			}
			a_vecTransitPathCost[i] = oTTransitPathCost;
			//a_vecTransitPathCost.push_back(oTTransitPathCost);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::LoadBusPathInput(TBaseSetData &a_oTBaseSetData)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT Max_Routes, Time_Board, Wait_Vdf_Alpha, Wait_Vdf_Beta, DWellTime FROM %s "), TABLE_BUS_PATH_INPUT);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			a_oTBaseSetData.nMaxRouteCnt        = pResult->GetValueInt   (0);
			a_oTBaseSetData.dDwellTimeatBus     = pResult->GetValueDouble(1); 
			a_oTBaseSetData.dAlpha              = pResult->GetValueDouble(2);
			a_oTBaseSetData.dBeta               = pResult->GetValueDouble(3);
			a_oTBaseSetData.dDWellTimeatRail	= pResult->GetValueDouble(4);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::LoadAccessClassInfo(std::vector<TAccessClassInfo> &a_vecLoadAccessClassInfo)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	a_vecLoadAccessClassInfo.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT class_id, class_name, time_min  FROM %s ORDER BY class_id "), TABLE_ACCESS_CLASS);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TAccessClassInfo oTAccessClassInfo;

			int nClassID                   = pResult->GetValueInt   (0);
			oTAccessClassInfo.strClassName = pResult->GetValueString(1);
			oTAccessClassInfo.nTimeMin     = pResult->GetValueInt   (2);

			a_vecLoadAccessClassInfo.push_back(oTAccessClassInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::InitReportB()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_BCOLUMN_MODE, _T("Mode"),                25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입명"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_BCOLUMN_TYPE, _T("Type"),                18));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_BCOLUMN_BASE_COST, _T("Base Fare"),        15));
	pColumn->SetTooltip(_T("Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기본운임"));
		pColumn->SetTooltip(_T("기본운임"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_BCOLUMN_BASE_DISTANCE, _T("Base Dist"),    15));
	pColumn->SetTooltip(_T("Base Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("~Km까지"));
		pColumn->SetTooltip(_T("~Km까지"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_BCOLUMN_ADD_COST, _T("Dist-Base Fare"),         20));
	pColumn->SetTooltip(_T("Dist-Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("추가운임"));
		pColumn->SetTooltip(_T("추가운임"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_5_BCOLUMN_ADD_DISTANCE, _T("Unit Dist"),     15));
	pColumn->SetTooltip(_T("Uint Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("매 ~Km마다"));
		pColumn->SetTooltip(_T("매 ~Km마다"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_6_BCOLUMN_CHECK_INTERGRATE, _T("D.F.P"),  15));
	pColumn->SetTooltip(_T("Discount Fare Policy"));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통합환승할인"));
		pColumn->SetTooltip(_T("통합환승할인제 적용"));
    }

	m_wndReportBCtrl.Populate();
}


void KGenerationBusPathStep2Dlg::InitReportBRecord(std::vector<TTransitPathCost> &a_vecTransitPathCost)
{
	m_wndReportBCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (size_t i= 0; i< a_vecTransitPathCost.size(); i++)
	{
		pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

		TTransitPathCost &oTTransitPathCost = a_vecTransitPathCost[i];

		CString strTransitName(_T(""));
		if ( TRANSIT_TYPE_BUS == oTTransitPathCost.nTransitType )
		{
			strTransitName = _T("Bus_Transit");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitName = _T("버스 타입");
			}
		}
		else if( TRANSIT_TYPE_SUBWAY == oTTransitPathCost.nTransitType)
		{
			strTransitName = _T("Rail_Transit");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitName = _T("철도 타입");
			}
		}

		if (TRANSIT_TYPE_BUS == oTTransitPathCost.nTransitType)
		{
			if (m_bUseBus == false && m_bUseBusSubway == false)
			{
				pRecord->SetVisible(FALSE);
			}
		}

		if (TRANSIT_TYPE_SUBWAY == oTTransitPathCost.nTransitType)
		{
			if (m_bUseSubway == false && m_bUseBusSubway ==false)
			{
				pRecord->SetVisible(FALSE);
			}
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTransitName));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetItemData((DWORD_PTR)oTTransitPathCost.nTransitType);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		CString strDetailTransit(_T(""));
		strDetailTransit.Format(_T("%d : %s"), oTTransitPathCost.nTransitDetailType, oTTransitPathCost.strDetailName);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDetailTransit));;
		pItem->SetAlignment(xtpColumnTextLeft);
		pItem->SetItemData((DWORD_PTR)oTTransitPathCost.nTransitDetailType);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitPathCost.dBaseCost,    _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextRight);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitPathCost.dBaseLength,  _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextRight);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitPathCost.dAddCost,     _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextRight);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitPathCost.dAddLength,   _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextRight);

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		if( 1 == oTTransitPathCost.nCostMethod )
			pItem->SetChecked(TRUE);
		else
			pItem->SetChecked(FALSE);
	}

	m_wndReportBCtrl.Populate();
}

void KGenerationBusPathStep2Dlg::InitSystemConstraintConditions()
{
	try
	{
		AutoType iter = mapDefaultPathGenerateConstraint.find(0);

		KPathGenerateConstraint oPathGenerateContraint = iter->second;

		double  dPathTime		= oPathGenerateContraint.dPathTime;
		int		nInvehicleTime	= oPathGenerateContraint.nInvehicleTime;
		int		nWatingTime		= oPathGenerateContraint.nWatingTime;
		int		nTransferTime	= oPathGenerateContraint.nTransferTime;
		int		nAETime			= oPathGenerateContraint.nAETime;
		double	dPathLength		= oPathGenerateContraint.dPathLength;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), dPathTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
		SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

		strValue.Format(_T("%.2f"), dPathLength);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
		SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

		if (1 == nInvehicleTime)
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
		}

		if (1 == nWatingTime)
		{
			CheckDlgButton(IDC_CHECK_WATING, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_WATING, FALSE);
		}

		if (1 == nTransferTime)
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
		}

		if (1 == nAETime)
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;  
}

void KGenerationBusPathStep2Dlg::DefaultConstraintConditions()
{
	try
	{
		AutoType iter = mapDefaultPathGenerateConstraint.find(1);
		AutoType end  = mapDefaultPathGenerateConstraint.end();

		if (iter == end)
		{
			return;
		}
		else
		{
			KPathGenerateConstraint oPathGenerateContraint = iter->second;

			double  dPathTime		= oPathGenerateContraint.dPathTime;
			int		nInvehicleTime	= oPathGenerateContraint.nInvehicleTime;
			int		nWatingTime		= oPathGenerateContraint.nWatingTime;
			int		nTransferTime	= oPathGenerateContraint.nTransferTime;
			int		nAETime			= oPathGenerateContraint.nAETime;
			double	dPathLength		= oPathGenerateContraint.dPathLength;

			CEdit* pEdit;
			CString strValue(_T(""));

			strValue.Format(_T("%.2f"), dPathTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
			SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

			strValue.Format(_T("%.2f"), dPathLength);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
			SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

			if (1 == nInvehicleTime)
			{
				CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
			}
			else
			{
				CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
			}

			if (1 == nWatingTime)
			{
				CheckDlgButton(IDC_CHECK_WATING, TRUE);
			}
			else
			{
				CheckDlgButton(IDC_CHECK_WATING, FALSE);
			}

			if (1 == nTransferTime)
			{
				CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
			}
			else
			{
				CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
			}

			if (1 == nAETime)
			{
				CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
			}
			else
			{
				CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;  
}

void KGenerationBusPathStep2Dlg::InitSystemPathInfo()
{
	try
	{
		AutoType iter = mapDefaultMultimodalPathInfo.find(0);

		KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

		int    nMax_Routes	   = oMultimodalPathInfo.nMax_Routes;
		double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
		double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%d"), nMax_Routes);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAXPATH);
		SetDlgItemText(IDC_EDIT_MAXPATH, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeBus);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeRail);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWTIME);
		SetDlgItemText(IDC_EDIT_DWTIME, strValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;
}

void KGenerationBusPathStep2Dlg::DefultPathInfo()
{
	try
	{
		AutoType iter = mapDefaultMultimodalPathInfo.find(1);

		KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

		int    nMax_Routes	   = oMultimodalPathInfo.nMax_Routes;
		double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
		double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%d"), nMax_Routes);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAXPATH);
		SetDlgItemText(IDC_EDIT_MAXPATH, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeBus);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeRail);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWTIME);
		SetDlgItemText(IDC_EDIT_DWTIME, strValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;
}

void KGenerationBusPathStep2Dlg::ResizeComponent()
{
	
	SetResize(IDC_SEPRATOR12,				  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	//SetResize(IDC_EDIT_MAXPATH,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR9,				  SZ_TOP_LEFT,  SZ_TOP_RIGHT);	
	//SetResize(IDC_EDIT_TIME,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	//SetResize(IDC_EDIT_DWTIME,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	//SetResize(IDC_STATIC5,				      SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	//SetResize(IDC_STATIC12,				      SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	SetResize(IDC_SEPRATOR10,				  SZ_TOP_LEFT,  SZ_TOP_RIGHT);	

	SetResize(IDC_REPORT2,				      SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);		
	SetResize(IDC_SEPRATOR11,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);		
	SetResize(IDC_BUTTON_DEFAULT,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON1,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDOK,						      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KGenerationBusPathStep2Dlg::SetDecimalRange(int a_nDecimalPlace, CString &a_strOutText)
{
	CString strDecimal(_T(""));
	strDecimal.Format(_T("%d"), a_nDecimalPlace);
	a_strOutText = _T("%.")+strDecimal+_T("f");
}


void KGenerationBusPathStep2Dlg::OnBnClickedRun()
{
	// 유효성 체크
	if ( !InvalidateInputInfo())
	{
		return;
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(GenerationBusPathThreadCaller, this, true, nLang);

	if (DLL_USER_STOPPING == m_nResultCode)
	{
		AfxMessageBox(_T("대중교통 경로 생성을 중단하였습니다."));
		TxLogDebug(_T("Transit Path Generator Stop ...."));

		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("대중교통 경로 생성 관련 DLL 파일이 존재하지 않습니다."));
		TxLogDebug(_T("대중교통 경로 생성 관련 DLL 파일이 존재하지 않습니다."));
		return;
	}
	else if (DLL_PLANNED_TERMINATION== m_nResultCode )
	{        
		AfxMessageBox(_T("대중교통 경로 생성을 완료하였습니다.\n대중교통 경로 편집 기능으로 생성된 결과에 대한 검색 및 편집이 가능합니다."));
		CopyFile(m_argFilePath, ConvertSavePath(T_ARG_GENERATEBUSPATH), FALSE);
	}
	else
	{
		AfxMessageBox(_T("대중교통 경로 생성 중 오류가 발생하였습니다."));
		TxLogDebug(_T("대중교통 경로 생성 중 오류가 발생하였습니다."));
		return;
	}

	m_nStatus = 0;
	KResizeDialogEx::OnOK();
}


bool KGenerationBusPathStep2Dlg::InvalidateInputInfo()
{
	double  dInputValue(0.0);
	int     nMaxRoutes        = GetDlgItemInt(IDC_EDIT_MAXPATH);

	if(nMaxRoutes < 2)
	{
		AfxMessageBox(_T("1보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MAXPATH)->SetFocus();
		return false;
	}

	CString strDwellTimebus;
	GetDlgItemText(IDC_EDIT_TIME, strDwellTimebus);

	if (!QBicStringChecker::IsNumeric(strDwellTimebus))
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_TIME)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strDwellTimebus);
	if (dInputValue < 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_TIME)->SetFocus();
		return false;
	}

	CString strDWellTime;
	GetDlgItemText(IDC_EDIT_DWTIME, strDWellTime);

	if (!QBicStringChecker::IsNumeric(strDWellTime))
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strDWellTime);
	if (dInputValue < 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
		return false;
	}

	CString strPathTime;
	GetDlgItemText(IDC_EDIT_PATHTIME, strPathTime);

	if (!QBicStringChecker::IsNumeric(strPathTime))
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strPathTime);
	if (dInputValue <= 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
		return false;
	}

	CString strPathLength;
	GetDlgItemText(IDC_EDIT_PATHLENGTH, strPathLength);

	if (!QBicStringChecker::IsNumeric(strPathLength))
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
		return false;
	}
	dInputValue = _ttof(strPathLength);
	if (dInputValue <= 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
		return false;
	}

	if(m_vecLoadAccessClassInfo.size() < 1)
	{
		AfxMessageBox(_T("접근 등급 관련 정보가 존재하지 않습니다."));
		return false;
	}

	return true;
}


unsigned __stdcall KGenerationBusPathStep2Dlg::GenerationBusPathThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("대중교통 경로 생성 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Transit Path Generator..."));
	}

	KGenerationBusPathStep2Dlg* pDlg = (KGenerationBusPathStep2Dlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}


void KGenerationBusPathStep2Dlg::Run()
{
	TxLogDebugStartMsg();
	try
	{
 		CreateArgument();
 
 		if( QBicSimpleProgressThread::IsUserStopped() == true )
 		{
 			return;
 		}
 
 		m_nResultCode = DLL_UNEXPECTED_ERROR;
 		CString strWindowName;
 		GetWindowText(strWindowName);
 
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("대중교통 경로 생성 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Transit Path Generator Module..."));
		}
 
		KLauncher::Run64(strWindowName, 13);
 		//KLauncher::Run(strWindowName, 13);
 		
 		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
 		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("대중교통 경로 생성 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Transit Path Generator Module Complete."));
			}
 			TxLogDebug(_T("Model Run Ok.."));
 		}
 		else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
 		{
            m_nResultCode = DLL_USER_STOPPING;
 			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
 			TxLogDebug(_T("Model Stop.."));
 			return;
 		}
 		else if (DLL_LOAD_ERROR == m_nResultCode)
 		{
 			AddStatusMessage(_T("> Generation Transit Dll Not Found ..."));
 			TxLogDebug(_T("DLL NOT Found.."));
 			return;
 		}
 		else
 		{
 			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
 			TxLogDebug(_T("Model Run Error.."));
 			return;
 		}
 
 		if( QBicSimpleProgressThread::IsUserStopped() == true )
 		{
 			return;
 		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}

		InsertBusPathInput();
		InsertBusPathCost();
		InsertBusMatrixInfo();
		
		CopyFile(m_argFilePath, ConvertSavePath(T_ARG_GENERATEBUSPATH), FALSE);
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
		return;
	}

	m_bRunResult = true;	

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::CreateArgument()
{
	ImChampDir::SetCurrentDirectoryToApp();

	ClearFiles();

	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(m_argFilePath, nOpenMode);

	CString strModelPath(_T(""));
	strModelPath.Format(_T("%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);

	CreateDirectory(strModelPath, NULL);
	CreateDirectory(ConvertSavePath(_T("")), NULL);

	CString strDllPath = ConvertSavePath(_T(""));

	CString strResultPath = strDllPath  + GENERATE_TRANSIT_RESULT_PATH;

	DeleteAllFolderFile(strResultPath);
	CreateDirectory(strResultPath, NULL);

	CString strGeneratorPath(_T(""));
	strGeneratorPath.Format(_T("%s\\%s"), DLL_MODEL_FOLDER_NAME, _T("TransitGenerator"));

	try 
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, strGeneratorPath);
	} catch (...) {
		ThrowException(_T("Dat 생성(DefineNetworkType.arg) 중 오류가 발생하였습니다. !!!"));
	}

	try
	{
		ArgumentWriteProjectName(of);// 아규먼트 쓰기 시작!!!!!!
		ArgumentWriteModeOD(of);
		ArgumentWriteTimeMethod(of);
		ArgumentWriteMaxTransitPath(of);
		ArgumentWriteTimeOfBoardAlight(of);
		//ArgumentWriteWaitVDF(of);
		ArgumentWriteBusPathCost(of);
		ArgumentWriteClass(of);
		ArgumentWriteCondition(of);
		ArgumentWriteFolder(of);
		of.Close();

		//TxLogDebug("START NODE CREAETE FILE ------ ");
		if ( !KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_NODE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}

		KIOColumns oLinkColumns;
		oLinkColumns.AddNewColumn(m_pIOColumnUseLink);
		if ( !KBulkDBase2File::ExportDB2LinkFileGeneratorPath(m_pTarget->GetDBaseConnection(), m_pIOColumnUseLink, ConvertSavePath(T_DAT_LINK) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LinkFileC) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_LINEINFO), ConvertSavePath(T_DAT_LINE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}

		KIOColumns oBusODColumns;
		
		AutoType iter = m_lstGenerationpathInfo.begin();
		AutoType end  = m_lstGenerationpathInfo.end();
		while(iter != end )
		{
			TGenerationPathInfo oInfo = *iter;
			oBusODColumns.AddNewColumn(oInfo.pColumn);
			++iter;
		}

		if ( !KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_MODE_OD, oBusODColumns, ConvertSavePath(T_DAT_IN_MODEOD_2) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2AccessStopNodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2AccessNodeStopFile) 중 오류가 발생하였습니다. !!!"));
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		of.Close();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)m_argFilePath);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_MODEOD_2));

		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_R_BUSPATH));
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::ArgumentWriteProjectName(CStdioFileEx& of)
{
	CString strProjectName = m_pProject->GetName();

	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("%s\r\n"), strProjectName);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteModeOD( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		int nCount = m_lstGenerationpathInfo.size();

		strOut.Format(_T("ModeOD\t%d\r\n"), nCount);
		of.WriteString(strOut);

		AutoType iter = m_lstGenerationpathInfo.begin();
		AutoType end  = m_lstGenerationpathInfo.end();

		while(iter != end)
		{
			TGenerationPathInfo oInfo = *iter;
			
			strOut.Format(_T("%s\t%d\t%d\t%d\r\n"), oInfo.strModeName, oInfo.nBus, oInfo.nSubway, oInfo.nExclusive);
			of.WriteString(strOut);
			++iter;
		}

	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

}


void KGenerationBusPathStep2Dlg::ArgumentWriteTimeMethod(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("TimeMethod\t%d\r\n"), m_nTimeMethod);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteMaxTransitPath( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		CString strMaxPath;
		GetDlgItemText(IDC_EDIT_MAXPATH, strMaxPath);
		strOut.Format(_T("MaxTransitPath\t%s\r\n"), strMaxPath);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteTimeOfBoardAlight(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));
		//int     nTimeBoardAlight  = GetDlgItemInt(IDC_EDIT_TIME);
		CString strDwellbus;
		GetDlgItemText(IDC_EDIT_TIME, strDwellbus);

		CString strDWellTime;
		GetDlgItemText(IDC_EDIT_DWTIME, strDWellTime);

		strOut.Format(_T("TimeOfBoardAlight\t%s\t%s\r\n"), strDwellbus, strDWellTime);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteWaitVDF(CStdioFileEx& of)
{
	CString strAlpha(_T(""));
	CString strBeta(_T(""));

	GetDlgItemText(IDC_EDIT_ALPHA, strAlpha);
	GetDlgItemText(IDC_EDIT_BETA, strBeta);

	double dAlpha = _ttof(strAlpha);
	double dBeta  = _ttof(strBeta);

	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("waitingVDF\t%.10f\t%.10f\r\n"), dAlpha, dBeta);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteBusPathCost(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));

		CXTPReportRecords*           pRecords   = m_wndReportBCtrl.GetRecords();
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

			pItem                  = pRecord->GetItem(_0_BCOLUMN_MODE);
			int nTransitType       = (int)pItem->GetItemData();

			if ( TRANSIT_TYPE_BUS == nTransitType )
				nBusTypeCount++;
			else if( TRANSIT_TYPE_SUBWAY == nTransitType )
				nRailTypeCount++;
		}
		strOut.Format(_T("detailedLineType\t%d\t%d\r\n"), nBusTypeCount, nRailTypeCount);
		of.WriteString(strOut);

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem                      = pRecord->GetItem(_0_BCOLUMN_MODE);
			int     nTransitType       = (int)pItem->GetItemData();
			CString strTransitType     = _T("");

			if(TRANSIT_TYPE_BUS == nTransitType)
				strTransitType = _T("buscost");
			else if(TRANSIT_TYPE_SUBWAY == nTransitType)
				strTransitType = _T("railcost");

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

			strOut.Format(_T("%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%d\r\n"), strTransitType, nDetailTransitType, dBaseCost, dBaseLength, dAddCost, dAddLength, nCostMethod);
			of.WriteString(strOut);

		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteClass(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));

		strOut.Format(_T("noClass\t%d\r\n"), (int)m_vecLoadAccessClassInfo.size());
		of.WriteString(strOut);

		for (size_t i= 0; i< m_vecLoadAccessClassInfo.size(); i++)
		{
			TAccessClassInfo &oTAccessClassInfo = m_vecLoadAccessClassInfo[i];
			strOut.Format(_T("%s\t%d\r\n"), oTAccessClassInfo.strClassName, oTAccessClassInfo.nTimeMin);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::ArgumentWriteCondition( CStdioFileEx& of )
{
	CString strOut(_T(""));
	strOut.Format(_T("Constraint Condition\t2\r\n"));
	of.WriteString(strOut);

	CString strtime;
	GetDlgItemText(IDC_EDIT_PATHTIME, strtime);

	CString strlength;
	GetDlgItemText(IDC_EDIT_PATHLENGTH, strlength);

	int nInVehecle = 0;
	int nWating    = 0;
	int nTransfer  = 0;
	int nAETime	   = 0;

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
	{
		nInVehecle = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
	{
		nWating = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
	{
		nTransfer = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
	{
		nAETime = 1;
	}

	strOut.Format(_T("time\t%s\t%d\t%d\t%d\t%d\r\n"), strtime, nInVehecle, nWating, nTransfer, nAETime);
	of.WriteString(strOut);

	strOut.Format(_T("length\t%s\r\n"), strlength);
	of.WriteString(strOut);

}



void KGenerationBusPathStep2Dlg::ArgumentWriteFolder(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("%s\r\n"), m_strOutputFolerPath);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KGenerationBusPathStep2Dlg::InsertBusPathInput()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString             strInsertQuery (_T(""));


	int     nMaxRoutes        = GetDlgItemInt(IDC_EDIT_MAXPATH);

	CString strDWTime(_T(""));
	CString strPathtime(_T(""));
	CString strPathlength(_T(""));
	CString strDwllbus(_T(""));
	GetDlgItemText(IDC_EDIT_DWTIME, strDWTime);
	GetDlgItemText(IDC_EDIT_PATHTIME, strPathtime);
	GetDlgItemText(IDC_EDIT_PATHLENGTH, strPathlength);
	GetDlgItemText(IDC_EDIT_TIME, strDwllbus);

	double  dDWTime		= _ttof(strDWTime);
	double  dPathtime	= _ttof(strPathtime);
	double  dPathlength = _ttof(strPathlength);
	double  dDwtimebus	= _ttof(strDwllbus);

	int nInVehicleTime(0);
	int nWatingTime(0);
	int nTransferTime(0);
	int nAETime(0);

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
	{
		nInVehicleTime = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
	{
		nWatingTime = 1;
	}
	
	if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
	{
		nTransferTime = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
	{
		nAETime = 1;
	}

	int nUseBus(0);
	if (m_bUseBus == true)
	{
		nUseBus = 1;
	}

	int nUseSubway(0);
	if (m_bUseSubway == true)
	{
		nUseSubway = 1;
	}

	int nUseBusSubway(0);
	if (m_bUseBusSubway == true)
	{
		nUseBusSubway = 1;
	}

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_BUS_PATH_INPUT);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		strInsertQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( Max_Routes, Time_Board, UseBus, UseSubway, UseBusSubway, DwellTime, path_time, invehicle_time, waiting_time, transfer_time, ae_time, path_length ) ")
			_T(" VALUES ")
			_T(" ( %d, %f, %d, %d, %d, %f, %f, %d, %d, %d, %d, %f ) "), TABLE_BUS_PATH_INPUT, nMaxRoutes, dDwtimebus, nUseBus, nUseSubway, nUseBusSubway, dDWTime, dPathtime, nInVehicleTime, nWatingTime, nTransferTime, nAETime, dPathlength);
		int nResult = spDBaseConnection->ExecuteUpdate(strInsertQuery);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::InsertBusPathCost()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString             strInsertQuery (_T(""));

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_BUS_PATH_COST);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		strInsertQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( Transit_Type, Transit_Detail_Type, Base_Cost, Base_Length, Add_Length, Add_Cost, Cost_Method ) ")
			_T(" VALUES ")
			_T(" ( ?, ?, ?, ?, ?, ?, ? ) "), TABLE_BUS_PATH_COST);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strInsertQuery);

		CXTPReportRecords*           pRecords      = m_wndReportBCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemNumber*  pItemNum      = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportRecordItem*        pExtItemCheck = nullptr;

		int nRecordCount   = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem                      = pRecord->GetItem(_0_BCOLUMN_MODE);
			int     nTransitType       = (int)pItem->GetItemData();
			spPrepareStatement->BindInt(1, nTransitType);

			pItem                      = pRecord->GetItem(_1_BCOLUMN_TYPE);
			int nDetailTransitType     = (int)pItem->GetItemData();
			spPrepareStatement->BindInt(2, nDetailTransitType);

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_BCOLUMN_BASE_COST);
			double dBaseCost           =  pItemNum->GetValue();
			spPrepareStatement->BindDouble(3, dBaseCost);

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_BCOLUMN_BASE_DISTANCE);
			double dBaseLength         =  pItemNum->GetValue();
			spPrepareStatement->BindDouble(4, dBaseLength);

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_BCOLUMN_ADD_DISTANCE);
			double dAddLength          =  pItemNum->GetValue();
			spPrepareStatement->BindDouble(5, dAddLength);

			pItemNum                   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_BCOLUMN_ADD_COST);
			double dAddCost            =  pItemNum->GetValue();
			spPrepareStatement->BindDouble(6, dAddCost);

			pExtItemCheck    = pRecord->GetItem(_6_BCOLUMN_CHECK_INTERGRATE);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
				nCostMethod = 1;
			else
				nCostMethod = 0;

			spPrepareStatement->BindInt(7, nCostMethod);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep2Dlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KGenerationBusPathStep2Dlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


CString KGenerationBusPathStep2Dlg::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH,a_strFileName);

	return strSavePath;
}

void KGenerationBusPathStep2Dlg::OnBnClickedButtonBack()
{
	m_nStatus = 2;
	OnOK();
}


BOOL KGenerationBusPathStep2Dlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}


	return KResizeDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void KGenerationBusPathStep2Dlg::GetUseModeData( void )
{
	
	AutoType iter = m_lstGenerationpathInfo.begin();
	AutoType end  = m_lstGenerationpathInfo.end();
	while(iter != end)
	{
		TGenerationPathInfo oInfo = *iter;

		if (1 == oInfo.nBus && 0 == oInfo.nSubway)
		{
			m_bUseBus = true;
		}
		else if (0 == oInfo.nBus && 1 == oInfo.nSubway)
		{
			m_bUseSubway = true;
		}
		else if (1 == oInfo.nBus && 1 == oInfo.nSubway)
		{
			m_bUseBusSubway = true;
		}

		++iter;
	}
}

void KGenerationBusPathStep2Dlg::InsertBusMatrixInfo()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_BUS_PATH_MATRIX);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		AutoType iter = m_lstGenerationpathInfo.begin();
		AutoType end  = m_lstGenerationpathInfo.end();
		while(iter != end)
		{
			TGenerationPathInfo oInfo = *iter;
			CString strColumnName	= oInfo.pColumn->Name();
			int nBus				= oInfo.nBus;
			int nSubway				= oInfo.nSubway;
			int nExclusive			= oInfo.nExclusive;
			int nModeID				= oInfo.pMode->ModeID();
			int nBusSubway			= 0;
			if (1 == nBus && 1== nSubway )
			{
				nBusSubway = 1;
			}

			CString strSQL = _T("");
			strSQL.Format(_T("Insert into %s Values ('%s', %d, %d, %d, %d, %d) "),TABLE_BUS_PATH_MATRIX, strColumnName,
				nBus, nSubway, nBusSubway, nExclusive, nModeID);

			spDBaseConnection->ExecuteUpdate(strSQL);
			++iter;
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

void KGenerationBusPathStep2Dlg::DeleteAllFolderFile( CString a_strLocation )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(a_strLocation.Right(1) != _T("\\"))
		a_strLocation += _T("\\");

	a_strLocation += _T("*.*");
	bContinue = finder.FindFile(a_strLocation);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeleteAllFolderFile(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}
}

void KGenerationBusPathStep2Dlg::OnBnClickedDefault()
{
	try
	{
		KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(mapDefaultPathGenerateConstraint);
		KDBaseDefaultParaMultimodalPathGeneration::LoadDefaultMultimodalPathInfo    (mapDefaultMultimodalPathInfo);

		AutoType iterPathInfo = mapDefaultMultimodalPathInfo.find(1);
		AutoType endPathInfo  = mapDefaultMultimodalPathInfo.end();
		//KMultimodalPathGenerationInfo & oMultimodalPathInfo = iterPathInfo->second;

		AutoType iter = mapDefaultPathGenerateConstraint.find(1);
		AutoType end  = mapDefaultPathGenerateConstraint.end();
		//KPathGenerateConstraint& oPathGenerateContraint = iter->second;

		if((iterPathInfo==endPathInfo) || (iter==end))
		{
			InitSystemConstraintConditions();
			InitSystemPathInfo();
		}
		else
		{
			DefaultConstraintConditions();
			DefultPathInfo();
		}
		std::vector<TTransitPathCost>      vecTransitPathCost;
		DefaultBusPathCost(vecTransitPathCost);
		InitReportBRecord(vecTransitPathCost);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}
}
