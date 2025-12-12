// EditAccessLineStationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EditAccessLineStationDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "KExRecordItem.h"
#include "CustomReportRecordItem.h"
#include "ShortestPathBase.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"

enum
{
	_0_COLUMN_SELECT      = 0,
	_1_COLUMN_STATIONID   = 1,
	_2_COLUMN_CLASS       = 2
};

// KEditAccessLineStationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KEditAccessLineStationDlg, KResizeDialogEx)

KEditAccessLineStationDlg::KEditAccessLineStationDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KEditAccessLineStationDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nxZoneID(0)
	, m_nxLineID(0)
	, KEditAccessLineStation(a_pTarget, m_nxZoneID, m_nxLineID)
{

}

KEditAccessLineStationDlg::~KEditAccessLineStationDlg()
{
}

void KEditAccessLineStationDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KEditAccessLineStationDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KEditAccessLineStationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KEditAccessLineStationDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,  &KEditAccessLineStationDlg::OnReportItemClick)
END_MESSAGE_MAP()


// KEditAccessLineStationDlg 메시지 처리기입니다.
void KEditAccessLineStationDlg::ShowInformation(Integer a_nxZoneID, Integer a_nxLineID)
{
	m_nxZoneID  = a_nxZoneID;
	m_nxLineID	= a_nxLineID;
	LoadInit();
}


BOOL KEditAccessLineStationDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	LoadAccessClass();
	if(m_mapClassNameId.size() < 1)
	{
		AfxMessageBox(_T("접근 등급 정보가 존재하지 않습니다."));
		//등급 정보 설정 하는 Dlg 오픈
		return FALSE;
	}
	KReportCtrlSetting::DefaultC(m_wndReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);
	InitReportHeader();
	ResizeComponent();
	
	return TRUE; 
}


void KEditAccessLineStationDlg::LoadInit()
{
	CString strTargetYear = KShortestPathBase::DisplayScenarioTitle( m_pTarget->Caption() );
	SetDlgItemText(IDC_STATIC_TARGETYEAR, strTargetYear);

	CString strZoneID(_T(""));
	CString strLineID(_T(""));
	strZoneID.Format(_T("Zone ID : %I64d"), m_nxZoneID);
	strLineID.Format(_T("Line ID : %I64d"), m_nxLineID);

	SetDlgItemText(IDC_STATIC3, strZoneID);
	SetDlgItemText(IDC_STATIC4, strLineID);

	std::vector<TStationAccessInfo> vecAllStation;
	std::map<Integer, int>          mapAccessStation;  /*노드ID, 등급ID*/

	LoadAccessStation(m_mapClassNameId, mapAccessStation);
	LoadStationID    (vecAllStation);
	JoinStaion       (mapAccessStation, vecAllStation);

	InitReportData(vecAllStation);
}


