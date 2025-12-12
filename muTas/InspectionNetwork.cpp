#include "stdafx.h"
#include "InspectionNetwork.h"

#include "Target.h"

KInspectionNetwork::KInspectionNetwork()
{

}

KInspectionNetwork::~KInspectionNetwork()
{

}

void KInspectionNetwork::Inspection( KTarget* a_pTarget, 
                                     std::set<int> a_setIncludeNodeType, 
                                     std::set<int> a_setIncludeLinkType, 
                                     std::set<Integer>& ar_setUsableNode, 
                                     std::set<Integer>& ar_setUsableLink )
{
    try
    {
        // <LinkID>, <FromNode, TNode, Link Type>
        std::map<Integer, TLinkFTNodeType> mapLinkFTNodeType;
        KBulkDBaseLink::AllLinkFTNodeType(a_pTarget, mapLinkFTNodeType);

        // <NodeID>, <Node Type>
        std::map<Integer, int>   mapNodeIDType;
        KBulkDBaseNode::NodeType(a_pTarget, mapNodeIDType);

        ar_setUsableNode.clear();
        ar_setUsableLink.clear();
        
        AutoType iterMapNodeType        = mapNodeIDType.begin();
        AutoType endMapNodeType         = mapNodeIDType.end();

        AutoType iterIncludeNodeType    = a_setIncludeNodeType.begin();
        AutoType endIncludeNodeType     = a_setIncludeNodeType.end();

        AutoType iterIncludeLinkType    = a_setIncludeLinkType.begin();
        AutoType iterEndIncludeLinkType = a_setIncludeLinkType.end();

        for(AutoType iter=mapLinkFTNodeType.begin(); iter != mapLinkFTNodeType.end(); ++iter)
        {            
            TLinkFTNodeType& oTLinkFTNodeType = iter->second;
            Integer  nxLinkID      = iter->first;
            Integer& nxFromNodeID  = oTLinkFTNodeType.TFromNodeID;
            Integer& nxToNodeID    = oTLinkFTNodeType.TToNodeID;
            int&     nLinkType     = oTLinkFTNodeType.TType;

            iterIncludeLinkType = a_setIncludeLinkType.find(oTLinkFTNodeType.TType);
            if (iterIncludeLinkType != iterEndIncludeLinkType)
            {// 포함링크 -> 노드검색 -> 제거대상 노드 -> 링크 추가 X
                iterMapNodeType = mapNodeIDType.find(nxFromNodeID);
                if (iterMapNodeType != endMapNodeType)
                {
                    int nFromNodeType   = iterMapNodeType->second;
                    iterIncludeNodeType = a_setIncludeNodeType.find(nFromNodeType);
                    if (iterIncludeNodeType != endIncludeNodeType)
                    {//FromNode는 생존가능
                        iterMapNodeType = mapNodeIDType.find(nxToNodeID);    
                        if (iterMapNodeType != endMapNodeType)
                        {
                            int nToNodeType     = iterMapNodeType->second;
                            iterIncludeNodeType = a_setIncludeNodeType.find(nToNodeType);
                            if (iterIncludeNodeType != endIncludeNodeType)
                            {// ToNode 생존 가능
                                ar_setUsableLink.insert(nxLinkID);
                                ar_setUsableNode.insert(nxFromNodeID);
                                ar_setUsableNode.insert(nxToNodeID);
                            }
                            else
                            {
                                CString strTemp(_T(""));
                                strTemp.Format(_T("Filter Out : %I64d"), nxLinkID);
                                TxLogDebug((LPCTSTR)strTemp);
                            }
                        }
                        else
                        {//링크는 존재하는데 종료 노드는 없다. 말도 안됨.
                            CString strTemp(_T(""));
                            strTemp.Format(_T("Filter Out : %I64d"), nxLinkID);
                            TxLogDebug((LPCTSTR)strTemp);
                        }
                    }
                }
                else
                {//링크는 존재하는데 시작 노드는 없다. 말도 안됨.
                    CString strTemp(_T(""));
                    strTemp.Format(_T("Filter Out : %I64d"), nxLinkID);
                    TxLogDebug((LPCTSTR)strTemp);
                }                
            }
            else
            {
                CString strTemp(_T(""));
                strTemp.Format(_T("Filter Out : %I64d"), nxLinkID);
                TxLogDebug((LPCTSTR)strTemp);
            }
        }

        // 존노드는 무조건 추가
        std::vector<Integer> vecZoneNodeID;
        KBulkDBaseNode::Nodes(a_pTarget, vecZoneNodeID, 0);

        size_t nxCount = vecZoneNodeID.size();
        for (size_t i=0; i<nxCount; i++)
        {
            ar_setUsableNode.insert(vecZoneNodeID[i]);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}

void KInspectionNetwork::Inspection( KTarget* a_pTarget, std::set<int> a_setIncludeNodeType, std::set<int> a_setIncludeLinkType, 
                                                         std::set<Integer>& ar_setUsableNode, std::set<Integer>& ar_setUsableLink, 
                                                         std::set<Integer>& ar_setTurnID )
{
    try
    {
        ar_setTurnID.clear();

        Inspection(a_pTarget, a_setIncludeNodeType, a_setIncludeLinkType, ar_setUsableNode, ar_setUsableLink);
        
        AutoType iterUsableLink = ar_setUsableLink.begin();
        AutoType endUsableLink  = ar_setUsableLink.end();
        
        //      <TurnID>, <InLinkID, OutLinkID>
        std::map<Integer, TTurnInOutLink> mapTurnInOutLink;
        KBulkDBaseTurn::TurnInOutLink(a_pTarget, mapTurnInOutLink);

        AutoType iterTurn       = mapTurnInOutLink.begin();      
        AutoType endTurn        = mapTurnInOutLink.end();

        while(iterTurn != endTurn)
        {
            TTurnInOutLink& oTurnInOutLink = iterTurn->second;
            iterUsableLink = ar_setUsableLink.find(oTurnInOutLink.TInLinkID);

            if (iterUsableLink != endUsableLink)
            { //진입 링크 검색 통과
                iterUsableLink = ar_setUsableLink.find(oTurnInOutLink.TOutLinkID);
                if (iterUsableLink != endUsableLink)
                { //진출 링크 검색 통과
                    ar_setTurnID.insert(iterTurn->first);
                }
            }
            ++iterTurn;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

void KInspectionNetwork::Inspection( KTarget* a_pTarget, std::set<int> a_setIncludeNodeType,  std::set<int> a_setIncludeLinkType, 
                                                         std::set<Integer>& ar_setUsableNode, std::set<Integer>& ar_setUsableLink, 
                                                         std::set<Integer>& ar_setTurnID,     std::set<Integer>& ar_setTransitID )
{
    try
    {
        ar_setTransitID.clear();
        Inspection(a_pTarget, a_setIncludeNodeType, a_setIncludeLinkType, ar_setUsableNode, ar_setUsableLink, ar_setTurnID);

        AutoType iterUsableLink = ar_setUsableLink.begin();
        AutoType endUsableLink  = ar_setUsableLink.end();

        //      <LinkID, TransitID>
        std::set<KODKey> setLinkTransitID;
        KBulkDBaseTransit::UsedLinkInTransit(a_pTarget, setLinkTransitID, ar_setTransitID);
                
        AutoType iterLinkTransit = setLinkTransitID.begin();
        AutoType endLinkTransit  = setLinkTransitID.end();

        while (iterLinkTransit != endLinkTransit)
        {
            KODKey  oKey     = *iterLinkTransit;
            Integer nxLinkID = oKey.OriginID;

            iterUsableLink   = ar_setUsableLink.find(nxLinkID);
            if (iterUsableLink == endUsableLink)
            {//제거
                ar_setTransitID.erase(oKey.DestinationID);
            }

            ++iterLinkTransit;
        }        
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}
