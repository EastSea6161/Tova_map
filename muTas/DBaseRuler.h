#pragma once

class KTarget;

struct TRulerInfo
{
	Integer nxLinkID;
	double  dValue;
};

class KDBaseRuler
{
public:
	KDBaseRuler(void);
	~KDBaseRuler(void);

public:
	static void GetRulerInfo(KTarget* a_pTarget, KIOColumn* a_pColumn, std::vector<TRulerInfo>& ar_vecRulerInfo);
	static void GetRulerInfo(KTarget* a_pTarget, KIOColumn* a_pColumn, TRulerInfo& ar_oRulerInfo); 
};

