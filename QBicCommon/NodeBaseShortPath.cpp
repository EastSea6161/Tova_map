#include "stdafx.h"
#include <chrono>
#include "NodeBaseShortPath.h"

// 전면개편
NodeBaseShortPath::~NodeBaseShortPath()
{
    Clear();
}

void NodeBaseShortPath::InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork)
{
    Clear();

    m_bUseAStar = false;

	std::vector<TxCostNode> vecNodeBox;
	std::map<__int64, std::vector<TxLinkPtr>> mapNodeOutLink;

	for (auto iter = vecLinkNetwork.begin(); iter != vecLinkNetwork.end(); ++iter) {
		TxLinkPtr spLink = *iter;
		__int64 nxFNodeID = spLink->FNode;

		std::vector<TxLinkPtr>& vecLink = mapNodeOutLink[nxFNodeID]; {
            vecLink.push_back(spLink);
		}
	}

    // 노드의 실제ID와 인덱스정의
	__int64 nxNodeIdx(0);
	for (auto iter = mapNodeOutLink.begin(); iter != mapNodeOutLink.end(); ++iter) {
		__int64 nxNodeID = iter->first;
		NodeIdx[nxNodeID] = nxNodeIdx;
		nxNodeIdx++;
	}

	std::vector<TxCostNode> vecTailNode;
	__int64 nxFNodeIdx = 0;
	for (auto iter = mapNodeOutLink.begin(); iter != mapNodeOutLink.end(); ++iter) {
        TxCostNode oNode(nxFNodeIdx); {
			std::vector<TxLink>& vecOutLink = oNode.m_vecOutLink;

			std::vector<TxLinkPtr>& vecLink = iter->second;
			for (auto iter = vecLink.begin(); iter != vecLink.end(); ++iter) 
            {
                TxLink oLink = *(*iter); {
                    oLink.FNode = nxFNodeIdx;
                }                
				
				__int64 nxOrgTNode = oLink.TNode;
				
				auto iFind = NodeIdx.find(nxOrgTNode); //네트워크의 종점이 누락될 수 있다.
				if (iFind == NodeIdx.end()) {					
                    TxCostNode oTailNode(nxNodeIdx); {
                        vecTailNode.push_back(oTailNode);
                    }
					
                    NodeIdx[nxOrgTNode] = nxNodeIdx;
                    oLink.TNode = nxNodeIdx;
					nxNodeIdx++;
				}
				else {
                    oLink.TNode = iFind->second;
				}

				vecOutLink.push_back(oLink); // 김군
			}
		}
		
		vecNodeBox.push_back(oNode);
		nxFNodeIdx++;
	}

	for (size_t i = 0; i < vecTailNode.size(); i++) {
		vecNodeBox.push_back(vecTailNode[i]);
	}
		
	NodeBox = new TxCostNode[vecNodeBox.size()];

	for (size_t i = 0; i < vecNodeBox.size(); i++) {
		NodeBox[i] = vecNodeBox[i];
	}

	BoxSize = vecNodeBox.size();
}

