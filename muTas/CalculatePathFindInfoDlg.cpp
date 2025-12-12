// CalculatePathFindInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CalculatePathFindInfoDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "PurposeInfoCollection.h"

#include "PurposeManager.h"
#include "Purpose.h"

//^#include "DBaseConnector.h"
#include "ShortestPathInfo.h"
#include "KBulkDBase.h"
#include "DefineNames.h"

#define PathFind_PURPOSE    0
#define PathFind_SELECTION	1

#define Link_Type_Zone               0
#define LInk_Type_Highway            1
#define LInk_Type_Expressway         2
#define LInk_Type_GeneralRoad        3
#define LInk_Type_metropolitanRoad   4
#define LInk_Type_StateLocalRoad     5
#define LInk_Type_LocalRoad          6
#define LInk_Type_SiGunRoad          7
#define LInk_Type_AccessRoad         8
#define LInk_Type_RailWay            9
#define LInk_Type_ShippingWay        70
#define LInk_Type_Shuttle            71


using namespace CalculatePathLength_Structure;


IMPLEMENT_DYNAMIC(KCalculatePathFindInfoDlg, KDialogEx)

KCalculatePathFindInfoDlg::KCalculatePathFindInfoDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KCalculatePathFindInfoDlg::IDD, pParent)
{

}

KCalculatePathFindInfoDlg::~KCalculatePathFindInfoDlg()
{
}

void KCalculatePathFindInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KCalculatePathFindInfoDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KCalculatePathFindInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void KCalculatePathFindInfoDlg::SetInfo( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}

	m_pTarget = a_pTarget;
}

BOOL KCalculatePathFindInfoDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initReportControl();
	updatePurposeRecord();
	ClearFile();

	return TRUE;
}

void KCalculatePathFindInfoDlg::initReportControl( void )
{
	// Report Header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(PathFind_PURPOSE, _T("Commdity"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(PathFind_SELECTION, _T("Selection"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	// Report control 설정
	m_wndReport.AllowEdit(FALSE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
}


void KCalculatePathFindInfoDlg::updatePurposeRecord()
{
	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter = purposeInfos.begin();
	std::list<KPurpose*>::iterator end  = purposeInfos.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;
		CString strCommdityname = pPurpose->PurposeName();
		int     nComdityID      = pPurpose->PurposeID();

		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(strCommdityname);
		pItem->SetItemData((DWORD_PTR)pPurpose);
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(TRUE);
		++iter;
	}
	m_wndReport.Populate();
}



void KCalculatePathFindInfoDlg::OnBnClickedOk()
{
	TxLogDebugStartMsg();
	try
	{
		ClearNodeArrivedMInInfo();
		SelectCommdityList();
		GetZoneNodeInfo();
		GetRoadFacilitiesNodeInfo();
		GetRailFacilitiesNodeInfo();
		GetExcludeCommdityNode();
		GetRoadLinkData();
		GetRailLinkData();
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(CalaculatePathThreadCaller, this, true, nLang);

	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("failed to run"));
		return;
	}

	TxLogDebugEndMsg();
	KDialogEx::OnOK();
}


void KCalculatePathFindInfoDlg::ClearNodeArrivedMInInfo()
{
	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRO =  m_mapRoadOriginMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRO  =  m_mapRoadOriginMinLink.end();
	while (iterRO != endRO)
	{
		KNodeArrivedMinLink* pMinLink = iterRO->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRO++;
	}
	m_mapRoadOriginMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRD =  m_mapRoadDestinationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRD  =  m_mapRoadDestinationMinLink.end();
	while (iterRD != endRD)
	{
		KNodeArrivedMinLink* pMinLink = iterRD->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRD++;
	}
	m_mapRoadDestinationMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRiD =  m_mapRailDestinationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRiD  =  m_mapRailDestinationMinLink.end();
	while (iterRiD != endRiD)
	{
		KNodeArrivedMinLink* pMinLink = iterRiD->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRiD++;
	}
	m_mapRailDestinationMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRiS =  m_mapRailStationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRiS  =  m_mapRailStationMinLink.end();
	while (iterRiS != endRiS)
	{
		KNodeArrivedMinLink* pMinLink = iterRiS->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRiS++;
	}
	m_mapRailStationMinLink.clear();

	m_oColLink.RemoveAll();
	m_oRoadColLink.RemoveAll();
	m_oRailColLink.RemoveAll();
}


void KCalculatePathFindInfoDlg::SelectCommdityList()
{
	m_mapSelectCommdity.clear();

	int	 nSeq			= 0;
	bool bUseCommodity	= false;

	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	
	for(int i = 0; i < nRecordCount; ++i)
	{
		bool bCheck = false;
		CXTPReportRecord* pRecord  = pRecords->GetAt(i);
		CXTPReportColumn* pColumn   = m_wndReport.GetColumns()->GetAt(PathFind_PURPOSE);
		CXTPReportRecordItem* pItem = pRecord->GetItem(PathFind_SELECTION);
		if (pItem->IsChecked() == TRUE)
		{
			bCheck		  = true;
			bUseCommodity = true;
		}

		pItem = pRecord->GetItem(PathFind_PURPOSE);
		CString strCommdityName = pItem->GetCaption(pColumn);

		KPurposeInfoCollection purposeInfos(m_pTarget);
		std::list<KPurpose*>::iterator iter = purposeInfos.begin();
		std::list<KPurpose*>::iterator end  = purposeInfos.end();
		while(iter != end)
		{
			KPurpose* pPurPose = *iter;
			if (pPurPose->PurposeName() == strCommdityName)
			{
				int nPurposeID = pPurPose->PurposeID();
				m_mapSelectCommdity.insert(std::make_pair(nPurposeID, bCheck));
			}
			++iter;
		}
	}

	if (bUseCommodity == false)
	{
		ThrowException(_T("선택된 품목이 없습니다."));
	}

}

void KCalculatePathFindInfoDlg::GetZoneNodeInfo()
{
	TxLogDebugStartMsg();
	m_setZoneNode.clear();
	m_mapZoneNodeInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type,Zone_ID FROM %s ORDER BY Node_Id "), TABLE_NODE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return;
	}

	Integer nxNodeId		= 0;
	Integer nxZoneID        = 0;
	CString strNodeName		= _T("");
	int		nNodeType		= 0;

	while( spResult->Next() )
	{
		nxNodeId	= spResult->GetValueInt64 (0);
		strNodeName = spResult->GetValueString(1);
		nNodeType	= spResult->GetValueInt   (2);
		nxZoneID    = spResult->GetValueInt64 (3);
		m_setAllNode.insert(nxNodeId);
		m_mapAllNodeInfo.insert(std::make_pair(nxNodeId, nxZoneID));

		if (nNodeType == 0 )
		{
			m_setZoneNode.insert(nxNodeId);
			m_mapZoneNodeInfo.insert(std::make_pair(nxNodeId, nxZoneID));
		}
	}
	TxLogDebugEndMsg();
	return;
}


