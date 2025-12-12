#pragma once

class AFX_EXT_CLASS TxNodeXY
{
public:
    TxNodeXY(double X=0, double Y=0) {
        XY[0] = X;
        XY[1] = Y;
    }
    ~TxNodeXY() {}
public:
    double XY[2];
};

class AFX_EXT_CLASS TxTurnInfo {
public:
    TxTurnInfo(__int64 nxPreLinkID, __int64 nxNxtLinkID) : PreLinkID(nxPreLinkID), NxtLinkID(nxNxtLinkID) {}
    ~TxTurnInfo() {}
public:
    __int64 PreLinkID = -1;
    __int64 NxtLinkID = -1;

public:
    bool operator <  (const TxTurnInfo& right) const {
        if (PreLinkID == right.PreLinkID) {
            return NxtLinkID < right.NxtLinkID;
        }

        return PreLinkID < right.PreLinkID;
    }
};

class AFX_EXT_CLASS TxLink {
public:
    TxLink(__int64 nxID, __int64 fromID, __int64 toID, double dCost = 0) : LinkID(nxID), FNode(fromID), TNode(toID), m_dCost(dCost) {
    }

    TxLink(const TxLink& right) {
        LinkID = right.LinkID;
        FNode = right.FNode;
        TNode = right.TNode;
        m_dCost = right.m_dCost;
    }

    TxLink(const std::shared_ptr<TxLink>& right) {
        LinkID = right->LinkID;
        FNode = right->FNode;
        TNode = right->TNode;
        m_dCost = right->m_dCost;
    }

    virtual ~TxLink() {}
public:
    __int64 LinkID;
    __int64 FNode;
    __int64 TNode;

protected:
    double m_dCost;
public:
    virtual double GetCost() {
        // 비용은 항상(반드시) 양수이어야 한다.
        return m_dCost;
    }

    virtual void SetCost(double dCost) {
        m_dCost = dCost;
    }

public:
    TxLink& operator = (const TxLink& right) {
        if (&right == this) {
            return *this;
        }

        LinkID = right.LinkID;
        FNode = right.FNode;
        TNode = right.TNode;
        m_dCost = right.m_dCost;

        return *this;
    }

    bool operator <  (const TxLink& right) const {
        return LinkID < right.LinkID;
    }
};
typedef std::shared_ptr<TxLink> TxLinkPtr;