void NodeBaseShortPath::InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork, std::map<__int64, TxNodeXY>& mapNodeXY)
{
    Clear();

    m_bUseAStar = true;

    std::vector<TxCostNode> vecNodeBox;
    std::map<__int64, std::vector<TxLinkPtr>> mapNodeOutLink;

    for (auto iter = vecLinkNetwork.begin(); iter != vecLinkNetwork.end(); ++iter) {
        TxLinkPtr spLink = *iter;
        __int64 nxFNodeID = spLink->FNode;

        std::vector<TxLinkPtr>& vecLink = mapNodeOutLink[nxFNodeID]; {
            vecLink.push_back(spLink);
        }
    }

    // 노드의 실제ID와 인덱스정의
    __int64 nxNodeIdx(0);
    for (auto iter = mapNodeOutLink.begin(); iter != mapNodeOutLink.end(); ++iter) {
        __int64 nxNodeID = iter->first;
        NodeIdx[nxNodeID] = nxNodeIdx;
        nxNodeIdx++;
    }

    std::vector<TxCostNode> vecTailNode;
    __int64 nxFNodeIdx = 0;
    for (auto iter = mapNodeOutLink.begin(); iter != mapNodeOutLink.end(); ++iter) {
        __int64 nxNodeID = iter->first;
        TxCostNode oNode(nxFNodeIdx);         
        {
            TxNodeXY& oInfo = mapNodeXY[nxNodeID]; {
                oNode.XY[0] = oInfo.XY[0];
                oNode.XY[1] = oInfo.XY[1];
            }            

            std::vector<TxLink>& vecOutLink = oNode.m_vecOutLink;
            std::vector<TxLinkPtr>& vecLink = iter->second;
            for (auto iter = vecLink.begin(); iter != vecLink.end(); ++iter)
            {
                TxLink oLink = *(*iter); {
                    oLink.FNode = nxFNodeIdx;
                }

                __int64 nxOrgTNode = oLink.TNode;

                auto iFind = NodeIdx.find(nxOrgTNode); //네트워크의 종점이 누락될 수 있다.
                if (iFind == NodeIdx.end()) {
                    TxCostNode oTailNode(nxNodeIdx); {
                        TxNodeXY& oInfo = mapNodeXY[nxOrgTNode]; {
                            oTailNode.XY[0] = oInfo.XY[0];
                            oTailNode.XY[1] = oInfo.XY[1];
                        }

                        vecTailNode.push_back(oTailNode);
                    }

                    NodeIdx[nxOrgTNode] = nxNodeIdx;
                    oLink.TNode = nxNodeIdx;
                    nxNodeIdx++;
                }
                else {
                    oLink.TNode = iFind->second;
                }

                vecOutLink.push_back(oLink); // 김군
            }
        }

        vecNodeBox.push_back(oNode);
        nxFNodeIdx++;
    }

    for (size_t i = 0; i < vecTailNode.size(); i++) {
        vecNodeBox.push_back(vecTailNode[i]);
    }

    NodeBox = new TxCostNode[vecNodeBox.size()];

    for (size_t i = 0; i < vecNodeBox.size(); i++) {
        NodeBox[i] = vecNodeBox[i];
    }

    BoxSize = vecNodeBox.size();
    m_mapNodeXY = mapNodeXY;
}

void NodeBaseShortPath::SetTurnInfo(std::map<__int64, std::set<TxTurnInfo>>& mapTurnInfo)
{
    bool bUseTurn(false);
    for (auto iter = mapTurnInfo.begin(); iter != mapTurnInfo.end(); ++iter) {
        __int64 nxID = iter->first;
        auto iFind = NodeIdx.find(nxID);
        if (iFind != NodeIdx.end()) {
            TxCostNode& oNode = NodeBox[iFind->second]; {
                oNode.m_setTurnInfo = iter->second;
            }

            bUseTurn = true;
        }
    }

    m_bUseTurn = bUseTurn;
}


