/**
 * @file DisplayColumnSetDlg.cpp
 * @brief KDisplayColumnSetDlg 구현
 * @author 
 * @date 2010.09.08
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DisplayColumnSetDlg.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^#include "DBaseConnector.h"
#include "kmz_common.h"
#include "DefineNames.h"
//#include "Table.h"

enum
{
	_0_COLUMN_NAME     = 0,
	_1_COLUMN_DESCRIPT
};
#define COLUMN_TOTAL_COUNT 2

/******************************************************************************
* KDisplayColumnSetDlg                                                        *
******************************************************************************/
IMPLEMENT_DYNAMIC(KDisplayColumnSetDlg, CXTPResizeDialog)


/**************************************
* Constructors / Destructor           *
**************************************/
KDisplayColumnSetDlg::KDisplayColumnSetDlg(CWnd* pParent /*=NULL*/)
	:	CXTPResizeDialog(KDisplayColumnSetDlg::IDD, pParent),
		m_pTable( NULL )
{

}


KDisplayColumnSetDlg::~KDisplayColumnSetDlg()
{
}


void KDisplayColumnSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLENAME, m_stcTableName);
	DDX_Control(pDX, IDC_REPORT,    m_ReportCtrl);
}


BEGIN_MESSAGE_MAP(KDisplayColumnSetDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDOK, &KDisplayColumnSetDlg::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT, OnReportCheckItem)
END_MESSAGE_MAP()


/**************************************
* Operations                          *
**************************************/
void KDisplayColumnSetDlg::addColumn( CXTPReportColumn* pColumn )
{
	m_DisplayColumnList.push_back( pColumn );
}


