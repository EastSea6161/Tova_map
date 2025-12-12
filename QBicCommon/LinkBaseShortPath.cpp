#include "stdafx.h"
#include "LinkBaseShortPath.h"

void LinkBaseShortPath::InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork, std::map<__int64, TxNodeXY>& mapNodeXY)
{
    Clear();
    m_bUseAStar = true;
    m_bUseTurn = false;

    size_t nxCnt = vecLinkNetwork.size();
    LinkBox = new TxCostLink[nxCnt]; {
        BoxSize = nxCnt;
    }

    for (size_t i = 0; i < nxCnt; i++) {
        TxLinkPtr spLink = vecLinkNetwork[i];
        __int64 nxFNode = spLink->FNode;
        __int64 nxTNode = spLink->TNode;

        if (true) {
            std::vector<int>& vecIdx = m_mapNodeOutLinkIdx[nxFNode];
            vecIdx.push_back((int)i);
        }
        
        std::vector<int>& vecIdx = m_mapNodeInLinkIdx[nxTNode];
        vecIdx.push_back((int)i);
    }

    for (size_t i = 0; i < nxCnt; i++) {
        TxLinkPtr  spLink = vecLinkNetwork[i];
        TxCostLink& oLink = LinkBox[i]; {
            oLink.Idx = (int)i;
            oLink.TxOrgID = spLink->LinkID;
            oLink.LinkCost = spLink->GetCost();

            __int64 nxFNode = spLink->FNode;
            __int64 nxTNode = spLink->TNode;
            auto iFind = mapNodeXY.find(nxTNode);
            if (iFind != mapNodeXY.end()) {
                TxNodeXY& oInfo = iFind->second;
                oLink.XY[0] = oInfo.XY[0];
                oLink.XY[1] = oInfo.XY[1];
            }
            
            oLink.TxToNodeID = nxTNode;
            oLink.OutLinkIdx = m_mapNodeOutLinkIdx[nxTNode];
        }

        LinkIdx.insert(std::make_pair(spLink->LinkID, (int)i));
    }

    m_mapNodeXY = mapNodeXY;
}

void LinkBaseShortPath::InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork)
{
    Clear();
    m_bUseAStar = false;
    m_bUseTurn = false;

    size_t nxCnt = vecLinkNetwork.size();
    LinkBox = new TxCostLink[nxCnt]; {
        BoxSize = nxCnt;
    }

    for (size_t i = 0; i < nxCnt; i++) {
        TxLinkPtr spLink = vecLinkNetwork[i];
        __int64 nxFNode = spLink->FNode;
        __int64 nxTNode = spLink->TNode;

        if (true) {
            std::vector<int>& vecIdx = m_mapNodeOutLinkIdx[nxFNode];
            vecIdx.push_back((int)i);
        }

        std::vector<int>& vecIdx = m_mapNodeInLinkIdx[nxTNode];
        vecIdx.push_back((int)i);
    }

    for (size_t i = 0; i < nxCnt; i++) {
        TxLinkPtr  spLink = vecLinkNetwork[i];
        TxCostLink& oLink = LinkBox[i]; {
            oLink.Idx = (int)i;
            oLink.TxOrgID = spLink->LinkID;
            oLink.LinkCost = spLink->GetCost();

            __int64 nxFNode = spLink->FNode;
            __int64 nxTNode = spLink->TNode;

            oLink.TxToNodeID = nxTNode;
            oLink.OutLinkIdx = m_mapNodeOutLinkIdx[nxTNode];
        }

        LinkIdx.insert(std::make_pair(spLink->LinkID, (int)i));
    }
}

void LinkBaseShortPath::SetTurnInfo(std::map<__int64, std::set<__int64>>& mapTurnInfo)
{
    bool bUseTurn(false);
    for (auto iter = mapTurnInfo.begin(); iter != mapTurnInfo.end(); ++iter) {
        __int64 nxFLink = iter->first;
        std::set<__int64>& setNxtLink = iter->second;

        auto iFind = LinkIdx.find(nxFLink);

        if (iFind != LinkIdx.end()) {
            TxCostLink& oLink = LinkBox[iFind->second];
            std::set<int>& setTurnInfo = oLink.TurnInfo;
            
            for (auto iter = setNxtLink.begin(); iter != setNxtLink.end(); ++iter) {
                auto iFind = LinkIdx.find(*iter);
                if (iFind != LinkIdx.end()) {
                    setTurnInfo.insert(iFind->second);
                    bUseTurn = true;
                }
            }
        }
    }

    m_bUseTurn = bUseTurn;
}

