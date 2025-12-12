#include "stdafx.h"
#include "BoundaryIntersectTransit.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "BulkDBaseNode.h"

void KBoundaryIntersectTransit::AddBoundaryIntersectTransit( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{
    try
    {    	
        std::map<Integer, std::set<Integer>> mapNodeTransits;
        std::map<Integer, std::set<Integer>> mapTransitZoneNodes;
        //노선검색 - 노드별 노선목록, 노선별 존ID 목록
        TransitToNodeID(a_pTarget, mapNodeTransits, mapTransitZoneNodes);

        std::map<Integer, std::set<Integer>>::iterator iter = mapNodeTransits.begin();
        std::map<Integer, std::set<Integer>>::iterator end  = mapNodeTransits.end();

        std::set<Integer> setNodeID;
        size_t nxCount = ar_vecNodeID.size();

        std::set<Integer> setFindTransit;
        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxNodeID = ar_vecNodeID[i];
            setNodeID.insert(nxNodeID);

            iter = mapNodeTransits.find(nxNodeID);

            if (iter != end)
            {
                std::set<Integer>& setTransit = iter->second;
                std::set<Integer>::iterator iterT = setTransit.begin();
                std::set<Integer>::iterator endT  = setTransit.end();

                while(iterT != endT)
                {
                    setFindTransit.insert(*iterT);
                    ++iterT;
                }
            }
        }

        iter = mapTransitZoneNodes.begin();
        end  = mapTransitZoneNodes.end();

        // 현재 선택된 노드를 통과하는 노선검색 목록 : setFindTransit
        std::set<Integer>::iterator iterT = setFindTransit.begin();
        std::set<Integer>::iterator endT  = setFindTransit.end();

        std::set<Integer> setTransitZoneNode;
        while(iterT != endT)
        {
            Integer nxTransitID = *iterT;
            iter = mapTransitZoneNodes.find(nxTransitID);
            if (iter != end)
            {
                std::set<Integer>& setZoneNode = iter->second;
                std::set<Integer>::iterator iterN = setZoneNode.begin();
                std::set<Integer>::iterator endN  = setZoneNode.end();

                while(iterN != endN)
                {                    
                    setTransitZoneNode.insert(*iterN);
                    ++iterN;
                }
            }

            ++iterT;
        }
        // 노선을 통과하는 존 노드목록 : setTransitZoneNode
        std::vector<TNodeIDZoneID> vecAllNodes;
        KBulkDBaseNode::Nodes(a_pTarget, vecAllNodes);
        
        nxCount = vecAllNodes.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TNodeIDZoneID& oNodeIDZoneID = vecAllNodes[i];
            if (setTransitZoneNode.find(oNodeIDZoneID.ZoneID) != setTransitZoneNode.end())
            {
                setNodeID.insert(oNodeIDZoneID.NodeID);
            }
        }

        //기존 선택된 노드 + 
        std::set<Integer>::iterator iterN = setNodeID.begin();
        std::set<Integer>::iterator endN  = setNodeID.end();

        ar_vecNodeID.clear();
        while(iterN != endN)
        {
            ar_vecNodeID.push_back(*iterN);
            ++iterN;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KBoundaryIntersectTransit::TransitToNodeID( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapNodeTransits, std::map<Integer, std::set<Integer>>& ar_mapTransitZoneNodes )
{
    CString strSQL(_T(""));
    strSQL.Append(_T(" Select " ));
    strSQL.Append(_T("   tb.node_id as node_id, Ta.Transit_ID as Transit_ID, tb.zone_id as zone_id "));
    strSQL.Append(_T(" From "));
    strSQL.Append(_T("   ( "));
    strSQL.Append(_T("     Select "));
    strSQL.Append(_T("       Tb.tnode_id, Ta.Transit_ID "));
    strSQL.Append(_T("     From "));
    strSQL.Append(_T("       Transit_Links Ta, Link Tb "));
    strSQL.Append(_T("     Where "));
    strSQL.Append(_T("       Ta.Link_ID = Tb.Link_ID "));
    strSQL.Append(_T("   ) Ta, "));
    strSQL.Append(_T("   Node Tb "));
    strSQL.Append(_T(" Where "));
    strSQL.Append(_T("   Ta.tnode_id = tb.node_id "));
        
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( pResultSet->Next() )
        {
            Integer nxNodeID    = pResultSet->GetValueInt64(0);
            Integer nxTransitID = pResultSet->GetValueInt64(1);
            Integer nxZoneID    = pResultSet->GetValueInt64(2);

            std::map<Integer, std::set<Integer>>::iterator iter = ar_mapNodeTransits.begin();
            std::map<Integer, std::set<Integer>>::iterator end  = ar_mapNodeTransits.end();

            iter = ar_mapNodeTransits.find(nxNodeID);
            if (iter != end)
            {
                std::set<Integer>& setTransits = iter->second;
                setTransits.insert(nxTransitID);
            }
            else
            {
                std::set<Integer> setTransits;
                setTransits.insert(nxTransitID);
                ar_mapNodeTransits.insert(std::make_pair(nxNodeID, setTransits));
            }

            iter = ar_mapTransitZoneNodes.find(nxTransitID);
            end  = ar_mapTransitZoneNodes.end();
            if (iter != end)
            {
                std::set<Integer>& setZoneNodes = iter->second;
                setZoneNodes.insert(nxZoneID);
            }
            else
            {
                std::set<Integer> setZoneNodes;
                setZoneNodes.insert(nxZoneID);
                ar_mapTransitZoneNodes.insert(std::make_pair(nxTransitID, setZoneNodes));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBoundaryIntersectTransit::InterSectionTransitNodeData( KTarget* a_pTarget, std::set<Integer>& ar_setNodeID, std::set<Integer>& ar_setLinkID)
{
	CString strSQL(_T(""));
	ar_setNodeID.clear();
	ar_setLinkID.clear();

	try
	{
		KDBaseConPtr spDBConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select tA.transit_id, tA.link_id, tA.seq, tA.station_yn,  tB.fnode_id, tB.tnode_id from %s tA, %s tB where tA.link_id = tB.link_id "), TABLE_TRANSITLINKS, TABLE_LINK); //★ 필요

		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nxTransitID = spResultset->GetValueInt64(0);
			Integer nxLinkID	= spResultset->GetValueInt64(1);
			int		nSeq		= spResultset->GetValueInt(2);
			int		nStaitionyn	= spResultset->GetValueInt(3);
			Integer nxFNodeID	= spResultset->GetValueInt64(4);
			Integer nxTNodeID	= spResultset->GetValueInt64(5);

			if (nSeq = 1)
			{
				ar_setNodeID.insert(nxFNodeID);
			}

			if (nStaitionyn = 1)
			{
				ar_setNodeID.insert(nxTNodeID);
			}

			ar_setLinkID.insert(nxLinkID);
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

/*백업
void KBoundaryIntersectTransit::AddBoundaryIntersectTransit( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{
    try
    {    	
        std::map<Integer, std::set<Integer>> mapNodeTransits;
        std::map<Integer, std::set<Integer>> mapTransitNodes;
        TransitToNodeID(a_pTarget, mapNodeTransits, mapTransitNodes);

        std::map<Integer, std::set<Integer>>::iterator iter = mapNodeTransits.begin();
        std::map<Integer, std::set<Integer>>::iterator end  = mapNodeTransits.end();

        std::set<Integer> setNodeID;
        size_t nxCount = ar_vecNodeID.size();

        std::set<Integer> setFindTransit;
        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxNodeID = ar_vecNodeID[i];
            setNodeID.insert(nxNodeID);

            iter = mapNodeTransits.find(nxNodeID);

            if (iter != end)
            {
                std::set<Integer>& setTransit = iter->second;
                std::set<Integer>::iterator iterT = setTransit.begin();
                std::set<Integer>::iterator endT  = setTransit.end();

                while(iterT != endT)
                {
                    setFindTransit.insert(*iterT);
                    ++iterT;
                }
            }
        }

        iter = mapTransitNodes.begin();
        end  = mapTransitNodes.end();

        std::set<Integer>::iterator iterT = setFindTransit.begin();
        std::set<Integer>::iterator endT  = setFindTransit.end();

        while(iterT != endT)
        {
            Integer nxTransitID = *iterT;
            iter = mapTransitNodes.find(nxTransitID);
            if (iter != end)
            {
                std::set<Integer>& setNode = iter->second;
                std::set<Integer>::iterator iterN = setNode.begin();
                std::set<Integer>::iterator endN  = setNode.end();

                while(iterN != endN)
                {
                    setNodeID.insert(*iterN);
                    ++iterN;
                }
            }

            ++iterT;
        }

        std::set<Integer>::iterator iterN = setNodeID.begin();
        std::set<Integer>::iterator endN  = setNodeID.end();

        ar_vecNodeID.clear();
        while(iterN != endN)
        {
            ar_vecNodeID.push_back(*iterN);
            ++iterN;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

void KBoundaryIntersectTransit::TransitToNodeID( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapNodeTransits, std::map<Integer, std::set<Integer>>& ar_mapTransitNodes )
{
    CString strSQL(_T(""));
    strSQL.Append(_T(" Select Tb.tnode_id, Ta.Transit_ID From Transit_Links Ta, Link Tb "));
    strSQL.Append(_T("    Where " ));
    strSQL.Append(_T("    Ta.Link_ID = Tb.Link_ID "));

	KDBaseConPtr spDBConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  pResultSet  = spDBConnectioin->ExecuteQuery(strSQL);

        while ( pResultSet->Next() )
        {
            Integer nxNodeID    = pResultSet->GetValueInt64(0);
            Integer nxTransitID = pResultSet->GetValueInt64(1);

            std::map<Integer, std::set<Integer>>::iterator iter = ar_mapNodeTransits.begin();
            std::map<Integer, std::set<Integer>>::iterator end  = ar_mapNodeTransits.end();

            iter = ar_mapNodeTransits.find(nxNodeID);
            if (iter != end)
            {
                std::set<Integer>& setTransits = iter->second;
                setTransits.insert(nxTransitID);
            }
            else
            {
                std::set<Integer> setTransits;
                setTransits.insert(nxTransitID);
                ar_mapNodeTransits.insert(std::make_pair(nxNodeID, setTransits));
            }

            iter = ar_mapTransitNodes.find(nxTransitID);
            end  = ar_mapTransitNodes.end();
            if (iter != end)
            {
                std::set<Integer>& setNodes = iter->second;
                setNodes.insert(nxNodeID);
            }
            else
            {
                std::set<Integer> setNodes;
                setNodes.insert(nxNodeID);
                ar_mapTransitNodes.insert(std::make_pair(nxTransitID, setNodes));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}
*/