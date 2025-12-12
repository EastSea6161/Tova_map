#pragma once

#include "model_common.h"

class KMode
{
public:
	KMode();
	KMode(KEMModeGroup emGroup, int nModeID, LPCTSTR strModeName);
	virtual ~KMode(void);

public :
	KEMModeGroup ModeGroup(void);
	void ModeGroup(KEMModeGroup emGroup);
public:
	int ModeID(void);
	void ModeID(int nID);
public:
	LPCTSTR ModeName(void);
	void ModeName(LPCTSTR strName);
public:
	bool MatchModeGroup(int a_nModeCode, KEMModeGroup &a_emModeGroup);

protected :
	KEMModeGroup m_emModeGroup;
	int     m_nID;
	CString m_strName;
};

struct TModeComparator
{
	bool operator () (KMode* const& lhs, KMode* const& rhs) const
	{
		return lhs->ModeID() < rhs->ModeID();
	}
};

class KModeFreight : public KMode
{
public:
    KModeFreight () {}
    virtual ~KModeFreight() {}
    KModeFreight (int a_nID, CString a_strName)
    {
        m_nID     = a_nID;
        m_strName = a_strName;
    }
public:
    void SetModeInfo(int a_nID, CString a_strName)
    {
        m_nID     = a_nID;
        m_strName = a_strName;
    }
};