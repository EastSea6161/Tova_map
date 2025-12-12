// NetworkOnOffDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NetworkOnOffDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "DefineNames.h"
#include "KExRecordItem.h"

enum 
{
	_0_COLUMN_NODE_CHECK = 0,
	_1_COLUMN_NODE_TYPE,
	_2_COLUMN_NODE_NAME,
	_3_COLUMN_NODE_COUNT

};

enum 
{
	_0_COLUMN_LINK_CHECK = 0,
	_1_COLUMN_LINK_TYPE,
	_2_COLUMN_LINK_NAME,
	_3_COLUMN_LINK_COUNT
};

#define CODE_GROUP_KEY_NODE 2
#define CODE_GROUP_KEY_LINK 3

#define NETWORK_OFF FALSE
#define NETWORK_ON  TRUE

// KNetworkOnOffDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNetworkOnOffDlg, CXTPResizeDialog)

KNetworkOnOffDlg::KNetworkOnOffDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KNetworkOnOffDlg::IDD, pParent)
{

}

KNetworkOnOffDlg::~KNetworkOnOffDlg()
{
}

void KNetworkOnOffDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_ReportNode);
	DDX_Control(pDX, IDC_REPORT2, m_ReportLink);
}


BEGIN_MESSAGE_MAP(KNetworkOnOffDlg, CXTPResizeDialog)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT1, OnNodeReportColumnRButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT2, OnLinkReportColumnRButtonClick)
	ON_COMMAND(ID_SELECT_CANCEL,        &KNetworkOnOffDlg::OnNodeSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,           &KNetworkOnOffDlg::OnNodeSelectAll)
	ON_COMMAND(ID_SELECT_CANCEL_SECOND, &KNetworkOnOffDlg::OnLinkSelectCancel)
	ON_COMMAND(ID_SELECT_ALL_SECOND,    &KNetworkOnOffDlg::OnLinkSelectAll)
	ON_BN_CLICKED(IDOK, &KNetworkOnOffDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KNetworkOnOffDlg 메시지 처리기입니다.

BOOL KNetworkOnOffDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	if(m_pTarget != NULL)
	{
		CString strTargetYear = DisplayScenarioTitle( m_pTarget->Caption() );
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strTargetYear);
	}

	InitReportHeader();
	m_ReportNode.Populate();
	m_ReportNode.CanCopy();
	m_ReportNode.SetGridStyle(TRUE, xtpReportGridSolid);    

	m_ReportLink.Populate();
	m_ReportLink.CanCopy();
	m_ReportLink.SetGridStyle(TRUE, xtpReportGridSolid);   

	InitReportData();

	SetResizeReportControl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


CString KNetworkOnOffDlg::DisplayScenarioTitle( CString a_strTitle )
{
	CString strReturn = _T("");
	strReturn.Format(_T("[ %s ]"), a_strTitle);

	return strReturn;
}


void KNetworkOnOffDlg::InitReportHeader()
{
	InitNodeReportHeader();
	IntiLinkReportHeader();
}


void KNetworkOnOffDlg::InitNodeReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	m_ReportNode.SelectionEnable(FALSE);

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_0_COLUMN_NODE_CHECK,   _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_1_COLUMN_NODE_TYPE,    _T("Type"),   25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_2_COLUMN_NODE_NAME,    _T("Name"),   50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_3_COLUMN_NODE_COUNT,   _T("Count"),  25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	CXTPReportHeader* pHeader = m_ReportNode.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(TRUE);
}


void KNetworkOnOffDlg::IntiLinkReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	m_ReportLink.SelectionEnable(FALSE);

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_0_COLUMN_LINK_CHECK,   _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_1_COLUMN_LINK_TYPE,    _T("Type"),   25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_2_COLUMN_LINK_NAME,    _T("Name"),   50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_3_COLUMN_LINK_COUNT,   _T("Count"),  25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	CXTPReportHeader* pHeader = m_ReportLink.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(TRUE);
}


