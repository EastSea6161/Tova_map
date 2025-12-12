#pragma once
#include "ShortestPathInfo.h"

class TxCostLink
{
public:
    TxCostLink() {
        Idx = -1;
        TxOrgID = -1;
        TxToNodeID = -1;
        MinCost = 0;
        LinkCost = 0;
        WhoID = 0;
        DestWhoID = 0;
        XY[0] = 0;
        XY[1] = 0;
        PreMinIdx = -1;
    }
    ~TxCostLink() {
    }

public:
    __int64 TxOrgID;
    __int64 TxToNodeID;
public:
    double  MinCost;
    double  LinkCost;
public:
    int WhoID;
    int DestWhoID;
    int Idx;
    int PreMinIdx;
public:
    double XY[2];
public:
    std::set<int>    TurnInfo;
    std::vector<int> OutLinkIdx;
};

class AFX_EXT_CLASS LinkBaseShortPath
{
public:
    LinkBaseShortPath() {
        LinkBox = nullptr;
        WhoID = 1;
        ArrXY[0] = 0;
        ArrXY[1] = 0;
        m_dNodeGlobalCost = 0;
        m_bUseAStar = true;
        m_bUseTurn = false;
    }

    ~LinkBaseShortPath() {
        Clear();
    }

private:
    int WhoID;
private:
    void Clear() {
        delete[] LinkBox;
        BoxSize = 0;

        LinkIdx.clear();
        m_mapNodeOutLinkIdx.clear();
        m_mapNodeInLinkIdx.clear();
    }
private:
    TxCostLink* LinkBox;
    size_t BoxSize;
    std::map<__int64, int> LinkIdx;

private:
    std::map<__int64, std::vector<int>> m_mapNodeOutLinkIdx;
    std::map<__int64, std::vector<int>> m_mapNodeInLinkIdx;
    std::map<__int64, TxNodeXY> m_mapNodeXY;

private:
    double m_dNodeGlobalCost;
public:
    void SetNodeGlobalCost(double dCost) {
        m_dNodeGlobalCost = dCost;
    }

private:
    bool m_bUseTurn;
private:
    bool m_bUseAStar;
	double DepXY[2];
    double ArrXY[2];
    inline double HeuristicCost(TxCostLink& Link) {
        if (m_bUseAStar) {
            double dDist(0.0);
            dDist = sqrt(pow(ArrXY[0] - Link.XY[0], 2.0) + pow(ArrXY[1] - Link.XY[1], 2.0));
            return dDist;
        }
        
        return 0.0;
    }

    inline double HeuristicCost(TxCostLink* Link) {
        if (m_bUseAStar) {
            double dDist(0.0);
            dDist = sqrt(pow(ArrXY[0] - Link->XY[0], 2.0) + pow(ArrXY[1] - Link->XY[1], 2.0));
            return dDist;
        }

        return 0.0;
    }

	inline bool CheckDistLimit(TxCostLink oLink, double dDistLimit) {

		bool bResult(false);
		double dDist = sqrt(pow(DepXY[0] - oLink.XY[0], 2) + pow(DepXY[1] - oLink.XY[1], 2));
		if (dDist < dDistLimit)
		{
			bResult = true;
		}
		return bResult;
	}

	inline double ChangeCost(TxCostLink& oLink, std::map<int, double>& mapCLinkCost)
	{
		double dCost = oLink.LinkCost;
		auto find = mapCLinkCost.find(oLink.Idx);
		if (find != mapCLinkCost.end())
		{
			dCost = find->second;
		}

		return dCost;
	}
	
public:
	inline double GetLinkCost(__int64 nxLinkID)
	{
		double dLength(-1);
		auto find = LinkIdx.find(nxLinkID);
		if (find == LinkIdx.end())
			return dLength;

		TxCostLink& oLink = LinkBox[find->second];
		return oLink.LinkCost;
	}

public:
    void InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork, std::map<__int64, TxNodeXY>& mapNodeXY);
    void InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork);
public:
    void SetTurnInfo(std::map<__int64, std::set<__int64>>& mapTurnInfo);
public:
    std::vector<__int64> MinPathN2Node(__int64 inNodeDepID, __int64 inNodeArrID);
    std::vector<__int64> MinPathN2Link(__int64 inNodeDepID, __int64 inLinkArrID);
    std::vector<__int64> MinPathL2Link(__int64 inLinkDepID, __int64 inLinkArrID);

	//링크 가중치 변경 및 바운더리 지정 
	//mapCost : 링크ID, 변경할 Cost , dLimitLength :바운더리
	std::vector<__int64> MinPathL2LinkDijkstraChangeCost(__int64 inLinkDepID, __int64 inLinkArrID, std::map<__int64, double>& mapNewCost, double dLimitLength = -1);
	
private:
    void Loop(std::multimap<double, TxCostLink*>& mmapHeap, std::vector<int>& vecArrIdx, std::vector<__int64>& vecPath);
	
	void LoopChangeCost(std::multimap<double, TxCostLink*>& mmapHeap, std::vector<int>& vecArrIdx, std::vector<__int64>& vecPath,
						std::map<int, double>& mapCLinkCost, double dLimitlength = -1, bool bUseAstar = false);

	void ReverseNodeTracePath(std::set<int>& inDestIdx, std::vector<__int64>& vecPath);
    void ReverseNodeTracePath(std::vector<int>& inDestIdx, std::vector<__int64>& vecPath);

//★ 1 : N (노드:노드들)
public:
    std::map<__int64, std::vector<__int64>> MinPathN2Nodes(__int64 inDepID, std::set<__int64>& inDest);
private:
    void LoopNodes(std::multimap<double, TxCostLink*>& mmapHeap, std::set<__int64>& inDestNode, std::map<__int64, std::vector<__int64>>& mapPath);
    void ReverseNodeTracePath(std::set<__int64>& inDestNode, std::map<__int64, std::vector<__int64>>& mapPath);
};