#include "StdAfx.h"
#include "PathFindZoneInfo.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "PurposeInfoCollection.h"
#include "IncludeIOTable.h"
#include "PurposeManager.h"
#include "PAPurpose.h"
#include "../kmzModel/KShortestPath.h"
#include "../kmzModel/KLinkCollection.h"
#include "../kmzModel/KCostLinkCollection.h"
#include "../kmzModel/KNodeArrivedMinLink.h"
#include "DBaseConnection.h"
#include "ShortestPathInfo.h"
#include "KBulkDBase.h"
#include "SimpleProgress.h"
#include "PathCalculateCommdityDlg.h"
#include "../kmzModel/KODKey.h"
#include "../kmzModel/KIntegerRecord.h"
#include "../kmzModel/KODKeyIntegerRecords.h"


using namespace PathFindZoneInfo_Structure;

KPathFindZoneInfo::KPathFindZoneInfo(void)
{
}


KPathFindZoneInfo::~KPathFindZoneInfo(void)
{
}

#define RoadNode   1 
#define RailNode   2



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

void KPathFindZoneInfo::SetInfo( KTarget* a_pTarget, KPurpose* a_pPurpose )
{
	if (a_pTarget != NULL)
	{
		m_pTarget = a_pTarget;
	}
	
	if (a_pPurpose != NULL)
	{
		m_pPurpose = a_pPurpose;
	}
}


bool KPathFindZoneInfo::GetZoneNodeInfo()
{
	KLogDebugStartMsg();
	m_setZoneNode.clear();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type,Zone_ID FROM %s ORDER BY Node_Id "), TABLE_NODE);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId		= 0;
	Integer nxZoneID        = 0;
	CString strNodeName		= _T("");
	int		nNodeType		= 0;

	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneID    = pResult->GetValueInt64 (3);
		m_setAllNode.insert(nxNodeId);

		if (nNodeType == 0 )
		{
			m_setZoneNode.insert(nxNodeId);
			m_mapZoneNodeInfo.insert(std::make_pair(nxNodeId, nxZoneID));
		}
	}
	KLogDebugEndMsg();
	return true;
}


bool KPathFindZoneInfo::GetRoadFacilitiesNodeInfo()
{
	KLogDebugStartMsg();
	m_setRoadFacilitiesNode.clear();
	m_mapRoadFacilitesNodeInfo.clear();

	CString strTrafficType  = _T("");
	
	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strTrafficSQL;
	strTrafficSQL.Format (_T("SELECT Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficTypeResult = oDBaseConnetion.ExecuteQuery(strTrafficSQL);
	if (pTrafficTypeResult == NULL)
	{
		return false;
	}

	std::set<CString> setUseTrafficType;

	while (pTrafficTypeResult->Next())
	{
		strTrafficType = pTrafficTypeResult->GetValueString(0);

		CString strElement      = _T("");
		int     nSub            = 0;
		CString strMathingElement = _T("");
		strMathingElement.Format (_T("%d"), 1);

		while(AfxExtractSubString(strElement, strTrafficType, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			if (strElement == strMathingElement)
			{
				setUseTrafficType.insert(strTrafficType);
			}
		}
	}

	CString strTrafficTypeSQL = _T("");
	std::set<CString>::iterator iterTraffic = setUseTrafficType.begin();
	std::set<CString>::iterator endTraffic = setUseTrafficType.end();
	while(iterTraffic != endTraffic)
	{
		strTrafficTypeSQL += _T("'");
		strTrafficTypeSQL += *iterTraffic;
		strTrafficTypeSQL += _T("'");
		iterTraffic ++;
		if (iterTraffic != endTraffic)
		{
			strTrafficTypeSQL += _T(",");
		}
	}

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type, Zone_ID FROM %s where Node_type = 2 and traffic_mean in (%s) ORDER BY Node_Id "), TABLE_NODE, strTrafficTypeSQL);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId        = 0;
	int     nNodeType       = 0;
	Integer nxZoneId        = 0;
	CString strNodeName		= _T("");

	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneId    = pResult->GetValueInt64 (3);

		m_setRoadFacilitiesNode.insert(nxNodeId);
		m_mapRoadFacilitesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));
	}

	KLogDebugEndMsg();
	return true;
}