std::vector<__int64> LinkBaseShortPath::MinPathN2Node(__int64 inNodeDepID, __int64 inNodeArrID)
{
    if (inNodeDepID == inNodeArrID) {
        std::vector<__int64> vecPath;
        return vecPath;
    }
    
    WhoID++;
    //★ 출발지 필터링
    std::vector<int> vecDepIdx; {
        auto iFind = m_mapNodeOutLinkIdx.find(inNodeDepID);
        if (iFind != m_mapNodeOutLinkIdx.end()) {
            vecDepIdx = iFind->second;
        }
        else {
            std::vector<__int64> vecPath;
            return vecPath;
        }
    }

    std::vector<int> vecArrIdx; {
        auto iFind = m_mapNodeInLinkIdx.find(inNodeArrID);
        if (iFind != m_mapNodeInLinkIdx.end()) {
            vecArrIdx = iFind->second;
        }
        else {
            std::vector<__int64> vecPath;
            return vecPath;
        }
    }
       
    TxNodeXY& oInfo = m_mapNodeXY[inNodeArrID]; {
        ArrXY[0] = oInfo.XY[0];
        ArrXY[1] = oInfo.XY[1];
    }    
    
    //★ 출발지 링크 설정 및 초기 Heap 설정
    std::multimap<double, TxCostLink*> mmapHeap;
    for (size_t i = 0; i < vecDepIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecDepIdx[i]];
        oLink.WhoID = WhoID;
        oLink.MinCost = oLink.LinkCost;
        oLink.PreMinIdx = -1;

        double dCompMaxCost = HeuristicCost(oLink) + oLink.MinCost;
        mmapHeap.insert(std::make_pair(dCompMaxCost, &oLink));
    }

    //★ 도착지 링크 설정
    for (size_t i = 0; i < vecArrIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecArrIdx[i]];
        oLink.DestWhoID = WhoID;
        oLink.MinCost   = 1.0e100;
        oLink.PreMinIdx = -1;
    }

    std::vector<__int64> vecPath; {
        Loop(mmapHeap, vecArrIdx, vecPath);
    }
    
    return vecPath;
}

std::vector<__int64> LinkBaseShortPath::MinPathN2Link(__int64 inNodeDepID, __int64 inLinkArrID)
{
    WhoID++;
    //★ 출발지 필터링
    std::vector<int> vecDepIdx; {
        auto iFind = m_mapNodeOutLinkIdx.find(inNodeDepID);
        if (iFind != m_mapNodeOutLinkIdx.end()) {
            vecDepIdx = iFind->second;
        }
        else {
            std::vector<__int64> vecPath;
            return vecPath;
        }
    }

    auto iFind = LinkIdx.find(inLinkArrID);
    if (iFind == LinkIdx.end()) {
        std::vector<__int64> vecPath;
        return vecPath;
    }

    int nArrLinkIdx = iFind->second;
    std::vector<int> vecArrIdx; {
        vecArrIdx.push_back(nArrLinkIdx);
    }

    TxCostLink& oDestLink = LinkBox[nArrLinkIdx];
    TxNodeXY& oInfo = m_mapNodeXY[oDestLink.TxToNodeID]; {
        ArrXY[0] = oInfo.XY[0];
        ArrXY[1] = oInfo.XY[1];
    }

    //★ 출발지 링크 설정 및 초기 Heap 설정
    std::multimap<double, TxCostLink*> mmapHeap;
    for (size_t i = 0; i < vecDepIdx.size(); i++) {
        int nDepIdx = vecDepIdx[i];
        TxCostLink& oLink = LinkBox[nDepIdx];
        if (nDepIdx == nArrLinkIdx) { //★ 도착링크의 From Node 필터
            std::vector<__int64> vecPath;
            vecPath.push_back(oLink.TxOrgID);
                return vecPath;
        }
        oLink.WhoID = WhoID;
        oLink.MinCost = oLink.LinkCost;
        oLink.PreMinIdx = -1;

        double dCompMaxCost = HeuristicCost(oLink) + oLink.MinCost;
        mmapHeap.insert(std::make_pair(dCompMaxCost, &oLink));
    }

    //★ 도착지 링크 설정
    for (size_t i = 0; i < vecArrIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecArrIdx[i]];
        oLink.DestWhoID = WhoID;
        oLink.MinCost = 1.0e100;
        oLink.PreMinIdx = -1;
    }

    std::vector<__int64> vecPath; {
        Loop(mmapHeap, vecArrIdx, vecPath);
    }

    return vecPath;
}

