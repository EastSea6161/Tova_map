#pragma once
#include <set>
#include "QBicDataStructureDefine.h"
#include "KMinHeap.h"
#include "KIDKeyIntegerValueList.h"
#include "KLinkCollection.h"
#include "KCostLink.h"
#include "KCostLinkCollection.h"
#include "KNodeArrivedMinLink.h"
//#include "Klog.h"

enum SearchResult
{
    Search_Zero  = 0, 
    Search_One   = 1,
    Search_All   = 2
};    

class KShortestPath
{
public:
    KShortestPath (void);
    ~KShortestPath(void);

private:
    static void ExtractNodeDepartureLink (KLinkCollection& _links, KIDKeyIntegerValueList& rFNode2LinkCollection);
    static void PushHeapFromNode2Links   (
                                   KHeapNode&             _fromNode,
                                   KLinkCollection&       _links,
                                   KIDKeyIntegerValueList& _FNode2LinkCollection, 
                                   KMinHeap&              rMinHeap,
                                   Double                 accumulateCost = 0.0
                                  );

    static bool IsDestination            ( std::set<Integer>& a_setDestination, Integer a_nxNodeID);

public:
    // 링크기반의 최단경로 알고리즘 
    static void MuLinkBaseExecute (Integer a_nxOiginNodeID, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink,
                            KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink );

	//
	static Integer MuLinkBaseExecuteClosedNode(Integer a_nxOiginNodeID, std::set<Integer>& a_setDestination, KLinkCollection& a_colLink, 
		KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink);

public:
	static void ExtractPath(Integer a_nxDestinationNodeID, KCostLinkCollection& a_oMinCostLinkCollection, KNodeArrivedMinLink& a_oDestinationNodeArrivedMinLink, std::vector<Integer>& ar_vecPathLink);
};