/**************************************
* Overrides                           *
**************************************/
BOOL KDisplayColumnSetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MakeDisplayInfo();
	KReportCtrlSetting::Default(m_ReportCtrl);
	InitReportHeader();
	InitReportData();
	CleanCheckState();
	m_ReportCtrl.Populate();
	m_ReportCtrl.ExpandAll(TRUE);

	//if( NULL != m_pTable )
	//{
	//	CString strStatic;
	//	m_stcTableName.GetWindowText( strStatic );
	//	strStatic += m_pTable->getDisplayName();
	//	m_stcTableName.SetWindowText( strStatic );
	//}
	if(NULL != m_pTable)
	{
		CString strStatic;
		m_stcTableName.GetWindowText(strStatic);
		strStatic += m_pTable->Caption();
		m_stcTableName.SetWindowText(strStatic);
	}
	
	SetResize(IDC_STATIC4,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC3,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDisplayColumnSetDlg::MakeDisplayInfo2()
{
	TDispColInfo oTTemp;
	oTTemp.strName     = _T("ID");
	oTTemp.strDescript = _T("노드ID");
	m_vecFieldData_System.push_back(oTTemp);
	oTTemp.strName     = _T("Name");
	oTTemp.strDescript = _T("노드명");
	m_vecFieldData_System.push_back(oTTemp);

	oTTemp.strName     = _T("Type");
	oTTemp.strDescript = _T("노드타입");
	m_vecFieldData_User_Normal.push_back(oTTemp);
	oTTemp.strName     = _T("zone_id");
	oTTemp.strDescript = _T("존");
	m_vecFieldData_User_Normal.push_back(oTTemp);

	oTTemp.strName     = _T("generation");
	oTTemp.strDescript = _T("generation");
	m_vecFieldData_User_Generation.push_back(oTTemp);
	oTTemp.strName     = _T("modechoice");
	oTTemp.strDescript = _T("modechoice");
	m_vecFieldData_User_ModeChoice.push_back(oTTemp);
	oTTemp.strName     = _T("distribution");
	oTTemp.strDescript = _T("distribution");
	m_vecFieldData_User_Distribution.push_back(oTTemp);
	oTTemp.strName     = _T("assign");
	oTTemp.strDescript = _T("assign");
	m_vecFieldData_User_Assign.push_back(oTTemp);
}


void KDisplayColumnSetDlg::MakeDisplayInfo()
{
	std::list< CXTPReportColumn* >::iterator itDisplayColumn = m_DisplayColumnList.begin();
	while( m_DisplayColumnList.end() != itDisplayColumn )
	{
		CXTPReportColumn* pReportColumn = *itDisplayColumn;
		if( NULL == pReportColumn )
		{
			++itDisplayColumn;
			continue;
		}

		TDispColInfo oTDispColInfo;
		oTDispColInfo.pColumn = pReportColumn;

		KIOColumn* pColumn     = ConvertReportColumn_To_IOColumn(pReportColumn);
		if(NULL == pColumn)
		{
			++itDisplayColumn;
			continue;
		}

		KEMIOColumnType emColumnType  = pColumn->ColumnType();

		if(KEMIOColumnTypeSystem == emColumnType)
			m_vecFieldData_System.push_back(oTDispColInfo);
		else if(KEMIOColumnTypeFixed == emColumnType)
			m_vecFiledData_Fixed.push_back(oTDispColInfo);
		else if(KEMIOColumnTypeUserDefine == emColumnType)
			m_mapFieldData_User.insert( std::make_pair(pColumn->Name(), oTDispColInfo) );

		++itDisplayColumn;
	}
	
	CString strTableName = m_pTable->Name();
	if( strTableName.CompareNoCase(TABLE_LINK) == 0 && KmzSystem::IsPassengerSystem() )// TODO: 조건문을 테이블이 'Link'일 경우에만
	{
		SelectAssignModelLink();
	}
	if( strTableName.CompareNoCase(TABLE_TRANSIT) == 0 && KmzSystem::IsPassengerSystem() )// TODO: 조건문을 테이블이 'TransitInfo'일 경우에만
	{
		SelectAssignModelTransitInfo();
	}
	if( strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0 && KmzSystem::IsPassengerSystem() )// TODO: 조건문을 테이블이 'Transitline'일 경우에만
	{
		SelectAssignModelTransitLine();
	}
	if (strTableName.CompareNoCase(TABLE_NODE) == 0 && KmzSystem::IsPassengerSystem())
	{
		SelectAssignModelNode();
	}
	if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0 && KmzSystem::IsPassengerSystem())
	{
		SelectAssignModelModeOD();
	}
	if (strTableName.CompareNoCase(TABLE_TERMINAL_SCHEDULE) == 0 && KmzSystem::IsPassengerSystem())
	{
		SelectAssignModelTerminal();
	}
	if (strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0 && KmzSystem::IsPassengerSystem())
	{
		SelectAssignModelParaOD();
	}

	// 나머지 처리(User Field로)
	std::map<CString, TDispColInfo>::iterator itmap = m_mapFieldData_User.begin();
	while(itmap != m_mapFieldData_User.end())
	{
		m_vecFieldData_User_Normal.push_back(itmap->second);

		++itmap;
	}
	
	/*
	m_vecFieldData_User_Generation.push_back(oTDispColInfo);
	m_vecFieldData_User_Distribution.push_back(oTDispColInfo);
	m_vecFieldData_User_ModeChoice.push_back(oTDispColInfo);
	m_vecFieldData_User_Assign.push_back(oTDispColInfo);
	}*/
}


void KDisplayColumnSetDlg::CleanCheckState()
{
	CXTPReportRecords*	  pParents    = NULL;
	CXTPReportRecord*     pParent     = NULL;
	CXTPReportRecordItem* pParentItem = NULL;
	CXTPReportRecords*	  pChilds     = NULL;
	CXTPReportRecord*     pChild      = NULL;
	CXTPReportRecordItem* pChildItem  = NULL;

	pParents         = m_ReportCtrl.GetRecords();
	int nRecordCount = pParents->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pParent    = pParents->GetAt(i);
		pChilds    = pParent->GetChilds();

		BOOL bAllChildChecked = TRUE;
		for(int k= 0; k< pChilds->GetCount(); k++)
		{
			pChild     = pChilds->GetAt(k);
			pChildItem = pChild->GetItem(_0_COLUMN_NAME);

			if( FALSE == pChildItem->IsChecked() )
			{
				bAllChildChecked = FALSE;
				break;
			}
		}

		pParentItem = pParent->GetItem(_0_COLUMN_NAME);
		pParentItem->SetChecked(bAllChildChecked);
	}
}


KIOColumn* KDisplayColumnSetDlg::ConvertReportColumn_To_IOColumn(CXTPReportColumn* a_pReportColumn)
{
	if(NULL == a_pReportColumn)
		return NULL;

	const KIOColumns* pColumns = m_pTable->Columns();

	CString strColumnName = a_pReportColumn->GetInternalName();
	int     nIndex        = pColumns->GetColumnIndex(strColumnName);

	if(nIndex < 0)
	{
		return NULL;
	}

	KIOColumn* pColumn = pColumns->GetColumn(nIndex);

	return pColumn;
}


