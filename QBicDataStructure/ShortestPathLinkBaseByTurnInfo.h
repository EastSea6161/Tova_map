#pragma once
#include <set>
#include "QBicDataStructureDefine.h"
#include "KIDKeyIntegerValueList.h"
#include "KLinkCollection.h"
#include "KCostLink.h"
#include "KCostLinkCollection.h"
#include "KNodeArrivedMinLink.h"
#include "MinHeapLink.h"
#include "KODKey.h"

class KShortestPathLinkBaseByTurnInfo
{
public:
	KShortestPathLinkBaseByTurnInfo(void);
	~KShortestPathLinkBaseByTurnInfo(void);

	
private:
	static void ExtractNodeDepartureLink (KLinkCollection& a_colLink, KIDKeyIntegerValueList& a_colNodeDepartureLink);
	static void PushHeapFromNode2Links   (
		KMinHeapLink&							a_fromHeapLink,
		KLinkCollection&						a_colLink,
		KIDKeyIntegerValueList&					a_colNodeDepartureLink, 
		std::map<KODKey, std::set<Integer>>&	a_mapTurnNodeInfo, 
		KMinHeapLk&								rMinHeap,
		Double									a_dAccumulateCost = 0.0
		);

	//Link 중 도착 링크
	static bool IsDestination            ( std::set<Integer>& a_setDestination, Integer a_nxLinkID);

public:
	// 링크기반의 최단경로 알고리즘  : Destination은 링크 기반으로 구성
	static void MuLinkBaseExecute (Integer a_nxOriginLink, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink );

public:
	static void ExtractPath(Integer a_nxDestinationLinkID, KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink, std::vector<Integer>& ar_vecPathLink);


};