std::map<__int64, std::vector<__int64>> NodeBaseShortPath::MinPathN2Nodes(__int64 inDepID, std::set<__int64>& inDest)
{
    WhoID++;

    //★ 출발지 필터링
	__int64 nxDepID(-1); {
		auto iFind = NodeIdx.find(inDepID);
		if (iFind != NodeIdx.end()) {
			nxDepID = iFind->second;
		}
		else {
			std::map<__int64, std::vector<__int64>> mapPath;
			return mapPath;
		}
	}

	// 내부 인덱스로 변경(목적지)
	std::set<__int64> setDest;
	for (auto iter = inDest.begin(); iter != inDest.end(); ++iter) {
		auto iFind = NodeIdx.find(*iter);
		if (iFind != NodeIdx.end()) {
			__int64 nxID = iFind->second;
            
            if (nxDepID == nxID) {
                continue;
            }

			setDest.insert(nxID);
			TxCostNode& oNode = NodeBox[nxID]; {
                oNode.DestWhoID = WhoID;
                oNode.MinCost = 1.0e100;
                oNode.PreMinLink = nullptr;
			}
		}
	}

    //★ 도착지 필터링
    if (setDest.size() == 0) {
        std::map<__int64, std::vector<__int64>> mapPath;
        return mapPath;
    }

    //★ 출발노드 설정 
    TxLink oTempLink(-1, -1, nxDepID, 0);
    TxCostNode& oDepNode = NodeBox[nxDepID]; {
        oDepNode.WhoID = WhoID;		
        oDepNode.MinCost = 0;//oDepNode.HeuCost;
        oDepNode.PreMinLink = &oTempLink;
	}

	// 초기 Heap 설정
	std::multimap<double, TxCostNode*> mmapHeap; {
		mmapHeap.insert(std::make_pair(oDepNode.MinCost, &oDepNode));
	}

	double dFindMaxCost(0); //반복적으로 찾은 것을 방지하기 위해
	for (auto iter = mmapHeap.begin(); iter != mmapHeap.end(); iter = mmapHeap.begin())
	{
        double dHeapCost = iter->first;
        TxCostNode* pFrNode = iter->second; {
			mmapHeap.erase(iter);
		}
        
        if (dHeapCost > /*pFrNode->HeuCost + */pFrNode->MinCost) {
            continue;
        }
						
		__int64 nxFrNodeID    = pFrNode->NodeID;
		double  dFromNodeCost = pFrNode->MinCost;
					
        bool bUseTurn(m_bUseTurn);
        std::set<TxTurnInfo>& setTurnInfo = pFrNode->m_setTurnInfo;
        if (bUseTurn && setTurnInfo.size() == 0) {
            bUseTurn = false;
        }

        TxLink* pPreLink = pFrNode->PreMinLink;
		std::vector<TxLink>& vecToLink = pFrNode->m_vecOutLink;

		for (auto iter = vecToLink.begin(); iter != vecToLink.end(); ++iter) {
			TxLink &oToLink = *iter;

            if (bUseTurn) {
                __int64 nxFrPreLinkID(pPreLink->LinkID);
                __int64 nxToLinkID = oToLink.LinkID;

                if (setTurnInfo.find(TxTurnInfo(nxFrPreLinkID, nxToLinkID)) != setTurnInfo.end()) {
                    continue;
                }
            }
            __int64 nxToNodeID = oToLink.TNode;
            
            double dToNodeCost = 0; {
				dToNodeCost = dFromNodeCost + oToLink.GetCost() + m_dNodeGlobalCost;
			}

			bool bInsertQue = true;
			TxCostNode& oTNode = NodeBox[nxToNodeID];
			
            if (oTNode.WhoID != WhoID) {
                oTNode.MinCost = dToNodeCost;
                oTNode.PreMinLink = &oToLink;
			}
            else if (oTNode.MinCost > dToNodeCost) {
                oTNode.MinCost = dToNodeCost;
                oTNode.PreMinLink = &oToLink;
            }
			else {
				bInsertQue = false;
			}
            oTNode.WhoID = WhoID;

			if (bInsertQue) {
				//double dHeuCost(oTNode.HeuCost);
                double dCompMaxCost = dToNodeCost; // +dHeuCost;
				
				if (oTNode.DestWhoID == WhoID) {
					if (dFindMaxCost < dCompMaxCost) {
						dFindMaxCost = dCompMaxCost;
					}

					setDest.erase(oTNode.NodeID);
				}

				if (setDest.size() == 0) {
					if (dFindMaxCost > dCompMaxCost) {
						mmapHeap.insert(std::make_pair(dCompMaxCost, &oTNode));
					}
				}
				else {
					mmapHeap.insert(std::make_pair(dCompMaxCost, &oTNode));
				}
			}		
		}
	}

    std::map<__int64, std::vector<__int64>> mapPath; {
        ReverseNodeTracePath(inDest, mapPath);
    }
    
	return mapPath;
}