void KDisplayColumnSetDlg::SelectAssignModelLink()
{
	try
	{
        KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

		CString strSQL;
		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
			_T(" vc, average_time, vdf_time, sum_volume, mode_volume, ")
			_T(" ave_time_toll, vdf_toll_time, trip, Bus_Initial_volume ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strVC             = pResult->GetValueString(4);
			ParseFiledName(strVC,             vecFieldData_User_Assign);
			CString strAverageTime    = pResult->GetValueString(5);
			ParseFiledName(strAverageTime,    vecFieldData_User_Assign);
			CString strVDFTime        = pResult->GetValueString(6);
			ParseFiledName(strVDFTime,        vecFieldData_User_Assign);
			CString strSumVolume      = pResult->GetValueString(7);
			ParseFiledName(strSumVolume,      vecFieldData_User_Assign);
			CString strModeVolume     = pResult->GetValueString(8);
			ParseFiledName(strModeVolume,     vecFieldData_User_Assign);
			CString strAveTimeToll     = pResult->GetValueString(9);
			ParseFiledName(strAveTimeToll,    vecFieldData_User_Assign);
			CString strVdfTimeToll     = pResult->GetValueString(10);
			ParseFiledName(strVdfTimeToll,    vecFieldData_User_Assign);
			CString strTrip            = pResult->GetValueString(11);
			ParseFiledName(strTrip,           vecFieldData_User_Assign);
			CString strBusInitVolume   = pResult->GetValueString(12);
			ParseFiledName(strBusInitVolume, vecFieldData_User_Assign);

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDisplayColumnSetDlg::SelectAssignModelTransitInfo()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

		CString strSQL;
		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description,linfo_num_passengers, ")
					_T(" linfo_total_length, linfo_avg_length, linfo_max_passengers, linfo_avg_passengers, ")
					_T(" linfo_max_second, linfo_avg_second, linfo_max_standing, linfo_avg_standing, ")
					_T(" linfo_max_congestion, linfo_avg_congestion, linfo_km_passengers, linfo_once_passengers, linfo_km_once_passengers ")
					_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			for (int i= 0; i< 14; i++)
			{
				CString strPassengers             = pResult->GetValueString(4+i);
				ParseFiledName(strPassengers,             vecFieldData_User_Assign);
			}

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
		}

	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDisplayColumnSetDlg::SelectAssignModelTransitLine()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

		CString strSQL;
// 		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
// 			_T(" line_boarding, line_alighting, line_second_ride ")
// 			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
			_T(" line_board_net, line_board_Transfer, line_board_Total, ")
			_T(" line_alight_net, line_alight_Transfer, line_alight_Total, ")
			_T(" line_passenger ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strLineField(_T(""));
		
			for (int i= 0; i< 7; i++)
			{
				strLineField = pResult->GetValueString(4+i);
				ParseFiledName(strLineField,     vecFieldData_User_Assign);
			}

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}



void KDisplayColumnSetDlg::SelectAssignModelModeOD()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

		CString strSQL;
		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
			_T(" od_inter_mode, od_bus, od_rail, od_busrail ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strInterOD             = pResult->GetValueString(4);
			ParseFiledName(strInterOD,             vecFieldData_User_Assign);
			CString strBusOD             = pResult->GetValueString(5);
			ParseFiledName(strBusOD,             vecFieldData_User_Assign);
			CString strRailOD             = pResult->GetValueString(6);
			ParseFiledName(strRailOD,             vecFieldData_User_Assign);
			CString strBusRailOD             = pResult->GetValueString(7);
			ParseFiledName(strBusRailOD,             vecFieldData_User_Assign);

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
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


void KDisplayColumnSetDlg::SelectAssignModelNode()
{
	try
	{
		KDBaseConPtr spDBaseConnetion = m_pTable->GetDBaseConnection();

		CString strSQL;
		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
			_T(" node_bus_net_board, node_bus_transfer_board, node_bus_sum_board, ")
			_T(" node_bus_net_alight, node_bus_transfer_alight, node_bus_sum_alight, ")
			_T(" pr_auto, pr_bus, pr_rail,  pr_transit ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnetion->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strBusNetBoard             = pResult->GetValueString(4);
			ParseFiledName(strBusNetBoard,             vecFieldData_User_Assign);
			CString strBusTransBoard    = pResult->GetValueString(5);
			ParseFiledName(strBusTransBoard,    vecFieldData_User_Assign);
			CString strBusSumBoard        = pResult->GetValueString(6);
			ParseFiledName(strBusSumBoard,        vecFieldData_User_Assign);
			CString strBusNetAli      = pResult->GetValueString(7);
			ParseFiledName(strBusNetAli,      vecFieldData_User_Assign);
			CString strBusTransAli     = pResult->GetValueString(8);
			ParseFiledName(strBusTransAli,     vecFieldData_User_Assign);
			CString strBusSumAli     = pResult->GetValueString(9);
			ParseFiledName(strBusSumAli,    vecFieldData_User_Assign);
			CString strPrAuto     = pResult->GetValueString(10);
			ParseFiledName(strPrAuto,    vecFieldData_User_Assign);
			CString strPrBus            = pResult->GetValueString(11);
			ParseFiledName(strPrBus,           vecFieldData_User_Assign);
			CString strPrRail   = pResult->GetValueString(12);
			ParseFiledName(strPrRail, vecFieldData_User_Assign);
			CString strPrTransit   = pResult->GetValueString(13);
			ParseFiledName(strPrTransit, vecFieldData_User_Assign);

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
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

void KDisplayColumnSetDlg::SelectAssignModelTerminal()
{
	try
	{
		KDBaseConPtr spDBaseConnetion = m_pTable->GetDBaseConnection();

		CString strSQL;
		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, terminal_trip ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnetion->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strTermianlTrip             = pResult->GetValueString(4);
			ParseFiledName(strTermianlTrip,             vecFieldData_User_Assign);
			
			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
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


void KDisplayColumnSetDlg::SelectAssignModelParaOD()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

		CString strSQL;

		strSQL.Format ( _T(" SELECT run_seq, start_date, end_date, description, ")
			_T(" od_average_time, od_vdf_time, od_average_bus, ")
			_T(" od_average_subway, od_average_bussubway, od_bus_direct, ")
			_T(" od_bus_one_transfer, od_bus_two_transfer ")
			_T(" FROM ASSIGN_MODEL ORDER BY run_seq DESC ") );

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TAssignDispColInfo oTAssignDispColInfo;

			CString                   &strTitle                 = oTAssignDispColInfo.strTitle;
			std::vector<TDispColInfo> &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;

			int     nRunSeq           = pResult->GetValueInt   (0);
			CString strStartDate      = pResult->GetValueString(1);
			CString strEndDate        = pResult->GetValueString(2);
			CString strDescription    = pResult->GetValueString(3);

			strTitle.Format(_T("%s #%d(%s~%s)"),strDescription, nRunSeq, strStartDate, strEndDate);
			//2011-10-09 00:00:00.000
			CString strLineField(_T(""));

			for (int i= 0; i< 8; i++)
			{
				strLineField = pResult->GetValueString(4+i);
				ParseFiledName(strLineField,     vecFieldData_User_Assign);
			}

			if(vecFieldData_User_Assign.size() > 0)
				m_vecFieldData_User_Double_Assign.push_back(oTAssignDispColInfo);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}




void KDisplayColumnSetDlg::ParseFiledName(CString &a_strIn, std::vector<TDispColInfo> &a_vecFieldData_User_Assign)
{
	std::vector<CString> vecOut;
	int nSize = ::ParseStringArr(a_strIn, vecOut);

	if(nSize < 1)
		return;

	std::map<CString, TDispColInfo>::iterator itFieldMap;
	for(size_t i= 0; i< vecOut.size(); i++)
	{
		CString &strFieldName = vecOut[i];
		itFieldMap = m_mapFieldData_User.find(strFieldName);

		if(m_mapFieldData_User.end() == itFieldMap)
			continue;

		a_vecFieldData_User_Assign.push_back( itFieldMap->second );
		m_mapFieldData_User.erase(itFieldMap);
	}
}


void KDisplayColumnSetDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_NAME,     _T("Column Name"),    60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼 명칭"));
    }

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_DESCRIPT, _T("Description"),    40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("데이터 내용"));
    }
}


void KDisplayColumnSetDlg::InitReportData()
{
	
	if(m_vecFieldData_System.size() > 0)			//###### System Field #######
	{
		SetReportGroup(_T("System Field"), m_vecFieldData_System);
	}
	
	if(m_vecFiledData_Fixed.size() > 0)				//###### Fixed Field #######
	{
		SetReportGroup(_T("Fixed Field"), m_vecFiledData_Fixed);
	}
	
	if(m_vecFieldData_User_Normal.size() > 0)		//###### User Field #######
	{
		SetReportGroup(_T("User Field"), m_vecFieldData_User_Normal);
	}
	
	if(m_vecFieldData_User_Generation.size() > 0)	//###### By Generation Field #######
	{
		SetReportGroup(_T("By Generation Field"), m_vecFieldData_User_Generation);
	}
	
	if(m_vecFieldData_User_Distribution.size() > 0)	//###### By Distribution Field #######
	{
		SetReportGroup(_T("By Distribution Field"), m_vecFieldData_User_Distribution);
	}
	
	if(m_vecFieldData_User_ModeChoice.size() > 0)	//###### By ModeChoice Field#######
	{
		SetReportGroup(_T("By ModeChoice Field"), m_vecFieldData_User_ModeChoice);
	}
													//###### By Assign Field#######
	for(size_t i= 0; i< m_vecFieldData_User_Double_Assign.size(); i++)
	{
		TAssignDispColInfo &oTAssignDispColInfo = m_vecFieldData_User_Double_Assign[i];
		std::vector<TDispColInfo>  &vecFieldData_User_Assign = oTAssignDispColInfo.vecFieldData;
		
		if(vecFieldData_User_Assign.size() > 0)		
		{
			SetReportGroup(oTAssignDispColInfo.strTitle, vecFieldData_User_Assign);
		}
	}

	/*
	//######User Field#######
	
	if(m_mapFieldData_User.size() > 0)
	{
		pParentRecord  = m_ReportCtrl.AddRecord(new CXTPReportRecord());
		pItem          = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("UserField")));
		pItem->HasCheckbox(TRUE);
		pItem->SetBold(TRUE);
		//pItem->SetItemData((DWORD_PTR) &);

		std::map<KEMField_Mode, std::vector<TDispColInfo>>::iterator iter = m_mapFieldData_User.begin();
		while(iter != m_mapFieldData_User.end())
		{
			KEMField_Mode              emFieldMode      = iter->first;
			std::vector<TDispColInfo> &vecFieldData_Use = iter->second;

			if(vecFieldData_Use.size() < 1)
			{
				++iter;
				continue;
			}

			pSecondRecords = pParentRecord->GetChilds();
			pSecondRecord  = pSecondRecords->Add((new CXTPReportRecord()));
			CString strModeName = GetFieldModeName(emFieldMode);
			pItem          = pSecondRecord->AddItem(new CXTPReportRecordItemText(strModeName));
			pItem->HasCheckbox(TRUE);
			pItem->SetBold(TRUE);
			pThirdRecords  = pSecondRecord->GetChilds();

			for(size_t i= 0; i< vecFieldData_Use.size(); i++)
			{
				pThirdRecord   = pThirdRecords->Add((new CXTPReportRecord()));
				SetReportLowData(pThirdRecord, vecFieldData_Use[i]);
			}

			++iter;
		}
	}
	*/
}


void KDisplayColumnSetDlg::SetReportGroup( CString a_strGroupName, std::vector<TDispColInfo> &a_vecFieldData )
{
	CXTPReportRecord*     pParentRecord  = NULL;
	CXTPReportRecords*    pSecondRecords = NULL;
	CXTPReportRecord*     pSecondRecord  = NULL;
	CXTPReportRecordItem* pItem          = NULL;

	pParentRecord  = m_ReportCtrl.AddRecord(new CXTPReportRecord());
	pItem          = pParentRecord->AddItem(new CXTPReportRecordItemText(a_strGroupName));
	pItem->HasCheckbox(TRUE);
	pItem->SetBold(TRUE);
	//pItem->SetItemData((DWORD_PTR) &);
	for (int i= 0; i< COLUMN_TOTAL_COUNT- 1; i++)
	{
		pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	}

	pSecondRecords = pParentRecord->GetChilds();

	for(size_t i= 0; i< a_vecFieldData.size(); i++)
	{
		pSecondRecord  = pSecondRecords->Add((new CXTPReportRecord()));
		SetReportLowData(pSecondRecord, a_vecFieldData[i]);
	}
}


void KDisplayColumnSetDlg::SetReportLowData2(CXTPReportRecord* a_pRecord, TDispColInfo &a_oTDispColInfo)
{
	CXTPReportRecordItem* pItem   = NULL;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(a_oTDispColInfo.strName));
	pItem->HasCheckbox(TRUE);
	if(true)
		pItem->SetChecked(TRUE);
	//pItem->SetItemData((DWORD_PTR)&);
	pItem->SetAlignment(DT_LEFT);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(a_oTDispColInfo.strDescript));
	pItem->SetAlignment(DT_LEFT);
}


void KDisplayColumnSetDlg::SetReportLowData(CXTPReportRecord* a_pRecord, TDispColInfo &a_oTDispColInfo)
{
	CXTPReportRecordItem* pItem   = NULL;
	CXTPReportColumn*     pColumn = a_oTDispColInfo.pColumn;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(pColumn->GetCaption()));
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(pColumn->IsVisible());
	pItem->SetItemData((DWORD_PTR)pColumn);
	pItem->SetAlignment(DT_LEFT);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(pColumn->GetTooltip()));
	pItem->SetAlignment(DT_LEFT);
}