void KCalculatePathFindInfoDlg::GetRoadFacilitiesNodeInfo()
{
	TxLogDebugStartMsg();
	m_setRoadFacilitiesNode.clear();
	m_mapRoadFacilitiesNodeInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strTrafficSQL		= _T("");
	strTrafficSQL.Format(_T("Select Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficResult = spDBaseConnection->ExecuteQuery(strTrafficSQL);

	if (nullptr == pTrafficResult)
	{
		ThrowException(_T("물류 노드에 이용가능 수단 값이 없습니다."));
		return;
	}

	std::set<CString> setUseTrafficType;

	while(pTrafficResult->Next())
	{
		CString strTrafficType		= pTrafficResult->GetValueString(0);
		CString strTrafficElement	= _T("");
		int     nSub				= 0;

		while(AfxExtractSubString(strTrafficElement, strTrafficType, nSub++, _T(',')))
		{
			if (strTrafficElement.IsEmpty() == TRUE)
			{
				continue;
			} 
			else if (strTrafficElement == _T("1"))
			{
				setUseTrafficType.insert(strTrafficType);
				continue;
			}
		}

	}

	CString strTrafficTypeList = _T("");
	std::set<CString>::iterator iterType = setUseTrafficType.begin();
	std::set<CString>::iterator endType  = setUseTrafficType.end();
	while(iterType != endType)
	{
		strTrafficTypeList += _T("'");
		strTrafficTypeList += *iterType;
		strTrafficTypeList += _T("'");

		iterType++;

		if (iterType != endType)
		{
			strTrafficTypeList += (",");
		}
	}


	CString strNodeSQL;
	strNodeSQL.Format(_T("Select node_id, zone_id From %s Where node_type = 2 and traffic_mean in (%s) Order By node_id"), 
								TABLE_NODE, strTrafficTypeList);

	KResultSetPtr pNodeResult = spDBaseConnection->ExecuteQuery(strNodeSQL);
	if (nullptr == pNodeResult)
	{
		ThrowException(_T("육송 경유가능한 물류시설이 없습니다."));
		return;
	}

	while(pNodeResult->Next())
	{
		Integer nxNodeId = pNodeResult->GetValueInt64(0);
		Integer nxZoneId = pNodeResult->GetValueInt64(1);

		m_setRoadFacilitiesNode.insert(nxNodeId);
		m_mapRoadFacilitiesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));

	}

	TxLogDebugEndMsg();
}


void KCalculatePathFindInfoDlg::GetRailFacilitiesNodeInfo()
{
	TxLogDebugStartMsg();
	m_setRailFacilitiesNode.clear();
	m_mapRailFacilitiesNodeInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strTrafficSQL		= _T("");
	strTrafficSQL.Format(_T("Select Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficResult = spDBaseConnection->ExecuteQuery(strTrafficSQL);

	if (nullptr == pTrafficResult)
	{
		ThrowException(_T("물류 노드에 이용가능 수단 값이 없습니다."));
		return;
	}

	std::set<CString> setUseTrafficType;

	while(pTrafficResult->Next())
	{
		CString strTrafficType		= pTrafficResult->GetValueString(0);
		CString strTrafficElement	= _T("");
		int     nSub				= 0;

		while(AfxExtractSubString(strTrafficElement, strTrafficType, nSub++, _T(',')))
		{
			if (strTrafficElement.IsEmpty() == TRUE)
			{
				continue;
			} 
			else if (strTrafficElement == _T("2"))
			{
				setUseTrafficType.insert(strTrafficType);
				continue;
			}
		}

	}

	CString strTrafficTypeList = _T("");
	std::set<CString>::iterator iterType = setUseTrafficType.begin();
	std::set<CString>::iterator endType  = setUseTrafficType.end();
	while(iterType != endType)
	{
		strTrafficTypeList += _T("'");
		strTrafficTypeList += *iterType;
		strTrafficTypeList += _T("'");

		iterType++;

		if (iterType != endType)
		{
			strTrafficTypeList += (",");
		}
	}


	CString strNodeSQL;
	strNodeSQL.Format(_T("Select node_id, zone_id From %s Where node_type = 2 and traffic_mean in (%s) Order By node_id"), 
		TABLE_NODE, strTrafficTypeList);

	KResultSetPtr pNodeResult = spDBaseConnection->ExecuteQuery(strNodeSQL);
	if (nullptr == pNodeResult)
	{
		ThrowException(_T("철송 경유가능한 물류시설이 없습니다."));
		return;
	}

	while(pNodeResult->Next())
	{
		Integer nxNodeId = pNodeResult->GetValueInt64(0);
		Integer nxZoneId = pNodeResult->GetValueInt64(1);

		m_setRailFacilitiesNode.insert(nxNodeId);
		m_mapRailFacilitiesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));
	}

	TxLogDebugEndMsg();
}

void KCalculatePathFindInfoDlg::GetExcludeCommdityNode()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT node_id, available_item FROM %s Where node_type = 2"), TABLE_NODE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return;
	}
	Integer nxNodeID = 0;
	CString strAvailableItem = _T("");

	while (spResult->Next())
	{
		nxNodeID	     = spResult->GetValueInt64 (0);
		strAvailableItem = spResult->GetValueString(1);

		int     nSub       = 0;
		CString strElement = _T("");
		int     nCodeValue = 0;
		std::set<int> setCodeValue;


		while(AfxExtractSubString(strElement, strAvailableItem, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			nCodeValue = _ttoi(strElement);
			setCodeValue.insert(nCodeValue);
		}


		bool bMatchCode = true;

		std::map<int, bool>::iterator iter = m_mapSelectCommdity.begin();
		std::map<int, bool>::iterator end  = m_mapSelectCommdity.end();
		while(iter != end)
		{
			int nCode = iter->first;
			if (iter->second == true)
			{
				std::set<int>::iterator iterfind = setCodeValue.find(nCode);
				std::set<int>::iterator findend  = setCodeValue.end();
				if (iter == end)
				{
					bMatchCode = false;
				}
			}

			++iter;
		}
		if (bMatchCode == false)
		{
			m_setExcludeNodeID.insert(nxNodeID);
		}
	}

}

