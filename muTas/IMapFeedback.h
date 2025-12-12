#pragma once

class IMapNodeSearch
{
public:
    IMapNodeSearch(void) {};
    virtual ~IMapNodeSearch(void) {};

public:
    virtual void ReceivedNodeIDFromMap(CString a_strNodeID) = 0;
};


class IMapZoning
{
public:
    IMapZoning(void) {};
    virtual ~IMapZoning(void) {};

public:
    virtual void ReceivedZoneInfoFromMap(TxPointPtr a_spNodeGeometry, TxPolygonPtr a_spDemarcationGeometry) = 0;
};


class IMapFeedbackLinkSelect
{
public:
	IMapFeedbackLinkSelect(void) {};
	virtual ~IMapFeedbackLinkSelect(void) {};

public:
	virtual void FeedbackFromMap(std::vector<Integer> a_vecLink) = 0;
};