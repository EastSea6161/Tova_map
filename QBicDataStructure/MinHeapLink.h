#pragma once

#include <queue>
#include <vector>
#include "QBicDataStructureDefine.h"

#define KMinHeapLk  std::priority_queue <KMinHeapLink, std::vector<KMinHeapLink>, KHeaplinkCompare >
#define MaxCost   999999999.0

class KMinHeapLink
{
public:
	KMinHeapLink(void);
	KMinHeapLink(Integer _linkID);
	KMinHeapLink(Integer _linkID, Integer _fromNodeID, Integer _toNodeID, Integer _preLinkID, Double _val);
	~KMinHeapLink(void);

public:
	KMinHeapLink(const KMinHeapLink& _link); 
	KMinHeapLink& operator = (const KMinHeapLink& _link);

private:
	Integer m_linkID;
	Integer m_fromNodeID;
	Integer m_toNodeID;
	Integer m_preLinkID;
	Double  m_cost;


public:
	void    Replacelink(const KMinHeapLink& _link);
	void    SetLinkID(Integer _id);
	Integer GetLinkID() const;

	void    SetFromNodeID(Integer _id);
	Integer SetFromNodeID() const;

	void    SetToNodekID(Integer _id);
	Integer GetToNodekID() const;

	void    SetPreLinkID(Integer _id);
	Integer GetPreLinkID() const;

	void    SetCostValue(Double _val);
	void    AddCostValue(Double _val);
	Double  GetCostValue() const;
};

class KHeaplinkCompare
{
public:
	bool operator() (const KMinHeapLink& _link1, const KMinHeapLink& _link2)
	{
		return _link1.GetCostValue() > _link2.GetCostValue();
	}
};