/**************************************
* Message handlers                    *
**************************************/
void KDisplayColumnSetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXTPReportRecords* pParentRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*  pParentRecord  = NULL;

	int nRecordCount = pParentRecords->GetCount();
	for(int i= 0; i< nRecordCount; i++)
	{
		pParentRecord = pParentRecords->GetAt(i);

		CXTPReportRecords*      pChildRecords = pParentRecord->GetChilds();
		int  nChildCount      = pChildRecords->GetCount();

		for(int i= 0; i< nChildCount; i++)
		{
			CXTPReportRecord*     pRecord       = pChildRecords->GetAt(i);
			CXTPReportRecordItem* pItem         = pRecord->GetItem(_0_COLUMN_NAME);
			CXTPReportColumn*     pReportColumn = (CXTPReportColumn*)pItem->GetItemData();

			pReportColumn->SetVisible( pItem->IsChecked() );

			KIOColumn* pColumn = ConvertReportColumn_To_IOColumn(pReportColumn);
			if(NULL == pColumn)
				continue;

            KIOColumn oUpdateColumn = *pColumn;

			if( pItem->IsChecked() == TRUE )
				oUpdateColumn.Visible(true);
			else
				oUpdateColumn.Visible(false);

			m_pTable->UpdateColumn(oUpdateColumn);
		}
	}

	OnOK();
}

void KDisplayColumnSetDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);
	
	BOOL             bChecked = pItemNotify->pItem->IsChecked();
	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	
	SyncReportTree(pRecord, bChecked);
}


void KDisplayColumnSetDlg::SyncReportTree(CXTPReportRecord* a_pCurRecord, const BOOL &a_bCurChecked)
{
	AllChildRecordCheck(a_pCurRecord, a_bCurChecked);

	AllParentRecordCheck(a_pCurRecord);
}


void KDisplayColumnSetDlg::AllChildRecordCheck(CXTPReportRecord* a_pCurRecord, const BOOL &a_bCurChecked)
{
	CXTPReportRecords* pChildRecords = a_pCurRecord->GetChilds();
	int nChildCount  = pChildRecords->GetCount();

	for(int i= 0; i< nChildCount; i++)
	{
		CXTPReportRecord*     pRecord = pChildRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_NAME);
		pItem->SetChecked(a_bCurChecked);

		AllChildRecordCheck(pRecord, a_bCurChecked);
	}
}


void KDisplayColumnSetDlg::AllParentRecordCheck(CXTPReportRecord* a_pCurRecord)
{
	CXTPReportRecord* pParentRecord = a_pCurRecord->GetParentRecord();
	if(pParentRecord != NULL)
	{
		CXTPReportRecords*      pChildRecords = pParentRecord->GetChilds();
		int  nChildCount      = pChildRecords->GetCount();
		BOOL bAllChildChecked = TRUE;

		for(int i= 0; i< nChildCount; i++)
		{
			CXTPReportRecord*     pRecord = pChildRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_NAME);
			if( pItem->IsChecked() == FALSE )
			{
				bAllChildChecked = FALSE;
				break;
			}
		}

		CXTPReportRecordItem* pParentItem = pParentRecord->GetItem(_0_COLUMN_NAME);
		pParentItem->SetChecked(bAllChildChecked);

		AllParentRecordCheck(pParentRecord);
	}
}