bool KPathFindZoneInfo::GetRailFacilitiesNodeInfo()
{
	KLogDebugStartMsg();
	m_setRailFacilitiesNode.clear();
	CString strTrafficType = _T("");
	CString strElement      = _T("");
	int     nSub           = 0;

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strTrafficSQL;
	strTrafficSQL.Format (_T("SELECT Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficTypeResult = oDBaseConnetion.ExecuteQuery(strTrafficSQL);
	if (pTrafficTypeResult == NULL)
	{
		return false;
	}

	std::set<CString> setUseTrafficType;

	while (pTrafficTypeResult->Next())
	{
		strTrafficType = pTrafficTypeResult->GetValueString(0);

		CString strElement      = _T("");
		int     nSub           = 0;
		CString strMathingElement = _T("");
		strMathingElement.Format (_T("%d"), 2);
		while(AfxExtractSubString(strElement, strTrafficType, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			if (strElement == strMathingElement)
			{
				setUseTrafficType.insert(strTrafficType);
			}
		}
	}

	CString strTrafficTypeSQL = _T("");
	std::set<CString>::iterator iterTraffic = setUseTrafficType.begin();
	std::set<CString>::iterator endTraffic = setUseTrafficType.end();
	while(iterTraffic != endTraffic)
	{
		strTrafficTypeSQL += _T("'");
		strTrafficTypeSQL += *iterTraffic;
		strTrafficTypeSQL += _T("'");
		iterTraffic ++;
		if (iterTraffic != endTraffic)
		{
			strTrafficTypeSQL += _T(",");
		}
	}

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type, Zone_id FROM %s where Node_type = 2 and traffic_mean in (%s) ORDER BY Node_Id "), TABLE_NODE, strTrafficTypeSQL);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId        = 0;
	Integer nxZoneId        = 0;
	int     nNodeType       = 0;
	CString strNodeName		= _T("");

	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneId    = pResult->GetValueInt64 (3);

		m_mapRailFacilitesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));
		m_setRailFacilitiesNode.insert(nxNodeId);
	}

	KLogDebugEndMsg();
	return true;
}

bool KPathFindZoneInfo::GetRoadLinkData()
{
	KLogDebugStartMsg();
	m_oRoadColLink.RemoveAll();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strSQL;
	strSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type in (%d, %d, %d, %d, %d)"),
		_T("length"), TABLE_LINK , Link_Type_Zone, LInk_Type_Highway, 
		LInk_Type_Expressway, LInk_Type_GeneralRoad, LInk_Type_AccessRoad);
	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	LinkIDType nxLinkId		   = 0;
	Integer    nxFNodeId       = 0;
	Integer    nxTNodeId       = 0;
	Integer    nxExcludeNodeID = 0;
	Double     dCost           = 0.0;
	CString    strExcludeNode  = _T("");
	KLink*     pLink           = NULL;

	while( pResult->Next() )
	{

		nxLinkId	= pResult->GetValueInt64 (0);
		nxFNodeId	= pResult->GetValueInt64 (1);
		nxTNodeId	= pResult->GetValueInt64 (2);
		dCost	    = pResult->GetValueDouble(3);

		pLink = m_oRoadColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);
	}

	std::set<Integer>::iterator iter = m_setExcludeNodeID.begin();
	std::set<Integer>::iterator end  = m_setExcludeNodeID.end();
	while (iter!= end)
	{
		nxExcludeNodeID          = *iter;
		CString strNodeID = _T("");
		strNodeID.Format(_T("%s"), nxExcludeNodeID);

		strExcludeNode += strNodeID;

		iter++;

		if (iter != end)
		{
			strExcludeNode += _T(", ");
		}
	}


	CString strExcludeSQL = _T("");
	strExcludeSQL.Format(_T(" Select Link_Id, FNode_Id, TNode_Id, %s FROM %s Where (fnode_id not in (%s) and link_type = %d) or (tnode_id not in (%s) and link_type =%d) "),
		_T("length"), TABLE_LINK, strExcludeNode, LInk_Type_Shuttle, strExcludeNode, LInk_Type_Shuttle);

	KResultSetPtr pExcludeResult = oDBaseConnetion.ExecuteQuery(strExcludeSQL);
	if(pExcludeResult == NULL)
	{
		return false;
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

	}

	KLogDebugEndMsg();
	return true;
}