void KEditAccessLineStationDlg::LoadAccessClass()
{
	TxLogDebugStartMsg();

	m_mapClassNameId.clear();
	m_mapClasssIdName.clear();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT class_id, class_name FROM access_class "), TABLE_ACCESS_CLASS);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			int     nClassID      = pResult->GetValueInt(0);
			CString strClassName  = pResult->GetValueString(1);  

			m_mapClassNameId.insert(std::make_pair(strClassName, nClassID));
			m_mapClasssIdName.insert(std::make_pair(nClassID, strClassName));
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KEditAccessLineStationDlg::LoadStationID(std::vector<TStationAccessInfo> &a_vecAllStation)
{
	TxLogDebugStartMsg();

	a_vecAllStation.clear();
	std::set<Integer> setStationID;

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT Seq, TBL2.Link_ID AS Link_Id, FNode_Id, TNode_Id ")
			_T(" FROM ")
			_T(" (SELECT link_id, fnode_id, tnode_id FROM %s)TBL1, ")
			_T(" (SELECT Link_ID, Seq FROM %s WHERE transit_id = %I64d and station_yn = 1)TBL2 ")
			_T(" WHERE ")
			_T(" TBL1.Link_id = TBL2.Link_ID ORDER BY Seq "), TABLE_LINK, TABLE_TRANSITLINKS, m_nxLineID); //★ 필요

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TStationAccessInfo oTStationAccessInfo = {0, 0};
			int nSeq               = pResult->GetValueInt(0);   //seq
			
			if( 1 == nSeq )
			{
				Integer nxFNodeID  = pResult->GetValueInt64(2);  //FNode_Id 
				if ( !ISDuplicateStaioID(nxFNodeID, setStationID))
				{
					oTStationAccessInfo.nxStationID = nxFNodeID;     
					a_vecAllStation.push_back(oTStationAccessInfo);
				}
			}

			Integer nxTNodeID     = pResult->GetValueInt64(3);  //TNode_Id 
			if ( !ISDuplicateStaioID(nxTNodeID, setStationID))
			{
				oTStationAccessInfo.nxStationID = nxTNodeID;     
				a_vecAllStation.push_back(oTStationAccessInfo);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


bool KEditAccessLineStationDlg::ISDuplicateStaioID(Integer a_nxStationID, std::set<Integer> &a_setStationID)
{
	bool bDuplicate = false;
	std::set<Integer>::iterator iter = a_setStationID.find(a_nxStationID);
	
	if(iter != a_setStationID.end())
	{
		bDuplicate = true;
	}
	a_setStationID.insert(a_nxStationID);

	return bDuplicate;
}


void KEditAccessLineStationDlg::LoadAccessStation(std::map<CString, int> &a_mapClass, std::map<Integer, int> &a_mapAccessStation)
{
	TxLogDebugStartMsg();

	std::map<CString, int>::iterator itClass  = a_mapClass.begin();
	std::map<CString, int>::iterator itEnd    = a_mapClass.end();

	a_mapAccessStation.clear();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT Station_access FROM %s ")
			_T(" WHERE zone_id = %I64d AND line_id = %I64d "), TABLE_ACCESS_LINE_STATION, m_nxZoneID, m_nxLineID);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			CString strStationAccess = pResult->GetValueString(0);

			int     nCount  = 0;
			int     nPos    = 0;
			CString strTemp = strStationAccess;
			strTemp.Trim();
			CString strPart = strTemp.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			while( strPart != _T(""))
			{
				CString  strStationNodeID = strPart.Mid(0,                          strPart.Find(_T(':')) );
				CString  strClassName     = strPart.Mid(strPart.Find(_T(':'))+1,    strPart.GetLength()   );
				
				Integer  nxStationID      = _ttoi64(strStationNodeID);
				int      nClassID         = 0;

				itClass = a_mapClass.find(strClassName);
				if(itClass != itEnd)
				{
					nClassID = itClass->second;
				}

				a_mapAccessStation.insert(std::make_pair(nxStationID, nClassID));

				strPart = strStationAccess.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KEditAccessLineStationDlg::JoinStaion(std::map<Integer, int> &a_mapAccessStation, std::vector<TStationAccessInfo> &a_vecAllStation)
{
	std::map<Integer, int>::iterator itAccess  = a_mapAccessStation.begin();
	std::map<Integer, int>::iterator itEnd     = a_mapAccessStation.end();

	for(size_t i =0; i< a_vecAllStation.size(); i++)
	{
		Integer &nxStationID = a_vecAllStation[i].nxStationID;
		int     &nClassID    = a_vecAllStation[i].nClassID;

		itAccess = a_mapAccessStation.find(nxStationID);
		if(itAccess != itEnd)
		{
			nClassID = itAccess->second; 
		}
	}
}


void KEditAccessLineStationDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;
	CXTPReportRecordItemEditOptions* pEditOptions = nullptr;
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,       _T("선택"),        60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_STATIONID,    _T("Station ID"),    50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_CLASS,        _T("Class"),      50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pEditOptions = pColumn->GetEditOptions();
	pEditOptions->AddComboButton(TRUE);
	pEditOptions->m_bAllowEdit = TRUE;
	pEditOptions->m_bExpandOnSelect = TRUE;

	std::map<CString, int>::iterator iter   = m_mapClassNameId.begin();
	std::map<CString, int>::iterator itEnd  = m_mapClassNameId.end();
	pEditOptions->AddConstraint(_T("-"), (DWORD_PTR)0 ); 
	while(iter != itEnd)
	{
		pEditOptions->AddConstraint(iter->first, (DWORD_PTR)iter->second); 
		++iter;
	}

	m_wndReportCtrl.Populate();
}


void KEditAccessLineStationDlg::InitReportData(std::vector<TStationAccessInfo> &a_vecAllStation)
{
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (size_t i= 0; i< a_vecAllStation.size(); i++)
	{
		Integer &nxStationID = a_vecAllStation[i].nxStationID;
		int     &nClassID    = a_vecAllStation[i].nClassID;

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		if(nClassID > 0)
			pItem->SetChecked(TRUE);
		else
			pItem->SetChecked(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxStationID,    _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextCenter);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData((DWORD_PTR)nClassID);
	}

	m_wndReportCtrl.Populate();
}


void KEditAccessLineStationDlg::ResizeComponent()
{
	SetResize(IDC_STATIC_TARGETYEAR,	      SZ_TOP_LEFT,    SZ_TOP_RIGHT);

	SetResize(IDC_STATIC4,                    SZ_TOP_CENTER,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,					  SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,			      SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
		
 	SetResize(IDOK,						      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
 	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KEditAccessLineStationDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KEditAccessLineStationDlg::OnBnClickedOk()
{
	if ( AfxMessageBox(_T("수정하시겠습니까?"), MB_YESNO) == IDNO )
	{
		return;
	}

	try
	{
		UpdateStationAccess();
		DeleteRelationFile();
	}
	catch (...)
	{
		return;	
	}

	KResizeDialogEx::OnOK();
}


void KEditAccessLineStationDlg::UpdateStationAccess()
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString          strUpdateQuery  (_T(""));
	CString          strStationAccess(_T(""));
	std::map<int, CString>::iterator iter = m_mapClasssIdName.begin();
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemNumber*  pItemNum      = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		
		int nRecordCount           = pRecords->GetCount();
		int nNum = 0;
		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord               = pRecords->GetAt(i);
			pItem                 = pRecord->GetItem(_0_COLUMN_SELECT);
			BOOL bChecked         = pItem->IsChecked(); 
			if(FALSE == bChecked)
				continue;
			
			pItemNum              = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_STATIONID);
			Integer nxStationID   = (Integer)pItemNum->GetValue();

			pItem                 = pRecord->GetItem(_2_COLUMN_CLASS);
			int     nClassID      = (int)pItem->GetItemData();
			if(nClassID < 1)
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("ID : %I64d 의 등급을 설정 하세요"), nxStationID);
				ThrowException(strMsg);
			}

			iter = m_mapClasssIdName.find(nClassID);
			if(iter == m_mapClasssIdName.end())
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("ID : %I64d 의 등급을 확인 하세요"), nxStationID);
				ThrowException(strMsg);
			}
			CString strClassName  = iter->second;

			CString strNodeClass(_T(""));
			strNodeClass.Format(_T("%I64d:%s"), nxStationID, strClassName);

			if( 0 != nNum)
			{
				strStationAccess.Append(COLUMN_STATION_ACCESS_SEPARATOR);
			}
			strStationAccess.Append(strNodeClass);
			
			++nNum;
		}

		if(strStationAccess.Compare(_T("")) == 0)
		{
			ThrowException(_T("정류장을 선택해 주세요"));
		}

		strUpdateQuery.Format(
			_T(" UPDATE %s SET no_station = %d, station_access = '%s' WHERE zone_id = %I64d AND line_id = %I64d "), TABLE_ACCESS_LINE_STATION, nNum, strStationAccess, m_nxZoneID, m_nxLineID);

		spDBaseConnection->ExecuteUpdate(strUpdateQuery);

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ACCESS_LINE_STATION);
		pIOTable->Notify();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KEditAccessLineStationDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if(_0_COLUMN_SELECT == nIndex)
	{
		if( FALSE == pItemNotify->pItem->IsChecked() )
		{
			CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_2_COLUMN_CLASS);
			pItem->SetItemData(0);
			m_wndReportCtrl.RedrawControl();
		}
	}
}