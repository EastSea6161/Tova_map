// InterPathGenTypeA.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGenTypeA.h"
#include "afxdialogex.h"
#include "Target.h"

//#include "CodeManager.h"
//#include "CodeGroup.h"
//^#include "DBaseConnector.h"

const int nREGION_PAHT_AUTO = -1;
const int nREGION_PATH_AE   = 999;

// KInterPathGenTypeA 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGenTypeA, KResizeDialogEx)

KInterPathGenTypeA::KInterPathGenTypeA(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGenTypeA::IDD, pParent)
{

}

KInterPathGenTypeA::~KInterPathGenTypeA()
{
}

void KInterPathGenTypeA::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_OD, m_wndReportCtrlOD);
	DDX_Control(pDX, IDC_REPORT_URBAN, m_wndReportCtrlUrban);
	DDX_Control(pDX, IDC_REPORT_REGION, m_wndReportCtrlRegional);
}


BEGIN_MESSAGE_MAP(KInterPathGenTypeA, KResizeDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// KInterPathGenTypeA 메시지 처리기입니다.

BOOL KInterPathGenTypeA::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrlOD, TRUE);
	KReportCtrlSetting::Default(m_wndReportCtrlUrban, TRUE);
	KReportCtrlSetting::Default(m_wndReportCtrlRegional, TRUE);

	InitReportHeaderOD();
	InitReportHeaderUrban();
	InitReportHeaderRegional();

	UpdateReportDataOD();
	UpdateReportDataUrban();
	UpdateReportDataRegion();
    

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterPathGenTypeA::SetTarget( KTarget* a_pTarget )
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

void KInterPathGenTypeA::InitReportHeaderOD( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlOD.AddColumn(new CXTPReportColumn(0, _T("Select"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlOD.AddColumn(new CXTPReportColumn(1, _T("OD"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KInterPathGenTypeA::InitReportHeaderUrban( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(1, _T("Max.# of Path"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KInterPathGenTypeA::InitReportHeaderRegional( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(1, _T("Max.# of path"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KInterPathGenTypeA::UpdateReportDataOD( void )
{
	try
	{
		std::map<CString, int> mapPurposeOD;
		std::map<CString, int> mapUsePurposeOD;
		KDBaseInterModal::GetPurposeODData(m_pTarget, mapPurposeOD);
		KDBaseInterModal::GetInterPurposeODData(m_pTarget, mapUsePurposeOD);

		AutoType iter = mapPurposeOD.begin();
		AutoType end  = mapPurposeOD.end();
		
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlOD.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->HasCheckbox(TRUE);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PURPOSE_OD);
			const KIOColumns* pIOColumns = pTable->Columns();
			int nColumnCount = pIOColumns->ColumnCount();
			KIOColumn* pUColumn = nullptr;

			KIOColumns oIOColumns;
			for(int i= 0; i< nColumnCount; i++)
			{
				KIOColumn* pColumn        = pIOColumns->GetColumn(i);
				if (pColumn->Name() == iter->first)
				{
					pUColumn = pColumn;
					AutoType find = mapUsePurposeOD.find(pColumn->Name());
					AutoType fend = mapUsePurposeOD.end();
					if (find != fend)
					{
						pItem->SetChecked(TRUE);
					}
				}
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pUColumn->Caption()));
			pItem->SetEditable(FALSE);
			pItem->SetItemData((DWORD_PTR)pUColumn);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
			++iter;
		}

		m_wndReportCtrlOD.Populate();

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

void KInterPathGenTypeA::UpdateReportDataUrban( void )
{
	try
	{
		std::map<int, TUrbanPathInfo> mapUrbanMode;
		InitUrbanModeData(mapUrbanMode);

		AutoType iter = mapUrbanMode.begin();
		AutoType end  = mapUrbanMode.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			TUrbanPathInfo oInfo = iter->second;
			CString strName = oInfo.strMode;
			CString strPath(_T(""));
			strPath.Format(_T("%d"), oInfo.nMaxofPath);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
			pItem->SetEditable(FALSE);
			pItem->SetItemData(iter->first);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nMode == P_MODE_AUTO || oInfo.nMode == P_MODE_TAXI)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPath));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}
			

			++iter;
		}
		m_wndReportCtrlUrban.Populate();

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

void KInterPathGenTypeA::InitUrbanModeData( std::map<int, TUrbanPathInfo>& a_mapUrbanMode )
{
	TUrbanPathInfo oAutoInfo;
	oAutoInfo.nMode			= P_MODE_AUTO;
	oAutoInfo.strMode		= _T("PassengerCar");
	oAutoInfo.nMaxofPath	= 1;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_AUTO, oAutoInfo));

	TUrbanPathInfo oTaxiInfo;
	oTaxiInfo.nMode			= P_MODE_TAXI;
	oTaxiInfo.strMode		= _T("Taxi");
	oTaxiInfo.nMaxofPath	= 1;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_TAXI, oTaxiInfo));

	TUrbanPathInfo oBusInfo;
	oBusInfo.nMode			= P_MODE_BUS;
	oBusInfo.strMode		= _T("Bus_Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oBusInfo.strMode		= _T("버스");
	}
	oBusInfo.nMaxofPath	= 10;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_BUS, oBusInfo));

	TUrbanPathInfo oSubwayInfo;
	oSubwayInfo.nMode			= P_MODE_RAIL;
	oSubwayInfo.strMode			= _T("Rail_Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oSubwayInfo.strMode		= _T("도시철도");
	}
	oSubwayInfo.nMaxofPath		= 10;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_RAIL, oSubwayInfo));

	TUrbanPathInfo oBSInfo;
	oBSInfo.nMode			= P_MODE_BUS_RAIL;
	oBSInfo.strMode			= _T("Composit_Mode");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oBSInfo.strMode		= _T("버스+도시철도");
	}
	oBSInfo.nMaxofPath		= 10;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_BUS_RAIL, oBSInfo));

	TUrbanPathInfo oPRInfo;
	oPRInfo.nMode			= P_MODE_PARK_RIDE;
	oPRInfo.strMode			= _T("Park & Ride");
	oPRInfo.nMaxofPath		= 5;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_PARK_RIDE, oPRInfo));

	TUrbanPathInfo oKRInfo;
	oKRInfo.nMode			= P_MODE_KISS_RIDE;
	oKRInfo.strMode			= _T("Kiss & Ride");
	oKRInfo.nMaxofPath		= 5;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_KISS_RIDE, oKRInfo));

	TUrbanPathInfo oTTInfo;
	oTTInfo.nMode			= P_MODE_TAXI_TRANSIT;
	oTTInfo.strMode			= _T("Taxi & Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oTTInfo.strMode		= _T("택시 & 대중교통");
	}
	oTTInfo.nMaxofPath		= 5;
	a_mapUrbanMode.insert(std::make_pair(P_MODE_TAXI_TRANSIT, oTTInfo));
}