void KCalculatePathFindInfoDlg::GetRoadLinkData()
{
	m_oColLink.RemoveAll();
	m_oRoadColLink.RemoveAll();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strAllLinkSQL;
	strAllLinkSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s, %s FROM %s Where link_type in (%d, %d, %d, %d, %d, %d)"),
		_T("length"), _T("speed"), TABLE_LINK , Link_Type_Zone, LInk_Type_Highway, 
		LInk_Type_Expressway, LInk_Type_GeneralRoad, LInk_Type_AccessRoad, LInk_Type_Shuttle);
	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strAllLinkSQL);

	if(spResult == NULL)
	{
		ThrowException(_T("링크데이터가 없습니다."));
		return;
	}

	LinkIDType nxLinkId		   = 0;
	Integer    nxFNodeId       = 0;
	Integer    nxTNodeId       = 0;
	Integer    nxExcludeNodeID = 0;
	Double     dCost           = 0.0;
	double	   dSpeed		   = 0.0;
	CString    strExcludeNode  = _T("");
	KLink*     pLink           = NULL;

	while( spResult->Next() )
	{
		nxLinkId	= spResult->GetValueInt64 (0);
		nxFNodeId	= spResult->GetValueInt64 (1);
		nxTNodeId	= spResult->GetValueInt64 (2);
		dCost	    = spResult->GetValueDouble(3);
		dSpeed		= spResult->GetValueDouble(4);

		pLink = m_oRoadColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);

		pLink = m_oColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);

		double dTime = 0.0;
		if (0 != dSpeed)
		{
			dTime = dCost/ dSpeed;
		}

		m_mapRoadLinkLength.insert(std::make_pair(nxLinkId, dCost));
		m_mapDirectPathLink.insert(std::make_pair(nxLinkId, dTime));
	}

	std::set<Integer>::iterator iter = m_setExcludeNodeID.begin();
	std::set<Integer>::iterator end  = m_setExcludeNodeID.end();
	while (iter!= end)
	{
		nxExcludeNodeID          = *iter;
		CString strNodeID = _T("");
		strNodeID.Format(_T("%s"), nxExcludeNodeID);

		strExcludeNode += strNodeID;

		++iter;

		if (iter != end)
		{
			strExcludeNode += _T(", ");
		}
	}

	//경로탐색 Facilities 
	/*CString strExcludeSQL = _T("");
	strExcludeSQL.Format(_T(" Select Link_Id, FNode_Id, TNode_Id, %s FROM %s Where (fnode_id not in (%s) and link_type = %d) or (tnode_id not in (%s) and link_type =%d) "),
		_T("length"), TABLE_LINK, strExcludeNode, LInk_Type_Shuttle, strExcludeNode, LInk_Type_Shuttle);

	KResultSetPtr pExcludeResult = oDBaseConnetion.ExecuteQuery(strExcludeSQL);
	if(pExcludeResult == NULL)
	{
		return;
	}

	while( pExcludeResult->Next() )
	{
		nxLinkId	= pExcludeResult->GetValueInt64 (0);
		nxFNodeId	= pExcludeResult->GetValueInt64 (1);
		nxTNodeId	= pExcludeResult->GetValueInt64 (2);
		dCost	    = pExcludeResult->GetValueDouble(3);

		pLink = m_oRoadColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);
	}*/

	CString strExcludeSQL = _T("");
	strExcludeSQL.Format(_T(" Select Link_Id, FNode_Id, TNode_Id, %s FROM %s Where (fnode_id in (%s)) or (tnode_id in (%s))"),
		_T("length"), TABLE_LINK, strExcludeNode, strExcludeNode );

	KResultSetPtr pExcludeResult = spDBaseConnection->ExecuteQuery(strExcludeSQL);
	if(pExcludeResult == NULL)
	{
		return;
	}

	while( pExcludeResult->Next() )
	{
		nxLinkId	= pExcludeResult->GetValueInt64 (0);
		nxFNodeId	= pExcludeResult->GetValueInt64 (1);
		nxTNodeId	= pExcludeResult->GetValueInt64 (2);
		dCost	    = pExcludeResult->GetValueDouble(3);

		pLink->GetLinkID();

		if (nullptr != pLink)
		{
			m_oRoadColLink.DeleteLink(nxLinkId);
		}
	}

	//경로탐색 Direct Shuttle
	/*CString strAllSuttleSQL = _T("");
	strAllSuttleSQL.Format(_T("Select link_id, fnode_id, tnode_id, %s From %s where link_type = %d"), _T("length"), TABLE_LINK, LInk_Type_Shuttle);

	KResultSetPtr pShuttleResult = oDBaseConnetion.ExecuteQuery(strAllSuttleSQL);
	if(pShuttleResult == NULL)
	{
	return;
	}

	while( pShuttleResult->Next() )
	{
	nxLinkId	= pShuttleResult->GetValueInt64 (0);
	nxFNodeId	= pShuttleResult->GetValueInt64 (1);
	nxTNodeId	= pShuttleResult->GetValueInt64 (2);
	dCost	    = pShuttleResult->GetValueDouble(3);

	pLink = m_oColLink.GetExistOrNew(nxLinkId);
	pLink->SetFromNodeID(nxFNodeId);
	pLink->SetToNodeID(nxTNodeId);
	pLink->SetCost(dCost);
	}*/

	TxLogDebugEndMsg();

}

void KCalculatePathFindInfoDlg::GetRailLinkData()
{
	TxLogDebugStartMsg();
	m_oRailColLink.RemoveAll();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strRailSQL;
	strRailSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type = %d"), 
		_T("length"), TABLE_LINK, LInk_Type_RailWay );

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strRailSQL);
	if(nullptr == spResult)
	{
		return;
	}

	LinkIDType nxLinkId		= 0;
	Integer    nxFNodeId    = 0;
	Integer    nxTNodeId    = 0;
	Double     dCost        = 0.0;
	KLink*     pLink        = NULL;

	while( spResult->Next() )
	{
		nxLinkId	= spResult->GetValueInt64 (0);
		nxFNodeId	= spResult->GetValueInt64 (1);
		nxTNodeId	= spResult->GetValueInt64 (2);
		dCost	    = spResult->GetValueDouble(3);

		pLink = m_oRailColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);

		m_mapRailLinkLength.insert(std::make_pair(nxLinkId, dCost));
	}

	TxLogDebugEndMsg();
	return;
}

bool KCalculatePathFindInfoDlg::SaveAllPathFindInfo()
{
	TxLogDebugStartMsg();
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(_T("경로 탐색 - 존 -> 물류시설/철도역, 존"));
	}

	//시작 노드에서 모든 노드까지
	std::map<Integer, Integer>::iterator iterOriginRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endOriginRoad  = m_mapZoneNodeInfo.end();
	while (iterOriginRoad != endOriginRoad)
	{
		Integer nxOrginNodeID = iterOriginRoad->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oShortestPath;
		oShortestPath.MuLinkBaseExecute(nxOrginNodeID, m_setAllNode, m_oColLink, *pCostLinkCollection, *pNodeArrivedMinLink);
		m_mapRoadOriginMinLink.insert(std::make_pair(nxOrginNodeID, pNodeArrivedMinLink));
		m_mapDirectRoadCostLink.insert(std::make_pair(nxOrginNodeID, pCostLinkCollection));
		iterOriginRoad++;

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ClearNodeArrivedMInInfo();
			return false;
		}
	}

	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(_T("경로 탐색 - 물류시설 -> 존"));
	}

	//육송 물류시설 부터 모든 존 노드까지 
	std::map<Integer, Integer>::iterator iterDestinationRoad = m_mapRoadFacilitiesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestiantionRoad  = m_mapRoadFacilitiesNodeInfo.end();
	while(iterDestinationRoad != endDestiantionRoad)
	{
		Integer nxFNodeID = iterDestinationRoad->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRoadFacilitiesPath;
		oRoadFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setZoneNode, m_oRoadColLink, *pCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRoadDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		m_mapRoadFacilityCostLink.insert(std::make_pair(nxFNodeID,pCostLinkCollection));
		iterDestinationRoad++;

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ClearNodeArrivedMInInfo();
			return false;
		}
	}	
	
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(_T("경로 탐색 - 철도역 -> 철도역"));
	}

	//철송 물류시설부터 철송 물류시설까지
	std::map<Integer, Integer>::iterator iterRailStation = m_mapRailFacilitiesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRailStation  = m_mapRailFacilitiesNodeInfo.end();
	while(iterRailStation != endRailStation)
	{
		Integer nxFNodeID = iterRailStation->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setRailFacilitiesNode, m_oRailColLink,*pCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailStationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		m_mapRailCostLink.insert(std::make_pair(nxFNodeID, pCostLinkCollection));
		iterRailStation++;

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ClearNodeArrivedMInInfo();
			return false;
		}
	}

	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(_T("경로 탐색 - 철도역 -> 존"));
	}
	//철송 물류시설부터 모든노드까지
	std::map<Integer, Integer>::iterator iterDestinationRail = m_mapRailFacilitiesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestinationRail  = m_mapRailFacilitiesNodeInfo.end();
	while(iterDestinationRail != endDestinationRail)
	{
		Integer nxFNodeID = iterDestinationRail->first;

		KCostLinkCollection oCostLinkCollection;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setZoneNode, m_oRoadColLink, *pCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		m_mapRailFacilityCostLink.insert(std::make_pair(nxFNodeID,pCostLinkCollection));
		iterDestinationRail++;

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ClearNodeArrivedMInInfo();
			return false;
		}
	}

	TxLogDebugEndMsg();
	return true;
}