bool KPathFindZoneInfo::GetUsingCommdityNode()
{
	int nCommdityID = m_pPurpose->PurposeID();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());

	CString strSQL;
	strSQL.Format (_T(" SELECT node_id, available_item FROM %s Where node_type = 2"), TABLE_NODE);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeID = 0;
	CString strAvailableItem = _T("");

	while (pResult->Next())
	{
		nxNodeID	     = pResult->GetValueInt64 (0);
		strAvailableItem = pResult->GetValueString(1);

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

		std::set<int>::iterator iterfind = setCodeValue.find(nCommdityID);
		std::set<int>::iterator findend  = setCodeValue.end();
		if (iterfind == findend)
		{
			m_setExcludeNodeID.insert(nxNodeID);
		}
	}
	return true;
}

bool KPathFindZoneInfo::GetRailLinkData()
{
	KLogDebugStartMsg();
	m_oRailColLink.RemoveAll();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());

	CString strRailSQL;
	strRailSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type = %d"), 
		_T("length"), TABLE_LINK, LInk_Type_RailWay );

	KResultSetPtr pRailResult = oDBaseConnetion.ExecuteQuery(strRailSQL);
	if(pRailResult == NULL)
	{
		return false;
	}

	LinkIDType nxLinkId		= 0;
	Integer    nxFNodeId    = 0;
	Integer    nxTNodeId    = 0;
	Double     dCost        = 0.0;
	KLink*     pLink        = NULL;

	while( pRailResult->Next() )
	{
		nxLinkId	= pRailResult->GetValueInt64 (0);
		nxFNodeId	= pRailResult->GetValueInt64 (1);
		nxTNodeId	= pRailResult->GetValueInt64 (2);
		dCost	    = pRailResult->GetValueDouble(3);

		pLink = m_oRailColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);
	}

	KLogDebugEndMsg();
	return true;
}


void KPathFindZoneInfo::SaveAllPathFindInfo()
{


	//시작 노드에서 모든 노드까지
	std::map<Integer, Integer>::iterator iterOriginRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endOriginRoad  = m_mapZoneNodeInfo.end();
	while (iterOriginRoad != endOriginRoad)
	{
		Integer nxOrginNodeID = iterOriginRoad->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oShortestPath;
		oShortestPath.MuLinkBaseExecute(nxOrginNodeID, m_setAllNode, m_oRoadColLink, *pCostLinkCollection, *pNodeArrivedMinLink);
		m_mapRoadOriginMinLink.insert(std::make_pair(nxOrginNodeID, pNodeArrivedMinLink));
		delete pCostLinkCollection;
		iterOriginRoad++;
	}

	//육송 물류시설 부터 모든 존 노드까지 
	std::map<Integer, Integer>::iterator iterDestinationRoad = m_mapRoadFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestiantionRoad  = m_mapRoadFacilitesNodeInfo.end();
	while(iterDestinationRoad != endDestiantionRoad)
	{
		Integer nxFNodeID = iterDestinationRoad->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRoadFacilitiesPath;
		oRoadFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setZoneNode, m_oRoadColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRoadDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterDestinationRoad++;
	}

	//철송 물류시설부터 모든존노드까지
	std::map<Integer, Integer>::iterator iterDestinationRail = m_mapRailFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestinationRail  = m_mapRailFacilitesNodeInfo.end();
	while(iterDestinationRail != endDestinationRail)
	{
		Integer nxFNodeID = iterDestinationRail->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setZoneNode, m_oRoadColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterDestinationRail++;
	}

	//철송 물류시설부터 철송 물류시설까지
	std::map<Integer, Integer>::iterator iterRaildStation = m_mapRailFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRailStation  = m_mapRailFacilitesNodeInfo.end();
	while(iterRaildStation != endRailStation)
	{
		Integer nxFNodeID = iterRaildStation->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setRailFacilitiesNode, m_oRailColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailStationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterRaildStation++;
	}

}