void KNetworkOnOffDlg::SetResizeReportControl()
{
	SetResize(IDC_STATIC_TARGETYEAR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	
	SetResize(IDC_STATIC4,		SZ_TOP_CENTER,  SZ_TOP_RIGHT);				// Edit Text - 'Codes'

	SetResize(IDC_STATIC3,		SZ_TOP_CENTER,  SZ_BOTTOM_CENTER);			// 가운데 세로 라인

	SetResize(IDC_REPORT1,		SZ_TOP_LEFT,     SZ_BOTTOM_CENTER);
	SetResize(IDC_REPORT2,		SZ_TOP_CENTER,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC2,		SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);			// 가로 라인

	SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KNetworkOnOffDlg::InitReportData()
{
	InitNodeReportData();
	InitLinkReportData();
}


void KNetworkOnOffDlg::InitNodeReportData()
{
	GetNodeTypeInfo(m_mapNodeGroupOnOff);

	m_ReportNode.ResetContent(TRUE);

	CXTPReportRecord     *pRecord = NULL;
	CXTPReportRecordItem *pItem   = NULL;

	std::map<int, TNodeGroupOnOff>::iterator itBegin = m_mapNodeGroupOnOff.begin();
	std::map<int, TNodeGroupOnOff>::iterator itEnd   = m_mapNodeGroupOnOff.end();

	while(itBegin != itEnd)
	{
		TNodeGroupOnOff        &oTNodeGroupOnOff = itBegin->second;
		BOOL &bOnOff          = oTNodeGroupOnOff.bOnOff;
		int  &nGroupType      = oTNodeGroupOnOff.nGroupType;
		CString &strGroupName = oTNodeGroupOnOff.strGroupName;
		int &nUsedCount       = oTNodeGroupOnOff.nUsedCount;

		pRecord = m_ReportNode.AddRecord(new CXTPReportRecord());

		pItem   = pRecord->AddItem(new KExRecordItemCheck());
		pItem->SetChecked(bOnOff);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)nGroupType);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nGroupType, _T("%.0f")));
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData((DWORD_PTR) &oTNodeGroupOnOff);
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strGroupName));
		pItem->SetAlignment(DT_LEFT);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nUsedCount, _T("%.0f")));
		pItem->SetAlignment(DT_RIGHT);

		++itBegin;
	}

	m_ReportNode.Populate();
}


void KNetworkOnOffDlg::InitLinkReportData()
{
	GetLinkTypeInfo(m_mapLinkGroupOnOff);

	m_ReportLink.ResetContent(TRUE);

	CXTPReportRecord     *pRecord = NULL;
	CXTPReportRecordItem *pItem   = NULL;

	std::map<int, TLinkGroupOnOff>::iterator itBegin = m_mapLinkGroupOnOff.begin();
	std::map<int, TLinkGroupOnOff>::iterator itEnd   = m_mapLinkGroupOnOff.end();

	while(itBegin != itEnd)
	{
		TLinkGroupOnOff        &oTLinkGroupOnOff = itBegin->second;
		BOOL &bOnOff          = oTLinkGroupOnOff.bOnOff;
		int  &nGroupType      = oTLinkGroupOnOff.nGroupType;
		CString &strGroupName = oTLinkGroupOnOff.strGroupName;
		int &nUsedCount       = oTLinkGroupOnOff.nUsedCount;

		pRecord = m_ReportLink.AddRecord(new CXTPReportRecord());

		pItem   = pRecord->AddItem(new KExRecordItemCheck());
		pItem->SetChecked(bOnOff);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)nGroupType);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nGroupType, _T("%.0f")));
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData((DWORD_PTR) &oTLinkGroupOnOff);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strGroupName));
		pItem->SetAlignment(DT_LEFT);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nUsedCount, _T("%.0f")));
		pItem->SetAlignment(DT_RIGHT);

		++itBegin;
	}

	m_ReportLink.Populate();
}


void KNetworkOnOffDlg::OnNodeReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_NODE_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KNetworkOnOffDlg::OnLinkReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK_SECOND);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_LINK_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KNetworkOnOffDlg::OnNodeSelectCancel()
{
	CheckboxControl(&m_ReportNode, _0_COLUMN_NODE_CHECK, FALSE);
}


void KNetworkOnOffDlg::OnNodeSelectAll()
{
	CheckboxControl(&m_ReportNode, _0_COLUMN_NODE_CHECK, TRUE);
}