void KInterPathGenTypeA::UpdateReportDataRegion( void )
{
	try
	{
		std::map<int, TRegionPathInfo> mapRegionMode;
		InitRegionModeData(mapRegionMode);

		AutoType iter = mapRegionMode.begin();
		AutoType end  = mapRegionMode.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlRegional.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			TRegionPathInfo oInfo = iter->second;
			CString strName = oInfo.strMode;
			CString strPath(_T(""));
			strPath.Format(_T("%d"), oInfo.nMaxofPath);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
			pItem->SetEditable(FALSE);
			pItem->SetItemData(iter->first);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nMode == -1)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPath));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}
			
			++iter;
		}
		m_wndReportCtrlRegional.Populate();

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

void KInterPathGenTypeA::InitRegionModeData( std::map<int, TRegionPathInfo>& a_mapRegionMode )
{
	TRegionPathInfo oAutoInfo;
	oAutoInfo.nMode = nREGION_PAHT_AUTO;
	oAutoInfo.strMode =_T("PassengerCar");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oAutoInfo.strMode = _T("Auto");
	}
	oAutoInfo.nMaxofPath = 1;
	a_mapRegionMode.insert(std::make_pair(oAutoInfo.nMode, oAutoInfo));

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
	std::map<int,CString> mapRegionMode;
	pCodeGroup->GetCodes(mapRegionMode);

	AutoType iter = mapRegionMode.begin();
	AutoType end  = mapRegionMode.end();
	while(iter != end)
	{
		TRegionPathInfo oInfo;
		oInfo.nMode = iter->first;
		oInfo.strMode = iter->second;
		oInfo.nMaxofPath = 5;
		a_mapRegionMode.insert(std::make_pair(oInfo.nMode, oInfo));
		++iter;
	}

	TRegionPathInfo oAEInfo;
	oAEInfo.nMode = nREGION_PATH_AE;
	oAEInfo.strMode =_T("AccEgrPath");
    if (KmzSystem::GetLanguage() == KEMKorea) {
        oAEInfo.strMode =_T("출발/도착 접근경로");
    }

	oAEInfo.nMaxofPath = 5;
	a_mapRegionMode.insert(std::make_pair(oAEInfo.nMode, oAEInfo));
}