std::vector<__int64> LinkBaseShortPath::MinPathL2Link(__int64 inLinkDepID, __int64 inLinkArrID)
{
    if (inLinkDepID == inLinkArrID) {
        std::vector<__int64> vecPath;
        return vecPath;
    }
    
    WhoID++;
    //★ 출발지 필터링
    std::vector<int> vecDepIdx; {
        auto iFind = LinkIdx.find(inLinkDepID);
        if (iFind == LinkIdx.end()) {
            std::vector<__int64> vecPath;
            return vecPath;
        }

        vecDepIdx.push_back(iFind->second);
    }

    auto iFind = LinkIdx.find(inLinkArrID);
    if (iFind == LinkIdx.end()) {
        std::vector<__int64> vecPath;
        return vecPath;
    }

    int nArrLinkIdx = iFind->second;
    std::vector<int> vecArrIdx; {
        vecArrIdx.push_back(nArrLinkIdx);
    }

    TxCostLink& oDestLink = LinkBox[nArrLinkIdx];
    TxNodeXY& oInfo = m_mapNodeXY[oDestLink.TxToNodeID]; {
        ArrXY[0] = oInfo.XY[0];
        ArrXY[1] = oInfo.XY[1];
    }

    //★ 출발지 링크 설정 및 초기 Heap 설정
    std::multimap<double, TxCostLink*> mmapHeap;
    for (size_t i = 0; i < vecDepIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecDepIdx[i]];
        oLink.WhoID = WhoID;
        oLink.MinCost = oLink.LinkCost;
        oLink.PreMinIdx = -1;

        double dCompMaxCost = HeuristicCost(oLink) + oLink.MinCost;
        mmapHeap.insert(std::make_pair(dCompMaxCost, &oLink));
    }

    //★ 도착지 링크 설정
    for (size_t i = 0; i < vecArrIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecArrIdx[i]];
        oLink.DestWhoID = WhoID;
        oLink.MinCost = 1.0e100;
        oLink.PreMinIdx = -1;
    }

    std::vector<__int64> vecPath; {
        Loop(mmapHeap, vecArrIdx, vecPath);
    }

    return vecPath;
}

std::vector<__int64> LinkBaseShortPath::MinPathL2LinkDijkstraChangeCost(__int64 inLinkDepID, __int64 inLinkArrID, std::map<__int64, double>& mapNewCost, double dLimitLength)
{
	//★ 변경될 링크의 인덱스와 값의 데이터로 변경
	std::map<int, double> mapCLinkCost;

	for (auto iterC = mapNewCost.begin(); iterC != mapNewCost.end(); iterC++)
	{
		auto findLink = LinkIdx.find(iterC->first);
		if (findLink != LinkIdx.end())
		{
			mapCLinkCost.insert(std::make_pair(findLink->second, iterC->second));
		}
	}

	WhoID++;
	//★ 출발지 필터링
	std::vector<int> vecDepIdx; {
		auto iFind = LinkIdx.find(inLinkDepID);
		if (iFind == LinkIdx.end()) {
			std::vector<__int64> vecPath;
			return vecPath;
		}

		vecDepIdx.push_back(iFind->second);

		TxCostLink& oDepLink = LinkBox[iFind->second];
		TxNodeXY& oInfo = m_mapNodeXY[oDepLink.TxToNodeID];
		DepXY[0] = oInfo.XY[0];
		DepXY[1] = oInfo.XY[1];
	}

	auto iFind = LinkIdx.find(inLinkArrID);
	if (iFind == LinkIdx.end()) {
		std::vector<__int64> vecPath;
		return vecPath;
	}

	int nArrLinkIdx = iFind->second;
	std::vector<int> vecArrIdx; {
		vecArrIdx.push_back(nArrLinkIdx);
	}

	TxCostLink& oDestLink = LinkBox[nArrLinkIdx];
	TxNodeXY& oInfo = m_mapNodeXY[oDestLink.TxToNodeID]; {
		ArrXY[0] = oInfo.XY[0];
		ArrXY[1] = oInfo.XY[1];
	}

	//★ 출발지 링크 설정 및 초기 Heap 설정
	std::multimap<double, TxCostLink*> mmapHeap;
	for (size_t i = 0; i < vecDepIdx.size(); i++) {
		TxCostLink& oLink = LinkBox[vecDepIdx[i]];
		oLink.WhoID = WhoID;
		oLink.MinCost = ChangeCost(oLink, mapCLinkCost);
		oLink.PreMinIdx = -1;

		double dCompMaxCost = oLink.MinCost;
		mmapHeap.insert(std::make_pair(dCompMaxCost, &oLink));
	}

	//★ 도착지 링크 설정
	for (size_t i = 0; i < vecArrIdx.size(); i++) {
		TxCostLink& oLink = LinkBox[vecArrIdx[i]];
		oLink.DestWhoID = WhoID;
		oLink.MinCost = 1.0e100;
		oLink.PreMinIdx = -1;
	}

	std::vector<__int64> vecPath; {
		LoopChangeCost(mmapHeap, vecArrIdx, vecPath, mapCLinkCost, dLimitLength);
	}

	return vecPath;
}

