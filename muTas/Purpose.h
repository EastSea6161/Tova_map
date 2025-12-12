#pragma once

#include "model_common.h"

class KPurpose
{
public:
	KPurpose();
	KPurpose(KEMPurposeGroup emPurposeGroup, int nPurposeID, LPCTSTR strPurposeName);
	~KPurpose(void);
public :
	KEMPurposeGroup PurposeGroup(void);
	void PurposeGroup(KEMPurposeGroup emGroup);

public:
	int PurposeID(void);
	void PurposeID(int nPurposeID);
public:
	LPCTSTR PurposeName(void);
	void PurposeName(LPCTSTR strPurposeName);
public:
	bool MatchPurposeGroup(int a_nGroupCode, KEMPurposeGroup &a_emPurposeGroup);
protected :
	KEMPurposeGroup m_emPurposeGroup;
	int m_nPurposeID;
	CString m_strPurposeName;
};

struct TPurposeComparator
{
	bool operator () (KPurpose* const& lhs, KPurpose* const& rhs) const
	{
		return lhs->PurposeID() < rhs->PurposeID();
	}
};
