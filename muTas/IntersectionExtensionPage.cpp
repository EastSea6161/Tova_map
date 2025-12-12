/**
 * @file IntersectionExtensionPage.cpp
 * @brief KIntersectionExtensionPage 구현 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */

#include "stdafx.h"
#include "IntersectionExtensionPage.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"

enum
{
	_0_COLUMN_TITLE = 0,
	_1_COLUMN_1B,
	_2_COLUMN_2B,
	_3_COLUMN_3B,
	_4_COLUMN_4B
};

// KIntersectionExtensionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionExtensionPage, KDialogEx)

KIntersectionExtensionPage::KIntersectionExtensionPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, CWnd* pParent /*=NULL*/)
	: KDialogEx(KIntersectionExtensionPage::IDD, pParent)
	, m_spDBaseConnection(spDBaseConnection)
	, m_oTSelectIntersection(a_oTSelectIntersection)
{

}

KIntersectionExtensionPage::~KIntersectionExtensionPage()
{
}

void KIntersectionExtensionPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KIntersectionExtensionPage, KDialogEx)
END_MESSAGE_MAP()


// KIntersectionExtensionPage 메시지 처리기입니다.
BOOL KIntersectionExtensionPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, TRUE);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetIntersectionExtensionInfo();

	InitReportHeader();
	InitReportControl();

	m_wndReportCtrl.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void KIntersectionExtensionPage::GetIntersectionExtensionInfo()
{
	/*CString strSQL;
	strSQL.Format (_T(" SELECT node_id, bound_direction, lt, th, rt, signal_type, lt_type, rt_type ")
		_T(" FROM Intersection_info ")
		_T(" WHERE node_id = %I64d order by bound_direction "), m_oTSelectIntersection.nxNodeID
		);*/

	CString strSQL;
	strSQL.Format (_T(" SELECT node_id, bound_direction, bus_stop, bus_activity, bus_stop_location, parking_permission, park_activity, pedstrian, offset ")
		_T(" FROM Intersection_extension ")
		_T(" WHERE node_id = %I64d order by bound_direction "), m_oTSelectIntersection.nxNodeID
		);

	KResultSetPtr pResult = m_spDBaseConnection->ExecuteQuery(strSQL);

	while( pResult->Next() )
	{
		TExtension oTExtension;

		oTExtension.nxNodeID           = pResult->GetValueInt64(0);
		oTExtension.nBoundDirection    = pResult->GetValueInt  (1);
		oTExtension.nBusStop           = pResult->GetValueInt  (2);
		oTExtension.nBusActivity       = pResult->GetValueInt  (3);
		oTExtension.nBusStopLocation   = pResult->GetValueInt  (4);
		oTExtension.nParkingPermission = pResult->GetValueInt  (5);
		oTExtension.nParkActivity      = pResult->GetValueInt  (6);
		oTExtension.nPedstrian         = pResult->GetValueInt  (7);
		oTExtension.nOffset            = pResult->GetValueInt  (8);

		m_mapExtensionInfo.insert(std::make_pair(oTExtension.nBoundDirection, oTExtension));
	}
}