void KCalculatePathFindInfoDlg::CalculatePathFind()
{
	TxLogDebugStartMsg();

	bool bResult = SaveAllPathFindInfo();
	if (bResult == false)
		return;

	std::map<Integer, Integer>::iterator iter = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator end  = m_mapZoneNodeInfo.end();
	while (iter != end)
	{
		Integer nxSNodeID = iter->first;
		Integer nxSZoneID = iter->second;
		GetFindPathDirectRoadResult(nxSNodeID, nxSZoneID);
		GetFindPathFacilitiesRoadResult(nxSNodeID, nxSZoneID);
		GetFindPathRailResult(nxSNodeID, nxSZoneID);
		++iter;

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ClearNodeArrivedMInInfo();
			TxLogDebug(_T("User Stopped"));
			return;
		}
	}

	WriteCalculateData();
	ClearNodeArrivedMInInfo();
	TxLogDebugEndMsg();
}


void KCalculatePathFindInfoDlg::GetFindPathDirectRoadResult( Integer a_nxStartNodeID, Integer a_StartZoneID )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if (spProgressWindow)
	{
		CString strStatus(_T(""));
		strStatus.Format(_T("Zone : %I64d - 직송 계산 중"), a_nxStartNodeID);
		spProgressWindow->SetStatus((LPCTSTR)strStatus);
	}

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRoad  = m_mapRoadOriginMinLink.find(a_nxStartNodeID);
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRoad   = m_mapRoadOriginMinLink.end();
	if (iterRoad == endRoad)
	{
		return;
	}

	KNodeArrivedMinLink* pNodeArrivedMinLink;
	pNodeArrivedMinLink = iterRoad->second;

	AutoType iterCost = m_mapDirectRoadCostLink.find(a_nxStartNodeID);
	AutoType Costend  = m_mapDirectRoadCostLink.end();

	KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
	if (iterCost != Costend)
	{
		pCostLinkCollection = iterCost->second;
	}

	CString strLog(_T(""));
	/*strLog.Format(_T(" >> 직송_Links"));
	WritePathLog(strLog);*/

	std::map<Integer, Integer>::iterator iterDRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDRoad  = m_mapZoneNodeInfo.end();
	while (iterDRoad != endDRoad)
	{
		Integer nxEndNodeID     = iterDRoad->first;
		Integer nxEndZoneID     = iterDRoad->second;
		double dSumDRoadValue   = 0.0;

		KMinLinkInfo* pLinkInfo = pNodeArrivedMinLink->GetMinLink(nxEndNodeID);
		if (pLinkInfo != NULL)
		{
			if (a_StartZoneID == nxEndZoneID)
			{
				dSumDRoadValue = FREIGHT_DEFUALT_DISTANCE;
			}
			else
			{
				dSumDRoadValue = pLinkInfo->Cost;
			}
		} 
		else
		{
			#pragma region origin
			// dSumDRoadValue = MAX_DISTANCE;
			#pragma endregion origin
			#pragma region modify
			if (a_StartZoneID == nxEndZoneID)
			{
				dSumDRoadValue = FREIGHT_DEFUALT_DISTANCE;
			}
			else
			{
				dSumDRoadValue = FREIGHT_MAX_DISTANCE;
			}		
			#pragma endregion modify
		}
		double dTime(0.0);

		std::vector<Integer> vecLink;

		if (nullptr != pLinkInfo )
		{
			Integer nxBackLinkId = pLinkInfo->LinkID;

			AutoType iterTime = m_mapDirectPathLink.find(nxBackLinkId);
			AutoType iterend  = m_mapDirectPathLink.end();
			
			while(nxBackLinkId > 0)
			{
				KCostLink* pCostLink = pCostLinkCollection->GetLink(nxBackLinkId);
				if(NULL == pCostLink)
				{
					dTime = 0.0;
					break;
				}
				else 
				{
					vecLink.insert(vecLink.begin(),nxBackLinkId);
					nxBackLinkId = pCostLink->GetPreLinkID();
					iterTime = m_mapDirectPathLink.find(nxBackLinkId);
					if (iterTime != iterend)
					{
						dTime = dTime + iterTime->second;
					}
				}
			}
			
			if (a_StartZoneID == nxEndNodeID)
			{
				dTime = 0.0;
			}
		}

		//결과값 쓰기
		strLog.Format(_T("직송 >> OZone = %I64d , DZone = %I64d "), a_StartZoneID, nxEndZoneID );
		WritePathLog(strLog);

		size_t nCount = vecLink.size();

		if (0 ==  nCount)
		{
			WritePathLog(_T("탐색된 결과가 없습니다."));
		}
		else if (a_nxStartNodeID == nxEndNodeID)
		{
			WritePathLog(_T("시작존과 종료존이 같습니다."));
		}
		else
		{
			for (size_t i = 0; i < nCount; i++)
			{
				Integer nxLinkID = vecLink[i];

				if (0 == i)
				{
					strLog.Format(_T("Link : %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}
				else
				{
					strLog.AppendFormat(_T(", %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}

			}
			WritePathLog(strLog);
		}

		KODKey oODKey (a_StartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator findLengthiter =  m_mapResultLengthRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findLengthend  =  m_mapResultLengthRecord.end();		
		KSDoubleRecord* pLengthRecord = NULL;

		if (findLengthiter == findLengthend)
		{
			pLengthRecord = new KSDoubleRecord(15);
			m_mapResultLengthRecord.insert(std::make_pair(oODKey, pLengthRecord));
		}
		else
		{
			pLengthRecord = findLengthiter->second;
		}
		
		pLengthRecord->SetAt(RoadD, dSumDRoadValue);
		pLengthRecord->SetAt(StN,	(double)a_nxStartNodeID);
		pLengthRecord->SetAt(EdN,	(double)nxEndNodeID);
		pLengthRecord->SetAt(RoadT, dTime);

		++iterDRoad;
	}
}

void KCalculatePathFindInfoDlg::GetFindPathFacilitiesRoadResult( Integer a_nxStrartNodeID, Integer a_nxStartZoneID )
{

	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if (spProgressWindow)
	{
		CString strStatus(_T(""));
		strStatus.Format(_T("Zone : %I64d - 육송 물류시설 경유 계산 중"), a_nxStrartNodeID);
		spProgressWindow->SetStatus((LPCTSTR)strStatus);
	}

	CString strLog(_T(""));
	//strLog.Format(_T(" >> 육송물류시설경유_Links"));
	//WritePathLog(strLog);

	Integer nxEndNodeID = 0;
	Integer nxEndZoneID = 0;

	std::map<Integer, Integer>::iterator iterRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRoad  = m_mapZoneNodeInfo.end();
	while (iterRoad != endRoad)
	{
		nxEndNodeID = iterRoad->first;
		nxEndZoneID = iterRoad->second;

		bool    bRoadFindPath              = false;
		Integer nxMinFacilitiesID          = 0;
		Integer nxMinZoneFacilitiesID      = 0;
		double  dMinOriginValue            = 0.0;
		double  dMInDestinaionValue        = 0.0;
		double  dMinSumValue               = 0.0;
		KMinLinkInfo* pOriginMinLinkInfo = new KMinLinkInfo;
		KMinLinkInfo* pDestinationMinLinkInfo = new KMinLinkInfo;

		std::map<Integer, KNodeArrivedMinLink*>::iterator iterORoad  = m_mapRoadOriginMinLink.find(a_nxStrartNodeID);
		std::map<Integer, KNodeArrivedMinLink*>::iterator endORoad   = m_mapRoadOriginMinLink.end();
		KNodeArrivedMinLink* pNodeArrivedMinLink = iterORoad->second;

		std::map<Integer, Integer>::iterator iterFacilities = m_mapRoadFacilitiesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndFacilities  = m_mapRoadFacilitiesNodeInfo.end();
		while(iterFacilities != EndFacilities)
		{
			Integer nxFacilitiesNodeID = iterFacilities->first;
			Integer nxFacilitiesZoneID = iterFacilities->second;

			double  dOrginValue       = 0.0;
			double  dDestinationValue = 0.0;
			double  dSumValue         = 0.0;

			std::set<Integer> setEndNode;
			setEndNode.insert(nxEndNodeID);

			KMinLinkInfo* pOriginLinkInfo = pNodeArrivedMinLink->GetMinLink(nxFacilitiesNodeID);
			if (pOriginLinkInfo != NULL)
			{
				dOrginValue = pOriginLinkInfo->Cost;

				std::map<Integer, KNodeArrivedMinLink*>::iterator iterDRoad  = m_mapRoadDestinationMinLink.find(nxFacilitiesNodeID);
				std::map<Integer, KNodeArrivedMinLink*>::iterator endDRoad   = m_mapRoadDestinationMinLink.end();
				KNodeArrivedMinLink* pDestiaNationMinLink = iterDRoad->second;

				KMinLinkInfo* pDestinationLinkInfo = pDestiaNationMinLink->GetMinLink(nxEndNodeID);
				if (pDestinationLinkInfo != NULL)
				{
					dDestinationValue = pDestinationLinkInfo->Cost;
					dSumValue  = dOrginValue + dDestinationValue;

					if (bRoadFindPath == false)
					{
						pOriginMinLinkInfo			= pOriginLinkInfo;
						pDestinationMinLinkInfo		= pDestinationLinkInfo;

						dMinOriginValue       = dOrginValue;
						dMInDestinaionValue   = dDestinationValue;
						nxMinFacilitiesID     = nxFacilitiesNodeID;
						nxMinZoneFacilitiesID = nxFacilitiesZoneID;
						dMinSumValue          = dSumValue;
						bRoadFindPath         = true;

					} 
					else if (dMinSumValue > dSumValue)
					{

						pOriginMinLinkInfo			= pOriginLinkInfo;
						pDestinationMinLinkInfo		= pDestinationLinkInfo;
						 
						dMinOriginValue       = dOrginValue;
						dMInDestinaionValue   = dDestinationValue;
						nxMinFacilitiesID     = nxFacilitiesNodeID;
						nxMinZoneFacilitiesID = nxFacilitiesZoneID;
						dMinSumValue          = dSumValue;
					}
				}
			}
			iterFacilities++;
		}
		
		std::vector<Integer> vecOriginLink;
		std::vector<Integer> vecDestinationLink;

		if (a_nxStartZoneID == nxEndZoneID)
		{
			dMinOriginValue       = FREIGHT_DEFUALT_DISTANCE;
			dMInDestinaionValue   = FREIGHT_DEFUALT_DISTANCE;
			nxMinFacilitiesID     = 0;
			nxMinZoneFacilitiesID = 0;
		}
		else if (bRoadFindPath == false)
		{
			dMinOriginValue       = FREIGHT_MAX_DISTANCE;
			dMInDestinaionValue   = FREIGHT_MAX_DISTANCE;
			nxMinFacilitiesID     = 0;
			nxMinZoneFacilitiesID = 0;
		}
		else
		{
			AutoType iterOrigin		= m_mapDirectRoadCostLink.find(a_nxStrartNodeID);
			AutoType iterDesiantion = m_mapRoadFacilityCostLink.find(nxMinFacilitiesID);
			AutoType end  = m_mapDirectRoadCostLink.end();

			KCostLinkCollection* pOCostLinkCollection = new KCostLinkCollection;
			KCostLinkCollection* pDCostLinkCollection = new KCostLinkCollection;

			pOCostLinkCollection = iterOrigin->second;
			pDCostLinkCollection = iterDesiantion->second;

			if (nullptr != pOriginMinLinkInfo )
			{
				Integer nxBackLinkId	= pOriginMinLinkInfo->LinkID;
				AutoType iterend		= m_mapDirectPathLink.end();

				while(nxBackLinkId > 0)
				{
					KCostLink* pCostLink = pOCostLinkCollection->GetLink(nxBackLinkId);
					if(NULL == pCostLink)
					{
						break;
					}
					else 
					{
						vecOriginLink.insert(vecOriginLink.begin(),nxBackLinkId);
						nxBackLinkId = pCostLink->GetPreLinkID();
					}
				}
			}

			if (nullptr != pDestinationMinLinkInfo )
			{
				Integer nxBackLinkId	= pDestinationMinLinkInfo->LinkID;
				AutoType iterend		= m_mapDirectPathLink.end();

				while(nxBackLinkId > 0)
				{
					KCostLink* pCostLink = pDCostLinkCollection->GetLink(nxBackLinkId);
					if(NULL == pCostLink)
					{
						break;
					}
					else 
					{
						vecDestinationLink.insert(vecDestinationLink.begin(),nxBackLinkId);
						nxBackLinkId = pCostLink->GetPreLinkID();
					}
				}
			}
		}

		strLog.Format(_T("육송 물류시설 >> OZone = %I64d , DZone = %I64d"), a_nxStrartNodeID, nxEndZoneID );
		WritePathLog(strLog);

		size_t nDCount = vecDestinationLink.size();
		size_t nOCount = vecOriginLink.size();

		if (a_nxStrartNodeID == nxEndNodeID)
		{
			WritePathLog(_T("시작존과 종료존이 같습니다."));
		}
		else if (0 ==  nDCount || 0 == nOCount)
		{
			WritePathLog(_T("탐색된 결과가 없습니다."));
		}
		else
		{
			for (size_t i = 0; i < nOCount; i++)
			{
				Integer nxLinkID = vecOriginLink[i];

				if (0 == i)
				{
					strLog.Format(_T("Link : %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}
				else
				{
					strLog.AppendFormat(_T(", %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}

			}

			for (size_t i= 0; i < nDCount; i++)
			{
				Integer nxLinkID = vecDestinationLink[i];
				strLog.AppendFormat(_T(", %I64d"), nxLinkID);
				AutoType find = m_mapRoadLinkLength.find(nxLinkID);
				AutoType end  = m_mapRoadLinkLength.end();
				if (find == end)
				{
					strLog.AppendFormat(_T("(0)"));
				}
				else
				{
					double dCost = find->second;
					strLog.AppendFormat(_T("(%.1f)"), dCost);
				}
			}

			WritePathLog(strLog);
		}

		KODKey oODKey (a_nxStartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultLengthRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultLengthRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(15);
			m_mapResultLengthRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}

		pRecord->SetAt(RoadS, dMinOriginValue);
		pRecord->SetAt(RoadE, dMInDestinaionValue);
		pRecord->SetAt(RoadFN,(double)nxMinFacilitiesID);
		pRecord->SetAt(RoadFZ,(double)nxMinZoneFacilitiesID);

		iterRoad++;
	}
}

void KCalculatePathFindInfoDlg::GetFindPathRailResult( Integer a_nxStartNodeID, Integer a_startZoneID )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	CString strLog(_T(""));
	/*strLog.Format(_T(" >> 철송물류시설경유_Links"));
	WritePathLog(strLog);*/

	if (spProgressWindow)
	{
		CString strStatus(_T(""));
		strStatus.Format(_T("Zone : %I64d - 철송 계산 중"), a_nxStartNodeID);
		spProgressWindow->SetStatus((LPCTSTR)strStatus);
	}

	Integer nxEndNodeID = 0;
	Integer nxEndZoneID = 0;

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterORoad  = m_mapRoadOriginMinLink.find(a_nxStartNodeID);
	std::map<Integer, KNodeArrivedMinLink*>::iterator endORoad   = m_mapRoadOriginMinLink.end();
	KNodeArrivedMinLink* pOriginArrivedMinLink = iterORoad->second;

	std::map<Integer, Integer>::iterator iterRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRoad  = m_mapZoneNodeInfo.end();
	while (iterRoad != endRoad)
	{
		nxEndNodeID = iterRoad->first;
		nxEndZoneID = iterRoad->second;

		std::set<Integer> setEndNode;
		setEndNode.insert(nxEndNodeID);

		Integer nxOriginFacilitiesNode      = 0;
		Integer nxOriginFacilitiesZone      = 0;
		Integer nxDestinationFacilitiesNode = 0;
		Integer nxDestinationFacilitiesZone = 0;
		double  dMinOriginValue             = 0.0;
		double  dMinDestinationValue        = 0.0;
		double  dMinRailValue               = 0.0;

		bool  bFindOriginPath      = false;
		bool  bFindDestinationPath = false;
		bool  bFindRailPath        = false;

		KMinLinkInfo* pOriginMinLinkInfo		= new KMinLinkInfo;
		KMinLinkInfo* pDestinationMinLinkInfo	= new KMinLinkInfo;
		KMinLinkInfo* pRailMinLinkInfo			= new KMinLinkInfo;

		std::map<Integer, Integer>::iterator iterOFacilities = m_mapRailFacilitiesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndOFacilities  = m_mapRailFacilitiesNodeInfo.end();
		while (iterOFacilities != EndOFacilities)
		{
			Integer nxOFacilitiesNode = iterOFacilities->first;

			KMinLinkInfo* pOriginLinkInfo = pOriginArrivedMinLink->GetMinLink(nxOFacilitiesNode);
			if (pOriginLinkInfo != NULL)
			{
				double dOrginValue = pOriginLinkInfo->Cost; 
				if (bFindOriginPath == false)
				{
					nxOriginFacilitiesNode	= nxOFacilitiesNode;
					nxOriginFacilitiesZone	= iterOFacilities->second;
					dMinOriginValue			=  dOrginValue;
					bFindOriginPath			= true;
					pOriginMinLinkInfo		= pOriginLinkInfo;
				} 
				else if (dMinOriginValue > dOrginValue)
				{
					nxOriginFacilitiesNode	= nxOFacilitiesNode;
					nxOriginFacilitiesZone	= iterOFacilities->second;
					dMinOriginValue			=  dOrginValue;
					pOriginMinLinkInfo		= pOriginLinkInfo;
				}

			}
			iterOFacilities++;
		}

		std::map<Integer, Integer>::iterator iterDFacilities = m_mapRailFacilitiesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndDFacilities  = m_mapRailFacilitiesNodeInfo.end();
		while(iterDFacilities != EndDFacilities)
		{
			Integer nxDFaciliteiesNode = iterDFacilities->first;

			if (nxDFaciliteiesNode != nxOriginFacilitiesNode)
			{
				std::map<Integer, KNodeArrivedMinLink*>::iterator iterDRail  = m_mapRailDestinationMinLink.find(nxDFaciliteiesNode);
				std::map<Integer, KNodeArrivedMinLink*>::iterator endDRail   = m_mapRailDestinationMinLink.end();
				KNodeArrivedMinLink* pDestinationArrivedMinLink = iterDRail->second;

				KMinLinkInfo* pDestinationLinkInfo = pDestinationArrivedMinLink->GetMinLink(nxEndNodeID);
				if (pDestinationLinkInfo != NULL)
				{
					double dDestinaionValue = pDestinationLinkInfo->Cost;
					if (bFindDestinationPath == false)
					{
						nxDestinationFacilitiesNode = nxDFaciliteiesNode;
						nxDestinationFacilitiesZone = iterDFacilities->second;
						dMinDestinationValue        = dDestinaionValue;
						bFindDestinationPath		= true;
						pDestinationMinLinkInfo		= pDestinationLinkInfo;
					} 
					else if (dMinDestinationValue > dDestinaionValue)
					{
						nxDestinationFacilitiesNode = nxDFaciliteiesNode;
						nxDestinationFacilitiesZone = iterDFacilities->second;
						dMinDestinationValue        = dDestinaionValue;
						pDestinationMinLinkInfo		= pDestinationLinkInfo;
					}
				}
			}
			iterDFacilities++;
		}

		if (a_nxStartNodeID == nxEndZoneID)
		{
			nxOriginFacilitiesNode      = 0;
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesNode = 0;
			nxDestinationFacilitiesZone = 0;
			dMinOriginValue             = FREIGHT_DEFUALT_DISTANCE;
			dMinDestinationValue        = FREIGHT_DEFUALT_DISTANCE;
			dMinRailValue               = FREIGHT_DEFUALT_DISTANCE;
		}
		else if (bFindOriginPath == false || bFindDestinationPath == false)
		{
			nxOriginFacilitiesNode      = 0;
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesNode = 0;
			nxDestinationFacilitiesZone = 0;
			dMinOriginValue             = FREIGHT_MAX_DISTANCE;
			dMinDestinationValue        = FREIGHT_MAX_DISTANCE;
			dMinRailValue               = FREIGHT_MAX_DISTANCE;

		}
		else
		{
			std::map<Integer, KNodeArrivedMinLink*>::iterator iterStation   = m_mapRailStationMinLink.find(nxOriginFacilitiesNode);
			std::map<Integer, KNodeArrivedMinLink*>::iterator endDStation   = m_mapRailStationMinLink.end();
			KNodeArrivedMinLink* pNodeArrivedMinLink = iterStation->second;

			KMinLinkInfo* pRailLinkInfo = pNodeArrivedMinLink->GetMinLink(nxDestinationFacilitiesNode);
			if (pRailLinkInfo != NULL)
			{
				dMinRailValue		= pRailLinkInfo->Cost;
				bFindRailPath		= true;
				pRailMinLinkInfo	= pRailLinkInfo;
			}
			else
			{
				nxOriginFacilitiesNode      = 0;
				nxOriginFacilitiesZone      = 0;
				nxDestinationFacilitiesNode = 0;
				nxDestinationFacilitiesZone = 0;
				dMinOriginValue             = FREIGHT_MAX_DISTANCE;
				dMinDestinationValue        = FREIGHT_MAX_DISTANCE;
				dMinRailValue               = FREIGHT_MAX_DISTANCE;
			}

		}

		std::vector<Integer> vecOriginLink;
		std::vector<Integer> vecDestinationLink;
		std::vector<Integer> vecRailLink;

		if (true == bFindDestinationPath && true == bFindOriginPath && true == bFindRailPath)
		{
			AutoType iterOrigin		= m_mapDirectRoadCostLink.find(a_nxStartNodeID);
			AutoType iterDesiantion = m_mapRailFacilityCostLink.find(nxDestinationFacilitiesNode);
			AutoType iterRail		= m_mapRailCostLink.find(nxOriginFacilitiesNode);

			AutoType end  = m_mapDirectRoadCostLink.end();

			KCostLinkCollection* pOCostLinkCollection = new KCostLinkCollection;
			KCostLinkCollection* pDCostLinkCollection = new KCostLinkCollection;
			KCostLinkCollection* pRCostLinkCollection = new KCostLinkCollection;

			pOCostLinkCollection = iterOrigin->second;
			pDCostLinkCollection = iterDesiantion->second;
			pRCostLinkCollection = iterRail->second;

			if (nullptr != pOriginMinLinkInfo )
			{
				Integer nxBackLinkId	= pOriginMinLinkInfo->LinkID;

				while(nxBackLinkId > 0)
				{
					KCostLink* pCostLink = pOCostLinkCollection->GetLink(nxBackLinkId);
					if(NULL == pCostLink)
					{
						break;
					}
					else 
					{
						vecOriginLink.insert(vecOriginLink.begin(),nxBackLinkId);
						nxBackLinkId = pCostLink->GetPreLinkID();
					}
				}
			}

			if(nullptr != pRailMinLinkInfo)
			{
				Integer nxBackLinkId	= pRailMinLinkInfo->LinkID;

				while(nxBackLinkId > 0)
				{
					KCostLink* pCostLink = pRCostLinkCollection->GetLink(nxBackLinkId);
					if(NULL == pCostLink)
					{
						break;
					}
					else 
					{
						vecRailLink.insert(vecRailLink.begin(),nxBackLinkId);
						nxBackLinkId = pCostLink->GetPreLinkID();
					}
				}
			}

			if (nullptr != pDestinationMinLinkInfo )
			{
				Integer nxBackLinkId	= pDestinationMinLinkInfo->LinkID;
				AutoType iterend		= m_mapDirectPathLink.end();

				while(nxBackLinkId > 0)
				{
					KCostLink* pCostLink = pDCostLinkCollection->GetLink(nxBackLinkId);
					if(NULL == pCostLink)
					{
						break;
					}
					else 
					{
						vecDestinationLink.insert(vecDestinationLink.begin(),nxBackLinkId);
						nxBackLinkId = pCostLink->GetPreLinkID();
					}
				}
			}
		}

		strLog.Format(_T("철송 >> OZone = %I64d , DZone = %I64d"), a_nxStartNodeID, nxEndZoneID );
		WritePathLog(strLog);

		size_t nDCount = vecDestinationLink.size();
		size_t nOCount = vecOriginLink.size();
		size_t nRCount = vecRailLink.size();

		if (a_nxStartNodeID == nxEndNodeID)
		{
			WritePathLog(_T("시작존과 종료존이 같습니다."));
		}
		else if (0 ==  nDCount || 0 == nOCount || 0 == nRCount)
		{
			WritePathLog(_T("탐색된 결과가 없습니다."));
		}
		else
		{
			for (size_t i = 0; i < nOCount; i++)
			{
				Integer nxLinkID = vecOriginLink[i];

				if (0 == i)
				{
					strLog.Format(_T("Link : %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}
				else
				{
					strLog.AppendFormat(_T(", %I64d"), nxLinkID);
					AutoType find = m_mapRoadLinkLength.find(nxLinkID);
					AutoType end  = m_mapRoadLinkLength.end();
					if (find == end)
					{
						strLog.AppendFormat(_T("(0)"));
					}
					else
					{
						double dCost = find->second;
						strLog.AppendFormat(_T("(%.1f)"), dCost);
					}
				}

			}

			for (size_t i= 0; i < nRCount; i++)
			{
				Integer nxLinkID = vecRailLink[i];
				strLog.AppendFormat(_T(", %I64d"), nxLinkID);
				AutoType find = m_mapRailLinkLength.find(nxLinkID);
				AutoType end  = m_mapRailLinkLength.end();
				if (find == end)
				{
					strLog.AppendFormat(_T("(0)"));
				}
				else
				{
					double dCost = find->second;
					strLog.AppendFormat(_T("(%.1f)"), dCost);
				}
			}

			for (size_t i= 0; i < nDCount; i++)
			{
				Integer nxLinkID = vecDestinationLink[i];
				strLog.AppendFormat(_T(", %I64d"), nxLinkID);
				AutoType find = m_mapRoadLinkLength.find(nxLinkID);
				AutoType end  = m_mapRoadLinkLength.end();
				if (find == end)
				{
					strLog.AppendFormat(_T("(0)"));
				}
				else
				{
					double dCost = find->second;
					strLog.AppendFormat(_T("(%.1f)"), dCost);
				}
			}

			WritePathLog(strLog);
		}


		KODKey oODKey (a_nxStartNodeID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultLengthRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultLengthRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(15);
			m_mapResultLengthRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}
		pRecord->SetAt(RailS, dMinOriginValue);
		pRecord->SetAt(RailM, dMinRailValue);
		pRecord->SetAt(RailE, dMinDestinationValue);
		pRecord->SetAt(RailFSN, (double)nxOriginFacilitiesNode);
		pRecord->SetAt(RailFEN, (double)nxDestinationFacilitiesNode);
		pRecord->SetAt(RailFSZ, (double)nxOriginFacilitiesZone);
		pRecord->SetAt(RailFEZ, (double)nxDestinationFacilitiesZone);

		iterRoad++;
	}
}

void KCalculatePathFindInfoDlg::WriteCalculateData()
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(_T("결과 정보 저장 중입니다"));
	}

	ResetParameterTable();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable    = pIOTables->FindTable(_T("parameter_od"));

	KIOColumns pOutColumnCollection; 
	GetOutputColumnDouble(pOutColumnCollection);

	KODKeyDoubleRecords oResult(15);
	std::map<KODKey, KSDoubleRecord*>::iterator iter = m_mapResultLengthRecord.begin();
	std::map<KODKey, KSDoubleRecord*>::iterator end  = m_mapResultLengthRecord.end();
	KSDoubleRecord* pRecord = NULL;
	for (iter; iter != end; ++iter)
	{

		KODKey oKODKey = iter->first;
		KSDoubleRecord* pOutPutResult = oResult.AddRecord(oKODKey);
		KSDoubleRecord* pRecord = iter->second;
		double dValue = 0;

		dValue = pRecord->GetAt(RoadD);
		pOutPutResult->SetAt(RoadD, dValue);

		dValue = pRecord->GetAt(StN);
		pOutPutResult->SetAt(StN, dValue);

		dValue = pRecord->GetAt(EdN);
		pOutPutResult->SetAt(EdN, dValue);

		dValue = pRecord->GetAt(RailFSN);
		pOutPutResult->SetAt(RailFSN, dValue);

		dValue = pRecord->GetAt(RailFEN);
		pOutPutResult->SetAt(RailFEN, dValue);

		dValue = pRecord->GetAt(RailFSZ);
		pOutPutResult->SetAt(RailFSZ, dValue);

		dValue = pRecord->GetAt(RailFEZ);
		pOutPutResult->SetAt(RailFEZ, dValue);

		dValue = pRecord->GetAt(RailS);
		pOutPutResult->SetAt(RailS, dValue);

		dValue = pRecord->GetAt(RailM);
		pOutPutResult->SetAt(RailM, dValue);

		dValue = pRecord->GetAt(RailE);
		pOutPutResult->SetAt(RailE, dValue);

		dValue = pRecord->GetAt(RoadFN);
		pOutPutResult->SetAt(RoadFN, dValue);

		dValue = pRecord->GetAt(RoadFZ);
		pOutPutResult->SetAt(RoadFZ, dValue);

		dValue = pRecord->GetAt(RoadS);
		pOutPutResult->SetAt(RoadS, dValue);

		dValue = pRecord->GetAt(RoadE);
		pOutPutResult->SetAt(RoadE, dValue);

		dValue = pRecord->GetAt(RoadT);
		pOutPutResult->SetAt(RoadT, dValue);

	}

	KBulkDBase::BulkODUpsert(pTable, pOutColumnCollection, oResult);
	pTable->Notify();
}

void KCalculatePathFindInfoDlg::GetOutputColumnDouble(KIOColumns& columnCollection)
{
	columnCollection.Clear();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable    = pIOTables->FindTable(_T("parameter_od"));

	const KIOColumns* pColumns = pTable->Columns();

	KIOColumn* pOutputColumnDirectDist	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_DIRECT_DIST);
	if (pOutputColumnDirectDist == NULL)
	{
		ThrowException(_T("존간 거리 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnRailOSt1	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_ORIGIN_STATION1_DIST);
	if (pOutputColumnRailOSt1 == NULL)
	{
		ThrowException(_T("철송 Origin ~ Station1 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnRailSt1St2	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION1_STATION2_DIST);
	if (pOutputColumnRailSt1St2 == NULL)
	{
		ThrowException(_T("철송 Station1 ~ Station2 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnRailSt2D	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION2_DESTINATION_DIST);
	if (pOutputColumnRailSt2D == NULL)
	{
		ThrowException(_T("철송 Staion2 ~ Destination 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnRoadOVia	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_ORGIN_VIA_DIST);
	if (pOutputColumnRoadOVia == NULL)
	{
		ThrowException(_T("육송 Origin ~ via  필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnRoadViaD	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_VIA_DESTINATION_DIST);
	if (pOutputColumnRoadViaD == NULL)
	{
		ThrowException(_T("육송 via ~ Destination 필드가 없습니다."));
		return;
	}

	KIOColumn* pOutputColumnONodeID		 = pColumns->GetColumn(COLUMN_PARAMETER_OD_ONODE_ID);
	if (pOutputColumnONodeID == NULL)
	{
		ThrowException(_T("Origin Node 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnDNodeID		 = pColumns->GetColumn(COLUMN_PARAMETER_OD_DNODE_ID);
	if (pOutputColumnDNodeID == NULL)
	{
		ThrowException(_T("Destination Node 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnST1NodeID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION1_NODE_ID);
	if (pOutputColumnST1NodeID == NULL)
	{
		ThrowException(_T("Station1 Node 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnST2NodeID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION2_NODE_ID);
	if (pOutputColumnST2NodeID == NULL)
	{
		ThrowException(_T("Station2 Node 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnST1ZoneID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION1_ZONE_ID);
	if (pOutputColumnST1ZoneID == NULL)
	{
		ThrowException(_T("Station1 Zone 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnST2ZoneID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_STATION2_ZONE_ID);
	if (pOutputColumnST2ZoneID == NULL)
	{
		ThrowException(_T("Station2 Zone 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnViaNodeID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_ROAD_VIA_NODE);
	if (pOutputColumnViaNodeID == NULL)
	{
		ThrowException(_T("Road Via Node 필드가 없습니다."));
		return;
	}
	KIOColumn* pOutputColumnViaZoneID	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_ROAD_VIA_ZONE);
	if (pOutputColumnViaZoneID == NULL)
	{
		ThrowException(_T("Road Via Zone 필드가 없습니다."));
		return;
	}

	KIOColumn* pOutputColumnRoadTime	 = pColumns->GetColumn(COLUMN_PARAMETER_OD_DIRECT_TIME);
	if (pOutputColumnViaZoneID == NULL)
	{
		ThrowException(_T("Direct_Time 필드가 없습니다."));
		return;
	}

	columnCollection.AddNewColumn(pOutputColumnDirectDist);
	columnCollection.AddNewColumn(pOutputColumnONodeID);
	columnCollection.AddNewColumn(pOutputColumnDNodeID);
	columnCollection.AddNewColumn(pOutputColumnST1NodeID);
	columnCollection.AddNewColumn(pOutputColumnST2NodeID);
	columnCollection.AddNewColumn(pOutputColumnST1ZoneID);
	columnCollection.AddNewColumn(pOutputColumnST2ZoneID);
	columnCollection.AddNewColumn(pOutputColumnRailOSt1);
	columnCollection.AddNewColumn(pOutputColumnRailSt1St2);
	columnCollection.AddNewColumn(pOutputColumnRailSt2D);
	columnCollection.AddNewColumn(pOutputColumnViaNodeID);
	columnCollection.AddNewColumn(pOutputColumnViaZoneID);
	columnCollection.AddNewColumn(pOutputColumnRoadOVia);
	columnCollection.AddNewColumn(pOutputColumnRoadViaD);
	columnCollection.AddNewColumn(pOutputColumnRoadTime);
}

unsigned __stdcall KCalculatePathFindInfoDlg::CalaculatePathThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("경로 탐색 계산 중입니다."));

	KCalculatePathFindInfoDlg* pDlg = (KCalculatePathFindInfoDlg*)pParameter->GetParameter();
	pDlg->CalculatePathFind();
	//pDlg->WriteCalculateData();
	return 0;
}

void KCalculatePathFindInfoDlg::ResetParameterTable()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format(_T("Delete From %s"), TABLE_PARAMETER_OD);
		spDBaseConnection->ExecuteUpdate(strSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KCalculatePathFindInfoDlg::WritePathLog( CString a_strLog )
{
	try
	{
		if (m_spPathLogger == NULL)
		{
			m_spPathLogger = QBicLoggerPtr(new QBicLogger());			
			if (!m_spPathLogger->LoggerOpen(m_pTarget->GetLocation(), _T("FPathFind")))
			{
				m_spPathLogger.reset();
				return;
			}
		}

		m_spPathLogger->WriteLog(a_strLog);
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

void KCalculatePathFindInfoDlg::ClearFile()
{
	remove((CStringA)ConvertLocation(_T("FPathFind.txt")));
}

CString KCalculatePathFindInfoDlg::ConvertLocation( CString a_strFile )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), _T("\\") ,a_strFile);
	return strSavePath;
}