void KPathFindZoneInfo::ClearNodeArrivedMInInfo()
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
	m_oAllCostLinkCollection.RemoveAll();
	m_oAllNodeArrivedMinLink.RemoveAll();

	m_mapZoneNodeInfo.clear();
	m_mapRoadFacilitesNodeInfo.clear();
	m_mapRailFacilitesNodeInfo.clear();

	m_setExcludeNodeID.clear();
	m_setZoneNode.clear();
	m_setFacilitiesNode.clear();
	m_setRailFacilitiesNode.clear();
	m_setRoadFacilitiesNode.clear();


}


void KPathFindZoneInfo::GetFindPathFacilitiesRoadResult(Integer a_nxStrartNodeID, Integer a_nxStartZoneID)
{
	KLogDebugStartMsg();

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

		std::map<Integer, KNodeArrivedMinLink*>::iterator iterORoad  = m_mapRoadOriginMinLink.find(a_nxStrartNodeID);
		std::map<Integer, KNodeArrivedMinLink*>::iterator endORoad   = m_mapRoadOriginMinLink.end();
		KNodeArrivedMinLink* pNodeArrivedMinLink = iterORoad->second;

		std::map<Integer, Integer>::iterator iterFacilities = m_mapRoadFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndFacilities  = m_mapRoadFacilitesNodeInfo.end();
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
						dMinOriginValue       = dOrginValue;
						dMInDestinaionValue   = dDestinationValue;
						nxMinFacilitiesID     = nxFacilitiesNodeID;
						nxMinZoneFacilitiesID = nxFacilitiesZoneID;
						dMinSumValue          = dSumValue;
						bRoadFindPath         = true;

					} else if (dMinSumValue > dSumValue)
					{
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

		if (a_nxStartZoneID == nxEndZoneID)
		{
			
			nxMinZoneFacilitiesID = 0;
		}
		else if (bRoadFindPath == false)
		{
			nxMinZoneFacilitiesID = 0;
		}

		KODKey oODKey (a_nxStartZoneID, nxEndZoneID);
		std::map<KODKey, KIntegerRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KIntegerRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KIntegerRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KIntegerRecord(3);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}

		pRecord->SetAt(RoadFZ, nxMinZoneFacilitiesID);

		iterRoad++;
	}
	KLogDebugEndMsg();
}