void KNetworkOnOffDlg::OnLinkSelectCancel()
{
	CheckboxControl(&m_ReportLink, _0_COLUMN_LINK_CHECK, FALSE);
}


void KNetworkOnOffDlg::OnLinkSelectAll()
{
	CheckboxControl(&m_ReportLink, _0_COLUMN_LINK_CHECK, TRUE);
}


void KNetworkOnOffDlg::CheckboxControl(CXTPReportControl *pReport, int a_nCheckFieldIdx,  BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = pReport->GetRecords();
	int nRecordCount = pRecords->GetCount();

	bool bRedraw = false;
	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(a_nCheckFieldIdx);
		pItemCheck->SetChecked(a_bCheck);
	}

	pReport->RedrawControl();
}


void KNetworkOnOffDlg::OnBnClickedOk()
{
	CString strNodeOffTypes(_T(""));
	CString strLinkOffTypes(_T(""));
	UpdateNodeOffTypes(strNodeOffTypes);
	UpdateLinkOffTypes(strLinkOffTypes);

	SaveNetworkOff(strNodeOffTypes, strLinkOffTypes);

	CXTPResizeDialog::OnOK();
}


void KNetworkOnOffDlg::UpdateNodeOffTypes(CString &a_strNodeOffTypes)
{
	CXTPReportRecords    *pRecords = m_ReportNode.GetRecords();
	CXTPReportRecord     *pRecord  = NULL;
	CXTPReportRecordItem *pItem    = NULL;

	int nRecordCount = pRecords->GetCount();

	a_strNodeOffTypes = _T("");
	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);

		pItem = pRecord->GetItem(_0_COLUMN_NODE_CHECK);
		if ( TRUE == pItem->IsChecked() )
			continue;

		int     nNodeOffType = (int)pItem->GetItemData();
		CString strType(_T(""));
		strType.Format(_T("%d"), nNodeOffType);

		a_strNodeOffTypes += strType;
		a_strNodeOffTypes += _T(",");
	}
	a_strNodeOffTypes.TrimRight( _T(",") );
}


void KNetworkOnOffDlg::UpdateLinkOffTypes(CString &a_strLinkOffTypes)
{
	CXTPReportRecords    *pRecords = m_ReportLink.GetRecords();
	CXTPReportRecord     *pRecord  = NULL;
	CXTPReportRecordItem *pItem    = NULL;

	int nRecordCount = pRecords->GetCount();

	a_strLinkOffTypes = _T("");
	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);

		pItem = pRecord->GetItem(_0_COLUMN_LINK_CHECK);
		if ( TRUE == pItem->IsChecked() )
			continue;

		int     nLinkOffType = (int)pItem->GetItemData();
		CString strType(_T(""));
		strType.Format(_T("%d"), nLinkOffType);

		a_strLinkOffTypes += strType;
		a_strLinkOffTypes += _T(",");
	}
	a_strLinkOffTypes.TrimRight( _T(",") );
}


void KNetworkOnOffDlg::SaveNetworkOff(CString &a_strNodeOffTypes, CString &a_strLinkOffTypes)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strDelSQL(_T(""));
	CString strSQL(_T(""));

	strDelSQL.Format(_T(" DELETE FROM Network_Off "));
	strSQL.Format(_T(" INSERT OR REPLACE INTO Network_Off (node_off, link_off) Values ('%s', '%s') "),  a_strNodeOffTypes, a_strLinkOffTypes );

	try
	{
		spDBaseConnection->BeginTransaction();

		spDBaseConnection->ExecuteUpdate(strDelSQL);
		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugWarning();
	}

	TxLogDebugEndMsg();
}