void LinkBaseShortPath::Loop(std::multimap<double, TxCostLink*>& mmapHeap, std::vector<int>& vecArrIdx, std::vector<__int64>& vecPath)
{
    std::set<int> setDest;
    std::set<int> setRDest;
    for (size_t i = 0; i < vecArrIdx.size(); i++) {
        setRDest.insert(vecArrIdx[i]);
        setDest.insert(vecArrIdx[i]);
    }

    double dFindMaxCost(0); //반복적으로 찾은 것을 방지하기 위해
    for (auto iter = mmapHeap.begin(); iter != mmapHeap.end(); iter = mmapHeap.begin())
    {
        double dHeapCost = iter->first;
        TxCostLink* pFrLink = iter->second; {
            mmapHeap.erase(iter);
        }

        double dHeuCost = HeuristicCost(pFrLink);
        if (dHeapCost > dHeuCost + pFrLink->MinCost) {
            continue;
        }

        bool bUseTurn(m_bUseTurn);
        std::set<int>& setTurnInfo = pFrLink->TurnInfo;
        if (bUseTurn && setTurnInfo.size() == 0) {
            bUseTurn = false;
        }
                
        std::vector<int>& vecToLink = pFrLink->OutLinkIdx;

        for (size_t i = 0; i < vecToLink.size(); i++) {
            int nToIdx = vecToLink[i];
            TxCostLink& oToLink = LinkBox[nToIdx];

            if (bUseTurn && setTurnInfo.find(nToIdx) != setTurnInfo.end()) {
                continue;
            }

            double dCost = pFrLink->MinCost + oToLink.LinkCost + m_dNodeGlobalCost;

            bool bInsertQue = true;

            if (oToLink.WhoID != WhoID) {
                oToLink.MinCost = dCost;
                oToLink.PreMinIdx = pFrLink->Idx;
            }
            else if (oToLink.MinCost > dCost) {
                oToLink.MinCost = dCost;
                oToLink.PreMinIdx = pFrLink->Idx;
            }
            else {
                bInsertQue = false;
            }

            oToLink.WhoID = WhoID;

            if (bInsertQue) {
                double dHeuCost = HeuristicCost(oToLink);
                double dCompMaxCost = dCost + dHeuCost;

                if (oToLink.DestWhoID == WhoID) {
                    if (dFindMaxCost < dCompMaxCost) {
                        dFindMaxCost = dCompMaxCost;
                    }

                    setRDest.erase(nToIdx);
                }

                if (setRDest.size() == 0) {
                    if (dFindMaxCost > (dCompMaxCost * 2)) {
                        mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
                    }
                }
                else {
                    mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
                }
            }
        }
    }

    ReverseNodeTracePath(setDest, vecPath);
}

