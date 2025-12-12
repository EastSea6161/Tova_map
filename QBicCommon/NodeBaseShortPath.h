#pragma once
#include "ShortestPathInfo.h"

// 전면개편
class AFX_EXT_CLASS NodeBaseShortPath
{
    class TxCostNode {
    public:
        TxCostNode() {
            NodeID = -1;
            MinCost = 1.0e100;
            DestWhoID = -1;
            WhoID = -1;
            PreMinLink = nullptr;
            XY[0] = 0;
            XY[1] = 0;
        }

        TxCostNode(__int64 nxID) : NodeID(nxID) {
            MinCost = 1.0e100;
            DestWhoID = -1;
            WhoID = -1;
            PreMinLink = nullptr;
            XY[0] = 0;
            XY[1] = 0;
        }

        TxCostNode(__int64 nxID, double dCost) : NodeID(nxID), MinCost(dCost) {
            DestWhoID = -1;
            WhoID = -1;
            PreMinLink = nullptr;
            XY[0] = 0;
            XY[1] = 0;
        }
        virtual ~TxCostNode() {}

        // 비용관련
    public:
        __int64 NodeID;
        double  MinCost;
        TxLink* PreMinLink;
        int WhoID;
    public:
        int DestWhoID;
    public:
        double XY[2];
    public:
        std::set<TxTurnInfo> m_setTurnInfo;
    public:
        std::vector<TxLink>  m_vecOutLink;
    };

public:
	NodeBaseShortPath() {
		m_dNodeGlobalCost = 0;
		NodeBox = nullptr;
		BoxSize = 0;
        WhoID = 0;
        ArrXY[0] = 0;
        ArrXY[1] = 0;
        m_bUseAStar = false;
        m_bUseTurn = false;
	}
	virtual ~NodeBaseShortPath();

private:
	TxCostNode* NodeBox;
	size_t BoxSize;
	std::map<__int64, __int64> NodeIdx;

private:
    std::vector<TxLink*> m_vecLinkBox;
    std::map<__int64, TxNodeXY> m_mapNodeXY;
private:
    void Clear() {
        for (size_t i = 0; i < m_vecLinkBox.size(); i++) {
            TxLink* pLink = m_vecLinkBox[i];
            delete pLink;
        }

        m_vecLinkBox.clear();

        NodeIdx.clear();
        delete[] NodeBox;
        BoxSize = 0;
    }
private:
    int WhoID;
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
    double ArrXY[2];
private:
    inline double HeuristicCost(TxCostNode& oNode) {
        if (m_bUseAStar) {
            double dDist(0.0);
            dDist = sqrt(pow(ArrXY[0] - oNode.XY[0], 2.0) + pow(ArrXY[1] - oNode.XY[1], 2.0));
            return dDist;
        }

        return 0.0;
    }

    inline double HeuristicCost(TxCostNode* pNode) {
        if (m_bUseAStar) {
            double dDist(0.0);
            dDist = sqrt(pow(ArrXY[0] - pNode->XY[0], 2.0) + pow(ArrXY[1] - pNode->XY[1], 2.0));
            return dDist;
        }

        return 0.0;
    }

public:
	void InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork);
    void InitNetWork(std::vector<TxLinkPtr>& vecLinkNetwork, std::map<__int64, TxNodeXY>& mapNodeXY);
	void SetTurnInfo(std::map<__int64, std::set<TxTurnInfo>>& mapTurnInfo /*회전금지*/);

public:
	std::map<__int64, std::vector<__int64>> MinPathN2Nodes(__int64 inDepID, std::set<__int64>& inDest);
    std::vector<__int64> MinPathN2Node(__int64 inNodeDepID, __int64 inNodeArrID);
private:
	void ReverseNodeTracePath(std::set<__int64>& inDest, std::map<__int64, std::vector<__int64>>& mapPath);
};