void KNetworkOnOffDlg::GetNodeTypeInfo(std::map<int, TNodeGroupOnOff> &a_mapNodeGroupOnOff)
{
	TxLogDebugStartMsg();

	std::set<int> setNodeOffType;

	GetNodeOffInfo(setNodeOffType);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(	_T(" SELECT TBL2.node_type AS node_type, TBL1.KCode_Display AS KCode_Display, TBL2.used_count AS used_count ")
					_T(" FROM ")
					_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL1, ")
					_T(" (SELECT node_type, count(*) AS used_count FROM node GROUP BY node_type)TBL2 ")
					_T("  WHERE TBL1.KCode_Key = TBL2.node_type "), CODE_GROUP_KEY_NODE          
				 );

	try
	{
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		a_mapNodeGroupOnOff.clear();

		while(pResult->Next())
		{
			TNodeGroupOnOff oTNodeGroupOnOff;

			int nGroupType                = pResult->GetValueInt(0);
			oTNodeGroupOnOff.nGroupType   = nGroupType;
			oTNodeGroupOnOff.strGroupName = pResult->GetValueString(1);
			oTNodeGroupOnOff.bOnOff       = IsNetworkOnOff(setNodeOffType, nGroupType);
			oTNodeGroupOnOff.nUsedCount   = pResult->GetValueInt(2);

			a_mapNodeGroupOnOff.insert(std::make_pair(nGroupType, oTNodeGroupOnOff));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	}
	
	TxLogDebugEndMsg();
}


void KNetworkOnOffDlg::GetLinkTypeInfo(std::map<int, TLinkGroupOnOff> &a_mapLinkGroupOnOff)
{
	TxLogDebugStartMsg();

	std::set<int> setLinkOffType;

	GetLinkOffInfo(setLinkOffType);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(	_T(" SELECT TBL2.link_type AS link_type, TBL1.KCode_Display AS KCode_Display, TBL2.used_count AS used_count ")
					_T(" FROM ")
					_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL1, ")
					_T(" (SELECT link_type, count(*) AS used_count FROM link GROUP BY link_type)TBL2 ")
					_T(" WHERE TBL1.KCode_Key = TBL2.link_type "), CODE_GROUP_KEY_LINK           
		);

	try
	{
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		a_mapLinkGroupOnOff.clear();

		while(pResult->Next())
		{
			pResult->GetValueInt(0);
			pResult->GetValueString(1);

			TLinkGroupOnOff oTLinkGroupOnOff;

			int nGroupType                = pResult->GetValueInt(0);
			oTLinkGroupOnOff.nGroupType   = nGroupType;
			oTLinkGroupOnOff.strGroupName = pResult->GetValueString(1);
			oTLinkGroupOnOff.bOnOff       = IsNetworkOnOff(setLinkOffType, nGroupType);
			oTLinkGroupOnOff.nUsedCount   = pResult->GetValueInt(2);

			a_mapLinkGroupOnOff.insert(std::make_pair(nGroupType, oTLinkGroupOnOff));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	}

	TxLogDebugEndMsg();
}


void KNetworkOnOffDlg::GetNodeOffInfo(std::set<int> &a_setNodeOffType)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(_T(" Select node_off from %s "), TABLE_NETWORKOFF);

	try
	{
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strNodeOff(_T(""));

		while( pResult->Next() )
		{
			strNodeOff = pResult->GetValueString(0);
		}

		int nSub = 0;
		CString strElement(_T(""));

		while(AfxExtractSubString(strElement, strNodeOff, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}

			int nVal = _ttoi(strElement);
			a_setNodeOffType.insert( nVal );
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	TxLogDebugEndMsg();
}


void KNetworkOnOffDlg::GetLinkOffInfo(std::set<int> &a_setLinkOffType)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(_T(" Select link_off from %s "), TABLE_NETWORKOFF);

	try
	{
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLinkOff(_T(""));

		while( pResult->Next() )
		{
			strLinkOff = pResult->GetValueString(0);
		}

		int nSub = 0;
		CString strElement(_T(""));

		while(AfxExtractSubString(strElement, strLinkOff, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}

			int nVal = _ttoi(strElement);
			a_setLinkOffType.insert( nVal );
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	TxLogDebugEndMsg();
}


BOOL KNetworkOnOffDlg::IsNetworkOnOff(std::set<int> &a_setNetworkOnOff, int &a_nGroupType)
{
	BOOL bOnOff = NETWORK_ON; 

	std::set<int>::iterator iter = a_setNetworkOnOff.find(a_nGroupType);

	if(iter != a_setNetworkOnOff.end())
		bOnOff = NETWORK_OFF;
	else
		bOnOff = NETWORK_ON;

	return bOnOff;
}


