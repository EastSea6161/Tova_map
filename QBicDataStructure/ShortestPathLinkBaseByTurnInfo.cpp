#include "ShortestPathLinkBaseByTurnInfo.h"


KShortestPathLinkBaseByTurnInfo::KShortestPathLinkBaseByTurnInfo(void)
{
}


KShortestPathLinkBaseByTurnInfo::~KShortestPathLinkBaseByTurnInfo(void)
{
}

void KShortestPathLinkBaseByTurnInfo::ExtractNodeDepartureLink( KLinkCollection& a_colLink, KIDKeyIntegerValueList& a_colNodeDepartureLink )
{
	a_colNodeDepartureLink.RemoveAll();

	std::map<__int64, KLink*>::iterator iter = a_colLink.begin();
	std::map<__int64, KLink*>::iterator end  = a_colLink.end();

	while (iter != end)
	{
		KLink*   pLink       = iter->second;
		KIDList* pLinkIDList = a_colNodeDepartureLink.GetExistOrNew(pLink->GetFromNodeID());

		// 노드를 출발하는 링크 정보 추가
		pLinkIDList->Add(pLink->GetLinkID());
		++iter;
	}
}

void KShortestPathLinkBaseByTurnInfo::PushHeapFromNode2Links( KMinHeapLink& a_fromHeapLink, KLinkCollection& a_colLink,
															  KIDKeyIntegerValueList& a_colNodeDepartureLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
															  KMinHeapLk& rMinHeap, Double a_dAccumulateCost /*= 0.0 */ )
{
	Integer      nxToNodeID         = a_fromHeapLink.GetToNodekID();
	Integer      nxPreLinkID		= a_fromHeapLink.GetLinkID();

// 	CString		 strPrarentPreLink(_T(""));
// 	strPrarentPreLink.Format(_T("%I64d"), nxPreLinkID);
// 	strPrarentPreLink = strPrarentPreLink.Left(strPrarentPreLink.GetLength()-2);
// 	Integer		 nxParentPreLinkID = _ttoi64(strPrarentPreLink);
	Integer		 nxParentPreLinkID = nxPreLinkID/100;


	// 노드를 시점으로 하는 링크검색
	KLinkIDList* pLinkIDList          = a_colNodeDepartureLink.GetAt(nxToNodeID);
	if ( NULL == pLinkIDList )
	{
		return;
	}

	//추가부분
	std::set<Integer> setLinkList;
	KODKey oODKey(nxToNodeID, nxParentPreLinkID);
	std::map<KODKey, std::set<Integer>>::iterator find = a_mapTurnNodeInfo.find(oODKey);
	std::map<KODKey, std::set<Integer>>::iterator fend = a_mapTurnNodeInfo.end();
	if (find == fend)
	{
		return;
	}
	else
	{
		setLinkList = find->second;
	}

	size_t nxLinkCount = 0;
	nxLinkCount = pLinkIDList->GetCount();

	for ( size_t i=0; i<nxLinkCount; i++ )
	{
		Integer nxLinkID = pLinkIDList->GetAt(i);
		KLink*  pLink    = a_colLink.GetLink(nxLinkID);

// 		CString		 strPrarentNextLink(_T(""));
// 		strPrarentNextLink.Format(_T("%I64d"), nxLinkID);
// 		strPrarentNextLink = strPrarentNextLink.Left(strPrarentNextLink.GetLength()-2);
// 		Integer		 nxParentNextLinkID = _ttoi64(strPrarentNextLink);

		Integer		 nxParentNextLinkID = nxLinkID/100;

		std::set<Integer>::iterator findL = setLinkList.find(nxParentNextLinkID);
		std::set<Integer>::iterator endL  = setLinkList.end();
		if (findL != endL)
		{
			if ( NULL != pLink )
			{
				KMinHeapLink heapNode(nxLinkID, pLink->GetFromNodeID(), pLink->GetToNodeID(), nxPreLinkID, a_dAccumulateCost + pLink->GetCost());
				rMinHeap.push(heapNode);
			}        
		}		
	}
}

bool KShortestPathLinkBaseByTurnInfo::IsDestination( std::set<Integer>& a_setDestination, Integer a_nxLinkID )
{
	std::set<Integer>::iterator iter = a_setDestination.find(a_nxLinkID);
	std::set<Integer>::iterator end  = a_setDestination.end();

	if (iter != end)
	{
		return true;
	}

	return false;
}

