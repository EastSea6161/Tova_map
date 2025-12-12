#pragma once

//forward declare
class KTarget;
class KIOColumn;

class KDBaseDevPlan
{
public:
	KDBaseDevPlan(void);
	~KDBaseDevPlan(void);

public:
	static void   LstPlanInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapPlanInfo);
	
	static double CalcBasicUnit(KTarget* a_pTarget, KIOColumn* a_pPAColumn, KIOColumn* a_pSEDColumn);

	//static void AddPlanInfo(KTarget* a_pTarget, )
};