void KInterPathGenTypeA::ArgumentPurPoseOD( KIOColumns& oPurposeColumnCollection, CStdioFileEx& of )
{
	try
	{
		CXTPReportRecords*          pRecords = m_wndReportCtrlOD.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;

		int nRecordCount = pRecords->GetCount();
		int nSelectCount(0);
		CString strOut(_T(""));

		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem = pRecord->GetItem(0);
			if( TRUE == pItem->IsChecked() )
			{
				pItem = pRecord->GetItem(1);
				KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
				oPurposeColumnCollection.AddNewColumn(pColumn);
				nSelectCount++;
			}
		}

		strOut.Format(_T("PurposeOD\t%d\r\n"), nSelectCount);
		of.WriteString(strOut);
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

void KInterPathGenTypeA::ArgumentUrbanPath( CStdioFileEx& of )
{
	try
	{
		CXTPReportRecords*          pRecords = m_wndReportCtrlUrban.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;

		CString strOut(_T(""));

		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 
			
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strPath		= pItemTXT->GetValue();
			int     nPath		= _ttoi(strPath);

			if (nModeID != P_MODE_AUTO && nModeID != P_MODE_TAXI)
			{
				strOut.Format(_T("%s\t%d\r\n"), strModeName, nPath);
				of.WriteString(strOut);
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

void KInterPathGenTypeA::ArgumentRegionalPath( CStdioFileEx& of )
{
	try
	{
		CXTPReportRecords*          pRecords = m_wndReportCtrlRegional.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;

		CString strOut(_T(""));
		CString strAETransit(_T(""));
		int nstrATnPath(0);

		int nRecordCount = pRecords->GetCount();

		strOut.Format(_T("noModeR\t%d\r\n"), nRecordCount - 1);
		of.WriteString(strOut);

		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 

			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strPath		= pItemTXT->GetValue();
			int     nPath		= _ttoi(strPath);

			if (nModeID != nREGION_PAHT_AUTO)
			{
				if (nModeID == nREGION_PATH_AE)
				{
					strAETransit = strModeName;
					nstrATnPath  = nPath;
				}
				else
				{
					strOut.Format(_T("%s\r\n"), strModeName);
					of.WriteString(strOut);
					strOut.Format(_T("%d\r\n"), nPath);
					of.WriteString(strOut);
				}
			}
		}

		strOut.Format(_T("%s\r\n"), strAETransit);
		of.WriteString(strOut);

		strOut.Format(_T("%d\r\n"), nstrATnPath);
		of.WriteString(strOut);
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

void KInterPathGenTypeA::SaveResultOD2DB( void )
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDelSQL(_T(""));
		strDelSQL.Format(_T("Delete From %s"), TABLE_INTER_PURPOSE_OD_INFO);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		KIOColumns oPurposeColumnCollection;
		CXTPReportRecords*          pRecords = m_wndReportCtrlOD.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;

		int nRecordCount = pRecords->GetCount();
		
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem = pRecord->GetItem(0);
			if( TRUE == pItem->IsChecked() )
			{
				pItem = pRecord->GetItem(1);
				KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
				oPurposeColumnCollection.AddNewColumn(pColumn);
			}
		}

		int nCount = oPurposeColumnCollection.ColumnCount();
		for (int i =0; i <nCount; i++)
		{
			KIOColumn* pColumn = oPurposeColumnCollection.GetColumn(i);
			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(purpose_id, purpose_column) Values(%d, '%s')"),TABLE_INTER_PURPOSE_OD_INFO,  i+ 1, pColumn->Name());
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
	
}

void KInterPathGenTypeA::SaveResultUrban2DB( void )
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDelSQL(_T(""));
		strDelSQL.Format(_T("Delete From %s"), TABLE_INTER_URBAN_PATH_INFO);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		CXTPReportRecords*          pRecords = m_wndReportCtrlUrban.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 

			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strPath		= pItemTXT->GetValue();
			int     nPath		= _ttoi(strPath);

			if (nModeID != P_MODE_AUTO && nModeID != P_MODE_TAXI)
			{
				CString strSQL(_T(""));
				strSQL.Format(_T("Insert Into %s(id, mode, no_path) Values(%d, '%s', %d)"), 
					TABLE_INTER_URBAN_PATH_INFO, nModeID, strModeName, nPath);
				spDBaseConnection->ExecuteUpdate(strSQL);
			}
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}

void KInterPathGenTypeA::SaveResultRegion2DB( void )
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDelSQL(_T(""));
		CString strSQL(_T(""));
		strDelSQL.Format(_T("Delete From %s"), TABLE_INTER_REGION_PATH_INFO);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		CXTPReportRecords*          pRecords = m_wndReportCtrlRegional.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;

		int nAETID(0);
		CString strAETransit(_T(""));
		int nAEnoPath(0);

		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 

			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strPath		= pItemTXT->GetValue();
			int     nPath		= _ttoi(strPath);

			if (nModeID != nREGION_PAHT_AUTO)
			{
				if (nModeID == nREGION_PATH_AE)
				{
					nAETID = nModeID;
					strAETransit = strModeName;
					nAEnoPath  = nPath;
				}
				else
				{
					strSQL.Format(_T("Insert Into %s(id, name, no_path) Values (%d, '%s', %d)"),TABLE_INTER_REGION_PATH_INFO,
						nModeID, strModeName, nPath);
					spDBaseConnection->ExecuteUpdate(strSQL);
				}
			}
		}

		strSQL.Format(_T("Insert Into %s(id, name, no_path) Values (%d, '%s', %d)"),TABLE_INTER_REGION_PATH_INFO,
			nAETID, strAETransit, nAEnoPath);
		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}