void LinkBaseShortPath::LoopChangeCost( std::multimap<double, TxCostLink*>& mmapHeap, std::vector<int>& vecArrIdx, std::vector<__int64>& vecPath, 
										std::map<int, double>& mapCLinkCost, double dLimitlength /*= -1*/, bool bUseAstar /*=false*/)
{
	std::set<int> setDest;
	std::set<int> setRDest;
	for (size_t i = 0; i < vecArrIdx.size(); i++) {
		setRDest.insert(vecArrIdx[i]);
		setDest.insert(vecArrIdx[i]);
	}

	double dFindMaxCost(0); //반복적으로 찾은 것을 방지하기 위해
	for (auto iter = mmapHeap.begin(); iter != mmapHeap.end(); iter = mmapHeap.begin())
	{
		double dHeapCost = iter->first;
		TxCostLink* pFrLink = iter->second; {
			mmapHeap.erase(iter);
		}

		double dHeuCost(0.0);
		if(bUseAstar == true)
			HeuristicCost(pFrLink);

		if (dHeapCost > dHeuCost + pFrLink->MinCost) {
			continue;
		}

		bool bUseTurn(m_bUseTurn);
		std::set<int>& setTurnInfo = pFrLink->TurnInfo;
		if (bUseTurn && setTurnInfo.size() == 0) {
			bUseTurn = false;
		}

		std::vector<int>& vecToLink = pFrLink->OutLinkIdx;

		for (size_t i = 0; i < vecToLink.size(); i++) {
			int nToIdx = vecToLink[i];
			TxCostLink& oToLink = LinkBox[nToIdx];

			if (m_mapNodeXY.size() > 0 && dLimitlength > 0)
			{
				if (CheckDistLimit(oToLink, dLimitlength) == false)
				{
					continue;
				}
			}


			if (bUseTurn && setTurnInfo.find(nToIdx) != setTurnInfo.end()) {
				continue;
			}

			double dCost = pFrLink->MinCost + ChangeCost(oToLink, mapCLinkCost) + m_dNodeGlobalCost;

			bool bInsertQue = true;

			if (oToLink.WhoID != WhoID) {
				oToLink.MinCost = dCost;
				oToLink.PreMinIdx = pFrLink->Idx;
			}
			else if (oToLink.MinCost > dCost) {
				oToLink.MinCost = dCost;
				oToLink.PreMinIdx = pFrLink->Idx;
			}
			else {
				bInsertQue = false;
			}

			oToLink.WhoID = WhoID;

			if (bInsertQue) {
				double dHeuCost(0.0);
				if (bUseAstar == true)
					HeuristicCost(pFrLink);

				double dCompMaxCost = dCost + dHeuCost;

				if (oToLink.DestWhoID == WhoID) {
					if (dFindMaxCost < dCompMaxCost) {
						dFindMaxCost = dCompMaxCost;
					}

					setRDest.erase(nToIdx);
				}

				if (setRDest.size() == 0) {
					if (dFindMaxCost > dCompMaxCost) {
						mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
					}
				}
				else {
					mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
				}
			}
		}
	}

	ReverseNodeTracePath(setDest, vecPath);
}

void LinkBaseShortPath::ReverseNodeTracePath(std::set<int>& inDestIdx, std::vector<__int64>& vecPath)
{
    vecPath.clear();
    double dMinCost(9999999999999);
    int nIdx(-1);

    for (auto iter = inDestIdx.begin(); iter != inDestIdx.end(); ++iter) {
        int nDestPreIdx = *iter;
        
        TxCostLink& oLink = LinkBox[nDestPreIdx];
        if (oLink.WhoID != WhoID) {
            continue; //방문하지 않았다.
        }

        double dCost = oLink.MinCost;
        if (dMinCost > dCost) {
            dMinCost = dCost;
            nIdx = oLink.Idx;
        }
    }
        
    while (nIdx != -1) {
        TxCostLink& oLink = LinkBox[nIdx];
        vecPath.push_back(oLink.TxOrgID);
        nIdx = oLink.PreMinIdx;
    }

    std::reverse(vecPath.begin(), vecPath.end());
}

void LinkBaseShortPath::ReverseNodeTracePath(std::vector<int>& inDestIdx, std::vector<__int64>& vecPath)
{
    vecPath.clear();
    double dMinCost(9999999999999);
    int nIdx(-1);

    for (auto iter = inDestIdx.begin(); iter != inDestIdx.end(); ++iter) {
        int nDestPreIdx = *iter;

        TxCostLink& oLink = LinkBox[nDestPreIdx];
        if (oLink.WhoID != WhoID) {
            continue; //방문하지 않았다.
        }

        double dCost = oLink.MinCost;
        if (dMinCost > dCost) {
            dMinCost = dCost;
            nIdx = oLink.Idx;
        }
    }

    while (nIdx != -1) {
        TxCostLink& oLink = LinkBox[nIdx];
        vecPath.push_back(oLink.TxOrgID);
        nIdx = oLink.PreMinIdx;
    }

    std::reverse(vecPath.begin(), vecPath.end());
}

