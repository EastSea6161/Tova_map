#pragma once
#include "TCoordinate.h"

class TReceiveValue
{
public:
    TReceiveValue()
    {
        m_nxLinkID    = 0;
        m_nxFNodeID   = 0;
        m_nFNodeInOut = 0;
        m_nxTNodeID   = 0;
        m_nTNodeInOut = 0;
    }
    ~TReceiveValue(){}
public:
    Integer m_nxLinkID;
    Integer m_nxFNodeID;
    int     m_nFNodeInOut;
    Integer m_nxTNodeID;
    int     m_nTNodeInOut;
};

class ImChampMapSelectionReceiver
{
public:
	ImChampMapSelectionReceiver()         {}
	virtual ~ImChampMapSelectionReceiver(){}
public:
	virtual void MapSelectionFeedback(Integer) = 0;
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection) {};
	virtual void MapSelectionNoneFeedback() {};
    virtual void MapAnalysisFeedback(std::vector<TReceiveValue> a_vecReceiveValue) {};
};

class ImTasTargetRegionReceiver
{
public:
    ImTasTargetRegionReceiver()          {}
    virtual ~ImTasTargetRegionReceiver() {}

public:
    virtual void TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate) = 0;
};