std::vector<__int64> NodeBaseShortPath::MinPathN2Node(__int64 inDepID, __int64 inArrID)
{
    std::vector<__int64> vecPath;
    
    if (inDepID == inArrID) {
        return vecPath;
    }
        
    WhoID++;

    //★ 출발지 필터링
    __int64 nxDepID(-1); {
        auto iFind = NodeIdx.find(inDepID);
        if (iFind != NodeIdx.end()) {
            nxDepID = iFind->second;
        }
        else {
            return vecPath;
        }
    }

    //★ 도착지 필터링
    __int64 nxArrID(-1); {
        auto iFind = NodeIdx.find(inArrID);
        if (iFind != NodeIdx.end()) {
            nxArrID = iFind->second;
        }
        else {
            return vecPath;
        }
    }

    TxNodeXY& oInfo = m_mapNodeXY[inArrID]; {
        ArrXY[0] = oInfo.XY[0];
        ArrXY[1] = oInfo.XY[1];
    }

    //★ 출발노드 설정 
    TxLink oTempLink(-1, -1, nxDepID, 0);
    TxCostNode& oDepNode = NodeBox[nxDepID]; {
        oDepNode.WhoID = WhoID;
        oDepNode.MinCost = 0;//oDepNode.HeuCost;
        oDepNode.PreMinLink = &oTempLink;
    }

    //★ 도착노드 설정
    TxCostNode& oArrNode = NodeBox[nxArrID]; {
        oArrNode.DestWhoID = WhoID;
        oArrNode.MinCost = 1.0e100;
        oArrNode.PreMinLink = nullptr;
    }

    // 초기 Heap 설정
    std::multimap<double, TxCostNode*> mmapHeap; {
        mmapHeap.insert(std::make_pair(oDepNode.MinCost, &oDepNode));
    }

    double dFindMaxCost(0); //반복적으로 찾은 것을 방지하기 위해
    bool bFindDest(false);
    for (auto iter = mmapHeap.begin(); iter != mmapHeap.end(); iter = mmapHeap.begin())
    {
        double dHeapCost = iter->first;
        TxCostNode* pFrNode = iter->second; {
            mmapHeap.erase(iter);
        }
        
        if (dHeapCost > HeuristicCost(pFrNode) + pFrNode->MinCost) {
            continue;
        }

        __int64 nxFrNodeID = pFrNode->NodeID;
        double  dFromNodeCost = pFrNode->MinCost;

        bool bUseTurn(m_bUseTurn);
        std::set<TxTurnInfo>& setTurnInfo = pFrNode->m_setTurnInfo;
        if (bUseTurn && setTurnInfo.size() == 0) {
            bUseTurn = false;
        }

        TxLink* pPreLink = pFrNode->PreMinLink;
        std::vector<TxLink>& vecToLink = pFrNode->m_vecOutLink;

        for (auto iter = vecToLink.begin(); iter != vecToLink.end(); ++iter) {
            TxLink &oToLink = *iter;

            if (bUseTurn) {
                __int64 nxFrPreLinkID(pPreLink->LinkID);
                __int64 nxToLinkID = oToLink.LinkID;

                if (setTurnInfo.find(TxTurnInfo(nxFrPreLinkID, nxToLinkID)) != setTurnInfo.end()) {
                    continue;
                }
            }
            __int64 nxToNodeID = oToLink.TNode;

            double dToNodeCost = 0; {
                dToNodeCost = dFromNodeCost + oToLink.GetCost() + m_dNodeGlobalCost;
            }

            bool bInsertQue = true;
            TxCostNode& oTNode = NodeBox[nxToNodeID];

            if (oTNode.WhoID != WhoID) {
                oTNode.MinCost = dToNodeCost;
                oTNode.PreMinLink = &oToLink;
            }
            else if (oTNode.MinCost > dToNodeCost) {
                oTNode.MinCost = dToNodeCost;
                oTNode.PreMinLink = &oToLink;
            }
            else {
                bInsertQue = false;
            }
            oTNode.WhoID = WhoID;

            if (bInsertQue) {
                double dHeuCost(HeuristicCost(oTNode));
                double dCompMaxCost = dToNodeCost + dHeuCost;

                if (oTNode.DestWhoID == WhoID) {
                    if (dFindMaxCost < dCompMaxCost) {
                        dFindMaxCost = dCompMaxCost;
                    }

                    bFindDest = true;
                }

                if (bFindDest) {
                    if (dFindMaxCost > dCompMaxCost) {
                        mmapHeap.insert(std::make_pair(dCompMaxCost, &oTNode));
                    }
                }
                else {
                    mmapHeap.insert(std::make_pair(dCompMaxCost, &oTNode));
                }                
            }
        }
    }

    TxLink* pLink = oArrNode.PreMinLink; {
        if (pLink == nullptr) {// 도착 정보가 없는 경우
            return vecPath;
        }
    }

    while (true) {
        __int64 nxPreLinkID = pLink->LinkID;
        if (nxPreLinkID == -1) {
            break;
        }

        vecPath.push_back(nxPreLinkID);
        pLink = NodeBox[pLink->FNode].PreMinLink;
    }

    std::reverse(vecPath.begin(), vecPath.end());

    return vecPath;
}

void NodeBaseShortPath::ReverseNodeTracePath(std::set<__int64>& inDest, std::map<__int64, std::vector<__int64>>& mapPath)
{
    mapPath.clear();

    for (auto iter = inDest.begin(); iter != inDest.end(); ++iter) {
        __int64 nxDestID = *iter;
        auto iFind = NodeIdx.find(nxDestID);
        if (iFind == NodeIdx.end()) {
            continue;
        }

        TxCostNode& oNode = NodeBox[iFind->second];
        TxLink* pLink = oNode.PreMinLink; {
            if (pLink == nullptr) {// 도착 정보가 없는 경우
                continue;
            }
        }        

        std::vector<__int64> vecReversePath;
        while (true) {
            __int64 nxPreLinkID = pLink->LinkID;
            if (nxPreLinkID == -1) {
                break;
            }

            vecReversePath.push_back(nxPreLinkID);
            pLink = NodeBox[pLink->FNode].PreMinLink;
        }

        std::reverse(vecReversePath.begin(), vecReversePath.end());
        mapPath.insert(std::make_pair(nxDestID, vecReversePath));
    }
}




