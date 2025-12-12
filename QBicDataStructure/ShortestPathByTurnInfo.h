#pragma once
#include <set>

#include "KMinHeap.h"
#include "KIDKeyIntegerValueList.h"
#include "KLinkCollection.h"
#include "KCostLink.h"
#include "KCostLinkCollection.h"
#include "KNodeArrivedMinLink.h"
#include "KODKey.h"

class KShortestPathByTurnInfo
{
public:
	KShortestPathByTurnInfo(void);
	~KShortestPathByTurnInfo(void);

private:
	static void ExtractNodeDepartureLink (KLinkCollection& a_colLink, KIDKeyIntegerValueList& a_colNodeDepartureLink);
	static void PushHeapFromNode2Links   (
		KHeapNode&             a_fromHeapNode,
		KLinkCollection&       a_colLink,
		KIDKeyIntegerValueList& a_colNodeDepartureLink, 
		std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KMinHeap&              rMinHeap,
		Double                 a_dAccumulateCost = 0.0
		);
	static void PushHeapFromNode2LinksPartial  (
		KHeapNode&             a_fromHeapNode,
		KLinkCollection&       a_colLink,
		KIDKeyIntegerValueList& a_colNodeDepartureLink, 
		std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KMinHeap&              rMinHeap,
		Double                 a_dAccumulateCost = 0.0
		);


	static void FirstPushHeapFromNode2Links   (
		KHeapNode&             a_fromHeapNode,
		KLinkCollection&       a_colLink,
		KIDKeyIntegerValueList& _FNode2LinkCollection, 
		KMinHeap&              rMinHeap,
		Double                 a_dAccumulateCost = 0.0
		);

	static bool IsDestination            ( std::set<Integer>& a_setDestination, Integer a_nxNodeID);

public:
	// 노드기반의 최단경로 알고리즘 (Turn 정보 입력)
	//** 주의사항 KTDB에서 사용되는 알고리즘으로 인하여 LinkID 주의 Turn 정보에는 양방향 ID 데이터에는 일방 ID 
	static void MuLinkBaseExecute (Integer a_nxOiginNodeID, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink );

	//최단경로 가장 가까운 노드 찾기!!
	//** 주의사항 KTDB에서 사용되는 알고리즘으로 인하여 LinkID 주의 Turn 정보에는 양방향 ID 데이터에는 일방 ID 
	static Integer MuLinkBaseExecuteClosedNode(Integer a_nxOiginNodeID, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink);

	//노드 기반 최단경로 알고리즘 (Turn 정보 입력) ** 일부의 Turn 정보만 입력 받음  입력받지 못한 노드에 대해서는 전체 진행 가능
	static void MuLinkBaseExecutepartialTurn(Integer a_nxOiginNodeID, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, std::map<KODKey, std::set<Integer>>& a_mapTurnNodeInfo, 
		KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink);

public:
	static void ExtractPath(Integer a_nxDestinationNodeID, KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink, std::vector<Integer>& ar_vecPathLink);
};