void KShortestPathLinkBaseByTurnInfo::MuLinkBaseExecute( Integer a_nxOriginLink, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink )
{
#pragma region Argument
	// a_nxOiginNodeID  : 시점노드
	// a_setDestination : 목적지
	// a_colLink        : 링크 정보의 집합
	//                    (Integer m_linkID, Integer m_fromNodeID, Integer m_toNodeID, Double  m_cost - 비용은 외부에서 정의)
	// a_oMinCostLinkCollection : 링크 비용 정보 집합
	//                    (Integer m_linkID, Integer m_preLinkID,  Double  m_cost)
	//                    전체 링크의 최소비용정보, 링크에 도착하는 최소 링크 정보(1:1)를 저장
	// a_oDestinationNodeArrivedMinLink : 목적지에 도착한 링크 중 최소 비용 링크 ID, 비용값을 보관
	//                    (Integer LinkID, Double  Cost)
#pragma endregion Argument
	////^TxLogDebug( _T("Start LinkBase MinPath.. ") );

	size_t nxDestinationCount = a_setDestination.size();
	if (nxDestinationCount < 1)
		return;

	// Step1. Output Initial
#pragma region Output_Initial
	a_oMinCostLinkCollection.RemoveAll();
	a_oDestinationNodeArrivedMinLink.RemoveAll();
#pragma endregion Output_Initial

	// Step2. Input Data Extract
	// 링크 정보로 부터 노드를 출발하는 링크 정보 추출 - <노드1(From) - 링크1, 링크2> <노드2(From) - 링크3, 링크4>
	KIDKeyIntegerValueList    colNodeDepartureLink;  
	ExtractNodeDepartureLink  (a_colLink, colNodeDepartureLink);    

	
#pragma region oMinHeap_First    
	
	// Step3. MinHeap에 시점 정보 추가
	KMinHeapLk oMinHeap;

	//시작 링크에 대한 Heap 정의 : 하지 않으면 무한 루프
	KLink* pLink = a_colLink.GetLink(a_nxOriginLink);
	if (pLink == nullptr)
	{
		return;
	}
	KMinHeapLink                oOriginHeapLink(a_nxOriginLink, pLink->GetFromNodeID(),pLink->GetToNodeID(), 0, 0.0);

	bool       isDestination   = IsDestination                   (a_setDestination, a_nxOriginLink); 
	KCostLink* pMinCostLink    = a_oMinCostLinkCollection.GetLink(a_nxOriginLink);
	if (NULL == pMinCostLink)
	{
		a_oMinCostLinkCollection.InsertOrReplaceLink(a_nxOriginLink, 0, 0);

		PushHeapFromNode2Links ( oOriginHeapLink, a_colLink, colNodeDepartureLink, a_mapTurnNodeInfo, oMinHeap, 0.0 );
		if ( isDestination )
		{
			a_oDestinationNodeArrivedMinLink.SetMinLink (a_nxOriginLink, a_nxOriginLink, 0.0);    
		}
	}
	          
#pragma endregion oMinHeap_First

	//Heap 노드 구성 및 노드에 대한 경로 저장
	while ( ! oMinHeap.empty() /*|| (nxDestinationCount == a_oDestinationNodeArrivedMinLink.GetCount())*/ )
	{
		KMinHeapLink  topHeapNode     =  oMinHeap.top();
		Integer    nxTopNodeID     =  topHeapNode.GetToNodekID();
		Integer    nxTopPreLinkID  =  topHeapNode.GetLinkID();
		Integer    nxTopPPreLinkID =  topHeapNode.GetPreLinkID();
		Double     dTopNodeCost    =  topHeapNode.GetCostValue();
		oMinHeap.pop();

		bool       isDestination   = IsDestination                   (a_setDestination, nxTopPreLinkID);     
		KCostLink* pMinCostLink    = a_oMinCostLinkCollection.GetLink(nxTopPreLinkID);       

		// 최초 방문
		if ( NULL == pMinCostLink) 
		{
			a_oMinCostLinkCollection.InsertOrReplaceLink(nxTopPreLinkID, nxTopPPreLinkID, dTopNodeCost);

			PushHeapFromNode2Links (topHeapNode, a_colLink, colNodeDepartureLink, a_mapTurnNodeInfo, oMinHeap, dTopNodeCost);
			if ( isDestination )
			{
				a_oDestinationNodeArrivedMinLink.SetMinLink (nxTopPreLinkID, nxTopPreLinkID, dTopNodeCost);    
			}
		}
		else /* 방문한 경우 */
		{
			if ( pMinCostLink->GetCost() > dTopNodeCost )
			{
				// 방문링크의 값을 업데이트(비용+이전링크정보)
				pMinCostLink->SetCost     (dTopNodeCost);
				pMinCostLink->SetPreLinkID(nxTopPPreLinkID);

				PushHeapFromNode2Links (topHeapNode, a_colLink, colNodeDepartureLink, a_mapTurnNodeInfo, oMinHeap, dTopNodeCost);
				if ( ! isDestination )
				{
					a_oDestinationNodeArrivedMinLink.SetMinLink (nxTopPreLinkID, nxTopPreLinkID, dTopNodeCost);
				}
			}
		}
	}
}

void KShortestPathLinkBaseByTurnInfo::ExtractPath( Integer a_nxDestinationLinkID, KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink, std::vector<Integer>& ar_vecPathLink )
{
	ar_vecPathLink.clear();

	KMinLinkInfo* pMinLinkInfo = a_oDestinationNodeArrivedMinLink.GetMinLink(a_nxDestinationLinkID);
	if (pMinLinkInfo == nullptr)
	{
		return;
	}

	Integer    nxLinkID  = pMinLinkInfo->LinkID;
	KCostLink* pCostLink = a_oMinCostLinkCollection.GetLink(nxLinkID);

	while(pCostLink != nullptr)
	{		
		ar_vecPathLink.push_back(nxLinkID);		
		nxLinkID = pCostLink->GetPreLinkID();

		pCostLink = a_oMinCostLinkCollection.GetLink(nxLinkID);
	}

	std::reverse(ar_vecPathLink.begin(), ar_vecPathLink.end());
}