void KIntersectionExtensionPage::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TITLE, _T("Variable"), 120, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("입력 항목"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_1B,    _T("1st Bound"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER); 

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_2B,    _T("2nd Bound"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_3B,    _T("3rd Bound"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if(m_oTSelectIntersection.nType == KEMIntersectionTypeFour)
	{
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_4B,    _T("4th Bound"), 25));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
}


void KIntersectionExtensionPage::InitReportControl()
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	std::vector<CString> vecTitle;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        vecTitle.push_back(_T("버스정차대수(대/시)"));
        vecTitle.push_back(_T("버스베이 및 활동"));
        vecTitle.push_back(_T("버스정류장위치(m)"));
        vecTitle.push_back(_T("주차여부"));
        vecTitle.push_back(_T("주차활동대수(대/시)"));
        vecTitle.push_back(_T("횡단보행자수(인/시)"));
        vecTitle.push_back(_T("옵셋(초)"));
    }
    else {
        vecTitle.push_back(_T("Bus Stop"));
        vecTitle.push_back(_T("Bus Activity"));
        vecTitle.push_back(_T("Bus Stop Location"));
        vecTitle.push_back(_T("Parking Permission"));
        vecTitle.push_back(_T("Park Activity"));
        vecTitle.push_back(_T("Pedstrian"));
        vecTitle.push_back(_T("Offset"));
    }	
	
	for(size_t i= 0; i< vecTitle.size(); i++)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		// Title
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(vecTitle[i]));
		pItem->SetAlignment(xtpColumnTextCenter);

		// - 1stB
		double dValue = GetValue(KEMBoundDirectionCode1B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 2ndB
		dValue = GetValue(KEMBoundDirectionCode2B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 3rdB
		dValue = GetValue(KEMBoundDirectionCode3B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 4thB 
		if(m_oTSelectIntersection.nType == KEMIntersectionTypeFour)
		{
			dValue = GetValue(KEMBoundDirectionCode4B, i);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
			pItem->SetAlignment(xtpColumnTextRight);
		}
	}
}


double KIntersectionExtensionPage::GetValue( KEMBoundDirectionCode emDirectionCode,  size_t a_nVariable )
{
	std::map<int, TExtension>::iterator iter = m_mapExtensionInfo.find(emDirectionCode);

	int nValue;
	if(iter != m_mapExtensionInfo.end())
	{
		TExtension &oTExtension = iter->second;

		if( KEMExSeqBusStop           == a_nVariable )
			nValue = oTExtension.nBusStop;
		else if( KEMExSeqBusActivity       == a_nVariable )
			nValue = oTExtension.nBusActivity;
		else if( KEMExSeqBusStopLocation   == a_nVariable )
			nValue = oTExtension.nBusStopLocation;
		else if( KEMExSeqParkingPermission == a_nVariable )
			nValue = oTExtension.nParkingPermission;
		else if( KEMExSeqParkActivity      == a_nVariable )
			nValue = oTExtension.nParkActivity;
		else if( KEMExSeqPedstrian         == a_nVariable )
			nValue = oTExtension.nPedstrian;
		else if( KEMExSeqOffset            == a_nVariable )
			nValue = oTExtension.nOffset;
		else
			return 0.0;
	}
	else 
	{
		return 0.0;
	}

	return (double)nValue;
}


void KIntersectionExtensionPage::SetValue( std::map<int, TExtension> &a_mapExtensionInfo, KEMBoundDirectionCode emDirectionCode,  int a_nRowIndex, int a_nValue )
{
	std::map<int, TExtension>::iterator iter = a_mapExtensionInfo.find(emDirectionCode);

	if(iter != a_mapExtensionInfo.end())
	{
		TExtension &oTExtension = iter->second;

		oTExtension.nxNodeID        = m_oTSelectIntersection.nxNodeID;
		oTExtension.nBoundDirection = emDirectionCode;

		if( KEMExSeqBusStop                == a_nRowIndex )
			oTExtension.nBusStop           =  a_nValue;
		else if( KEMExSeqBusActivity       == a_nRowIndex )
			oTExtension.nBusActivity       =  a_nValue;
		else if( KEMExSeqBusStopLocation   == a_nRowIndex )
			oTExtension.nBusStopLocation   =  a_nValue;
		else if( KEMExSeqParkingPermission == a_nRowIndex )
			oTExtension.nParkingPermission =  a_nValue;
		else if( KEMExSeqParkActivity      == a_nRowIndex )
			oTExtension.nParkActivity      =  a_nValue;
		else if( KEMExSeqPedstrian         == a_nRowIndex )
			oTExtension.nPedstrian         =  a_nValue;
		else if( KEMExSeqOffset            == a_nRowIndex )
			oTExtension.nOffset            =  a_nValue;
	}
	else 
	{
		TExtension oTExtension;

		oTExtension.nxNodeID        = m_oTSelectIntersection.nxNodeID;
		oTExtension.nBoundDirection = emDirectionCode;

		if( KEMExSeqBusStop                == a_nRowIndex )
			oTExtension.nBusStop           =  a_nValue;
		else if( KEMExSeqBusActivity       == a_nRowIndex )
			oTExtension.nBusActivity       =  a_nValue;
		else if( KEMExSeqBusStopLocation   == a_nRowIndex )
			oTExtension.nBusStopLocation   =  a_nValue;
		else if( KEMExSeqParkingPermission == a_nRowIndex )
			oTExtension.nParkingPermission =  a_nValue;
		else if( KEMExSeqParkActivity      == a_nRowIndex )
			oTExtension.nParkActivity      =  a_nValue;
		else if( KEMExSeqPedstrian         == a_nRowIndex )
			oTExtension.nPedstrian         =  a_nValue;
		else if( KEMExSeqOffset            == a_nRowIndex )
			oTExtension.nOffset            =  a_nValue;
		
		a_mapExtensionInfo.insert(std::make_pair(oTExtension.nBoundDirection, oTExtension));

	}
}


// BOOL KIntersectionExtensionPage::OnApply()
// {
// 	std::map<int, TExtension> mapExtensionInfo;
// 
// 	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
// 	for( int i= 0; i< pRecords->GetCount(); i++ )
// 	{
// 		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
// 		CXTPReportRecordItemNumber* pItemNumber = NULL;  
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
// 		SetValue( mapExtensionInfo, KEMBoundDirectionCode1B, i, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
// 		SetValue( mapExtensionInfo, KEMBoundDirectionCode2B, i, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
// 		SetValue( mapExtensionInfo, KEMBoundDirectionCode3B, i, (int)(pItemNumber->GetValue()) );
// 
// 		if(m_oTSelectIntersection.nType == KEMIntersectionTypeFour)
// 		{
// 			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_4B);
// 			SetValue( mapExtensionInfo, KEMBoundDirectionCode4B, i, (int)(pItemNumber->GetValue()) );
// 		}
// 	}
// 
// 	if( false == UpsertExtensionData(mapExtensionInfo) )
// 	{
// 		AfxMessageBox(_T("Update Extension Data 오류 발생"));
// 		return FALSE;
// 	}
// 
// 	KDialogEx::OnApply();
// 	return TRUE;
// }


BOOL KIntersectionExtensionPage::SaveExtension()
{
	std::map<int, TExtension> mapExtensionInfo;

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItemNumber = NULL;  

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
		SetValue( mapExtensionInfo, KEMBoundDirectionCode1B, i, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
		SetValue( mapExtensionInfo, KEMBoundDirectionCode2B, i, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
		SetValue( mapExtensionInfo, KEMBoundDirectionCode3B, i, (int)(pItemNumber->GetValue()) );

		if(m_oTSelectIntersection.nType == KEMIntersectionTypeFour)
		{
			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_4B);
			SetValue( mapExtensionInfo, KEMBoundDirectionCode4B, i, (int)(pItemNumber->GetValue()) );
		}
	}

	if( false == UpsertExtensionData(mapExtensionInfo) )
	{
		AfxMessageBox(_T("Update Extension Data 오류 발생"));
		return FALSE;
	}

	return TRUE;
}


bool KIntersectionExtensionPage::UpsertExtensionData(std::map<int, TExtension> &a_mapExtensionInfo)
{
	CString  strSQLDeleteTable(_T(""));
	CString  strSQLInsert     (_T(""));

	strSQLDeleteTable.Format( _T(" Delete From Intersection_extension where node_id = %I64d "), m_oTSelectIntersection.nxNodeID );
	strSQLInsert.Format(_T(" Insert Into Intersection_extension ")
		_T(" ( node_id, bound_direction, bus_stop, bus_activity, bus_stop_location, parking_permission, park_activity, pedstrian, offset ) ")
		_T(" Values ( ?, ?, ?, ?, ?, ?, ?, ?, ? ) ") );
	try
	{  
		m_spDBaseConnection->BeginTransaction();

		if ( m_spDBaseConnection->ExecuteUpdate(strSQLDeleteTable) != SQLITE_DONE )
		{
			TxLogDebug(strSQLDeleteTable.AllocSysString());

			m_spDBaseConnection->RollBack();

			return false;
		}

		KPreparedStatementPtr prepareStatementPtr = m_spDBaseConnection->PrepareStatement(strSQLInsert);

		std::map<int, TExtension>::iterator iter = a_mapExtensionInfo.begin();
		std::map<int, TExtension>::iterator end  = a_mapExtensionInfo.end();

		while(iter != end)
		{
			int         nBooundDirection  = iter->first;
			TExtension  &oTExtension      = iter->second;

			prepareStatementPtr->BindInt64(1, oTExtension.nxNodeID);
			prepareStatementPtr->BindInt  (2, nBooundDirection);
			prepareStatementPtr->BindInt  (3, oTExtension.nBusStop);
			prepareStatementPtr->BindInt  (4, oTExtension.nBusActivity);
			prepareStatementPtr->BindInt  (5, oTExtension.nBusStopLocation);
			prepareStatementPtr->BindInt  (6, oTExtension.nParkingPermission);
			prepareStatementPtr->BindInt  (7, oTExtension.nParkActivity);
			prepareStatementPtr->BindInt  (8, oTExtension.nPedstrian);
			prepareStatementPtr->BindInt  (9, oTExtension.nOffset);

			prepareStatementPtr->ExecuteUpdate();
			prepareStatementPtr->Reset();

			++iter;
		}    

		m_spDBaseConnection->Commit(); 

		return true;
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
		m_spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return false;
}