std::map<__int64, std::vector<__int64>> LinkBaseShortPath::MinPathN2Nodes(__int64 inNodeDepID, std::set<__int64>& inDestNode)
{
    WhoID++;

    std::map<__int64, std::vector<__int64>> mapPath;
    //★ 출발지 필터링
    std::vector<int> vecDepIdx; {
        auto iFind = m_mapNodeOutLinkIdx.find(inNodeDepID);
        if (iFind != m_mapNodeOutLinkIdx.end()) {
            vecDepIdx = iFind->second;
        }
        else {      
            return mapPath;
        }
    }
        
    //★ 출발지 링크 설정 및 초기 Heap 설정
    std::multimap<double, TxCostLink*> mmapHeap;
    for (size_t i = 0; i < vecDepIdx.size(); i++) {
        TxCostLink& oLink = LinkBox[vecDepIdx[i]];
        oLink.WhoID = WhoID;
        oLink.MinCost = oLink.LinkCost;
        oLink.PreMinIdx = -1;

        double dCompMaxCost = /*HeuristicCost(oLink) + */oLink.MinCost;
        mmapHeap.insert(std::make_pair(dCompMaxCost, &oLink));
    }
     
    LoopNodes(mmapHeap, inDestNode, mapPath);

    return mapPath;
}

void LinkBaseShortPath::LoopNodes(std::multimap<double, TxCostLink*>& mmapHeap, std::set<__int64>& inDestNode, std::map<__int64, std::vector<__int64>>& mapPath)
{
    std::set<__int64> setDest = inDestNode;

    double dFindMaxCost(0); //반복적으로 찾은 것을 방지하기 위해
    for (auto iter = mmapHeap.begin(); iter != mmapHeap.end(); iter = mmapHeap.begin())
    {
        double dHeapCost = iter->first;
        TxCostLink* pFrLink = iter->second; {
            mmapHeap.erase(iter);
        }

        double dHeuCost(0); //= HeuristicCost(pFrLink);
        if (dHeapCost > dHeuCost + pFrLink->MinCost) {
            continue;
        }

        bool bUseTurn(m_bUseTurn);
        std::set<int>& setTurnInfo = pFrLink->TurnInfo;
        if (bUseTurn && setTurnInfo.size() == 0) {
            bUseTurn = false;
        }

        std::vector<int>& vecToLink = pFrLink->OutLinkIdx;

        for (size_t i = 0; i < vecToLink.size(); i++) {
            int nToIdx = vecToLink[i];
            TxCostLink& oToLink = LinkBox[nToIdx];

            if (bUseTurn && setTurnInfo.find(nToIdx) != setTurnInfo.end()) {
                continue;
            }

            double dCost = pFrLink->MinCost + oToLink.LinkCost + m_dNodeGlobalCost;

            bool bInsertQue = true;

            if (oToLink.WhoID != WhoID) {
                oToLink.MinCost = dCost;
                oToLink.PreMinIdx = pFrLink->Idx;
            }
            else if (oToLink.MinCost > dCost) {
                oToLink.MinCost = dCost;
                oToLink.PreMinIdx = pFrLink->Idx;
            }
            else {
                bInsertQue = false;
            }

            oToLink.WhoID = WhoID;

            if (bInsertQue) {
                double dHeuCost(0);  //= HeuristicCost(oToLink);
                double dCompMaxCost = dCost + dHeuCost;

                auto iFind = setDest.find(oToLink.TxToNodeID);
                if (iFind != setDest.end()) {
                    if (dFindMaxCost < dCompMaxCost) {
                        dFindMaxCost = dCompMaxCost;
                    }

                    setDest.erase(iFind);
                }
                
                if (setDest.size() == 0) {
                    if (dFindMaxCost > dCompMaxCost) {
                        mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
                    }
                }
                else {
                    mmapHeap.insert(std::make_pair(dCompMaxCost, &oToLink));
                }
            }
        }
    }

    ReverseNodeTracePath(inDestNode, mapPath);
}


void LinkBaseShortPath::ReverseNodeTracePath(std::set<__int64>& inDestNode, std::map<__int64, std::vector<__int64>>& mapPath)
{    
    for (auto iter = inDestNode.begin(); iter != inDestNode.end(); ++iter) {
        __int64 nxDest = *iter;
        auto iFind = m_mapNodeInLinkIdx.find(nxDest);
        if (iFind == m_mapNodeInLinkIdx.end()) {
            continue;
        }

        std::vector<int>& vecDestIdx = iFind->second;
        std::vector<__int64>& vecPath = mapPath[nxDest];

        ReverseNodeTracePath(vecDestIdx, vecPath);
    }
}