void KPathFindZoneInfo::GetFindPathRailResult( Integer a_nxStartNodeID, Integer a_nxstartZoneID )
{
	KLogDebugStartMsg();
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

		std::map<Integer, Integer>::iterator iterOFacilities = m_mapRailFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndOFacilities  = m_mapRailFacilitesNodeInfo.end();
		while (iterOFacilities != EndOFacilities)
		{
			Integer nxOFacilitiesNode = iterOFacilities->first;

			KMinLinkInfo* pOriginLinkInfo = pOriginArrivedMinLink->GetMinLink(nxOFacilitiesNode);
			if (pOriginLinkInfo != NULL)
			{
				double dOrginValue = pOriginLinkInfo->Cost; 
				if (bFindOriginPath == false)
				{
					nxOriginFacilitiesNode = nxOFacilitiesNode;
					nxOriginFacilitiesZone = iterOFacilities->second;
					dMinOriginValue        =  dOrginValue;
					bFindOriginPath = true;
				} 
				else if (dMinOriginValue > dOrginValue)
				{
					nxOriginFacilitiesNode = nxOFacilitiesNode;
					nxOriginFacilitiesZone = iterOFacilities->second;
					dMinOriginValue        =  dOrginValue;
				}

			}
			iterOFacilities++;
		}

		std::map<Integer, Integer>::iterator iterDFacilities = m_mapRailFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndDFacilities  = m_mapRailFacilitesNodeInfo.end();
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
						bFindDestinationPath = true;
					} else if (dMinDestinationValue > dDestinaionValue)
					{
						nxDestinationFacilitiesNode = nxDFaciliteiesNode;
						nxDestinationFacilitiesZone = iterDFacilities->second;
						dMinDestinationValue        = dDestinaionValue;
					}
				}
			}
			iterDFacilities++;
		}

		if (a_nxstartZoneID == nxEndZoneID)
		{
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesZone = 0;
		}
		else if (bFindOriginPath == false || bFindDestinationPath == false)
		{
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesZone = 0;
		}
		else
		{
			std::map<Integer, KNodeArrivedMinLink*>::iterator iterStation   = m_mapRailStationMinLink.find(nxOriginFacilitiesNode);
			std::map<Integer, KNodeArrivedMinLink*>::iterator endDStation   = m_mapRailStationMinLink.end();
			KNodeArrivedMinLink* pNodeArrivedMinLink = iterStation->second;

			KMinLinkInfo* pRailLinkInfo = pNodeArrivedMinLink->GetMinLink(nxDestinationFacilitiesNode);
			if (pRailLinkInfo != NULL)
			{
				dMinRailValue = pRailLinkInfo->Cost;
			}
			else
			{
				nxOriginFacilitiesZone      = 0;
				nxDestinationFacilitiesZone = 0;
			}

		}

		KODKey oODKey (a_nxstartZoneID, nxEndZoneID);
		std::map<KODKey, KIntegerRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KIntegerRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KIntegerRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KIntegerRecord(3);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}
		pRecord->SetAt(RailFSZ, nxOriginFacilitiesZone);
		pRecord->SetAt(RailFEZ, nxDestinationFacilitiesZone);

		iterRoad++;
	}
	KLogDebugEndMsg();
}

void KPathFindZoneInfo::CalculateZoneInfo( KODKeyIntegerRecords& a_ODKeyIntegerRecords )
{
	ClearNodeArrivedMInInfo();

	GetZoneNodeInfo();
	GetRoadFacilitiesNodeInfo();
	GetRailFacilitiesNodeInfo();
	GetUsingCommdityNode();
	GetRoadLinkData();
	GetRailLinkData();
	SaveAllPathFindInfo();

	std::map<Integer, Integer>::iterator iter = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator end  = m_mapZoneNodeInfo.end();
	while (iter != end)
	{
		Integer nxSNodeID = iter->first;
		Integer nxSZoneID = iter->second;
		GetFindPathFacilitiesRoadResult(nxSNodeID, nxSZoneID);
		GetFindPathRailResult(nxSNodeID, nxSZoneID);
		++iter;
	}

	std::map<KODKey, KIntegerRecord*>::iterator iterResult = m_mapResultRecord.begin();
	std::map<KODKey, KIntegerRecord*>::iterator endResult  = m_mapResultRecord.end();
	while (iterResult != endResult)
	{
		KODKey oODKey = iterResult->first;
		KIntegerRecord* pResultRecord = a_ODKeyIntegerRecords.AddRecord(oODKey);
		KIntegerRecord* pRecord = NULL;
		pRecord = iterResult->second;
		Integer nxRoad   =  pRecord->GetAt(RoadFZ);
		Integer nxRailO  =  pRecord->GetAt(RailFSZ);
		Integer nxRailD  =  pRecord->GetAt(RailFEZ);

		pResultRecord->SetAt(RoadFZ, nxRoad);
		pResultRecord->SetAt(RailFSZ, nxRailO);
		pResultRecord->SetAt(RailFEZ, nxRailD);

		iterResult++;
	}


}


